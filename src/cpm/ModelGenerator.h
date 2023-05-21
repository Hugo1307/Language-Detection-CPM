#ifndef SRC_MODELGENERATOR_H
#define SRC_MODELGENERATOR_H


#include <string>
#include <vector>
#include <map>

#include "../IO/reader/ReferenceReader.h"

class ModelGenerator {

    ReferenceReader* referenceReader;
    std::string outputPath; // Path for the directory where the generated models will be stored
    std::map<std::string, std::vector<int>> sequencePositions;
    std::map<std::string, std::map<std::string, int>> finiteContextCounts;

public:

    explicit ModelGenerator(ReferenceReader* referenceReader, std::string outputPath);

    /**
     * Check if we already have a cached model for the current reference file
     * @return true if we have a stored model, false otherwise
     */
    bool isCached();

    /**
     * Run the Model Generator
     *
     * It stores the generated model in "sequencePositions" variable.
     */
    void run();

    /**
     * Saves the generated model in the output path
     */
    void save();

    /**
     * Loads the cached model
     */
    void load();

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

private:

    /**
     * Builds the path for the model output file
     *
     * @return the path for the model output file
     */
    std::string obtainModelOutputPath();

};

#endif
