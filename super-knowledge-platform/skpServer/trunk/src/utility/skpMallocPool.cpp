#include "skpMallocPool.h"
#include "skpMalloc.h"
#include "skpLog.h"

/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */

#define skp_pagesize 4 * 1024

#define SKP_MAX_ALLOC_FROM_POOL  (skp_pagesize - 1)

#define SKP_DEFAULT_POOL_SIZE    (16 * 1024)
#define SKP_ALIGNMENT   sizeof(unsigned long)
#define SKP_POOL_ALIGNMENT       16
#define SKP_MIN_POOL_SIZE                                                     \
    skp_align((sizeof(skp_pool_t) + 2 * sizeof(skp_pool_large_t)),            \
              SKP_POOL_ALIGNMENT)


struct skp_pool_large_s {
    struct skp_pool_large_s     *next;
    void                 *alloc;
};

typedef struct skp_pool_large_s  skp_pool_large_t;


typedef struct {
    uchar               *last;
    uchar               *end;
    struct skp_pool_s           *next;
    uint            failed;
} skp_pool_data_t;


struct skp_pool_s {
    skp_pool_data_t       d;
    int                max;
    struct skp_pool_s           *current;
    skp_pool_large_t     *large;
};


SkpMallocPool::SkpMallocPool(int size)
{
    pool = create((size > skp_pagesize)? size:skp_pagesize);
}

SkpMallocPool::~SkpMallocPool()
{
    destroy();
}


skp_pool_t *SkpMallocPool::create(int size)
{
    skp_pool_t  *p;

    p = (skp_pool_t  *)skp_memalign(SKP_POOL_ALIGNMENT, size);

    if (p == NULL) {
        SKP_ASSERT(p);
        return NULL;
    }

    p->d.last = (u_char *) p + sizeof(skp_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = NULL;
    p->d.failed = 0;

    size = size - sizeof(skp_pool_t);
    p->max = (size < SKP_MAX_ALLOC_FROM_POOL) ? size : SKP_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->large = NULL;

    return p;
}


void SkpMallocPool::destroy()
{
    skp_pool_t          *p, *n;
    skp_pool_large_t    *l;

    for (l = pool->large; l; l = l->next) {
        if (l->alloc) {
            skp_free(l->alloc);
        }
    }

    for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        skp_free(p);
        if (n == NULL) {
            break;
        }
    }
}


void SkpMallocPool::reset()
{
    skp_pool_t        *p;
    skp_pool_large_t  *l;

    for (l = pool->large; l; l = l->next) {
        if (l->alloc) {
            skp_free(l->alloc);
        }
    }

    pool->large = NULL;

    for (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + sizeof(skp_pool_t);
    }
}


void *
SkpMallocPool::malloc(int size)
{
    u_char      *m;
    skp_pool_t  *p;

    if (size <= pool->max) {

        p = pool->current;

        do {
            m = skp_align_ptr(p->d.last, SKP_ALIGNMENT);

            if ((int) (p->d.end - m) >= size) {
                p->d.last = m + size;

                return m;
            }

            p = p->d.next;

        } while (p);

        return mallocBlock(size);
    }

    return mallocLarge(size);
}


void *SkpMallocPool::mallocEx(int size)
{
    u_char      *m;
    skp_pool_t  *p;

    if (size <= pool->max) {

        p = pool->current;

        do {
            m = p->d.last;

            if ((int) (p->d.end - m) >= size) {
                p->d.last = m + size;

                return m;
            }

            p = p->d.next;

        } while (p);

        return mallocBlock(size);
    }

    return mallocLarge(size);
}


void *SkpMallocPool::mallocBlock(int size)
{
    uchar      *m;
    int       psize;
    skp_pool_t  *p, *pnew, *current;

    psize = (int) (pool->d.end - (u_char *) pool);

    m = (uchar *)skp_memalign(SKP_POOL_ALIGNMENT, psize);
    if (m == NULL) {
        return NULL;
    }

    pnew = (skp_pool_t *) m;

    pnew->d.end = m + psize;
    pnew->d.next = NULL;
    pnew->d.failed = 0;

    m += sizeof(skp_pool_data_t);
    m = skp_align_ptr(m, SKP_ALIGNMENT);
    pnew->d.last = m + size;

    current = pool->current;

    for (p = current; p->d.next; p = p->d.next) {
        if (p->d.failed++ > 4) {
            current = p->d.next;
        }
    }

    p->d.next = pnew;

    pool->current = current ? current : pnew;

    return m;
}


void *SkpMallocPool::mallocLarge(int size)
{
    void *p = skp_malloc(size);
    if (p == NULL) {
        return NULL;
    }

    uint n = 0;

    skp_pool_large_t  *large;
    for (large = pool->large; large; large = large->next) {
        if (large->alloc == NULL) {
            large->alloc = p;
            return p;
        }

        if (n++ > 3) {
            break;
        }
    }

    large = (skp_pool_large_t  *)malloc(sizeof(skp_pool_large_t));
    if (large == NULL) {
        skp_free(p);
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}


void *SkpMallocPool::memalign(int size, int alignment)
{
    void *p = skp_memalign(alignment, size);
    SKP_ASSERT(p);
    if (p == NULL) {
        return NULL;
    }

    skp_pool_large_t *large = (skp_pool_large_t  *)malloc(sizeof(skp_pool_large_t));
    SKP_ASSERT(large);
    if (large == NULL) {
        skp_free(p);
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}


bool SkpMallocPool::free(void *p)
{
    skp_pool_large_t  *l;

    for (l = pool->large; l; l = l->next) {
        if (p == l->alloc) {
            skp_free(l->alloc);
            return skp_true;
        }
    }

    return skp_false;
}


void *SkpMallocPool::calloc(int size)
{
    void *p = malloc(size);
    SKP_ASSERT(p);
    if (p) {
        skp_memzero(p, size);
    }
    return p;
}
