#ifndef SRC_COPYMODELEXECUTOR_H
#define SRC_COPYMODELEXECUTOR_H


#include <map>
#include <vector>

class CopyModelExecutor {

    std::map<std::string, std::vector<int>> model;
    double informationAmount = 0;

public:

    explicit CopyModelExecutor(const std::map<std::string, std::vector<int>> &model);

    void run();

    double getInformationAmount();

};


#endif
