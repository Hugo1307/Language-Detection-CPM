import subprocess
from os import scandir
import time

def main():

    targets_directory = '../references/single_language/'
    output_file = open('findLangResults.txt', 'w')

    output_file.write('Target,Time,WindowSize,DetectedLanguage,Confidence,SimilarLanguages\n')

    correct_detections = 0
    wrong_detections = 0

    with scandir(targets_directory) as entries:
        
        for target_entry in entries:  
            
            target_name = remove_file_extension(target_entry.name)

            for cpm_window_size in [3, 5, 10, 12]:

                print(f'Running for: Target: {target_name}, K {cpm_window_size}')

                start_time = time.time()

                output = subprocess.getoutput(f'python3 ../src/findlang.py -e ../bin/lang -r ../references/europarl20/ -t ../references/single_language/{target_entry.name} -k {cpm_window_size}')
                
                output_lines = output.splitlines()
                confidence = 0
                similar_languages = [None]*3

                for line in output_lines:

                    if 'Detected Language' in line:
                        detected_language = line.split(':')[1].strip(' ').lower()

                    if 'Confidence:' in line:
                        confidence = float(line.split(':')[1].replace('%', ''))
                    
                    if '1)' in line:
                        similar_languages[0] = line.split('1) ')[1]
                    
                    if '2)' in line:
                        similar_languages[1] = line.split('2) ')[1]
                    
                    if '3)' in line:
                        similar_languages[2] = line.split('3) ')[1]

                time_elapsed = round(time.time() - start_time, 3)

                output_file.write(f'{target_name},{time_elapsed},{cpm_window_size},{detected_language},{confidence},{similar_languages}\n')
                output_file.flush()

                if detected_language.lower() == target_name:
                    print(f'Correctly detected {detected_language}')
                    correct_detections += 1
                else:
                    print(f'Failed to detect {target_name}. Detected as {detected_language}')
                    wrong_detections += 1
    
    output_file.close()

    print(f'Accuracy: {correct_detections/(correct_detections+wrong_detections)}')


def remove_file_extension(file_name: str) -> str:
    return file_name.split('.')[0]


if __name__ == "__main__":
    main()