#ifndef SRC_COPYMODELOUTPUTWRITER_H
#define SRC_COPYMODELOUTPUTWRITER_H


#include "../../models/CopyModelOutput.h"
#include "Writer.h"

class CopyModelOutputWriter : public Writer {

    CopyModelOutput* copyModelOutput;

public:

    explicit CopyModelOutputWriter(std::string outputPath, CopyModelOutput* copyModelOutput);

    void write();

};


#endif
