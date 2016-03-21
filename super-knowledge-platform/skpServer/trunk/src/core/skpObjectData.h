#ifndef SKPOBJECTDATA_H
#define SKPOBJECTDATA_H

#include "skpGlobal.h"
#include "skpUtility.h"

class SkpObjectDataPrivate;

class SkpObjectData
{
protected:
    SkpObjectData(SkpObjectDataPrivate &d);
    virtual ~SkpObjectData();
protected:
    SkpObjectDataPrivate * const d_ptr;
private:
    SKP_DECLARE_PRIVATE(SkpObjectData);
};

#endif // SKPOBJECTDATA_H
