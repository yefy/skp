LIBEVENT_CURR_PATH = $${PWD}
INCLUDEPATH +=  \
            $${LIBEVENT_CURR_PATH} \
            $${LIBEVENT_CURR_PATH}/include \
            $${LIBEVENT_CURR_PATH}/compat

DEPENDPATH  +=  \
            $${LIBEVENT_CURR_PATH}

SOURCES += \
    $${LIBEVENT_CURR_PATH}/buffer.c \
    $${LIBEVENT_CURR_PATH}/bufferevent.c \
    $${LIBEVENT_CURR_PATH}/bufferevent_filter.c \
    $${LIBEVENT_CURR_PATH}/bufferevent_pair.c \
    $${LIBEVENT_CURR_PATH}/bufferevent_ratelim.c \
    $${LIBEVENT_CURR_PATH}/bufferevent_sock.c \
    $${LIBEVENT_CURR_PATH}/epoll.c \
    $${LIBEVENT_CURR_PATH}/evdns.c \
    $${LIBEVENT_CURR_PATH}/event.c \
    $${LIBEVENT_CURR_PATH}/event_tagging.c \
    $${LIBEVENT_CURR_PATH}/evmap.c \
    $${LIBEVENT_CURR_PATH}/evrpc.c \
    $${LIBEVENT_CURR_PATH}/evthread.c \
    $${LIBEVENT_CURR_PATH}/evthread_pthread.c \
    $${LIBEVENT_CURR_PATH}/evutil.c \
    $${LIBEVENT_CURR_PATH}/evutil_rand.c \
    $${LIBEVENT_CURR_PATH}/http.c \
    $${LIBEVENT_CURR_PATH}/listener.c \
    $${LIBEVENT_CURR_PATH}/log.c \
    $${LIBEVENT_CURR_PATH}/poll.c \
    $${LIBEVENT_CURR_PATH}/select.c \
    $${LIBEVENT_CURR_PATH}/signal.c \
    $${LIBEVENT_CURR_PATH}/strlcpy.c \
    $${LIBEVENT_CURR_PATH}/include/arc4random.c

HEADERS += \
    $${LIBEVENT_CURR_PATH}/bufferevent-internal.h \
    $${LIBEVENT_CURR_PATH}/defer-internal.h \
    $${LIBEVENT_CURR_PATH}/evbuffer-internal.h \
    $${LIBEVENT_CURR_PATH}/evthread-internal.h \
    $${LIBEVENT_CURR_PATH}/ipv6-internal.h \
    $${LIBEVENT_CURR_PATH}/log-internal.h \
    $${LIBEVENT_CURR_PATH}/mm-internal.h \
    $${LIBEVENT_CURR_PATH}/ratelim-internal.h \
    $${LIBEVENT_CURR_PATH}/util-internal.h \
    $${LIBEVENT_CURR_PATH}/compat/sys/queue.h \
    $${LIBEVENT_CURR_PATH}/include/changelist-internal.h \
    $${LIBEVENT_CURR_PATH}/include/evdns.h \
    $${LIBEVENT_CURR_PATH}/include/event.h \
    $${LIBEVENT_CURR_PATH}/include/event-internal.h \
    $${LIBEVENT_CURR_PATH}/include/evhttp.h \
    $${LIBEVENT_CURR_PATH}/include/evmap-internal.h \
    $${LIBEVENT_CURR_PATH}/include/evrpc.h \
    $${LIBEVENT_CURR_PATH}/include/evrpc-internal.h \
    $${LIBEVENT_CURR_PATH}/include/evsignal-internal.h \
    $${LIBEVENT_CURR_PATH}/include/evutil.h \
    $${LIBEVENT_CURR_PATH}/include/ht-internal.h \
    $${LIBEVENT_CURR_PATH}/include/http-internal.h \
    $${LIBEVENT_CURR_PATH}/include/iocp-internal.h \
    $${LIBEVENT_CURR_PATH}/include/minheap-internal.h \
    $${LIBEVENT_CURR_PATH}/include/strlcpy-internal.h \
    $${LIBEVENT_CURR_PATH}/include/event2/buffer.h \
    $${LIBEVENT_CURR_PATH}/include/event2/buffer_compat.h \
    $${LIBEVENT_CURR_PATH}/include/event2/bufferevent.h \
    $${LIBEVENT_CURR_PATH}/include/event2/bufferevent_compat.h \
    $${LIBEVENT_CURR_PATH}/include/event2/bufferevent_ssl.h \
    $${LIBEVENT_CURR_PATH}/include/event2/bufferevent_struct.h \
    $${LIBEVENT_CURR_PATH}/include/event2/dns.h \
    $${LIBEVENT_CURR_PATH}/include/event2/dns_compat.h \
    $${LIBEVENT_CURR_PATH}/include/event2/dns_struct.h \
    $${LIBEVENT_CURR_PATH}/include/event2/event.h \
    $${LIBEVENT_CURR_PATH}/include/event2/event_compat.h \
    $${LIBEVENT_CURR_PATH}/include/event2/event-config.h \
    $${LIBEVENT_CURR_PATH}/include/event2/event_struct.h \
    $${LIBEVENT_CURR_PATH}/include/event2/http.h \
    $${LIBEVENT_CURR_PATH}/include/event2/http_compat.h \
    $${LIBEVENT_CURR_PATH}/include/event2/http_struct.h \
    $${LIBEVENT_CURR_PATH}/include/event2/keyvalq_struct.h \
    $${LIBEVENT_CURR_PATH}/include/event2/listener.h \
    $${LIBEVENT_CURR_PATH}/include/event2/rpc.h \
    $${LIBEVENT_CURR_PATH}/include/event2/rpc_compat.h \
    $${LIBEVENT_CURR_PATH}/include/event2/rpc_struct.h \
    $${LIBEVENT_CURR_PATH}/include/event2/tag.h \
    $${LIBEVENT_CURR_PATH}/include/event2/tag_compat.h \
    $${LIBEVENT_CURR_PATH}/include/event2/thread.h \
    $${LIBEVENT_CURR_PATH}/include/event2/util.h
