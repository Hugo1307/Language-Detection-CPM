from os import listdir
from os.path import isfile, join
from random import sample, randint, choice
from json import dump, dumps

path = "europarl20/"
target_path = "target/"


onlyfiles = {f.rstrip(".txt") : join(path, f) for f in listdir(path) if isfile(join(path, f))}

def mix_languages(lang_files : dict, n_languages = 3, size = 1000):
    languages = sample(list(lang_files.keys()), n_languages)
    files_name = '_'.join(languages) + ".txt"
    language_content = dict()
    for language in languages:
        with open(lang_files[language], "r") as f:
            content = f.read().split()
            language_content[language] = content

    results = list()
    with open(join(target_path, files_name), "w") as output_text:
        n_words = 0
        while n_words < size:
            language = choice(languages)
            words = randint(3,15)
            index = randint(0, len(language_content[language]) - words)
            content = language_content[language][index : index+words]
            output_text.write(' '.join(content))
            output_text.write(' ')
            result = (language, n_words, n_words + words - 1)
            results.append(result)

            n_words += words

    with open(join(target_path+"results/", files_name), "w") as result_file:
        for result in results:
            result_file.write(f"{result[0]} -> ({result[1]} : {result[2]})\n")


mix_languages(onlyfiles)