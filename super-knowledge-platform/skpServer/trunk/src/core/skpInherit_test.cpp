#include "skpInherit_test.h"
#include "skpInherit_p_test.h"

SkpInherit_testPrivate::SkpInherit_testPrivate() :
    SkpObjectPrivate(),
    m_inheritCount(0)
{
}

SkpInherit_testPrivate::~SkpInherit_testPrivate()
{

}

///===================SkpInherit_test

SkpInherit_test::SkpInherit_test(SkpObject *parent) :
    SkpObject(*new SkpInherit_testPrivate(), parent)
{

}

SkpInherit_test::SkpInherit_test(SkpThread &thread) :
    SkpObject(*new SkpInherit_testPrivate(), thread)
{

}

SkpInherit_test::SkpInherit_test(SkpInherit_testPrivate &d, SkpObject *parent) :
    SkpObject(d, parent)
{

}

SkpInherit_test::SkpInherit_test(SkpInherit_testPrivate &d, SkpThread &thread) :
    SkpObject(d, thread)
{

}

SkpInherit_test::~SkpInherit_test()
{

}

int SkpInherit_test::inheritCount()
{
    SKP_D(SkpInherit_test);

    return skpD->m_inheritCount;
}
