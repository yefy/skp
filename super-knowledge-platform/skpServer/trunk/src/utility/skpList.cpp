#include "skpList.h"
#include "skpAutoFree.h"

SkpList::SkpList()
{
    m_head = new SkpListHead_t();
    init();
}

SkpList::~SkpList()
{
    clear();
    skp_delete(m_head);
}

void SkpList::init()
{
    m_head->size = 0;
    m_head->head = skp_null;
    m_head->tail = skp_null;
}

void SkpList::push_back(void *data, SkpListNode_t *node)
{
    SKP_ASSERT(node && data);

    node->flag = LIST_NODE_FLAG;
    node->data = data;
    node->pre = skp_null;
    node->next = skp_null;

    if(isEmpty()) {
        m_head->head = node;
        m_head->tail = node;
    } else {
        m_head->tail->next = node;
        node->pre = m_head->tail;
        m_head->tail = node;
    }

    m_head->size++;
}

void *SkpList::take_back()
{
    if(isEmpty()) {
        return skp_null;
    }

    SkpListNode_t *node = m_head->tail;
    SKP_ASSERT(node->flag == LIST_NODE_FLAG);

    void *data = node->data;
    if(m_head->size == 1) {
        init();
    } else {
        m_head->tail = m_head->tail->pre;
        m_head->tail->next = skp_null;
        m_head->size--;
    }

    return data;
}

void SkpList::push_pop(void *data, SkpListNode_t *node)
{
    SKP_ASSERT(node && data);

    node->flag = LIST_NODE_FLAG;
    node->data = data;
    node->pre = skp_null;
    node->next = skp_null;
    if(isEmpty()) {
        m_head->head = node;
        m_head->tail = node;
    } else {
        node->next = m_head->head;
        m_head->head->pre = node;
        m_head->head = node;
    }
    m_head->size++;
}

void *SkpList::take_pop()
{
    if(isEmpty()) {
        return skp_null;
    }

    SkpListNode_t *node = m_head->head;

    SKP_ASSERT(node->flag == LIST_NODE_FLAG);

    void *data = node->data;
    if(m_head->size == 1) {
        init();
    } else {
        m_head->head = m_head->head->next;
        m_head->head->pre = skp_null;
        m_head->size--;
    }

    return data;
}

void SkpList::remove(SkpListNode_t *node)
{
    if(isEmpty())
        return;

    SKP_ASSERT(node->flag == LIST_NODE_FLAG);

    if(m_head->head == node) {
        take_pop();
        return;
    }

    if(m_head->tail == node) {
        take_back();
        return;
    }

    SkpListNode_t *nodePrev = node->pre;
    SkpListNode_t *nodeNext = node->next;

    SKP_ASSERT(nodePrev->flag == LIST_NODE_FLAG);

    SKP_ASSERT(nodeNext->flag == LIST_NODE_FLAG);

    node->pre->next = node->next;
    node->next->pre = node->pre;
    m_head->size--;
}

void SkpList::clear()
{
    init();
}

bool SkpList::isEmpty()
{
    return (m_head->size == 0);
}

int SkpList::size()
{
    return m_head->size;
}

void SkpList::begin()
{
    m_data = m_head->head;
    if(m_data) {
        SKP_ASSERT(m_data->flag == LIST_NODE_FLAG);
    }
}

void *SkpList::data()
{
    if(m_data) {
        SKP_ASSERT(m_data->flag == LIST_NODE_FLAG);
        return m_data->data;
    }
    return skp_null;
}

void SkpList::next()
{
    if(m_data) {
        SKP_ASSERT(m_data->flag == LIST_NODE_FLAG);
        m_data = m_data->next;
        if(m_data) {
            SKP_ASSERT(m_data->flag == LIST_NODE_FLAG);
        }
    }
}
