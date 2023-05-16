#include "FileInfoReader.h"
#include "../../utils/utils.h"

#include <utility>

FileInfoReader::FileInfoReader(const std::string& filePath) : Reader(filePath) {}

void FileInfoReader::obtainMetrics() {

    if (!Reader::isFileOpen())
        throw std::runtime_error("[FileInfoReader::obtainMetrics] File is not open.");

    int characterRead;

    while ((characterRead = Reader::getFileInputStream()->get()) != EOF) {

        if (!isWhiteLineCharacter(characterRead) && !isForbiddenCharacter(characterRead)) {
            this->alphabet.insert((char) characterRead);
        }

        this->size++;

    }

}

std::set<char> FileInfoReader::getAlphabet() {
    return this->alphabet;
}

int FileInfoReader::getSize() const {
    return this->size;
}
