#include "skp_local_protocal.h"
#include "skpThread.h"
#include "skp_public_ins.h"
#include "skp_public_message_class.h"
#include "skp_local_key.h"

#include "message.pb.h"
#include "ack.pb.h"
#include "skpAutoFree.h"

SkpLocalProtocal::SkpLocalProtocal(int index, SkpObject *parent) :
    SkpObject(parent)
{
    m_index = index;
    m_tlvRequest = new SkpLocalTLV();
    m_tlvResponse = new SkpLocalTLV(NULL, 0);
    m_tlvServer = new SkpLocalTLV(NULL, 0);

    m_hiredis = new SkpHiredis(this->thread());
    m_hiredis->connect("localhost", 6379, 1000);

    m_mysqlConn = new SkpMysqlConn();
    const char *server = "localhost";
    const char *user = "yefy";
    const char *password = "yfy@389";
    const char *database = "skp_origin";
    m_mysqlConn->skp_connect(server, user, password, database);
}

SkpLocalProtocal::~SkpLocalProtocal()
{
    skp_delete(m_tlvRequest);
    skp_delete(m_tlvResponse);
    skp_delete(m_tlvServer);
}

bool SkpLocalProtocal::skp_parse_protocal(void *head, int headSize, void *data, int dataSize)
{
    m_head = head;
    m_headSize = headSize;
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)data;
    m_sender = headProtocal->send;
    m_recv = headProtocal->recv;
    m_proxy = headProtocal->proxyID;
    m_proxyAddrSocket = headProtocal->proxyAddrSocket;
    m_param = headProtocal->param;
    m_socketTcp = NULL;
    m_isServerReply = skp_false;

    //printf("sender = %lld, receiver = %lld \n", m_sender, m_recv);

    if(m_head) {
        long socketAddr;
        memcpy(&socketAddr, m_head, sizeof(long));
        m_socketTcp = (SkpLocalSocketTcp *)socketAddr;
    }
    SKP_ASSERT(headProtocal->orderType != OrderTypeResponse);

    m_tlvRequest->skp_parse_buffer((char *)data, dataSize);

    m_tlvResponse->reset(sizeof(SkpProtocalHead));
    m_tlvResponse->skp_set_order_type(OrderTypeResponse);
    m_tlvResponse->skp_set_protocal_head(headProtocal, OrderTypeResponse);
    if(headProtocal->orderType == OrderTypeServer) {
        m_tlvResponse->skp_set_send(m_sender);
        m_tlvResponse->skp_set_recv(m_recv);
    }

    m_tlvServer->reset(sizeof(SkpProtocalHead));
    m_tlvServer->skp_set_order_type(OrderTypeServer);
    m_tlvServer->skp_set_protocal_head(headProtocal, OrderTypeServer);
    m_tlvServer->skp_set_send(m_recv);
    m_tlvServer->skp_set_recv(m_recv);

    skp_dispatch();

    if(headProtocal->orderType == OrderTypeRequest) {
        m_tlvResponse->skp_post(m_socketTcp);
    }

    if(m_isServerReply) {
        m_tlvResponse->skp_post();
    }

    return skp_true;
}

void SkpLocalProtocal::skp_server_reply()
{
    m_isServerReply = skp_true;
}

void SkpLocalProtocal::skpRedisAddAck(Ack::MessageAck &messageAck)
{
    const std::string &ackStr = messageAck.SerializeAsString();

    SkpPassage(passageAck, m_recv, m_recv, MessageClass_Ack, MessagePassage_Ack);
    skpWatchRedisSortedSet((char *)&passageAck, SkpPassage_length);

    SkpKey(AckIndexKey, KeyClass_Ack, KeyType_AckIndex);
    SkpPassage(passageAckIndex, m_recv, m_recv, MessageClass, MessagePassage);
    int number = 1;
    int64 index = skpIncrByRedisHash((char *)&passageAckIndex, SkpPassage_length, (char *)&AckIndexKey, SkpKey_Length, number);
    printf("skpRedisAddAck ack index = %lld, friend = %lld \n ", index, messageAck.friend_());
    skpMultiRedis();
    skpAddRedisSortedSet((char *)&passageAck, SkpPassage_length, index, ackStr.c_str(), ackStr.size());
    skpExecRedis();
}

void SkpLocalProtocal::skpRedisGetAck(Ack::MessageAckList &ackList)
{
    SkpKey(AckIndexKey, KeyClass_Ack, KeyType_AckIndex);

    SkpPassage(passageCommon, m_recv, m_recv, MessageClass, MessagePassage);

    int indexLength = 0;
    char *pIndex = skpGetRedisHash((char *)&passageCommon, SkpPassage_length, (char *)&AckIndexKey, SkpKey_Length, indexLength);
    if(!pIndex)
        return;

    std::string str(pIndex, indexLength);
    int64 index = atoll(str.c_str());
    printf("skpRedisGetAck ack index = %lld \n ", index);

    int64 start = 0;
    int64 end = index;

    SkpPassage(passageAck, m_recv, m_recv, MessageClass_Ack, MessagePassage_Ack);
    skpGetRedisSortedSet((char *)&passageAck, SkpPassage_length, start, end);

    printf("skpRedisGetAck :type = %d, elements = %d \n", m_hiredis->m_reply->type, m_hiredis->m_reply->elements);
    if(m_hiredis->m_reply->type == REDIS_REPLY_ARRAY) {
        for(uint i = 0; i < m_hiredis->m_reply->elements; i++) {
            struct redisReply *element = m_hiredis->m_reply->element[i];
            if(!element || element->type == REDIS_REPLY_NIL || !element->len) continue;
            Ack::MessageAck *ack = ackList.add_ack();
            ack->ParseFromArray(element->str, element->len);
        }
    }

    skpDelRedisSortedSet((char *)&passageAck, SkpPassage_length, start, end);
}

void SkpLocalProtocal::skpRedisGetMessageSortedSet(uint64 friendCode, int class_, int passage, \
                                 int64 start, int64 end, bool isDel, \
                            Message::MessageDataList &messageDataList)
{
    SkpPassage(passageMessage, m_recv, friendCode, class_, passage);
    skpGetRedisSortedSet((char *)&passageMessage, SkpPassage_length, start, end);
    printf("skpRedisGetMessageSortedSet :type = %d, elements = %d \n", m_hiredis->m_reply->type, m_hiredis->m_reply->elements);
    if(m_hiredis->m_reply->type == REDIS_REPLY_ARRAY) {
        for(uint i = 0; i < m_hiredis->m_reply->elements; i++) {
            struct redisReply *element = m_hiredis->m_reply->element[i];
            if(!element || element->type == REDIS_REPLY_NIL || !element->len) continue;
            Message::MessageData *messageData = messageDataList.add_data();
            messageData->ParseFromArray(element->str, element->len);
        }
    }
    if(isDel)
        skpDelRedisSortedSet((char *)&passageMessage, SkpPassage_length, start, end);
}

void SkpLocalProtocal::skpRedisGetMessageHash(uint64 friendCode, int class_, int passage,
                            std::vector<int64> &messageIndex, \
                            Message::MessageDataList &messageDataList)
{
    SkpPassage(passageMessage, m_recv, friendCode, class_, passage);
    skpMGetRedisHash((char *)&passageMessage, SkpPassage_length, messageIndex);

    printf("skpRedisGetMessageHash :type = %d, elements = %d, integer = %lld, len = %d \n",
           m_hiredis->m_reply->type, m_hiredis->m_reply->elements, m_hiredis->m_reply->integer, m_hiredis->m_reply->len);
    if(m_hiredis->m_reply->type == REDIS_REPLY_ARRAY) {
        for(uint i = 0; i < m_hiredis->m_reply->elements; i++) {
            struct redisReply *element = m_hiredis->m_reply->element[i];
            //printf("element :type = %d, elements = %d, integer = %lld, len = %d \n",
            //       element->type, element->elements, element->integer, element->len);
            if(!element || element->type == REDIS_REPLY_NIL || !element->len) continue;
            Message::MessageData *messageData = messageDataList.add_data();
            messageData->ParseFromArray(element->str, element->len);

            //int64 index = messageData->index();
            //int class_ = messageData->class_();
            //int type = messageData->type();
            //printf("index = %lld, class_ = %d, type = %d \n", index, class_, type);
        }
    }
}


void SkpLocalProtocal::skpMultiRedis()
{
    m_hiredis->command("MULTI");
}

bool SkpLocalProtocal::skpExecRedis()
{
    m_hiredis->command("EXEC");
    if(!m_hiredis->m_reply || m_hiredis->m_reply->type == REDIS_REPLY_NIL)
        return skp_false;

    return skp_true;
}

void SkpLocalProtocal::skpWatchRedisSortedSet(const char *passage, int passageSize)
{
    m_hiredis->command("WATCH SortedSet%b", passage, passageSize);
}

void SkpLocalProtocal::skpAddRedisSortedSet(const char *passage, int passageSize, \
                                            int64 time, const char *value, int valueSize)
{
    m_hiredis->command("ZADD SortedSet%b %lld %b", passage, passageSize, time, value, valueSize);
}

void SkpLocalProtocal::skpGetRedisSortedSet(const char *passage, int passageSize, \
                                            int64 start, int64 end)
{
    m_hiredis->command("ZRANGEBYSCORE SortedSet%b %lld %lld", passage, passageSize, start, end);
}

void SkpLocalProtocal::skpDelRedisSortedSet(const char *passage, int passageSize, \
                                            int64 start, int64 end)
{
    m_hiredis->command("zremrangebyscore SortedSet%b %lld %lld", passage, passageSize, start, end);
}

void SkpLocalProtocal::skpExpireRedisSortedSet(const char *passage, int passageSize, \
                                               time_t expiration)
{
    m_hiredis->command("EXPIRE SortedSet%b %d", passage, passageSize, expiration);

}

void SkpLocalProtocal::skpWatchRedisHash(const char *passage, int passageSize)
{
    m_hiredis->command("WATCH Hash%b", passage, passageSize);
}

void SkpLocalProtocal::skpSetRedisHash(const char *passage, int passageSize, \
                                       const char *key, int keySize, \
                                       const char *value, int valueSize)
{
    m_hiredis->command("HSET Hash%b %b %b", passage, passageSize, key, keySize, value, valueSize);
}

char *SkpLocalProtocal::skpGetRedisHash(const char *passage, int passageSize, \
                                        char *key, int keySize, int &valueSize)
{
    m_hiredis->command("HGET Hash%b %b", passage, passageSize, key, keySize);

    printf("skpGetRedisHash type = %d, len = %d \n", m_hiredis->m_reply->type, m_hiredis->m_reply->len);

    printf("integer = %lld, len = %d \n", m_hiredis->m_reply->integer, m_hiredis->m_reply->len);
    valueSize = m_hiredis->m_reply->len;
    return m_hiredis->m_reply->str;
}

void SkpLocalProtocal::skpMGetRedisHash(const char *passage, int passageSize, \
                                        std::vector<int64> &keys)
{
    std::string keyHash("Hash");
    keyHash.append(std::string(passage, passageSize));
    m_hiredis->m_argv->reset();
    m_hiredis->m_argv->addKey("HMGET");
    m_hiredis->m_argv->addKey(keyHash);
    for(uint i = 0; i < keys.size(); i++) {
        const std::string &key = skp_itos(keys.at(i));
        m_hiredis->m_argv->addKey(key);
    }

    m_hiredis->command();

}

void SkpLocalProtocal::skpDelRedisHash(const char *passage, int passageSize, \
                                       char *key, int keySize)
{
    m_hiredis->command("HDEL Hash%b %b", passage, passageSize, key, keySize);
}

int64 SkpLocalProtocal::skpIncrByRedisHash(const char *passage, int passageSize, \
                                           char *key, int keySize, int number)
{
    m_hiredis->command("HINCRBY Hash%b %b %d", passage, passageSize, key, keySize, number);
    printf("skpIncrByRedisHash type = %d, len = %d \n", m_hiredis->m_reply->type, m_hiredis->m_reply->len);

    printf("integer = %lld, len = %d \n", m_hiredis->m_reply->integer, m_hiredis->m_reply->len);
    return m_hiredis->m_reply->integer;
}

void SkpLocalProtocal::skpExpireRedisHash(const char *passage, int passageSize, \
                                          time_t expiration)
{
    m_hiredis->command("EXPIRE Hash%b %d", passage, passageSize, expiration);
}

void SkpLocalProtocal::skpAck(int class_, uint64 friendCode, uint64 sender, int passage)
{
    Ack::MessageAck messageAck;
    messageAck.set_class_(class_);
    messageAck.set_friend_(sender);
    messageAck.set_passage(passage);
    messageAck.set_from(m_recv);

    m_tlvServer->skp_set_ins(OrderLocalAck);
    m_tlvServer->skp_set_recv(friendCode);
    m_tlvServer->skp_append_messge(messageAck.ByteSize(), &messageAck);
    m_tlvServer->skp_post();
}
