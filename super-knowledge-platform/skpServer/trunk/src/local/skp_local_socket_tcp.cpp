#include "skp_local_socket_tcp.h"
#include "skpSocketTcp_p.h"
#include "skpTcpStream.h"
#include "skp_local_application.h"
#include "skp_local_manage_parse.h"


#define SOCKET_TCP_SEND_BUFFER_SIZE  1024 * 8 * 8 * 2
#define SOCKET_TCP_RECV_BUFFER_SIZE  1024 * 8 * 8 * 2

//==================SkpLocalSocketTcpPrivate
class SkpLocalSocketTcpPrivate : public SkpSocketTcpPrivate
{
public:
    SkpLocalSocketTcpPrivate();
    virtual ~SkpLocalSocketTcpPrivate();
private:
    SKP_DECLARE_PUBLIC(SkpLocalSocketTcp);

private:
};

SkpLocalSocketTcpPrivate::SkpLocalSocketTcpPrivate() :
    SkpSocketTcpPrivate()
{

}

SkpLocalSocketTcpPrivate::~SkpLocalSocketTcpPrivate()
{
}

//===================SkpLocalSocketTcp
SkpLocalSocketTcp::SkpLocalSocketTcp(SkpObject *parent) :
    SkpSocketTcp(SkpSocketTcp::socket(), *new SkpLocalSocketTcpPrivate(), parent)
{
    init();
}

SkpLocalSocketTcp::SkpLocalSocketTcp(SkpThread &thread) :
    SkpSocketTcp(SkpSocketTcp::socket(), *new SkpLocalSocketTcpPrivate(), thread)
{
    init();
}

SkpLocalSocketTcp::SkpLocalSocketTcp(int fd, SkpObject *parent) :
    SkpSocketTcp(fd, *new SkpLocalSocketTcpPrivate(), parent)
{
    init();
}

SkpLocalSocketTcp::SkpLocalSocketTcp(int fd, SkpThread &thread) :
    SkpSocketTcp(fd, *new SkpLocalSocketTcpPrivate(), thread)
{
    init();
}

SkpLocalSocketTcp::SkpLocalSocketTcp(int fd, SkpLocalSocketTcpPrivate &d, SkpObject *parent) :
    SkpSocketTcp(fd, d, parent)
{
    init();
}

SkpLocalSocketTcp::SkpLocalSocketTcp(int fd, SkpLocalSocketTcpPrivate &d, SkpThread &thread) :
    SkpSocketTcp(fd, d, thread)
{
    init();
}


SkpLocalSocketTcp::~SkpLocalSocketTcp()
{
    if(m_connHead.isKeep) {
    }
}

void SkpLocalSocketTcp::init()
{
    m_isConn = skp_false;
    m_isOwner = skp_false;
    m_funcDel = skp_null;
    skp_set_socket_buffer(socketDescriptor(), SOCKET_TCP_SEND_BUFFER_SIZE, SOCKET_TCP_RECV_BUFFER_SIZE);
}

void SkpLocalSocketTcp::start()
{
    bindRead(std::bind(&SkpLocalSocketTcp::onLocalReadStream, this, std::placeholders::_1, std::placeholders::_2), 0);
    bindWrite(std::bind(&SkpLocalSocketTcp::onLocalWriteStream, this, std::placeholders::_1, std::placeholders::_2), 0);
    startReadStream();
}

void SkpLocalSocketTcp::onLocalReadStream(int size, SkpSocketStreamTcp *tcpStreamRead)
{
    if(size <= SOCKET_BUFFER_TIMEOUT) {
        release();
        return;
    }

    skp_parse_protocol(tcpStreamRead);
}

void SkpLocalSocketTcp::onLocalWriteStream(int size, SkpSocketStreamTcp *tcpStreamWrite)
{
    SKP_UNUSED(tcpStreamWrite);
    if(size <= SOCKET_BUFFER_TIMEOUT) {
        release();
        return;
    }
}


void SkpLocalSocketTcp::skp_parse_protocol(SkpSocketStreamTcp *tcpStreamRead)
{
    if(tcpStreamRead->size() > 0) {
        if(m_isConn == skp_false) {
            tcpStreamRead->series(sizeof(SkpProtocalConnHead));
            bool isOK = SkpCheckConnHead((char *)tcpStreamRead->byte(), tcpStreamRead->size());
            if( !isOK ) {
                return;
            }

            tcpStreamRead->copy(&m_connHead, sizeof(SkpProtocalConnHead));

            tcpStreamRead->skip(m_connHead.headSize + m_connHead.dataSize);
            m_isConn = skp_true;

            if(m_connHead.isKeep) {
            }
        }

        tcpStreamRead->series(sizeof(SkpProtocalHead));
        bool isOK = SkpCheckHead((char *)tcpStreamRead->byte(), tcpStreamRead->size());
        if( !isOK ) {
            return;
        }

        SkpProtocalHead *head = (SkpProtocalHead *)tcpStreamRead->byte();
        int allSize = head->headSize + head->dataSize;
        tcpStreamRead->series(allSize);
        head = (SkpProtocalHead *)tcpStreamRead->byte();


        acquire();
        long socketAddr = (long)this;
        if(SkpLocalManageParse::instance(head->recv)->parseChan(&socketAddr, sizeof(long), tcpStreamRead->byte(), allSize)) {
            tcpStreamRead->skip(allSize);
        }
    }
}

uint64 SkpLocalSocketTcp::serverID()
{
    uint64 serverID;
    if(m_isOwner) {
        serverID = m_connHead.toServerID;
    } else {
        serverID = m_connHead.fromServerID;
    }
    return serverID;
}

void SkpLocalSocketTcp::release()
{
    __SkpAutoLocker(objectCountMutex());
    int count = objectCountDecr();

    SKP_ASSERT(count >= 0);
    if(count == 0) {
        SkpLocalSocketTcp::deleteLater();
    }
}

void SkpLocalSocketTcp::deleteLater()
{
    if(isDeleteLater())
        return;
    if(m_funcDel)
        m_funcDel(serverID(), this);
    SkpSocketTcp::deleteLater();
}

void SkpLocalSocketTcp::addDel(std::function<bool(uint64 serverID, SkpLocalSocketTcp *)> funcDel)
{
    m_funcDel = funcDel;
}
