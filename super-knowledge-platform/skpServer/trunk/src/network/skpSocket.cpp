#include "skpSocket.h"
#include "skpSocket_p.h"
#include "skpEventRead.h"
#include "skpEventWrite.h"
#include "skpAutoFree.h"
#include "skpLog.h"


#define MAX_SENDBUF_SIZE (256 * 1024 * 1024)

#define SOCKET_UDP_SEND_BUFFER_SIZE  1024 * 8 * 8 * 2
#define SOCKET_UDP_RECV_BUFFER_SIZE  1024 * 8 * 8 * 2

//=============SkpSocketPrivate
SkpSocketPrivate::SkpSocketPrivate() :
    SkpObjectPrivate(),
    m_fd(-1),
    m_eventRead(skp_null),
    m_eventWrite(skp_null),
    m_isET(skp_true)
{

}

SkpSocketPrivate::~SkpSocketPrivate()
{

}


//==================SkpSocket

SkpSocket::SkpSocket(int fd, SkpObject *parent) :
    SkpObject(*new SkpSocketPrivate(), parent)
{
    init(fd);
}

SkpSocket::SkpSocket(int fd, SkpThread &thread) :
    SkpObject(*new SkpSocketPrivate(), thread)
{
    init(fd);
}

SkpSocket::SkpSocket(int fd, SkpSocketPrivate &d, SkpObject *parent) :
    SkpObject(d, parent)
{
    init(fd);
}

SkpSocket::SkpSocket(int fd, SkpSocketPrivate &d, SkpThread &thread) :
    SkpObject(d, thread)
{
    init(fd);
}

void SkpSocket::init(int fd)
{
    SKP_D(SkpSocket);
    skpD->m_fd = fd;
    SkpSocket::setNoblock();
    skpD->m_eventRead = new SkpEventRead(this);
    skpD->m_eventWrite = new SkpEventWrite(this);
}


SkpSocket::~SkpSocket()
{
    SKP_D(SkpSocket);
    skp_delete(skpD->m_eventRead);
    skp_delete(skpD->m_eventWrite);
    SkpSocket::close();
}

void SkpSocket::startRead(int timeout, std::function<void(bool)> func)
{
    SKP_D(SkpSocket);
    skpD->m_eventRead->connect(skpD->m_fd, func);
    skpD->m_eventRead->setET(skpD->m_isET);
    skpD->m_eventRead->start(timeout);
}

void SkpSocket::stopRead()
{
    SKP_D(SkpSocket);
    skpD->m_eventRead->stop();
}

void SkpSocket::startWrite(int timeout, std::function<void(bool)> func)
{
    SKP_D(SkpSocket);
    skpD->m_eventWrite->connect(skpD->m_fd, func);
    skpD->m_eventWrite->setET(skpD->m_isET);
    skpD->m_eventWrite->start(timeout);
}

void SkpSocket::stopWrite()
{
    SKP_D(SkpSocket);
    skpD->m_eventWrite->stop();
}

void SkpSocket::free()
{
    SKP_D(SkpSocket);
    skpD->m_eventRead->free();
    skpD->m_eventWrite->free();
}

void SkpSocket::deleteLater()
{
    if(isDeleteLater())
        return;
    SkpSocket::free();
    SkpObject::deleteLater();
}

void SkpSocket::removeReadReady()
{
    SKP_D(SkpSocket);
    skpD->m_eventRead->removeReadReady();
}

void SkpSocket::addReadReady()
{
    SKP_D(SkpSocket);
    skpD->m_eventRead->addReadReady();
}

void SkpSocket::setET(bool isET)
{
    SKP_D(SkpSocket);
    skpD->m_isET = isET;
}

bool SkpSocket::isET()
{
    SKP_D(SkpSocket);
    return skpD->m_isET;
}

//设置socker缓冲区
void SkpSocket::skp_set_socket_buffer(int fd,int sendSize, int recvSize)
{
    ///设置socket缓冲区大小
    int optSize = sizeof(int);

    setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (void *)&sendSize, (socklen_t )optSize);
    setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (void *)&recvSize, (socklen_t )optSize);
    //printf("sendsize = %d \n", sendSize);
    //printf("recvSize = %d \n", recvSize);
    skp_get_socket_buffer(fd, &sendSize, &recvSize);

    struct linger ling = {1, 10};
    int error = 0;
    int flags =1;

    error = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *)&flags, sizeof(flags));
    if (error != 0) {
        skpLogError_g("setsockopt SO_REUSEADDR\n");
        return;
    }

    error = setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&flags, sizeof(flags));
    if (error != 0) {
        skpLogError_g("setsockopt SO_KEEPALIVE\n");
        return;
    }

    error = setsockopt(fd, SOL_SOCKET, SO_LINGER, (void *)&ling, sizeof(ling));
    if (error != 0) {
        skpLogError_g("setsockopt SO_LINGER");
        return;
    }

    error = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)&flags, sizeof(flags));
    if (error != 0) {
        skpLogError_g("setsockopt TCP_NODELAY");
        return;
    }
}

void SkpSocket::skp_get_socket_buffer(int fd, int *sendSize, int *recvSize)
{
    int optSize = sizeof(int);

    getsockopt(fd, SOL_SOCKET, SO_SNDBUF, (void *)sendSize, (socklen_t *)&optSize);
    getsockopt(fd, SOL_SOCKET, SO_RCVBUF, (void *)recvSize, (socklen_t *)&optSize);
    //printf("sendsize = %d \n", *sendSize);
    //printf("recvSize = %d \n", *recvSize);
}

bool SkpSocket::setNoblock()
{
    SKP_D(SkpSocket);
    return ::setNoblock(skpD->m_fd);
}

bool SkpSocket::setBlock()
{
    SKP_D(SkpSocket);
    return ::setBlock(skpD->m_fd);
}

int SkpSocket::skp_max_send_buf(int fd)
{
    socklen_t intsize = sizeof(int);
    //int last_good = 0;
    int min, max, avg;
    int old_size;

    /* Start with the default size. */
    if (getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &old_size, &intsize) != 0) {
        skpLogError_g("getsockopt SO_SNDBUF failed\n");
        return skp_false;
    }

    /* Binary-search for the real maximum. */
    min = old_size;
    max = MAX_SENDBUF_SIZE;

    while (min <= max) {
        avg = ((unsigned int)(min + max)) / 2;
        if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (void *)&avg, intsize) == 0) {
            //last_good = avg;
            min = avg + 1;
        } else {
            max = avg - 1;
        }
    }

    return skp_true;
}

void SkpSocket::close()
{
    SKP_D(SkpSocket);
    ::close(skpD->m_fd);
    skpD->m_fd = -1;
}

int SkpSocket::socketDescriptor()
{
    SKP_D(SkpSocket);
    return skpD->m_fd;
}
