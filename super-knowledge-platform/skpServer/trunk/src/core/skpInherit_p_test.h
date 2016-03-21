#ifndef SKPINHERIT_P_TEST_H
#define SKPINHERIT_P_TEST_H

#include "skpObject_p.h"
#include "skpInherit_test.h"

class SkpInherit_testPrivate : public SkpObjectPrivate
{
public:
  SkpInherit_testPrivate();
  virtual ~SkpInherit_testPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpInherit_test);

private:
  int m_inheritCount;
};


#endif // SKPINHERIT_P_TEST_H
