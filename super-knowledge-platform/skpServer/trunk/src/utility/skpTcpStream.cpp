#include "skpTcpStream.h"
#include "skpLog.h"
#include "skpMallocPoolEx.h"

SkpSocketStreamTcp::SkpSocketStreamTcp(int fd) :
    SkpBuffer(),
    m_fd(fd),
    m_iovec(NULL),
    m_iovecSize()
{
}

SkpSocketStreamTcp::~SkpSocketStreamTcp()
{
}

//static int closeNumber = 0;
//static int errorNumber = 0;
int SkpSocketStreamTcp::skp_readv(int size)
{
    if(size <= 0) {
        size = SKP_ONCE_BUFFER;
    }
    int socketSize;

    if (ioctl(m_fd, FIONREAD, &socketSize) < 0) {
        skpLogError_g("ioctl error \n");
        return SOCKET_BUFFER_ERROR;
    }

    if(socketSize < 0) {
        skpLogError_g("ioctl socketSize error \n");
        return SOCKET_BUFFER_ERROR;
    }

    if(socketSize == 0){
        int bufferMax = SKP_BUFFER_MAX;
        int left = bufferMax - m_data.totalSize;
        if(left < 0) {
            SKP_ASSERT(skp_false);
        } else if(left == 0) {
            skpLogError_g("m_data.totalSize == SKP_BUFFER_MAX \n");
            return SOCKET_BUFFER_FULL;
        } else {
            socketSize = left;
        }
    }

    size = skp_min(size, socketSize);


    int max = (size > 0)?size:SKP_ONCE_BUFFER;
    int bufferMax = SKP_BUFFER_MAX;
    int left = bufferMax - m_data.totalSize;
    if(left < 0) {
        SKP_ASSERT(skp_false);
    }

    left = max = skp_min(max, left);

    if(left == 0) {
        skpLogError_g("read buffer is max \n");
        return SOCKET_BUFFER_FULL;
    }

    skp_buffer_list_t m_tempHead;
    QUEUE_INIT(&m_tempHead);

    int iovecSize = 0;
    while(left > 0) {
        int nodeSize = SKP_BUFFER_NODE_MAX;
        nodeSize = skp_min(nodeSize, left);
        skp_buffer_node_t *node = mallocNode(nodeSize);
        if(!node) {
            SKP_ASSERT(skp_false);
        }
        QUEUE_INSETT_LAST(&m_tempHead, node, chain);

        left -= nodeSize;
        iovecSize++;
    }


    if(QUEUE_EMPTY(&m_tempHead)) {
        return SOCKET_BUFFER_ERROR;
    }

    if(!m_iovec) {
        m_iovec = (struct iovec *)skp_pool_calloc(m_data.pool, sizeof(struct iovec) * iovecSize);
        m_iovecSize = iovecSize;
    }

    if(iovecSize > m_iovecSize) {
        //m_data.pool->SKP_PFREE(m_iovec);
        skp_pool_free(m_data.pool, m_iovec);
        m_iovec = (struct iovec *)skp_pool_calloc(m_data.pool, sizeof(struct iovec) * iovecSize);
        m_iovecSize = iovecSize;
    }


    skp_buffer_node_t *node = m_tempHead.first;
    int nodeSize = 0;

    while(node) {

        m_iovec[nodeSize].iov_base = node->buffer;
        m_iovec[nodeSize].iov_len = node->bufferSize;

        nodeSize++;
        node = node->chain.next;
    }

    left = size = skp_readv(m_fd, m_iovec, iovecSize);


    if(size == 0) {
        //skp_gerror("read socket close \n");
        //closeNumber++;
        //printf("closeNumber = %d\n", closeNumber);

        size = SOCKET_BUFFER_CLOSE;
        left = 0;
    } else if(size < 0) {
        if(errno == EAGAIN || errno == EWOULDBLOCK || errno ==  EINTR) {
            //skp_gerror("read EAGAIN EWOULDBLOCK \n");

            size = SOCKET_BUFFER_NULL;
            left = 0;
        } else {
            //skp_gerror("read socket error \n");
            //errorNumber++;
            //printf("errorNumber = %d\n", errorNumber);

            size = SOCKET_BUFFER_ERROR;
            left = 0;
        }
    }

    m_data.totalSize += left;

    node = m_tempHead.first;

    while(node && left > 0) {
        if(left >= node->bufferSize) {
            node->dataSize = node->bufferSize;
        } else {
            node->dataSize = left;
        }

        left -= node->dataSize;

        QUEUE_REMOVE(&m_tempHead, node, chain);
        QUEUE_INSETT_LAST(&m_data.head, node, chain);

        node = m_tempHead.first;
    }

    while(node) {
        QUEUE_REMOVE(&m_tempHead, node, chain);
        skp_pool_free(m_data.pool, node);
        node = m_tempHead.first;
    }

    return size;
}

int SkpSocketStreamTcp::skp_writev(int size)
{

    int max = ((size > 0)?size:m_data.totalSize);
    int left = m_data.totalSize;
    left = max = skp_min(max, left);
    if(left == 0) {
        skpLogError_g("write buffer is null \n");
        return SOCKET_BUFFER_NULL;
    }

    skp_buffer_node_t *node = m_data.head.first;

    int iovecSize = 0;
    while(node && left > 0) {

        left -= node->dataSize;
        iovecSize++;

        node = node->chain.next;
    }

    if(!m_iovec) {
        m_iovec = (struct iovec *)skp_pool_calloc(m_data.pool, sizeof(struct iovec) * iovecSize);
        m_iovecSize = iovecSize;
    }

    if(iovecSize > m_iovecSize) {
        skp_pool_free(m_data.pool, m_iovec);
        m_iovec = (struct iovec *)skp_pool_calloc(m_data.pool, sizeof(struct iovec) * iovecSize);
        m_iovecSize = iovecSize;
    }

    node = m_data.head.first;
    int nodeSize = 0;
    left = max;
    while(node && (left > 0) && (nodeSize < iovecSize)) {
        int size;
        if(left >= node->dataSize) {
            size = node->dataSize;
        } else {
            size = left;
        }

        left -= size;

        m_iovec[nodeSize].iov_base = node->buffer + node->skipSize;
        m_iovec[nodeSize].iov_len = size;

        nodeSize++;

        node = node->chain.next;
    }

    size = skp_writev(m_fd, m_iovec, iovecSize);


    if(size == 0) {
        //skp_gerror("write socket close \n");
        //closeNumber++;
        //printf("closeNumber = %d\n", closeNumber);

        return SOCKET_BUFFER_CLOSE;
    }
    if(size < 0) {
        if(errno == EAGAIN || errno == EWOULDBLOCK || errno ==  EINTR) {
            //skp_gerror("write EAGAIN EWOULDBLOCK \n");

            return SOCKET_BUFFER_NULL;
        } else {
            //skp_gerror("write socket error \n");
            //errorNumber++;
            //printf("errorNumber = %d\n", errorNumber);

            return SOCKET_BUFFER_ERROR;
        }
    }


    if(skip(size) <= 0) {
        skpLogError_g("skp_skip error \n");
        return SOCKET_BUFFER_ERROR;
    }

    return size;
}

int SkpSocketStreamTcp::skp_readv(int fd, const struct iovec *iov, int iovcnt)
{
    return readv(fd,iov,iovcnt);
}

int SkpSocketStreamTcp::skp_writev(int fd, const struct iovec *iov, int iovcnt)
{
    return writev(fd,iov,iovcnt);
}
