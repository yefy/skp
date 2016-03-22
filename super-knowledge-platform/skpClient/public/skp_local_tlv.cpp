#include "skp_local_tlv.h"
#include "skp_public_transfer_protocal.h"
#include "google/protobuf/message_lite.h"

SkpLocalTLV::SkpLocalTLV() :
    SkpTLV(),
    m_orderType(OrderTypeRequest),
    m_proxyID(0)
{
}

SkpLocalTLV::SkpLocalTLV(char *pBuffer, int64 nSize) :
    SkpTLV(pBuffer, nSize),
    m_orderType(OrderTypeRequest),
    m_proxyID(0)
{

}

SkpLocalTLV::SkpLocalTLV(char *pBuffer, int64 nSize, int tlvSize, int headSize) :
    SkpTLV(pBuffer, nSize, tlvSize, headSize),
    m_orderType(OrderTypeRequest),
    m_proxyID(0)
{

}

void SkpLocalTLV::skp_parse_buffer(const char *data, int size)
{
    SkpProtocalHead *protocalHead = (SkpProtocalHead *)data;

    init(const_cast<char *> (data), size, protocalHead->dataSize, protocalHead->headSize);
}

void SkpLocalTLV::skp_parse_buffer(const QByteArray &bytes)
{
    QByteArray &tempBytes = const_cast<QByteArray &>(bytes);
    SkpProtocalHead *protocalHead = (SkpProtocalHead *)tempBytes.data();

    init(tempBytes.data(), tempBytes.size(), protocalHead->dataSize, protocalHead->headSize);
}

void SkpLocalTLV::skp_append_messge(int size, google::protobuf::MessageLite *lite)
{
    appen(size);
    lite->SerializeToArray(seek_value(), size);
}

void SkpLocalTLV::skp_Parse_message(google::protobuf::MessageLite *lite)
{
    lite->ParseFromArray(byte_tlv(), tlv_size());
}

void SkpLocalTLV::skp_set_status(uint16 status, uint16 error)
{
    SKP_ASSERT(is_Head());
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)Head();
    headProtocal->status = status;
    headProtocal->error = error;
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

uint64 SkpLocalTLV::skp_send()
{
    SKP_ASSERT(is_Head());
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)Head();
    return headProtocal->send;
}

void SkpLocalTLV::skp_set_recv(uint64 code)
{
    SKP_ASSERT(is_Head());
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)Head();
    headProtocal->recv = code;
}

uint64 SkpLocalTLV::skp_recv()
{
    SKP_ASSERT(is_Head());
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)Head();
    return headProtocal->recv;
}

void SkpLocalTLV::skp_set_ins(uint64 ins)
{
    SKP_ASSERT(is_Head());
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)Head();
    if(m_orderType == OrderTypeServer) {
        headProtocal->orderServer = ins;
    } else {
        headProtocal->orderRequst = ins;
    }

}

void SkpLocalTLV::skp_set_order_type(uint8 orderType)
{
    m_orderType = orderType;
}

void SkpLocalTLV::skp_set_proxy(uint32 proxyID)
{
    m_proxyID = proxyID;
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


void SkpLocalTLV::skp_start()
{
    reset(sizeof(SkpProtocalHead));
}

bool SkpLocalTLV::skp_post()
{
    SKP_ASSERT(is_Head());
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)Head();
    if(m_orderType > 0) {
            headProtocal->orderType = m_orderType;
    }

    if(m_proxyID > 0) {
            headProtocal->proxyID = m_proxyID;
    }

    headProtocal->headSize = sizeof(SkpProtocalHead);
    headProtocal->dataSize = tlv_size();

}
