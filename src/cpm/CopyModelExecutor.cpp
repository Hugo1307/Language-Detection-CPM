#include <cmath>
#include "CopyModelExecutor.h"
#include "../utils/utils.h"
#include "HitsMissesInfo.h"
#include <iostream>

#define THRESHOLD 0.5
#define ALPHA 0.5

CopyModelExecutor::CopyModelExecutor(CopyModelReader* reader, FileInfoReader* fileInfo, RandomAccessReader* randomReader,
                                     const std::map<std::string, std::vector<int>> &model) {
    this->model = model;
    this->copyModelReader = reader;
    this->fileInfoReader = fileInfo;
    this->randomAccessReader = randomReader;
}

void CopyModelExecutor::run() {

    CopyModelReader* fileReader = this->copyModelReader;

    while (fileReader->readWindow()) {

        std::string sequenceAsString = convertVectorToString(fileReader->getCurrentWindow());

        // If the sequence exists in the model
        if (model.count(sequenceAsString) > 0 && model[sequenceAsString][0] < fileReader->getCurrentPosition() - 1) {

            // std::cout << "Exists in model: "  << sequenceAsString << std::endl;

            int currentPointerIndex = 0;

            // If I haven't chosen a pointer for this sequence yet, chose the first pointer
            if (currentPointerIndexForSequence.count(sequenceAsString) <= 0) {
                currentPointerIndexForSequence.insert(std::make_pair(sequenceAsString, currentPointerIndex));
            } else {
                currentPointerIndex = currentPointerIndexForSequence[sequenceAsString];
            }

            int pastSequencePosition = model[sequenceAsString][currentPointerIndex];

            // The probability P (probability of the next character being equal to the one I'm seeing now in the
            // current copy model)
            double probabilityOfCorrectPrediction;

            // Hits and misses info for the current copy model
            HitsMissesInfo hitsMissesInfo;

            int expandedCharacters = 0;

            while (fileReader->expand() && probabilityOfCorrectPrediction >= THRESHOLD) {

                // int pastSequenceOffset = (int)fileReader.getCurrentSequence()->size() - (fileReader.getWindowSize()-1);
                char predictedChar = randomAccessReader->getCharAt(pastSequencePosition + expandedCharacters++);
                char nextCharacterInSequence = fileReader->getCurrentWindow()[fileReader->getCurrentWindow().size()-1];

                // Calculate the probability P (a.k.a. probability of Hit)
                probabilityOfCorrectPrediction = calculateHitProbability(hitsMissesInfo.getHits(),
                                                                         hitsMissesInfo.getMisses(), ALPHA);

                // Value to distribute for other characters
                double complementaryProbability = 1-probabilityOfCorrectPrediction;

                // If it turns out the next character in the copy model to be equal to the next character in the
                // sequence then we have a hit.
                if (predictedChar == nextCharacterInSequence) {

                    hitsMissesInfo.incrementHits();

                    // The total information is the sum of the information of each character at that point taking into
                    // account the probability of the character being correct
                    informationAmount += -std::log2(probabilityOfCorrectPrediction);

                } else { // Otherwise, next character in sequence != of character in copy model, we have a miss

                    hitsMissesInfo.incrementMisses();

                    double probabilityOfFail = complementaryProbability / (int) (fileInfoReader->getAlphabet().size()-1);

                    // The total information is the sum of the information of each character at that point taking into
                    // account the probability of the character being correct
                    informationAmount += -std::log2(probabilityOfFail);

                }

            }

            if (probabilityOfCorrectPrediction < THRESHOLD && model[sequenceAsString].size() > 1) {

                // Change the pointer to the next one
                currentPointerIndexForSequence[sequenceAsString] += 1;

                // std::cout << "Changed Pointer" << std::endl;

            }

        } else {

            // Once we can't initialize the copy model, we do a uniform distribution of probabilities for all characters
            // calculate the information and add it to the total amount of information
            informationAmount += -std::log2(1.0 / (int)fileInfoReader->getAlphabet().size());

        }

    }

}

double CopyModelExecutor::getInformationAmount() const {
    return informationAmount;
}
