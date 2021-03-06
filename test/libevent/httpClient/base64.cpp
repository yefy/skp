﻿#include "base64.h"

#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <string.h>
#include <string>
#include <iostream>
using namespace std;

static char * Base64Encode(const char * input, int length, int *outSize, bool with_new_line)
{
    BIO * bmem = NULL;
    BIO * b64 = NULL;
    BUF_MEM * bptr = NULL;

    b64 = BIO_new(BIO_f_base64());
    if(!with_new_line) {
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    }
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, input, length);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    char * buff = (char *)malloc(bptr->length + 1);
    memcpy(buff, bptr->data, bptr->length);
    buff[bptr->length] = 0;

    *outSize = bptr->length;

    BIO_free_all(b64);

    return buff;
}

static char * Base64Decode(char * input, int length, int *outSize, bool with_new_line)
{
    BIO * b64 = NULL;
    BIO * bmem = NULL;
    char * buffer = (char *)malloc(length);
    memset(buffer, 0, length);

    b64 = BIO_new(BIO_f_base64());
    if(!with_new_line) {
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    }
    bmem = BIO_new_mem_buf(input, length);
    bmem = BIO_push(b64, bmem);
    int size = BIO_read(bmem, buffer, length);
    *outSize = size;

    BIO_free_all(bmem);

    return buffer;
}


void base64_test()
{
    cout << "With new line? y/n ";
    std::string option;
    cin >> option;
    bool with_new_line = ( ("y" == option || "Y" == option) ? true : false );

    string enc_input = "Henry Alfred Kissinger is a German-born American writer, political scientist, diplomat, and businessman. A recipient of the Nobel Peace Prize, he served as National Security Advisor and later concurrently as Secretary of State in the administrations of Presidents Richard Nixon and Gerald Ford.";

    cout << endl << "To be encoded:" << endl << "~" << enc_input << "~" << endl << endl;
    int size = 0;
    char * enc_output = Base64Encode(enc_input.c_str(), enc_input.length(), &size, with_new_line);
    cout << "Base64 Encoded:" << endl << "~" << enc_output << "~" << endl << endl;

    string dec_input = enc_output;
    char * dec_output = Base64Decode((char *)dec_input.c_str(), dec_input.length(), &size, with_new_line);
    cout << "Base64 Decoded:" << endl << "~" << dec_output << "~" << endl << endl;

    free(enc_output);
    free(dec_output);
}


base64::base64()
{
}

char *base64::encode(const char * input, int length, int *outSize, bool with_new_line)
{
    return ::Base64Encode(input, length, outSize, with_new_line);
}

char *base64::decode(char * input, int length, int *outSize, bool with_new_line)
{
    return ::Base64Decode(input, length, outSize, with_new_line);
}
