#include "skp_local_chat.h"
#include "skpHiredis.h"
#include "message.pb.h"
#include "skp_public_ins.h"
#include "skp_public_message_class.h"
#include "skp_local_key.h"

SkpLocalChat::SkpLocalChat(int index, SkpObject *parent) :
    SkpLocalProtocal(index, parent)
{
}

SkpLocalChat::~SkpLocalChat()
{

}

bool SkpLocalChat::skp_dispatch()
{
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)m_tlvRequest->Head();
    switch (headProtocal->orderType) {
    case OrderTypeRequest:
    {
        switch (headProtocal->orderRequst) {
        case OrderLocalChat:
            skp_request_chat();
            break;
        case OrderLocalMessageGetMessage:
            skp_request_get_message();
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
        case OrderLocalChat:
            skp_server_chat();
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

void SkpLocalChat::skp_request_chat()
{
    printf("skp_request_chat ************************************ \n");

    Message::MessageChat chat;
    m_tlvRequest->skp_parse_message(&chat);

    const std::string &chatStr = chat.data();
    uint64 receiver = chat.receiver();
    printf("skp_request_chat receiver = %lld \n", receiver);

    int64 time = skp_get_system_time_ms_ex();

    Message::MessageData data;
    data.set_time(time);
    data.set_class_(MessageClass_Chat);
    data.set_type(MessageType_Chat);
    data.set_sender(m_sender);
    data.set_receiver(receiver);
    data.set_friend_(m_recv);
    data.set_data_1(chatStr);

    int64 index;

    do {
        SkpPassage(passageChat, m_recv, receiver, MessageClass_Chat, MessagePassage_Chat);
        skpWatchRedisHash((char *)&passageChat, SkpPassage_length);

        SkpKey_MessageEx(messageIndexKey, KeyClass_Chat, KeyType_ChatIndex, m_recv, receiver, MessageClass_Chat, MessagePassage_Chat);
        SkpPassage(passageCommon, m_recv, m_recv, MessageClass, MessagePassage);
        int number = 1;
        index = skpIncrByRedisHash((char *)&passageCommon, SkpPassage_length, (char *)&messageIndexKey, SkpKey_MessageExLength, number);

        data.set_index(index);
        data.set_real_index(index);
        const std::string &dataStr = data.SerializeAsString();
        const std::string &keyStr = skp_itos(index);

        skpMultiRedis();
        skpSetRedisHash((char *)&passageChat, SkpPassage_length, keyStr.c_str(), keyStr.size(), dataStr.c_str(), dataStr.size());
    } while(!skpExecRedis());

    printf("index = %lld \n", index);

    Message::MessageData friendData;
    friendData.set_real_index(index);
    friendData.set_class_(MessageClass_Chat);
    friendData.set_type(MessageType_Chat);
    friendData.set_sender(m_sender);
    friendData.set_receiver(receiver);
    friendData.set_friend_(m_recv);

    m_tlvServer->skp_set_recv(receiver);
    m_tlvServer->skp_append_messge(friendData.ByteSize(), &friendData);
    m_tlvServer->skp_post();

    data.clear_data_1();
    m_tlvResponse->skp_append_messge(data.ByteSize(), &data);

    //skp_insert_chat_virtual(m_recv, friendCode, timeVirtual, class_, type, time, m_recv);
    //skp_insert_chat(m_recv, friendCode, time, class_, type, str);
}

void SkpLocalChat::skp_request_get_message()
{
    printf("skp_request_get_message************************************ \n");

    Message::MessageRequest messageRequest;
    m_tlvRequest->skp_parse_message(&messageRequest);

    int class_ = messageRequest.class_();
    int passage = messageRequest.passage();
    uint64 friendCode = messageRequest.friend_();
    printf("class_ = %d, passage = %d, friendCode %lld\n", class_, passage, friendCode);

    struct timeval startTime = skp_gettimeofday();
    Message::MessageDataList messageDataList;

    if(class_ == MessageClass_Chat && passage == MessagePassage_Chat) {
        std::vector<int64> messageIndex;
        for(int i = 0; i < messageRequest.index_size(); i++)
        {
            messageIndex.push_back(messageRequest.index(i));
        }

        for(int i = 0; i < messageRequest.index_range_size(); i++)
        {
            Message::MessageRequestRange *indexRange = messageRequest.mutable_index_range(i);
            int64 start = indexRange->start();
            int64 end = indexRange->end();
            int32 length = indexRange->length();

            if(!end) {
                SkpKey_MessageEx(messageIndexKey, KeyClass_Chat, KeyType_ChatIndex, m_recv, friendCode, class_, passage);
                SkpPassage(passageCommon, m_recv, m_recv, MessageClass, MessagePassage);
                int size = 0;
                char *value = skpGetRedisHash((char *)&passageCommon, SkpPassage_length, (char *)&messageIndexKey, SkpKey_MessageExLength, size);
                end = skp_stoi(value, size);
                printf("****end = %lld \n", end);
            }

            if(!end)
                return;

            for(int j = end; j > start; j--)
            {
                if(length > 0 && (end - j) >= length)
                    break;

                messageIndex.push_back(j);
            }
        }

        std::sort(messageIndex.begin(), messageIndex.end(),std::greater<int64>());
        if(messageIndex.size() > 100)
            messageIndex.resize(100);

        for(uint i = 0; i < messageIndex.size(); i++) {
            printf("messageIndex = %lld \n", messageIndex.at(i));
        }

        skpRedisGetMessageHash(friendCode, class_, passage, messageIndex, messageDataList);


    } else if(class_ == MessageClass_MemoryFile && passage == MessagePassage_MemoryFile) {

        SkpKey_MessageEx(fileDataIndexKey, KeyClass_Chat, KeyType_ChatIndex, m_recv, friendCode, MessageClass_MemoryFile, MessagePassage_MemoryFile);

        SkpPassage(passageCommon, m_recv, m_recv, MessageClass, MessagePassage);

        int indexLength = 0;
        char *pIndex = skpGetRedisHash((char *)&passageCommon, SkpPassage_length, (char *)&fileDataIndexKey, SkpKey_MessageExLength, indexLength);
        if(!pIndex)
            return;

        std::string str(pIndex, indexLength);
        int64 index = atoll(str.c_str());
        printf("MessagePassage_MemoryFile index = %lld \n ", index);

        int64 start = 0;
        int64 end = index;

        skpRedisGetMessageSortedSet(friendCode, class_, passage, start, end, skp_true, messageDataList);
    }
    m_tlvResponse->skp_append_messge(messageDataList.ByteSize(), &messageDataList);


    int64 diffTime = skp_diff_time(startTime);
    printf("*********skp_request_get_message = %lld \n", diffTime);
}

void SkpLocalChat::skp_server_chat()
{
    printf("skp_server_chat ************************************ \n");

    Message::MessageData friendData;
    m_tlvRequest->skp_parse_message(&friendData);

    int64 time = skp_get_system_time_ms_ex();
    friendData.set_time(time);
    int64 index  = 0;

    do {
        SkpPassage(passageChat, m_recv, m_sender, MessageClass_Chat, MessagePassage_Chat);
        skpWatchRedisHash((char *)&passageChat, SkpPassage_length);

        SkpKey_MessageEx(messageIndexKey, KeyClass_Chat, KeyType_ChatIndex, m_recv, m_sender, MessageClass_Chat, MessagePassage_Chat);
        SkpPassage(passageCommon, m_recv, m_recv, MessageClass, MessagePassage);
        int number = 1;
        index = skpIncrByRedisHash((char *)&passageCommon, SkpPassage_length, (char *)&messageIndexKey, SkpKey_MessageExLength, number);


        friendData.set_index(index);
        const std::string &dataStr = friendData.SerializeAsString();
        const std::string &keyStr = skp_itos(index);

        skpMultiRedis();
        skpSetRedisHash((char *)&passageChat, SkpPassage_length, keyStr.c_str(), keyStr.size(), dataStr.c_str(), dataStr.size());
    } while(!skpExecRedis());

    printf("index = %lld \n", index);

    int class_ = MessageClass_Chat;
    int passage = MessagePassage_Chat;

    //skp_insert_chat_virtual(m_recv, friendCode, timeVirtual, class_, type, time, friendCode);

    skpAck(class_, m_recv, m_sender, passage);

    printf("ack \n");
}

void SkpLocalChat::skp_insert_chat(uint64 code, uint64 friendCode, uint64 messageCode, int class_, int type, std::string &data)
{
    return;

    struct timeval startTime = skp_gettimeofday();
    printf("skp_insert_chat start \n");
    const char *insertSql = "INSERT INTO table_chat(code, friendCode, messageTime, class, type, data) VALUES(?,?,?,?,?,?)";
    m_mysqlConn->skp_prepare(insertSql);
    m_mysqlConn->skp_param_large_int(code);
    m_mysqlConn->skp_param_large_int(friendCode);
    m_mysqlConn->skp_param_large_int(messageCode);
    m_mysqlConn->skp_param_int(class_);
    m_mysqlConn->skp_param_int(type);
    m_mysqlConn->skp_param_buffer(data.c_str(), data.size());
    int res = m_mysqlConn->skp_execute();
    SKP_ASSERT(res == AF::Mysql_Error_Success);
    int64 diffTime = skp_diff_time(startTime);
    printf("*****************************skp_insert_chat ok = %lld \n", diffTime);
    while(m_mysqlConn->skp_db_next()) {
        while(m_mysqlConn->skp_next()) {
            printf("insert ok ************ \n");
        }
    }
}

void SkpLocalChat::skp_insert_chat_virtual(uint64 code, uint64 friendCode, uint64 virtualTime, int class_, int type, uint64 messageTime, uint64 messageSender)
{
    return;

    struct timeval startTime = skp_gettimeofday();
    printf("skp_insert_chat_virtual start \n");
    const char *insertSql = "INSERT INTO table_chat_virtual(code, friendCode, virtualTime, class, type, messageTime, messageSender) VALUES(?,?,?,?,?,?,?)";
    m_mysqlConn->skp_prepare(insertSql);
    m_mysqlConn->skp_param_large_int(code);
    m_mysqlConn->skp_param_large_int(friendCode);
    m_mysqlConn->skp_param_large_int(virtualTime);
    m_mysqlConn->skp_param_int(class_);
    m_mysqlConn->skp_param_int(type);
    m_mysqlConn->skp_param_large_int(messageTime);
    m_mysqlConn->skp_param_large_int(messageSender);
    int res = m_mysqlConn->skp_execute();
    SKP_ASSERT(res == AF::Mysql_Error_Success);
    int64 diffTime = skp_diff_time(startTime);
    printf("*****************************skp_insert_chat_virtual ok = %lld \n", diffTime);
    while(m_mysqlConn->skp_db_next()) {
        while(m_mysqlConn->skp_next()) {
            printf("insert ok ************ \n");
        }
    }
}
