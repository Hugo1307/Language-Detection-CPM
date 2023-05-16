#ifndef SRC_COPYMODELEXECUTOR_H
#define SRC_COPYMODELEXECUTOR_H


#include <map>
#include <vector>
#include "../IO/reader/CopyModelReader.h"
#include "../IO/reader/FileInfoReader.h"
#include "../IO/reader/RandomAccessReader.h"
#include "../models/CopyModelOutput.h"
#include "../models/GeneratedModel.h"

class CopyModelExecutor {

    CopyModelReader* copyModelReader;
    FileInfoReader* fileInfoReader;
    RandomAccessReader* randomAccessReader;
    GeneratedModel* generatedModel;

    std::vector<double>* informationPerIteration;
    std::map<std::string, int> currentPointerIndexForSequence;

    double informationAmount = 0;

public:

    CopyModelExecutor(CopyModelReader* reader, FileInfoReader* fileInfo, RandomAccessReader* randomReader,
                               GeneratedModel* generatedModel);

    void run(double alpha, double threshold);

    [[nodiscard]] CopyModelOutput generateOutput();

    [[nodiscard]] double getInformationAmount() const;
    [[nodiscard]] double getInformationPerSymbol() const;
    [[nodiscard]] std::vector<double>* getInformationPerIteration();

};


#endif
