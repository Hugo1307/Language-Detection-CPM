#ifndef TAI_GROUP_6_LANGINPUTARGUMENTS_H
#define TAI_GROUP_6_LANGINPUTARGUMENTS_H

#include <string>
#include <iostream>


class LangInputArguments {

private:

    std::string referenceFilePath{};
    std::string targetFilePath{};
    std::string outputFilePath{};

    // Copy Model

    double alpha = 0.5;
    int k = 3;
    double threshold = 0.4;
    // inputFilePath -> Same as referenceFilePath
    std::string outputModelPath{};

public:

    LangInputArguments();

    std::string getReferenceFilePath();
    std::string getTargetFilePath();
    std::string getOutputFilePath();

    double getAlpha() const;
    int getK() const;
    double getThreshold() const;
    std::string getOutputModelPath();

    void parseArguments(int argc, char *argv[]);

    [[nodiscard]] bool checkArguments() const;

    static void printUsage();

};


#endif
