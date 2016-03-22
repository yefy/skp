#include "rsa1.h"


//初学openssl库，写了一例子，记录一下。
//PS：openssl和openssl库的安装就不好了，网上一大把
//1：输入命令，生成公钥和私钥（1024位）

//openssl genrsa -out prikey.pem 1024
//openssl rsa -in privkey.pem -pubout -out pubkey.pem


//demo.cpp
// g++ demo.cpp -o demo -lcrypto
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/pem.h>

#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
using namespace std;

//加密
std::string EncodeRSAKeyFile( const std::string& strPemFileName, const std::string& strData )
{
  if (strPemFileName.empty() || strData.empty())
  {
    assert(false);
    return "";
  }
  FILE* hPubKeyFile = fopen(strPemFileName.c_str(), "rb");
  if( hPubKeyFile == NULL )
  {
    assert(false);
    return "";
  }
  std::string strRet;
  RSA* pRSAPublicKey = RSA_new();
  if(PEM_read_RSA_PUBKEY(hPubKeyFile, &pRSAPublicKey, 0, 0) == NULL)
  {
    assert(false);
    return "";
  }
  //if((p_rsa=PEM_read_RSAPublicKey(file,NULL,NULL,NULL))==NULL){  换成这句死活通不过，无论是否将公钥分离源文件


  int nLen = RSA_size(pRSAPublicKey);
  char* pEncode = new char[nLen + 1];
  int ret = RSA_public_encrypt(strData.length(), (const unsigned char*)strData.c_str(), (unsigned char*)pEncode, pRSAPublicKey, RSA_PKCS1_PADDING);
  if (ret >= 0)
  {
    strRet = std::string(pEncode, ret);
  }
  delete[] pEncode;
  RSA_free(pRSAPublicKey);
  fclose(hPubKeyFile);
  CRYPTO_cleanup_all_ex_data();
  return strRet;
}

//解密
std::string DecodeRSAKeyFile( const std::string& strPemFileName, const std::string& strData )
{
  if (strPemFileName.empty() || strData.empty())
  {
    assert(false);
    return "";
  }
  FILE* hPriKeyFile = fopen(strPemFileName.c_str(),"rb");
  if( hPriKeyFile == NULL )
  {
    assert(false);
    return "";
  }
  std::string strRet;
  RSA* pRSAPriKey = RSA_new();
  if(PEM_read_RSAPrivateKey(hPriKeyFile, &pRSAPriKey, 0, 0) == NULL)
  {
    assert(false);
    return "";
  }
  int nLen = RSA_size(pRSAPriKey);
  char* pDecode = new char[nLen+1];

  int ret = RSA_private_decrypt(strData.length(), (const unsigned char*)strData.c_str(), (unsigned char*)pDecode, pRSAPriKey, RSA_PKCS1_PADDING);
  if(ret >= 0)
  {
    strRet = std::string((char*)pDecode, ret);
  }
  delete [] pDecode;
  RSA_free(pRSAPriKey);
  fclose(hPriKeyFile);
  CRYPTO_cleanup_all_ex_data();
  return strRet;
}

int main1()
{
    /*
  //原文
  const string one = "skl;dfhas;lkdfhslk;dfhsidfhoiehrfoishfsidf";
  cout << "one: " << one << endl;

  //密文（二进制数据）
  string two = EncodeRSAKeyFile("pubkey.pem", one);
  cout << "two: " << two << endl;

  //顺利的话，解密后的文字和原文是一致的
  string three = DecodeRSAKeyFile("prikey.pem", two);
  cout << "three: " << three << endl;
  */

    //原文
    const string one = "skl;dfhas;lkdfhslk;dfhsidfhoiehrfoishfsidf";
    cout << "one: " << one << endl;

    //密文（二进制数据）
    string two = EncodeRSAKeyFile("anyfish_public_key.pem", one);
    cout << "two: " << two << endl;

    //顺利的话，解密后的文字和原文是一致的
    string three = DecodeRSAKeyFile("anyfish_private_key.pem", two);
    cout << "three: " << three << endl;


  return 0;
}

