#ifndef SKPTCPSTREAM_H
#define SKPTCPSTREAM_H

#include "skpBuffer.h"


#define SOCKET_BUFFER_NULL     0
#define SOCKET_BUFFER_FULL     -1
#define SOCKET_BUFFER_TIMEOUT  -2
#define SOCKET_BUFFER_CLOSE    -3
#define SOCKET_BUFFER_ERROR    -4

class SkpSocketStreamTcp : public SkpBuffer
{
public:
    explicit SkpSocketStreamTcp(int fd);
    virtual ~SkpSocketStreamTcp();
    virtual int skp_readv(int size = 0);
    virtual int skp_writev(int size = 0);

private:
    int skp_readv(int fd, const struct iovec *iov, int iovcnt);
    int skp_writev(int fd, const struct iovec *iov, int iovcnt);

private:
    int m_fd;
    struct iovec * m_iovec;
    int m_iovecSize;
};

#endif // SKPTCPSTREAM_H
