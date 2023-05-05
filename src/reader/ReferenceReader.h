#ifndef SRC_REFERENCEREADER_H
#define SRC_REFERENCEREADER_H

#include <string>
#include <fstream>
#include <vector>

class ReferenceReader {

    // Constructor parameters
    std::string filePath;
    int windowSize;

    // File reading parameters
    std::ifstream fileInputStream;

    // Handle windows
    char* currentWindow;
    char* lastWindow;

    int currentPosition;
    std::vector<char> currentSequence;
    char nextCharacterInSequence{};

public:

    explicit ReferenceReader(std::string filePath, int windowSize);

    std::ifstream openFile();
    void closeFile();

    bool next();

    FileInfo getFileInfo();

    char* getWindowContent();
    char* getLastWindowContent();

    std::vector<char>* getCurrentSequence();

    [[nodiscard]] int getWindowSize() const;
    [[nodiscard]] int getCurrentPosition() const;

    [[nodiscard]] char getNextCharacterInSequence() const;

private:

    void readInitialWindow();
    bool shiftWindow();
    void resetCurrentSequence();

};


#endif
