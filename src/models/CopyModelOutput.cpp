#include "CopyModelOutput.h"

#include <utility>

CopyModelOutput::CopyModelOutput(std::string referencePath, std::string targetPath,
                                 double totalInformationAmount, double informationPerSymbol,
                                 std::vector<double> *informationPerIteration)
        : referencePath(std::move(referencePath)), targetPath(std::move(targetPath)),
            totalInformationAmount(totalInformationAmount), informationPerSymbol(informationPerSymbol),
            informationPerIteration(informationPerIteration) {}

const std::string &CopyModelOutput::getReferencePath() const {
    return referencePath;
}

const std::string &CopyModelOutput::getTargetPath() const {
    return targetPath;
}

double CopyModelOutput::getTotalInformationAmount() const {
    return totalInformationAmount;
}

std::vector<double>* CopyModelOutput::getInformationPerIteration() {
    return informationPerIteration;
}

double CopyModelOutput::getInformationPerSymbol() const {
    return informationPerSymbol;
}
