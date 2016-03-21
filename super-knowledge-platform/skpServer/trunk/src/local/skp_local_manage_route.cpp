#include "skp_local_manage_route.h"
#include "skp_local_socket_tcp.h"
#include "skpEventChan.h"
#include "skpThread.h"
#include "skpAutoFree.h"

#define MANAGE_ROUTE_MAX 1
#define MANAGE_SOCKET_MAX 10

std::vector<SkpLocalManageRoute *> SkpLocalManageRoute::m_routeManages;
std::vector<SkpThread *> SkpLocalManageRoute::m_threads;

SkpLocalManageRoute::SkpLocalManageRoute(int index, SkpThread &thread) :
    SkpObject(thread)
{
    m_index = index;

    srand(time(NULL));

    m_serverType = ConnTypeLocal;
    m_serverID = 33331;
    m_routeServerID = 44441;
    m_routeServerIP = "127.0.0.1";
    //m_routeServerIP = "192.168.164.141";
    //m_routeServerIP = "192.168.164.147";



    m_chan = new SkpEventChan(this);
    m_chan->connect(std::bind(&SkpLocalManageRoute::onRouteChan, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

 }
SkpLocalManageRoute::~SkpLocalManageRoute()
{
    socketMap.clear();
}

void SkpLocalManageRoute::init()
{
    SKP_ASSERT(m_routeManages.empty());

    for(int i = 0; i < MANAGE_ROUTE_MAX; i++) {
        SkpThread *thread = new SkpThread();
        thread->start();
        m_threads.push_back(thread);
        SkpLocalManageRoute *localManage = new SkpLocalManageRoute(i, *thread);
        m_routeManages.push_back(localManage);
    }
}

void SkpLocalManageRoute::destroy()
{
    for(auto iter = m_threads.begin(); iter != m_threads.end(); iter++) {
        SkpThread *thread = *iter;
        thread->exit();
        while(!thread->isExit());
    }
}

SkpLocalManageRoute *SkpLocalManageRoute::instance()
{
    static int tid = 0;
    int index = tid % MANAGE_ROUTE_MAX;
    tid++;
    return m_routeManages.at(index);
}

std::vector<SkpLocalSocketTcp*> * SkpLocalManageRoute::findSocketList(uint64 serverID)
{
    std::vector<SkpLocalSocketTcp*> *socketList;

    auto iter =socketMap.find(serverID);
    if(iter == socketMap.end()) {
        socketList = new std::vector<SkpLocalSocketTcp*>();
        socketList->reserve(100);
        socketMap.insert(std::pair<uint64, std::vector<SkpLocalSocketTcp*> *>(serverID, socketList));
    } else {
        socketList = iter->second;
    }

    return socketList;
}

bool SkpLocalManageRoute::addSocket(uint64 serverID, SkpLocalSocketTcp *socketTcp)
{
    std::vector<SkpLocalSocketTcp*> *socketList = findSocketList(serverID);

    socketList->push_back(socketTcp);

    return skp_true;
}

bool SkpLocalManageRoute::delSocket(uint64 serverID,SkpLocalSocketTcp *socketTcp)
{
    std::vector<SkpLocalSocketTcp*> *socketList = findSocketList(serverID);

    auto iter = std::find(socketList->begin(), socketList->end(), socketTcp);
    if(iter != socketList->end())
        socketList->erase(iter);

    return skp_true;
}

bool SkpLocalManageRoute::sendSocket(uint64 serverID, void *data, int size)
{
    std::vector<SkpLocalSocketTcp*> *socketList = findSocketList(serverID);
    if(socketList->size() < MANAGE_SOCKET_MAX) {
        return skp_false;
    }

    int range = rand() % socketList->size();
    SkpLocalSocketTcp *socketTcp = socketList->at(range);
    socketTcp->appendWriteChan(data, size);
    return skp_true;
}

bool SkpLocalManageRoute::routeChan(void *data, int dataSize)
{
    return m_chan->push_back(skp_null, 0, data, dataSize);
}

bool SkpLocalManageRoute::onRouteChan(void *head, int headSize, void *data, int dataSize)
{
    SKP_UNUSED(head);
    SKP_UNUSED(headSize);
    uint64 serverID = m_routeServerID;
    printf("send to serverID = %lld \n", serverID);
    if(sendSocket(serverID, data, dataSize)) {
        return skp_true;
    }

    printf("connect to serverID = %lld \n", serverID);
    SkpLocalSocketTcp *localSocketTcp = new SkpLocalSocketTcp(this);
    localSocketTcp->addDel(std::bind(&SkpLocalManageRoute::delSocket, this, std::placeholders::_1, std::placeholders::_2));
    int res = localSocketTcp->connect(m_routeServerIP.c_str(), m_routeServerID);

    if(res != Skp::Connect_Error_Success) {
        printf("skp_send_route_conn connect error \n");
        skp_delete(localSocketTcp);
        return skp_false;
    }

    localSocketTcp->m_isConn = skp_true;
    localSocketTcp->m_isOwner = skp_true;

    localSocketTcp->start();
    uchar connTypeLocal = ConnTypeLocal;
    uchar connTypeRemote = ConnTypeRoute;
    SkpPakegeConnHead(&localSocketTcp->m_connHead, 1, connTypeLocal, 0, m_serverID, serverID);

    bool isRes = localSocketTcp->appendWriteChan(&localSocketTcp->m_connHead, localSocketTcp->m_connHead.headSize);
    if(!isRes) {
        printf("skp_send_route_conn push 222 error \n");
        skp_delete(localSocketTcp);
        return isRes;
    } else {
        //printf("skp_send_route_conn ok \n");
    }

    localSocketTcp->m_connHead.connType = connTypeRemote;

    isRes = localSocketTcp->appendWriteChan(data, dataSize);
    if(!isRes) {
        //printf("skp_send_route_conn push 333 error \n");
        skp_delete(localSocketTcp);
        return isRes;
    }

    addSocket(serverID, localSocketTcp);

    return skp_true;
}
