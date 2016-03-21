#include "skpObjectData.h"
#include "skpObjectData_p.h"

//==============================SkpObjectDataPrivate

SkpObjectDataPrivate::SkpObjectDataPrivate() :
    q_ptr(NULL)
{
}

SkpObjectDataPrivate::~SkpObjectDataPrivate()
{

}

//==============================SkpObjectData

SkpObjectData::SkpObjectData(SkpObjectDataPrivate &d) :
    d_ptr(&d)
{
    SKP_D(SkpObjectData);
    skpD->q_ptr = this;
}

SkpObjectData::~SkpObjectData()
{
    delete d_ptr;
}
