#ifndef HTTPCLIENT_YPL_H
#define HTTPCLIENT_YPL_H

#include <string>
#include <event2/util.h>

struct evhttp_request;
struct evhttp_connection;

class HttpClient_YPL
{
public:
    HttpClient_YPL();

    static char *uriEncode(const char *uri);
    static char *uriDecode(const char *uri);

    static void onRequestDone(struct evhttp_request *req, void *arg);
    static void onClose(struct evhttp_connection *conn, void *arg);
    void post(const char *url, const char *data, int size);
    void post(const char *prefix, const char *url);

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
