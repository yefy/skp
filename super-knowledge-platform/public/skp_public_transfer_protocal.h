#ifndef SKP_PUBLIC_TRANSFER_PROTOCAL_H
#define SKP_PUBLIC_TRANSFER_PROTOCAL_H

#include "skp_public_utility.h"

/*
namespace skp {

enum SkpInsClass
{
    InsClassPublic,
    InsClassServer
};

enum SkpInsType
{
    InsTypeIns,
    InsTypeAct
};

enum SkpChannelType
{
    ChannelPhone,
    ChannelPC,
    ChannelWeb
};

enum SkpChannelTypePhone
{
    ChannelTypePhoneAndroid,
    ChannelTypePhoneApple
};

enum SkpChannelTypePC
{
    ChannelTypePCWindow,
    ChannelTypePCLinux,
    ChannelTypePCMac,
};

enum SkpChannelTypeWeb
{
    ChannelTypeWeb
};

}


typedef struct SkpTransferIns_s SkpTransferIns_t;
struct SkpTransferIns_s
{
    uint ins :    	8;			// 指令编号
    uint insType: 		3;			// 指令类别
    uint insClass:      3;
    uint module:   8;
    uint isCryptoAsk:    1;
    uint isCryptoRet:    1;
    uint R:8;
};

typedef struct SkpTransferAsk_s SkpTransferAsk_t;
struct SkpTransferAsk_s
{
    uint16 sign;				// 包签名
    uint16 len;
    uint16 version;      	// 客户端版本
    uint16 modelversion;

    uint channel:         4;      // 通道
    uint channelType:		4;      // 通道类别
    uint param:         8;
    uint R:16;
    union{
        SkpTransferIns_t ins_t;
        uint ins;
    };
    uint insSerial;
    uint bodyLen;
    uint64 nSend;							// 发起用户编码
    uint64 nRecv;								// 接收目标编码
};
*/

typedef enum {
    ConnTypeClient = 0,
    ConnTypeProxy,
    ConnTypeLocal,
    ConnTypeRoute,
    ConTypenMonitor,
}ConnType;

typedef enum {
    OrderTypeRequest = 0,
    OrderTypeResponse,
    OrderTypeServer,
}OrderType;


typedef enum {
    Ins_Public = 0,
    Ins_Player,
}InType;

typedef struct SkpIns_s SkpIns_t;
struct SkpIns_s
{
    uint16 number;
    uint16 module;
    uint8 isCryptoRequest;			// 指令是加密的
    uint8 isCryptoResponse;			// 回复需要加密
    uint8 insType;
    uint8 R;
};

typedef struct {
    uint16 headSize;
    uint8 isKeep;
    uint8 isConnHead;
    uint32 dataSize;
    uint64 fromServerID ;
    uint64 toServerID ;
    uint8 connType;
    uint8 R;
    uint8 R2;
    uint8 R3;
    uint32 R4;
}SkpProtocalConnHead;

typedef struct {
    uint16 headSize;
    uint8 orderType;
    uint8 isConnHead;
    uint32 dataSize;
    uint64 send;
    uint64 recv;
    uint32 proxyID;
    uint16 status;
    uint16 error;
    uint64 proxyAddrSocket;
    uint64 clientMark;
    uint32 param;
    uint32 R;
    union {
        SkpIns_t orderRequst_t;
        uint64 orderRequst;
    };
    union {
        SkpIns_t orderServer_t;
        uint64 orderServer;
    };
}SkpProtocalHead;

void SkpPakegeConnHead(SkpProtocalConnHead *head, uint8 isKeep, uint8 connType, uint32 dataSize, uint64 fromServerID, uint64 toServerID);

void SkpPakegeHead(SkpProtocalHead *head, uint32 dataSize, uint64 send, uint64 recv, uint8 orderType, uint32 proxyID, uint64 orderRequst, uint64 orderServer);

bool SkpCheckConnHead(char *data, int allDataSize);

bool SkpCheckHead(char *data, int allDataSize);

#endif // SKP_PUBLIC_TRANSFER_PROTOCAL_H
