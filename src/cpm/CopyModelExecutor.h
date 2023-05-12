#ifndef SRC_COPYMODELEXECUTOR_H
#define SRC_COPYMODELEXECUTOR_H


#include <map>
#include <vector>
#include "../reader/CopyModelReader.h"
#include "../reader/FileInfoReader.h"
#include "../reader/RandomAccessReader.h"

class CopyModelExecutor {

    CopyModelReader* copyModelReader;
    FileInfoReader* fileInfoReader;
    RandomAccessReader* randomAccessReader;
    std::map<std::string, std::vector<int>> model;

    std::map<std::string, int> currentPointerIndexForSequence;

    double informationAmount = 0;

public:

    explicit CopyModelExecutor(CopyModelReader* reader, FileInfoReader* fileInfo, RandomAccessReader* randomReader,
                               const std::map<std::string, std::vector<int>> &model);

    void run();

    double getInformationAmount();

};


#endif
