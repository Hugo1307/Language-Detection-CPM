from os import scandir
from subprocess import getoutput
import time
import sys

def main():
    
    targets_directory = '../target'

    target_file_name = sys.argv[1]
    target_name = remove_file_extension(target_file_name)

    output_file = open(f'./locateLangResults-{target_name}.csv', 'w')

    output_file.write('Target,Accuracy,K,Window_Size,Window_Overlap,Optimization_Level,Finite_Context,Time\n')
            
    for k in [3, 5, 8, 10, 12]:

        for optimization_level in [0, 1, 2, 3]:

            for window_size in [3, 5, 8, 10, 12, 15]:

                for window_overlap in [0, 0.2, 0.4, 0.6, 0.8]:

                    for use_finite_context in [True, False]:
                        
                        start_time = time.time()

                        print(f'Testing {target_file_name}: K {k}, Optimization {optimization_level}, Window Size {window_size}, Window Overlap {window_overlap}, Finite Context {use_finite_context}')                              

                        if use_finite_context:
                            output = getoutput(f'python3 ../src/locatelang.py -e ../bin/lang -r ../references/europarl20/ -t ../target/{target_file_name} -k {k} -w {window_size} -o {window_overlap} -O {optimization_level}')                                
                        else:
                            output = getoutput(f'python3 ../src/locatelang.py -e ../bin/lang -r ../references/europarl20/ -t ../target/{target_file_name} -k {k} -w {window_size} -o {window_overlap} -O {optimization_level} -nFC')

                        elapsed_time = round(time.time() - start_time, 3)

                        # Write to output file
                        for line in output.splitlines():
                            
                            if line.startswith('Accuracy'):
                                accuracy = float(line.split(':')[1].strip('%'))

                        output_file.write(f'{target_file_name},{accuracy},{k},{window_size},{window_overlap},{optimization_level},{use_finite_context},{elapsed_time}\n')
                        output_file.flush()

                        print(f'Finished in {elapsed_time}. Accuracy: {accuracy}')
            
            
 
    output_file.close()


def remove_file_extension(file_name: str) -> str:
    return file_name.split('.')[0]


if __name__ == "__main__":
    main()
