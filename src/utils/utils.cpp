#include "utils.h"

std::string getFileNameFromPath(const std::string& path) {
    std::string fileNameWithExtension = path.substr(path.find_last_of("/\\") + 1);
    return fileNameWithExtension.substr(0, fileNameWithExtension.find_last_of('.')); // Remove extension
}

std::string convertUCharArrToString(unsigned char* charArr, int size) {

    std::string generatedString;

    for (int i = 0; i < size; i++)
        generatedString += (char) charArr[i];

    return generatedString;

}

std::string convertVectorToString(const std::vector<char>& vector) {

    std::string generatedString;

    for (char i : vector)
        generatedString += i;

    return generatedString;

}

bool isWhiteLineCharacter(int character) {

    switch (character) {
        case 0x9: // Tab
        case 0xA: // New Line
        case 0xD: // Carriage Return
            return true;
        default:
            return false;
    }

}

bool isForbiddenCharacter(int character) {

    switch (character) {
        case 0x2c: // ,
        case 0x3b: // ;
            return true;
        default:
            return false;
    }

}

double calculateHitProbability(const unsigned int Nh, const unsigned int Nf, const double alpha) {
    return (double) (Nh + alpha) / (Nh + Nf + 2*alpha);
}

std::string getLastCharacterInString(const std::string& sourceString) {

    std::string lastCharacter;

    for (int characterIdx = (int)sourceString.size()-1; characterIdx >= 0; characterIdx--) {

        int numberOfBytesForCharacter = numOfBytesInUTF8(sourceString[characterIdx]);

        if (numberOfBytesForCharacter != -1) {
            lastCharacter = sourceString.substr(characterIdx, sourceString.size() - 1);
            break;
        }

    }

    return lastCharacter;

}

std::string getAllButLastCharacterInString(const std::string& sourceString) {

    std::string lastCharacter;

    for (int characterIdx = (int)sourceString.size()-1; characterIdx >= 0; characterIdx--) {

        int numberOfBytesForCharacter = numOfBytesInUTF8(sourceString[characterIdx]);

        if (numberOfBytesForCharacter != -1) {
            lastCharacter = sourceString.substr(0, characterIdx);
            break;
        }

    }

    return lastCharacter;

}

short numOfBytesInUTF8(unsigned char character) {

    unsigned int characterAsInt = character;
    unsigned int mostSignificantNibble = characterAsInt >> 4;

    if (mostSignificantNibble <= 0x7) {
        return 1;
    } else if (mostSignificantNibble >= 0xC && mostSignificantNibble <= 0xD) {
        return 2;
    } else if (mostSignificantNibble == 0xE) {
        return 3;
    } else if (mostSignificantNibble == 0xF) {
        return 4;
    }

    // We are not analysing the most significant Byte!
    return -1;

}
