#ifndef SKP_CORE_EVENT_DATA_H
#define SKP_CORE_EVENT_DATA_H

#include "skpEventObject.h"

class SkpEventDataPrivate;

class SkpEventData : public SkpEventObject
{
public:
    SkpEventData(SkpObject *parent = 0);
    SkpEventData(SkpThread &thread);
    virtual ~SkpEventData();
protected:
    SkpEventData(SkpEventDataPrivate &d, SkpObject *parent = 0);
    SkpEventData(SkpEventDataPrivate &d, SkpThread &thread);
private:
    SKP_DECLARE_PRIVATE(SkpEventData);
public:
    void connect(std::function<void()> func);
    void start();
    void addEvent();
    void onEvent();
    void doEvent();
};

#endif // SKP_CORE_EVENT_DATA_H
