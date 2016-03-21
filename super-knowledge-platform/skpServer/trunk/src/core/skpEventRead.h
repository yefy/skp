#ifndef SKPEVENTREAD_H
#define SKPEVENTREAD_H

#include "skpEventObject.h"


class SkpEventReadPrivate;


class SkpEventRead : public SkpEventObject
{
public:
    SkpEventRead(SkpObject *parent = 0);
    SkpEventRead(SkpThread &thread);
    virtual ~SkpEventRead();
protected:
    SkpEventRead(SkpEventReadPrivate &d, SkpObject *parent = 0);
    SkpEventRead(SkpEventReadPrivate &d, SkpThread &thread);
private:
    SKP_DECLARE_PRIVATE(SkpEventRead);
public:
    void connect(int fd, std::function<void(bool)> func);
    void setSingleShot(bool singleShot);
    void setET(bool isET);
    void start(int msec);
    void stop();
    void free();
    virtual void *registerEvent();
    void onData(short event);
    void removeReadReady();
    void addReadReady();
};

#endif // SKPEVENTREAD_H
