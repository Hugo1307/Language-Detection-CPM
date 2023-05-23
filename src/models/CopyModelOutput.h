#ifndef SRC_COPYMODELOUTPUT_H
#define SRC_COPYMODELOUTPUT_H

#include <string>
#include <map>

#include "../IO/writer/Writer.h"

class CopyModelOutput {

    std::string referencePath;
    std::string targetPath;
    double totalInformationAmount;
    double informationPerSymbol;
    std::map<int, double>* informationPerIteration;

public:

    CopyModelOutput(std::string referencePath, std::string targetPath, double totalInformationAmount,
                    double informationPerSymbol, std::map<int, double>* informationPerIteration);

    [[nodiscard]] const std::string &getReferencePath() const;

    [[nodiscard]] const std::string &getTargetPath() const;

    [[nodiscard]] double getTotalInformationAmount() const;

    [[nodiscard]] double getInformationPerSymbol() const;

    [[nodiscard]] std::map<int, double>* getInformationPerIteration();

    void presentInformationPerIteration(bool truncated);

    void storeInformationPerIteration(Writer* writer);

};


#endif
