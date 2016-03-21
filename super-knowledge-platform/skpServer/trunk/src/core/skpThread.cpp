#include "skpThread.h"
#include "skpThread_p.h"
#include "skpAutoFree.h"
#include "skpEventObject.h"
#include "skpEventRead.h"
#include "skpEventTimer.h"
#include "skpApplication.h"
#include "skpLog.h"

//std::vector<SkpThread *> SkpThreadPrivate::m_eventThreads;
SkpMutex SkpThreadPrivate::m_threadmutex;
std::map<pthread_t, SkpThread *> SkpThreadPrivate::m_threadMap;


SkpThreadPrivate::SkpThreadPrivate() :
    SkpEventPrivate(),
    m_isRequestStop(skp_false),
    m_isRunning(skp_false),
    m_isRequestExit(skp_false),
    m_isExit(skp_true),
    m_isStart(skp_false),
    m_isJoin(skp_false),
    m_threadID(-1),
    m_eventRead(NULL),
    m_timer(NULL)
{

}

SkpThreadPrivate::~SkpThreadPrivate()
{

}

SkpThread* SkpThreadPrivate::currThread(pthread_t threadID)
{
//    __SkpAutoLocker(m_threadmutex);
//    for(uint i = 0; i < m_eventThreads.size(); i++) {
//        SkpThread *thread = m_eventThreads.at(i);
//        if(thread->threadID() == threadID)
//            return thread;
//    }

//    SKP_ASSERT(skp_false);

//    return skp_null;

    __SkpAutoLocker(m_threadmutex);
    SkpThread *thread = m_threadMap[threadID];
    SKP_ASSERT(thread);
    return thread;
}

void SkpThreadPrivate::appendThread(SkpThread * thread)
{
//    __SkpAutoLocker(m_threadmutex);
//    m_eventThreads.push_back(thread);
    __SkpAutoLocker(m_threadmutex);
    m_threadMap.insert(std::pair<pthread_t, SkpThread*>(thread->threadID(), thread));
}

void SkpThreadPrivate::removeThread(SkpThread * thread)
{
//    __SkpAutoLocker(m_threadmutex);

//    if(m_eventThreads.empty())
//        throw;

//    auto iter = std::find(m_eventThreads.begin(), m_eventThreads.end(), thread);
//    if(iter != m_eventThreads.end())
//        m_eventThreads.erase(iter);
    __SkpAutoLocker(m_threadmutex);
    m_threadMap.erase(thread->threadID());
}


///===============================SkpThread
SkpThread::SkpThread() :
    SkpEvent(*new SkpThreadPrivate())
{
    init();
}

SkpThread::SkpThread(SkpThreadPrivate &d) :
    SkpEvent(d)
{
    init();
}

SkpThread::~SkpThread()
{
    exit();
    join();
}

void SkpThread::init()
{
    SKP_D(SkpThread);
    skpD->m_eventIn.reserve(1000);
    skpD->m_eventOut.reserve(1000);
    skpD->m_deleteLaterObjects.reserve(1000);
    skpD->m_waitObjects.reserve(1000);
    createPipe();
    createDeleteLater();
}

void SkpThread::destory()
{
    SKP_D(SkpThread);
    onDeleteLater();
    onEvent();

    skp_delete(skpD->m_timer);
    skp_delete(skpD->m_eventRead);

    SkpThread::removeThread(this);
}

void SkpThread::start(Skp::THREAD_STATE property)
{
    SKP_D(SkpThread);
    if(!skpD->m_isStart)
    {
        updateThreadID();
        SkpThread::appendThread(this);

        skpD->m_isStart = skp_true;
        pthread_attr_t attr;

        if(property == Skp::DETACHED)
        {
            pthread_attr_init (&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        } else {
            skpD->m_isJoin = skp_true;
        }

        int ret = 0;
        ret = pthread_create(&skpD->m_threadID, (property == Skp::DETACHED)? &attr:NULL, SkpThread::work, this);

        if(property == Skp::DETACHED)
            pthread_attr_destroy(&attr);

        SKP_ASSERT(ret == 0);

        ///等待线程运行
        while(isExit());
    }
    else
    {
        if(!skpD->m_isExit)
            skpD->m_isRequestStop = skp_false;
    }
}

void SkpThread::run()
{
    SKP_D(SkpThread);
    while(!skpD->m_isRequestStop) {
        loop();
    }
}

void SkpThread::stop()
{
    SKP_D(SkpThread);
    skpD->m_isRequestStop = skp_true;
}

void SkpThread::exit()
{
    SKP_D(SkpThread);
    skpD->m_isRequestStop = skp_true;
    skpD->m_isRequestExit = skp_true;
}

void SkpThread::join()
{
    ///等待线程推出
    while(!isExit());

    SKP_D(SkpThread);
    if(skpD->m_isJoin)
    {
        skpD->m_isJoin = skp_false;
        pthread_join(threadID(), NULL);
    }
}

bool SkpThread::isRunning()
{
    SKP_D(SkpThread);
    return skpD->m_isRunning;
}

bool SkpThread::isExit()
{
    SKP_D(SkpThread);
    return skpD->m_isExit;
}

pthread_t SkpThread::threadID()
{
    SKP_D(SkpThread);
    return skpD->m_threadID;
}

void SkpThread::updateThreadID()
{
    SKP_D(SkpThread);
    skpD->m_threadID = pthread_self();
}

void *SkpThread::work(void *arg)
{
    if(!arg)
        return NULL;
     SkpThread *thread = (SkpThread *)arg;
     return thread->do_work();
}


void *SkpThread::do_work()
{
    SKP_D(SkpThread);
    //有可能线程ID还没更新
    updateThreadID();
    skpD->m_isExit = skp_false;
    while(skp_true)
    {
        skpD->m_isRunning = skp_true;
        run();
        skpD->m_isRunning = skp_false;
        while(skpD->m_isRequestStop)
        {
            usleep(1000);
            ///pthread_yield(); 放入线程队列后面
            if(skpD->m_isRequestExit)
                break;
        }

        if(skpD->m_isRequestExit)
            break;
    }



    destory();

    skpD->m_isExit = skp_true;

    return NULL;
}


void SkpThread::addEvent(SkpEventObject *eventObject)
{
    SKP_D(SkpThread);
    __SkpAutoLocker(skpD->m_pipeMutex);
    skpD->m_eventIn.push_back(eventObject);
    if(skpD->m_eventIn.size() > 1)
        return;

    char buf[] = "p";
    ::write(skpD->m_pipe.notifyWrite(), buf, 1);
}

void SkpThread::onEvent(bool isTimeOut)
{
    SKP_D(SkpThread);
    SKP_UNUSED(isTimeOut);
    if(skp_true) {
        __SkpAutoLocker(skpD->m_pipeMutex);

        char buf[4096];

        while(skp_true) {
            if(::read(skpD->m_pipe.notifyRead(), buf, 4096) <= 0) {
                break;
            }
        }

        if(skpD->m_eventIn.empty()) {
            return;
        }

        skpD->m_eventOut.swap(skpD->m_eventIn);
    }

    for(auto iter = skpD->m_eventOut.begin(); iter != skpD->m_eventOut.end(); ++iter)
    {
        SkpEventObject *eventObject = *iter;
        if(eventObject) {
            eventObject->onEvent();
        }
    }
    if(!skpD->m_eventOut.empty())
        skpD->m_eventOut.clear();
}

void SkpThread::createPipe()
{
    SKP_D(SkpThread);
    skpD->m_eventRead = new SkpEventRead(*this);
    skpD->m_eventRead->connect(skpD->m_pipe.notifyRead(), std::bind(&SkpThread::onEvent, this, std::placeholders::_1));
    skpD->m_eventRead->setET(skp_false);
    skpD->m_eventRead->start(0);
}

void SkpThread::deleteLater(SkpObject *object)
{
    skpLogDebug_g("deleteLater object = %#x \n", object);
    SKP_D(SkpThread);

    __SkpAutoLocker(skpD->m_deleteLatermutex);

    //if(std::find(skpD->m_waitObjects.begin(), skpD->m_waitObjects.end(), object) != skpD->m_waitObjects.end())
    //    return;

    skpD->m_waitObjects.push_back(object);
    skpD->m_timer->start(500);
}

void SkpThread::onDeleteLater()
{
    SKP_D(SkpThread);

    if(skp_true) {
        __SkpAutoLocker(skpD->m_deleteLatermutex);

        if(skpD->m_waitObjects.empty())
            return;
        skpD->m_waitObjects.swap(skpD->m_deleteLaterObjects);
    }

    SKP_ASSERT(!skpD->m_deleteLaterObjects.empty());
    for(auto itr = skpD->m_deleteLaterObjects.begin(); itr != skpD->m_deleteLaterObjects.end(); itr++) {
        SkpObject *object = *itr;
        skpLogDebug_g("onDeleteLater object = %#x \n", object);
        SKP_ASSERT(object);
        skp_delete(object);
    }

    if(!skpD->m_deleteLaterObjects.empty())
        skpD->m_deleteLaterObjects.clear();
}

void SkpThread::createDeleteLater()
{
    SKP_D(SkpThread);
    skpD->m_timer = new SkpTimer(*this);
    skpD->m_timer->connect(std::bind(&SkpThread::onDeleteLater, this));
    skpD->m_timer->setSingleShot(skp_false);
}


SkpThread* SkpThread::currThread(pthread_t threadID)
{
    return SkpThreadPrivate::currThread(threadID);
}

void SkpThread::appendThread(SkpThread * thread)
{
    SkpThreadPrivate::appendThread(thread);
}

void SkpThread::removeThread(SkpThread * thread)
{
    SkpThreadPrivate::removeThread(thread);
}
