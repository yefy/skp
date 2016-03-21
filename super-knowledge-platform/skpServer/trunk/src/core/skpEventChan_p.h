#ifndef SKPEVENTCHAN_P_H
#define SKPEVENTCHAN_P_H

#include "skpEventData_p.h"
#include "skpEventChan.h"

class SkpListEx;

class SkpEventChanPrivate : public SkpEventDataPrivate
{
public:
  SkpEventChanPrivate();
  virtual ~SkpEventChanPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpEventChan);

private:
  std::function<bool(void *, int, void *, int)> m_func;

  SkpMutex m_eventListInMutex;
  SkpListEx *m_eventListIn;
  SkpListEx *m_eventListOut;

  int m_max;
};

#endif // SKPEVENTCHAN_P_H
