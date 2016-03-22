#ifndef HTTPCLIENT_YPL_H
#define HTTPCLIENT_YPL_H

#include <string>

struct evhttp_request;
struct evhttp_connection;

class HttpClient_YPL
{
public:
    HttpClient_YPL();
    static void onRequestDone(struct evhttp_request *req, void *arg);
    static void onClose(struct evhttp_connection *conn, void *arg);
    void post();

private:
    struct event_base *m_base;
    struct evdns_base * m_dnsbase;
    struct evhttp_connection *m_conn;
    std::string m_host;
    unsigned short int m_port;
    char *m_buffer;
    int m_bufferSize;
};

#endif // HTTPCLIENT_YPL_H
