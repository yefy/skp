#include "md5.h"
#include <openssl/md5.h>
#include <string.h>
#include <iostream>
#include <stdio.h>

MD5::MD5()
{
}

std::string MD5::string(const char *str)
{
    MD5_CTX ctx;
    const char *data = str;
    unsigned char md[16];
    char buf[33]={'\0'};
    char tmp[3]={'\0'};
    int i;
    MD5_Init(&ctx);
    MD5_Update(&ctx,data,strlen(data));
    MD5_Final(md,&ctx);
    for( i=0; i<16; i++ )
    {
        sprintf(tmp,"%02x",md[i]);
        strcat(buf,tmp);
    }

    return std::string(buf);
}

std::string MD5::string(const std::string &str)
{
    return string(str.c_str());
}

