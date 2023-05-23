#include "FileInfoReader.h"
#include "../../utils/utils.h"

#include <utility>

FileInfoReader::FileInfoReader(const std::string& filePath) : Reader(filePath) {}

void FileInfoReader::obtainMetrics() {

    if (!Reader::isFileOpen())
        throw std::runtime_error("[FileInfoReader::obtainMetrics] File is not open.");

    while (!Reader::getFileInputStream()->eof()) {

        std::string character = Reader::readCharacter();

        if (!character.empty()) {
            this->alphabet.insert(character);
            this->size++;
        }

    }

}

std::set<std::string> FileInfoReader::getAlphabet() {
    return this->alphabet;
}

int FileInfoReader::getSize() const {
    return this->size;
}
