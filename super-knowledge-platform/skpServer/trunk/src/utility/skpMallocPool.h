#ifndef SKPMALLOCPOOL_H
#define SKPMALLOCPOOL_H

#include "skpUtility.h"

typedef struct skp_pool_s skp_pool_t;

class SkpMallocPool
{
public:
    SkpMallocPool(int size = 0);
    ~SkpMallocPool();

    void reset();
    void *malloc(int size);
    void *mallocEx(int size);
    void *calloc(int size);
    void *memalign(int size, int alignment);
    bool free(void *p);


private:
    skp_pool_t *create(int size);
    void destroy();
    void *mallocBlock(int size);
    void *mallocLarge(int size);

private:
    skp_pool_t *pool;
};

#endif // SKPMALLOCPOOL_H
