#include "FileInfoReader.h"

#include <utility>

FileInfoReader::FileInfoReader(const std::string& filePath, std::set<char> alphabet, std::map<char, int> symbolsCount, int size) {
    this->alphabet = std::move(alphabet);
    this->symbolsCount = std::move(symbolsCount);
    this->size = size;
    this->filePath = filePath;
}

std::set<char> FileInfoReader::getAlphabet() {
    return this->alphabet;
}

std::map<char, int> FileInfoReader::getSymbolsCount() {
    return this->symbolsCount;
}

std::map<char, double> FileInfoReader::getSymbolsDistribution() {

    std::map<char, double> symbolsDistribution;

    for (auto &symbol : this->symbolsCount)
        symbolsDistribution[symbol.first] = (double) symbol.second / this->size;

    return symbolsDistribution;

}

int FileInfoReader::getSize() const {
    return this->size;
}

std::string FileInfoReader::getFilePath() const {
    return this->filePath;
}

