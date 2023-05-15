#include "CopyModelOutputWriter.h"

CopyModelOutputWriter::CopyModelOutputWriter(std::string outputPath, CopyModelOutput *copyModelOutput)
    : Writer(std::move(outputPath)), copyModelOutput(copyModelOutput) {}

void CopyModelOutputWriter::write() {

    if (!Writer::isFileOpen())
        throw std::runtime_error("[CopyModelOutputWriter::write] File is not open");

    /*
    * Reference:
    * Target:
    * Total Information:
    * Information per iteration:
    */

    *Writer::getFileOutputStream() << "Reference: " << this->copyModelOutput->getReferencePath() << std::endl;
    *Writer::getFileOutputStream() << "Target: " << this->copyModelOutput->getTargetPath() << std::endl;
    *Writer::getFileOutputStream() << "Total Information: " << this->copyModelOutput->getTotalInformationAmount() << std::endl;
    *Writer::getFileOutputStream() << "Information p/ Symbol: " << this->copyModelOutput->getInformationPerSymbol() << std::endl;

    // TODO: The getInformationPerIteration returns a pointer to a vector. We need to deference it and output the values
    *Writer::getFileOutputStream() << "Information p/ Iteration: " << this->copyModelOutput->getInformationPerIteration() << std::endl;

}
