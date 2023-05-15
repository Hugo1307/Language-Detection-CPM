#ifndef SRC_FILEINFOREADER_H
#define SRC_FILEINFOREADER_H

#include <set>
#include <map>
#include <string>
#include <fstream>

#include "Reader.h"

// TODO: Change this to be capable of reading the file and retrieving the necessary metrics
class FileInfoReader : public Reader {

    std::set<char> alphabet;
    int size{};

    public:

        explicit FileInfoReader(const std::string& filePath);

        void obtainMetrics();

        std::set<char> getAlphabet();
        [[nodiscard]] int getSize() const;

};


#endif //SRC_FILEINFOREADER_H
