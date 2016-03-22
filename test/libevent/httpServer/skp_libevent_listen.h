#ifndef SKP_LIBEVENT_LISTEN_H
#define SKP_LIBEVENT_LISTEN_H

#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <arpa/inet.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
         struct sockaddr *sock, int socklen, void *arg);

static void
accept_error_cb(struct evconnlistener *listener, void *ctx);

void socket_read_cb(struct bufferevent *bev, void *arg);
void socket_error_cb(struct bufferevent *bev, short events, void *arg);

int run_listen();

#endif // SKP_LIBEVENT_LISTEN_H
