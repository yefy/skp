#ifndef SKP_LOCAL_DEMO_H
#define SKP_LOCAL_DEMO_H

#include "skp_local_protocal.h"

class SkpLocalDemo : public SkpLocalProtocal
{
public:
    SkpLocalDemo(int index, SkpObject *parent);
    ~SkpLocalDemo();
    bool skp_dispatch();
    void skp_request_demo();
    void skp_server_demo();
};

#endif // SKP_LOCAL_DEMO_H
