#ifndef SKP_LOCAL_TLV_H
#define SKP_LOCAL_TLV_H

#include "skp_core_tlv.h"
#include "skp_public_transfer_protocal.h"

#include <QByteArray>

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
    void skp_parse_buffer(const char *data, int size);
    void skp_parse_buffer(const QByteArray &bytes);
    void skp_append_messge(int size, google::protobuf::MessageLite *lite);
    void skp_Parse_message(google::protobuf::MessageLite *lite);
    void skp_set_status(uint16 status, uint16 error);
    uint16 skp_status();
    uint16 skp_err();
    void skp_set_protocal_head(SkpProtocalHead *headProtocal, uint8 orderType);
    void skp_set_send(uint64 code);
    uint64 skp_send();
    void skp_set_recv(uint64 code);
    uint64 skp_recv();
    void skp_set_ins(uint64 ins);
    void skp_set_order_type(uint8 orderType);
    void skp_set_proxy(uint32 proxyID);
    void skp_set_client_mark(uint64 clientMark);
    void skp_set_param(uint32 param);
    void skp_start();
    bool skp_post();
    uint8 m_orderType;
    uint32 m_proxyID;
};

#endif // SKP_LOCAL_TLV_H
