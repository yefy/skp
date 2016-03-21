#ifndef SKPEVENTOBJECT_P_H
#define SKPEVENTOBJECT_P_H

#include "skpObject_p.h"
#include "skpEventObject.h"

enum EventType
{
    EventTypeInit = 0,
    EventTypeStart,
    EventTypeStop,
    EventTypeFree,
};

class SkpEventObjectPrivate : public SkpObjectPrivate
{
public:
  SkpEventObjectPrivate();
  virtual ~SkpEventObjectPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpEventObject);
private:
    SkpMutex m_mutex;
    std::vector<int> m_in;
    std::vector<int> m_out;
protected:
    int m_currType;
    void *m_data;
    int64 m_time;
};

#endif // SKPEVENTOBJECT_P_H
