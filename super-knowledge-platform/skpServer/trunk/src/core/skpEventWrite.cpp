#include "skpEventWrite.h"
#include "skpEventWrite_p.h"
#include "skpThread.h"

//===============SkpEventWritePrivate

SkpEventWritePrivate::SkpEventWritePrivate() :
    SkpEventObjectPrivate(),
    m_fd(-1),
    m_singleShot(skp_true),
    m_isET(skp_true),
    m_func(skp_null)
{
}

SkpEventWritePrivate::~SkpEventWritePrivate()
{

}

//===============SkpEventWrite
SkpEventWrite::SkpEventWrite(SkpObject *parent) :
    SkpEventObject(*new SkpEventWritePrivate(), parent)
{

}

SkpEventWrite::SkpEventWrite(SkpThread &thread) :
    SkpEventObject(*new SkpEventWritePrivate(), thread)
{
    free();
}

SkpEventWrite::SkpEventWrite(SkpEventWritePrivate &d, SkpObject *parent) :
    SkpEventObject(d, parent)
{

}

SkpEventWrite::SkpEventWrite(SkpEventWritePrivate &d, SkpThread &thread) :
    SkpEventObject(d, thread)
{

}

SkpEventWrite::~SkpEventWrite()
{
}

void SkpEventWrite::connect(int fd, std::function<void(bool)> func)
{
    SKP_D(SkpEventWrite);
    skpD->m_fd = fd;
    skpD->m_func = func;
}

void SkpEventWrite::setSingleShot(bool singleShot)
{
    SKP_D(SkpEventWrite);
    skpD->m_singleShot = singleShot;
}

void SkpEventWrite::setET(bool isET)
{
    SKP_D(SkpEventWrite);
    skpD->m_isET = isET;
}

void SkpEventWrite::start(int msec)
{
    SKP_D(SkpEventWrite);
    SKP_ASSERT(msec >= 0);
    SKP_ASSERT(skpD->m_fd >= 0);
    SKP_ASSERT(skpD->m_func);
    skpD->m_time = msec;
    addEvent(EventTypeStart);
}

void SkpEventWrite::stop()
{
    addEvent(EventTypeStop);
}

void SkpEventWrite::free()
{
    addEvent(EventTypeFree);
}

void *SkpEventWrite::registerEvent()
{
    SKP_D(SkpEventWrite);
    return thread()->SkpEvent::write(skpD->m_fd, &SkpEventObject::onCallback, (void *)this, skpD->m_singleShot, skpD->m_time, skpD->m_isET);
}

void SkpEventWrite::onData(short event)
{
    SKP_D(SkpEventWrite);
    ///底层已经将这个事件停止
    if(!skpD->m_singleShot)
        skpD->m_currType = EventTypeStop;
    skpD->m_func(event);
}
