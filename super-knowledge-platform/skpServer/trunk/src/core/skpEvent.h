#ifndef SKPEVENT_H
#define SKPEVENT_H

#include "skpObjectData.h"

class SkpEventPrivate;

class SkpEvent : public SkpObjectData
{
public:
    virtual ~SkpEvent();
protected:
    SkpEvent(SkpEventPrivate &d);
private:
    SKP_DECLARE_PRIVATE(SkpEvent);

public:
    void loop();
    void dispatch();

    void *timeout(int64 time, skp_callback_function func, void *arg, bool singleShot);
    void *read(int fd, skp_callback_function func, void *arg, bool singleShot, int64 time = 0, bool isET = skp_false);
    void *write(int fd, skp_callback_function func, void *arg, bool singleShot, int64 time = 0, bool isET = skp_false);

    void stop(void *data);
    void free(void *data);
    void start(void *data, int64 time);
    void skp_break();
    void *skp_base();


    void removeReadReady(void *data);
    void addReadReady(void *data);

    void skp_set_event_thread();
};

#endif // SKPEVENT_H
