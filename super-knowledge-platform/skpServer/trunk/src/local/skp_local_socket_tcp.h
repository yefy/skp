#ifndef SKP_LOCAL_SOCKET_TCP_H
#define SKP_LOCAL_SOCKET_TCP_H

#include "skpSocketTcp.h"
#include "skp_public_transfer_protocal.h"

class SkpLocalSocketTcpPrivate;
class SkpLocalApplication;

class SkpLocalSocketTcp : public SkpSocketTcp
{
public:
    SkpLocalSocketTcp(SkpObject *parent = 0);
    SkpLocalSocketTcp(SkpThread &thread);
    SkpLocalSocketTcp(int fd, SkpObject *parent = 0);
    SkpLocalSocketTcp(int fd, SkpThread &thread);
    virtual ~SkpLocalSocketTcp();
protected:
    SkpLocalSocketTcp(int fd, SkpLocalSocketTcpPrivate &d, SkpObject *parent = 0);
    SkpLocalSocketTcp(int fd, SkpLocalSocketTcpPrivate &d, SkpThread &thread);
private:
    SKP_DECLARE_PRIVATE(SkpLocalSocketTcp);
public:
    void init();
    void start();
    void onLocalReadStream(int size, SkpSocketStreamTcp *tcpStreamRead);
    void onLocalWriteStream(int size, SkpSocketStreamTcp *tcpStreamWrite);
    void skp_parse_protocol(SkpSocketStreamTcp *tcpStreamRead);
    uint64 serverID();
    void release();
    void deleteLater();
    void addDel(std::function<bool(uint64 serverID, SkpLocalSocketTcp *)> funcDel);

    SkpProtocalConnHead m_connHead;
    SkpProtocalHead m_head;
    bool m_isConn;
    bool m_isOwner;
    std::function<bool(uint64 serverID, SkpLocalSocketTcp *)> m_funcDel;
};

#endif // SKP_LOCAL_SOCKET_TCP_H
