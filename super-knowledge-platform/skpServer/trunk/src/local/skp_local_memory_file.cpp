#include "skp_local_memory_file.h"
#include "skpMallocPoolEx.h"
#include "skp_local_status.h"
#include "skp_local_socket_tcp.h"
#include "skpMD5.h"
#include "memory_file.pb.h"
#include "skpHiredis.h"
#include "message.pb.h"
#include "skp_public_ins.h"
#include "skp_public_message_class.h"
#include "skp_local_key.h"
#include "skpAutoFree.h"


SkpLocalMemoryFile::SkpLocalMemoryFile(int index, SkpObject *parent) :
    SkpLocalProtocal(index, parent)
{
    m_lastTime = 0;
    m_ratebytes = 0;
    m_poolEx = new SkpMallocPoolEx();
}

SkpLocalMemoryFile::~SkpLocalMemoryFile()
{
    skp_delete(m_poolEx);
}

bool SkpLocalMemoryFile::skp_dispatch()
{
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)m_tlvRequest->Head();
    switch (headProtocal->orderType) {
    case OrderTypeRequest:
    {
        switch (headProtocal->orderRequst) {
        case OrderLocalMemoryFileIsExist:
            skp_request_is_exist();
            break;
        case OrderLocalMemoryFilePushRuest:
            skp_request_push_request();
            break;
        case OrderLocalMemoryFilePush:
            skp_request_push();
            break;
        case OrderLocalMemoryFilePull:
            skp_request_pull();
            break;
        case OrderLocalMemoryFilePushOK:
            skp_request_push_ok();
            break;
        case OrderLocalMemoryFileFileList:
            skp_request_file_list();
            break;
        case OrderLocalMemoryFileDelete:
            skp_request_delete();
            break;
        case OrderLocalMemoryFileToFriend:
            skp_request_file_to_friend();
            break;
        case OrderLocalMemoryFileToFriendReply:
            skp_request_file_to_friend_reply();
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
        case OrderLocalMemoryFileToFriend:
            //skp_server_file_to_friend();
            break;
        case OrderLocalMemoryFileToFriendReply:
            //skp_server_file_to_friend_reply();
            break;
        case OrderLocalMemoryFilePushRuest:
            //skp_server_push_request();
            break;
        case OrderLocalMemoryFilePush:
            //skp_server_push();
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

void SkpLocalMemoryFile::skp_request_is_exist()
{
    MemoryFile::MemoryFileMD5 fileMD5;
    m_tlvRequest->skp_parse_message(&fileMD5);

    SkpFileData_t *fileDataTemp = skp_is_exist_md5(fileMD5.md5());
    if(!fileDataTemp) {
        printf("md5 = %s not exist \n", fileMD5.md5().c_str());

        m_tlvResponse->skp_set_status(LocalStatusClassMemoryFile, LocalStatusTypeMemoryFileMD5);
        m_tlvResponse->skp_append_messge(fileMD5.ByteSize(), &fileMD5);
        return;
    }

    printf("md5 = %s exist \n", fileMD5.md5().c_str());

    MemoryFile::MemoryFileData fileData;
    fileData.set_md5(fileDataTemp->md5, sizeof(fileDataTemp->md5));
    fileData.set_name(fileDataTemp->name, fileDataTemp->nameSize);
    fileData.set_lenth(fileDataTemp->lenth);
    fileData.set_offset((uint64)fileDataTemp->offset);
    fileData.set_status(fileDataTemp->status);
    m_tlvResponse->skp_append_messge(fileData.ByteSize(), &fileData);

}

void SkpLocalMemoryFile::skp_request_push_request()
{
    MemoryFile::MemoryFileData fileData;
    m_tlvRequest->skp_parse_message(&fileData);

    //printf("skp_request_file_to_friend : name = %s, md5 = %s, length = %lld, friend = %lld \n", fileData.name().c_str(), fileData.md5().c_str(), fileData.lenth(), fileData.friend_());


    uint64 friendCode = fileData.friend_();

    SkpFileData_t *fileDataTemp = skp_is_exist_md5(fileData.md5());
    if(fileDataTemp) {
        printf("free file data \n");
        m_protocaltMap.erase(fileData.md5());
        skp_pool_free(m_poolEx, fileDataTemp);
    }

    printf("malloc file data \n");
    fileDataTemp = (SkpFileData_t *)skp_pool_calloc(m_poolEx, sizeof(SkpFileData_t) + fileData.lenth());
    fileDataTemp->lenth = fileData.lenth();
    fileDataTemp->offset = fileDataTemp->buffer;
    fileDataTemp->status = FileDataStatusPushing;
    memcpy(fileDataTemp->md5, fileData.md5().data(), fileData.md5().size());
    memcpy(fileDataTemp->name, fileData.name().data(), fileData.name().size());
    fileDataTemp->nameSize = fileData.name().size();
    printf("size = %d \n", fileData.name().size());
    printf("name 111 = %s \n", fileData.name().data());
    printf("name 222 = %s \n", fileDataTemp->name);
    m_protocaltMap.insert(std::pair<std::string, SkpFileData_t *>(fileData.md5(), fileDataTemp));


    fileData.clear_offset();
    fileData.clear_status();
    fileData.set_offset((uint64)fileDataTemp->offset);
    fileData.set_status(fileDataTemp->status);

    m_tlvResponse->skp_append_messge(fileData.ByteSize(), &fileData);

    if(friendCode) {

        fileData.clear_friend_();
        fileData.set_friend_(m_recv);
        const std::string &fileDataStr = fileData.SerializeAsString();

        skp_message_to_redis(friendCode, MessageType_MemoryFilePushRequest, fileDataStr);

    }

}

void SkpLocalMemoryFile::skp_request_push()
{
    MemoryFile::MemoryFileData fileData;
    m_tlvRequest->skp_parse_message(&fileData);

    uint64 lenth = fileData.lenth();
    uint64 offset = fileData.offset();
    uint64 friendCode = fileData.friend_();

    SkpFileData_t *fileDataTemp = skp_is_exist_md5(fileData.md5());
    if(!fileDataTemp) {
        printf("name = %s, md5 = %s not find \n", fileData.name().c_str(), fileData.md5().c_str());

        m_tlvResponse->skp_set_status(LocalStatusClassMemoryFile, LocalStatusTypeMemoryFileMD5);
        return;
    }

    if(fileDataTemp->status == FileDataStatusFinish) {
        printf("name = %s, md5 = %s is finish \n", fileData.name().c_str(), fileData.md5().c_str());

        m_tlvResponse->skp_set_status(LocalStatusClassMemoryFile, LocalStatusTypeMemoryFileFinish);
        return;
    }

    if(offset < (uint64)fileDataTemp->offset ||
            offset > (uint64)fileDataTemp->offset + fileDataTemp->lenth ||
            offset + lenth > (uint64)fileDataTemp->offset + fileDataTemp->lenth||
            lenth == 0) {
        printf("name = %s, md5 = %s offset or lenth error \n", fileData.name().c_str(), fileData.md5().c_str());

        m_tlvResponse->skp_set_status(LocalStatusClassMemoryFile, LocalStatusTypeMemoryFileSeek);
        return;
    }

    if(!m_lastTime) {
        m_lastTime = time(NULL);
    }
    if(time(NULL) - m_lastTime >= 1) {
        m_lastTime = time(NULL);
        uint64 _G = 1024 * 1024 * 1024 * 1024LL;
        std::string rateB("");
        uint64 rateSize = 0;
        if(m_ratebytes < 1024) {
            rateB = " kb";
            rateSize = m_ratebytes;
        } else if(m_ratebytes < 1024 * 1024) {
            rateB = " KB";
            rateSize = m_ratebytes / 1024;
        } else if(m_ratebytes < 1024 * 1024 * 1024) {
            rateB = " M";
            rateSize = m_ratebytes / (1024 * 1024);
        } else if(m_ratebytes < _G) {
            rateB = " G";
            rateSize = m_ratebytes / (1024 * 1024 * 1024);
        } else {
            rateB = " T";
            rateSize = m_ratebytes / _G;
        }
        printf("%lld %s \n", rateSize, rateB.c_str());
        m_ratebytes = 0;
    } else {
        m_ratebytes += fileData.buffer().size();
    }

    m_poolEx->write((void *)offset, fileData.buffer().data(), fileData.buffer().size());

    fileData.clear_buffer();
    m_tlvResponse->skp_append_messge(fileData.ByteSize(), &fileData);

    if(friendCode) {

        fileData.clear_friend_();
        fileData.set_friend_(m_recv);
        const std::string &fileDataStr = fileData.SerializeAsString();

        skp_message_to_redis(friendCode, MessageType_MemoryFilePush, fileDataStr);

    }
}

void SkpLocalMemoryFile::skp_request_pull()
{
    MemoryFile::MemoryFileData fileData;
    m_tlvRequest->skp_parse_message(&fileData);

    uint64 lenth = fileData.lenth();
    uint64 offset = fileData.offset();

    SkpFileData_t *fileDataTemp = skp_is_exist_md5(fileData.md5());
    if(!fileDataTemp) {
        printf("name = %s, md5 = %s not find \n", fileData.name().c_str(), fileData.md5().c_str());

        m_tlvResponse->skp_set_status(LocalStatusClassMemoryFile, LocalStatusTypeMemoryFileMD5);
        return;
    }

    if(/*fileDataTemp->status == FileDataStatusPushing ||*/
            fileDataTemp->status == FileDataStatusNull) {
        printf("name = %s, md5 = %s pushing \n", fileData.name().c_str(), fileData.md5().c_str());

        m_tlvResponse->skp_set_status(LocalStatusClassMemoryFile, LocalStatusTypeMemoryFilePushing);
        return;
    }

    if(offset < (uint64)fileDataTemp->offset ||
            offset > (uint64)fileDataTemp->offset + fileDataTemp->lenth ||
            offset + lenth > (uint64)fileDataTemp->offset + fileDataTemp->lenth||
            lenth == 0) {
        printf("name = %s, md5 = %s offset or lenth error \n", fileData.name().c_str(), fileData.md5().c_str());

        m_tlvResponse->skp_set_status(LocalStatusClassMemoryFile, LocalStatusTypeMemoryFileSeek);
        return;
    }

    if(!m_lastTime) {
        m_lastTime = time(NULL);
    }
    if(time(NULL) - m_lastTime >= 1) {
        m_lastTime = time(NULL);
        uint64 _G = 1024 * 1024 * 1024 * 1024LL;
        std::string rateB("");
        uint64 rateSize = 0;
        if(m_ratebytes < 1024) {
            rateB = " kb";
            rateSize = m_ratebytes;
        } else if(m_ratebytes < 1024 * 1024) {
            rateB = " KB";
            rateSize = m_ratebytes / 1024;
        } else if(m_ratebytes < 1024 * 1024 * 1024) {
            rateB = " M";
            rateSize = m_ratebytes / (1024 * 1024);
        } else if(m_ratebytes < _G) {
            rateB = " G";
            rateSize = m_ratebytes / (1024 * 1024 * 1024);
        } else {
            rateB = " T";
            rateSize = m_ratebytes / _G;
        }
        printf("%lld %s \n", rateSize, rateB.c_str());
        m_ratebytes = 0;
    } else {
        m_ratebytes += lenth;
    }

    fileData.clear_buffer();
    fileData.set_buffer((char *)offset, lenth);
    fileData.clear_status();
    fileData.set_status(fileDataTemp->status);

    m_tlvResponse->skp_append_messge(fileData.ByteSize(), &fileData);

}

void SkpLocalMemoryFile::skp_request_push_ok()
{
    MemoryFile::MemoryFileMD5 fileMD5;
    m_tlvRequest->skp_parse_message(&fileMD5);

    SkpFileData_t *fileData = skp_is_exist_md5(fileMD5.md5());
    if(!fileData) {
        printf("md5 = %s offset or lenth error \n", fileMD5.md5().c_str());

        m_tlvResponse->skp_set_status(LocalStatusClassMemoryFile, LocalStatusTypeMemoryFileMD5);
        return;
    }

    fileData->status = FileDataStatusFinish;

    m_tlvResponse->skp_append_messge(fileMD5.ByteSize(), &fileMD5);
}

SkpFileData_t *SkpLocalMemoryFile::skp_is_exist_md5(const std::string &md5)
{
    auto iter = m_protocaltMap.find(md5);
    if(iter == m_protocaltMap.end()) {
        return NULL;
    }

    SkpFileData_t *fileData = (SkpFileData_t *)iter->second;
    return fileData;
}

void SkpLocalMemoryFile::skp_message_to_redis(uint64 friendCode, int type, const std::string &fileDataStr)
{
    int64 time = skp_get_system_time_ms_ex();

    Message::MessageData data;
    data.set_time(time);
    data.set_class_(MessageClass_MemoryFile);
    data.set_type(type);
    data.set_sender(m_recv);
    data.set_receiver(friendCode);
    data.set_friend_(m_recv);
    data.set_data_1(fileDataStr);


    SkpPassage(passagefileData, m_recv, friendCode, MessageClass_MemoryFile, MessagePassage_MemoryFile);
    skpWatchRedisSortedSet((char *)&passagefileData, SkpPassage_length);

    SkpKey_MessageEx(fileDataIndexKey, KeyClass_Chat, KeyType_ChatIndex, m_recv, friendCode, MessageClass_MemoryFile, MessagePassage_MemoryFile);
    SkpPassage(passageCommon, m_recv, m_recv, MessageClass, MessagePassage);
    int number = 1;
    int64 index = skpIncrByRedisHash((char *)&passageCommon, SkpPassage_length, (char *)&fileDataIndexKey, SkpKey_MessageExLength, number);

    data.set_index(index);
    const std::string &fileDataMsg = data.SerializeAsString();

    printf("passagefileData index = %lld, friend = %lld \n ", index, friendCode);
    skpMultiRedis();
    skpAddRedisSortedSet((char *)&passagefileData, SkpPassage_length, index, fileDataMsg.c_str(), fileDataMsg.size());
    skpExecRedis();

    int class_ = MessageClass_MemoryFile;
    int passage = MessagePassage_MemoryFile;

    skpAck(class_, friendCode, m_recv, passage);
}

void SkpLocalMemoryFile::skp_request_file_list()
{
    MemoryFile::MemoryFileDataList fileDataList;
    for(auto iter = m_protocaltMap.begin(); iter != m_protocaltMap.end(); iter++) {
        SkpFileData_t *fileData_t = (SkpFileData_t *)iter->second;
        MemoryFile::MemoryFileData *fileData = fileDataList.add_file_data();
        fileData->set_md5(fileData_t->md5, sizeof(fileData_t->md5));
        fileData->set_name(fileData_t->name, fileData_t->nameSize);
        fileData->set_lenth(fileData_t->lenth);
        fileData->set_offset((uint64)fileData_t->offset);
        fileData->set_status(fileData_t->status);
    }

    m_tlvResponse->skp_append_messge(fileDataList.ByteSize(), &fileDataList);

}

void SkpLocalMemoryFile::skp_request_delete()
{
    MemoryFile::MemoryFileMD5 fileMD5;
    m_tlvRequest->skp_parse_message(&fileMD5);

    SkpFileData_t *fileDataTemp = skp_is_exist_md5(fileMD5.md5());
    if(!fileDataTemp) {
        printf("md5 = %s not exist \n", fileMD5.md5().c_str());

        m_tlvResponse->skp_set_status(LocalStatusClassMemoryFile, LocalStatusTypeMemoryFileMD5);
        m_tlvResponse->skp_append_messge(fileMD5.ByteSize(), &fileMD5);
        return;
    }
    m_protocaltMap.erase(fileMD5.md5());
    skp_pool_free(m_poolEx, fileDataTemp);

    printf("md5 = %s exist \n", fileMD5.md5().c_str());
    m_tlvResponse->skp_append_messge(fileMD5.ByteSize(), &fileMD5);
}

void SkpLocalMemoryFile::skp_request_file_to_friend()
{
    MemoryFile::MemoryFileToFriend fileData;
    m_tlvRequest->skp_parse_message(&fileData);

    printf("skp_request_file_to_friend : name = %s, md5 = %s, length = %lld, friend = %lld \n", fileData.name().c_str(), fileData.md5().c_str(), fileData.lenth(), fileData.friend_());

    uint64 friendCode = fileData.friend_();

    fileData.clear_friend_();
    fileData.set_friend_(m_recv);
    const std::string &fileDataStr = fileData.SerializeAsString();

    skp_message_to_redis(friendCode, MessageType_MemoryFileRequestToFriend, fileDataStr);
}

void SkpLocalMemoryFile::skp_request_file_to_friend_reply()
{
    MemoryFile::MemoryFileToFriend fileData;
    m_tlvRequest->skp_parse_message(&fileData);

    //printf("skp_request_file_to_friend_reply : name = %s, md5 = %s, length = %lld, friend = %lld \n", fileData.name().c_str(), fileData.md5().c_str(), fileData.lenth(), fileData.friend_());

    uint64 friendCode = fileData.friend_();

    fileData.clear_friend_();
    fileData.set_friend_(m_recv);
    const std::string &fileDataStr = fileData.SerializeAsString();

    skp_message_to_redis(friendCode, MessageType_MemoryFileReplyToFriend, fileDataStr);
}
