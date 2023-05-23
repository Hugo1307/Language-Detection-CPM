#ifndef SRC_SEQUENTIALREADER_H
#define SRC_SEQUENTIALREADER_H

#include "Reader.h"

#include <string>

class SequentialReader : public Reader {

    std::vector<std::string> textInMemory;
    std::string filePath;

public:

    explicit SequentialReader(std::string filePath);

    bool readFile();

    std::string getCharacterAt(int position);

};


#endif
