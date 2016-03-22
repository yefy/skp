#ifndef SKP_LIBEVENT_BUFFER_TEST_H
#define SKP_LIBEVENT_BUFFER_TEST_H

/* For sockaddr_in */
#include <netinet/in.h>
/* For socket functions */
#include <sys/socket.h>
/* For fcntl */
#include <fcntl.h>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define MAX_LINE 16384

void do_read_buffer(evutil_socket_t fd, short events, void *arg);
void do_write_buffer(evutil_socket_t fd, short events, void *arg);

char rot13_char_buffer(char c);

void readcb_buffer(struct bufferevent *bev, void *ctx);

void errorcb_buffer(struct bufferevent *bev, short error, void *ctx);

void do_accept_buffer(evutil_socket_t listener, short event, void *arg);

void run_buffer(void);

static void discard_cb(int severity, const char *msg);

static FILE *logfile = NULL;

static void write_to_file_cb(int severity, const char *msg);
/* Turn off all logging from Libevent */
void suppress_logging(void);

/* Recirect all Libevent log messages to the C stdio file 'f'. */
void set_logfile(FILE *f);


static void *replacement_malloc(size_t sz);

static void *replacement_realloc(void *ptr, size_t sz);

static void *replacement_free(void *ptr);


#endif // SKP_LIBEVENT_BUFFER_TEST_H
