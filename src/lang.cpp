#include <codecvt>
#include "input/LangInputArguments.h"
#include "reader/ReferenceReader.h"
#include "cpm/ModelGenerator.h"
#include "reader/CopyModelReader.h"
#include "reader/RandomAccessReader.h"
#include "reader/FileInfoReader.h"

std::map<std::string, std::vector<int>> obtainModel(LangInputArguments* arguments);

int main(int argc, char **argv) {

    // Read command line arguments
    LangInputArguments allArguments = LangInputArguments();
    allArguments.parseArguments(argc, argv);

    // Obtain the model for the reference file
    std::map<std::string, std::vector<int>> model = obtainModel(&allArguments);

    // Todo: Run the model over the target file
    CopyModelReader reader = CopyModelReader(allArguments.getTargetFilePath(), allArguments.getK());
    FileInfoReader fileInfoReader = FileInfoReader(allArguments.getTargetFilePath(), allArguments.getK());
    RandomAccessReader randomAccessReader = RandomAccessReader(allArguments.getReferenceFilePath());


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