#include "skpEventChan.h"
#include"skpEventChan_p.h"
#include "skpAutoFree.h"
#include "skpListEx.h"

//==================SkpEventChanPrivate
SkpEventChanPrivate::SkpEventChanPrivate() :
    SkpEventDataPrivate(),
    m_max(0)
{
    m_eventListIn = new SkpListEx();
    m_eventListOut = new SkpListEx();
}

SkpEventChanPrivate::~SkpEventChanPrivate()
{
    skp_delete(m_eventListIn);
    skp_delete(m_eventListOut);
}

//====================SkpEventChan
SkpEventChan::SkpEventChan(SkpObject *parent) :
    SkpEventData(*new SkpEventChanPrivate(), parent)
{
    init();
}

SkpEventChan::SkpEventChan(SkpThread &thread) :
    SkpEventData(*new SkpEventChanPrivate(), thread)
{
    init();
}

SkpEventChan::SkpEventChan(SkpEventChanPrivate &d, SkpObject *parent) :
    SkpEventData(d, parent)
{
    init();
}

SkpEventChan::SkpEventChan(SkpEventChanPrivate &d, SkpThread &thread) :
    SkpEventData(d, thread)
{
    init();
}

SkpEventChan::~SkpEventChan()
{
}

void SkpEventChan::init()
{
    SkpEventData::connect(std::bind(&SkpEventChan::take_pop, this));
}

void SkpEventChan::connect(std::function<bool(void *, int, void *, int)> func)
{
    SKP_D(SkpEventChan);
    skpD->m_func = func;
}

void SkpEventChan::setMax(int size)
{
    SKP_D(SkpEventChan);
    skpD->m_max = size;
}

bool SkpEventChan::push_back(void *head, int headSize, void *data, int dataSize)
{
    return push_back(head, headSize, data, dataSize, NULL);
}

bool SkpEventChan::push_back(void *head, int headSize, void *data, int dataSize, std::function<bool(void *, int, void *, int)> func)
{
    SKP_D(SkpEventChan);
    if(skp_true) {
        __SkpAutoLocker(skpD->m_eventListInMutex);
        if(isFull())
            return skp_false;

        skpD->m_eventListIn->push(head, headSize, data, dataSize, func);
     }

    SkpEventData::start();
    return skp_true;
}

void SkpEventChan::take_pop()
{
    SKP_D(SkpEventChan);
    if(skp_true) {
        __SkpAutoLocker(skpD->m_eventListInMutex);

        SkpListEx *temp = skpD->m_eventListIn;
        skpD->m_eventListIn = skpD->m_eventListOut;
        skpD->m_eventListOut = temp;
    }

    while(skp_true) {
        SkpPoolData_t *writeData = NULL;
        std::function<bool(void *, int, void *, int)> func = skpD->m_eventListOut->pop(&writeData);
        if(writeData == NULL)
            break;
        SKP_ASSERT(skpD->m_func || func);
        if(skpD->m_func) {
            skpD->m_func(writeData->buffer, writeData->headSize, writeData->buffer + writeData->headSize, writeData->dataSize);
        } else {
            func(writeData->buffer, writeData->headSize, writeData->buffer + writeData->headSize, writeData->dataSize);
        }

        skpD->m_eventListOut->free(writeData);
    }
}

bool SkpEventChan::isFull()
{
    SKP_D(SkpEventChan);
    if(skpD->m_max) {
        int all = skpD->m_eventListIn->size();
        all += skpD->m_eventListOut->size();
        if(all > skpD->m_max)
            return skp_true;
        else
            return skp_false;
    }
    return skp_false;
}
