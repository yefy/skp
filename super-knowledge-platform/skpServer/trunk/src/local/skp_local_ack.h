#ifndef SKP_LOCAL_ACK_H
#define SKP_LOCAL_ACK_H

#include "skp_local_protocal.h"

class SkpLocalAck : public SkpLocalProtocal
{
public:
    SkpLocalAck(int index, SkpObject *parent);
    ~SkpLocalAck();
    bool skp_dispatch();
    void skp_request_ack_list();
    void skp_server_ack();
    void skp_server_real_time();
};

#endif // SKP_LOCAL_ACK_H
