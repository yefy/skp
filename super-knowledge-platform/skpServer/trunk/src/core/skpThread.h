#ifndef SKPTHREAD_H
#define SKPTHREAD_H

#include "skpEvent.h"

namespace Skp {

enum THREAD_STATE
{
    DETACHED = 0,
    JOINABLE
};

}

class SkpThreadPrivate;
class SkpEventObject;
class SkpObject;

class SkpThread : public SkpEvent
{
public:
    SkpThread();
    virtual ~SkpThread();
protected:
    SkpThread(SkpThreadPrivate &d);
private:
    SKP_DECLARE_PRIVATE(SkpThread);

public:
    void init();
    void destory();
    void start(Skp::THREAD_STATE property = Skp::JOINABLE);
    void stop();
    void exit();
    void join();
    virtual void run();
    bool isRunning();
    bool isExit();
    pthread_t threadID();
    void updateThreadID();

    static SkpThread *currThread(pthread_t threadID = pthread_self());
    static void appendThread(SkpThread * thread);
    static void removeThread(SkpThread * thread);


private:
    static void *work(void *arg);
public:
    void *do_work();

public:
    void addEvent(SkpEventObject *eventObject);
    void onEvent(bool isTimeOut = skp_false);
private:
    void createPipe();

public:
    void deleteLater(SkpObject *object);
    void onDeleteLater();
private:
    void createDeleteLater();
};

#endif // SKPTHREAD_H
