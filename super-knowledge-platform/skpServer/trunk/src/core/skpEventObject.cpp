#include "skpEventObject.h"
#include "skpEventObject_p.h"
#include "skpThread.h"
//===============SkpEventObjectPrivate
SkpEventObjectPrivate::SkpEventObjectPrivate() :
    SkpObjectPrivate(),
    m_currType(EventTypeInit),
    m_data(skp_null),
    m_time(0)
{
    m_in.reserve(1000);
    m_out.reserve(1000);
}

SkpEventObjectPrivate::~SkpEventObjectPrivate()
{

}
//================SkpEventObject

SkpEventObject::SkpEventObject(SkpObject *parent) :
    SkpObject(*new SkpObjectPrivate(), parent)
{

}

SkpEventObject::SkpEventObject(SkpThread &thread) :
    SkpObject(*new SkpObjectPrivate(), thread)
{

}

SkpEventObject::SkpEventObject(SkpEventObjectPrivate &d, SkpObject *parent) :
    SkpObject(d, parent)
{
}

SkpEventObject::SkpEventObject(SkpEventObjectPrivate &d, SkpThread &thread) :
    SkpObject(d, thread)
{

}

SkpEventObject::~SkpEventObject()
{
}

void SkpEventObject::onCallback(int fd, short event, void *arg)
{
    SKP_UNUSED(fd);
    SkpEventObject *eventObject = (SkpEventObject *)arg;
    eventObject->onData(event);
}

void SkpEventObject::onData(short event)
{
    SKP_UNUSED(event);
}

void SkpEventObject::addEvent(int type)
{
    SKP_D(SkpEventObject);
    if(pthread_self() == thread()->threadID()
            || thread()->threadID() == (pthread_t)-1) {
        bool isPush = skp_false;
        if(skp_true) {
            __SkpAutoLocker(skpD->m_mutex);
            if(!skpD->m_in.empty()) {
                skpD->m_in.push_back(type);
                isPush = skp_true;
            }
        }

        if(isPush) {
            onEvent();
        } else {
            doEvent(type);
        }

    } else {
        if(skp_true) {
            __SkpAutoLocker(skpD->m_mutex);
            skpD->m_in.push_back(type);
            if(skpD->m_in.size() > 1)
                return;
        }
        thread()->addEvent(this);
    }
}


void SkpEventObject::onEvent()
{
    SKP_D(SkpEventObject);
    if(skp_true) {
        __SkpAutoLocker(skpD->m_mutex);
        skpD->m_out.swap(skpD->m_in);
    }

    for(uint i = 0; i < skpD->m_out.size(); i++) {
        int type = skpD->m_out[i];
        doEvent(type);
    }

    skpD->m_out.clear();
}

void SkpEventObject::doEvent(int type)
{
    SKP_D(SkpEventObject);
    if(type == skpD->m_currType)
        return;
    if(skpD->m_currType == EventTypeFree)
        return;
        //SKP_ASSERT(skp_false);

    if(type != EventTypeStart && skpD->m_currType == EventTypeInit)
        return;

    skpD->m_currType = type;

    if(type == EventTypeStart) {
        if(skpD->m_data)
            thread()->SkpEvent::start(skpD->m_data, skpD->m_time);
        else
            skpD->m_data = registerEvent();
    }else if(type == EventTypeStop) {
        thread()->SkpEvent::stop(skpD->m_data);
    } else if(type == EventTypeFree){
        thread()->SkpEvent::free(skpD->m_data);
    } else {
        SKP_ASSERT(skp_false);
    }
}
