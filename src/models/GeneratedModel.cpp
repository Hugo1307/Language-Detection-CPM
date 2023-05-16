#include "GeneratedModel.h"

#include <utility>

GeneratedModel::GeneratedModel(std::string referencePath, const std::map<std::string, std::vector<int>> &model) :
    referencePath(std::move(referencePath)), model(model) {}

const std::string &GeneratedModel::getReferencePath() const {
    return this->referencePath;
}

const std::map<std::string, std::vector<int>> &GeneratedModel::getModel() const {
    return this->model;
}

