#ifndef SKP_LOCAL_KEY_H
#define SKP_LOCAL_KEY_H

#include "skpUtility.h"

#define SkpKey_Length sizeof(SkpLocalKey_t)

#define SkpKey(keyName_, keyClass_, keyType_) \
    SkpLocalKey_t keyName_; \
    keyName_.keyClass = keyClass_; \
    keyName_.keyType = keyType_;

typedef struct SkpLocalKey_s SkpLocalKey_t;
struct SkpLocalKey_s
{
    uint keyClass;
    uint keyType;
};

#define SkpKey_MessageLength sizeof(SkpLocalKeyMessage_t)

#define SkpKey_Message(keyName_, keyClass_, keyType_, keySender_, keyRecv_) \
    SkpLocalKeyMessage_t keyName_; \
    keyName_.keyClass = keyClass_; \
    keyName_.keyType = keyType_; \
    keyName_.keySender = keySender_; \
    keyName_.keyRecv = keyRecv_;

typedef struct SkpLocalKeyMessage_s SkpLocalKeyMessage_t;
struct SkpLocalKeyMessage_s
{
    uint keyClass;
    uint keyType;
    uint64 keySender;
    uint64 keyRecv;
};

#define SkpKey_MessageExLength sizeof(SkpLocalKeyMessageEx_t)

#define SkpKey_MessageEx(keyName_, keyClass_, keyType_, keySender_, keyRecv_, messageClass_, messagePassage_) \
    SkpLocalKeyMessageEx_t keyName_; \
    keyName_.keyClass = keyClass_; \
    keyName_.keyType = keyType_; \
    keyName_.keySender = keySender_; \
    keyName_.keyRecv = keyRecv_; \
    keyName_.messageClass = messageClass_; \
    keyName_.messagePassage = messagePassage_;

typedef struct SkpLocalKeyMessageEx_s SkpLocalKeyMessageEx_t;
struct SkpLocalKeyMessageEx_s
{
    uint keyClass;
    uint keyType;
    uint64 keySender;
    uint64 keyRecv;
    uint messageClass;
    uint messagePassage;
};


typedef enum {
    KeyClass = 0,
    KeyClass_Login,
    KeyClass_Ack,
    KeyClass_Chat,
}KeyClass_;

typedef enum {
    KeyType_Login = 0,
}KeyType_Login_;

typedef enum {
    KeyType_Ack = 0,
    KeyType_AckIndex,
}KeyType_Ack_;

typedef enum {
    KeyType_Chat = 0,
    KeyType_ChatIndex,
}KeyType_Chat_;


typedef struct SkpLoginSession_s SkpLoginSession_t;
struct SkpLoginSession_s {
    uint64 proxy;
    uint64 proxyAddrSocket;
};


#endif // SKP_LOCAL_KEY_H
