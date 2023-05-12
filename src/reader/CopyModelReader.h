#ifndef SRC_COPYMODELREADER_H
#define SRC_COPYMODELREADER_H


#include <string>
#include <vector>
#include <fstream>

class CopyModelReader {

    std::string targetPath;
    int windowSize;

    std::ifstream fileInputStream;
    std::vector<char> currentWindow;

    int currentPosition;    // Position where we are reading currently
    int pastPosition;       // Position in the past that indicates the character we are comparing to our current character

public:

    explicit CopyModelReader(std::string filePath, int windowSize);

    void openFile();
    bool isFileOpen();
    void closeFile();

    bool readWindow();
    bool expand();

    // Getters
    std::string getTargetPath();
    [[nodiscard]] int getWindowSize() const;

    std::vector<char> getCurrentWindow();

    int getCurrentPosition() const;
    int getPastPosition() const;

};


#endif
