#include "skp_socket_manage.h"
#include "skp_tcp_socket.h"
#include "skp_public_transfer_protocal.h"
#include "assert.h"

#include <QTime>

#define SOCKET_MANAGE_NUMBER 2
#define SOCKET_MOMENT_NUMBER 20

quint64 SkpSocketManage::m_loginCode = 0;
QString SkpSocketManage::m_connectIP = "192.168.164.152";
quint16 SkpSocketManage::m_connectPort = 22221;
//quint16 SkpSocketManage::m_connectPort = 33331;

//QString SkpSocketManage::m_connectIP = "172.16.100.40";
//quint16 SkpSocketManage::m_connectPort = 33331;

//static const char *connectIP = "192.168.164.134";
//static const quint16 connectPort = 33331;

//static const char *connectIP = "172.16.100.40";
//static const quint16 connectPort = 33331;

QVector<SkpSocketManage *> SkpSocketManage::m_socketManages;
QVector<QThread *> SkpSocketManage::m_threads;

SkpSocketManage::SkpSocketManage(SocketType type, QObject *parent) :
    QObject(parent),
    m_type(type)
{

}

void SkpSocketManage::init()
{
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    for(int i = 0; i < SOCKET_MANAGE_NUMBER; i++) {
        QThread *thread = new QThread();
        m_threads.append(thread);
        thread->start();
        while(!thread->isRunning()) {
            ;
        }

        SkpSocketManage *socketManage = new SkpSocketManage((SocketType)i);
        m_socketManages.append(socketManage);
        socketManage->moveToThread(thread);
    }
}

SkpSocketManage *SkpSocketManage::instance(SocketType type)
{
   return m_socketManages.at(type);
}

void SkpSocketManage::addSocketMonent(quint64 proxyID, SkpTcpSocket *socket)
{
    QVector<SkpTcpSocket *> *tcpSockets = m_hashMoment.value(proxyID);
    if(!tcpSockets) {
        tcpSockets = new QVector<SkpTcpSocket *>();
        m_hashMoment.insert(proxyID, tcpSockets);
    }
    tcpSockets->append(socket);
}

SkpTcpSocket *SkpSocketManage::findSocketMonent(quint64 proxyID)
{
    QVector<SkpTcpSocket *> *tcpSockets = m_hashMoment.value(proxyID);
    if(!tcpSockets)
        return NULL;

    if(tcpSockets->size() < SOCKET_MOMENT_NUMBER)
        return NULL;

    int index = qrand()% tcpSockets->size();
    return tcpSockets->value(index);
}

void SkpSocketManage::onRequest(const QByteArray &data)
{
    if(!SkpSocketManage::m_loginCode) {
        qDebug() << "SkpSocketManage::m_loginCode" << SkpSocketManage::m_loginCode;
        return;
    }

    SkpProtocalHead *protocalHead = (SkpProtocalHead *)data.data();
//    protocalHead->send = SkpSocketManage::m_loginCode;
//    if(!protocalHead->recv)
//        protocalHead->recv = SkpSocketManage::m_loginCode;

    SkpTcpSocket *tcpSocket = NULL;
    if(m_type == SocketTypeKeep) {
        tcpSocket = (SkpTcpSocket *)m_hashKeep.value(protocalHead->proxyID);
    } else {
        tcpSocket = findSocketMonent(protocalHead->proxyID);
    }

    if(!tcpSocket) {

        tcpSocket = new SkpTcpSocket();
        uint8 isKeep = 0;
        if(m_type == SocketTypeKeep) {
            isKeep = 1;
            m_hashKeep.insert(protocalHead->proxyID, tcpSocket);
        } else {
            addSocketMonent(protocalHead->proxyID, tcpSocket);
        }
        tcpSocket->setServerID(protocalHead->proxyID);
        tcpSocket->setKeep(isKeep);
        tcpSocket->setSend(protocalHead->send);
        tcpSocket->setIP(SkpSocketManage::m_connectIP);
        tcpSocket->setPort(SkpSocketManage::m_connectPort);

        connect(tcpSocket, SIGNAL(readyRead()), tcpSocket, SLOT(onRead()));
        connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), tcpSocket, SLOT(onError(QAbstractSocket::SocketError)));
        connect(tcpSocket, SIGNAL(disconnected()), tcpSocket, SLOT(onDisconnected()));
        connect(tcpSocket, SIGNAL(connected()), tcpSocket, SLOT(onConnected()));

        connect(tcpSocket, SIGNAL(sigResponse(QByteArray)), this, SIGNAL(sigResponse(QByteArray)));

    }

    tcpSocket->connectToProxy();

    tcpSocket->addData(data);
}
