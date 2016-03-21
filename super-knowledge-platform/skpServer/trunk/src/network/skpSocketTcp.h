#ifndef SKPSOCKETTCP_H
#define SKPSOCKETTCP_H

#include "skpSocket.h"

namespace Skp {
enum Connect_Error_Status {
    Connect_Error_Success = 0,
    Connect_Error,
    Connect_Error_Timeout,
};
}

class SkpSocketTcpPrivate;
class SkpSocketStreamTcp;

class SkpSocketTcp : public SkpSocket
{
public:
    SkpSocketTcp(SkpObject *parent = 0);
    SkpSocketTcp(SkpThread &thread);
    SkpSocketTcp(int fd, SkpObject *parent = 0);
    SkpSocketTcp(int fd, SkpThread &thread);
    virtual ~SkpSocketTcp();
protected:
    SkpSocketTcp(int fd, SkpSocketTcpPrivate &d, SkpObject *parent = 0);
    SkpSocketTcp(int fd, SkpSocketTcpPrivate &d, SkpThread &thread);
private:
    SKP_DECLARE_PRIVATE(SkpSocketTcp);
public:
    void init();
    static int socket();
    int connect(const char *ip, uint16 port);
    int connect(struct sockaddr_in *addr);
    void bindRead(std::function<void(int, SkpSocketStreamTcp *)> func, int timeout);
    void bindWrite(std::function<void(int, SkpSocketStreamTcp *)> func, int timeout);
    void startReadStream();
    void appendWriteStream(void *data, int size);
    bool appendWriteChan(void *data, int size);
    virtual void acquire();
    virtual void release();
private:
    void onReadStream(bool isTimeout);
    void onWriteStream(bool isTimeout);
    bool onChanWrite(void *head, int headSize, void *data, int dataSize);


};

#endif // SKPSOCKETTCP_H
