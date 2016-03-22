#include "skp_libevent_test.h"
#include "time.h"
#include "stdio.h"


char
rot13_char(char c)
{
    /* We don't want to use isalpha here; setting the locale would change
     * which characters are considered alphabetical. */
    if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
        return c + 13;
    else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
        return c - 13;
    else
        return c;
}



struct fd_state *
alloc_fd_state(struct event_base *base, evutil_socket_t fd)
{
    struct fd_state *state = (struct fd_state *)malloc(sizeof(struct fd_state));
    if (!state)
        return NULL;
    state->read_event = event_new(base, fd, EV_READ|EV_PERSIST|EV_ET, do_read, state);
    if (!state->read_event) {
        free(state);
        return NULL;
    }
    //state->write_event = event_new(base, fd, EV_WRITE|EV_PERSIST|EV_ET, do_write, state);

//    if (!state->write_event) {
//        event_free(state->read_event);
//        free(state);
//        return NULL;
//    }

    state->buffer_used = state->n_written = state->write_upto = 0;

   // assert(state->write_event);
    return state;
}

void
free_fd_state(struct fd_state *state)
{
    event_free(state->read_event);
    event_free(state->write_event);
    free(state);
}

void
do_read(evutil_socket_t fd, short events, void *arg)
{

    unsigned int currentTime = time(NULL);
    printf("currentTime = %d, do_read Got an event on socket %d:%s%s%s%s\n",
            currentTime,
            (int) fd,
            (events&EV_TIMEOUT) ? " timeout" : "",
            (events&EV_READ)    ? " read" : "",
            (events&EV_WRITE)   ? " write" : "",
            (events&EV_SIGNAL)  ? " signal" : "");


    struct fd_state *state = (struct fd_state *)arg;
    char buf[10];
    int i;
    ssize_t result;
    //while (1) {
        //assert(state->write_event);
        result = recv(fd, buf, 2, 0);
        printf("read result = %d\n", result);
        if (result <= 0) {

            //break;
        }

      /*

//        if(result < 5) {
//            //event_add(state->write_event, NULL);
//        }
        for (i=0; i < result; ++i)  {
            if (state->buffer_used < sizeof(state->buffer))
                state->buffer[state->buffer_used++] = rot13_char(buf[i]);
            //if (buf[i] == '\n') {
                assert(state->write_event);
                struct timeval tv;
                tv.tv_sec = 3;
                tv.tv_usec = 0;
                event_add(state->write_event, NULL);
                state->write_upto = state->buffer_used;
            //}
        }
    //}

    if (result == 0) {
        free_fd_state(state);
    } else if (result < 0) {
        if (errno == EAGAIN) // XXXX use evutil macro
            return;
        perror("recv");
        free_fd_state(state);
    }
    */
}

void
do_write(evutil_socket_t fd, short events, void *arg)
{
    unsigned int currentTime = time(NULL);
    printf("currentTime = %d, do_write Got an event on socket %d:%s%s%s%s\n",
            currentTime,
            (int) fd,
            (events&EV_TIMEOUT) ? " timeout" : "",
            (events&EV_READ)    ? " read" : "",
            (events&EV_WRITE)   ? " write" : "",
            (events&EV_SIGNAL)  ? " signal" : "");

    struct fd_state *state = (struct fd_state *)arg;

    while (state->n_written < state->write_upto) {
        ssize_t result = send(fd, state->buffer + state->n_written,
                              state->write_upto - state->n_written, 0);
        printf("write result = %d\n", result);
        if (result < 0) {
            if (errno == EAGAIN) // XXX use evutil macro
                return;
            free_fd_state(state);
            return;
        }
        assert(result != 0);

        state->n_written += result;
    }

    if (state->n_written == state->buffer_used)
        state->n_written = state->write_upto = state->buffer_used = 1;

    event_del(state->write_event);
}

void
do_accept(evutil_socket_t listener, short event, void *arg)
{

    unsigned int currentTime = time(NULL);
    printf("currentTime = %d, do_accept Got an event on socket %d:%s%s%s%s\n",
            currentTime,
            (int) listener,
            (event&EV_TIMEOUT) ? " timeout" : "",
            (event&EV_READ)    ? " read" : "",
            (event&EV_WRITE)   ? " write" : "",
            (event&EV_SIGNAL)  ? " signal" : "");


    struct event_base *base = (struct event_base *)arg;
    struct sockaddr_storage ss;
    socklen_t slen = sizeof(ss);
    int fd = accept(listener, (struct sockaddr*)&ss, &slen);
    printf("accept fd = %d\n", fd);
    if (fd < 0) { // XXXX eagain??
        //perror("accept");
    } else if (fd > FD_SETSIZE) {
        close(fd); // XXX replace all closes with EVUTIL_CLOSESOCKET */
    } else {
        struct fd_state *state;
        evutil_make_socket_nonblocking(fd);
        state = alloc_fd_state(base, fd);
        assert(state);
        //assert(state->write_event);
        struct timeval tv;
        tv.tv_sec = 3;
        tv.tv_usec = 0;
        event_add(state->read_event, NULL);
    }
}

void
run(void)
{
    /*
     *#define EV_TIMEOUT	0x01
     *#define EV_READ		0x02
     *#define EV_WRITE	0x04
     *#define EV_SIGNAL	0x08
     *#define EV_PERSIST	0x10
     *#define EV_ET       0x20
    */

    int flags = EV_READ| EV_WRITE;

    if(flags& (EV_READ| EV_WRITE| EV_SIGNAL)) {
        printf("111\n");
    }

    flags = EVLIST_INSERTED|EVLIST_ACTIVE;
    if(!(flags & (EVLIST_INSERTED|EVLIST_ACTIVE))) {
        printf("222\n");
    }

    flags = EVLIST_INSERTED|EVLIST_ACTIVE;
    if(flags & EVLIST_ALL) {
        printf("333\n");
    }

    if(flags & ~EVLIST_ALL) {
        printf("444\n");
    }

    if(!(flags & ~EVLIST_ALL)) {
        printf("555\n");
    }

    //return;




    evutil_socket_t listener;
    struct sockaddr_in sin;
    struct event_base *base;
    struct event *listener_event;

    //base = event_base_new();

    struct event_config *config = event_config_new();
    event_config_require_features(config, EV_FEATURE_ET);
    event_config_set_flag(config, EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST);
    base = event_base_new_with_config(config);
    event_config_free(config);
    if (!base)
        return; /*XXXerr*/

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(19555);

    listener = socket(AF_INET, SOCK_STREAM, 0);
    evutil_make_socket_nonblocking(listener);

#ifndef WIN32
    {
        int one = 1;
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    }
#endif

    if (bind(listener, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        perror("bind");
        return;
    }

    if (listen(listener, 16)<0) {
        perror("listen");
        return;
    }


    listener_event = event_new(base, listener, EV_READ|EV_PERSIST, do_accept, (void*)base);


    /*XXX check it */
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    event_add(listener_event, &tv);


    //struct event *listener_eventxxx = event_new(base, listener, EV_READ|EV_PERSIST, do_accept, (void*)base);
    //event_add(listener_eventxxx, &tv);

    //event_del(listener_event);


    while(1) {

//        struct timeval tv;
//        evutil_timerclear(&tv);
//        tv.tv_sec = 0;
//        tv.tv_usec = 1000 * 500;
//        event_base_loopexit(base, &tv);
        event_base_loop(base, EVLOOP_NONBLOCK | EVLOOP_ONCE);
    }
}
