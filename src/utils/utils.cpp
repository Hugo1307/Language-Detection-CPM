#include "utils.h"

std::string getFileNameFromPath(const std::string& path) {
    std::string fileNameWithExtension = path.substr(path.find_last_of("/\\") + 1);
    return fileNameWithExtension.substr(0, fileNameWithExtension.find_last_of('.')); // Remove extension
}

std::string convertCharArrToString(char* charArr, int size) {

    std::string generatedString;

    for (int i = 0; i < size; i++)
        generatedString += charArr[i];

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