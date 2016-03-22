#include "skp_libevent_test.h"
#include "skp_libevent_buffer_test.h"
#include "skp_libevent_listen.h"
#include "http-server.h"

int main(void)
{
    //setvbuf(stdout, NULL, _IONBF, 0);
    //run();
    //run_buffer();
    //run_listen();
    http_server_main();
    return 0;
}

