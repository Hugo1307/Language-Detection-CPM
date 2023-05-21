#include "FileInfoReader.h"
#include "../../utils/utils.h"

#include <utility>

FileInfoReader::FileInfoReader(const std::string& filePath) : Reader(filePath) {}

void FileInfoReader::obtainMetrics() {

    if (!Reader::isFileOpen())
        throw std::runtime_error("[FileInfoReader::obtainMetrics] File is not open.");

    while (!Reader::getFileInputStream()->eof()) {

        unsigned char characterRead = Reader::getFileInputStream()->get();
        int numberOfBytes = numOfBytesInUTF8(characterRead);
        std::string characterAsString = std::string();

        characterAsString += (char) characterRead;

        // If we are reading the first byte of a UTF-8 character
        if (numberOfBytes != -1) {
            for (int i = 0; i < numberOfBytes - 1; i++) {
                characterAsString += (char) Reader::getFileInputStream()->get();
            }
        }

        if (!isWhiteLineCharacter(characterRead) && !isForbiddenCharacter(characterRead)) {
            this->alphabet.insert(characterAsString);
        }

        this->size++;

    }

}

std::set<std::string> FileInfoReader::getAlphabet() {
    return this->alphabet;
}

int FileInfoReader::getSize() const {
    return this->size;
}
