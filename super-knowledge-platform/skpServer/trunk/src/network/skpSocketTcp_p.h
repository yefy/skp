#ifndef SKPSOCKETTCP_P_H
#define SKPSOCKETTCP_P_H

#include "skpSocket_p.h"
#include "skpSocketTcp.h"

class SkpSocketStreamTcp;
class SkpEventChan;

class SkpSocketTcpPrivate : public SkpSocketPrivate
{
public:
  SkpSocketTcpPrivate();
  virtual ~SkpSocketTcpPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpSocketTcp);

private:
    int m_epfd;
    struct sockaddr_in m_addr;
    SkpSocketStreamTcp *m_tcpStreamRead;
    SkpSocketStreamTcp *m_tcpStreamWrite;
    int m_timeoutRead;
    int m_timeoutWrite;
    std::function<void(int, SkpSocketStreamTcp *)> m_funcRead;
    std::function<void(int, SkpSocketStreamTcp *)> m_funcWrite;
    bool m_isRemoveReadReady;
    SkpEventChan *m_tcpChanWrite;
};

#endif // SKPSOCKETTCP_P_H
