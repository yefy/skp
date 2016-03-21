#include "skp_test_socket_tcp.h"
#include "skpSocketTcp_p.h"
#include "skpTcpStream.h"

//==================SkpTestSocketTcpPrivate
class SkpTestSocketTcpPrivate : public SkpSocketTcpPrivate
{
public:
  SkpTestSocketTcpPrivate();
  virtual ~SkpTestSocketTcpPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpTestSocketTcp);

private:
};

SkpTestSocketTcpPrivate::SkpTestSocketTcpPrivate() :
    SkpSocketTcpPrivate()
{

}

SkpTestSocketTcpPrivate::~SkpTestSocketTcpPrivate()
{
}

//===================SkpTestSocketTcp
SkpTestSocketTcp::SkpTestSocketTcp(SkpObject *parent) :
    SkpSocketTcp(SkpSocketTcp::socket(), *new SkpTestSocketTcpPrivate(), parent)
{

}

SkpTestSocketTcp::SkpTestSocketTcp(SkpThread &thread) :
    SkpSocketTcp(SkpSocketTcp::socket(), *new SkpTestSocketTcpPrivate(), thread)
{

}

SkpTestSocketTcp::SkpTestSocketTcp(int fd, SkpObject *parent) :
    SkpSocketTcp(fd, *new SkpTestSocketTcpPrivate(), parent)
{

}

SkpTestSocketTcp::SkpTestSocketTcp(int fd, SkpThread &thread) :
    SkpSocketTcp(fd, *new SkpTestSocketTcpPrivate(), thread)
{

}

SkpTestSocketTcp::SkpTestSocketTcp(int fd, SkpTestSocketTcpPrivate &d, SkpObject *parent) :
    SkpSocketTcp(fd, d, parent)
{

}

SkpTestSocketTcp::SkpTestSocketTcp(int fd, SkpTestSocketTcpPrivate &d, SkpThread &thread) :
    SkpSocketTcp(fd, d, thread)
{

}


SkpTestSocketTcp::~SkpTestSocketTcp()
{

}


void SkpTestSocketTcp::onLocalReadStream(int size, SkpSocketStreamTcp *tcpStreamRead)
{
    printf("onLocalReadStream size = %d \n", size);
    if(size <= SOCKET_BUFFER_TIMEOUT) {
        deleteLater();
        return;
    }

    tcpStreamRead->skip(size);
}
