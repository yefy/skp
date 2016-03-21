#include "skpListEx.h"
#include "skpMallocPoolEx.h"
#include "skpAutoFree.h"


SkpListEx::SkpListEx() :
    SkpList()
{
    m_PoolEx = new SkpMallocPoolEx();
}

SkpListEx::~SkpListEx()
{
    skp_delete(m_PoolEx);
}

void SkpListEx::push(void *head, int headSize, void *data, int dataSize)
{
    push(head, headSize, data, dataSize, NULL);
}

void SkpListEx::push(void *head, int headSize, void *data, int dataSize, std::function<bool(void *, int, void *, int)> func)
{
    int size = headSize + dataSize;
    int allSize = sizeof(SkpPoolData_t) + size;
    SkpPoolData_t *writeData = (SkpPoolData_t *)skp_pool_calloc(m_PoolEx, allSize);
    writeData->headSize = headSize;
    writeData->dataSize = dataSize;
    writeData->isFunc = 0;
    if(func) {
        writeData->isFunc = 1;
        m_func.push_back(func);
    }

    if(head)
        m_PoolEx->write(writeData->buffer, head, headSize);
    if(data)
        m_PoolEx->write(writeData->buffer + headSize, data, dataSize);

    push_back(writeData, &writeData->node);
}

void SkpListEx::push(SkpPoolData_t *poolData, std::function<bool(void *, int, void *, int)> func)
{
    int allSize = sizeof(SkpPoolData_t) + poolData->headSize + poolData->dataSize;
    SkpPoolData_t *writeData = (SkpPoolData_t *)skp_pool_calloc(m_PoolEx, allSize);

    writeData->headSize = poolData->headSize;
    writeData->dataSize = poolData->dataSize;
    writeData->errorNumber = ++poolData->errorNumber;
    writeData->isFunc = 0;
    if(func) {
        writeData->isFunc = 1;
        m_func.push_back(func);
    }

    m_PoolEx->write(writeData->buffer, poolData->buffer, poolData->dataSize);

    push_back(writeData, &writeData->node);
}

std::function<bool(void *, int, void *, int)> SkpListEx::pop(SkpPoolData_t **data)
{
    SkpPoolData_t *poolData = (SkpPoolData_t *)take_pop();
    *data = poolData;
    if(poolData && poolData->isFunc) {
        std::function<bool(void *, int, void *, int)> func = m_func.front();
        m_func.pop_front();
        return func;
    }
    return skp_null;
}

void SkpListEx::free(void *data)
{
    skp_pool_free(m_PoolEx, data);
}
