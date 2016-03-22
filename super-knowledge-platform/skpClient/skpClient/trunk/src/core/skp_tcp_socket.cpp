#include "skp_tcp_socket.h"
#include "skp_public_transfer_protocal.h"
#include <QDebug>

SkpTcpSocket::SkpTcpSocket() :
    m_proxy(0),
    m_isKeep(0),
    m_send(0),
    m_isConn(0)
{
}

SkpTcpSocket::~SkpTcpSocket()
{

}

void SkpTcpSocket::onConnected()
{
    addHead();
    m_isConn = true;
}

void SkpTcpSocket::onError(QAbstractSocket::SocketError err)
{
    m_isConn = false;
}

void SkpTcpSocket::onDisconnected()
{
    m_isConn = false;
}

void SkpTcpSocket::onRead()
{
//    if(this->bytesAvailable() > 0) {
//        QByteArray byte(readAll());
//        if(!byte.isEmpty()) {
//            emit sigResponse(byte, this);
//        }
//    }

    while(this->bytesAvailable() > 0) {
        QByteArray byte(readAll());

        if(!byte.isEmpty()) {
            m_bytes.append(byte);

            while(true) {
                if(!SkpCheckHead(m_bytes.data(), m_bytes.size())) {
                    break;
                }

                SkpProtocalHead *protocalHead = (SkpProtocalHead *)m_bytes.data();
                int allSize = (protocalHead->headSize + protocalHead->dataSize);
                QByteArray bytes = m_bytes.mid(0, allSize);
                m_bytes.remove(0, allSize);

                emit sigResponse(bytes);
            }
        }
    }
}

void SkpTcpSocket::addHead()
{
    SkpProtocalConnHead head;
    SkpPakegeConnHead(&head, m_isKeep, ConnTypeClient, 0, m_send, m_proxy);
    write((char *)&head, head.headSize);
}

void SkpTcpSocket::addData(const QByteArray &byte)
{
    connectToProxy();

    write(byte);

//    if(m_isKeep) {
//        bool wait = waitForBytesWritten(3000);
//        if(!wait) {
//            qDebug() << "waitForBytesWritten error";
//            m_isConn = false;
//            connectToProxy();
//            write(byte);
//        }
//    }
}

bool SkpTcpSocket::connectToProxy()
{
    if(!m_isConn) {
        abort();
        connectToHost(m_ip, m_port);

        if(!m_isKeep) {
            this->setSocketOption(LowDelayOption, 1);
            this->setSocketOption(SendBufferSizeSocketOption, 1024 * 128);
            this->setSocketOption(ReceiveBufferSizeSocketOption, 1024 * 128);
        }

        return waitForConnected(3000);
    }
    return true;
}
