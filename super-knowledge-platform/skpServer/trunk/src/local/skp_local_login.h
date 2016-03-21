#ifndef SKP_LOCAL_LOGIN_H
#define SKP_LOCAL_LOGIN_H

#include "skp_local_protocal.h"

class SkpLocalLogin : public SkpLocalProtocal
{
public:
    SkpLocalLogin(int index, SkpObject *parent);
    ~SkpLocalLogin();
    bool skp_dispatch();
    void skp_request_login();
};

#endif // SKP_LOCAL_LOGIN_H
