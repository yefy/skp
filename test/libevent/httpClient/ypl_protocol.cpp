#include "ypl_protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"
#include "rsa.h"
#include "base64.h"
#include "httpclient_ypl.h"

yplProtocol::yplProtocol() :
    m_join(NULL),
    realSize(0),
    allSize(0)
{
}

char *yplProtocol::getJoin(int size)
{
    if(NULL == m_join)
    {
        allSize = 1024 + size;
        m_join = (char *)malloc(allSize);
    }
    else
    {
        if(allSize - realSize < size)
        {
            allSize = (allSize + size) * 2;
            m_join = (char *)realloc(m_join, allSize);
        }
    }

    char *temp = m_join + realSize;
    realSize += size;
    return temp;
}

void yplProtocol::add(std::string &key, std::string &value)
{
    m_map[key] = value;
}

void yplProtocol::add(const char *key, const char *value)
{
    m_map[std::string(key)] = std::string(value);
}

void yplProtocol::join()
{
    realSize = 0;
    char *flag1 = "=";
    char *flag2 = "&";
    bool isFirst = true;
    for(auto iter = m_map.begin(); iter != m_map.end(); iter++)
    {
        const std::string &key = iter->first;
        const std::string &value = iter->second;
        printf("key = %s, value = %s \n", key.c_str(), value.c_str());

        int size;
        if(isFirst)
        {
            size = key.size() + strlen(flag1) + value.size();

        }
        else
        {
            size = strlen(flag2) + key.size() + strlen(flag1) + value.size();
        }

        char *data = getJoin(size);

        if(isFirst)
        {
            isFirst = false;
            sprintf(data, "%s%s%s", key.c_str(), flag1, value.c_str());
        }
        else
        {
            sprintf(data, "%s%s%s%s", flag2, key.c_str(), flag1, value.c_str());
        }
    }

    m_join[realSize] = '\0';
    printf("join = [%s] \n", m_join);
}

void yplProtocol::sign()
{
    join();
    const std::string &md5 = MD5::string(m_join);
    const std::string &rsa = _RSA::RSAPrivateEncrypt("anyfish_private_key.pem", md5);
    int base64Size = 0;
    char *base64 = base64::encode(rsa.c_str(), rsa.size(), &base64Size);
    add("sign", base64);
    ::free(base64);

//    join();
//    const std::string &md5 = MD5::string(m_join);
//    const std::string &rsa = _RSA::RSAPrivateEncrypt("anyfish_private_key.pem", md5);
//    char *base64 = base64::encode(rsa.c_str(), rsa.size(), false);
//    char *uriEncode = HttpClient_YPL::uriEncode(base64, strlen(base64), 1);
//    printf("uriEncode = [%s] \n", uriEncode);
//    m_sign = std::string("sign=") + uriEncode;
//    ::free(base64);
//    printf("m_sign = [%s] \n", m_sign.c_str());
//    add("sign", uriEncode);

//    m_join = "api_name=epaylinks_umps_user_open_account&app_id=IRqmcUmK8TeIR6QSvo9NyWFi&app_user_id=15880271625&app_user_type=03&busi_water_no=1456901649259&cust_name=test&format=json&login_passwd=aK2cSLX6x5S7oKlKqnBgq5bZvwAZdT6zO9gWcBe8JePYUWCjlh6B5XWS6O2D7dm2YZuurV+EiIh1ICfxdgwTB2xjaKX1c8Kk1ocIPszLj8aJtZkknSwY9EmvPFWMThyrWhI9Ma9x59HV5wl5RCSB6OdWjSyzZ20ePWIrEQYjIdd7VoDEtuLdLmf2hmnKdzl42PsKAB2hCmhw2MlthsT90SaNgd1tUCZAkiySFfm2/nh+5UG4uNlF+jCQL1pRTmN5Nr2PCkSOQWOhD51527yLYt55tDikQh/pZpHH8Er5bh/sE8iIQoMsMgPCD4JnMnnyMujl34+bUxb5L+A0SAEYIw==&pay_passwd=gMiau21Xi0Kj4S7VwIQLzudA2j2Q2+ylGL0sa/zj0yZpJsVbisT1D76l8iI6sd5VFYOsyWOctWgaurE0/59M5h7IONe7jvYojO5F1dLwcu4fN1Z7RqzUFHYDNcBWBlF4guchxJliMF1KKnmmLi66g0v5kpUNC0JaGoijaIDFSt4wnokFIhMGCCifKxvYHaa/Ja2L6fc6KE6q9upJSGoYI01q48MDrE3uX1FEhfS19wBXbc9j0FjPS6MPsUXQC2BGdx8oDUgBUyCLwLcT5/VnMSri2NMz9Tu1+tA0CTG6IopETbAeo9eKnNbTTxiBeLUHCHFNM66ANihH7/cHQ/Ee0w==&terminal_no=43181666&timestamp=2016-03-02 14:54:09&ver=1.0";
//    const std::string &md5 = MD5::string(m_join);
//    const std::string &rsa = _RSA::RSAPrivateEncrypt("anyfish_private_key.pem", md5);
//    char *base64 = base64::encode(rsa.c_str(), rsa.size(), false);
//    printf("base64 = [%s] \n", base64);
//    char *uriEncode = HttpClient_YPL::uriEncode(base64, strlen(base64), 1);
//    printf("uriEncode = [%s] \n", uriEncode);
//    ::free(base64);
//    add("sign", uriEncode);
}

void yplProtocol::uriEncoding()
{
    for(auto iter = m_map.begin(); iter != m_map.end(); iter++)
    {
        const std::string &key = iter->first;
        const std::string &value = iter->second;
        char *uriEncode = HttpClient_YPL::uriEncode(value.c_str());
        m_map[key] = std::string(uriEncode);
    }
}

std::string &yplProtocol::url()
{
    sign();
    uriEncoding();
    join();
    m_url = std::string(m_join, realSize);
    printf("url = [%s] \n", m_url.c_str());
    return m_url;
}

std::string &yplProtocol::yplUrl()
{
    const std::string &url_ = url();

    m_url = std::string("/api/service?") + url_;
    printf("ypl url = [%s] \n", m_url.c_str());
    return m_url;
}
