#include <iostream>
#include "ModelGenerator.h"
#include "../utils/utils.h"

ModelGenerator::ModelGenerator(ReferenceReader *referenceReader, std::string outputPath) {
    this->referenceReader = referenceReader;
    this->outputPath = std::move(outputPath);
}

bool ModelGenerator::isCached() {

    std::string cachedModelPath = this->obtainModelOutputPath();
    std::ifstream file(cachedModelPath);
    return file.good();

}

void ModelGenerator::save() {

    std::string modelOutputPath = this->obtainModelOutputPath();
    std::ofstream modelOutputFile(modelOutputPath);

    if (!modelOutputFile.is_open()) {
        throw std::runtime_error("[ModelGenerator::save] Could not openFile file: " + modelOutputPath);
    }

    modelOutputFile << "Model - " << this->referenceReader->getReferenceName() << std::endl;

    // Iterate over map
    for (const auto& sequencePositionsPair: this->sequencePositions) {

        std::string sequence = sequencePositionsPair.first;
        std::vector<int> positions = sequencePositionsPair.second;

        modelOutputFile << sequence << ";";

        for (auto position: positions)
            modelOutputFile << position << ",";

        modelOutputFile << std::endl;

    }

    modelOutputFile.close();

}

void ModelGenerator::run() {

    ReferenceReader* reader = this->referenceReader;

    if (!reader->isFileOpen())
        reader->openFile();

    while (reader->next()) {

        std::string sequenceStr = convertCharArrToString(reader->getCurrentWindow(), reader->getWindowSize());
        this->sequencePositions[sequenceStr].push_back(reader->getCurrentPosition());

    }

    reader->closeFile();

}

std::string ModelGenerator::obtainModelOutputPath() {
    // Todo: Check if the output path ends with a "/", if it doesn't add it instead of always adding it
    return this->outputPath + "/" + this->referenceReader->getReferenceName() + "-model.txt";
}

void ModelGenerator::load() {

    std::string modelOutputPath = this->obtainModelOutputPath();
    std::ifstream modelFileInputStream(modelOutputPath);

    if (!modelFileInputStream.is_open()) {
        throw std::runtime_error("[ModelGenerator::load] Could not openFile file: " + modelOutputPath);
    }

    std::string line;

    std::getline(modelFileInputStream, line); // Discard Header Line

    while (std::getline(modelFileInputStream, line)) {

        std::string window = line.substr(0, line.find(';'));
        std::string positions = line.substr(line.find(';') + 1, line.length());

        std::vector<int> positionsVector;

        size_t currentSplitPosition = positions.find(',');

        while (currentSplitPosition != std::string::npos) {
            positionsVector.push_back(std::stoi(positions.substr(0, currentSplitPosition)));
            positions.erase(0, currentSplitPosition + 1);
            currentSplitPosition = positions.find(',');
        }

        this->sequencePositions[window] = positionsVector;

    }

    modelFileInputStream.close();

}

std::map<std::string, std::vector<int>> ModelGenerator::getModel() {
    return this->sequencePositions;
}

