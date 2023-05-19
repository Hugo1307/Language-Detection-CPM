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
    # -o: Output File Path for Lang (Optional)
    # -m: Model Directory Path for Lang (Optional)

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
            
            execute_lang(input_arguments.executable, entry, target_file_path, input_arguments.lang_output_path)

            with open(input_arguments.lang_output_path, 'r') as outputFile:
                lines = outputFile.readlines()
                information_per_file[entry.name] = float(lines[2].split(':')[1])

    information_per_file = dict(sorted(information_per_file.items(), key=lambda item: item[1]))

    print_results(information_per_file)


def execute_lang(executable_path: str, reference_path: str, target_path: str, output_path: str):

    models_path = '../models'

    subprocess.run([executable_path, '-r', reference_path, '-i', target_path, '-o',  output_path, '-m', models_path], 
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)


def remove_file_extension(file_name: str) -> str:
    return file_name.split('.')[0]


def calculate_confidence(information_per_reference: dict):

    max_information = max(information_per_reference.values())
    min_information = min(information_per_reference.values())

    # A distance between the First and Second references corresponds to a Confidence of 100%
    difference = max_information - min_information

    if difference == 0:
        return 0

    top_1_information = list(information_per_reference.items())[0][1]
    top_2_information = list(information_per_reference.items())[1][1]

    difference_between_top = top_2_information-top_1_information

    return round(difference_between_top*100/difference, 2)


def print_results(information_per_reference: dict):

    print('\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=')
    print('FindLang Results')
    print()
    print(f'  Detected Language: {remove_file_extension(list(information_per_reference.items())[0][0]).upper()}')
    print(f'  Confidence: {calculate_confidence(information_per_reference)}%')
    print()
    print(f'  Other Possible Languages: (Sorted by Confidence)')
    for i in range(1,4):
        print(f'    {i}) {remove_file_extension(list(information_per_reference.items())[i][0]).upper()}')
    print('\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=')

class InputArguments:

    def __init__(self):
        self.executable = None
        self.references_directory = None
        self.target_file_path = None
        self.interactive_mode = False
        self.lang_output_path = None
        self.lang_models_directory = None

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
            elif arg == '-o' or arg == '--output':
                self.lang_output_path = argv[argv.index(arg) + 1]
            elif arg == '-m' or arg == '--model':
                self.lang_models_directory = argv[argv.index(arg) + 1]

    def check_arguments(self):

        if not self.references_directory:
            exit('You must provide a directory containing Reference files.')
        
        if not self.interactive_mode:
            if not self.target_file_path:
                exit('You must provide a Target file.')

        if not self.lang_output_path:
            self.lang_output_path = '../output/lang_output.txt'

        if not self.lang_models_directory:
            self.lang_models_directory = '../models'

if __name__ == "__main__":
    main()