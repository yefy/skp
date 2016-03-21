#ifndef SKPEVENTWRITE_H
#define SKPEVENTWRITE_H

#include "skpEventObject.h"

class SkpEventWritePrivate;

class SkpEventWrite : public SkpEventObject
{
public:
    SkpEventWrite(SkpObject *parent = 0);
    SkpEventWrite(SkpThread &thread);
    virtual ~SkpEventWrite();
protected:
    SkpEventWrite(SkpEventWritePrivate &d, SkpObject *parent = 0);
    SkpEventWrite(SkpEventWritePrivate &d, SkpThread &thread);
private:
    SKP_DECLARE_PRIVATE(SkpEventWrite);
public:
    void connect(int fd, std::function<void(bool)> func);
    void setSingleShot(bool singleShot);
    void setET(bool isET);
    void start(int msec);
    void stop();
    void free();
    virtual void *registerEvent();
    void onData(short event);
};

#endif // SKPEVENTWRITE_H
