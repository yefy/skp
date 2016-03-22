#ifndef SKP_LIBEVENT_TEST_H
#define SKP_LIBEVENT_TEST_H

/* For sockaddr_in */
#include <netinet/in.h>
/* For socket functions */
#include <sys/socket.h>
/* For fcntl */
#include <fcntl.h>

#include <event2/event.h>
#include <event2/event_struct.h>

#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define MAX_LINE 16384

struct fd_state {
    char buffer[MAX_LINE];
    size_t buffer_used;

    size_t n_written;
    size_t write_upto;

    struct event *read_event;
    struct event *write_event;
};

void do_read(evutil_socket_t fd, short events, void *arg);
void do_write(evutil_socket_t fd, short events, void *arg);

char
rot13_char(char c);

struct fd_state *
alloc_fd_state(struct event_base *base, evutil_socket_t fd);
void
free_fd_state(struct fd_state *state);
void
do_accept(evutil_socket_t listener, short event, void *arg);

void
run(void);

#endif // SKP_LIBEVENT_TEST_H
