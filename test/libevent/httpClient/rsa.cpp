#include "rsa.h"

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

//Public加密
static std::string RSAPublicEncrypt(const std::string& strPemFileName, const std::string& strData )
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


//  if((pRSAPublicKey=PEM_read_RSAPublicKey(hPubKeyFile,NULL,NULL,NULL))==NULL){
//      //换成这句死活通不过，无论是否将公钥分离源文件
//      assert(false);
//      return "";
//  }


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

//Privat解密
static std::string RSAPrivateDecrypt( const std::string& strPemFileName, const std::string& strData )
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



//public解密
static std::string RSAPublicDecrypt(const std::string& strPemFileName, const std::string& strData )
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
  int ret = RSA_public_decrypt(strData.length(), (const unsigned char*)strData.c_str(), (unsigned char*)pEncode, pRSAPublicKey, RSA_PKCS1_PADDING);
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

//Private加密
static std::string RSAPrivateEncrypt( const std::string& strPemFileName, const std::string& strData )
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
  //PKCS1Padding ：填充方式
  //ECB：工作模式

  int ret = RSA_private_encrypt(strData.length(), (const unsigned char*)strData.c_str(), (unsigned char*)pDecode, pRSAPriKey, RSA_PKCS1_PADDING);
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


void rsa_test()
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
    const string one = "123456789";
    cout << "one: " << one << endl;


    //密文（二进制数据）
    string two2222 = ::RSAPublicEncrypt("publicKey_13.keystore", one);
    cout << "two2222: " << two2222 << endl;


    //密文（二进制数据）
    string two = ::RSAPublicEncrypt("anyfish_public_key.pem", one);
    cout << "two: " << two << endl;

    //顺利的话，解密后的文字和原文是一致的
    string three = ::RSAPrivateDecrypt("anyfish_private_key.pem", two);
    cout << "three: " << three << endl;

    cout << "***************************** " << endl;

    //原文
    const string one1 = "123456789";
    cout << "one1: " << one1 << endl;

    //密文（二进制数据）
    string two1 = ::RSAPrivateEncrypt("anyfish_private_key.pem", one1);
    cout << "two1: " << two1 << endl;

    //顺利的话，解密后的文字和原文是一致的
    string three1 = ::RSAPublicDecrypt("anyfish_public_key.pem", two1);
    cout << "three1: " << three1 << endl;
}



_RSA::_RSA()
{
}

std::string _RSA::RSAPublicEncrypt(const std::string& strPemFileName, const std::string& strData )
{
    return ::RSAPublicEncrypt(strPemFileName, strData);
}

std::string _RSA::RSAPrivateDecrypt(const std::string& strPemFileName, const std::string& strData )
{
    return ::RSAPrivateDecrypt(strPemFileName, strData);
}

std::string _RSA::RSAPublicDecrypt(const std::string& strPemFileName, const std::string& strData )
{
    return ::RSAPublicDecrypt(strPemFileName, strData);
}

std::string _RSA::RSAPrivateEncrypt(const std::string& strPemFileName, const std::string& strData )
{
    return ::RSAPrivateEncrypt(strPemFileName, strData);
}

