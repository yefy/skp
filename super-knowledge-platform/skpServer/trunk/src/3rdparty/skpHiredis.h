#ifndef SKPHIREDIS_H
#define SKPHIREDIS_H

#include "skpUtility.h"
#include "skpMutex.h"
#include "skpList.h"
#include "hiredis.h"
#include <async.h>

#define SKP_HIREDIS_COMMAND(object, format, ...) \
    if((object)->m_reply) { \
        freeReplyObject((object)->m_reply); \
    } \
    object->m_reply = (redisReply *)redisCommand(object->m_context, format, ##__VA_ARGS__)

class SkpEventRead;
class SkpEventWrite;
class SkpMallocPoolEx;
class SkpThread;


class SkpHiredisArgv
{
public:
    SkpHiredisArgv();
    ~SkpHiredisArgv();
    void reset();
    void relloc();
    char *argv(size_t size);
    void addKey(const std::string &str);
    void addKey(const char *data);
    void addKey(const char *data, int size);
    void addKey(const char *data_1, int size_1, const char *data_2, int size_2);
    int argc();
    const char **argv();
    size_t *argvLen();
    int m_argc;
    char **m_argv;
    size_t *m_argvlen;

    int m_argvSize;
    size_t *m_argvBufferlen;
    SkpMallocPoolEx *m_pool;
};

class SkpHiredis
{
public:
    SkpHiredis(SkpThread *thread);
    ~SkpHiredis();
    int connect(const char *ip, uint16 port, int timeout);
    void command(const char *format, ...);
    void command();
    redisContext *m_context;
    redisReply *m_reply;

    static void asyncConnectCallback(const redisAsyncContext *asyncContext, int status);
    static void asyncDisconnectCallback(const redisAsyncContext *asyncContext, int status);
    static void asyncCommandCallback(redisAsyncContext *asyncContext, void *r, void *privdata);
    static void onAddRead(void *privdata);
    static void onDelRead(void *privdata);
    static void onAddWrite(void *privdata);
    static void onDelWrite(void *privdata);
    static void onCleanup(void *privdata);

    void asyncCommandCallback_do(redisReply *reply);
    void onRead(bool isTimeOut);
    void onWrite(bool isTimeOut);


    int asyncConnect(const char *ip, uint16 port);
    int asyncCommand(const char *format, ...);

    redisAsyncContext *m_asyncContext;
    SkpEventRead *m_eventRead;
    SkpEventWrite *m_eventWrite;
    SkpHiredisArgv *m_argv;
    SkpListNode_t m_node;
    SkpThread *m_thread;
};

class SkpHiredisPool
{
public:
    SkpHiredisPool(SkpThread *thread);
    ~SkpHiredisPool();
     void connect(const char *ip, uint16 port, int timeout)
     {
         m_ip = ip;
         m_port = port;
         m_timeout = timeout;

     }

     SkpHiredis *getConnect()
     {
        SkpMutexLocker locker(&m_mutex);
        SKP_UNUSED(locker);

        if(m_freeList.size() > 0) {
            SkpHiredis *hiredis = (SkpHiredis *)m_freeList.take_pop();
            return hiredis;
        }

        SkpHiredis *hiredis = new SkpHiredis(m_thread);
        hiredis->connect(m_ip.c_str(), m_port, m_timeout);

        return hiredis;

     }

     void addConnect(SkpHiredis *hiredis)
     {
         SkpMutexLocker locker(&m_mutex);
         SKP_UNUSED(locker);
         m_freeList.push_back(hiredis, &hiredis->m_node);
     }



private:
     std::string m_ip;
     uint16 m_port;
     int m_timeout;
     int m_maxConnect;
     SkpThread *m_thread;

     SkpMutex m_mutex;
     SkpList m_freeList;
};


class SkpHiredisQuery
{
public:
    SkpHiredisQuery(SkpHiredisPool *hiredisPool)
    {
        m_hiredisPool = hiredisPool;
        m_hiredis = hiredisPool->getConnect();
    }

    ~SkpHiredisQuery()
    {
        m_hiredisPool->addConnect(m_hiredis);
    }

    SkpHiredisPool *m_hiredisPool;
    SkpHiredis *m_hiredis;
};


#endif // SKPHIREDIS_H
