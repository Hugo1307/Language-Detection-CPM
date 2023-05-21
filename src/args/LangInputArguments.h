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

    bool useFiniteContext = true;

public:

    LangInputArguments();

    std::string getReferenceFilePath();
    std::string getTargetFilePath();
    std::string getOutputFilePath();

    [[nodiscard]] double getAlpha() const;
    [[nodiscard]] int getK() const;
    [[nodiscard]] double getThreshold() const;
    std::string getOutputModelPath();
    bool getUseFiniteContext() const;

    void parseArguments(int argc, char *argv[]);

    [[nodiscard]] bool checkArguments() const;

    static void printUsage();

};


#endif
