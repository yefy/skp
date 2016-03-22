#include "rsa4.h"


#include <stdio.h>
#include <stdlib.h>

#include <openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h>
#include <openssl/bio.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#define IS_PASSWORD 0

static void generateKey() {

    /* 生成公钥 */
    RSA* rsa = RSA_generate_key( 1024, RSA_F4, NULL, NULL);
    BIO *bp = BIO_new( BIO_s_file() );
    char *publicpem = "public.pem";
    BIO_write_filename( bp, publicpem );
    PEM_write_bio_RSAPublicKey(bp, rsa);
    BIO_free_all( bp );
    /* 生成私钥 */
    bp = BIO_new_file("private.pem", "w+");
#if IS_PASSWORD
    char passwd[]="1234";
    PEM_write_bio_RSAPrivateKey(bp, rsa, NULL/*EVP_des_ede3()*/, (unsigned char*)passwd, 4, NULL, NULL);
#else
    PEM_write_bio_RSAPrivateKey(bp, rsa, NULL/*EVP_des_ede3()*/, NULL, NULL, NULL, NULL);
#endif
    BIO_free_all( bp );
    RSA_free(rsa);
}

std::string bio_read_privateKey(string data) {
    OpenSSL_add_all_algorithms();

    BIO* bp = BIO_new( BIO_s_file() );

    BIO_read_filename( bp, "private.pem" );
#if IS_PASSWORD
    char passwd[]="1234";
    RSA* rsaK = PEM_read_bio_RSAPrivateKey( bp, NULL, NULL, passwd );
#else
    RSA* rsaK = PEM_read_bio_RSAPrivateKey( bp, NULL, NULL, NULL );
#endif
    if (NULL == rsaK) {
        perror("read key file fail!");
    }else{
        printf("read success!\n");
    }

    int nLen = RSA_size(rsaK);
    //printf("len:%d\n",nLen);
    char *pEncode = new char[nLen + 1];
    int ret = RSA_private_decrypt(data.length(),(const unsigned char*)data.c_str(),(unsigned char *)pEncode,rsaK,RSA_PKCS1_PADDING);
    std::string strRet;
    if (ret >= 0) {
        strRet = std::string(pEncode, ret);
        //printf("%s",strRet.c_str());
    }

    delete[] pEncode;
    CRYPTO_cleanup_all_ex_data();
    BIO_free_all( bp );
    RSA_free(rsaK);
    return strRet;
}

std::string bio_read_publicKey(string data){
    OpenSSL_add_all_algorithms();
    BIO* bp = BIO_new( BIO_s_file() );
    BIO_read_filename( bp, "public.pem" );
    RSA* rsaK = PEM_read_bio_RSAPublicKey( bp, NULL, NULL, NULL );
    if (NULL == rsaK) {
        perror("read key file fail!");
    }else{
        printf("read success!");
        int nLen = RSA_size(rsaK);
        printf("len:%d\n",nLen);
    }
    int nLen = RSA_size(rsaK);
    char *pEncode = new char[nLen + 1];
    int ret = RSA_public_encrypt(data.length(),(const unsigned char*)data.c_str(),
        (unsigned char *)pEncode,rsaK,RSA_PKCS1_PADDING);
    std::string strRet;
    if (ret >= 0) {
        strRet = std::string(pEncode, ret);
        //printf("%s\n",strRet.c_str());
    }
    delete[] pEncode;
    CRYPTO_cleanup_all_ex_data();
    BIO_free_all( bp );
    RSA_free(rsaK);
    return strRet;
}

void encryptFile(string inputfile,string outputfile){
    ifstream file(inputfile.c_str());
    ofstream outfile(outputfile.c_str());
    string tsum;
    string ss;
    while (getline(file,ss)) {
        tsum.append(ss.append("\n"));
    }
    cout<<"加密内容："<<tsum<<endl;
    string mw = bio_read_publicKey(tsum);
    cout<<mw<<endl;
    outfile<<mw;
    outfile.flush();
    outfile.close();
    file.close();
}

void decryptFile(string inputfile,string outputfile){
    ifstream file(inputfile.c_str());
    ofstream outfile(outputfile.c_str());
    std::string tsum,ss;
    while (getline(file,ss)) {
        tsum.append(ss);
    }
    std::string cw = bio_read_privateKey(tsum);
    cout<<"恢复明文："<<cw;
    outfile<<cw;
    outfile.flush();
    outfile.close();
    file.close();
}

int main4() {
    //char *str = "第一步，首先需要在openssl官网下载openssl包http://www.openssl.org/source/；\n第二步，自己查资料去！";
    //system("openssl genrsa -out private.key 1024");
    generateKey();
    //bio_read_publicKey();
    char *str = "1234567890abcdefg";
    printf("原文：%s\n",str);
    std::string m = bio_read_publicKey(str);
    printf("密文：\n------------%s--------------\n\n",m.c_str());
    string miwen = m;
    std::string c = bio_read_privateKey(miwen);
    printf("解密后：\n------------%s--------------\n\n",c.c_str());

    printf("----------------加密文件--------------------------\n");
    encryptFile("before.txt","before1.txt");
    cout<<"------------------done-------------------------------"<<endl;
    /*
    ifstream infile("f:/my.txt");
    std::string instr,intemp;
    while (getline(infile,intemp)) {
        instr.append(intemp);
    }
    cout<<instr<<endl;
    std::string cwen = bio_read_privateKey(instr);
    cout<<cwen;
    */
    printf("-----------------解密文件----------------------------\n");
    decryptFile("before1.txt","before2.txt");
    cout<<"------------------done-------------------------------"<<endl;

    return 0;
}

