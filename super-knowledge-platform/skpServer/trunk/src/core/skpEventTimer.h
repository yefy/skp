#ifndef SKPEVENTTIMER_H
#define SKPEVENTTIMER_H

#include "skpEventObject.h"

class SkpTimerPrivate;

class SkpTimer : public SkpEventObject
{
public:
    SkpTimer(SkpObject *parent = 0);
    SkpTimer(SkpThread &thread);
    virtual ~SkpTimer();
protected:
    SkpTimer(SkpTimerPrivate &d, SkpObject *parent = 0);
    SkpTimer(SkpTimerPrivate &d, SkpThread &thread);
private:
    SKP_DECLARE_PRIVATE(SkpTimer);

public:
    void connect(std::function<void()> func);
    void setSingleShot(bool singleShot);
    void start(int msec);
    void stop();
    void free();
    virtual void *registerEvent();
    void onData(short event);
};

#endif // SKPEVENTTIMER_H
