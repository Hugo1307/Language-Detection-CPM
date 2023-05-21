#include <cmath>
#include <utility>
#include <iostream>

#include "CopyModelExecutor.h"
#include "../utils/utils.h"
#include "HitsMissesInfo.h"

CopyModelExecutor::CopyModelExecutor(CopyModelReader* reader, FileInfoReader* fileInfo, RandomAccessReader* randomReader,
                                     GeneratedModel* generatedModel) {

    this->generatedModel = generatedModel;
    this->copyModelReader = reader;
    this->fileInfoReader = fileInfo;
    this->randomAccessReader = randomReader;
    this->informationPerIteration = new std::vector<double>();

}

void CopyModelExecutor::run(double alpha, double threshold, bool useFiniteContext) {

    CopyModelReader* fileReader = this->copyModelReader;
    std::map<std::string, std::vector<int>> positionalModel = this->generatedModel->getPositionalModel();
    std::map<std::string, std::map<std::string, int>> finiteContextModel = this->generatedModel->getFiniteContextModel();

    while (fileReader->readWindow()) {

        std::string sequenceAsString = convertVectorToString(fileReader->getCurrentWindow());

        // If the sequence exists in the positional Model
        if (positionalModel.count(sequenceAsString) > 0) {

            // std::cout << "Exists in Positional Model: "  << sequenceAsString << std::endl;

            int currentPointerIndex = 0;

            // If I haven't chosen a pointer for this sequence yet, chose the first pointer
            if (currentPointerIndexForSequence.count(sequenceAsString) <= 0) {
                currentPointerIndexForSequence.insert(std::make_pair(sequenceAsString, currentPointerIndex));
            } else {
                currentPointerIndex = currentPointerIndexForSequence[sequenceAsString];
            }

            int pastSequencePosition = positionalModel[sequenceAsString][currentPointerIndex];

            // The probability P (probability of the next character being equal to the one I'm seeing now in the
            // current copy model)
            double probabilityOfCorrectPrediction;

            // Hits and misses info for the current copy model
            HitsMissesInfo hitsMissesInfo;

            int expandedCharacters = 0;

            while (fileReader->expand()) {

                // int pastSequenceOffset = (int)fileReader.getCurrentSequence()->size() - (fileReader.getWindowSize()-1);
                char predictedChar = randomAccessReader->getCharAt(pastSequencePosition + expandedCharacters++);
                char nextCharacterInSequence = fileReader->getCurrentWindow()[fileReader->getCurrentWindow().size()-1];

                // Calculate the probability P (a.k.a. probability of Hit)
                probabilityOfCorrectPrediction = calculateHitProbability(hitsMissesInfo.getHits(),
                                                                         hitsMissesInfo.getMisses(), alpha);

                // Value to distribute for other characters
                double complementaryProbability = 1-probabilityOfCorrectPrediction;

                // If it turns out the next character in the copy model to be equal to the next character in the
                // sequence then we have a hit.
                if (predictedChar == nextCharacterInSequence) {

                    hitsMissesInfo.incrementHits();

                    // The total information is the sum of the information of each character at that point taking into
                    // account the probability of the character being correct
                    double currentInformation = -std::log2(probabilityOfCorrectPrediction);

                    // std::cout << "Current Information (on Hit " << convertVectorToString(fileReader->getCurrentWindow()) << "): " << currentInformation << std::endl;

                    informationAmount += currentInformation;
                    informationPerIteration->push_back(currentInformation);

                } else { // Otherwise, next character in sequence != of character in copy model, we have a miss

                    hitsMissesInfo.incrementMisses();

                    // Calculate probability of fail a.k.a. non-hit using Finite Context Model
                    std::string lastCharacterAsString = getLastCharacterInString(sequenceAsString);
                    std::string contextAsString = getAllButLastCharacterInString(sequenceAsString);

                    // Equals to N(e|c)
                    double countOfCharWithContext = finiteContextModel[lastCharacterAsString][contextAsString];
                    // Equals to Sum(N(s|c))
                    double countOfAlphabetWithContext = 0;

                    for (const auto& alphabetChar : fileInfoReader->getAlphabet()) {
                        countOfAlphabetWithContext += (finiteContextModel[alphabetChar][contextAsString] + alpha * (int)fileInfoReader->getAlphabet().size());
                    }

                    double probabilityOfFail;

                    if (useFiniteContext) {
                        probabilityOfFail = (countOfCharWithContext + alpha) / countOfAlphabetWithContext;
                    } else {
                        probabilityOfFail = complementaryProbability / (int) (fileInfoReader->getAlphabet().size()-1);
                    }

                    // The total information is the sum of the information of each character at that point taking into
                    // account the probability of the character being correct
                    double currentInformation = -std::log2(probabilityOfFail);

                    // std::cout << "Current Information (on Fail " << convertVectorToString(fileReader->getCurrentWindow()) << "): " << currentInformation << std::endl;

                    informationAmount += currentInformation;
                    informationPerIteration->push_back(currentInformation);

                }

                // Interrupt loop if the probability is below the threshold
                if (probabilityOfCorrectPrediction < threshold) break;

            }

            if (probabilityOfCorrectPrediction < threshold && positionalModel[sequenceAsString].size() > 1) {
                // Change the pointer to the next one
                currentPointerIndexForSequence[sequenceAsString] += 1;
            }

        } else {

            // Once we can't initialize the copy model, we do a uniform distribution of probabilities for all characters
            // calculate the information and add it to the total amount of information
            double currentInformation = -std::log2(1.0 / (int)fileInfoReader->getAlphabet().size());

            // std::cout << "Current Information (on Not Found " << sequenceAsString << "): " << currentInformation << std::endl;

            informationAmount += currentInformation;
            informationPerIteration->push_back(currentInformation);

        }

    }

}

double CopyModelExecutor::getInformationAmount() const {
    return informationAmount;
}

double CopyModelExecutor::getInformationPerSymbol() const {
    return informationAmount / fileInfoReader->getSize();
}

std::vector<double>* CopyModelExecutor::getInformationPerIteration() {
    return informationPerIteration;
}

CopyModelOutput CopyModelExecutor::generateOutput() {

    return {generatedModel->getReferencePath(), fileInfoReader->getFilePath(),
                           getInformationAmount(),
                           getInformationPerSymbol(),
                           getInformationPerIteration()};

}
