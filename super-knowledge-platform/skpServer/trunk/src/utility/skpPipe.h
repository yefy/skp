#ifndef SKPPIPE_H
#define SKPPIPE_H

class SkpPipe
{
public:
    SkpPipe();
    ~SkpPipe();
    int fds[2];
    int notify_read_fd; // = fds[0];
    int notify_write_fd; // = fds[1];
    int notifyRead();
    int notifyWrite();
};

#endif // SKPPIPE_H
