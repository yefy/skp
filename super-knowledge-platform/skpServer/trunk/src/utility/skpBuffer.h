#ifndef SKPBUFFER_H
#define SKPBUFFER_H

#include "skpUtility.h"
#include "skpQueue.h"

#define SKP_BUFFER_MAX      1024 * 8 * 1024
#define SKP_BUFFER_NODE_MAX 512
#define SKP_ONCE_BUFFER     1024 * 8
#define SKP_TCP_POOL_ONCE_MAX   1024 * 8

#define SKP_BUFFER_NODE_SIZE sizeof(skp_buffer_node_t)

class SkpMallocPoolEx;

#define SKP_BUFFER_SPACE(node) ((node).size - (node).skipSize - (node).dataSize)

typedef struct skp_buffer_node_s skp_buffer_node_t;
struct skp_buffer_node_s {
    int skipSize;
    int dataSize;
    int bufferSize;
    char *buffer;
    QUEUE_NODE(skp_buffer_node_t) chain;
};

typedef QUEUE_HEAD(skp_buffer_list_s, skp_buffer_node_t) skp_buffer_list_t;

typedef struct skp_buffer_data_s{
    uint64 totalSize;
    skp_buffer_list_t head;
    SkpMallocPoolEx *pool;
}skp_buffer_data_t;


class SkpBuffer
{
public:
    explicit SkpBuffer();
    virtual ~SkpBuffer();
    int append(void *data, int size);
    int copy(void *data, int size);
    int read(void *data, int size);
    int read(SkpBuffer *buffer, int size);
    int skip(int size);
    int series(int size);
    void *  byte();
    int size();
    void *appendNode(int size);

    skp_buffer_node_t *mallocNode(int size);
    skp_buffer_data_t m_data;
};

#endif // SKPBUFFER_H
