#ifndef SKPEVENTWRITE_P_H
#define SKPEVENTWRITE_P_H

#include "skpEventObject_p.h"
#include "skpEventWrite.h"
#include "skpMutex.h"

class SkpEventWritePrivate : public SkpEventObjectPrivate
{
public:
  SkpEventWritePrivate();
  virtual ~SkpEventWritePrivate();
private:
  SKP_DECLARE_PUBLIC(SkpEventWrite);

private:
    int m_fd;
    bool m_singleShot;
    bool m_isET;
    std::function<void(bool)> m_func;
};

#endif // SKPEVENTWRITE_P_H
