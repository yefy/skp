#include "skp_libevent_buffer_test.h"

void do_write_buffer(evutil_socket_t fd, short events, void *arg)
{
    printf("do_write\n");
}

char rot13_char_buffer(char c)
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

void
readcb_buffer(struct bufferevent *bev, void *ctx)
{
    struct evbuffer *input, *output;
    char *line;
    size_t n;
    int i;
    input = bufferevent_get_input(bev);
    output = bufferevent_get_output(bev);

    while ((line = evbuffer_readln(input, &n, EVBUFFER_EOL_LF))) {
        for (i = 0; i < n; ++i)
            line[i] = rot13_char(line[i]);
        evbuffer_add(output, line, n);
        evbuffer_add(output, "\n", 1);
        free(line);
    }

    if (evbuffer_get_length(input) >= MAX_LINE) {
        /* Too long; just process what there is and go on so that the buffer
         * doesn't grow infinitely long. */
        char buf[1024];
        while (evbuffer_get_length(input)) {
            int n = evbuffer_remove(input, buf, sizeof(buf));
            for (i = 0; i < n; ++i)
                buf[i] = rot13_char(buf[i]);
            evbuffer_add(output, buf, n);
        }
        evbuffer_add(output, "\n", 1);
    }
}

void
errorcb_buffer(struct bufferevent *bev, short error, void *ctx)
{
    if (error & BEV_EVENT_EOF) {
        /* connection has been closed, do any clean up here */
        /* ... */
    } else if (error & BEV_EVENT_ERROR) {
        /* check errno to see what error occurred */
        /* ... */
    } else if (error & BEV_EVENT_TIMEOUT) {
        /* must be a timeout event handle, handle it */
        /* ... */
    }
    bufferevent_free(bev);
}

void
do_accept_buffer(evutil_socket_t listener, short event, void *arg)
{
    struct event_base *base = arg;
    struct sockaddr_storage ss;
    socklen_t slen = sizeof(ss);
    int fd = accept(listener, (struct sockaddr*)&ss, &slen);
    printf("accept fd = %d\n", fd);
    if (fd < 0) {
        perror("accept");
    } else if (fd > FD_SETSIZE) {
        close(fd);
    } else {
        struct bufferevent *bev;
        evutil_make_socket_nonblocking(fd);
        bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
        bufferevent_setcb(bev, readcb_buffer, NULL, errorcb_buffer, NULL);
        bufferevent_setwatermark(bev, EV_READ, 0, MAX_LINE);
        bufferevent_enable(bev, EV_READ|EV_WRITE);
    }
}

void
run_buffer(void)
{
    suppress_logging();
    event_set_mem_functions(replacement_malloc,
                                replacement_realloc,
                                replacement_free);
    evutil_socket_t listener;
    struct sockaddr_in sin;
    struct event_base *base;
    struct event *listener_event;

    base = event_base_new();
    if (!base)
        return; /*XXXerr*/

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(40713);

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

    listener_event = event_new(base, listener, EV_READ|EV_PERSIST, do_accept_buffer, (void*)base);
    /*XXX check it */
    event_add(listener_event, NULL);

    event_base_dispatch(base);
}

static void discard_cb(int severity, const char *msg)
{
    /* This callback does nothing */
    char *s = NULL;
    switch(severity){
        case _EVENT_LOG_DEBUG:  s = "debug";    break;
        case _EVENT_LOG_MSG:    s = "msg";      break;
        case _EVENT_LOG_WARN:   s = "warn";     break;
        case _EVENT_LOG_ERR:    s = "err";      break;
        default:                s = "?";        break;  /* never reached */
    }
    printf("my log : [%s] %s\n", s, msg);

}

static void write_to_file_cb(int severity, const char *msg)
{
    const char *s;
    if(!logfile){
        return;
    }

    switch(severity){
        case _EVENT_LOG_DEBUG:  s = "debug";    break;
        case _EVENT_LOG_MSG:    s = "msg";      break;
        case _EVENT_LOG_WARN:   s = "warn";     break;
        case _EVENT_LOG_ERR:    s = "err";      break;
        default:                s = "?";        break;  /* never reached */
    }
    fprintf(logfile, "my log : [%s] %s\n", s, msg);
}

/* Turn off all logging from Libevent */
void suppress_logging(void)
{
    event_set_log_callback(discard_cb);
}

/* Recirect all Libevent log messages to the C stdio file 'f'. */
void set_logfile(FILE *f)
{
    logfile = f;
    event_set_log_callback(write_to_file_cb);
}


union alignment {
    size_t sz;
    void *ptr;
    double dbl;
};

#define ALIGNMENT sizeof(union alignment)

#define OUTPTR(ptr) (((char *) ptr) + ALIGNMENT)
#define INPTR(ptr) (((char *) ptr) - ALIGNMENT)

static size_t total_allocated = 0;
static void *replacement_malloc(size_t sz)
{
    printf("my memory replacement_malloc\n");
    void *chunk = malloc(sz + ALIGNMENT);
    if(!chunk){
        return chunk;
    }

    total_allocated += sz;
    *(size_t *)chunk = sz;
    return OUTPTR(chunk);
}

static void *replacement_realloc(void *ptr, size_t sz)
{
    printf("my memory replacement_realloc\n");
    size_t old_size = 0;
    if(ptr) {
        ptr = INPTR(ptr);
        old_size = *(size_t *)ptr;
    }

    ptr = realloc(ptr, sz + ALIGNMENT);
    if(!ptr){
        return NULL;
    }

    *(size_t *)ptr = sz;
    total_allocated = total_allocated - old_size + sz;
    return OUTPTR(ptr);
}

static void *replacement_free(void *ptr)
{
    printf("my memory replacement_free\n");
    ptr = INPTR(ptr);
    total_allocated -=*(size_t *) ptr;
    free(ptr);
}
