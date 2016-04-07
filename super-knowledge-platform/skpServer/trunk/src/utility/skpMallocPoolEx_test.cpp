#include "gmock/gmock.h"
#include "skpMallocPoolEx.h"

TEST(skpMallocPoolExTest, skpMallocPoolExTestCase)
{
    SkpMallocPoolEx *pool = new SkpMallocPoolEx();
    void *p1 = skp_pool_calloc(pool, 10);
    ASSERT_TRUE(p1);

    void *p2 = skp_pool_calloc(pool, 10);
    EXPECT_TRUE(p2);
}
