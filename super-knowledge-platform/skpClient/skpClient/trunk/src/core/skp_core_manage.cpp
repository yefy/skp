#include "skp_core_manage.h"
#include "skp_socket_manage.h"
#include "skp_public_transfer_protocal.h"
#include "skp_tcp_socket.h"
#include "memory_file.pb.h"
#include "skp_local_tlv.h"
#include "ack.pb.h"
#include "message.pb.h"
#include "skp_public_ins.h"
#include "skp_core_signal.h"


#include <QDebug>

#include <assert.h>


SkpManage *SkpManage::m_manage = NULL;
QThread *SkpManage::m_thread = NULL;

SkpManage::SkpManage(QObject *parent) :
    QObject(parent),
    m_signal(NULL)
{
    //qRegisterMetaType<SkpDataCallback_t>("SkpDataCallback_t");
    qRegisterMetaType<std::function<void(QByteArray &)> >("std::function<void(QByteArray &)>");

    SkpSocketManage *keepManage = SkpSocketManage::instance(SocketTypeKeep);
    SkpSocketManage *momentManage = SkpSocketManage::instance(SocketTypeKeepMoment);
    connect(keepManage, SIGNAL(sigResponse(QByteArray)), this, SLOT(onResponse(QByteArray)));
    connect(momentManage, SIGNAL(sigResponse(QByteArray)), this, SLOT(onResponse(QByteArray)));
    connect(this, SIGNAL(sigKeep(QByteArray)), keepManage, SLOT(onRequest(QByteArray)));
    connect(this, SIGNAL(sigMoment(QByteArray)), momentManage, SLOT(onRequest(QByteArray)));

    registerIns(OrderLocalAck, std::bind(&SkpManage::registerCallbackAck, this, std::placeholders::_1));
    registerIns(OrderLocalRealTime, std::bind(&SkpManage::registerCallbackRealTime, this, std::placeholders::_1));

}

SkpManage *SkpManage::instance()
{
    if(!m_manage) {
        m_thread = new QThread();
        m_thread->start();
        while(!m_thread->isRunning()) {
            ;
        }
        m_manage = new SkpManage();
        m_manage->moveToThread(m_thread);
    }
    return m_manage;
}

QThread * SkpManage::instanceThread()
{
    return m_thread;
}

void SkpManage::registerIns(quint64 ins, std::function<void(QByteArray &)> func)
{
    SkpInsKey_t insKey;
    insKey.ins = ins;
    insKey.clientMark = 0;

    if(m_hash.find(insKey) != m_hash.end()) {
        assert(false);
    }
    //m_hash.insert(insKey, func);
    m_hash.insert(std::pair<SkpInsKey_t, std::function<void(QByteArray &)>>(insKey, func));
}

void SkpManage::onKeep(const QByteArray &data, std::function<void(QByteArray &)> func)
{
    SkpProtocalHead *protocalHead = (SkpProtocalHead *)data.data();

    SkpInsKey_t insKey;
    insKey.ins = protocalHead->orderRequst;
    insKey.clientMark = protocalHead->clientMark;

    int number = insKey.clientMark & 0xFFFFFFFF;
    //qDebug() << "onKeep" << insKey.clientMark << number << protocalHead->orderRequst;

    if(m_hash.find(insKey) != m_hash.end()) {
        assert(false);
    }
    //m_hash.insert(insKey, func);
    m_hash.insert(std::pair<SkpInsKey_t, std::function<void(QByteArray &)>>(insKey, func));

    emit sigKeep(data);
}

void SkpManage::onMoment(const QByteArray &data, std::function<void(QByteArray &)> func)
{
    SkpProtocalHead *protocalHead = (SkpProtocalHead *)data.data();

    SkpInsKey_t insKey;
    insKey.ins = protocalHead->orderRequst;
    insKey.clientMark = protocalHead->clientMark;

    int number = insKey.clientMark & 0xFFFFFFFF;
    //qDebug() << "onMoment" << insKey.clientMark << number << protocalHead->orderRequst;


    if(m_hash.find(insKey) != m_hash.end()) {
        assert(false);
    }
    //m_hash.insert(insKey, func);

    m_hash.insert(std::pair<SkpInsKey_t, std::function<void(QByteArray &)>>(insKey, func));


    emit sigMoment(data);
}

void SkpManage::onResponse(const QByteArray &data)
{
    /*
    tcpSocket->m_bytes.append(data);
    while(true) {
        if(!SkpCheckHead(tcpSocket->m_bytes.data(), tcpSocket->m_bytes.size())) {
            break;
        }

        SkpProtocalHead *protocalHead = (SkpProtocalHead *)tcpSocket->m_bytes.data();
        int allSize = (protocalHead->headSize + protocalHead->dataSize);

        SkpInsKey_t insKey;
        insKey.ins = protocalHead->orderRequst;
        insKey.clientMark = protocalHead->clientMark;

        QByteArray bytes = tcpSocket->m_bytes.mid(0, allSize);
        tcpSocket->m_bytes.remove(0, allSize);

        if(m_hash.find(insKey) == m_hash.end()) {
            int number = insKey.clientMark & 0xFFFFFFFF;
            qDebug() << protocalHead->headSize << protocalHead->dataSize << protocalHead->send << protocalHead->recv;
            qDebug() << "onResponse" << insKey.clientMark << number << tcpSocket->m_bytes.size() << allSize;;
            assert(false);
        }

        std::function<void(QByteArray &)> func = m_hash.value(insKey);
        func(bytes);

        if(OrderLocalAck != insKey.ins)
            m_hash.remove(insKey);

    }
    */

    SkpProtocalHead *protocalHead = (SkpProtocalHead *)data.data();

    SkpInsKey_t insKey;
    insKey.ins = protocalHead->orderRequst;
    insKey.clientMark = protocalHead->clientMark;

    int number = insKey.clientMark & 0xFFFFFFFF;
    //qDebug() << "onResponse" << insKey.clientMark << number << protocalHead->orderRequst;


    if(m_hash.find(insKey) == m_hash.end()) {
        //int number = insKey.clientMark & 0xFFFFFFFF;
        //qDebug() << "onResponse" << insKey.clientMark << number << protocalHead->orderRequst;

        assert(false);
    }

    //std::function<void(QByteArray &)> func = m_hash.value(insKey);
    std::function<void(QByteArray &)> func = m_hash[insKey];

    func(const_cast<QByteArray &>(data));

    if(OrderLocalAck != insKey.ins && OrderLocalRealTime != insKey.ins)
        m_hash.erase(insKey);
        //m_hash.remove(insKey);
}

void SkpManage::onRegisterAck(quint64 classType, std::function<void(QByteArray &)> func)
{
    SkpInsKey_t insKey;
    insKey.ins = classType;
    insKey.clientMark = 0;

    if(m_hashAck.find(insKey) != m_hashAck.end()) {
        assert(false);
    }

    //m_hashAck.insert(insKey, func);
    m_hashAck.insert(std::pair<SkpInsKey_t, std::function<void(QByteArray &)>>(insKey, func));
}

void SkpManage::registerCallbackAck(QByteArray &bytes)
{
//    while(m_hashAck.isEmpty()) {
//        this->thread()->sleep(1);
//    }

    while(m_hashAck.empty()) {
        this->thread()->sleep(1);
    }

    if(!m_signal)
        m_signal = new SkpSignal(NULL, 0);

    m_signal->skp_start();
    m_signal->skp_set_ins(OrderLocalAckList);
    m_signal->emit_sigKeep([this](const QByteArray &bytes)
    {
        SkpLocalTLV tlvResponse;
        tlvResponse.skp_parse_buffer(bytes.data(), bytes.size());

        Ack::MessageAckList messageAckList;
        tlvResponse.skp_Parse_message(&messageAckList);

        for(int i = 0; i < messageAckList.ack_size(); i++) {
            const Ack::MessageAck &messageAck = messageAckList.ack(i);


            const std::string &ackStr = messageAck.SerializeAsString();
            QByteArray ackByte(ackStr.c_str(), ackStr.size());

            SkpInsKey_t insKey;
            insKey.ins = messageAck.class_();
            insKey.clientMark = 0;

            if(m_hashAck.find(insKey) == m_hashAck.end()) {
                assert(false);
            }

            //std::function<void(QByteArray &)> func = m_hashAck.value(insKey);
            std::function<void(QByteArray &)> func = m_hashAck[insKey];
            func(ackByte);
        }

    });
}

void SkpManage::registerCallbackRealTime(QByteArray &bytes)
{
//    while(m_hashAck.isEmpty()) {
//        this->thread()->sleep(1);
//    }

    while(m_hashAck.empty()) {
        this->thread()->sleep(1);
    }

    SkpLocalTLV tlvResponse;
    tlvResponse.skp_parse_buffer(bytes.data(), bytes.size());

    Message::MessageData messageData;
    tlvResponse.skp_Parse_message(&messageData);

    SkpInsKey_t insKey;
    insKey.ins = messageData.class_();
    insKey.clientMark = 0;

    if(m_hashAck.find(insKey) == m_hashAck.end()) {
        assert(false);
    }

    QByteArray messageByte((char *)tlvResponse.byte_tlv(),tlvResponse.tlv_size());

    //std::function<void(QByteArray &)> func = m_hashAck.value(insKey);
    std::function<void(QByteArray &)> func = m_hashAck[insKey];
    func(messageByte);
}
