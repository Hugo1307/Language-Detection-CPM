#include "ModelGenerator.h"
#include "../utils/utils.h"

ModelGenerator::ModelGenerator(ReferenceReader *referenceReader) {
    this->referenceReader = referenceReader;
}

void ModelGenerator::run() {

    ReferenceReader* reader = this->referenceReader;

    if (!reader->isFileOpen())
        reader->openFile();

    while (reader->next()) {

        std::string sequenceStr = convertUCharArrToString(reader->getCurrentWindow(), reader->getAllocatedWindowSize());

        std::string lastCharacterOfSequence = getLastCharacterInString(sequenceStr);
        std::string sequenceWithoutLastCharacter = getAllButLastCharacterInString(sequenceStr);

        this->sequencePositions[sequenceStr].push_back(reader->getCurrentPosition());

        if (this->finiteContextCounts[lastCharacterOfSequence].count(sequenceWithoutLastCharacter) != 0) {
            this->finiteContextCounts[lastCharacterOfSequence][sequenceWithoutLastCharacter] += 1;
        } else {
            this->finiteContextCounts[lastCharacterOfSequence][sequenceWithoutLastCharacter] += 0;
        }

    }

    reader->closeFile();

}

std::map<std::string, std::vector<int>> ModelGenerator::getPositionalModel() {
    return this->sequencePositions;
}

std::map<std::string, std::map<std::string, int>> ModelGenerator::getFiniteContextModel() {
    return this->finiteContextCounts;
}
