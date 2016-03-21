#ifndef SKPOBJECT_H
#define SKPOBJECT_H

#include "skpObjectData.h"

class SkpObjectPrivate;
class SkpThread;
class SkpMutex;


class SkpObject : public SkpObjectData
{
public:
    SkpObject(SkpObject *parent = 0);
    SkpObject(SkpThread &thread);
    virtual ~SkpObject();
protected:
    SkpObject(SkpObjectPrivate &d, SkpObject *parent = 0);
    SkpObject(SkpObjectPrivate &d, SkpThread &thread);
private:
    SKP_DECLARE_PRIVATE(SkpObject);
public:
    int objectCount();
    int objectCountIncr();
    int objectCountDecr();
    int objectCountIncr_atomic();
    int objectCountDecr_atomic();
    SkpMutex &objectCountMutex();
    SkpObject *parent();
    void init(SkpObject *parent, SkpThread *thread);
    void destroy();
    void setParent(SkpObject *parent);
    void removeChilden(SkpObject *childen);
    void addChilden(SkpObject *childen);
    void clearChilden();

    SkpThread *thread();
    virtual void deleteLater();
    bool isDeleteLater();
};

#endif // SKPOBJECT_H
