#ifndef TAI_GROUP_6_LANGINPUTARGUMENTS_H
#define TAI_GROUP_6_LANGINPUTARGUMENTS_H

#include <string>
#include <iostream>

#include "../abstract/AbstractInputArguments.h"
#include "../../utils/generalUtils.h"
#include "CopyModelInputArguments.h"


class LangInputArguments : public AbstractInputArguments {

    std::string inputFilePath{};

    // TODO: Check if redundant
    std::string referenceFilePath{}; // Same as the target file of the copy model
    std::string outputModelPath{}; // Same as the output file of the copy model

    // Parameters for the copy model
    CopyModelInputArguments copyModelInputArguments;

    public:

        LangInputArguments();

        std::string getReferenceFilePath();
        std::string getInputFilePath();
        std::string getOutputModelPath();

        CopyModelInputArguments getCopyModelInputArguments();

        void parseArguments(int argc, char *argv[]) override;

        [[nodiscard]] bool checkArguments() const override;

        static void printUsage();

};


#endif //TAI_GROUP_6_LANGINPUTARGUMENTS_H
