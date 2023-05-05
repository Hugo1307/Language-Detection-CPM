#ifndef SRC_COPYMODELGENERATOR_H
#define SRC_COPYMODELGENERATOR_H


#include <string>
#include <vector>
#include <map>

class CopyModelGenerator {

    std::map<std::string, std::vector<int>> sequencePositions;
    // FileInfo
    // FileReader

    void run();

    void save();

};


#endif
