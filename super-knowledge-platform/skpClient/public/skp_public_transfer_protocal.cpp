#include "skp_public_transfer_protocal.h"
#include <QDebug>

void SkpPakegeConnHead(SkpProtocalConnHead *head, uint8 isKeep, uint8 connType, uint32 dataSize, uint64 fromServerID, uint64 toServerID) {
    head->headSize = sizeof(SkpProtocalConnHead);
    head->isConnHead = 1;
    head->isKeep = isKeep;
    head->connType = connType;
    head->dataSize = dataSize;
    head->fromServerID = fromServerID;
    head->toServerID = toServerID;
}

void SkpPakegeHead(SkpProtocalHead *head, uint32 dataSize, uint64 send, uint64 recv, uint8 orderType, uint32 proxyID, uint64 orderRequst, uint64 orderServer) {
    head->headSize = sizeof(SkpProtocalHead);
    head->isConnHead = 0;
    head->orderType = orderType;
    head->dataSize = dataSize;
    head->send = send;
    head->recv = recv;
    head->proxyID = proxyID;
    head->orderRequst = orderRequst;
    head->orderServer = orderServer;
}

bool SkpCheckConnHead(char *data, int allDataSize){
    int size = sizeof(SkpProtocalConnHead);
    if (allDataSize < size) {
        return false;
    }

    SkpProtocalConnHead *head = (SkpProtocalConnHead *)data;

    if (allDataSize < head->headSize) {
        return false;
    }

    int allSize = head->headSize + head->dataSize;
    if (allDataSize < allSize) {
        return false;
    }

    return true;
}

bool SkpCheckHead(char *data, int allDataSize) {

    int size = sizeof(SkpProtocalHead);
    if (allDataSize < size) {

        return false;
    }

    SkpProtocalHead *head = (SkpProtocalHead *)data;

    if (allDataSize < head->headSize) {

        return false;
    }

    int allSize = (head->headSize + head->dataSize);

    if (allDataSize < allSize) {

        return false;
    }

    return true;
}

