#ifndef SRC_COPYMODELEXECUTOR_H
#define SRC_COPYMODELEXECUTOR_H


#include <map>
#include <vector>
#include "../IO/reader/CopyModelReader.h"
#include "../IO/reader/FileInfoReader.h"
#include "../IO/reader/RandomAccessReader.h"
#include "../models/CopyModelOutput.h"

class CopyModelExecutor {

    CopyModelReader* copyModelReader;
    FileInfoReader* fileInfoReader;
    RandomAccessReader* randomAccessReader;
    std::map<std::string, std::vector<int>> model;

    std::map<std::string, int> currentPointerIndexForSequence;

    double informationAmount = 0;

public:

    CopyModelExecutor(CopyModelReader* reader, FileInfoReader* fileInfo, RandomAccessReader* randomReader,
                               const std::map<std::string, std::vector<int>> &model);

    void run();

    [[nodiscard]] CopyModelOutput generateOutput();

    [[nodiscard]] double getInformationAmount() const;
    [[nodiscard]] double getInformationPerSymbol() const;
    [[nodiscard]] std::vector<double>* getInformationPerIteration();

};


#endif
