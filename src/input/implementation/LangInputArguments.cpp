#include "LangInputArguments.h"

LangInputArguments::LangInputArguments() = default;

bool LangInputArguments::checkArguments() const {

    if (this->inputFilePath.empty()) {
        std::cerr << "[!!!] The input file was not provided." << std::endl;
        return false;
    }

    // Note: Checked in the copy model arguments

//    if (this->referenceFilePath.empty()) {
//        std::cerr << "[!!!] The reference file was not provided." << std::endl;
//        return false;
//    }
//
//    if (this->outputModelPath.empty()) {
//        std::cerr << "[!!!] The output model path was not provided." << std::endl;
//        return false;
//    }

    if (!this->copyModelInputArguments.checkArguments()) {
        std::cerr << "[!!!] The copy model arguments are not valid." << std::endl;
        return false;
    }

    return true;
}

void LangInputArguments::printUsage() {
    std::cout << "lang: lang [OPTIONS]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "-a \t Alpha" << std::endl;
    std::cout << "-k \t Window size" << std::endl;
    std::cout << "-t \t Threshold" << std::endl;
    std::cout << "-r \t File with reference sequence" << std::endl;
    std::cout << "-i \t File with input sequence" << std::endl;
    std::cout << "-o \t File with output model" << std::endl;
    std::cout << "-h \t Print this help message" << std::endl;
}

void LangInputArguments::parseArguments(int argc, char **argv) {

    if (argc == 1) {
        std::cerr << "No arguments were entered." << std::endl;
        LangInputArguments::printUsage();
        return;
    }

    // Copy model arguments
    int parsedK = 0;
    double parsedAlpha = 0;
    double parsedThreshold = 0;

    std::string parsedReferenceFilePath;
    std::string parsedInputFilePath;
    std::string parsedOutputFilePath;

    // Print help
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            LangInputArguments::printUsage();
            exit(EXIT_SUCCESS);
        }
    }

    // Note: argc - 1 because the last argument can't be a flag
    for (int i = 0; i < argc - 1; ++i) {

        if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--alpha") == 0) {

            if (isNumber(argv[i + 1]))
                parsedAlpha = atof(argv[i + 1]);

        } else if (strcmp(argv[i], "-k") == 0) {

            if (isNumber(argv[i + 1]))
                parsedK = atoi(argv[i + 1]);

        } else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--threshold") == 0) {

            if (isNumber(argv[i + 1]))
                parsedThreshold = atof(argv[i + 1]);

        } else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--reference") == 0) {

            parsedReferenceFilePath = argv[i + 1];

        } else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) {

            parsedInputFilePath = argv[i + 1];

        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {

            parsedOutputFilePath = argv[i + 1];

        }
    }

    if (parsedAlpha == 0) {
        std::cout << "Alpha was not provided. Using default value: 1" << std::endl;
        parsedAlpha = 1;
    }

    if (parsedK == 0) {
        std::cout << "Window size was not provided. Using default value: 3" << std::endl;
        parsedK = 3;
    }

    if (parsedThreshold == 0) {
        std::cout << "Threshold was not provided. Using default value: 0.4" << std::endl;
        parsedThreshold = 0.4;
    }

    // Lang Arguments
    this->inputFilePath = parsedInputFilePath;

    // TODO: Check if redundant
    this->referenceFilePath = parsedReferenceFilePath;
    this->outputModelPath = parsedOutputFilePath;

    // Initialize Copy Model Arguments
    this->copyModelInputArguments = CopyModelInputArguments();

    // Set Copy Model Arguments
    this->copyModelInputArguments.setAlpha(parsedAlpha);  // Set alpha (default: 1)
    this->copyModelInputArguments.setK(parsedK);  // Set window size (default: 3)
    this->copyModelInputArguments.setThreshold(parsedThreshold);  // Set threshold (default: 0.4)
    this->copyModelInputArguments.setInputFilePath(parsedReferenceFilePath); // Set input file path
    this->copyModelInputArguments.setOutputModelPath(parsedOutputFilePath); // Set output model path
}

std::string LangInputArguments::getInputFilePath() {
    return this->inputFilePath;
}

std::string LangInputArguments::getReferenceFilePath() {
    return this->referenceFilePath;
}

std::string LangInputArguments::getOutputModelPath() {
    return this->outputModelPath;
}

CopyModelInputArguments LangInputArguments::getCopyModelInputArguments() {
    return this->copyModelInputArguments;
}