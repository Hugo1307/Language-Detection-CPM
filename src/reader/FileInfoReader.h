#ifndef SRC_FILEINFOREADER_H
#define SRC_FILEINFOREADER_H

#include <set>
#include <map>
#include <string>
#include <fstream>

// TODO: Change this to be capable of reading the file and retrieving the necessary metrics
class FileInfoReader {

    std::string filePath;
    std::ifstream fileInputStream;

    std::set<char> alphabet;
    std::map<char, int> symbolsCount;
    int size{};

    public:

        explicit FileInfoReader(const std::string& filePath);

        void openFile();
        bool isFileOpen();
        void closeFile();

        void obtainMetrics();

        std::set<char> getAlphabet();
        std::map<char, int> getSymbolsCount();
        std::map<char, double> getSymbolsDistribution();
        [[nodiscard]] int getSize() const;
        [[nodiscard]] std::string getFilePath() const;

};


#endif //SRC_FILEINFOREADER_H
