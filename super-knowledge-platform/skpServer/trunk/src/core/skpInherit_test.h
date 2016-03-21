#ifndef SKPINHERIT_TEST_H
#define SKPINHERIT_TEST_H

#include "skpObject.h"

class SkpInherit_testPrivate;


class SkpInherit_test : public SkpObject
{
public:
    SkpInherit_test(SkpObject *parent = 0);
    SkpInherit_test(SkpThread &thread);
    virtual ~SkpInherit_test();
protected:
    SkpInherit_test(SkpInherit_testPrivate &d, SkpObject *parent = 0);
    SkpInherit_test(SkpInherit_testPrivate &d, SkpThread &thread);
private:
    SKP_DECLARE_PRIVATE(SkpInherit_test);

public:
    int inheritCount();
};

#endif // SKPINHERIT_TEST_H
