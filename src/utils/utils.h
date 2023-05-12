#ifndef SRC_UTILS_H
#define SRC_UTILS_H

#include <string>

std::string getFileNameFromPath(const std::string& path);

std::string convertCharArrToString(char* charArr, int size);

bool isWhiteLineCharacter(int character);

bool isForbiddenCharacter(int character);

#endif //SRC_UTILS_H
