#include "skpPipe.h"
#include "skpUtility.h"

SkpPipe::SkpPipe()
{
    if (pipe(fds)) {
        SKP_ASSERT(skp_false);
    }

    notify_read_fd = fds[0];
    notify_write_fd = fds[1];

    if(setNoblock(notify_write_fd) == skp_false) {
        ::close(notify_read_fd);
        ::close(notify_write_fd);
        SKP_ASSERT(skp_false);
    }

    if(setNoblock(notify_read_fd) == skp_false) {
        ::close(notify_read_fd);
        ::close(notify_write_fd);
        SKP_ASSERT(skp_false);
    }
}

SkpPipe::~SkpPipe()
{
    ::close(notify_read_fd);
    ::close(notify_write_fd);
}

int SkpPipe::notifyRead()
{
    return notify_read_fd;
}

int SkpPipe::notifyWrite()
{
    return notify_write_fd;
}
