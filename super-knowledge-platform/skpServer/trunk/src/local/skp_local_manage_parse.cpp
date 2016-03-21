#include "skp_local_manage_parse.h"
#include "skp_local_socket_tcp.h"
#include "skpEventChan.h"
#include "skp_public_transfer_protocal.h"
#include "skp_public_utility.h"
#include "skp_public_ins.h"
#include "skp_local_memory_file.h"
#include "skp_local_login.h"
#include "skp_local_ack.h"
#include "skp_local_chat.h"
#include "skp_local_friend.h"
#include "skpAutoFree.h"
#include "skpThread.h"

#define MANAGE_PARSE_MAX 1

std::vector<SkpLocalManageParse *> SkpLocalManageParse::m_manages;
std::vector<SkpThread *> SkpLocalManageParse::m_threads;

SkpLocalManageParse::SkpLocalManageParse(int index, SkpThread &thread) :
    SkpObject(thread)
{
    m_index = index;

    m_chan = new SkpEventChan(this);
    m_chan->connect(std::bind(&SkpLocalManageParse::onParseChan, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,std::placeholders::_4));

    m_protocaltMap.insert(std::pair<uint64, SkpLocalProtocal*>(ModuleTypeLocalMemoryFile, new SkpLocalMemoryFile(index, this)));
    m_protocaltMap.insert(std::pair<uint64, SkpLocalProtocal*>(ModuleTypeLocalLogin, new SkpLocalLogin(index, this)));
    m_protocaltMap.insert(std::pair<uint64, SkpLocalProtocal*>(ModuleTypeLocalFriend, new SkpLocalFriend(index, this)));
    m_protocaltMap.insert(std::pair<uint64, SkpLocalProtocal*>(ModuleTypeLocalAck, new SkpLocalAck(index, this)));
    m_protocaltMap.insert(std::pair<uint64, SkpLocalProtocal*>(ModuleTypeLocalChat, new SkpLocalChat(index, this)));
    m_protocaltMap.insert(std::pair<uint64, SkpLocalProtocal*>(ModuleTypeLocalMessage, new SkpLocalChat(index, this)));
}
SkpLocalManageParse::~SkpLocalManageParse()
{
    m_protocaltMap.clear();
}

void SkpLocalManageParse::init()
{
    SKP_ASSERT(m_manages.empty());

    for(int i = 0; i < MANAGE_PARSE_MAX; i++) {
        SkpThread *thread = new SkpThread();
        thread->start();
        m_threads.push_back(thread);
        SkpLocalManageParse *localManage = new SkpLocalManageParse(i, *thread);
        m_manages.push_back(localManage);
    }
}

void SkpLocalManageParse::destroy()
{
    for(auto iter = m_threads.begin(); iter != m_threads.end(); iter++) {
        SkpThread *thread = *iter;
        thread->exit();
        while(!thread->isExit());
    }
}

SkpLocalManageParse *SkpLocalManageParse::instance(uint64 userID)
{
    uint64 index = userID % MANAGE_PARSE_MAX;
    return m_manages.at(index);
}


bool SkpLocalManageParse::onParseChan(void *head, int headSize, void *data, int dataSize)
{
    SkpProtocalHead *headProtocal = (SkpProtocalHead *)data;
    auto iter = m_protocaltMap.find(headProtocal->orderRequst_t.module);
    if(iter == m_protocaltMap.end()) {
        SKP_ASSERT(skp_false);
        return skp_true;
    }

    SkpLocalProtocal *protocal = (SkpLocalProtocal *)iter->second;
    return protocal->skp_parse_protocal(head, headSize, data, dataSize);
}

bool SkpLocalManageParse::parseChan(void *head, int headSize, void *data, int dataSize)
{
    return m_chan->push_back(head, headSize, data, dataSize);
}
