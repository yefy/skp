#include "skp_core_signal.h"
#include "skp_core_manage.h"
#include "skp_socket_manage.h"
#include <QThread>
#include <QDebug>

SkpSignal::SkpSignal() :
    QObject(0),
    SkpLocalTLV(),
    m_clientMark(0)
{
    init_signal();
}

SkpSignal::SkpSignal(char *pBuffer, int64 nSize) :
    QObject(0),
    SkpLocalTLV(pBuffer, nSize),
    m_clientMark(0)
{
    init_signal();
}

SkpSignal::SkpSignal(char *pBuffer, int64 nSize, int tlvSize, int headSize) :
    QObject(0),
    SkpLocalTLV(pBuffer, nSize, tlvSize, headSize),
    m_clientMark(0)
{
    init_signal();
}

SkpSignal::~SkpSignal()
{

}

void SkpSignal::init_signal()
{
    //qRegisterMetaType<std::function<void(const QByteArray &)> >("std::function<void(const QByteArray &)>");


    SkpManage *manage = SkpManage::instance();
    //connect(this, SIGNAL(sigKeep(QByteArray,std::function<const ( QByteArray&)>)), manage, SLOT(onKeep(QByteArray,std::function<const ( QByteArray&)>)));
    //connect(this, SIGNAL(sigMoment(QByteArray,std::function<const ( QByteArray&)>)), manage, SLOT(onMoment(QByteArray,std::function<const ( QByteArray&)>)));

    connect(this, &SkpSignal::sigKeep, manage, &SkpManage::onKeep);
    connect(this, &SkpSignal::sigMoment, manage, &SkpManage::onMoment);
    connect(this, &SkpSignal::sigRegisterAck, manage, &SkpManage::onRegisterAck);

}

void SkpSignal::emit_sigKeep(std::function<void(const QByteArray &)> func)
{
    if(true) {
        QMutexLocker mutexLocker(&m_clientMarkMutex);
        quint64 addr = (quint64)this;
        quint64 clientMark = addr << 32;
        m_clientMark++;
        clientMark += m_clientMark;
        //qDebug() << "emit_sigMoment" << clientMark << m_clientMark;
        skp_set_client_mark(clientMark);
    }

    skp_set_send(SkpSocketManage::m_loginCode);
    if(!skp_recv())
        skp_set_recv(SkpSocketManage::m_loginCode);
    skp_post();
    QByteArray sendData((char *)byte(), use_size());

    emit sigKeep(sendData, func);

//    SkpManage *manage = SkpManage::instance();
//    if(manage->thread()->currentThreadId() == QThread::currentThreadId()) {
//        manage->onKeep(sendData, func);
//    } else {
//        emit sigKeep(sendData, func);
//    }
}

void SkpSignal::emit_sigMoment(std::function<void(const QByteArray &)> func)
{
    if(true) {
        QMutexLocker mutexLocker(&m_clientMarkMutex);
        quint64 addr = (quint64)this;
        quint64 clientMark = addr << 32;
        m_clientMark++;
        clientMark += m_clientMark;
        //qDebug() << "emit_sigMoment" << clientMark << m_clientMark;
        skp_set_client_mark(clientMark);
    }

    skp_set_send(SkpSocketManage::m_loginCode);
    if(!skp_recv())
        skp_set_recv(SkpSocketManage::m_loginCode);
    skp_post();

    QByteArray sendData((char *)byte(), use_size());

    emit sigMoment(sendData, func);

//    SkpManage *manage = SkpManage::instance();
//    if(manage->thread()->currentThreadId() == QThread::currentThreadId()) {
//        manage->onMoment(sendData, func);
//    }else {
//        emit sigMoment(sendData, func);
//    }
}

void SkpSignal::emit_register_ack(quint64 classType, std::function<void(QByteArray &)> func)
{
    emit sigRegisterAck(classType, func);
}
