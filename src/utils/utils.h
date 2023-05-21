#ifndef SRC_UTILS_H
#define SRC_UTILS_H

#include <string>
#include <vector>

std::string getFileNameFromPath(const std::string& path);

std::string convertUCharArrToString(unsigned char* charArr, int size);

std::string convertVectorToString(const std::vector<char>& vector);

bool isWhiteLineCharacter(int character);

bool isForbiddenCharacter(int character);

std::string getLastCharacterInString(const std::string& sourceString);

std::string getAllButLastCharacterInString(const std::string& sourceString);

double calculateHitProbability(unsigned int Nh, unsigned int Nf, double alpha);

short numOfBytesInUTF8(unsigned char character);

#endif //SRC_UTILS_H
