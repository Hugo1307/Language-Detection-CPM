#include "Writer.h"

Writer::Writer(std::string filePath) : filePath(std::move(filePath)) {}

void Writer::openFile() {

    std::ofstream modelOutputFile(this->filePath);

    if (!modelOutputFile.good()) {
        throw std::runtime_error("[Writer::openFile] File doesn't exist: " + this->filePath);
    }

    if (!modelOutputFile.is_open()) {
        throw std::runtime_error("[Writer::openFile] Could not open file: " + this->filePath);
    }

    this->fileOutputStream = std::move(modelOutputFile);

}

bool Writer::isFileOpen() {
    return this->fileOutputStream.is_open();
}

void Writer::closeFile() {
    this->fileOutputStream.close();
}

const std::string &Writer::getFilePath() const {
    return filePath;
}

std::ofstream* Writer::getFileOutputStream() {
    return &fileOutputStream;
}
