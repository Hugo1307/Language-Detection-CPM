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
    # -s: Sensibility

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

            # if not os.path.exists(output_path):
            #    execute_lang(input_arguments.executable, entry, target_file_path, output_path)

            execute_lang(input_arguments.executable, entry, target_file_path, output_path)

            with open(output_path, 'r') as outputFile:

                lines = outputFile.readlines()
                information_per_file[entry.name] = read_information(lines)

    results = identify_language_window(information_per_file, 15)

    if results:
        for result in results:
            print(f'Language {result[0]} at index {result[2]} - Weight: {result[1]}')

    # Evaluate model if it is not in interactive mode
    if not input_arguments.interactive_mode:

        target_file_name = os.path.basename(input_arguments.target_file_path)
        target_dir_name = os.path.dirname(input_arguments.target_file_path)

        expected_results = obtain_expected_results(join(join(target_dir_name, 'results'), target_file_name))
        obtained_results = obtain_results(results)
        
        evaluate_model(expected_results, obtained_results)


def execute_lang(executable_path: str, reference_path: str, target_path: str, output_path: str):

    subprocess.run([executable_path, '-r', reference_path, '-i', target_path, '-o', output_path], 
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


def identify_language(information_data: dict, sensibility: float):

    threshold = 1

    # Identify language with most information
    max_information = max([len(information_data[language]) for language in information_data])

    current_language = None
    languages_hits = {}

    for idx in range(max_information):

        min_info = float('inf')
        language_with_min = None

        for language in information_data:
            if len(information_data[language]) > idx:
                if information_data[language][idx][1] < threshold:
                    min_info = information_data[language][idx]
                    language_with_min = language

        if language_with_min is None:
            continue

        if current_language is None:
            current_language = language_with_min
            print(f"Current Language is {current_language}")

        # Count hits for each language
        if language_with_min in languages_hits:
            
            if languages_hits[language_with_min] <= 30:
                languages_hits[language_with_min] += 1

            if language_with_min != current_language:
                # Maybe we can count the hits and misses and apply the probability formula from the first work, i.e., 
                # as we find more and more misses we will remove more from the current language instead of removing a 
                # static value
                languages_hits[current_language] -= sensibility

            # If some language has more hits than the current language, change the language prediction
            if languages_hits[language_with_min] > languages_hits[current_language]:
                
                current_language = language_with_min
                current_language_hits = languages_hits[current_language]

                languages_hits = {}
                languages_hits[current_language] = current_language_hits

                print(f'Current Language changed to {language_with_min} with min {min_info} for index {idx}.')

        else:
            # Initialize Hits Count
            languages_hits[language_with_min] = 1
            

def identify_language_window(languages_info: dict, window_size: int):

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
        # for current_lang in ['polish.txt', 'english.txt', 'czech.txt']:
            
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
        # if finalized_languages == 3:
            break

        # Update the Window for Next Iteration
        window_start_idx += int(window_size)
        window_last_idx = window_start_idx + window_size

    print("Languages Counts: ", dict(sorted(languages_counts.items(), key=lambda x: x[1], reverse=True)))

    return languages_detection_history


#def identify_language_v2(information_data: dict, window_size: float):

    # Identify language with most information
    complete_information_data = information_data

    information_data = {key : list(map(lambda x: x[1], value)) for key, value in information_data.items() }

    min_information = min([len(information_data[language]) for language in information_data])

    current_starting_index = 0

    current_language = None
    current_min = float('inf')

    languages_found = {}

    while current_starting_index < min_information - window_size:
        
        iteration_languages_with_avg = {}

        for language in information_data.keys():
            
            current_language_data = information_data[language]
            current_window = current_language_data[current_starting_index : min(len(current_language_data), current_starting_index+window_size)]

            if len(current_window) != 0:
                window_average = mean(current_window)
                iteration_languages_with_avg[language] = current_window

        iteration_languages_sorted = dict(sorted(iteration_languages_with_avg.items(), key=lambda item: calc_window_weight(item[1])))

        items_list = list(iteration_languages_sorted.items())

        if items_list[0][1] is not None and items_list[0][0] != current_language and items_list[0][0] in ['portuguese.txt', 'bulgarian.txt', 'dutch.txt']:
            current_language = items_list[0][0]

            if current_language in languages_found:
                languages_found[current_language] += 1
            else:
                languages_found[current_language] = 1

            print(f'Changed Current Language to {current_language} on index {current_starting_index} - {calc_window_weight(items_list[0][1])}')
            # print(f'{iteration_languages_sorted}')

            # print(f'Current window average for language {language} is {window_average}')

        current_starting_index += 1

    print(dict(sorted(languages_found.items(), key=lambda x: x[1], reverse=True)))
        

def calc_window_weight(window: list):

    reciprocal_values = [1/val for val in window]
    reciprocal_values_sum = sum(reciprocal_values)
    weights = [reciproque_value/reciprocal_values_sum for reciproque_value in reciprocal_values ]

    return sum(value * weight for value, weight in zip(window, weights))


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
            print(f'Added One Miss because idx {idx} was not found in obtained results')
            continue

        if expected_results[idx] == obtained_results[idx]:
            # print(f"Hit: Idx {idx}, Expected {expected_results[idx]}, Obtained {obtained_results[idx]}")
            hits += 1
        else:
            # print(f"Miss: Idx {idx}, Expected {expected_results[idx]}, Obtained {obtained_results[idx]}")
            misses += 1

    print(f"Correct percentage: {round(hits/(hits+misses)*100, 3)}")

class InputArguments:

    def __init__(self):
        self.executable = None
        self.references_directory = None
        self.target_file_path = None
        self.interactive_mode = False
        self.lang_output_path = None
        self.lang_models_directory = None
        self.sensibility = None

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
            elif arg == '-m' or arg == '--model':
                self.lang_models_directory = argv[argv.index(arg) + 1]
            elif arg == '-s' or arg == '--sensibility':
                self.sensibility = float(argv[argv.index(arg) + 1])

    def check_arguments(self):

        if not self.references_directory:
            exit('You must provide a directory containing Reference files.')
        
        if not self.interactive_mode:
            if not self.target_file_path:
                exit('You must provide a Target file.')

        if not self.lang_models_directory:
            self.lang_models_directory = '../models'
            print('[-] Using default "models" directory...')

         # Sensibility of the model (from 0 to 1): Higher number = more sensible
        if not self.sensibility:
            self.sensibility = 0.2
            print('[-] Using default Sensibility value of 0.2')

if __name__ == "__main__":
    #evaluate_model('/Users/hugogoncalves/Documents/Faculdade/Mestrado/Semestre_2/TAI/Assignment2/TAI_Group_6/references/target/results/english_polish_czech.txt')
    main()