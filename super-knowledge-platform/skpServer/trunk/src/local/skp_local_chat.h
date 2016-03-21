#ifndef SKP_LOCAL_CHAT_H
#define SKP_LOCAL_CHAT_H

#include "skp_local_protocal.h"

class SkpLocalChat : public SkpLocalProtocal
{
public:
    SkpLocalChat(int index, SkpObject *parent);
    ~SkpLocalChat();
    bool skp_dispatch();
    void skp_request_chat();
    void skp_request_get_message();



    void skp_server_chat();

    void skp_insert_chat(uint64 code, uint64 friendCode, uint64 messageCode, int class_, int type, std::string &data);
    void skp_insert_chat_virtual(uint64 code, uint64 friendCode, uint64 virtualTime, int class_, int type, uint64 messageTime, uint64 messageSender);

};

#endif // SKP_LOCAL_CHAT_H
