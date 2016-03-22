#include "rsa2.h"
/* filename: tmp.c
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h>

//openssl genrsa -out plainPrv.key 1024
//openssl rsa -in plainPrv.key -pubout -out plainPub.key
//openssl rsa -in plainPrv.key -RSAPublicKey_out -out plainPub2.key

//#define RSAPUBKEY

void hexprint(char *str,int len)
{
    int i=0;
    for(i=0;i<len;i++){
        printf("%s%02x%s",((i%16==0?"|":"")),*((unsigned char*)str+i),(((i+1)%16==0)?"|\n":" "));
    }
    if(i%16!=0)
        printf("|\n");
}

static int do_operation(RSA* rsa_ctx,char *instr,char* path_key,int inlen,char** outstr,int type)
{
    if(rsa_ctx == NULL || instr == NULL || path_key == NULL)
    {
        perror("input elems error,please check them!");
        return -1;
    }
    int rsa_len,num;
    rsa_len=RSA_size(rsa_ctx);
    *outstr=(char *)malloc(rsa_len+1);
    memset(*outstr,0,rsa_len+1);
    switch(type){
        case 1: //pub enc
        if(inlen == 0){
            perror("input str len is zero!");
            goto err;
        }
        num = RSA_public_encrypt(inlen,(unsigned char *)instr,(unsigned char*)*outstr,rsa_ctx,RSA_PKCS1_OAEP_PADDING);
        break;
        case 2: //prv dec
        num = RSA_private_decrypt(inlen,(unsigned char *)instr,(unsigned char*)*outstr,rsa_ctx,RSA_PKCS1_OAEP_PADDING);
        default:
        break;
    }

    if(num == -1)
    {
        printf("Got error on enc/dec!\n");
err:
        free(*outstr);
        *outstr = NULL;
        num = -1;
    }
    return num;
}

int rsa_pub_encrypt(char *str,char *path_key,char** outstr){
    RSA *p_rsa;
    FILE *file;
    int flen,rsa_len,num;
    if((file=fopen(path_key,"r"))==NULL){
        perror("open key file error");
        return -1;
    }
#ifdef RSAPUBKEY
    if((p_rsa=PEM_read_RSA_PUBKEY(file,NULL,NULL,NULL))==NULL){
#else
    if((p_rsa=PEM_read_RSAPublicKey(file,NULL,NULL,NULL))==NULL){
#endif
        ERR_print_errors_fp(stdout);
        return -1;
    }

    num = do_operation(p_rsa,str,path_key,strlen(str),outstr,1);
    RSA_free(p_rsa);
    fclose(file);
    return num;
}

int rsa_prv_decrypt(char *str,char *path_key,int inlen,char** outstr){
    RSA *p_rsa;
    FILE *file;
    int rsa_len,num;

    if((file=fopen(path_key,"r"))==NULL){
        perror("open key file error");
        return -1;
    }
    if((p_rsa=PEM_read_RSAPrivateKey(file,NULL,NULL,NULL))==NULL){
        ERR_print_errors_fp(stdout);
        return -1;
    }
    num = do_operation(p_rsa,str,path_key,inlen,outstr,2);
    RSA_free(p_rsa);
    fclose(file);
    return num;
}

int main2(){
    char *ptr_en,*ptr_de;
    int len;

    char *argv1 = "common";
#ifdef RSAPUBKEY
    char *argv2 = "plainPub.key";
#else
    char *argv2 = "plainPub2.key";
#endif
    char *argv3 = "plainPrv.key";

    printf("source is :%s\n",argv1);
    len=rsa_pub_encrypt(argv1,argv2,&ptr_en);
    printf("pubkey encrypt:\n");
    hexprint(ptr_en,len);

    rsa_prv_decrypt(ptr_en,argv3,len,&ptr_de);
    printf("prvkey decrypt:%s\n",ptr_de==NULL?"NULL":ptr_de);

    if(ptr_en!=NULL){
        free(ptr_en);
    }
    if(ptr_de!=NULL){
        free(ptr_de);
    }

    return 0;
}

