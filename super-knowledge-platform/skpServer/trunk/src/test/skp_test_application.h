#ifndef SKP_TEST_APPLICATION_H
#define SKP_TEST_APPLICATION_H

#include "skpApplication.h"


class SkpTestApplicationPrivate;

class SkpTestApplication : public SkpApplication
{
public:
    SkpTestApplication(int argc, char **argv);
    virtual ~SkpTestApplication();
private:
    SKP_DECLARE_PRIVATE(SkpTestApplication);
public:
    virtual void listen();
    virtual void start();
    void onAccept(int fd);
};

#endif // SKP_TEST_APPLICATION_H
