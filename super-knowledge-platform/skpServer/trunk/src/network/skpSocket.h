#ifndef SKPSOCKET_H
#define SKPSOCKET_H

#include "skpObject.h"

class SkpSocketPrivate;


class SkpSocket : public SkpObject
{
public:
    SkpSocket(int fd, SkpObject *parent = 0);
    SkpSocket(int fd, SkpThread &thread);
    virtual ~SkpSocket();
protected:
    SkpSocket(int fd, SkpSocketPrivate &d, SkpObject *parent = 0);
    SkpSocket(int fd, SkpSocketPrivate &d, SkpThread &thread);
private:
    SKP_DECLARE_PRIVATE(SkpSocket);
public:
    void init(int fd);
    bool setNoblock();
    bool setBlock();

    static int skp_max_send_buf(int fd);
    static void skp_set_socket_buffer(int fd, int sendSize, int recvSize);
    static void skp_get_socket_buffer(int fd, int *sendSize, int *recvSize);

    void close();

    void startRead(int timeout, std::function<void(bool)> func);
    void stopRead();
    void startWrite(int timeout, std::function<void(bool)> func);
    void stopWrite();
    void free();

    virtual void deleteLater();

    void removeReadReady();
    void addReadReady();
    void setET(bool isET);
    bool isET();
    int socketDescriptor();
};

#endif // SKPSOCKET_H
