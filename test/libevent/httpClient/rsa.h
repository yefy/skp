#ifndef _RSA_H
#define _RSA_H

#include <string>

void rsa_test();

class _RSA
{
public:
    _RSA();
    static std::string RSAPublicEncrypt(const std::string& strPemFileName, const std::string& strData );
    static std::string RSAPrivateDecrypt(const std::string& strPemFileName, const std::string& strData );

    static std::string RSAPublicDecrypt(const std::string& strPemFileName, const std::string& strData );
    static std::string RSAPrivateEncrypt(const std::string& strPemFileName, const std::string& strData );

};

#endif // _RSA_H
