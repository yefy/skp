#ifndef SKPLISTEX_H
#define SKPLISTEX_H

#include "skpList.h"

typedef struct SkpPoolData_s SkpPoolData_t;
struct SkpPoolData_s {
    SkpListNode_t node;
    int headSize;
    int dataSize;
    uint isFunc:1;
    uint errorNumber:8;
    uint R:23;
    uint R2;
    char buffer[0];
};


class SkpMallocPoolEx;

class SkpListEx : public SkpList
{
public:
    SkpListEx();
    virtual ~SkpListEx();
    void push(void *head, int headSize, void *data, int dataSize);
    void push(void *head, int headSize, void *data, int dataSize, std::function<bool(void *, int, void *, int)> func);
    void push(SkpPoolData_t *poolData, std::function<bool(void *, int, void *, int)> func);
    std::function<bool(void *, int, void *, int)> pop(SkpPoolData_t **data);
    void free(void *data);
    SkpMallocPoolEx *m_PoolEx;
    std::deque<std::function<bool(void *, int, void *, int)> > m_func;

};
#endif // SKPLISTEX_H
