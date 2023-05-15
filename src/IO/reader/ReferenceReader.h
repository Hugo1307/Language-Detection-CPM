#ifndef SRC_REFERENCEREADER_H
#define SRC_REFERENCEREADER_H

#include <string>
#include <fstream>
#include <vector>

#include "Reader.h"

class ReferenceReader : public Reader {

    int windowSize;
    char* currentWindow;
    int currentPosition;

public:

    explicit ReferenceReader(std::string filePath, int windowSize);

    bool next();

    std::string getReferenceName();

    // Getters
    [[nodiscard]] int getWindowSize() const;
    char* getCurrentWindow();
    int getCurrentPosition() const;

};


#endif
