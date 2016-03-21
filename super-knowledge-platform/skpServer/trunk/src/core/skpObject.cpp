#include "skpObject.h"
#include "skpObject_p.h"
#include "skpAutoFree.h"
#include "skpApplication.h"
#include "skpThread.h"

SkpObjectPrivate::SkpObjectPrivate() :
    SkpObjectDataPrivate(),
    m_parent(skp_null),
    m_objectCount(0),
    m_thread(skp_null),
    m_isDeleteLater(skp_false)
{
}

SkpObjectPrivate::~SkpObjectPrivate()
{

}

///============================SkpObject

SkpObject::SkpObject(SkpObject *parent) :
    SkpObjectData(*new SkpObjectPrivate())
{
    init(parent, skp_null);
}

SkpObject::SkpObject(SkpThread &thread) :
    SkpObjectData(*new SkpObjectPrivate())
{
    init(skp_null, &thread);
}

SkpObject::SkpObject(SkpObjectPrivate &d, SkpObject *parent) :
    SkpObjectData(d)
{
    init(parent, skp_null);
}

SkpObject::SkpObject(SkpObjectPrivate &d, SkpThread &thread) :
    SkpObjectData(d)
{
    init(skp_null, &thread);
}

SkpObject::~SkpObject()
{
    destroy();
}

int SkpObject::objectCount()
{
    SKP_D(SkpObject);

    return skpD->m_objectCount;
}

int SkpObject::objectCountIncr()
{
    SKP_D(SkpObject);
    skpD->m_objectCount++;

    return skpD->m_objectCount;
}

int SkpObject::objectCountDecr()
{
    SKP_D(SkpObject);
    skpD->m_objectCount--;

    return skpD->m_objectCount;
}

int SkpObject::objectCountIncr_atomic()
{
    SKP_D(SkpObject);
    __SkpAutoLocker(skpD->m_objectCountMutex);
    return objectCountIncr();
}

int SkpObject::objectCountDecr_atomic()
{
    SKP_D(SkpObject);
    __SkpAutoLocker(skpD->m_objectCountMutex);
    return objectCountDecr();
}

SkpMutex &SkpObject::objectCountMutex()
{
    SKP_D(SkpObject);
    return skpD->m_objectCountMutex;
}

SkpObject *SkpObject::parent()
{
    SKP_D(SkpObject);
    return skpD->m_parent;
}

void SkpObject::init(SkpObject *parent, SkpThread *thread)
{
    SKP_D(SkpObject);
    skpD->q_ptr = this;
    skpD->m_parent = parent;
    skpD->m_thread = (parent ? parent->thread() : thread);
    if(skpD->m_thread == skp_null) {
        skpD->m_thread = SkpThread::currThread();
        SKP_ASSERT(skpD->m_thread);
    }

    if(skpD->m_parent)
        skpD->m_parent->addChilden(this);
}

void SkpObject::destroy()
{
    SKP_D(SkpObject);
    clearChilden();

    if(skpD->m_parent) {
        skpD->m_parent->removeChilden(this);
        skpD->m_parent = 0;
    }
}

void SkpObject::setParent(SkpObject *parent)
{
    SKP_D(SkpObject);
    skpD->m_parent = parent;
    if(skpD->m_parent)
        skpD->m_parent->addChilden(this);
}

void SkpObject::removeChilden(SkpObject *childen)
{
    SKP_D(SkpObject);
    skpD->m_ChildenObjectList.remove(childen);
}

void SkpObject::addChilden(SkpObject *childen)
{
    SKP_D(SkpObject);
    skpD->m_ChildenObjectList.push_back(childen);
}

void SkpObject::clearChilden()
{
    SKP_D(SkpObject);
    for(auto iter = skpD->m_ChildenObjectList.begin(); iter != skpD->m_ChildenObjectList.end(); ++iter)
    {
        SkpObject *childen = *iter;
        if(childen)
        {
            childen->setParent(0);
            skp_delete(childen);
        }
    }

    if(!skpD->m_ChildenObjectList.empty())
        skpD->m_ChildenObjectList.clear();
}

SkpThread *SkpObject::thread()
{
    SKP_D(SkpObject);
    return skpD->m_thread;
}

void SkpObject::deleteLater()
{
    SKP_D(SkpObject);
    if(skpD->m_isDeleteLater)
        return;
    skpD->m_isDeleteLater = skp_true;

    thread()->deleteLater(this);
}

bool SkpObject::isDeleteLater()
{
    SKP_D(SkpObject);
    return skpD->m_isDeleteLater;
}
