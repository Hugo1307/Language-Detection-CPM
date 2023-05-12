#include <iostream>
#include "CopyModelReader.h"
#include "../utils/utils.h"

void CopyModelReader::openFile() {

    std::ifstream inputStream(this->targetPath);

    if (!inputStream.is_open())
        throw std::runtime_error("Could not open file " + this->targetPath);

    this->fileInputStream = std::move(inputStream);

}

bool CopyModelReader::isFileOpen() {
    return this->fileInputStream.is_open();
}

void CopyModelReader::closeFile() {
    this->fileInputStream.close();
}

bool CopyModelReader::readWindow() {

    if (!this->fileInputStream.is_open())
        return false;

    this->currentWindow.clear();

    for (int i = 0; i < this->windowSize; i++)
        this->expand();
    return true;

}

bool CopyModelReader::expand() {

    if (!this->fileInputStream.is_open()) {
        std::cerr << "[CopyModelReader::expand] File is not open" << std::endl;
        return false;
    }

    if (this->fileInputStream.eof() || this->fileInputStream.fail()) {
        std::cerr << "[CopyModelReader::expand] File is at the end or failed to read." << std::endl;
        return false;
    }

    // TODO: When we read a new character, we are reading an integer. This integer is then converted to a char.
    // In practice, it will be truncated and we will only obtain the first 1 Byte. This is the reason why our windows
    // are not working properly, i.e., they usually have the "?" mark at the beginning and at the end.
    // We need to find a way to solve this in the future.

    int characterRead = fileInputStream.get();

    // We want to read only characters that are not white lines, i.e., \n, \t, \r, etc.
    if (!isWhiteLineCharacter(characterRead) && !isForbiddenCharacter(characterRead))
        this->currentWindow.push_back((char) characterRead);

    this->currentPosition++;

    return true;

}
