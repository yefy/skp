#ifndef SKP_LOCAL_PROTOCAL_H
#define SKP_LOCAL_PROTOCAL_H

#include "skpObject.h"
#include "skpUtility.h"
#include "skp_local_tlv.h"
#include "skp_public_transfer_protocal.h"
#include "skpHiredis.h"
#include "skp_mysql_conn.h"

class skpThread;

namespace Message {
class MessageDataList;
}

namespace Ack {
class MessageAck;
class MessageAckList;
}

class SkpLocalProtocal : public SkpObject
{
public:
    SkpLocalProtocal(int index, SkpObject *parent);
    virtual ~SkpLocalProtocal();
    virtual bool skp_dispatch() = 0;
    bool skp_parse_protocal(void *head, int headSize, void *data, int dataSize);
    void skp_server_reply();
    int m_index;
    void *m_head;
    int m_headSize;
    uint64 m_sender;
    uint64 m_recv;
    uint64 m_proxy;
    uint64 m_proxyAddrSocket;
    uint32 m_param;
    bool m_isServerReply;
    SkpLocalSocketTcp *m_socketTcp;
    SkpLocalTLV *m_tlvRequest;
    SkpLocalTLV *m_tlvResponse;
    SkpLocalTLV *m_tlvServer;

    SkpHiredis *m_hiredis;
    SkpMysqlConn *m_mysqlConn;

    void skpRedisAddAck(Ack::MessageAck &messageAck);
    void skpRedisGetAck(Ack::MessageAckList &ackList);


    void skpRedisGetMessageSortedSet(uint64 friendCode, int class_, int passage, \
                                     int64 start, int64 end, bool isDel, \
                                Message::MessageDataList &messageDataList);

    void skpRedisGetMessageHash(uint64 friendCode, int class_, int passage,
                                std::vector<int64> &messageIndex, \
                                Message::MessageDataList &messageDataList);


    void skpMultiRedis();
    bool skpExecRedis();
    //*********************SortedSet******************
    void skpWatchRedisSortedSet(const char *passage, int passageSize);

    void skpAddRedisSortedSet(const char *passage, int passageSize, \
                              int64 time, const char *value, int valueSize);
    void skpGetRedisSortedSet(const char *passage, int passageSize, \
                              int64 start, int64 end);
    void skpDelRedisSortedSet(const char *passage, int passageSize, \
                              int64 start, int64 end);
    void skpExpireRedisSortedSet(const char *passage, int passageSize, \
                                 time_t expiration);

    //*********************hash******************
    void skpWatchRedisHash(const char *passage, int passageSize);

    void skpSetRedisHash(const char *passage, int passageSize, \
                         const char *key, int keySize, \
                         const char *value, int valueSize);
    char *skpGetRedisHash(const char *passage, int passageSize, \
                          char *key, int keySize, int &valueSize);
    void skpMGetRedisHash(const char *passage, int passageSize, \
                          std::vector<int64> &keys);
    void skpDelRedisHash(const char *passage, int passageSize, \
                         char *key, int keySize);
    int64 skpIncrByRedisHash(const char *passage, int passageSize, \
                             char *key, int keySize, int number);
    void skpExpireRedisHash(const char *passage, int passageSize, \
                            time_t expiration);

    void skpAck(int class_, uint64 friendCode, uint64 sender, int passage);

};

#endif // SKP_LOCAL_PROTOCAL_H
