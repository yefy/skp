#include "skpEventData.h"
#include "skpEventData_p.h"
#include "skpThread.h"

//=================SkpEventDataPrivate
SkpEventDataPrivate::SkpEventDataPrivate() :
    SkpEventObjectPrivate(),
    m_func(skp_null),
    m_count(0)
{

}

SkpEventDataPrivate::~SkpEventDataPrivate()
{

}

//=================SkpEventData
SkpEventData::SkpEventData(SkpObject *parent) :
    SkpEventObject(*new SkpEventDataPrivate(), parent)
{

}

SkpEventData::SkpEventData(SkpThread &thread) :
    SkpEventObject(*new SkpEventDataPrivate(), thread)
{

}

SkpEventData::SkpEventData(SkpEventDataPrivate &d, SkpObject *parent) :
    SkpEventObject(d, parent)
{

}

SkpEventData::SkpEventData(SkpEventDataPrivate &d, SkpThread &thread) :
    SkpEventObject(d, thread)
{

}

SkpEventData::~SkpEventData()
{
}

void SkpEventData::connect(std::function<void()> func)
{
    SKP_D(SkpEventData);
    skpD->m_func = func;
}

void SkpEventData::start()
{
    SKP_D(SkpEventData);
    SKP_ASSERT(skpD->m_func);
    addEvent();
}

void SkpEventData::addEvent()
{
    SKP_D(SkpEventData);
    if(pthread_self() == thread()->threadID()
            || thread()->threadID() == (pthread_t)-1) {
        bool isAdd = skp_false;
         if(skp_true) {
            __SkpAutoLocker(skpD->m_mutex);
            if(skpD->m_count > 0) {
                skpD->m_count++;
                isAdd = skp_true;
            }
         }
         if(isAdd)
            onEvent();
         else
             doEvent();
    } else {
        if(skp_true) {
            __SkpAutoLocker(skpD->m_mutex);
            skpD->m_count++;
            if(skpD->m_count > 1)
                return;
        }
        thread()->addEvent(this);
    }
}

void SkpEventData::onEvent()
{
    SKP_D(SkpEventData);
    int count = 0;
    if(skp_true) {
        __SkpAutoLocker(skpD->m_mutex);
        count = skpD->m_count;
        skpD->m_count = 0;
    }
    while(count > 0) {
        doEvent();
        count--;
    }
}

void SkpEventData::doEvent()
{
    SKP_D(SkpEventData);
    skpD->m_func();
}

