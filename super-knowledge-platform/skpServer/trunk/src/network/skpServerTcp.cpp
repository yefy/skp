#include "skpServerTcp.h"
#include "skpServerTcp_p.h"
#include "skpEventRead.h"
#include "skpSocketTcp.h"
#include "skpAutoFree.h"
#include "skpLog.h"
#include "skpThread.h"

//===============SkpServerTcpPrivate
SkpServerTcpPrivate::SkpServerTcpPrivate() :
    SkpObjectDataPrivate(),
    m_fd(-1),
    m_eventRead(skp_null),
    m_func(skp_null)
{

}

SkpServerTcpPrivate::~SkpServerTcpPrivate()
{

}

//===============SkpServerTcp
SkpServerTcp::SkpServerTcp() :
    SkpObjectData(*new SkpServerTcpPrivate())
{
}

SkpServerTcp::SkpServerTcp(SkpServerTcpPrivate &d) :
    SkpObjectData(d)
{
}

SkpServerTcp::~SkpServerTcp()
{
    SKP_D(SkpServerTcp);
    skp_delete(skpD->m_eventRead);
    if(skpD->m_fd != -1) {
        ::close(skpD->m_fd);
        skpD->m_fd = -1;
    }
}

int SkpServerTcp::listen(uint16 port)
{
    struct sockaddr_in addr;
    bzero(&addr,sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //addr_in.sin_addr.s_addr = inet_addr("127.0.0.1");

    return listen(&addr);
}

int SkpServerTcp::listen(struct sockaddr_in *addr)
{
    SKP_D(SkpServerTcp);

    skpD->m_fd = SkpSocketTcp::socket();
    ::setNoblock(skpD->m_fd);

    skpD->m_addr = *addr;
    struct linger ling = {0, 0};
    int error = 0;
    int flags =1;

    error = setsockopt(skpD->m_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&flags, sizeof(flags));
    if (error != 0) {
        skpLogError_g("setsockopt SO_REUSEADDR\n");
        return skp_false;
    }

    error = setsockopt(skpD->m_fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&flags, sizeof(flags));
    if (error != 0) {
        skpLogError_g("setsockopt SO_KEEPALIVE\n");
        return skp_false;
    }

    error = setsockopt(skpD->m_fd, SOL_SOCKET, SO_LINGER, (void *)&ling, sizeof(ling));
    if (error != 0) {
        skpLogError_g("setsockopt SO_LINGER");
        return skp_false;
    }

    error = setsockopt(skpD->m_fd, IPPROTO_TCP, TCP_NODELAY, (void *)&flags, sizeof(flags));
    if (error != 0) {
        skpLogError_g("setsockopt TCP_NODELAY");
        return skp_false;
    }

    if(::bind(skpD->m_fd,(struct sockaddr*)&skpD->m_addr,sizeof(struct sockaddr)) < 0)
    {

        if (errno != EADDRINUSE) {
            skpLogError_g("Address already in use\n");
        } else {
            skpLogError_g("bind failed\n");
        }
        SKP_ASSERT(skp_false);
        return skp_false;
    } else {
        if (::listen(skpD->m_fd, /*SOMAXCONN*/511) < 0) {
            skpLogError_g("listen failed\n");
            return skp_false;
        }
    }

    return skp_true;
}

void SkpServerTcp::start(std::function<void(int)> func)
{
    SKP_D(SkpServerTcp);
    skpD->m_func = func;
    if(!skpD->m_eventRead) {
        SkpThread *thread = SkpThread::currThread();
        skpD->m_eventRead = new SkpEventRead(*thread);
        skpD->m_eventRead->connect(skpD->m_fd, std::bind(&SkpServerTcp::onAccept, this, std::placeholders::_1));
        skpD->m_eventRead->setET(skp_false);
    }
    skpD->m_eventRead->start(0);
}

void SkpServerTcp::stop()
{
    SKP_D(SkpServerTcp);
    skpD->m_eventRead->stop();
}

void SkpServerTcp::free()
{
    SKP_D(SkpServerTcp);
    skpD->m_eventRead->free();
}

void SkpServerTcp::onAccept(bool isTimeOut)
{
    SKP_D(SkpServerTcp);
    if(isTimeOut) {
        skpLogError_g("skp_accept timeout\n ");
        return;
    }
    struct sockaddr addr;
    socklen_t addrlen = sizeof(struct sockaddr);
    bool stop = skp_false;
    SKP_UNUSED(stop);

    int fd;

    if ((fd = ::accept(skpD->m_fd, (struct sockaddr *)&addr, &addrlen)) < 0) {
        removeReadReady();
    } else {
        skpD->m_func(fd);
    }
}

void SkpServerTcp::removeReadReady()
{
    SKP_D(SkpServerTcp);
    skpD->m_eventRead->removeReadReady();
}
