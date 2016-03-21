#ifndef SKPLIST_H
#define SKPLIST_H

#include "skpUtility.h"

typedef struct SkpListHead_s SkpListHead_t;
typedef struct SkpListNode_s SkpListNode_t;

struct SkpListHead_s
{
    int size;
    int R1;
    SkpListNode_t *head;
    SkpListNode_t *tail;
};

struct SkpListNode_s
{
#define LIST_NODE_FLAG  0xAACCBBDD
    uint flag;
    void *data;
    SkpListNode_t *pre;
    SkpListNode_t *next;
};

class SkpList
{
public:
    SkpList();
    virtual ~SkpList();
    void init();
    void push_back(void *data, SkpListNode_t *node);
    void *take_back();
    void push_pop(void *data, SkpListNode_t *node);
    void *take_pop();
    void remove(SkpListNode_t *node);
    void clear();
    bool isEmpty();
    int size();
    void begin();
    void *data();
    void next();
private:
    SkpListHead_t *m_head;
    SkpListNode_t *m_data;
};

#endif // SKPLIST_H
