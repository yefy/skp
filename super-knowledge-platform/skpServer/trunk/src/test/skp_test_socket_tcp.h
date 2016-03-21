#ifndef SKP_TEST_SOCKET_TCP_H
#define SKP_TEST_SOCKET_TCP_H

#include "skpSocketTcp.h"

class SkpTestSocketTcpPrivate;

class SkpTestSocketTcp : public SkpSocketTcp
{
public:
    SkpTestSocketTcp(SkpObject *parent = 0);
    SkpTestSocketTcp(SkpThread &thread);
    SkpTestSocketTcp(int fd, SkpObject *parent = 0);
    SkpTestSocketTcp(int fd, SkpThread &thread);
    virtual ~SkpTestSocketTcp();
protected:
    SkpTestSocketTcp(int fd, SkpTestSocketTcpPrivate &d, SkpObject *parent = 0);
    SkpTestSocketTcp(int fd, SkpTestSocketTcpPrivate &d, SkpThread &thread);
private:
    SKP_DECLARE_PRIVATE(SkpTestSocketTcp);
public:
    void onLocalReadStream(int size, SkpSocketStreamTcp *tcpStreamRead);
};

#endif // SKP_TEST_SOCKET_TCP_H
