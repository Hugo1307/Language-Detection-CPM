#ifndef SRC_COPYMODELOUTPUT_H
#define SRC_COPYMODELOUTPUT_H

#include <string>

class CopyModelOutput {

    std::string referencePath;
    std::string targetPath;
    double totalInformationAmount;
    double informationPerSymbol;
    std::vector<double>* informationPerIteration;

public:

    CopyModelOutput(std::string referencePath, std::string targetPath, double totalInformationAmount,
                    double informationPerSymbol, std::vector<double>* informationPerIteration);

    [[nodiscard]] const std::string &getReferencePath() const;

    [[nodiscard]] const std::string &getTargetPath() const;

    [[nodiscard]] double getTotalInformationAmount() const;

    [[nodiscard]] double getInformationPerSymbol() const;

    [[nodiscard]] std::vector<double>* getInformationPerIteration();

};


#endif
