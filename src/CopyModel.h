#ifndef SRC_COPYMODEL_H
#define SRC_COPYMODEL_H

#include <cmath>
#include <chrono>

#include "input/implementation/CopyModelInputArguments.h"
#include "reader/FileReader.h"

#include "cpm/builder/CopyModelBuilder.h"
#include "logging/Logger.h"

#include "cpm/generation/ModelGenerator.h"
#include "serialization/utils/ModelType.h"

#include "serialization/implementation/ProbabilisticModelSerializer.h"
#include "serialization/implementation/PositionalModelSerializer.h"


class CopyModel {

    public:

        CopyModel();

        void runCopyModel(const CopyModelInputArguments &inputArguments);

    private:

        FileInfo getFileInfo(const CopyModelInputArguments& inputArguments);
        FileReader getFileReaderInstance(const CopyModelInputArguments& inputArguments);

        void logCopyModelResults(CopyModelBuilder* copyModelBuilder, Logger logger);
        CopyModelBuilder* runModelBuilder(const CopyModelInputArguments& inputArguments, const FileInfo& fileInfo,
                                          const FileReader& fileReader, Logger logger);

        ModelGenerator* generateModel(const FileInfo& fileInfo, const FileReader& fileReader, Logger logger);
        void serializeModel(ModelGenerator* modelGenerator, const std::string& modelPath, const std::string& inputFilePath,
                            int generatedModelType, Logger logger);

};


#endif //SRC_COPYMODEL_H
