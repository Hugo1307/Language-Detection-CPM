#ifndef SRC_UTILS_H
#define SRC_UTILS_H

#include <string>
#include <vector>

std::string getFileNameFromPath(const std::string& path);

std::string convertCharArrToString(char* charArr, int size);

std::string convertVectorToString(const std::vector<char>& vector);

bool isWhiteLineCharacter(int character);

bool isForbiddenCharacter(int character);

double calculateHitProbability(unsigned int Nh, unsigned int Nf, double alpha);

#endif //SRC_UTILS_H
