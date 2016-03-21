#include "skpSocketTcp.h"
#include "skpSocketTcp_p.h"
#include "skpTcpStream.h"
#include "skpAutoFree.h"
#include "skpUtility.h"
#include "skpEventChan.h"

#include <sys/epoll.h>

//=================SkpSocketTcpPrivate
SkpSocketTcpPrivate::SkpSocketTcpPrivate() :
    SkpSocketPrivate(),
    m_epfd(-1),
    m_tcpStreamRead(skp_null),
    m_tcpStreamWrite(skp_null),
    m_timeoutRead(0),
    m_timeoutWrite(0),
    m_funcRead(skp_null),
    m_funcWrite(skp_null),
    m_isRemoveReadReady(skp_true),
    m_tcpChanWrite(skp_null)
{

}

SkpSocketTcpPrivate::~SkpSocketTcpPrivate()
{

}

//===================SkpSocketTcp

SkpSocketTcp::SkpSocketTcp(SkpObject *parent) :
    SkpSocket(SkpSocketTcp::socket(), *new SkpSocketTcpPrivate(), parent)
{
    init();
}

SkpSocketTcp::SkpSocketTcp(SkpThread &thread) :
    SkpSocket(SkpSocketTcp::socket(), *new SkpSocketTcpPrivate(), thread)
{
    init();
}

SkpSocketTcp::SkpSocketTcp(int fd, SkpObject *parent) :
    SkpSocket(fd, *new SkpSocketTcpPrivate(), parent)
{
    init();
}

SkpSocketTcp::SkpSocketTcp(int fd, SkpThread &thread) :
    SkpSocket(fd, *new SkpSocketTcpPrivate(), thread)
{
    init();
}

SkpSocketTcp::SkpSocketTcp(int fd, SkpSocketTcpPrivate &d, SkpObject *parent) :
    SkpSocket(fd, d, parent)
{
    init();
}

SkpSocketTcp::SkpSocketTcp(int fd, SkpSocketTcpPrivate &d, SkpThread &thread) :
    SkpSocket(fd, d, thread)
{
    init();
}

SkpSocketTcp::~SkpSocketTcp()
{
    SKP_D(SkpSocketTcp);
    skp_delete(skpD->m_tcpStreamRead);
    skp_delete(skpD->m_tcpStreamWrite);
    ::close(skpD->m_epfd);
}

void SkpSocketTcp::init()
{
    acquire();
    SKP_D(SkpSocketTcp);
    skpD->m_tcpStreamRead = new SkpSocketStreamTcp(socketDescriptor());
    skpD->m_tcpStreamWrite = new SkpSocketStreamTcp(socketDescriptor());
    skpD->m_tcpChanWrite = new SkpEventChan(this);
    skpD->m_tcpChanWrite->connect(std::bind(&SkpSocketTcp::onChanWrite, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}

int SkpSocketTcp::socket()
{
    int fd = ::socket(AF_INET,SOCK_STREAM,0);
    SKP_ASSERT(fd >= 0);
    return fd;
}

int SkpSocketTcp::connect(const char *ip, uint16 port)
{
    struct sockaddr_in addr;
    bzero(&addr,sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    return connect(&addr);
}

int SkpSocketTcp::connect(struct sockaddr_in *addr)
{
    SKP_D(SkpSocketTcp);
    memcpy(&skpD->m_addr, &addr, sizeof(struct sockaddr_in));

    if(::connect(socketDescriptor(),(struct sockaddr*)addr,sizeof(struct sockaddr)) != 0) {
        if(errno != EINPROGRESS) {
            printf("create connect failed \n");
            return Skp::Connect_Error;
        }
    } else {
        //printf("create connect succse \n");
        return Skp::Connect_Error_Success;
    }

    skpD->m_epfd = epoll_create(32000);


    struct epoll_event ee;
    ee.events = EPOLLOUT | EPOLLIN | EPOLLERR;
    ee.data.fd = socketDescriptor();

    int ret = epoll_ctl(skpD->m_epfd, EPOLL_CTL_ADD, socketDescriptor(), &ee);
    if(ret == -1) {
        printf("epoll_ctl error\n");
        return Skp::Connect_Error;
    }

    struct epoll_event events[4096];

    int nevents = epoll_wait(skpD->m_epfd, events, 4096, 1000 * 3);
    if(nevents < 0) {
        printf("epoll_wait error \n");
        return Skp::Connect_Error;
    } else if(nevents == 0) {
        printf("epoll_wait timeout \n");
        return Skp::Connect_Error;
    } else {
        int res = 0;
        int i;
        for(i = 0; i < nevents; i++)
        {
            if(events[i].data.fd == socketDescriptor())
            {
                if(events[i].events & (EPOLLIN | EPOLLERR))
                {
                    printf("epoll_wait connect failed \n");
                    res = Skp::Connect_Error;
                }

                if(events[i].events & EPOLLOUT)
                {
                    //printf("epoll_wait connect ok \n");
                    res = Skp::Connect_Error_Success;
                }

                break;
            }
        }


        struct epoll_event eee;
        eee.events = 0;
        eee.data.fd = 0;

        epoll_ctl(skpD->m_epfd, EPOLL_CTL_DEL, socketDescriptor(), &eee);

        if(i >= nevents)
        {
            return Skp::Connect_Error;
        }

        return res;
    }
}

void SkpSocketTcp::bindRead(std::function<void(int, SkpSocketStreamTcp *)> func, int timeout)
{
    SKP_D(SkpSocketTcp);
    SKP_ASSERT(func);
    SKP_ASSERT(timeout >= 0);
    skpD->m_funcRead = func;
    skpD->m_timeoutRead = timeout;
}

void SkpSocketTcp::bindWrite(std::function<void(int, SkpSocketStreamTcp *)> func, int timeout)
{
    SKP_D(SkpSocketTcp);
    SKP_ASSERT(func);
    SKP_ASSERT(timeout >= 0);
    skpD->m_funcWrite = func;
    skpD->m_timeoutWrite = timeout;
}

void SkpSocketTcp::startReadStream()
{
    SKP_D(SkpSocketTcp);
    stopWrite();
    startRead(skpD->m_timeoutRead, std::bind(&SkpSocketTcp::onReadStream, this, std::placeholders::_1));
}

void SkpSocketTcp::appendWriteStream(void *data, int size)
{
    SKP_D(SkpSocketTcp);
    int err = SOCKET_BUFFER_NULL;
    if(skpD->m_tcpStreamWrite->append(data, size) != size)
        err = SOCKET_BUFFER_ERROR;

    if(err == SOCKET_BUFFER_NULL) {
        err = skpD->m_tcpStreamWrite->skp_writev(0);
    }

    if(err >= SOCKET_BUFFER_NULL) {
        if(skpD->m_tcpStreamWrite->size() > 0) {
            stopRead();
            startWrite(skpD->m_timeoutWrite, std::bind(&SkpSocketTcp::onWriteStream, this, std::placeholders::_1));
            return;
        }
    }

    startReadStream();
    skpD->m_funcWrite(err, skpD->m_tcpStreamWrite);

/*
    if(m_tcpStreamWrite->skp_size() > 0) {
        skp_start_write();
    } else {
        skp_start_read();
        if(m_isMore)
            skp_add_read_ready();
    }

    return SKP_OK;

    int result = m_tcpStreamWrite->skp_writev(size);
    if(result <= SOCKET_BUFFER_CLOSE) {
        if(m_eventChanData)
            m_eventChanData->skp_free();
        skp_deleteLater();
        return SKP_ERROR;
    }

    if(m_tcpStreamWrite->skp_size() > 0) {
        skp_start_write();
    } else {
        skp_start_read();
        if(m_isMore)
            skp_add_read_ready();
    }

    return SKP_OK;
    */
}

void SkpSocketTcp::onReadStream(bool isTimeout)
{
    SKP_D(SkpSocketTcp);
    int err = SOCKET_BUFFER_NULL;
    if(isTimeout) {
        err = SOCKET_BUFFER_TIMEOUT;
    }

    if(err == SOCKET_BUFFER_NULL) {
        int result = skpD->m_tcpStreamRead->skp_readv(0);

        if(!isET() && skpD->m_tcpStreamRead->size() > 0) {
            addReadReady();
        }

        if(!isET() && result == SOCKET_BUFFER_NULL && skpD->m_tcpStreamRead->size() == 0) {
            removeReadReady();
            return;
        }

        if(isET() && result == SOCKET_BUFFER_NULL && skpD->m_tcpStreamRead->size() == 0) {
            removeReadReady();
            return;
        }

        if(result <= SOCKET_BUFFER_CLOSE) {
        }

        err = result;
    }


    skpD->m_funcRead(err, skpD->m_tcpStreamRead);

    //if(skpD->m_tcpStreamWrite->skp_size())
    //    appendWriteStream(skp_null, 0, 0, skp_null);
}

void SkpSocketTcp::onWriteStream(bool isTimeout)
{
    SKP_D(SkpSocketTcp);
    if(isTimeout) {
        int err = SOCKET_BUFFER_TIMEOUT;
        startReadStream();
        skpD->m_funcWrite(err, skpD->m_tcpStreamWrite);
        return;
    }

    appendWriteStream(skp_null, 0);
}

bool SkpSocketTcp::appendWriteChan(void *data, int size)
{
    SKP_D(SkpSocketTcp);
    return skpD->m_tcpChanWrite->push_back(NULL, 0, data, size);
}

bool SkpSocketTcp::onChanWrite(void *head, int headSize, void *data, int dataSize)
{
    SKP_UNUSED(head);
    SKP_UNUSED(headSize);
    appendWriteStream(data, dataSize);
    return skp_true;
}

void SkpSocketTcp::acquire()
{
    objectCountIncr_atomic();
}

void SkpSocketTcp::release()
{
    __SkpAutoLocker(objectCountMutex());
    int count = objectCountDecr();
    SKP_ASSERT(count >= 0);
    if(count == 0) {
        SkpSocket::deleteLater();
    }
}
