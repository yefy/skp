#ifndef MAIN_CPP
#define MAIN_CPP

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <evhttp.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/bufferevent.h>
#include <event2/dns.h>
void http_request_done(struct evhttp_request *req, void *arg){
    printf("req->response_code = %d \n", req->response_code);
    printf("req->major = %d, req->minor = %d \n", req->major, req->minor);
    printf("req->chunked = %d \n", req->chunked);
    struct evkeyvalq *headers = evhttp_request_get_input_headers(req);
    struct evkeyval *header;
    for (header = headers->tqh_first; header;
        header = header->next.tqe_next) {
        printf("  %s: %s\n", header->key, header->value);
    }
    int length = evbuffer_get_length(req->input_buffer);
    printf("body length = %d \n", length);
    char buf[1024];
    int s = evbuffer_remove(req->input_buffer, buf, length);
    printf("s*** = %d \n", s);
    buf[s] = '\0';
    printf("%s \n", buf);

    printf("1111111 \n");
    printf("req 111 = %#x \n", req);
    //evhttp_request_free(req);
    //evhttp_connection_free(req->evcon);
    printf("22222 \n");
    // terminate event_base_dispatch()
    //event_base_loopbreak((struct event_base *)arg);
}

void onClose(struct evhttp_connection *conn, void *arg)
{
    printf("*************onClose \n");
    event_base_loopbreak((struct event_base *)arg);
}

static void
request_chunked_cb(struct evhttp_request *req, void *arg)
{
    printf("*************request_chunked_cb \n");
}

void main_test_1()
{
    struct event_base *base;
    struct evhttp_connection *conn;
    struct evhttp_request *req;
    base = event_base_new();
    struct evdns_base * dnsbase = evdns_base_new(base, 1);
    conn = evhttp_connection_base_new(base, NULL, "222.76.151.10", 51010);

    //conn = evhttp_connection_base_new(base, NULL, "192.168.164.148", 8080);
    req = evhttp_request_new(http_request_done, base);
    printf("req 111 = %#x \n", req);
    //evhttp_request_set_chunked_cb(req, request_chunked_cb);
    evhttp_add_header(req->output_headers, "Host", "localhost");
    //evhttp_add_header(req->output_headers, "Connection", "keep-alive");
    //evhttp_add_header(req->output_headers, "Connection", "close");
    //evhttp_make_request(conn, req, EVHTTP_REQ_GET, "/favicon.ico");
    //evhttp_make_request(conn, req, EVHTTP_REQ_GET, "/a.pdf");
    evhttp_make_request(conn, req, EVHTTP_REQ_POST, "/portal/sss");
    //evhttp_connection_set_timeout(req->evcon, 3);
    evhttp_connection_set_closecb(req->evcon, onClose, base);
    evbuffer_add(req->output_buffer, "123", 3);
    event_base_dispatch(base);
    printf("3333 \n");
}

/***********************************/

/* Base on code from:
 http://archives.seul.org/libevent/users/Sep-2010/msg00050.html
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/keyvalq_struct.h>

// (default)
#define HTTP_CONTENT_TYPE_URL_ENCODED   "application/x-www-form-urlencoded"
// (use for files: picture, mp3, tar-file etc.)
#define HTTP_CONTENT_TYPE_FORM_DATA     "multipart/form-data"
// (use for plain text)
#define HTTP_CONTENT_TYPE_TEXT_PLAIN    "text/plain"

#define REQUEST_POST_FLAG               2
#define REQUEST_GET_FLAG                3

#define MITLogOpen printf
#define MITLog_DetPrintf printf
#define MITLog_DetPuts printf

struct http_request_get {
    struct evhttp_uri *uri;
    struct event_base *base;
    struct evhttp_connection *cn;
    struct evhttp_request *req;
};

struct http_request_post {
    struct evhttp_uri *uri;
    struct event_base *base;
    struct evhttp_connection *cn;
    struct evhttp_request *req;
    char *content_type;
    char *post_data;
};

/************************** Ahead Declare ******************************/
void http_requset_post_cb(struct evhttp_request *req, void *arg);
void http_requset_get_cb(struct evhttp_request *req, void *arg);
int start_url_request(struct http_request_get *http_req, int req_get_flag);

/************************** Tools Function ******************************/
static inline void print_request_head_info(struct evkeyvalq *header)
{
    struct evkeyval *first_node = header->tqh_first;
    while (first_node) {
        MITLog_DetPrintf("key:%s  value:%s \n", first_node->key, first_node->value);
        first_node = first_node->next.tqe_next;
    }
}

static inline void print_uri_parts_info(const struct evhttp_uri * http_uri)
{
    MITLog_DetPrintf("scheme:%s \n", evhttp_uri_get_scheme(http_uri));
    MITLog_DetPrintf("host:%s \n", evhttp_uri_get_host(http_uri));
    MITLog_DetPrintf("path:%s \n", evhttp_uri_get_path(http_uri));
    MITLog_DetPrintf("port:%d \n", evhttp_uri_get_port(http_uri));
    MITLog_DetPrintf("query:%s \n", evhttp_uri_get_query(http_uri));
    MITLog_DetPrintf("userinfo:%s \n", evhttp_uri_get_userinfo(http_uri));
    MITLog_DetPrintf("fragment:%s \n", evhttp_uri_get_fragment(http_uri));
}

/************************** Request Function ******************************/
void http_requset_post_cb(struct evhttp_request *req, void *arg)
{
    struct http_request_post *http_req_post = (struct http_request_post *)arg;
    switch(req->response_code)
    {
        case HTTP_OK:
        {
            struct evbuffer* buf = evhttp_request_get_input_buffer(req);
            size_t len = evbuffer_get_length(buf);
            MITLog_DetPuts("print the head info: \n");
            print_request_head_info(req->output_headers);

            MITLog_DetPrintf("len:%zu  body size:%zu \n", len, req->body_size);
            char *tmp = (char *)malloc(len+1);
            memcpy(tmp, evbuffer_pullup(buf, -1), len);
            tmp[len] = '\0';
            MITLog_DetPuts("print the body: \n");
            MITLog_DetPrintf("HTML BODY:%s \n", tmp);
            free(tmp);

            event_base_loopexit(http_req_post->base, 0);
            break;
        }
        case HTTP_MOVEPERM:
            MITLog_DetPrintf("%s", "the uri moved permanently \n");
            break;
        case HTTP_MOVETEMP:
        {
            const char *new_location = evhttp_find_header(req->input_headers, "Location");
            struct evhttp_uri *new_uri = evhttp_uri_parse(new_location);
            evhttp_uri_free(http_req_post->uri);
            http_req_post->uri = new_uri;
            start_url_request((struct http_request_get *)http_req_post, REQUEST_POST_FLAG);
            return;
        }

        default:
            event_base_loopexit(http_req_post->base, 0);
            return;
    }
}
void http_requset_get_cb(struct evhttp_request *req, void *arg)
{
    struct http_request_get *http_req_get = (struct http_request_get *)arg;
    switch(req->response_code)
    {
        case HTTP_OK:
        {
            struct evbuffer* buf = evhttp_request_get_input_buffer(req);
            size_t len = evbuffer_get_length(buf);
            MITLog_DetPuts("print the head info: \n");
            print_request_head_info(req->output_headers);

            MITLog_DetPrintf("len:%zu  body size:%zu \n", len, req->body_size);
            char *tmp = (char *)malloc(len+1);
            memcpy(tmp, evbuffer_pullup(buf, -1), len);
            tmp[len] = '\0';
            MITLog_DetPuts("print the body: \n");
            MITLog_DetPrintf("HTML BODY:%s \n", tmp);
            free(tmp);

            event_base_loopexit(http_req_get->base, 0);
            break;
        }
        case HTTP_MOVEPERM:
            MITLog_DetPrintf("%s", "the uri moved permanently \n");
            break;
        case HTTP_MOVETEMP:
        {
            const char *new_location = evhttp_find_header(req->input_headers, "Location");
            struct evhttp_uri *new_uri = evhttp_uri_parse(new_location);
            evhttp_uri_free(http_req_get->uri);
            http_req_get->uri = new_uri;
            start_url_request(http_req_get, REQUEST_GET_FLAG);
            return;
        }

        default:
            event_base_loopexit(http_req_get->base, 0);
            return;
    }
}

int start_url_request(struct http_request_get *http_req, int req_get_flag)
{
    if (http_req->cn)
        evhttp_connection_free(http_req->cn);

    int port = evhttp_uri_get_port(http_req->uri);
    http_req->cn = evhttp_connection_base_new(http_req->base,
                                                   NULL,
                                                   evhttp_uri_get_host(http_req->uri),
                                                   (port == -1 ? 80 : port));

    /**
     * Request will be released by evhttp connection
     * See info of evhttp_make_request()
     */
    if (req_get_flag == REQUEST_POST_FLAG) {
        http_req->req = evhttp_request_new(http_requset_post_cb, http_req);
    } else if (req_get_flag ==  REQUEST_GET_FLAG) {
        http_req->req = evhttp_request_new(http_requset_get_cb, http_req);
    }

    if (req_get_flag == REQUEST_POST_FLAG) {
        const char *path = evhttp_uri_get_path(http_req->uri);
        evhttp_make_request(http_req->cn, http_req->req, EVHTTP_REQ_POST,
                            path ? path : "/");
        /** Set the post data */
        struct http_request_post *http_req_post = (struct http_request_post *)http_req;
        evbuffer_add(http_req_post->req->output_buffer, http_req_post->post_data, strlen(http_req_post->post_data));
        evhttp_add_header(http_req_post->req->output_headers, "Content-Type", http_req_post->content_type);
    } else if (req_get_flag == REQUEST_GET_FLAG) {
        const char *query = evhttp_uri_get_query(http_req->uri);
        const char *path = evhttp_uri_get_path(http_req->uri);
        size_t len = (query ? strlen(query) : 0) + (path ? strlen(path) : 0) + 1;
        char *path_query = NULL;
        if (len > 1) {
            path_query = (char *)calloc(len, sizeof(char));
            sprintf(path_query, "%s?%s", path, query);
        }
        evhttp_make_request(http_req->cn, http_req->req, EVHTTP_REQ_GET,
                             path_query ? path_query: "/");
    }
    /** Set the header properties */
    evhttp_add_header(http_req->req->output_headers, "Host", evhttp_uri_get_host(http_req->uri));

    return 0;
}

/************************** New/Free Function ******************************/
/**
 * @param get_flag: refer REQUEST_GET_*
 *
 */
void *http_request_new(struct event_base* base, const char *url, int req_get_flag, \
                       const char *content_type, const char* data)
{
    int len = 0;
    if (req_get_flag == REQUEST_GET_FLAG) {
        len = sizeof(struct http_request_get);
    } else if(req_get_flag == REQUEST_POST_FLAG) {
        len = sizeof(struct http_request_post);
    }

    struct http_request_get *http_req_get = (struct http_request_get *)calloc(1, len);
    http_req_get->uri = evhttp_uri_parse(url);
    print_uri_parts_info(http_req_get->uri);

    http_req_get->base = base;

    if (req_get_flag == REQUEST_POST_FLAG) {
        struct http_request_post *http_req_post = (struct http_request_post *)http_req_get;
        if (content_type == NULL) {
            content_type = HTTP_CONTENT_TYPE_URL_ENCODED;
        }
        http_req_post->content_type = strdup(content_type);

        if (data == NULL) {
            http_req_post->post_data = NULL;
        } else {
            http_req_post->post_data = strdup(data);
        }
    }

    return http_req_get;
}

void http_request_free(struct http_request_get *http_req_get, int req_get_flag)
{
    evhttp_connection_free(http_req_get->cn);
    evhttp_uri_free(http_req_get->uri);
    if (req_get_flag == REQUEST_GET_FLAG) {
        free(http_req_get);
    } else if(req_get_flag == REQUEST_POST_FLAG) {
        struct http_request_post *http_req_post = (struct http_request_post*)http_req_get;
        if (http_req_post->content_type) {
            free(http_req_post->content_type);
        }
        if (http_req_post->post_data) {
            free(http_req_post->post_data);
        }
        free(http_req_post);
    }
    http_req_get = NULL;
}

/************************** Start POST/GET Function ******************************/
/**
 * @param content_type: refer HTTP_CONTENT_TYPE_*
 */
void *start_http_requset(struct event_base* base, const char *url, int req_get_flag, \
                                                const char *content_type, const char* data)
{
    struct http_request_get *http_req_get = (struct http_request_get *)http_request_new(base, url, req_get_flag, content_type, data);
    start_url_request(http_req_get, req_get_flag);

    return http_req_get;
}


int main_test_2()
{

    struct event_base* base = event_base_new();

    struct http_request_post *http_req_post = (struct http_request_post *)start_http_requset(base,
                                                                 "http://172.16.239.93:8899/base/truck/delete",
                                                                 REQUEST_POST_FLAG,
                                                                 HTTP_CONTENT_TYPE_URL_ENCODED,
                                                                 "name=winlin&code=1234");
    struct http_request_get *http_req_get = (struct http_request_get *)start_http_requset(base, \
                                                               "http://127.0.0.1?name=winlin", \
                                                               REQUEST_GET_FLAG, \
                                                               NULL, NULL);

    event_base_dispatch(base);

    http_request_free((struct http_request_get *)http_req_post, REQUEST_POST_FLAG);
    http_request_free(http_req_get, REQUEST_GET_FLAG);
    event_base_free(base);

    return 0;
}


#include <stdio.h>
#include <event2/util.h>
#include <event2/event.h>

#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int n_pending_requests = 0;
struct event_base *base = NULL;

struct user_data
{
    char *name;
    int idx;
};

void call_back(int errcode, struct evutil_addrinfo *addr, void *ptr)
{
    struct user_data *data = (struct user_data *)ptr;
    const char *name = data->name;
    if(errcode)
    {
        printf("%d.%s->%s\n", data->idx, name, evutil_gai_strerror(errcode));
    }
    else
    {
        struct evutil_addrinfo *ai;
        printf("*******%d.%s", data->idx, name);
        if(addr->ai_canonname)
            printf("[%s]", addr->ai_canonname);
        puts("");
        for(ai = addr; ai; ai=ai->ai_next)
        {
            char buf[128];
            const char *s = NULL;
            if(ai->ai_family == AF_INET)
            {
                struct sockaddr_in *sin = (struct sockaddr_in*)ai->ai_addr;
                s = evutil_inet_ntop(AF_INET, &sin->sin_addr, buf, 128);
            }
            else if(ai->ai_family == AF_INET6)
            {
                struct sockaddr_in6 *sin6 = (struct sockaddr_in6*)ai->ai_addr;
                s = evutil_inet_ntop(AF_INET6, &sin6->sin6_addr, buf, 128);
            }
            if(s)
            {
                printf("  ---->%s\n", s);
            }
        }
        evutil_freeaddrinfo(addr);
    }
    free(data->name);
    free(data);
    if(--n_pending_requests == 0)
        event_base_loopexit(base, NULL);
}

int main_test_3()
{
    char *argv[2] = {"www.baidu.com", "www.sina.com.cn"};
    int i;
    struct evdns_base *dnsbase;

    base = event_base_new();
    if(!base)
        return 1;
    dnsbase = evdns_base_new(base, 1);
    if(!dnsbase)
        return 2;
    for(i=0;i<2;++i)
    {
        struct evutil_addrinfo hints;
        struct evdns_getaddrinfo_request *req;
        struct user_data *user_data;
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_flags = EVUTIL_AI_CANONNAME;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        if(!(user_data = (struct user_data *)malloc(sizeof user_data)))
        {
            perror("malloc");
            exit(1);
        }
        if(!(user_data->name = strdup(argv[i])))
        {
            perror("strdup");
            exit(1);

        }
        user_data->idx = i;
        ++n_pending_requests;
        req = evdns_getaddrinfo(dnsbase, argv[i], NULL, &hints, call_back, user_data);
        if(req == NULL)
            printf("[request for %s returned immediately]\n", argv[i]);
    }
    if(n_pending_requests)
    {
        event_base_dispatch(base);
    }
    evdns_base_free(dnsbase, 0);
    event_base_free(base);
    return 0;
}

#include "httpclient_ypl.h"

int main(int argc, char **argv){
    //main_test_1();
    //main_test_2();
    //main_test_3();
    HttpClient_YPL httpClient;
    httpClient.post();
    return 0;
}

#endif // MAIN_CPP
