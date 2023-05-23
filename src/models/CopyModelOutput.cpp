#include "CopyModelOutput.h"

#include <utility>
#include <iostream>

CopyModelOutput::CopyModelOutput(std::string referencePath, std::string targetPath,
                                 double totalInformationAmount, double informationPerSymbol,
                                 std::map<int, double> *informationPerIteration)
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

std::map<int, double>* CopyModelOutput::getInformationPerIteration() {
    return informationPerIteration;
}

double CopyModelOutput::getInformationPerSymbol() const {
    return informationPerSymbol;
}

void CopyModelOutput::presentInformationPerIteration(bool truncated) {

    if (!truncated) {
        for (auto & i : *informationPerIteration)
            std::cout << "   " << i.first << ":" << i.second << std::endl;
    } else {
        int count = 0;
        for (auto & pair : *informationPerIteration) {
            if (count++ >= 10) break;
            std::cout << "   " << pair.first << ":" << pair.second << std::endl;
        }
        std::cout << "   ..." << std::endl;
    }


}

void CopyModelOutput::storeInformationPerIteration(Writer* writer) {

    for (auto & i : *informationPerIteration)
        *writer->getFileOutputStream() << i.first << ":" << i.second << std::endl;

}
