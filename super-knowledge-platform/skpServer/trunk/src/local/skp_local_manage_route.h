#ifndef SKP_LOCAL_MANAGE_ROUTE_H
#define SKP_LOCAL_MANAGE_ROUTE_H

#include "skpObject.h"

class SkpLocalSocketTcp;
class SkpEventChan;

class SkpLocalManageRoute : public SkpObject
{
public:

    static void init();
    static void destroy();
    static SkpLocalManageRoute *instance();

    static std::vector<SkpLocalManageRoute *> m_routeManages;
    static std::vector<SkpThread *> m_threads;
    ~SkpLocalManageRoute();

    std::vector<SkpLocalSocketTcp*> * findSocketList(uint64 serverID);
    bool addSocket(uint64 serverID, SkpLocalSocketTcp *socketTcp);
    bool delSocket(uint64 serverID,SkpLocalSocketTcp *socketTcp);
    bool sendSocket(uint64 serverID, void *data, int size);

    int m_index;

    uchar m_serverType;
    uint m_serverID;

    uint m_routeServerID ;
    std::string m_routeServerIP ;

    std::map<uint64, std::vector<SkpLocalSocketTcp*> *> socketMap;

    SkpEventChan *m_chan;

    bool routeChan(void *data, int dataSize);
    bool onRouteChan(void *head, int headSize, void *data, int dataSize);
private:
    SkpLocalManageRoute(int index, SkpThread &thread);
};

#endif // SKP_LOCAL_MANAGE_ROUTE_H
