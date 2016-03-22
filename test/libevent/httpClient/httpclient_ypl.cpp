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
#include <string.h>

char *HttpClient_YPL::uriEncode(const char *uri)
{
    return evhttp_encode_uri(uri);
}

char *HttpClient_YPL::uriDecode(const char *uri)
{
    return evhttp_decode_uri(uri);
}

HttpClient_YPL::HttpClient_YPL()
{
    m_base = event_base_new();
    m_dnsbase = evdns_base_new(m_base, 1);
    //URL=http\://14.23.90.103/api/service
//    m_host = "14.23.90.103";
//    m_port = 80;

//    m_host = "222.76.151.10";
//    m_port = 51010;
//    m_host = "192.168.164.148";
//    m_port = 8080;
    m_host = "172.16.11.228";
    m_port = 7777;
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

void HttpClient_YPL::post(const char *url, const char *data, int size)
{
    //char *url = "/api/service?api_name=epaylinks_umps_user_open_account&app_id=IRqmcUmK8TeIR6QSvo9NyWFi&app_user_id=15880271625&app_user_type=03&busi_water_no=1456901649259&cust_name=test&format=json&login_passwd=Ra9iKFH%2BckngKChoxcQmDFYUYOoS9Sy8Jpq6%2B7EeR9PlqxJG%2Bu%2F1CzuYfECF2vDbfWfVjqJnyoLdyZPMCcF9842YRlnqGfTz7CfE%2BwjkdETwrpj8xfraW8MqH%2Ba8dBNGvdCJoDPU7ShAiGKrRLWaXfNFQmG%2BI4lNoysztINCj6eoSpFWNRPnGkxNiPmxAFf6BkPg2f3GfXK2tCg5t%2FFrpmbTuHCXThjNVR4szhO0QH2yuEKPXFIFMfQY%2F1Qfe17Oa%2BpjXojStaxH0sZfkKJMt5YVChjfVXE7jI%2Fnm6Xii%2F6X2fY4eKRfmo6mGilTAhJ11ExUPbp%2B46CMex4JfkVzRQ%3D%3D&pay_passwd=L%2BGqvqtrH7xe4lp3mSyoIvfIl4d%2FaBYsNYqM8ABh%2BaHkJdfiQguqFQ7%2FdRqhp7VKZ4smY1voGbQIY0gbpFKEVS4PsLVXgvDReb%2BNHzNeHggIREJDHluUXxzwcu6BOv0bcULAUf%2BE37T%2B290gi%2F%2B0QIRKBiScrBpbP0FF5wfJNSVEbGiBjkafzZQojkg%2FYYNk1Zn%2FjSumBR9DB5aZPtXDmb1auR5kQ%2BPdl5m3FazzondxH8vBKf45BU0PFaPWIolAn%2Fd9G9JZDg1shCb2WIWvNXs%2B4U65%2F6ITHRZv5z0muK3txtXf9ISHLcKPtV%2F9KAPZxGAM3%2FY53ETqDkrVz79G7w%3D%3D&sign=CFbGkc6EFB7XT9e0XnMNMk%2FyHJEmNH%2FUjq56ZVBH%2BoBNpeC4hPJBdDLojCBaX1%2BeWcf4Q63060ljECUQ5bvQ7OFG%2Ba%2BLDdgUqiXQxRlTMfkL%2BYY58l5FMZMQ9EBmDCrIf2U6ZSqG7OHFlIzd6n6rm2ck%2BwN1wENgUX4lojfMrLw%3D&terminal_no=43181666&timestamp=2016-03-02+14%3A54%3A09&ver=1.0";
    printf("*********url = %s \n", url);
    struct evhttp_request *req = evhttp_request_new(onRequestDone, this);
    evhttp_add_header(req->output_headers, "Host", m_host.c_str());
    evhttp_make_request(m_conn, req, EVHTTP_REQ_POST, url);
    evhttp_connection_set_timeout(req->evcon, 13);
    evhttp_connection_set_closecb(req->evcon, onClose, this);
    if(data)
        evbuffer_add(req->output_buffer, data, size);
    event_base_dispatch(m_base);
}

void HttpClient_YPL::post(const char *prefix, const char *url)
{
    std::string temp;
    temp.reserve(strlen(prefix) + strlen(url));
    temp += std::string(prefix);
    temp += std::string(url);
    post(temp.c_str(), NULL, 0);
}
