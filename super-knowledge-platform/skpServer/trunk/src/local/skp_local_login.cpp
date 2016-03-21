#include "skp_local_login.h"
#include "skp_local_status.h"
#include "login.pb.h"
#include "skp_local_key.h"
#include "skp_public_ins.h"
#include "skp_public_message_class.h"

SkpLocalLogin::SkpLocalLogin(int index, SkpObject *parent) :
    SkpLocalProtocal(index, parent)
{
}

SkpLocalLogin::~SkpLocalLogin()
{

}

bool SkpLocalLogin::skp_dispatch()
{
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)m_tlvRequest->Head();
    switch (headProtocal->orderType) {
    case OrderTypeRequest:
    {
        switch (headProtocal->orderRequst) {
        case OrderLocalLoginLogin:
        {
            skp_request_login();
            break;
        }
        default:
        {
            SKP_ASSERT(skp_false);
            break;
        }
        }
        break;
    }
    case OrderTypeServer:
    {
        switch (headProtocal->orderServer) {
        default:
        {
            SKP_ASSERT(skp_false);
            break;
        }
        }
        break;
    }
    default:
    {
        SKP_ASSERT(skp_false);
        break;
    }
    }

    return skp_true;
}

void SkpLocalLogin::skp_request_login()
{
    Login::Login login;
    m_tlvRequest->skp_parse_message(&login);
    //uint32 loginType = login.login_type();
    uint64 user = login.user();
    std::string password = login.password();
    std::string selectPassword = "";
/*
    const char *selectSql = "SELECT password FROM table_user WHERE code = ?";
    m_mysqlConn->skp_prepare(selectSql);
    m_mysqlConn->skp_param_large_int(user);
    int res = m_mysqlConn->skp_execute();
    SKP_ASSERT(res == AF::Mysql_Error_Success);
    while(m_mysqlConn->skp_db_next()) {
        while(m_mysqlConn->skp_next()) {
            int size;
            char *str = m_mysqlConn->skp_field_buffer(size);
            selectPassword = std::string(str, size);
            printf("db userID = %lld, password = %s \n", user, selectPassword.c_str());
        }
    }
*/
    selectPassword = password;
    if(password != selectPassword) {
        m_tlvResponse->skp_set_status(LocalStatusClassLogin, LocalStatusTypeLoginPassword);
    } else {

        printf("redis m_proxy = %lld, m_proxyAddrSocket = %lld \n", m_proxy, m_proxyAddrSocket);

        SkpLoginSession_t loginSession;
        loginSession.proxy = m_proxy;
        loginSession.proxyAddrSocket = m_proxyAddrSocket;

        SkpKey(loginKey, KeyClass_Login, KeyType_Login);

        SkpPassage(passageCommon, m_recv, m_recv, MessageClass, MessagePassage);
        skpSetRedisHash((char *)&passageCommon, SkpPassage_length, (char *)&loginKey, SkpKey_Length, (char *)&loginSession, sizeof(loginSession));
    }
}
