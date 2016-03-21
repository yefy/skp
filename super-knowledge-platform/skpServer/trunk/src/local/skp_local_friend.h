#ifndef SKP_LOCAL_FRIEND_H
#define SKP_LOCAL_FRIEND_H

#include "skp_local_protocal.h"

class SkpLocalFriend : public SkpLocalProtocal
{
public:
    SkpLocalFriend(int index, SkpObject *parent);
    ~SkpLocalFriend();
    bool skp_dispatch();
    void skp_request_get_friend_list();
};

#endif // SKP_LOCAL_FRIEND_H
