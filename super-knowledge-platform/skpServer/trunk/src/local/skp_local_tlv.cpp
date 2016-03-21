#include "skp_local_tlv.h"
#include "skp_public_transfer_protocal.h"
#include "skp_local_manage_route.h"
#include "skp_local_socket_tcp.h"

#include "google/protobuf/message_lite.h"

SkpLocalTLV::SkpLocalTLV() :
    SkpTLV(),
    m_orderType(0),
    m_proxyID(0)
{
}

SkpLocalTLV::SkpLocalTLV(char *pBuffer, int64 nSize) :
    SkpTLV(pBuffer, nSize),
    m_orderType(0),
    m_proxyID(0)
{

}

SkpLocalTLV::SkpLocalTLV(char *pBuffer, int64 nSize, int tlvSize, int headSize) :
    SkpTLV(pBuffer, nSize, tlvSize, headSize),
    m_orderType(0),
    m_proxyID(0)
{

}

void SkpLocalTLV::skp_set_status(uint16 status, uint16 error)
{
    SKP_ASSERT(is_Head());
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)Head();
    headProtocal->status = status;
    headProtocal->error = error;
}

void SkpLocalTLV::skp_parse_buffer(char *data, int size)
{
    SkpProtocalHead *protocalHead = (SkpProtocalHead *)data;

    init(data, size, protocalHead->dataSize, protocalHead->headSize);
}

void SkpLocalTLV::skp_append_messge(int size, google::protobuf::MessageLite *lite)
{
    appen(size);
    lite->SerializeToArray(seek_value(), size);
}

void SkpLocalTLV::skp_append_messge(const std::string &message)
{
    appen(message.size());
    memcpy(seek_value(), message.c_str(), message.size());
}

void SkpLocalTLV::skp_parse_message(google::protobuf::MessageLite *lite)
{
    lite->ParseFromArray(byte_tlv(), tlv_size());
}

uint16 SkpLocalTLV::skp_status()
{
    SKP_ASSERT(is_Head());
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)Head();
    return headProtocal->status;

}

uint16 SkpLocalTLV::skp_err()
{
    SKP_ASSERT(is_Head());
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)Head();
    return headProtocal->error;

}

void SkpLocalTLV::skp_set_protocal_head(SkpProtocalHead *headProtocal, uint8 orderType)
{
    SKP_ASSERT(is_Head());
    SkpProtocalHead *head = (SkpProtocalHead *)Head();
    head->headSize = sizeof(SkpProtocalHead);
    head->orderType = orderType;
    head->isConnHead = 0;
    head->dataSize = 0;
    head->send = headProtocal->recv;
    head->recv = headProtocal->send;
    head->proxyID = headProtocal->proxyID;
    head->proxyAddrSocket = headProtocal->proxyAddrSocket;
    head->status = 0;
    head->error = 0;
    head->clientMark = headProtocal->clientMark;
    head->orderRequst = headProtocal->orderRequst;
    head->orderServer = headProtocal->orderServer > 0? headProtocal->orderServer : headProtocal->orderRequst;

}

void SkpLocalTLV::skp_set_send(uint64 code)
{
    SKP_ASSERT(is_Head());
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)Head();
    headProtocal->send = code;
}

void SkpLocalTLV::skp_set_recv(uint64 code)
{
    SKP_ASSERT(is_Head());
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)Head();
    headProtocal->recv = code;
}

void SkpLocalTLV::skp_set_ins(uint64 ins)
{
    SKP_ASSERT(is_Head());
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)Head();
//    if(m_orderType == OrderTypeServer) {
//        headProtocal->orderServer = ins;
//    } else {
//        headProtocal->orderRequst = ins;
//    }
    headProtocal->orderRequst = ins;
    headProtocal->orderServer = ins;

}

void SkpLocalTLV::skp_set_order_type(uint8 orderType)
{
    m_orderType = orderType;
}

void SkpLocalTLV::skp_set_proxy(uint32 proxyID)
{
    m_proxyID = proxyID;
}

void SkpLocalTLV::skp_set_proxy_socket_addr(uint64 proxyAddrSocket)
{
    SKP_ASSERT(is_Head());
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)Head();
    headProtocal->proxyAddrSocket = proxyAddrSocket;
}

void SkpLocalTLV::skp_set_client_mark(uint64 clientMark)
{
    SKP_ASSERT(is_Head());
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)Head();
    headProtocal->clientMark = clientMark;
}

void SkpLocalTLV::skp_set_param(uint32 param)
{
    SKP_ASSERT(is_Head());
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)Head();
    headProtocal->param = param;
}

SkpProtocalHead *SkpLocalTLV::skp_head()
{
    SKP_ASSERT(is_Head());
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)Head();
    return headProtocal;
}


bool SkpLocalTLV::skp_post(SkpLocalSocketTcp *socketTcp)
{
    SKP_ASSERT(is_Head());
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)Head();
    if(m_orderType > 0) {
        headProtocal->orderType = m_orderType;
    }
    if(m_proxyID > 0) {
        headProtocal->proxyID = m_proxyID;
    }
    //printf("send = %lld, recv = %lld \n", headProtocal->send, headProtocal->recv);
    headProtocal->headSize = sizeof(SkpProtocalHead);
    headProtocal->dataSize = tlv_size();

    if(headProtocal->orderType == OrderTypeServer || !socketTcp) {
        SkpLocalManageRoute::instance()->routeChan(byte(), use_size());
    } else {
        if(socketTcp) {
             socketTcp->appendWriteChan(byte(), use_size());
             socketTcp->release();
             return skp_true;
        }
    }

    return skp_true;
}
