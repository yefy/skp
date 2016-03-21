#include "skpMutex.h"
#include "skpObjectData_p.h"
#include "skpObject_p.h"

#ifdef __linux

//===========================SkpMutexPrivate

class SkpMutexPrivate : public SkpObjectDataPrivate
{
public:
  SkpMutexPrivate();
  ~SkpMutexPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpMutex);
  pthread_mutex_t m_mutex;
};

SkpMutexPrivate::SkpMutexPrivate() :
    SkpObjectDataPrivate()
{

}

SkpMutexPrivate::~SkpMutexPrivate()
{


}


//=============================SkpMutex


SkpMutex::SkpMutex() :
    SkpObjectData(*new SkpMutexPrivate())
{
    init();
}

SkpMutex::~SkpMutex()
{
    destroy();
}

int SkpMutex::init()
{
    SKP_D(SkpMutex);
    return pthread_mutex_init(&skpD->m_mutex,NULL);
}

int SkpMutex::lock()
{
    SKP_D(SkpMutex);
    return pthread_mutex_lock(&skpD->m_mutex);
}

int SkpMutex::unlock()
{
    SKP_D(SkpMutex);
    return pthread_mutex_unlock(&skpD->m_mutex);
}

int SkpMutex::trylock()
{
    SKP_D(SkpMutex);
    return pthread_mutex_trylock(&skpD->m_mutex);
}

int SkpMutex::destroy()
{
    SKP_D(SkpMutex);
    return pthread_mutex_destroy(&skpD->m_mutex);
}

#endif



#ifdef _WIN32

#include "skpAutoFree.h"
//================SkpMutexPrivate
class SkpMutexPrivate : public SkpObjectDataPrivate
{
public:
  SkpMutexPrivate(SkpMutex *q, SkpObject *parent);
  ~SkpMutexPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpMutex);
  std::mutex *m_mutex;
};

SkpMutexPrivate::SkpMutexPrivate(SkpMutex *q, SkpObject *parent) :
    SkpObjectDataPrivate(q, parent)
{

}

SkpMutexPrivate::~SkpMutexPrivate()
{

}

//================SkpMutex
SkpMutex::SkpMutex() :
    SkpObjectData(*new SkpMutexPrivate())
{
    init();
}

SkpMutex::~SkpMutex()
{
    destroy();
}

int SkpMutex::init()
{
    SKP_D(SkpMutex);
    skpD->m_mutex = new std::mutex();
}

int SkpMutex::lock()
{
    SKP_D(SkpMutex);
    skpD->m_mutex->lock();
}

int SkpMutex::unlock()
{
    SKP_D(SkpMutex);
    skpD->m_mutex->unlock();
}

int SkpMutex::trylock()
{
    SKP_D(SkpMutex);
    skpD->m_mutex->try_lock();
}

int SkpMutex::destroy()
{
    SKP_D(SkpMutex);
    skp_delete(skpD->m_mutex);
}

#endif



//=============================SkpMutexPrivate
class SkpMutexLockerPrivate : public SkpObjectDataPrivate
{
public:
  SkpMutexLockerPrivate();
  virtual ~SkpMutexLockerPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpMutexLocker);
  SkpMutex *m_mutex;
};

SkpMutexLockerPrivate::SkpMutexLockerPrivate() :
    SkpObjectDataPrivate(),
    m_mutex(skp_null)
{

}

SkpMutexLockerPrivate::~SkpMutexLockerPrivate()
{
}

//=============================SkpMutexLocker

SkpMutexLocker::SkpMutexLocker(SkpMutex *mutex) :
    SkpObjectData(*new SkpMutexLockerPrivate())
{
    SKP_D(SkpMutexLocker);
    skpD->m_mutex = mutex;
    skpD->m_mutex->lock();
}

SkpMutexLocker::~SkpMutexLocker()
{
    SKP_D(SkpMutexLocker);
    skpD->m_mutex->unlock();
}
