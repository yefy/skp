#include "skp_local_ack.h"
#include "ack.pb.h"
#include "message.pb.h"
#include "skp_local_key.h"
#include "skp_public_ins.h"
#include "skp_public_message_class.h"

SkpLocalAck::SkpLocalAck(int index, SkpObject *parent) :
    SkpLocalProtocal(index, parent)
{
}

SkpLocalAck::~SkpLocalAck()
{

}

bool SkpLocalAck::skp_dispatch()
{
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)m_tlvRequest->Head();
    switch (headProtocal->orderType) {
    case OrderTypeRequest:
    {
        switch (headProtocal->orderRequst) {
        case OrderLocalAckList:
            skp_request_ack_list();
            break;
        default:
            SKP_ASSERT(skp_false);
            break;
        }

        break;
    }

    case OrderTypeServer:
    {
        switch (headProtocal->orderServer) {
        case OrderLocalAck:
            skp_server_ack();
            break;
        case OrderLocalRealTime:
            skp_server_real_time();
            break;
        default:
            SKP_ASSERT(skp_false);
            break;
        }

        break;
    }

    default:
        SKP_ASSERT(skp_false);
        break;
    }

    return skp_true;
}

void SkpLocalAck::skp_request_ack_list()
{
    printf("get ack list***********************************\n");
    Ack::MessageAckList ackList;
    skpRedisGetAck(ackList);
    m_tlvResponse->skp_append_messge(ackList.ByteSize(), &ackList);
}

void SkpLocalAck::skp_server_ack()
{
    printf("add skp_server_ack \n");

    Ack::MessageAck messageAck;
    m_tlvRequest->skp_parse_message(&messageAck);

    skpRedisAddAck(messageAck);

    SkpKey(loginKey, KeyClass_Login, KeyType_Login);
    SkpPassage(passageCommon, m_recv, m_recv, MessageClass, MessagePassage);
    int valueSize;
    SkpLoginSession_t *loginSession = (SkpLoginSession_t *)skpGetRedisHash((char *)&passageCommon, SkpPassage_length, (char *)&loginKey, SkpKey_Length, valueSize);
    if(valueSize <= 0) {
        printf("user not login \n");
        return;
    }

    printf("redis m_proxy = %lld, m_proxyAddrSocket = %lld \n", loginSession->proxy, loginSession->proxyAddrSocket);

    m_tlvResponse->skp_set_proxy(loginSession->proxy);
    m_tlvResponse->skp_set_proxy_socket_addr(loginSession->proxyAddrSocket);
    m_tlvResponse->skp_set_client_mark(0);

    skp_server_reply();

}

void SkpLocalAck::skp_server_real_time()
{
    printf("add skp_server_real_time \n");

    Message::MessageData messageData;
    m_tlvRequest->skp_parse_message(&messageData);

    SkpKey(loginKey, KeyClass_Login, KeyType_Login);
    SkpPassage(passageCommon, m_recv, m_recv, MessageClass, MessagePassage);
    int valueSize;
    SkpLoginSession_t *loginSession = (SkpLoginSession_t *)skpGetRedisHash((char *)&passageCommon, SkpPassage_length, (char *)&loginKey, SkpKey_Length, valueSize);
    if(valueSize <= 0) {
        printf("user not login \n");
        return;
    }

    printf("redis m_proxy = %lld, m_proxyAddrSocket = %lld \n", loginSession->proxy, loginSession->proxyAddrSocket);

    m_tlvResponse->skp_set_proxy(loginSession->proxy);
    m_tlvResponse->skp_set_proxy_socket_addr(loginSession->proxyAddrSocket);
    m_tlvResponse->skp_set_client_mark(0);
    m_tlvResponse->skp_append_messge(messageData.ByteSize(), &messageData);

    skp_server_reply();
}
