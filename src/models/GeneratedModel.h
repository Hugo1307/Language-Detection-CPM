#ifndef SRC_GENERATEDMODEL_H
#define SRC_GENERATEDMODEL_H

#include <map>
#include <string>
#include <vector>

class GeneratedModel {

    std::string referencePath;
    std::map<std::string, std::vector<int>> positionalModel;
    std::map<std::string, std::map<std::string, int>> finiteContextModel;

public:

    GeneratedModel(std::string referencePath, const std::map<std::string, std::vector<int>> &positionalModel,
                   const std::map<std::string, std::map<std::string, int>> &finiteContextModel);

    [[nodiscard]] const std::string &getReferencePath() const;
    [[nodiscard]] const std::map<std::string, std::vector<int>> &getPositionalModel() const;
    [[nodiscard]] const std::map<std::string, std::map<std::string, int>> &getFiniteContextModel() const;

};

#endif
