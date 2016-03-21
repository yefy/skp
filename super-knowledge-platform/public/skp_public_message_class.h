#ifndef SKP_PUBLIC_MESSAGE_CLASS_H
#define SKP_PUBLIC_MESSAGE_CLASS_H

#include "skp_public_utility.h"

#define SkpPassage_length sizeof(SkpLocalPassage_t)

#define SkpPassage(passageName_, passageUserCode_, passageFriendCode_, passageClass_,passage_) \
    SkpLocalPassage_t passageName_; \
    passageName_.userCode = passageUserCode_; \
    passageName_.friendCode = passageFriendCode_; \
    passageName_.passageClass = passageClass_; \
    passageName_.passage = passage_;

typedef struct SkpLocalPassage_s SkpLocalPassage_t;
struct SkpLocalPassage_s
{
    uint64 userCode;
    uint64 friendCode;
    uint passageClass;
    uint passage;
};


//------------------ class
typedef enum {
    MessageClass = 0,
    MessageClass_MemoryFile,
    MessageClass_Chat,
    MessageClass_Login,
    MessageClass_Friend,
    MessageClass_Ack,
}MessageClass_;

//-------------------------------- type
typedef enum {
    MessageType = 0,
}MessageType_;

typedef enum {
    MessagePassage = 0,
}MessagePassage_;

//---------------------memory_file
typedef enum {
    MessageType_MemoryFile = 0,
    MessageType_MemoryFileRequestToFriend,
    MessageType_MemoryFileReplyToFriend,
    MessageType_MemoryFilePushRequest,
    MessageType_MemoryFilePush,
}MessageType_MemoryFile_;

typedef enum {
    MessagePassage_MemoryFile = 0,
}MessagePassage_MemoryFile_;


//--------------------------chat
typedef enum {
    MessageType_Chat = 0,
}MessageType_Chat_;

typedef enum {
    MessagePassage_Chat = 0,
}MessagePassage_Chat_;


//-----------------------------ack
typedef enum {
    MessageType_Ack = 0,
}MessageType_Ack_;

typedef enum {
    MessagePassage_Ack = 0,
}MessagePassage_Ack_;


#endif // SKP_PUBLIC_MESSAGE_CLASS_H
