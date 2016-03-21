#ifndef SKPSERVERTCP_P_H
#define SKPSERVERTCP_P_H

#include "skpObjectData_p.h"
#include "skpServerTcp.h"

class SkpEventRead;

class SkpServerTcpPrivate : public SkpObjectDataPrivate
{
public:
  SkpServerTcpPrivate();
  virtual ~SkpServerTcpPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpServerTcp);

private:
  int m_fd;
  SkpEventRead *m_eventRead;
  struct sockaddr_in m_addr;
  std::function<void(int)> m_func;
};

#endif // SKPSERVERTCP_P_H
