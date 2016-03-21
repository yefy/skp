#ifndef SKPEVENT_P_H
#define SKPEVENT_P_H

#include "skpObjectData_p.h"
#include "skpEvent.h"

#include "skpRbtree.h"
#include "skpList.h"
#include "skpQueue.h"

#include <sys/epoll.h>


class SkpMallocPoolEx;

namespace skp {
enum epoll_state
{
    epoll_state_null = 0,
    epoll_state_start,
    epoll_state_stop
};

enum epoll_type
{
    epoll_type_null = 0,
    epoll_type_timeout,
    epoll_type_read,
    epoll_type_write
};
}

typedef struct skp_epoll_node_s skp_epoll_node;
typedef struct skp_epoll_data_s skp_epoll_data;
struct skp_epoll_data_s
{
    skp::epoll_type m_type;
    skp::epoll_state m_state;
    int m_fd;
    util_rbtree_node_t *m_rbNode;
    int64 m_time;
    int64 m_updateTime;
    skp_callback_function m_func;
    void *m_arg;
    bool m_active;
    bool m_ready;
    bool m_singleShot;
    bool R2;
    skp_epoll_node *m_node;
    QUEUE_NODE(skp_epoll_data) chain;
    int m_readyNumber;
    int m_callbackNumber;
};

struct skp_epoll_node_s
{
    int m_fd;
    uint m_flags;
    uint m_oldFlags;
    uint R;
    bool m_isReady;
    bool m_isChange;
    bool R3;
    bool R1;
    int m_errorNumber;
    skp_epoll_data m_read;
    skp_epoll_data m_write;
    SkpListNode_t m_nodeChange;
    SkpListNode_t m_nodeReady;
};

typedef QUEUE_HEAD(skp_epoll_data_head_s, skp_epoll_data) skp_epoll_data_head_t;


typedef struct
{
    int m_epfd;

    struct epoll_event m_events[4096];

    SkpRBTree m_rbtree;

    std::map<int, skp_epoll_node *> m_nodeMap;
    skp_epoll_node **m_nodeList;
    int m_nodeSize;

    SkpList *m_nodeChangeList;
    SkpList *m_nodeReadyList;
    SkpList *m_nodeRemoveReadyList;
    SkpMallocPoolEx *pool;
    std::map<int64, skp_epoll_data_head_t *> m_rbtreeMap;
}epoll_base;


class SkpEventPrivate : public SkpObjectDataPrivate
{
public:
  SkpEventPrivate();
  virtual ~SkpEventPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpEvent);

public:
  virtual void skp_event_base_loop();
  void skp_change_event();
  void skp_ready_callback();
  void skp_event_callback(skp_epoll_data *data);
  void skp_min_tbtree();
  void skp_event_base_dispatch();
  void *skp_event_base_timeout(int64 time, skp_callback_function func, void *arg, bool singleShot);
  void *skp_event_base_read(int fd, skp_callback_function func, void *arg, bool singleShot, int64 time = 0, bool isET = skp_false);
  void *skp_event_base_write(int fd, skp_callback_function func, void *arg, bool singleShot, int64 time = 0, bool isET = skp_false);

  void skp_event_delete(void *event);
  void skp_event_free(void *event);
  void skp_event_start(void *event, int64 time);
  void skp_event_break(){}

  void *skp_base(){ return NULL;}

  void skp_add_timeout(skp_epoll_data *event);
  void skp_sub_timeout(skp_epoll_data *event);
  void skp_insert_timeout(skp_epoll_data *event, int64 time);
  void skp_delete_timeout(skp_epoll_data *event);
  int64 skp_rbtree_time(skp_epoll_data *event, int64 time);
  bool skp_is_delete_timeout(skp_epoll_data *event);
  void skp_change_node(skp_epoll_data *event, skp::epoll_state state);



  skp_epoll_data *skp_malloc_data();
  skp_epoll_node *skp_malloc_node();
  skp_epoll_node *skp_malloc_node(int fd);

  void skp_free_data(skp_epoll_data *data);
  void skp_free_node(skp_epoll_node *node);



  void skp_reset_node(skp_epoll_node *node);
  bool skp_add_change_node(skp_epoll_node *node);
  skp_epoll_node *skp_remove_change_node();

  void skp_add_ready_node(skp_epoll_node *node);
  void skp_remove_ready_node(skp_epoll_node *node);

  void removeReadReady(void *data);
  void addReadReady(void *data);
  void skp_set_event_thread();

private:
  virtual void skp_event_base_new();
  virtual void skp_event_base_free();
private:
  epoll_base m_base;
  bool m_isThread;
};



#endif // SKPEVENT_P_H
