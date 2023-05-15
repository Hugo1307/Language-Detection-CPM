#include "input/LangInputArguments.h"
#include "cpm/ModelGenerator.h"
#include "cpm/CopyModelExecutor.h"

#include "IO/reader/CopyModelReader.h"
#include "IO/reader/FileInfoReader.h"
#include "IO/reader/RandomAccessReader.h"
#include "IO/reader/ReferenceReader.h"
#include "IO/writer/CopyModelOutputWriter.h"

std::map<std::string, std::vector<int>> obtainModel(LangInputArguments* arguments);

int main(int argc, char **argv) {

    // Read command line arguments
    LangInputArguments allArguments = LangInputArguments();
    allArguments.parseArguments(argc, argv);

    if (!allArguments.checkArguments()) {
        std::cerr << "Invalid arguments" << std::endl;
        return EXIT_FAILURE;
    }

    // Obtain the model for the reference file
    std::map<std::string, std::vector<int>> model = obtainModel(&allArguments);

    // Todo: Run the model over the target file
    CopyModelReader copyModelReader = CopyModelReader(allArguments.getTargetFilePath(), allArguments.getK());
    FileInfoReader fileInfoReader = FileInfoReader(allArguments.getTargetFilePath());
    RandomAccessReader randomAccessReader = RandomAccessReader(allArguments.getReferenceFilePath());

    // Obtain File Information - Alphabet and File Size
    fileInfoReader.openFile();
    fileInfoReader.obtainMetrics();
    fileInfoReader.closeFile();

    copyModelReader.openFile();
    randomAccessReader.openFile();

    CopyModelExecutor copyModelExecutor = CopyModelExecutor(&copyModelReader, &fileInfoReader,
                                                            &randomAccessReader, model);

    // Run the Copy Model on the target using the Model trained for the reference
    copyModelExecutor.run();

    copyModelReader.closeFile();
    randomAccessReader.closeFile();

    CopyModelOutput copyModelOutput = copyModelExecutor.generateOutput();

    CopyModelOutputWriter copyModelWriter = CopyModelOutputWriter(allArguments.getOutputFilePath(),
                                                                  &copyModelOutput);

    copyModelWriter.openFile();
    copyModelWriter.write();
    copyModelWriter.closeFile();

    return EXIT_SUCCESS;

}

std::map<std::string, std::vector<int>> obtainModel(LangInputArguments* arguments) {

    ReferenceReader reader = ReferenceReader(arguments->getReferenceFilePath(), arguments->getK());
    ModelGenerator copyModelGenerator = ModelGenerator(&reader, arguments->getOutputModelPath());

    if (copyModelGenerator.isCached()) {
        std::cout << "[!] Loading cached model" << std::endl;
        copyModelGenerator.load();
    } else {
        std::cout << "[!] Generating model" << std::endl;
        copyModelGenerator.run();
        copyModelGenerator.save();
    }

    return copyModelGenerator.getModel();

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