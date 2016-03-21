#include "skpEvent.h"
#include "skpEvent_p.h"
#include "skpMallocPoolEx.h"
#include "skpAutoFree.h"
#include "skpLog.h"


#define EPOLL_EVENT_LT    0 ///全部LT
#define EPOLL_EVENT_ET    1 ///全部ET
#define EPOLL_EVENT_LT_ET 2 ///LT ET一起支持
#define EPOLL_EVENT_TYPE EPOLL_EVENT_LT_ET

#define NODE_TYPE_MAP   0   ///即使申请
#define NODE_TYPE_NODE  1   ///固定申请
#define NODE_TYPE_MORE  2   ///变长申请
#define NODE_TYPE NODE_TYPE_MORE


#define IS_CHECK_ERROR 0

SkpEventPrivate::SkpEventPrivate() :
    SkpObjectDataPrivate()
{
    skp_event_base_new();
}

SkpEventPrivate::~SkpEventPrivate()
{
    skp_event_base_free();
}

void SkpEventPrivate::skp_event_base_new()
{
    m_base.m_epfd = epoll_create(32000);
    SKP_ASSERT(m_base.m_epfd != -1);

    m_base.m_nodeSize = 0;
    m_base.m_nodeList = NULL;
    m_base.m_nodeChangeList = new SkpList();
    m_base.m_nodeReadyList = new SkpList();
    m_base.pool = new SkpMallocPoolEx(1024 * 1024);
    m_isThread = skp_false;
}

void SkpEventPrivate::skp_event_base_free()
{
    ::close(m_base.m_epfd);
    skp_delete(m_base.m_nodeChangeList);
    skp_delete(m_base.m_nodeReadyList);
    m_base.m_nodeSize = 0;
    m_base.m_nodeList = NULL;
    skp_delete(m_base.pool);
}

void SkpEventPrivate::skp_event_base_loop()
{
    skp_change_event();

    int64 timer = 100;
    if(m_base.m_nodeReadyList->size() > 0) {
        timer = 0;
    }

    int nevents = 0;
    nevents = epoll_wait(m_base.m_epfd, m_base.m_events, sizeof(m_base.m_events) / sizeof(struct epoll_event), timer);

    if(!m_isThread) {
        skp_update_system_time_ms(skp_true);
    }

    for(int i = 0; i < nevents; ++i)
    {
        struct epoll_event *epollEvent = &m_base.m_events[i];
        skp_epoll_node *node = (skp_epoll_node *)epollEvent->data.ptr;

        if ((epollEvent->events & (EPOLLERR|EPOLLHUP))
                && (epollEvent->events & (EPOLLIN|EPOLLOUT)) == 0)
        {
            epollEvent->events |= EPOLLIN|EPOLLOUT;
            node->m_errorNumber++;
        }

        if(node->m_flags == EPOLLET) {

            skp_add_ready_node(node);

            if(epollEvent->events & EPOLLIN)
            {
                node->m_read.m_ready = 1;
                node->m_read.m_readyNumber++;
            }

            if(epollEvent->events & EPOLLOUT) {
                node->m_write.m_ready = 1;
                node->m_write.m_readyNumber++;
            }

        } else {

            if(epollEvent->events & EPOLLIN)
            {
                skp_epoll_data *read = &node->m_read;
                skp_event_callback(read);
            }

            if(epollEvent->events & EPOLLOUT) {
                skp_epoll_data *write = &node->m_write;
                skp_event_callback(write);
            }
        }
    }

    skp_ready_callback();

    skp_min_tbtree();

}

void SkpEventPrivate::skp_change_event()
{
    skp_epoll_node *node = NULL;
    while((node = skp_remove_change_node())) {

        uint flags = 0;
        int op = 0;
        struct epoll_event ee;
        bool isDeleteNode = skp_false;

        if(node->m_read.m_state == skp::epoll_state_null && node->m_write.m_state == skp::epoll_state_null) {

            op = EPOLL_CTL_DEL;
            node->m_read.m_active = 0;
            node->m_write.m_active = 0;

            node->m_read.m_ready = 0;
            node->m_write.m_ready = 0;
            isDeleteNode = skp_true;
        }

        if(op != EPOLL_CTL_DEL) {
            if(node->m_read.m_active || node->m_write.m_active) {
                op = EPOLL_CTL_MOD;
            } else {
                op = EPOLL_CTL_ADD;
            }
        }

        if(node->m_read.m_state == skp::epoll_state_start) {
            flags |= EPOLLIN;
            node->m_read.m_active = skp_true;
        } else {
            node->m_read.m_ready = 0;
        }

        if(node->m_write.m_state == skp::epoll_state_start) {
            flags |= EPOLLOUT;
            node->m_write.m_active = skp_true;
        } else {
            node->m_write.m_ready = 0;
        }

        if(!node->m_read.m_ready && !node->m_write.m_ready) {
            skp_remove_ready_node(node);
        }

        flags |= node->m_flags;

        if(op != EPOLL_CTL_DEL) {

            if(node->m_oldFlags && node->m_oldFlags == flags)
                continue;
            node->m_oldFlags = flags;

            ee.events = flags;
            ee.data.ptr = node;
        } else {
            node->m_oldFlags = 0;
            ee.events = 0;
            ee.data.ptr = NULL;
        }

        int ret = epoll_ctl(m_base.m_epfd, op, node->m_fd, &ee);
        skpLogDebug_g("epoll change fd = %d, ret = %d\n", node->m_fd, ret);
        if(ret == -1) {
            if (op == EPOLL_CTL_MOD && errno == ENOENT) {
                skpLogError_g("epoll change EPOLL_CTL_ADD \n");
                ret = epoll_ctl(m_base.m_epfd, EPOLL_CTL_ADD, node->m_fd, &ee);
            } else if (op == EPOLL_CTL_ADD && errno == EEXIST) {
                skpLogError_g("epoll change EPOLL_CTL_MOD \n");
                ret = epoll_ctl(m_base.m_epfd, EPOLL_CTL_MOD, node->m_fd, &ee);
            }
            if(op != EPOLL_CTL_DEL)
                SKP_ASSERT(ret != -1);
        }


        if(isDeleteNode) {
            skp_remove_ready_node(node);
            skp_reset_node(node);
        }
    }
}

void SkpEventPrivate::skp_ready_callback()
{
    if(!m_base.m_nodeReadyList->isEmpty()) {
        m_base.m_nodeReadyList->begin();
        void *data = NULL;
        while((data = m_base.m_nodeReadyList->data())) {
            skp_epoll_node *node = (skp_epoll_node *)data;

            if(node->m_read.m_ready) {
                skp_epoll_data *read = &node->m_read;
                skp_event_callback(read);
            }

            if(node->m_write.m_ready) {
                skp_epoll_data *write = &node->m_write;
                skp_event_callback(write);
            }

            m_base.m_nodeReadyList->next();

            if(!node->m_read.m_ready && !node->m_write.m_ready) {
                skp_remove_ready_node(node);
            }
        }
    }
}

void SkpEventPrivate::skp_event_callback(skp_epoll_data *data)
{
    if(data->m_fd > 0 && (data->m_state == skp::epoll_state_start)) {
        data->m_callbackNumber++;
        data->m_updateTime = skp_get_system_time_ms();
        if(!data->m_singleShot) {
            skp_delete_timeout(data);
            skp_event_delete(data);
        }
        (*(data->m_func))(data->m_fd, skp_false, data->m_arg);
    }
}

void SkpEventPrivate::skp_min_tbtree()
{
    util_rbtree_node_t *rbNode = NULL;
    while((rbNode = m_base.m_rbtree.min()) && rbNode && rbNode->key <= skp_get_system_time_ms()) {
        skp_epoll_data *data = (skp_epoll_data *)rbNode->data;
        if(data->m_type != skp::epoll_type_timeout) {
            int64 key = data->m_rbNode->key;
            int64 updateTime = data->m_updateTime;
            int64 diff = key - updateTime;

            if(diff < 0)
                diff = 0;
            if(diff >= data->m_time) {
                skp_delete_timeout(data);
                if(data->m_singleShot) {
                    skp_insert_timeout(data, data->m_time);
                } else {
                    skp_event_delete(data);
                }

                if(data->m_fd > 0 && (data->m_state == skp::epoll_state_start)) {
                    (*(data->m_func))(data->m_fd, skp_true, data->m_arg);
                }
            } else {
                int64 time = data->m_time - diff;
                skp_delete_timeout(data);
                skp_insert_timeout(data, time);
                data->m_updateTime = updateTime;
            }
        } else {
            skp_delete_timeout(data);
            if(data->m_singleShot)
                skp_insert_timeout(data, data->m_time);
            (*(data->m_func))(data->m_fd, skp_true, data->m_arg);

        }
    }
}

void SkpEventPrivate::skp_event_base_dispatch()
{

}

void *SkpEventPrivate::skp_event_base_timeout(int64 time, skp_callback_function func, void *arg, bool singleShot)
{
    SKP_ASSERT(time > 0);
    skp_epoll_data *timeout = skp_malloc_data();
    timeout->m_type = skp::epoll_type_timeout;
    timeout->m_state = skp::epoll_state_start;
    timeout->m_time = time;
    timeout->m_func = func;
    timeout->m_arg = arg;
    timeout->m_singleShot = singleShot;

    skp_add_timeout(timeout);

    return timeout;
}

void *SkpEventPrivate::skp_event_base_read(int fd, skp_callback_function func, void *arg, bool singleShot, int64 time, bool isET)
{
    skp_epoll_node *epollNode = skp_malloc_node(fd);
    if(epollNode->m_fd != 0 && epollNode->m_fd != fd) {
        SKP_ASSERT(skp_false);
    }

    SKP_ASSERT(epollNode->m_read.m_fd == 0);

    if(epollNode->m_fd == 0) {
        skp_reset_node(epollNode);
        epollNode->m_fd = fd;
    }

    if(EPOLL_EVENT_TYPE == EPOLL_EVENT_LT) {
        epollNode->m_flags = 0;
    } else if(EPOLL_EVENT_TYPE == EPOLL_EVENT_ET) {
        epollNode->m_flags = EPOLLET;
    }  else if(EPOLL_EVENT_TYPE == EPOLL_EVENT_LT_ET) {
        if(isET) {
            epollNode->m_flags = EPOLLET;
        }
    } else {
        SKP_ASSERT(skp_false);
    }

    skp_add_change_node(epollNode);

    skp_epoll_data *read = &epollNode->m_read;
    read->m_type = skp::epoll_type_read;
    read->m_state = skp::epoll_state_start;
    read->m_fd = fd;
    read->m_time = time;
    read->m_func = func;
    read->m_arg = arg;
    read->m_singleShot = singleShot;

    skp_add_timeout(read);

    return read;
}

void *SkpEventPrivate::skp_event_base_write(int fd, skp_callback_function func, void *arg, bool singleShot, int64 time, bool isET)
{
    skp_epoll_node *epollNode = skp_malloc_node(fd);
    if(epollNode->m_fd != 0 && epollNode->m_fd != fd) {
        SKP_ASSERT(skp_false);
    }

    SKP_ASSERT(epollNode->m_write.m_fd == 0);

    if(epollNode->m_fd == 0) {
        skp_reset_node(epollNode);
        epollNode->m_fd = fd;
    }

    if(EPOLL_EVENT_TYPE == EPOLL_EVENT_LT) {
        epollNode->m_flags = 0;
    } else if(EPOLL_EVENT_TYPE == EPOLL_EVENT_ET) {
        epollNode->m_flags = EPOLLET;
    }  else if(EPOLL_EVENT_TYPE == EPOLL_EVENT_LT_ET) {
        if(isET) {
            epollNode->m_flags = EPOLLET;
        }
    } else {
        SKP_ASSERT(skp_false);
    }

    skp_add_change_node(epollNode);

    skp_epoll_data *write = &epollNode->m_write;
    write->m_type = skp::epoll_type_write;
    write->m_state = skp::epoll_state_start;
    write->m_fd = fd;
    write->m_time = time;
    write->m_func = func;
    write->m_arg = arg;
    write->m_singleShot = singleShot;

    skp_add_timeout(write);

    return write;
}


#define IS_ADD_TIMEOUT  0

void SkpEventPrivate::skp_event_delete(void *data)
{
    skp_epoll_data *event = (skp_epoll_data *)data;
    SKP_ASSERT(event);
    SKP_ASSERT(event->m_type >= skp::epoll_type_timeout && event->m_type <= skp::epoll_type_write);

    if(event->m_type == skp::epoll_type_timeout) {
        skp_sub_timeout(event);
    } else {
        if(event->m_state != skp::epoll_state_stop)
            skp_change_node(event, skp::epoll_state_stop);

#if IS_ADD_TIMEOUT
        skp_delete_timeout(event);
#endif

    }
}

void SkpEventPrivate::skp_event_free(void *data)
{
    skp_epoll_data *event = (skp_epoll_data *)data;
    SKP_ASSERT(event);
    SKP_ASSERT(event->m_type >= skp::epoll_type_timeout && event->m_type <= skp::epoll_type_write);

    if(event->m_type == skp::epoll_type_timeout) {
        skp_delete_timeout(event);
        skp_free_data(event);
    } else {
        if(event->m_state != skp::epoll_state_null)
            skp_change_node(event, skp::epoll_state_null);

        skp_delete_timeout(event);
    }
}

void SkpEventPrivate::skp_event_start(void *data, int64 time)
{
    skp_epoll_data *event = (skp_epoll_data *)data;
    SKP_UNUSED(time);
    SKP_ASSERT(event);
    SKP_ASSERT(event->m_type >= skp::epoll_type_timeout && event->m_type <= skp::epoll_type_write);

    if(event->m_type == skp::epoll_type_timeout) {
        skp_add_timeout(event);
    } else {
        if(event->m_state != skp::epoll_state_start)
            skp_change_node(event, skp::epoll_state_start);
#if IS_ADD_TIMEOUT
        skp_add_timeout(event);
#else
        if(event->m_time != time) {
            printf("event->m_time != time \n");
            skp_delete_timeout(event);
            event->m_time = time;
            skp_add_timeout(event);
        } else {
            event->m_updateTime = skp_get_system_time_ms();
        }
#endif
    }
}

void SkpEventPrivate::skp_add_timeout(skp_epoll_data *event)
{
    SKP_ASSERT(event);
    SKP_ASSERT(!event->m_rbNode);
    skp_insert_timeout(event, event->m_time);
}

void SkpEventPrivate::skp_sub_timeout(skp_epoll_data *event)
{
    SKP_ASSERT(event);
    SKP_ASSERT(event->m_rbNode);
    skp_delete_timeout(event);
}

void SkpEventPrivate::skp_insert_timeout(skp_epoll_data *event, int64 time)
{
    if(event->m_time > 0 && !event->m_rbNode) {
        event->m_updateTime = skp_get_system_time_ms();
        int64 msec = skp_rbtree_time(event, time);

        util_rbtree_node_t *rbNode = m_base.m_rbtree.insert(msec, event);
        event->m_rbNode = rbNode;
    }
}

void SkpEventPrivate::skp_delete_timeout(skp_epoll_data *event)
{
    if(event->m_rbNode) {
        m_base.m_rbtree.remove(event->m_rbNode);
        event->m_rbNode = NULL;
    }
}

int64 SkpEventPrivate::skp_rbtree_time(skp_epoll_data *event, int64 time)
{
    SKP_UNUSED(event);
    int64 msec = time + skp_get_system_time_ms();

    return msec;
}

bool SkpEventPrivate::skp_is_delete_timeout(skp_epoll_data *event)
{
    if(event->m_rbNode) {
        int64 msec = skp_rbtree_time(event, event->m_time);
        if(msec != event->m_rbNode->key)
            return skp_true;
    }

    return skp_false;
}

void SkpEventPrivate::skp_change_node(skp_epoll_data *event, skp::epoll_state state)
{
    skp_epoll_node *epollNode = event->m_node;
    if(!epollNode) {
        SKP_ASSERT(skp_false);;
    }

    skp_add_change_node(epollNode);

    event->m_state = state;
}

skp_epoll_data *SkpEventPrivate::skp_malloc_data()
{
    skp_epoll_data *data = (skp_epoll_data *)skp_pool_calloc(m_base.pool, sizeof(skp_epoll_data));
    return data;
}

skp_epoll_node *SkpEventPrivate::skp_malloc_node()
{
    skp_epoll_node *node = (skp_epoll_node *)skp_pool_calloc(m_base.pool, sizeof(skp_epoll_node));
    return node;
}

skp_epoll_node *SkpEventPrivate::skp_malloc_node(int fd)
{
    if (NODE_TYPE == NODE_TYPE_MAP) {
        skp_epoll_node *epollNode = NULL;
        auto iter = m_base.m_nodeMap.find(fd);
        if(iter != m_base.m_nodeMap.end()) {
            epollNode = iter->second;
        } else {
            epollNode = skp_malloc_node();
            m_base.m_nodeMap.insert(std::pair<int, skp_epoll_node*>(fd, epollNode));
        }

        return epollNode;
    }

    int size = m_base.m_nodeSize;

    if(m_base.m_nodeSize == 0) {
        if (NODE_TYPE == NODE_TYPE_NODE) {
            m_base.m_nodeSize = 1000;
        } else if (NODE_TYPE == NODE_TYPE_MORE) {
            m_base.m_nodeSize = 10000;
        } else {
            SKP_ASSERT(skp_false);
            return skp_null;
        }
    }

    if(fd >= m_base.m_nodeSize) {
        while(fd >= m_base.m_nodeSize)
            m_base.m_nodeSize = (m_base.m_nodeSize * 2);
    }

    if(m_base.m_nodeSize > size) {
        skp_epoll_node **nodeList = (skp_epoll_node **)skp_pool_calloc(m_base.pool, sizeof(skp_epoll_node *) * m_base.m_nodeSize);
        if(size > 0) {
            memcpy(nodeList, m_base.m_nodeList, sizeof(skp_epoll_node *) * size);

            skp_pool_free(m_base.pool, m_base.m_nodeList);
        }

        m_base.m_nodeList = nodeList;

        skp_epoll_node *node = (skp_epoll_node *)skp_pool_calloc(m_base.pool, sizeof(skp_epoll_node) * (m_base.m_nodeSize - size));

        int lenth = m_base.m_nodeSize - size;
        for(int i = 0; i < lenth; i++) {
            m_base.m_nodeList[i + size] = &node[i];
        }
    }

    return m_base.m_nodeList[fd];
}

void SkpEventPrivate::skp_free_data(skp_epoll_data *data)
{
    if(data) {
        skp_pool_free(m_base.pool, data);
    }
}

void SkpEventPrivate::skp_free_node(skp_epoll_node *node)
{
    if(node) {
        skp_pool_free(m_base.pool, node);
    }
}

void SkpEventPrivate::skp_reset_node(skp_epoll_node *node)
{
    memset(node, 0x00, sizeof(skp_epoll_node));
    node->m_read.m_node = node;
    node->m_write.m_node = node;
}

bool SkpEventPrivate::skp_add_change_node(skp_epoll_node *node)
{
    SKP_ASSERT(node);
    if(!node->m_isChange) {
        node->m_isChange = skp_true;
        m_base.m_nodeChangeList->push_back(node, &node->m_nodeChange);
        return skp_true;
    } else {
        if(IS_CHECK_ERROR) {
            m_base.m_nodeChangeList->begin();
            void *data = NULL;
            while((data = m_base.m_nodeChangeList->data())) {
                skp_epoll_node *tempNode = (skp_epoll_node *)data;
                if(tempNode == node)
                    return skp_false;
                m_base.m_nodeChangeList->next();
            }

            SKP_ASSERT(skp_false);
        }
    }

    return skp_false;
}

skp_epoll_node *SkpEventPrivate::skp_remove_change_node()
{
    if(m_base.m_nodeChangeList && !m_base.m_nodeChangeList->isEmpty()) {
        skp_epoll_node *node = (skp_epoll_node *)m_base.m_nodeChangeList->take_pop();
        node->m_isChange = !node->m_isChange;
        return node;
    }
    return NULL;
}

void SkpEventPrivate::skp_add_ready_node(skp_epoll_node *node)
{
    SKP_ASSERT(node);
    if(!node->m_isReady) {
        node->m_isReady = skp_true;
        m_base.m_nodeReadyList->push_pop(node, &node->m_nodeReady);
    } else {
        if(IS_CHECK_ERROR) {
            m_base.m_nodeReadyList->begin();
            void *data = NULL;
            while((data = m_base.m_nodeReadyList->data())) {
                skp_epoll_node *tempNode = (skp_epoll_node *)data;
                if(tempNode == node)
                    return;
                m_base.m_nodeReadyList->next();
            }

            SKP_ASSERT(skp_false);
        }
    }
}

void SkpEventPrivate::skp_remove_ready_node(skp_epoll_node *node)
{
    SKP_ASSERT(node);
    if(node->m_isReady) {
        if(IS_CHECK_ERROR) {
            m_base.m_nodeReadyList->begin();
            void *data = NULL;
            while((data = m_base.m_nodeReadyList->data())) {
                skp_epoll_node *tempNode = (skp_epoll_node *)data;
                if(tempNode == node)
                    break;
                m_base.m_nodeReadyList->next();
            }

            if(!data) {
                SKP_ASSERT(skp_false);
            }
        }


        node->m_isReady = skp_false;
        m_base.m_nodeReadyList->remove(&node->m_nodeReady);
    }
}

void SkpEventPrivate::removeReadReady(void *data)
{
    skp_epoll_data *event = (skp_epoll_data *)data;
    if(event) {
        skp_epoll_node *node = event->m_node;
        if(node)
            node->m_read.m_ready = 0;
    }
}

void SkpEventPrivate::addReadReady(void *data)
{
    skp_epoll_data *event = (skp_epoll_data *)data;
    if(event) {
        skp_epoll_node *node = event->m_node;
        if(node) {
            skp_add_ready_node(node);
            node->m_read.m_ready = 1;
        }
    }
}

void SkpEventPrivate::skp_set_event_thread()
{
    m_isThread = skp_true;
}


///===========================SkpEvent

SkpEvent::SkpEvent(SkpEventPrivate &d) :
    SkpObjectData(d)
{
}

SkpEvent::~SkpEvent()
{

}

void SkpEvent::loop()
{
    SKP_D(SkpEvent);
    skpD->skp_event_base_loop();
}

void SkpEvent::dispatch()
{
    SKP_D(SkpEvent);
    skpD->skp_event_base_dispatch();
}

void *SkpEvent::timeout(int64 time, skp_callback_function func, void *arg, bool singleShot)
{
    SKP_D(SkpEvent);
    return skpD->skp_event_base_timeout(time, func, arg, singleShot);
}

void *SkpEvent::read(int fd, skp_callback_function func, void *arg, bool singleShot, int64 time, bool isET)
{
    SKP_D(SkpEvent);
    return skpD->skp_event_base_read(fd, func, arg, singleShot, time, isET);
}

void *  SkpEvent::write(int fd, skp_callback_function func, void *arg, bool singleShot, int64 time, bool isET)
{
    SKP_D(SkpEvent);
    return skpD->skp_event_base_write(fd, func, arg, singleShot, time, isET);
}

void SkpEvent::stop(void *data)
{
    SKP_D(SkpEvent);
    skpD->skp_event_delete(data);
}

void SkpEvent::free(void *data)
{
    SKP_D(SkpEvent);
    skpD->skp_event_free(data);
}

void SkpEvent::start(void *data, int64 time)
{
    SKP_D(SkpEvent);
    skpD->skp_event_start(data, time);
}

void SkpEvent::skp_break()
{
    SKP_D(SkpEvent);
    skpD->skp_event_break();
}

void *SkpEvent::skp_base()
{
    SKP_D(SkpEvent);
    return skpD->skp_base();
}

void SkpEvent::removeReadReady(void *data)
{
    SKP_D(SkpEvent);
    skpD->removeReadReady(data);
}

void SkpEvent::addReadReady(void *data)
{
    SKP_D(SkpEvent);
    skpD->addReadReady(data);
}

void SkpEvent::skp_set_event_thread()
{
    SKP_D(SkpEvent);
    skpD->skp_set_event_thread();
}
