#ifndef SRC_GENERATEDMODEL_H
#define SRC_GENERATEDMODEL_H

#include <map>
#include <string>

class GeneratedModel {

    std::string referencePath;
    std::map<std::string, std::vector<int>> model;

public:

    GeneratedModel(std::string referencePath, const std::map<std::string, std::vector<int>> &model);

    [[nodiscard]] const std::string &getReferencePath() const;
    [[nodiscard]] const std::map<std::string, std::vector<int>> &getModel() const;

};


#endif
