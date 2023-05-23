#ifndef SRC_READER_H
#define SRC_READER_H

#include <fstream>

#include "../IOEntity.h"


class Reader : public IOEntity {

    std::string filePath;
    std::ifstream fileInputStream;

public:

    explicit Reader(std::string filePath);

    void openFile() override;
    bool isFileOpen() override;
    void closeFile() override;

    std::string readCharacter();

    // Getters
    const std::string &getFilePath() const;

    std::ifstream* getFileInputStream();

};


#endif
