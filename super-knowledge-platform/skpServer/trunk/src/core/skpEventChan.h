#ifndef SKPEVENTCHAN_H
#define SKPEVENTCHAN_H

#include "skpEventData.h"


class SkpEventChanPrivate;

class SkpEventChan : public SkpEventData
{
public:
    SkpEventChan(SkpObject *parent = 0);
    SkpEventChan(SkpThread &thread);
    virtual ~SkpEventChan();
protected:
    SkpEventChan(SkpEventChanPrivate &d, SkpObject *parent = 0);
    SkpEventChan(SkpEventChanPrivate &d, SkpThread &thread);
private:
    SKP_DECLARE_PRIVATE(SkpEventChan);
public:
    void init();
    void connect(std::function<bool(void *, int, void *, int)> func);
    void setMax(int size);
    bool push_back(void *head, int headSize, void *data, int dataSize);
    bool push_back(void *head, int headSize, void *data, int dataSize, std::function<bool(void *, int, void *, int)> func);
    void take_pop();
    bool isFull();
};

#endif // SKPEVENTCHAN_H
