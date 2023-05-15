#include <codecvt>
#include "input/LangInputArguments.h"
#include "reader/ReferenceReader.h"
#include "cpm/ModelGenerator.h"
#include "reader/CopyModelReader.h"
#include "reader/RandomAccessReader.h"
#include "reader/FileInfoReader.h"
#include "cpm/CopyModelExecutor.h"

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

    fileInfoReader.openFile();
    fileInfoReader.obtainMetrics();
    fileInfoReader.closeFile();


    copyModelReader.openFile();
    randomAccessReader.openFile();

    CopyModelExecutor copyModelExecutor = CopyModelExecutor(&copyModelReader, &fileInfoReader,
                                                            &randomAccessReader, model);

    copyModelExecutor.run();

    std::cout << copyModelExecutor.getInformationAmount() << std::endl;

    copyModelReader.closeFile();
    randomAccessReader.closeFile();


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