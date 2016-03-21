#ifndef SKPSIGNAL_P_H
#define SKPSIGNAL_P_H

#include "skpObjectData_p.h"
#include "skpSignal.h"
#include "skpPipe.h"

class SkpEventRead;
typedef struct SkpAddSignal_s SkpAddSignal_t;

class SkpSignalPrivate : public SkpObjectDataPrivate
{
public:
  SkpSignalPrivate();
  virtual ~SkpSignalPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpSignal);

private:
    std::map<int, SkpAddSignal_t> m_signoMap;
    SkpPipe m_pipe;
    SkpEventRead *m_eventRead;
};


#endif // SKPSIGNAL_P_H
