#ifndef SRC_WRITER_H
#define SRC_WRITER_H

#include <string>
#include <fstream>

#include "../IOEntity.h"

class Writer : public IOEntity {

    std::string filePath;
    std::ofstream fileOutputStream;

public:

    explicit Writer(std::string filePath);

    void openFile() override;
    bool isFileOpen() override;
    void closeFile() override;

    // Getters
    const std::string &getFilePath() const;
    std::ofstream* getFileOutputStream();

};


#endif
