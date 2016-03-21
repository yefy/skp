#ifndef SKP_LOCAL_MEMORY_FILE_H
#define SKP_LOCAL_MEMORY_FILE_H

#include "skp_local_protocal.h"

class skpThread;
class SkpMallocPoolEx;

namespace Message {
    class MessageDataList;
}

typedef enum {
    FileDataStatusNull = 0,
    FileDataStatusPushing,
    FileDataStatusFinish
}FileDataStatus;

typedef struct SkpFileData_s SkpFileData_t;
struct SkpFileData_s {
    char md5[32];
    char name[256];
    uint32 nameSize;
    uint32 R4;
    uint64 lenth;
    char *offset;
    char status;
    char R;
    char R2;
    char R3;
    char buffer[0];
};

class SkpLocalMemoryFile : public SkpLocalProtocal
{
public:
    SkpLocalMemoryFile(int index, SkpObject *parent);
    ~SkpLocalMemoryFile();
    bool skp_dispatch();
    void skp_request_is_exist();
    void skp_request_push_request();
    void skp_request_push();
    void skp_request_pull();
    void skp_request_push_ok();
    void skp_request_file_list();
    void skp_request_delete();
    void skp_request_file_to_friend();
    void skp_request_file_to_friend_reply();

    SkpFileData_t *skp_is_exist_md5(const std::string &md5);
    void skp_message_to_redis(uint64 friendCode, int type, const std::string &fileDataStr);

    SkpMallocPoolEx *m_poolEx;
    std::map<std::string, SkpFileData_t *> m_protocaltMap;

    uint32 m_lastTime;
    uint64 m_ratebytes;

};

#endif // SKP_LOCAL_MEMORY_FILE_H
