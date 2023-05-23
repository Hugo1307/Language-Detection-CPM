#include <iostream>
#include "ReferenceReader.h"
#include "../../utils/utils.h"

ReferenceReader::ReferenceReader(std::string filePath, int windowSize) : Reader(std::move(filePath)) {
    this->windowSize = windowSize;
    this->allocatedWindowSize = windowSize;
    this->currentWindow = new std::string();
    this->currentPosition = 0;
}

bool ReferenceReader::next() {

    if (!Reader::getFileInputStream()->is_open())
        return false;

    this->currentWindow->clear();

    for (int i = 0; i < this->windowSize; i++) {

        if (Reader::getFileInputStream()->eof())
            return false;

        if (Reader::getFileInputStream()->fail())
            return false;

        std::string character = Reader::readCharacter();
        this->currentWindow->append(character);

    }

    // std::cout << "Current Position: " << this->currentPosition << std::endl;
    this->currentPosition += this->windowSize;

    return true;

}

int ReferenceReader::getWindowSize() const {
    return this->windowSize;
}

std::string* ReferenceReader::getCurrentWindow() {
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
