#ifndef SKP_PUBLIC_INS_H
#define SKP_PUBLIC_INS_H

#include "skp_public_utility.h"

typedef enum {
    ModuleTypeMonitor = 0,
    ModuleTypeProxy,
    ModuleTypeLocalMemoryFile,
    ModuleTypeLocalTest,
    ModuleTypeLocalLogin,
    ModuleTypeLocalFriend,
    ModuleTypeLocalAck,
    ModuleTypeLocalChat,
    ModuleTypeLocalMessage,
}ModuleType;


const uint64 ModuleMonitor                  = ModuleTypeMonitor << 16;
const uint64 ModuleProxy                    = ModuleTypeProxy << 16;
const uint64 ModuleLocalMemoryFile          = ModuleTypeLocalMemoryFile << 16;
const uint64 ModuleLocalTest                = ModuleTypeLocalTest << 16;
const uint64 ModuleLocalLogin               = ModuleTypeLocalLogin << 16;
const uint64 ModuleLocalFriend              = ModuleTypeLocalFriend << 16;
const uint64 ModuleLocalAck                 = ModuleTypeLocalAck << 16;
const uint64 ModuleLocalChat                = ModuleTypeLocalChat << 16;
const uint64 ModuleLocalMessage             = ModuleTypeLocalMessage << 16;




const uint64  OrderMonitorLoadConfig        = ModuleMonitor + 0x01;

const uint64  OrderLocalTestNetwork         = ModuleLocalTest + 0x01;

const uint64  OrderLocalMemoryFileIsExist   = ModuleLocalMemoryFile + 0x01;
const uint64  OrderLocalMemoryFilePushRuest = ModuleLocalMemoryFile + 0x02;
const uint64  OrderLocalMemoryFilePush      = ModuleLocalMemoryFile + 0x03;
const uint64  OrderLocalMemoryFilePull      = ModuleLocalMemoryFile + 0x04;
const uint64  OrderLocalMemoryFilePushOK    = ModuleLocalMemoryFile + 0x05;
const uint64  OrderLocalMemoryFileFileList  = ModuleLocalMemoryFile + 0x06;
const uint64  OrderLocalMemoryFileDelete    = ModuleLocalMemoryFile + 0x07;
const uint64  OrderLocalMemoryFileToFriend  = ModuleLocalMemoryFile + 0x08;
const uint64  OrderLocalMemoryFileToFriendReply  = ModuleLocalMemoryFile + 0x09;
const uint64  OrderLocalMemoryFileToFriendData  = ModuleLocalMemoryFile + 0x0A;

const uint64  OrderLocalLoginLogin          = ModuleLocalLogin + 0x01;

const uint64  OrderLocalLoginFriendList     = ModuleLocalFriend + 0x01;

const uint64  OrderLocalAck                 = ModuleLocalAck + 0x01;
const uint64  OrderLocalAckList             = ModuleLocalAck + 0x02;
const uint64  OrderLocalRealTime            = ModuleLocalAck + 0x03;

const uint64  OrderLocalChat                = ModuleLocalChat + 0x01;

const uint64  OrderLocalMessageGetMessage    = ModuleLocalMessage + 0x01;


#endif // SKP_PUBLIC_INS_H
