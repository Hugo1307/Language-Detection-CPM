#include "CopyModel.h"

CopyModel::CopyModel() = default;

void CopyModel::runCopyModel(const CopyModelInputArguments& inputArguments) {

    // First Pass: Get File Info (Alphabet and size)
    FileInfo fileInfo = getFileInfo(inputArguments);

    // Initialize logger
    Logger logger = Logger();
    logger.setLevel(inputArguments.getLoggingLevel());

    // Get File Reader instance for second pass
    FileReader fileReader = getFileReaderInstance(inputArguments);

    std::chrono::steady_clock::time_point modelBuildingStart = std::chrono::steady_clock::now();

    if (inputArguments.shouldCalculateInformation()) {

        CopyModelBuilder* copyModelBuilder = runModelBuilder(inputArguments, fileInfo, fileReader, logger);
        logCopyModelResults(copyModelBuilder, logger);

        delete copyModelBuilder;
    }

    std::chrono::steady_clock::time_point modelBuildingEnd = std::chrono::steady_clock::now();

    logger.info("Model Building Time: " + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(modelBuildingEnd - modelBuildingStart).count()) + "ms");

    std::chrono::steady_clock::time_point modelSerializeStart = std::chrono::steady_clock::now();

    if (inputArguments.shouldSerializeForGenerator()) {

        logger.info("[!] Generating a model to be used in Generator...");

        ModelGenerator* modelGenerator = generateModel(fileInfo, fileReader, logger);

        serializeModel(modelGenerator, inputArguments.getOutputModelPath(),
                       inputArguments.getInputFilePath(),inputArguments.getSerializerType(),
                       logger);

        delete modelGenerator;

        logger.info("[!] Model Serialized to: " + inputArguments.getOutputModelPath());

    }

    std::chrono::steady_clock::time_point modelSerializeEnd = std::chrono::steady_clock::now();

    logger.info("Model Serialization Time: " + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(modelSerializeEnd - modelSerializeStart).count()) + "ms");
}

CopyModelBuilder* CopyModel::runModelBuilder(const CopyModelInputArguments& inputArguments, const FileInfo& fileInfo,
                                  const FileReader& fileReader, Logger logger) {

    auto* copyModelBuilder = new CopyModelBuilder(fileReader, fileInfo);
    copyModelBuilder->setLogger(logger);

    copyModelBuilder->buildModel(inputArguments.getAlpha(), inputArguments.getThreshold());

    return copyModelBuilder;

}

void CopyModel::logCopyModelResults(CopyModelBuilder* copyModelBuilder, Logger logger) {

    logger.info("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
    logger.info("Copy Model Results");
    logger.info("");
    logger.info("Analyzed File: " + copyModelBuilder->getFileInfo().getFilePath());
    logger.info("File Size (Characters): " + std::to_string(copyModelBuilder->getFileInfo().getSize()));

    std::string alphabetStr;
    for (char characterInAlphabet : copyModelBuilder->getFileInfo().getAlphabet()) {
        alphabetStr.append(std::string(1, characterInAlphabet));
        alphabetStr.append(",");
    }

    logger.info("Alphabet: " + alphabetStr);
    logger.info("");
    logger.info("Total Amount of Information: " + std::to_string(copyModelBuilder->calculateTotalInformation()));
    logger.info("Information p/ character: " + std::to_string(copyModelBuilder->calculateInformationByCharacter()));
    logger.info("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
    logger.info("\n");

}

ModelGenerator* CopyModel::generateModel(const FileInfo& fileInfo, const FileReader& fileReader, Logger logger) {

    auto* modelGenerator = new ModelGenerator(fileReader, fileInfo);
    modelGenerator->setLogger(logger);

    modelGenerator->generateModel();

    return modelGenerator;

}

void CopyModel::serializeModel(ModelGenerator* modelGenerator, const std::string& modelPath, const std::string& inputFilePath,
                    int generatedModelType, Logger logger) {

    if (generatedModelType == ModelType::PROBABILISTIC) {

        logger.info("[!] Serializing PROBABILISTIC model!");

        ProbabilisticModelSerializer probabilisticModelSerializer = ProbabilisticModelSerializer(modelPath);
        probabilisticModelSerializer.setInputFilePath(inputFilePath);
        probabilisticModelSerializer.setModel(modelGenerator->getProbabilisticModel());

        probabilisticModelSerializer.outputModel();

    } else if (generatedModelType == ModelType::POSITIONAL) {

        logger.info("[!] Serializing POSITIONAL model!");

        PositionalModelSerializer positionalModelSerializer = PositionalModelSerializer(modelPath);
        positionalModelSerializer.setInputFilePath(inputFilePath);
        positionalModelSerializer.setModel(modelGenerator->getPositionModel());

        positionalModelSerializer.outputModel();

    } else {
        logger.error("[!] Invalid model type! Please check your input arguments.");
    }

}

FileInfo CopyModel::getFileInfo(const CopyModelInputArguments& inputArguments) {

    FileReader fileReader = getFileReaderInstance(inputArguments);

    fileReader.openFile();
    FileInfo fileInfo = fileReader.getFileInfo();
    fileReader.closeFile();

    return fileInfo;
}

FileReader CopyModel::getFileReaderInstance(const CopyModelInputArguments& inputArguments) {
    return FileReader(inputArguments.getInputFilePath(), inputArguments.getK());
}
