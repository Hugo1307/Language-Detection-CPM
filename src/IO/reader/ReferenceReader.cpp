#include <iostream>
#include "ReferenceReader.h"
#include "../../utils/utils.h"

ReferenceReader::ReferenceReader(std::string filePath, int windowSize) : Reader(std::move(filePath)) {
    this->windowSize = windowSize;
    this->currentWindow = static_cast<char*>(calloc(windowSize, sizeof(char)));
    this->currentPosition = 0;
}

bool ReferenceReader::next() {

    if (!Reader::getFileInputStream()->is_open())
        return false;

    for (int i = 0; i < this->windowSize; i++) {

        if (Reader::getFileInputStream()->eof())
            return false;

        if (Reader::getFileInputStream()->fail())
            return false;

        // TODO: When we read a new character, we are reading an integer. This integer is then converted to a char.
        // In practice, it will be truncated and we will only obtain the first 1 Byte. This is the reason why our windows
        // are not working properly, i.e., they usually have the "?" mark at the beginning and at the end.
        // We need to find a way to solve this in the future.

        int characterRead = Reader::getFileInputStream()->get();

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

char* ReferenceReader::getCurrentWindow() {
    return this->currentWindow;
}

std::string ReferenceReader::getReferenceName() {
    return getFileNameFromPath(Reader::getFilePath());
}

int ReferenceReader::getCurrentPosition() const {
    return this->currentPosition;
}
