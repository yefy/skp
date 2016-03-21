#include "skp_local_friend.h"
#include "friend.pb.h"
#include "skp_public_ins.h"
#include "skp_public_message_class.h"

SkpLocalFriend::SkpLocalFriend(int index, SkpObject *parent) :
    SkpLocalProtocal(index, parent)
{
}

SkpLocalFriend::~SkpLocalFriend()
{

}

bool SkpLocalFriend::skp_dispatch()
{
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)m_tlvRequest->Head();
    switch (headProtocal->orderType) {
    case OrderTypeRequest:
    {
        switch (headProtocal->orderRequst) {
        case OrderLocalLoginFriendList:
        {
            skp_request_get_friend_list();
            break;
        }
        default:
            SKP_ASSERT(skp_false);
            break;
        }

        break;
    }

    case OrderTypeServer:
    {
        switch (headProtocal->orderServer) {
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

void SkpLocalFriend::skp_request_get_friend_list()
{
    Friend::FriendList friendList;

/*
    const char *selectSql = "SELECT code, password, name FROM table_user";
    m_mysqlConn->skp_prepare(selectSql);
    int res = m_mysqlConn->skp_execute();
    SKP_ASSERT(res == AF::Mysql_Error_Success);
    while(m_mysqlConn->skp_db_next()) {
        while(m_mysqlConn->skp_next()) {
            uint64 friend_ = m_mysqlConn->skp_field_large_int();
            int passwordSize;
            char *password = m_mysqlConn->skp_field_buffer(passwordSize);
            int nameSize;
            char *name = m_mysqlConn->skp_field_buffer(nameSize);

            Friend::Friend *friendData = friendList.add_friend_();
            friendData->set_friend_(friend_);
            friendData->set_password(password, passwordSize);
            friendData->set_name(name, nameSize);

            printf("db userID = %lld, password = %s, name = %s \n", friend_, friendData->password().c_str(), friendData->name().c_str());
        }
    }
*/

    m_tlvResponse->skp_append_messge(friendList.ByteSize(), &friendList);
}
