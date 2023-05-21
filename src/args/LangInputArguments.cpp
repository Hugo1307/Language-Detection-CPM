#include <cstring>
#include "LangInputArguments.h"

LangInputArguments::LangInputArguments() = default;

bool LangInputArguments::checkArguments() const {

    if (this->referenceFilePath.empty()) {
        std::cerr << "[!!!] The reference file was not provided." << std::endl;
        return false;
    }

    if (this->targetFilePath.empty()) {
        std::cerr << "[!!!] The target file was not provided." << std::endl;
        return false;
    }

    if (this->outputFilePath.empty()) {
        std::cerr << "[!!!] The output file was not provided." << std::endl;
        return false;
    }

    if (this->alpha <= 0 || this->alpha > 5) {
        std::cerr << "[!!!] Alpha should be a value in the interval ]0, 5]." << std::endl;
        return false;
    }

    if (this->threshold <= 0 || this->threshold > 0.5) {
        std::cerr << "[!!!] The threshold should be a value in the interval ]0, 0.5]." << std::endl;
        return false;
    }

    if (this->k <= 0) {
        std::cerr << "[!!!] K should be a value greater than 0." << std::endl;
        return false;
    }

    if (this->outputModelPath.empty()) {
        std::cerr << "[!!!] The output model path was not provided." << std::endl;
        return false;
    }

    return true;
}

void LangInputArguments::printUsage() {
    std::cout << "lang: lang [OPTIONS]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "-r \t File with reference sequence" << std::endl;
    std::cout << "-i \t File with target sequence" << std::endl;
    std::cout << "-o \t File with lang output" << std::endl;
    std::cout << "-m \t Output model path" << std::endl;
    std::cout << "-a \t Alpha" << std::endl;
    std::cout << "-k \t Window size" << std::endl;
    std::cout << "-t \t Threshold" << std::endl;
    std::cout << "-nFC \t Disable finite context for Non-Hit Symbols" << std::endl;
    std::cout << "-h \t Print this help message" << std::endl;
}

void LangInputArguments::parseArguments(int argc, char **argv) {

    if (argc == 1) {
        std::cerr << "No arguments were entered." << std::endl;
        LangInputArguments::printUsage();
        return;
    }

    // Print help
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            LangInputArguments::printUsage();
            exit(EXIT_SUCCESS);
        } else if (strcmp(argv[i], "-nFC") == 0 || strcmp(argv[i], "--noFiniteContext") == 0) {
            this->useFiniteContext = false;
        }
    }

    // Note: argc - 1 because the last argument can't be a flag
    for (int i = 0; i < argc - 1; ++i) {
        if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--alpha") == 0) {
            this->alpha = strtod(argv[i + 1], nullptr);
        } else if (strcmp(argv[i], "-k") == 0) {
            this->k = (int) strtol(argv[i + 1], nullptr, 10);
        } else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--threshold") == 0) {
            this->threshold = strtod(argv[i + 1], nullptr);
        } else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--reference") == 0) {
            this->referenceFilePath = argv[i + 1];
        } else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) {
            this->targetFilePath = argv[i + 1];
        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
            this->outputFilePath = argv[i + 1];
        } else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--model") == 0) {
            this->outputModelPath = argv[i + 1];
        }
    }
}

std::string LangInputArguments::getReferenceFilePath() {
    return this->referenceFilePath;
}

std::string LangInputArguments::getTargetFilePath() {
    return this->targetFilePath;
}

std::string LangInputArguments::getOutputFilePath() {
    return this->outputFilePath;
}

double LangInputArguments::getAlpha() const {
    return this->alpha;
}

int LangInputArguments::getK() const {
    return this->k;
}

double LangInputArguments::getThreshold() const {
    return this->threshold;
}

std::string LangInputArguments::getOutputModelPath() {
    return this->outputModelPath;
}

bool LangInputArguments::getUseFiniteContext() const {
    return this->useFiniteContext;
}
