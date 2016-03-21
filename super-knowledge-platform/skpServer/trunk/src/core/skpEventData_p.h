#ifndef SKPEVENTDATA_P_H
#define SKPEVENTDATA_P_H

#include "skpEventObject_p.h"
#include "skpEventData.h"
#include "skpMutex.h"

class SkpEventDataPrivate : public SkpEventObjectPrivate
{
public:
  SkpEventDataPrivate();
  virtual ~SkpEventDataPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpEventData);
private:
  std::function<void()> m_func;
  int m_count;
  SkpMutex m_mutex;
};

#endif // SKPEVENTDATA_P_H
