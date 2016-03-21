#include "skpEventTimer.h"
#include "skpEventTimer_p.h"
#include "skpThread.h"
//=================SkpTimerPrivate
SkpTimerPrivate::SkpTimerPrivate() :
    SkpEventObjectPrivate(),
    m_singleShot(skp_true),
    m_func(skp_null)
{
}

SkpTimerPrivate::~SkpTimerPrivate()
{

}
//===================SkpTimer

SkpTimer::SkpTimer(SkpObject *parent) :
    SkpEventObject(*new SkpTimerPrivate(), parent)
{

}

SkpTimer::SkpTimer(SkpThread &thread) :
    SkpEventObject(*new SkpTimerPrivate(), thread)
{

}

SkpTimer::SkpTimer(SkpTimerPrivate &d, SkpObject *parent) :
    SkpEventObject(d, parent)
{

}

SkpTimer::SkpTimer(SkpTimerPrivate &d, SkpThread &thread) :
    SkpEventObject(d, thread)
{

}

SkpTimer::~SkpTimer()
{
    free();
}

void SkpTimer::connect(std::function<void()> func)
{
    SKP_D(SkpTimer);
    skpD->m_func = func;
}

void SkpTimer::setSingleShot(bool singleShot)
{
    SKP_D(SkpTimer);
    skpD->m_singleShot = singleShot;
}

void SkpTimer::start(int msec)
{
    SKP_D(SkpTimer);
    SKP_ASSERT(msec >= 0);
    SKP_ASSERT(skpD->m_func);
    skpD->m_time = msec;
    addEvent(EventTypeStart);
}

void SkpTimer::stop()
{
    addEvent(EventTypeStop);
}

void SkpTimer::free()
{
    addEvent(EventTypeFree);
}

void *SkpTimer::registerEvent()
{
    SKP_D(SkpTimer);
    return thread()->SkpEvent::timeout(skpD->m_time, &SkpEventObject::onCallback, (void *)this, skpD->m_singleShot);
}

void SkpTimer::onData(short event)
{
    SKP_D(SkpTimer);
    SKP_UNUSED(event);
    ///底层已经将这个事件停止
    if(!skpD->m_singleShot)
        skpD->m_currType = EventTypeStop;
    skpD->m_func();
}
