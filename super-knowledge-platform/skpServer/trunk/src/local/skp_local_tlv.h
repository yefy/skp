#ifndef SKP_LOCAL_TLV_H
#define SKP_LOCAL_TLV_H

#include "skpTlv.h"
#include "skp_public_transfer_protocal.h"

class SkpLocalSocketTcp;

namespace google {
    namespace protobuf {

    class MessageLite;

    }
}

class SkpLocalTLV : public SkpTLV
{
public:
    SkpLocalTLV();
    SkpLocalTLV(char *pBuffer, int64 nSize);
    SkpLocalTLV(char *pBuffer, int64 nSize, int tlvSize, int headSize);
    void skp_parse_buffer(char *data, int size);
    void skp_append_messge(int size, google::protobuf::MessageLite *lite);
    void skp_append_messge(const std::string &message);
    void skp_parse_message(google::protobuf::MessageLite *lite);
    void skp_set_status(uint16 status, uint16 error);
    uint16 skp_status();
    uint16 skp_err();
    void skp_set_protocal_head(SkpProtocalHead *headProtocal, uint8 orderType);
    void skp_set_send(uint64 code);
    void skp_set_recv(uint64 code);
    void skp_set_ins(uint64 ins);
    void skp_set_order_type(uint8 orderType);
    void skp_set_proxy(uint32 proxyID);
    void skp_set_proxy_socket_addr(uint64 proxyAddrSocket);
    void skp_set_client_mark(uint64 clientMark);
    void skp_set_param(uint32 param);
    SkpProtocalHead *skp_head();
    bool skp_post(SkpLocalSocketTcp *socketTcp = NULL);
    uint8 m_orderType;
    uint32 m_proxyID;
};

#endif // SKP_LOCAL_TLV_H
