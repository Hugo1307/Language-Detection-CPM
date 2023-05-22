from os import listdir
from os.path import isfile, join
from random import sample, randint, choice
from json import dump, dumps

path = "../references/europarl20/"
target_path = "../references/target/"

onlyfiles = {f.rstrip(".txt"): join(path, f) for f in listdir(path) if isfile(join(path, f))}


def mix_languages(lang_files: dict, n_languages=3, size=1000):
    # Choose n languages and build file name accordingly
    languages = sample(list(lang_files.keys()), n_languages)
    files_name = '_'.join(languages) + ".txt"

    # Get content for each of the chosen languages
    language_content = dict()
    for language in languages:
        with open(lang_files[language], "r", encoding="utf8") as f:
            content = f.read().split()
            language_content[language] = content

    # Generate mixed text
    results = list()
    with open(join(target_path, files_name), "w", encoding="utf8") as output_text:
        curr_char = 0
        while curr_char < size:
            # Select a language and the number of words to be added
            language = choice(languages)
            words = randint(3, 15)

            # Point to a random part of the file
            index = randint(0, len(language_content[language]) - words)

            # Fetch content and write to resulting file
            content = language_content[language][index: index + words]
            to_write = ' '.join(content)
            output_text.write(to_write)
            output_text.write(' ')
            added_chars = len(to_write)+1
            result = (language, curr_char, curr_char + added_chars - 1)
            results.append(result)

            curr_char += added_chars

    with open(join(target_path + "results/", files_name), "w") as result_file:
        prev_result = None
        begin = 0
        end = 0
        for result in results:
            print("_____________")
            print(prev_result)
            print(begin, end)
            print(result)
            if result[0] == prev_result or not prev_result:
                end = result[2]
            else:
                result_file.write(f"{prev_result} -> ({begin} : {end})\n")
                begin = result[1]
                end = result[2]
            prev_result = result[0]
        print("_____________")
        print(prev_result)
        print(begin, end)
        print(result)
        if result[0] == prev_result:
            end = result[2]
        else:
            result_file.write(f"{prev_result} -> ({begin} : {end})\n")
            begin = result[1]
            end = result[2]
        result_file.write(f"{result[0]} -> ({begin} : {end})\n")


mix_languages(onlyfiles, 3, 1000)
