from math import ceil
from os import scandir
from os.path import join
import subprocess
import sys
import os
from statistics import mean

def main():

    # Input Arguments
    # -e: Executable path
    # -r: Directory with the reference files
    # -it: Interactive mode (allows to input text in the terminal and use it as target)
    # -t: Target File 
    # -w: Window Size
    # -O: Optimization level (1-3)
    # -o: Window Overlap (0-1)

    input_arguments = InputArguments()

    input_arguments.parse_arguments(sys.argv)
    input_arguments.check_arguments()

    target_file_path = input_arguments.target_file_path

    if input_arguments.interactive_mode:
        
        target_file_path = join(os.path.dirname(input_arguments.target_file_path), 'tempTarget.txt')

        print('[!] Interactive Mode')
        print('[!] Please Insert Text: ')
        user_input = input()
        
        with open(target_file_path, 'w') as targetFile:
            targetFile.write(user_input)

    # Test Several References to get the one with smallest Amount of Information
    information_per_file = {}

    with scandir(input_arguments.references_directory) as entries:

        for entry in entries:  
            
            print(f'Testing Language: {remove_file_extension(entry.name).upper()}     ', end='\r')
            
            output_path = f'../output/lang_output_{remove_file_extension(entry.name)}.txt'
            
            lang_window_size = input_arguments.lang_window_size
            use_finite_context = input_arguments.use_finite_context

            if os.path.exists(output_path):
                os.remove(output_path)

            execute_lang(input_arguments.executable, entry, target_file_path, output_path, lang_window_size, use_finite_context)

            with open(output_path, 'r') as outputFile:

                lines = outputFile.readlines()
                information_per_file[entry.name] = read_information(lines)


    history = identify_language(information_per_file, input_arguments.window_size, input_arguments.optimization, input_arguments.window_overlap)
    obtained_results = obtain_results(history)

    # Evaluate model if it is not in interactive mode
    if not input_arguments.interactive_mode:

        target_file_name = os.path.basename(input_arguments.target_file_path)
        target_dir_name = os.path.dirname(input_arguments.target_file_path)

        exact_results_path = join(join(target_dir_name, 'results'), target_file_name)

        expected_results = obtain_expected_results(exact_results_path)
        obtained_results = obtain_results(history)
        
        precision = evaluate_model(expected_results, obtained_results)

    with open(target_file_path, 'r') as target_file:

        txt = target_file.read()
        print_results(txt, obtained_results, precision)


def execute_lang(executable_path: str, reference_path: str, target_path: str, output_path: str, window_size: int, use_finite_context: bool):

    if use_finite_context:
        subprocess.run([executable_path, '-r', reference_path, '-i', target_path, '-o', output_path, '-k', str(window_size)], 
                stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    else:
        subprocess.run([executable_path, '-r', reference_path, '-i', target_path, '-o', output_path, '-k', str(window_size), '-nFC'], 
                stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)


def remove_file_extension(file_name: str) -> str:
    return file_name.split('.')[0]


def read_information(file_lines: list[str]) -> list[float]:

    lines_with_information = file_lines[5:]
    information_list = []

    for lineIdx in range(len(lines_with_information)):

        line = lines_with_information[lineIdx]
        line_splited = line.split(':')

        if len(line_splited) != 2:
            print(f'Unable to parse line at {lineIdx}')
            return []
        
        information_list.append(( int(line_splited[0]), float(line_splited[1]), ))

    return information_list
     

def obtain_languages_window(languages_info: dict, window_size: int, window_overlap: float):

    # languages_info_list = {key : list(map(lambda x: x[1], value)) for key, value in language_info.items() }

    window_start_idx = 0
    window_last_idx = window_start_idx + window_size

    languages_detection_history = []

    # Language that was currently detected
    globaly_accepted_language = (None, float('inf'), None)

    last_index_registered = 0

    languages_counts = {}

    while True: 
        
        # [0]: Lang name, [1]: Lang weight, [2]: Target Index
        locally_detected_language = (None, float('inf'), None)
        finalized_languages = 0

        for current_lang in languages_info.keys():
        
            current_lang_info_list = list(map(lambda item: item[1], languages_info[current_lang]))

            if len(current_lang_info_list) > window_last_idx:
                current_lang_window_weight = calc_window_weight(current_lang_info_list[window_start_idx : window_last_idx])
            else:
                # The language has no more information 
                current_lang_window_weight = calc_window_weight(current_lang_info_list[window_start_idx : len(current_lang_info_list)])
                finalized_languages += 1

            if current_lang_window_weight < locally_detected_language[1] and current_lang_window_weight < 2:
                # Get the index of the character in the target file
                window_final_target_idx = languages_info[current_lang][min(window_last_idx, len(current_lang_info_list)-1)][0]
                locally_detected_language = (current_lang, current_lang_window_weight, f'{last_index_registered}-{window_final_target_idx}')

        # If we have detected a different language
        if locally_detected_language[0] is not None:
            
            if locally_detected_language[0] in languages_counts:
                languages_counts[locally_detected_language[0]] += 1
            else:
                languages_counts[locally_detected_language[0]] = 1
            
            globaly_accepted_language = (locally_detected_language[0], locally_detected_language[1], f'{last_index_registered}-{window_final_target_idx}')
            last_index_registered = window_final_target_idx
            languages_detection_history.append(globaly_accepted_language)

        # We have analyzed all languages
        if finalized_languages == len(languages_info.keys()):
            break

        # Update the Window for Next Iteration
        window_start_idx += max(int(int(window_size)*window_overlap), 1)
        window_last_idx = window_start_idx + window_size

    languages_counts = dict(sorted(languages_counts.items(), key=lambda x: x[1], reverse=True))
    return languages_detection_history, languages_counts


def identify_language(information_per_file: str, window_size: int, optimization_cycles: int, window_overlap: float):

    languages_to_exclude = {}

    if not optimization_cycles: # Not Optimizing
        print('[!] Not Optimizing')
        return obtain_languages_window(information_per_file, window_size, window_overlap)[0]
    else:
        
        print('[!] Optimizing')
        
        files_info = information_per_file

        for i in range(optimization_cycles+1):
            
            files_info = dict(filter(lambda x: x[0] not in languages_to_exclude, files_info.items()))

            history, languages = obtain_languages_window(files_info, window_size, window_overlap)
            
            # optimization_threshold = ceil(mean([lang_count for lang_count in languages.values()]) / (4-optimization_cycles))
            optimization_threshold = ceil(mean([lang_count for lang_count in languages.values()]) / (len(languages.values())+1) * optimization_cycles)

            languages_to_exclude = set(dict(filter(lambda x: x[1] <= optimization_threshold, languages.items())).keys())

            print(f'[!] Optimization Step {i}: Languages: {languages}')

            # If not in the last optimization cycle
            if i < optimization_cycles-1:

                print(f'[!] Optimization Step {i}: Threshold: {optimization_threshold}')
                print(f'[!] Optimization Step {i}: Excluding: {languages_to_exclude}')

            if len(languages_to_exclude) == 0 or len(languages) <= 2:
                break 

        return history


def calc_window_weight(window: list):
    return mean(window) if len(window) > 0 else 0


def obtain_expected_results(target_results_path: str) -> dict:

    expected_results = {}

    with open(target_results_path) as results_file:
        results_file_lines = results_file.readlines()
    
    for line in results_file_lines:
        
        line_split = line.split(' -> ')
        language = line_split[0]
        start_idx = int(line_split[1].split(":")[0].replace('(', ''))
        end_idx = int(line_split[1].split(":")[1].replace(")", ""))

        for i in range(start_idx, end_idx):
            expected_results[i] = language

    return expected_results


def obtain_results(languages_detection_history: list):

    results_obtained = {}
    for history_item in languages_detection_history:

        language = remove_file_extension(history_item[0])
        range_str = history_item[2]
        start_idx = int(range_str.split("-")[0])
        end_idx = int(range_str.split("-")[1])

        for i in range(start_idx, end_idx):
            results_obtained[i] = language

    return results_obtained


def evaluate_model(expected_results: dict, obtained_results: dict):

    hits = 0
    misses = 0

    for idx in expected_results.keys():

        if idx not in obtained_results.keys():
            misses += 1
            continue

        if expected_results[idx] == obtained_results[idx]:
            hits += 1
        else:
            misses += 1

    return round(hits/(hits+misses)*100, 2)


def print_results(target_text: str, obtained_results: dict, accuracy: float):

    obtained_languages_list = list(set(obtained_results.values()))
    available_colors = [Colors.FORE_RED, Colors.FORE_CYAN, Colors.FORE_YELLOW, Colors.FORE_GREEN, Colors.FORE_BLUE, Colors.FORE_MAGENTA,
                        Colors.FORE_RED_UNDER, Colors.FORE_CYAN_UNDER, Colors.FORE_YELLOW_UNDER, Colors.FORE_GREEN_UNDER, Colors.FORE_BLUE_UNDER, Colors.FORE_MAGENTA_UNDER,
                        Colors.FORE_RED_ITALIC, Colors.FORE_CYAN_ITALIC, Colors.FORE_YELLOW_ITALIC, Colors.FORE_GREEN_ITALIC, Colors.FORE_BLUE_ITALIC, Colors.FORE_MAGENTA_ITALIC,
                        Colors.FORE_RED_STRIKE, Colors.FORE_CYAN_STRIKE, Colors.FORE_YELLOW_STRIKE, Colors.FORE_GREEN_STRIKE, Colors.FORE_BLUE_STRIKE, Colors.FORE_MAGENTA_STRIKE]
    
    languages_color_dict = { language : color for language, color in zip(obtained_languages_list, available_colors) }

    last_language = None

    print('-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=')
    print()
    print(f'{Colors.BOLD}Results{Colors.RESET}')
    print()
    print(f'Accuracy: {accuracy}%')
    print()

    for character_idx in range(len(target_text)):

        if character_idx in obtained_results:
            character_language = obtained_results[character_idx]
        else:
            character_language = last_language

        character_color = languages_color_dict[character_language]

        last_language = character_language

        print(f'{character_color}{target_text[character_idx]}{Colors.RESET}', end='')

    print(f'\n\n{Colors.BOLD}Legend\n{Colors.RESET}')
    
    for language in languages_color_dict:
        print(f'  {Colors.BOLD}{language}{Colors.RESET}: {languages_color_dict[language]}COLOR{Colors.RESET}')

    print()
    print('-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=')


class InputArguments:

    def __init__(self):
        self.executable = None
        self.references_directory = None
        self.target_file_path = None
        self.interactive_mode = False
        self.lang_output_path = None
        self.window_size = None
        self.optimization = None     
        self.window_overlap = None    
        self.lang_window_size = None
        self.use_finite_context = None                                                            


    def parse_arguments(self, argv):

        for arg in argv:
            if arg == '-e' or arg == '--executable':
                self.executable = argv[argv.index(arg) + 1]
            if arg == '-r' or arg == '--references':
                self.references_directory = argv[argv.index(arg) + 1]
            elif arg == '-t' or arg == '--target':
                self.target_file_path = argv[argv.index(arg) + 1]
            elif arg == '-it' or arg == '--interactive':
                self.interactive_mode = True
            elif arg == '-O' or arg == '--optimize':
                self.optimization = int(argv[argv.index(arg) + 1])
            elif arg == '-w' or arg == '--window':
                self.window_size = int(argv[argv.index(arg) + 1])
            elif arg == '-o' or arg == '--overlap':
                self.window_overlap = float(1-float(argv[argv.index(arg) + 1]))
            elif arg == '-k' or arg == '--langWindowSize':
                self.lang_window_size = int(argv[argv.index(arg) + 1])
            elif arg == '-nFC' or arg == '--noFiniteContext':
                self.use_finite_context = False
            elif arg == '-h' or arg == '--help':
                self.print_usage()
    
    def check_arguments(self):
        
        if not self.executable:
            exit('[!] You must provide the Lang executable')

        if not self.references_directory:
            exit('[!] You must provide a directory containing Reference files.')
        
        if not self.window_size:
            self.window_size = 10
            print(f'[-] Using default window size: {self.window_size}')

        if self.window_overlap and 0 > self.window_overlap > 1:
            print('[!] You must specify a value between 0 and 1 for window overlap.')
        elif not self.window_overlap:
            self.window_overlap = 1
            print(f'[-] Using default window overlap: {1-self.window_overlap}')

        if self.optimization and 1 > self.optimization > 3:
            exit('[!] You must an integer value in [1, 3] for Optimization.')

        if not self.interactive_mode:
            if not self.target_file_path:
                exit('[!] You must provide a Target file.')

        if not self.lang_window_size:
            self.lang_window_size = 5
            print(f'[-] Using default window size {self.lang_window_size}')

        if self.use_finite_context is None:
            self.use_finite_context = True


    def print_usage(self):

        print('Usage: locatelang [arguments]')
        print('Arguments:')
        print('-e \t Executable path')
        print('-r \t Directory with reference files')
        print('-t \t Target file')
        print('-it \t Interactive mode')
        print('-w \t Window size')
        print('-O \t Optimization Level')
        print('-o \t Window Overlap')
        print('-k \t Window Size for Lang CPM')
        print('-nFC \t Disable Finite Context for Lang CPM')


class Colors:

    RESET = '\033[0m'
    HEADER = '\033[95m'

    FORE_RED = '\033[31m'
    FORE_BLUE = '\033[94m'
    FORE_CYAN = '\033[96m'
    FORE_GREEN = '\033[92m'
    FORE_YELLOW = '\033[33m'
    FORE_MAGENTA = '\033[35m'

    FORE_RED_UNDER = '\033[31;4m'
    FORE_BLUE_UNDER = '\033[34;4m'
    FORE_CYAN_UNDER = '\033[36;4m'
    FORE_GREEN_UNDER = '\033[32;4m'
    FORE_YELLOW_UNDER = '\033[33;4m'
    FORE_MAGENTA_UNDER = '\033[35;4m'

    FORE_RED_ITALIC = '\033[31;3m'
    FORE_BLUE_ITALIC = '\033[34;3m'
    FORE_CYAN_ITALIC = '\033[36;3m'
    FORE_GREEN_ITALIC = '\033[32;3m'
    FORE_YELLOW_ITALIC = '\033[33;3m'
    FORE_MAGENTA_ITALIC = '\033[35;3m'

    FORE_RED_STRIKE = '\033[31;9m'
    FORE_BLUE_STRIKE = '\033[34;9m'
    FORE_CYAN_STRIKE = '\033[36;9m'
    FORE_GREEN_STRIKE = '\033[32;9m'
    FORE_YELLOW_STRIKE = '\033[33;9m'
    FORE_MAGENTA_STRIKE = '\033[35;9m'

    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


if __name__ == "__main__":
    main()