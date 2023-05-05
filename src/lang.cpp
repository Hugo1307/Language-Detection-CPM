#include "input/LangInputArguments.h"

int main(int argc, char **argv) {

    // Read command line arguments
    LangInputArguments allArguments = LangInputArguments();
    allArguments.parseArguments(argc, argv);



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
     * /

    bool areArgumentsValid = allArguments.checkArguments();

    if (!areArgumentsValid) {
        LangInputArguments::printUsage();
        std::exit(EXIT_FAILURE);
    }

    // TODO: Check if copy model was already computed for current reference

    // Obtain copy model input arguments
    CopyModelInputArguments inputArguments = allArguments.getCopyModelInputArguments();

    // Run copy model
    CopyModel copyModel = CopyModel();
    copyModel.runCopyModel(inputArguments);

    // Load computed copy model (Model Type is always positional)
    PositionalModelSerializer sequentialModelSerializer = PositionalModelSerializer(allArguments.getOutputModelPath());
    sequentialModelSerializer.loadModel();

    // TODO: Calculate the estimated number of bits required to compress the input file, using the model computed

    return EXIT_SUCCESS;
}
