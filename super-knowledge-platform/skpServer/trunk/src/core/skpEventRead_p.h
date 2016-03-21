#ifndef SKPEVENTREAD_P_H
#define SKPEVENTREAD_P_H

#include "skpEventObject_p.h"
#include "skpEventRead.h"
#include "skpMutex.h"

class SkpEventReadPrivate : public SkpEventObjectPrivate
{
public:
  SkpEventReadPrivate();
  virtual ~SkpEventReadPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpEventRead);

private:
    int m_fd;
    bool m_singleShot;
    bool m_isET;
    std::function<void(bool)> m_func;

};

#endif // SKPEVENTREAD_P_H
