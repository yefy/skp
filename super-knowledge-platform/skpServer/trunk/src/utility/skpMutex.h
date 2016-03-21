#ifndef SKPMUTEX_H
#define SKPMUTEX_H

#include "skpObjectData.h"
#include "skpObject.h"

class SkpMutexPrivate;

class SkpMutex : public SkpObjectData
{
public:
    SkpMutex();
    ~SkpMutex();
    int init();
    int lock();
    int unlock();
    int trylock();
    int destroy();
private:
    SKP_DECLARE_PRIVATE(SkpMutex);
};


class SkpMutexLockerPrivate;

class SkpMutexLocker : public SkpObjectData
{
public:
    SkpMutexLocker(SkpMutex *mutex);
    virtual ~SkpMutexLocker();
private:
    SKP_DECLARE_PRIVATE(SkpMutexLocker);
};

#define __SkpAutoLocker(mutex) \
    SkpMutexLocker locker(&mutex); \
    SKP_UNUSED(locker)

#endif // SKPMUTEX_H
