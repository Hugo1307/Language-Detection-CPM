#include "SequentialReader.h"

#include <utility>
#include <iostream>

SequentialReader::SequentialReader(std::string filePath) : Reader(std::move(filePath)) {}

bool SequentialReader::readFile() {

    if (!Reader::getFileInputStream()->is_open())
        return false;

    if (Reader::getFileInputStream()->fail())
        return false;

    while (!Reader::getFileInputStream()->eof()) {
        std::string characterRead = Reader::readCharacter();
        this->textInMemory.push_back(characterRead);
    }

    return true;

}

std::string SequentialReader::getCharacterAt(int position) {
    // std::cout << "Size of Memory: " << this->textInMemory.size() << std::endl;
    // std::cout << "Position: " << position << std::endl;
    return this->textInMemory[position];
}
