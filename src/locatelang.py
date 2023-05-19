from os import scandir
from os.path import join
import subprocess
import sys
import os

def main():

    # Input Arguments
    # -e: Executable path
    # -r: Directory with the reference files
    # -it: Interactive mode (allows to input text in the terminal and use it as target)
    # -t: Target File 
    # -m: Model Directory Path for Lang (Optional)
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

    identify_language(information_per_file, input_arguments.sensibility)
    # print(information_per_file)


def execute_lang(executable_path: str, reference_path: str, target_path: str, output_path: str):

    models_path = '../models'

    subprocess.run([executable_path, '-r', reference_path, '-i', target_path, '-o',  output_path, '-m', models_path], 
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)


def remove_file_extension(file_name: str) -> str:
    return file_name.split('.')[0]


def read_information(file_lines: list[str]) -> list[float]:

    lines_with_information = file_lines[5:]
    information_list = []

    for lineIdx in range(len(lines_with_information)):

        line = lines_with_information[lineIdx]
        
        if not line.startswith("Iteration"):
            print(f'Line with wrong format at {lineIdx}')
            return []
        
        line_splited = line.split(':')

        if len(line_splited) != 2:
            print(f'Unable to parse line at {lineIdx}')
            return []
        
        information_list.append(float(line_splited[1]))

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
                if information_data[language][idx] < threshold:
                    min_info = information_data[language][idx]
                    language_with_min = language

        if language_with_min is None:
            continue

        if current_language is None:
            current_language = language_with_min
            print(f"Current Language is {current_language}")

        # Count hits for each language
        if language_with_min in languages_hits:
        
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
    main()
