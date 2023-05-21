#include "args/LangInputArguments.h"
#include "cpm/ModelGenerator.h"
#include "cpm/CopyModelExecutor.h"

#include "IO/reader/CopyModelReader.h"
#include "IO/reader/FileInfoReader.h"
#include "IO/reader/RandomAccessReader.h"
#include "IO/reader/ReferenceReader.h"
#include "IO/writer/CopyModelOutputWriter.h"
#include "models/GeneratedModel.h"

GeneratedModel obtainModel(LangInputArguments* arguments);
FileInfoReader obtainFileInfo(LangInputArguments* arguments);
CopyModelOutput runCopyModel(LangInputArguments* arguments, FileInfoReader* fileInfoReader, GeneratedModel* model);
void storeCopyModelOutput(LangInputArguments* arguments, CopyModelOutput* output);
void printCopyModelOutput(CopyModelOutput* output);

int main(int argc, char **argv) {

    // Read command line arguments
    LangInputArguments allArguments = LangInputArguments();
    allArguments.parseArguments(argc, argv);

    if (!allArguments.checkArguments()) {
        std::cerr << "Invalid arguments" << std::endl;
        return EXIT_FAILURE;
    }

    // Obtain the model for the reference file
    GeneratedModel model = obtainModel(&allArguments);

    // Obtain information for target File
    FileInfoReader fileInfoReader = obtainFileInfo(&allArguments);

    // Run the Copy Model and obtain the output
    CopyModelOutput copyModelOutput = runCopyModel(&allArguments, &fileInfoReader, &model);

    // Write the Copy Model output
    storeCopyModelOutput(&allArguments, &copyModelOutput);

    // Print the Copy Model Results
    printCopyModelOutput(&copyModelOutput);

    return EXIT_SUCCESS;

}

GeneratedModel obtainModel(LangInputArguments* arguments) {

    ReferenceReader reader = ReferenceReader(arguments->getReferenceFilePath(), arguments->getK());
    ModelGenerator copyModelGenerator = ModelGenerator(&reader, arguments->getOutputModelPath());

//    if (copyModelGenerator.isCached()) {
//        std::cout << "[!] Loading cached model" << std::endl;
//        copyModelGenerator.load();
//    } else {
//        std::cout << "[!] Generating model" << std::endl;
//        copyModelGenerator.run();
//        copyModelGenerator.save();
//    }

    std::cout << "[!] Generating model" << std::endl;
    copyModelGenerator.run();

    return {arguments->getReferenceFilePath(), copyModelGenerator.getPositionalModel(),
            copyModelGenerator.getFiniteContextModel()};

}

FileInfoReader obtainFileInfo(LangInputArguments* arguments) {

    FileInfoReader fileInfoReader = FileInfoReader(arguments->getTargetFilePath());

    fileInfoReader.openFile();
    fileInfoReader.obtainMetrics();

    std::cout << "Alphabet is" << std::endl;
    for (auto const& x : fileInfoReader.getAlphabet()) {
        std::cout << x << std::endl;
    }

    fileInfoReader.closeFile();

    return fileInfoReader;

}

CopyModelOutput runCopyModel(LangInputArguments* arguments, FileInfoReader* fileInfoReader, GeneratedModel* model) {

    // Todo: Run the model over the target file
    auto* copyModelReader = new CopyModelReader(arguments->getTargetFilePath(), arguments->getK());
    auto* randomAccessReader = new RandomAccessReader(arguments->getReferenceFilePath());

    copyModelReader->openFile();
    randomAccessReader->openFile();

    CopyModelExecutor copyModelExecutor = CopyModelExecutor(copyModelReader, fileInfoReader,
                                                            randomAccessReader, model);

    std::cout << "[!] Running Copy Model" << std::endl;

    // Run the Copy Model on the target using the Model trained for the reference
    copyModelExecutor.run(arguments->getAlpha(), arguments->getThreshold(),
                          arguments->getUseFiniteContext());

    copyModelReader->closeFile();
    randomAccessReader->closeFile();

    return copyModelExecutor.generateOutput();

}

void storeCopyModelOutput(LangInputArguments* arguments, CopyModelOutput* output) {

    CopyModelOutputWriter copyModelWriter = CopyModelOutputWriter(arguments->getOutputFilePath(),
                                                                  output);

    copyModelWriter.openFile();
    copyModelWriter.write();
    copyModelWriter.closeFile();

}

void printCopyModelOutput(CopyModelOutput* output) {

    std::cout << "[!] Finished Copy Model Execution" << std::endl;
    std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
    std::cout << "Results: " << std::endl;
    std::cout << std::endl;
    std::cout << "  Reference: " << output->getReferencePath() << std::endl;
    std::cout << "  Target: " << output->getTargetPath() << std::endl;
    std::cout << "  Total Information: " << output->getTotalInformationAmount() << std::endl;
    std::cout << "  Information per symbol: " << output->getInformationPerSymbol() << std::endl;
    std::cout << "  Information per iteration: " << std::endl;
    output->presentInformationPerIteration(true);
    std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;

}

/* Reference Path
 * Target Path
 * Output Path
 * Copy Model Alpha
 * Copy Model Threshold
 * Copy Model Window Size
 * Copy Model Output
 */

/* Output Path:
 *
 * Reference:
 * Target:
 * Total Information:
 * Information per iteration:
 */