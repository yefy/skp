#include "skp_local_demo.h"

///m_protocaltMap.insert(std::pair<uint64, SkpLocalProtocal*>(ModuleTypeLocalChat, new SkpLocalChat(index, thread)));


SkpLocalDemo::SkpLocalDemo(int index, SkpObject *parent) :
    SkpLocalProtocal(index, parent)
{
}

SkpLocalDemo::~SkpLocalDemo()
{

}

bool SkpLocalDemo::skp_dispatch()
{
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)m_tlvRequest->Head();
    switch (headProtocal->orderType) {
    case OrderTypeRequest:
    {
        switch (headProtocal->orderRequst) {
        case 0 /*OrderLocalAck*/:
            skp_request_demo();
            break;
        default:
            SKP_ASSERT(skp_false);
            break;
        }

        break;
    }

    case OrderTypeServer:
    {
        switch (headProtocal->orderServer) {
        case 0/*OrderLocalAck*/:
            skp_server_demo();
            break;
        default:
            SKP_ASSERT(skp_false);
            break;
        }

        break;
    }

    default:
        SKP_ASSERT(skp_false);
        break;
    }

    return skp_true;
}

void SkpLocalDemo::skp_request_demo()
{

}

void SkpLocalDemo::skp_server_demo()
{

}
