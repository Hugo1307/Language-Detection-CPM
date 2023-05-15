#ifndef SRC_IOENTITY_H
#define SRC_IOENTITY_H


class IOEntity {

public:

    IOEntity()= default;
    virtual ~IOEntity()= default;

    virtual void openFile() = 0;
    virtual bool isFileOpen() = 0;
    virtual void closeFile() = 0;

};


#endif
