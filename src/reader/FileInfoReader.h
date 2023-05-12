#ifndef SRC_FILEINFOREADER_H
#define SRC_FILEINFOREADER_H

#include <set>
#include <map>
#include <string>

class FileInfoReader {

    std::string filePath;
    std::set<char> alphabet;
    std::map<char, int> symbolsCount;
    int size;

    public:

        explicit FileInfoReader(const std::string& filePath, std::set<char> alphabet, std::map<char, int> symbolsCount, int size);

        std::set<char> getAlphabet();
        std::map<char, int> getSymbolsCount();
        std::map<char, double> getSymbolsDistribution();
        [[nodiscard]] int getSize() const;
        [[nodiscard]] std::string getFilePath() const;

};


#endif //SRC_FILEINFOREADER_H
