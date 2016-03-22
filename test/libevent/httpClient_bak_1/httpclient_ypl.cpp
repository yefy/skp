#include "httpclient_ypl.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <evhttp.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/bufferevent.h>
#include <event2/dns.h>

HttpClient_YPL::HttpClient_YPL()
{
    m_base = event_base_new();
    m_dnsbase = evdns_base_new(m_base, 1);
    //URL=http\://14.23.90.103/api/service
    m_host = "14.23.90.103";
    m_port = 80;
    //m_host = "222.76.151.10";
    //m_port = 51010;
    m_conn = evhttp_connection_base_new(m_base, m_dnsbase, m_host.c_str(), m_port);
    m_buffer = NULL;
    m_bufferSize = 0;
}

void HttpClient_YPL::onRequestDone(struct evhttp_request *req, void *arg)
{
    HttpClient_YPL *self = (HttpClient_YPL *)arg;
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

    if(!self->m_buffer)
    {
        self->m_buffer = (char *)malloc(length);
        if(!self->m_buffer)
            return;
        self->m_bufferSize = length;
    }
    else
    {
        if(self->m_bufferSize < length)
        {
            self->m_buffer = (char *)realloc(self->m_buffer, length);
            if(!self->m_buffer)
                return;
            self->m_bufferSize = length;
        }
    }


    int realSize = evbuffer_remove(req->input_buffer, self->m_buffer, length);
    printf("body realSize = %d \n", realSize);
    self->m_buffer[realSize] = '\0';
    printf("buffer = %s \n", self->m_buffer);

    event_base_loopbreak(self->m_base);
}

void HttpClient_YPL::onClose(struct evhttp_connection *conn, void *arg)
{
    printf("*************onClose \n");
    HttpClient_YPL *self = (HttpClient_YPL *)arg;
    event_base_loopbreak(self->m_base);
}

void HttpClient_YPL::post()
{
    //char *url = "/portal/sss";
    char *url = "/api/service";
    struct evhttp_request *req = evhttp_request_new(onRequestDone, this);
    evhttp_add_header(req->output_headers, "Host", m_host.c_str());
    evhttp_make_request(m_conn, req, EVHTTP_REQ_POST, url);
    evhttp_connection_set_timeout(req->evcon, 3);
    evhttp_connection_set_closecb(req->evcon, onClose, this);
    //evbuffer_add(req->output_buffer, "123", 3);
    event_base_dispatch(m_base);
}
