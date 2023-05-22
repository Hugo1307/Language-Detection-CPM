#ifndef SRC_MODELGENERATOR_H
#define SRC_MODELGENERATOR_H


#include <string>
#include <vector>
#include <map>

#include "../IO/reader/ReferenceReader.h"

class ModelGenerator {

    ReferenceReader* referenceReader;
    std::map<std::string, std::vector<int>> sequencePositions;
    std::map<std::string, std::map<std::string, int>> finiteContextCounts;

public:

    explicit ModelGenerator(ReferenceReader* referenceReader);

    /**
     * Run the Model Generator
     *
     * It stores the generated model in "sequencePositions" variable.
     */
    void run();

    /**
     * Returns the positional model
     * @return the generated model
     */
    std::map<std::string, std::vector<int>> getPositionalModel();

    /**
     * Returns the finite context model
     * @return the finite context model
     */
    std::map<std::string, std::map<std::string, int>> getFiniteContextModel();

};

#endif
