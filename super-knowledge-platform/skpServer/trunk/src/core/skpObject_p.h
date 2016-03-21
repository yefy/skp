#ifndef SKPOBJECT_P_H
#define SKPOBJECT_P_H

#include "skpObjectData_p.h"
#include "skpMutex.h"

class SkpObject;
class SkpThread;


class SkpObjectPrivate : public SkpObjectDataPrivate
{
public:
  SkpObjectPrivate();
  virtual ~SkpObjectPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpObject);
private:
  SkpObject *m_parent;
  SkpMutex m_objectCountMutex;
  int m_objectCount;
  SkpThread *m_thread;
  bool m_isDeleteLater;
  std::list<SkpObject *> m_ChildenObjectList;
};



#endif // SKPOBJECT_P_H
