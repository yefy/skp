#ifndef SKPOBJECTDATA_P_H
#define SKPOBJECTDATA_P_H

#include "skpGlobal.h"
#include "skpUtility.h"

class SkpObjectData;

class SkpObjectDataPrivate
{
public:
  SkpObjectDataPrivate();
  virtual ~SkpObjectDataPrivate();
protected:
  SkpObjectData *q_ptr;
private:
  SKP_DECLARE_PUBLIC(SkpObjectData);
};


#endif // SKPOBJECTDATA_P_H
