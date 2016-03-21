#ifndef SKPSOCKET_P_H
#define SKPSOCKET_P_H

#include "skpObject_p.h"
#include "skpSocket.h"


class SkpEventRead;
class SkpEventWrite;

class SkpSocketPrivate : public SkpObjectPrivate
{
public:
  SkpSocketPrivate();
  virtual ~SkpSocketPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpSocket);

private:
  int m_fd;
  SkpEventRead *m_eventRead;
  SkpEventWrite *m_eventWrite;
  bool m_isET;
};

#endif // SKPSOCKET_P_H
