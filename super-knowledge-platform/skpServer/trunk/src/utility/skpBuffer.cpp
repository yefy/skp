#include "skpBuffer.h"
#include "skpMallocPoolEx.h"
#include "skpAutoFree.h"

SkpBuffer::SkpBuffer()
{
    m_data.totalSize = 0;
    QUEUE_INIT(&m_data.head);
    m_data.pool = new SkpMallocPoolEx(SKP_TCP_POOL_ONCE_MAX);
}

SkpBuffer::~SkpBuffer()
{
    skp_buffer_node_t *node = m_data.head.first;
    while(node) {
        QUEUE_REMOVE(&m_data.head, node, chain);
        skp_pool_free(m_data.pool, node);
        node = m_data.head.first;
    }

    skp_delete(m_data.pool);
}

int SkpBuffer::append(void *data, int size)
{
    if(!data || size <= 0)
        return 0;

    skp_buffer_node_t *node = mallocNode(size);
    SKP_ASSERT(node);

    if(m_data.pool->write(node->buffer, data, size) == skp_false) {
        return 0;
    }

    node->dataSize = size;
    QUEUE_INSETT_LAST(&m_data.head, node, chain);

    m_data.totalSize += size;

    return size;
}

int SkpBuffer::copy(void *data, int size)
{
    skp_buffer_node_t *node = m_data.head.first;
    int tatolSize = 0;
    int readSize = 0;
    while(node && size > 0) {
        if(size >= node->dataSize) {
            readSize =  node->dataSize;
        } else {
            readSize = size;
        }

        int res = m_data.pool->read(node->buffer + node->skipSize, ((char *)data) + tatolSize, readSize);
        if(res == skp_false) {
            return -1;
        }

        tatolSize += readSize;
        size -= readSize;
        node = node->chain.next;
    }

    return tatolSize;
}

int SkpBuffer::read(void *data, int size)
{
    size = copy(data, size);
    return skip(size);
}

int SkpBuffer::read(SkpBuffer *buffer, int size)
{
    void *data = buffer->appendNode(size);
    int copySize = copy(data, size);
    SKP_ASSERT(copySize == size);
    int skipSize =  skip(copySize);
    SKP_ASSERT(skipSize == size);
    return size;
}

int SkpBuffer::skip(int size)
{
    if((uint64)size > m_data.totalSize || size <= 0) {
        return 0;
    }

    int left = size;
    m_data.totalSize -= size;

    skp_buffer_node_t *node = m_data.head.first;
    while(node && size > 0) {
        if(size >= node->dataSize) {

            size -= node->dataSize;
            QUEUE_REMOVE(&m_data.head, node, chain);

            skp_pool_free(m_data.pool, node);
        } else {

            node->skipSize += size;
            node->dataSize -= size;
            size -= size;
        }

        node = m_data.head.first;
    }

    return left - size;
}

int SkpBuffer::series(int size)
{
    if(m_data.totalSize == 0) {
        return 0;
    }

    if(size < 0) {
        return -1;
    }



    if(size == 0 || (uint64)size > m_data.totalSize) {
        size = m_data.totalSize;
    }

    skp_buffer_node_t *node = m_data.head.first;
    if(node->dataSize >= size)
        return size;

    skp_buffer_node_t * newNode = (skp_buffer_node_t *)this->mallocNode(size);
    if(!newNode) {
        return -1;
    }

    int left = size;
    int copySize = 0;
    while(node && left > 0) {
        if(left >= node->dataSize) {
            copySize = node->dataSize;
            m_data.pool->write(newNode->buffer + newNode->skipSize + newNode->dataSize, node->buffer + node->skipSize, copySize);
            QUEUE_REMOVE(&m_data.head, node, chain);
            skp_pool_free(m_data.pool, node);
        } else {
            copySize = left;
            m_data.pool->write(newNode->buffer + newNode->skipSize + newNode->dataSize, node->buffer + node->skipSize, copySize);
            node->skipSize += copySize;
            node->dataSize -= copySize;

        }
        newNode->dataSize += copySize;
        left -= copySize;

        node = m_data.head.first;
    }

    QUEUE_INSERT_FIRST(&m_data.head, newNode, chain);

    return size - left;
}

void *  SkpBuffer::byte()
{
    if(m_data.totalSize == 0) {
        return NULL;
    }

    return m_data.head.first->buffer + m_data.head.first->skipSize;
}

int SkpBuffer::size()
{
    return m_data.totalSize;
}

void *SkpBuffer::appendNode(int size)
{
    skp_buffer_node_t *node = mallocNode(size);
    SKP_ASSERT(node);

    node->dataSize = size;
    QUEUE_INSETT_LAST(&m_data.head, node, chain);

    m_data.totalSize += size;

    return node->buffer;
}

skp_buffer_node_t *SkpBuffer::mallocNode(int size)
{
    uint mallocSize = size + SKP_BUFFER_NODE_SIZE;
    skp_buffer_node_t * node = (skp_buffer_node_t *)skp_pool_calloc(m_data.pool, mallocSize);
    SKP_ASSERT(node);

    node->skipSize = 0;
    node->dataSize = 0;
    node->bufferSize = size;
    node->buffer = (char *)(node + 1);
    QUEUE_NODE_INIT(node, chain);

    return node;
}


