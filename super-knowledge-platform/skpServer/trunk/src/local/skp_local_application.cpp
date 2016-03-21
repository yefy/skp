#include "skp_local_application.h"
#include "skpApplication_p.h"
#include "skpServerTcp.h"
#include "skpAutoFree.h"
#include "skp_local_socket_tcp.h"
#include "skpThread.h"
#include "skpMutex.h"

#include "skp_local_manage_parse.h"
#include "skp_local_manage_route.h"

//=============SkpLocalApplicationPrivate

#define SOCKET_THREAD_NUMBER 0
class SkpLocalApplicationPrivate : public SkpApplicationPrivate
{
public:
  SkpLocalApplicationPrivate();
  virtual ~SkpLocalApplicationPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpLocalApplication);

private:
  SkpServerTcp *serverTcp;
  std::vector<SkpThread *> m_threads;
  SkpMutex m_tcpScoketsMutex;
};

SkpLocalApplicationPrivate::SkpLocalApplicationPrivate() :
    SkpApplicationPrivate(),
    serverTcp(skp_null)
{

}

SkpLocalApplicationPrivate::~SkpLocalApplicationPrivate()
{
}

//===================SkpLocalApplication
SkpLocalApplication::SkpLocalApplication(int argc, char **argv) :
    SkpApplication(*new SkpLocalApplicationPrivate(), argc, argv)
{
    listen();
}

SkpLocalApplication::~SkpLocalApplication()
{
    SKP_D(SkpLocalApplication);
    skp_delete(skpD->serverTcp);
    SkpLocalManageParse::destroy();
    SkpLocalManageRoute::destroy();

    for(auto iter = skpD->m_threads.begin(); iter != skpD->m_threads.end(); iter++) {
        SkpThread *thread = *iter;
        thread->exit();
        while(!thread->isExit());
    }
}

void SkpLocalApplication::listen()
{
    printf("listen \n");
    SKP_D(SkpLocalApplication);
    skpD->serverTcp = new SkpServerTcp();
    skpD->serverTcp->listen(33331);
}


void SkpLocalApplication::start()
{
    printf("start \n");
    SKP_D(SkpLocalApplication);

    SkpLocalManageParse::init();
    SkpLocalManageRoute::init();

    for(int i = 0; i < SOCKET_THREAD_NUMBER; i ++) {
        SkpThread *thread = new SkpThread();
        thread->start();
        skpD->m_threads.push_back(thread);
    }

    skpD->serverTcp->start(std::bind(&SkpLocalApplication::onAccept, this, std::placeholders::_1));
}

void SkpLocalApplication::onAccept(int fd)
{
    /*
    SKP_D(SkpLocalApplication);

    SkpThread *thread = skp_null;
    if(!skpD->m_threads.empty()) {
        static int i = 0;
        thread = skpD->m_threads.at(i % skpD->m_threads.size());
        i++;
    } else {
        thread = SkpThread::currThread();
    }

    SkpLocalSocketTcp *localSocketTcp = new SkpLocalSocketTcp(fd, *thread);
    localSocketTcp->start();
    */

    SkpLocalSocketTcp *localSocketTcp = new SkpLocalSocketTcp(fd);
    localSocketTcp->start();
}
