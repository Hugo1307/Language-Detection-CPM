#include "Reader.h"

#include <iostream>

Reader::Reader(std::string filePath) : filePath(std::move(filePath)) {}

void Reader::openFile() {

    // Read file character by character
    std::ifstream inputStream(this->filePath);

    if (!inputStream.is_open())
        throw std::runtime_error("Could not open File: " + this->filePath);

    this->fileInputStream = std::move(inputStream);

}

bool Reader::isFileOpen() {
    return this->fileInputStream.is_open();
}

void Reader::closeFile() {
    this->fileInputStream.close();
}

const std::string &Reader::getFilePath() const {
    return filePath;
}

std::ifstream* Reader::getFileInputStream() {
    return &fileInputStream;
}
