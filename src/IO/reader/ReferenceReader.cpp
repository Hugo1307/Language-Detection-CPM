#include <iostream>
#include "ReferenceReader.h"
#include "../../utils/utils.h"

ReferenceReader::ReferenceReader(std::string filePath, int windowSize) : Reader(std::move(filePath)) {
    this->windowSize = windowSize;
    this->allocatedWindowSize = windowSize;
    this->currentWindow = static_cast<unsigned char*>(calloc(allocatedWindowSize, sizeof(char)));
    this->currentPosition = 0;
}

bool ReferenceReader::next() {

    if (!Reader::getFileInputStream()->is_open())
        return false;

    // Reset Allocated Window Size
    // It needs to be in the start of the function!!
    this->allocatedWindowSize = this->windowSize;

    for (int i = 0; i < this->allocatedWindowSize; i++) {

        if (Reader::getFileInputStream()->eof())
            return false;

        if (Reader::getFileInputStream()->fail())
            return false;

        unsigned char characterRead = Reader::getFileInputStream()->get();
        int numberOfBytes = numOfBytesInUTF8(characterRead);

        // If we are reading the first byte of a UTF-8 character
        if (numberOfBytes != -1) {

            int newCharacters = numberOfBytes - 1;

            // We will expand the window to accommodate the bytes that belong to the current character
            this->allocatedWindowSize += newCharacters;
            this->currentWindow = (unsigned char*) realloc(this->currentWindow, this->allocatedWindowSize);

        }

        // We want to read only characters that are not white lines, i.e., \n, \t, \r, etc.
        if (!isWhiteLineCharacter(characterRead) && !isForbiddenCharacter(characterRead))
            this->currentWindow[i] = (char) characterRead;

        this->currentPosition++;

    }

    return true;

}

int ReferenceReader::getWindowSize() const {
    return this->windowSize;
}

unsigned char* ReferenceReader::getCurrentWindow() {
    return this->currentWindow;
}

std::string ReferenceReader::getReferenceName() {
    return getFileNameFromPath(Reader::getFilePath());
}

int ReferenceReader::getCurrentPosition() const {
    return this->currentPosition;
}

int ReferenceReader::getAllocatedWindowSize() const {
    return this->allocatedWindowSize;
}
