#include "skp_libevent_listen.h"

int run_listen()
{
  //evthread_use_pthreads();//enable threads

  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(struct sockaddr_in));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(8989);

  struct event_base *base = event_base_new();
  struct evconnlistener *listener
      = evconnlistener_new_bind(base, listener_cb, base,
                    LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE | LEV_OPT_THREADSAFE,
                    10, (struct sockaddr*)&sin,
                    sizeof(struct sockaddr_in));

  evconnlistener_set_error_cb(listener, accept_error_cb);

  event_base_dispatch(base);

   evconnlistener_free(listener);
  event_base_free(base);

  return 0;
}


void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
         struct sockaddr *sock, int socklen, void *arg)
{
  struct event_base *base = (struct event_base*)arg;

  struct bufferevent *bev =  bufferevent_socket_new(base, fd,
                         BEV_OPT_CLOSE_ON_FREE);

  bufferevent_setcb(bev, socket_read_cb, NULL, socket_error_cb, NULL);
  bufferevent_enable(bev, EV_READ | EV_PERSIST);
}

static void
accept_error_cb(struct evconnlistener *listener, void *ctx)
{
        struct event_base *base = evconnlistener_get_base(listener);
        int err = EVUTIL_SOCKET_ERROR();
        fprintf(stderr, "Got an error %d (%s) on the listener. "
                "Shutting down.\n", err, evutil_socket_error_to_string(err));

        event_base_loopexit(base, NULL);
}


void socket_read_cb(struct bufferevent *bev, void *arg)
{
  char msg[4096];

  size_t len = bufferevent_read(bev, msg, sizeof(msg)-1 );

  msg[len] = '\0';
  printf("server read the data %s\n", msg);

  char reply[] = "I has read your data";
  bufferevent_write(bev, reply, strlen(reply) );
}


void socket_error_cb(struct bufferevent *bev, short events, void *arg)
{
  if (events & BEV_EVENT_EOF)
    printf("connection closed\n");
  else if (events & BEV_EVENT_ERROR)
    printf("some other error\n");

  //这将自动close套接字和free读写缓冲区
  bufferevent_free(bev);
}
