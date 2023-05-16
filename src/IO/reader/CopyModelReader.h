#ifndef SRC_COPYMODELREADER_H
#define SRC_COPYMODELREADER_H


#include <string>
#include <vector>
#include <fstream>
#include "Reader.h"

class CopyModelReader : public Reader {

    int windowSize;
    std::vector<char> currentWindow;

    int currentPosition;    // Position where we are reading currently
    int pastPosition;       // Position in the past that indicates the character we are comparing to our current character

public:

    explicit CopyModelReader(std::string filePath, int windowSize);

    bool readWindow();
    bool expand();

    // Getters
    [[nodiscard]] int getWindowSize() const;

    std::vector<char> getCurrentWindow();

    int getCurrentPosition() const;
    int getPastPosition() const;

private:

    void shiftWindow();

};


#endif
