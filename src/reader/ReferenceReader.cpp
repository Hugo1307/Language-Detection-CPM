#include <cstring>
#include <iostream>
#include <codecvt>
#include <locale>
#include "ReferenceReader.h"
#include "../utils/utils.h"

ReferenceReader::ReferenceReader(std::string filePath, int windowSize) {
    this->referencePath = std::move(filePath);
    this->windowSize = windowSize;
    this->currentWindow = static_cast<char*>(calloc(windowSize, sizeof(char)));
    this->currentPosition = 0;
}

void ReferenceReader::openFile() {

    // Read file character by character
    std::ifstream inputStream(this->referencePath);

    if (!inputStream.is_open())
        throw std::runtime_error("Could not open file: " + this->referencePath);

    this->fileInputStream = std::move(inputStream);

}

void ReferenceReader::closeFile() {
    this->fileInputStream.close();
}

bool ReferenceReader::next() {

    if (!this->fileInputStream.is_open())
        return false;

    for (int i = 0; i < this->windowSize; i++) {

        if (this->fileInputStream.eof())
            return false;

        if (this->fileInputStream.fail())
            return false;

        // TODO: When we read a new character, we are reading an integer. This integer is then converted to a char.
        // In practice, it will be truncated and we will only obtain the first 1 Byte. This is the reason why our windows
        // are not working properly, i.e., they usually have the "?" mark at the beginning and at the end.
        // We need to find a way to solve this in the future.

        int characterRead = fileInputStream.get();

        // We want to read only characters that are not white lines, i.e., \n, \t, \r, etc.
        if (!isWhiteLineCharacter(characterRead) && !isForbiddenCharacter(characterRead))
            this->currentWindow[i] = (char) characterRead;

        this->currentPosition++;

    }

    return true;

}

std::string ReferenceReader::getReferencePath() {
    return this->referencePath;
}

int ReferenceReader::getWindowSize() const {
    return this->windowSize;
}

std::ifstream* ReferenceReader::getFileInputStream() {
    return &this->fileInputStream;
}

char* ReferenceReader::getCurrentWindow() {
    return this->currentWindow;
}

std::string ReferenceReader::getReferenceName() {
    return getFileNameFromPath(this->referencePath);
}

bool ReferenceReader::isFileOpen() {
    return this->fileInputStream.is_open();
}

int ReferenceReader::getCurrentPosition() const {
    return this->currentPosition;
}
