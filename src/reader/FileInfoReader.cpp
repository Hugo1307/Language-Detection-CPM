#include "FileInfoReader.h"
#include "../utils/utils.h"

#include <utility>

FileInfoReader::FileInfoReader(const std::string& filePath) {
    this->filePath = filePath;
}

void FileInfoReader::openFile() {

    // Read file character by character
    std::ifstream inputStream(this->filePath);

    if (!inputStream.is_open())
        throw std::runtime_error("Could not open file: " + this->filePath);

    this->fileInputStream = std::move(inputStream);

}

bool FileInfoReader::isFileOpen() {
    return this->fileInputStream.is_open();
}

void FileInfoReader::closeFile() {
    this->fileInputStream.close();
}

void FileInfoReader::obtainMetrics() {

    if (!this->fileInputStream.is_open())
        throw std::runtime_error("File is not open");

    int characterRead;

    while ((characterRead = this->fileInputStream.get()) != EOF) {

        if (isWhiteLineCharacter(characterRead) || isForbiddenCharacter(characterRead))
            continue;

        this->alphabet.insert((char) characterRead);
        this->size++;

    }

}

std::string FileInfoReader::getFilePath() const {
    return this->filePath;
}

std::set<char> FileInfoReader::getAlphabet() {
    return this->alphabet;
}

int FileInfoReader::getSize() const {
    return this->size;
}
