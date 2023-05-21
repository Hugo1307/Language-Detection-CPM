#include "GeneratedModel.h"

#include <utility>

GeneratedModel::GeneratedModel(std::string referencePath, const std::map<std::string, std::vector<int>> &positionalModel,
                               const std::map<std::string, std::map<std::string, int>> &finiteContextModel) :
    referencePath(std::move(referencePath)), positionalModel(positionalModel), finiteContextModel(finiteContextModel) {}

const std::string &GeneratedModel::getReferencePath() const {
    return this->referencePath;
}

const std::map<std::string, std::vector<int>> &GeneratedModel::getPositionalModel() const {
    return this->positionalModel;
}

const std::map<std::string, std::map<std::string, int>> &GeneratedModel::getFiniteContextModel() const {
    return this->finiteContextModel;
}
