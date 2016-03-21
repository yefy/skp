#ifndef SKPEVENTTIMER_P_H
#define SKPEVENTTIMER_P_H

#include "skpEventObject_p.h"
#include "skpMutex.h"

class SkpTimerPrivate : public SkpEventObjectPrivate
{
public:
  SkpTimerPrivate();
  virtual ~SkpTimerPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpTimer);

private:
  bool m_singleShot;
  std::function<void()> m_func;
};

#endif // SKPEVENTTIMER_P_H
