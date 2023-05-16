#include "CopyModelOutput.h"

#include <utility>
#include <iostream>

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

void CopyModelOutput::presentInformationPerIteration(bool truncated) {

    if (!truncated) {
        for (int i = 0; i < informationPerIteration->size(); i++)
            std::cout << "   Iteration " << i << ": " << informationPerIteration->at(i) << std::endl;
    } else {
        for (int i = 0; i < std::min((int) informationPerIteration->size(), 10); i++)
            std::cout << "   Iteration " << i << ": " << informationPerIteration->at(i) << std::endl;
        std::cout << "   ..." << std::endl;
    }


}

void CopyModelOutput::storeInformationPerIteration(Writer* writer) {

    for (int i = 0; i < informationPerIteration->size(); i++)
        *writer->getFileOutputStream() << "Iteration " << i << ": " << informationPerIteration->at(i) << std::endl;

}
