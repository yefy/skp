#include "skp_test_application.h"
#include "skpApplication_p.h"
#include "skpServerTcp.h"
#include "skpAutoFree.h"
#include "skp_test_socket_tcp.h"

//=============SkpTestApplicationPrivate


class SkpTestApplicationPrivate : public SkpApplicationPrivate
{
public:
  SkpTestApplicationPrivate();
  virtual ~SkpTestApplicationPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpTestApplication);

private:
  SkpServerTcp *serverTcp;
};

SkpTestApplicationPrivate::SkpTestApplicationPrivate() :
    SkpApplicationPrivate(),
    serverTcp(skp_null)
{

}

SkpTestApplicationPrivate::~SkpTestApplicationPrivate()
{
}

//===================SkpLocalApplication
SkpTestApplication::SkpTestApplication(int argc, char **argv) :
    SkpApplication(*new SkpTestApplicationPrivate(), argc, argv)
{
    listen();
}

SkpTestApplication::~SkpTestApplication()
{
    SKP_D(SkpTestApplication);
    skp_delete(skpD->serverTcp);
}

void SkpTestApplication::listen()
{
    printf("listen \n");
    SKP_D(SkpTestApplication);
    skpD->serverTcp = new SkpServerTcp();
    skpD->serverTcp->listen(33331);
}


void SkpTestApplication::start()
{
    printf("start \n");
    SKP_D(SkpTestApplication);
    skpD->serverTcp->start(std::bind(&SkpTestApplication::onAccept, this, std::placeholders::_1));
}

void SkpTestApplication::onAccept(int fd)
{
    printf("fd = %d \n", fd);
    SkpTestSocketTcp *testSocketTcp = new SkpTestSocketTcp(fd, *(SkpThread *)this);
    testSocketTcp->startReadStream(10000, std::bind(&SkpTestSocketTcp::onLocalReadStream, testSocketTcp, std::placeholders::_1, std::placeholders::_2));
}
