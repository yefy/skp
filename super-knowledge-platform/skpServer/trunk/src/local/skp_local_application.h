#ifndef SKP_LOCAL_APPLICATION_H
#define SKP_LOCAL_APPLICATION_H

#include "skpApplication.h"


class SkpLocalApplicationPrivate;
class SkpLocalSocketTcp;

class SkpLocalApplication : public SkpApplication
{
public:
    SkpLocalApplication(int argc, char **argv);
    virtual ~SkpLocalApplication();
private:
    SKP_DECLARE_PRIVATE(SkpLocalApplication);
public:
    virtual void listen();
    virtual void start();
    void onAccept(int fd);
};

#endif // SKP_LOCAL_APPLICATION_H
