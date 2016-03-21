#include "skpEventRead.h"
#include "skpEventRead_p.h"
#include "skpThread.h"

//==============SkpEventReadPrivate
SkpEventReadPrivate::SkpEventReadPrivate() :
    SkpEventObjectPrivate(),
    m_fd(-1),
    m_singleShot(skp_true),
    m_isET(skp_true),
    m_func(skp_null)
{
}

SkpEventReadPrivate::~SkpEventReadPrivate()
{
}

//==============SkpEventRead
SkpEventRead::SkpEventRead(SkpObject *parent) :
    SkpEventObject(*new SkpEventReadPrivate(), parent)
{

}

SkpEventRead::SkpEventRead(SkpThread &thread) :
    SkpEventObject(*new SkpEventReadPrivate(), thread)
{

}

SkpEventRead::SkpEventRead(SkpEventReadPrivate &d, SkpObject *parent) :
    SkpEventObject(d, parent)
{

}

SkpEventRead::SkpEventRead(SkpEventReadPrivate &d, SkpThread &thread) :
    SkpEventObject(d, thread)
{

}

SkpEventRead::~SkpEventRead()
{
    free();
}

void SkpEventRead::connect(int fd, std::function<void(bool)> func)
{
    SKP_D(SkpEventRead);
    skpD->m_fd = fd;
    skpD->m_func = func;
}

void SkpEventRead::setSingleShot(bool singleShot)
{
    SKP_D(SkpEventRead);
    skpD->m_singleShot = singleShot;
}

void SkpEventRead::setET(bool isET)
{
    SKP_D(SkpEventRead);
    skpD->m_isET = isET;
}

void SkpEventRead::start(int msec)
{
    SKP_D(SkpEventRead);
    SKP_ASSERT(msec >= 0);
    SKP_ASSERT(skpD->m_fd >= 0);
    SKP_ASSERT(skpD->m_func);
    skpD->m_time = msec;
    addEvent(EventTypeStart);
}

void SkpEventRead::stop()
{
    addEvent(EventTypeStop);
}

void SkpEventRead::free()
{
    addEvent(EventTypeFree);
}

void *SkpEventRead::registerEvent()
{
    SKP_D(SkpEventRead);
    return thread()->SkpEvent::read(skpD->m_fd, &SkpEventObject::onCallback, (void *)this, skpD->m_singleShot, skpD->m_time, skpD->m_isET);
}

void SkpEventRead::onData(short event)
{
    SKP_D(SkpEventRead);
    ///底层已经将这个事件停止
    if(!skpD->m_singleShot)
        skpD->m_currType = EventTypeStop;
    skpD->m_func(event);
}

void SkpEventRead::removeReadReady()
{
    SKP_D(SkpEventRead);
    thread()->removeReadReady(skpD->m_data);
}

void SkpEventRead::addReadReady()
{
    SKP_D(SkpEventRead);
    thread()->addReadReady(skpD->m_data);
}
