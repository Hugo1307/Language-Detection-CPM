#ifndef SRC_REFERENCEREADER_H
#define SRC_REFERENCEREADER_H

#include <string>
#include <fstream>
#include <vector>

class ReferenceReader {

    std::string referencePath;
    int windowSize;

    // File reading parameters
    std::ifstream fileInputStream;
    char* currentWindow;

    int currentPosition;

public:

    explicit ReferenceReader(std::string filePath, int windowSize);

    void openFile();
    bool isFileOpen();
    void closeFile();

    bool next();

    std::string getReferenceName();

    // Getters
    std::string getReferencePath();

    [[nodiscard]] int getWindowSize() const;

    std::ifstream* getFileInputStream();

    char* getCurrentWindow();

    int getCurrentPosition() const;

};


#endif
