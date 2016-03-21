#include "skpHiredis.h"
#include "skpEventRead.h"
#include "skpEventWrite.h"
#include "skpMallocPoolEx.h"
#include "skpThread.h"
#include "skpLog.h"

SkpHiredisArgv::SkpHiredisArgv()
{
    m_pool = new SkpMallocPoolEx();
    m_argc = 0;
    m_argvSize = 1024;
    m_argv = (char **)skp_pool_calloc(m_pool, m_argvSize * sizeof(char **));
    m_argvlen = (size_t *)skp_pool_calloc(m_pool, m_argvSize * sizeof(size_t));
    m_argvBufferlen = (size_t *)skp_pool_calloc(m_pool, m_argvSize * sizeof(size_t));
}

SkpHiredisArgv::~SkpHiredisArgv()
{
    if(m_pool) {
        delete m_pool;
        m_pool = NULL;
    }
}

void SkpHiredisArgv::reset()
{
    m_argc = 0;
}

void SkpHiredisArgv::relloc()
{
    int lastArgvSize = m_argvSize;
    m_argvSize *= m_argvSize;
    char **argv = (char **)skp_pool_calloc(m_pool, m_argvSize * sizeof(char **));
    size_t *argvlen = (size_t *)skp_pool_calloc(m_pool, m_argvSize * sizeof(size_t));
    size_t *argvBufferlen = (size_t *)skp_pool_calloc(m_pool, m_argvSize * sizeof(size_t));

    memcpy(argv, m_argv, lastArgvSize * sizeof(char **));
    memcpy(argvlen, m_argvlen, lastArgvSize * sizeof(size_t));
    memcpy(argvBufferlen, m_argvBufferlen, lastArgvSize * sizeof(size_t));

    skp_pool_free(m_pool, m_argv);
    skp_pool_free(m_pool, m_argvlen);
    skp_pool_free(m_pool, m_argvBufferlen);

    m_argv = argv;
    m_argvlen = argvlen;
    m_argvBufferlen = argvBufferlen;

}

char *SkpHiredisArgv::argv(size_t size)
{
    if(m_argc >= m_argvSize) {
        relloc();
    }

    if(m_argvBufferlen[m_argc] < size) {
        skp_pool_free(m_pool, m_argv[m_argc]);
        m_argv[m_argc] = (char *)skp_pool_calloc(m_pool, size);
        m_argvBufferlen[m_argc] = size;
    }

    m_argvlen[m_argc] = size;

    int argc = m_argc;
    m_argc++;
    return m_argv[argc];
}

void SkpHiredisArgv::addKey(const std::string &str)
{
    addKey(str.c_str(), str.size());

}

void SkpHiredisArgv::addKey(const char *data)
{
    int size = strlen(data);
    char *_argv = argv(size);
    memcpy(_argv, data, size);
}

void SkpHiredisArgv::addKey(const char *data, int size)
{
    char *_argv = argv(size);
    memcpy(_argv, data, size);
}

void SkpHiredisArgv::addKey(const char *data_1, int size_1, const char *data_2, int size_2)
{
    char *_argv = argv(size_1 + size_2);
    memcpy(_argv, data_1, size_1);
    memcpy(_argv + size_2, data_2, size_2);
}

int SkpHiredisArgv::argc()
{
    return m_argc;
}

const char **SkpHiredisArgv::argv()
{
    return (const char **)m_argv;
}

size_t *SkpHiredisArgv::argvLen()
{
    return m_argvlen;
}


//-----------------------------------------------------

SkpHiredis::SkpHiredis(SkpThread *thread) :
    m_context(NULL),
    m_reply(NULL),
    m_asyncContext(NULL),
    m_thread(thread)
{
    m_argv = new SkpHiredisArgv();
}

SkpHiredis::~SkpHiredis()
{
    if(m_context) {
        redisFree(m_context);
        m_context = NULL;
    }

    if(m_eventRead) {
        delete m_eventRead;
        m_eventRead = NULL;
    }

    if(m_eventWrite) {
        delete m_eventWrite;
        m_eventWrite = NULL;
    }

    if(m_asyncContext) {
        redisAsyncDisconnect(m_asyncContext);
        m_asyncContext = NULL;
    }

    if(m_argv) {
        delete m_argv;
        m_argv = NULL;
    }

}

int SkpHiredis::connect(const char *ip, uint16 port, int timeout)
{
    struct timeval tv;
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    m_context = redisConnectWithTimeout(ip, port, tv);

    if (m_context == NULL || m_context->err) {
        if (m_context) {
            skpLogError_g("Connection error: %s\n", m_context->errstr);
            redisFree(m_context);
        } else {
            skpLogDebug_g("Connection error: can't allocate redis context\n");
        }
        return skp_false;
    }

    return skp_true;
}

void SkpHiredis::command(const char *format, ...)
{
    SKP_ASSERT(m_context);

    if(m_reply) {
        freeReplyObject(m_reply);
    }

    va_list ap;
    va_start(ap, format);
    m_reply = (redisReply *)redisvCommand(m_context, format, ap);
    va_end(ap);
}

void SkpHiredis::command()
{
    SKP_ASSERT(m_context);

    if(m_reply) {
        freeReplyObject(m_reply);
    }

    m_reply = (redisReply *)redisCommandArgv(m_context, m_argv->argc(), m_argv->argv(), m_argv->argvLen());
}

void SkpHiredis::asyncConnectCallback(const redisAsyncContext *asyncContext, int status)
{
    if (status != REDIS_OK) {
        printf("Error: %s\n", asyncContext->errstr);
        return;
    }
}

void SkpHiredis::asyncDisconnectCallback(const redisAsyncContext *asyncContext, int status)
{
    if (status != REDIS_OK) {
        printf("Error: %s\n", asyncContext->errstr);
        return;
    }
}

void SkpHiredis::onAddRead(void *privdata) {
    SkpHiredis *hiredis = (SkpHiredis *)privdata;
    hiredis->m_eventRead->start(0);
}

void SkpHiredis::onDelRead(void *privdata) {
    SkpHiredis *hiredis = (SkpHiredis *)privdata;
    hiredis->m_eventRead->stop();
}

void SkpHiredis::onAddWrite(void *privdata) {
    SkpHiredis *hiredis = (SkpHiredis *)privdata;
    hiredis->m_eventWrite->start(0);
}

void SkpHiredis::onDelWrite(void *privdata) {
    SkpHiredis *hiredis = (SkpHiredis *)privdata;
    hiredis->m_eventWrite->stop();
}

void SkpHiredis::onCleanup(void *privdata) {
    SkpHiredis *hiredis = (SkpHiredis *)privdata;
    hiredis->m_eventRead->stop();
    hiredis->m_eventWrite->stop();
}

void SkpHiredis::onRead(bool isTimeOut)
{
    if(isTimeOut) {
        return;
    }

    redisAsyncHandleRead(m_asyncContext);
}

void SkpHiredis::onWrite(bool isTimeOut)
{
    if(isTimeOut) {
        return;
    }
    redisAsyncHandleWrite(m_asyncContext);
}

int SkpHiredis::asyncConnect(const char *ip, uint16 port)
{
    m_asyncContext = redisAsyncConnect(ip, port);
    if (m_asyncContext->err) {
        printf("Error: %s\n", m_asyncContext->errstr);
        return skp_false;
    }
    m_asyncContext->ev.addRead = onAddRead;
    m_asyncContext->ev.delRead = onDelRead;
    m_asyncContext->ev.addWrite = onAddWrite;
    m_asyncContext->ev.delWrite = onDelWrite;
    m_asyncContext->ev.cleanup = onCleanup;
    m_asyncContext->ev.data = this;

    m_eventRead = new SkpEventRead(*m_thread);
    m_eventWrite = new SkpEventWrite(*m_thread);

    m_eventRead->connect(m_asyncContext->c.fd, std::bind(&SkpHiredis::onRead, this, std::placeholders::_1));
    m_eventRead->start(0);

    m_eventWrite->connect(m_asyncContext->c.fd, std::bind(&SkpHiredis::onWrite, this, std::placeholders::_1));
    m_eventWrite->start(0);


    redisAsyncSetConnectCallback(m_asyncContext,asyncConnectCallback);
    redisAsyncSetDisconnectCallback(m_asyncContext,asyncDisconnectCallback);

    return skp_true;
}

void SkpHiredis::asyncCommandCallback(redisAsyncContext *asyncContext, void *r, void *privdata)
{
    SKP_UNUSED(asyncContext);
    redisReply *reply = (redisReply *)r;
    if (reply == NULL) return;
    SkpHiredis *hiredis = (SkpHiredis *)privdata;
    hiredis->asyncCommandCallback_do(reply);
}

void SkpHiredis::asyncCommandCallback_do(redisReply *reply)
{
    SKP_UNUSED(reply);
    //printf("str: %s \n", reply->str);
    // not freeReplyObject
    //freeReplyObject(reply);
}

int SkpHiredis::asyncCommand(const char *format, ...)
{
    SKP_ASSERT(m_asyncContext);
    va_list ap;
    int status;
    va_start(ap,format);
    status = redisvAsyncCommand(m_asyncContext,asyncCommandCallback,this,format,ap);
    va_end(ap);
    return status;
}


SkpHiredisPool::SkpHiredisPool(SkpThread *thread) :
    m_maxConnect(100),
    m_thread(thread)
{

}

SkpHiredisPool::~SkpHiredisPool()
{

}
