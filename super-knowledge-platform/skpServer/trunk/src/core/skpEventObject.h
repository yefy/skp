#ifndef SKPEVENTOBJECT_H
#define SKPEVENTOBJECT_H

#include "skpObject.h"

class SkpEventObjectPrivate;


class SkpEventObject : public SkpObject
{
public:
    SkpEventObject(SkpObject *parent = 0);
    SkpEventObject(SkpThread &thread);
    virtual ~SkpEventObject();
protected:
    SkpEventObject(SkpEventObjectPrivate &d, SkpObject *parent = 0);
    SkpEventObject(SkpEventObjectPrivate &d, SkpThread &thread);
private:
    SKP_DECLARE_PRIVATE(SkpEventObject);
public:
    static void onCallback(int fd, short event, void *arg);
    virtual void onData(short event);
    void addEvent(int type);
    virtual void onEvent();
    void doEvent(int type);
    virtual void *registerEvent(){ return skp_null; }
};

#endif // SKPEVENTOBJECT_H
