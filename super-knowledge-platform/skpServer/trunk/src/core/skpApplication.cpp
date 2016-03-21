#include "skpApplication.h"
#include "skpApplication_p.h"
#include "skpThread.h"
#include "skpEventRead.h"
#include "skpAutoFree.h"
#include "skpLog.h"
#include "skpSignal.h"

//=============SkpApplicationPrivate

SkpApplication *SkpApplicationPrivate::m_application = NULL;
SkpApplicationPrivate::SkpApplicationPrivate() :
    SkpObjectDataPrivate(),
    m_isReload(skp_false),
    m_isQuit(skp_false),
    m_isTerminate(skp_false),
    m_isMaster(skp_true),
    m_exitChild(0),
    m_childNumber(0),
    m_mainThread(skp_null),
    m_processSlot(0),
    m_channelEventRead(skp_null)
{

}

SkpApplicationPrivate::~SkpApplicationPrivate()
{

}
//======================SkpApplication

SkpApplication::SkpApplication(int argc, char **argv) :
    SkpObjectData(*new SkpApplicationPrivate())
{
    init(argc, argv);
}

SkpApplication::SkpApplication(SkpApplicationPrivate &d, int argc, char **argv) :
    SkpObjectData(d)
{
    init(argc, argv);
}

SkpApplication::~SkpApplication()
{
    destory();
}

void SkpApplication::init(int argc, char **argv)
{
    SKP_D(SkpApplication);

    SKP_UNUSED(argc);
    SKP_UNUSED(argv);


    SkpApplicationPrivate::m_application = this;

    skp_update_system_time_ms(skp_true);

    memset(skpD->m_processes, 0x00, sizeof(skpD->m_processes));
    for(int i = 0; i < SKP_MAX_PROCESSES; i++) {
        skpD->m_processes[i].pid = -1;
        skpD->m_processes[i].channel[0] = -1;
        skpD->m_processes[i].channel[1] = -1;
    }

    SkpSignal::instance()->addSigno(SIGTERM, "close", std::bind(&SkpApplication::onSigno, this, std::placeholders::_1));
    SkpSignal::instance()->addSigno(SIGINT, "close", std::bind(&SkpApplication::onSigno, this, std::placeholders::_1));
    SkpSignal::instance()->addSigno(SIGQUIT, "quit", std::bind(&SkpApplication::onSigno, this, std::placeholders::_1));
    SkpSignal::instance()->addSigno(SIGCHLD, "child close", std::bind(&SkpApplication::onSigno, this, std::placeholders::_1));

}

void SkpApplication::destory()
{
    SKP_D(SkpApplication);
    skp_delete(skpD->m_channelEventRead);
    SkpApplicationPrivate::m_application = NULL;
    skp_delete(skpD->m_mainThread);
}

void SkpApplication::listen()
{
    SKP_ASSERT(skp_false);
}

void SkpApplication::start()
{
    SKP_ASSERT(skp_false);
}

int SkpApplication::exec(int forkNumber)
{
    SKP_D(SkpApplication);
    skpD->m_childNumber = forkNumber;
    if(skpD->m_childNumber == 0) {
        singleProcess();
    } else {
        masterProcess();
    }

    return 0;
}

SkpApplication *SkpApplication::instance()
{
    return SkpApplicationPrivate::m_application;
}

void SkpApplication::singleProcess()
{
    SKP_D(SkpApplication);
    skpD->m_mainThread = new SkpThread();
    skpD->m_mainThread->updateThreadID();
    SkpThread::appendThread(skpD->m_mainThread);

    start();

    singleWork();
}

void SkpApplication::singleWork()
{
    SKP_D(SkpApplication);
    while(skp_true) {
        skpD->m_mainThread->loop();
        if(skpD->m_isQuit) {
            break;
        }
    }
}


bool SkpApplication::createProcessPipe()
{
    SKP_D(SkpApplication);
    long on = 1;
    int slot = 0;
    for(slot = 0; slot < SKP_MAX_PROCESSES; slot++) {
        if(skpD->m_processes[slot].pid == -1)
            break;
    }

    SKP_ASSERT(slot < SKP_MAX_PROCESSES);

    SkpProcess *currPorcess = &skpD->m_processes[slot];

    //创建socketpair，并设置相关属性
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, currPorcess->channel) == -1)
    {
        return skp_false;
    }

    skpLogDebug_g("channel %d:%d\n", currPorcess->channel[0], currPorcess->channel[1]);

    //设置非阻塞模式
    if(setNoblock(currPorcess->channel[0]) == -1) {
        goto find_error;
    }

    if(setNoblock(currPorcess->channel[1]) == -1) {
        goto find_error;
    }

    //打开异步模式

    if (ioctl(currPorcess->channel[0], FIOASYNC, &on) == -1) {
        goto find_error;
    }

    //设置异步IO的所有者
    if (fcntl(currPorcess->channel[0], F_SETOWN, getpid()) == -1) {
        goto find_error;
    }

//    if (fcntl(currPorcess->channel[1], F_SETOWN, getpid()) == -1) {
//      goto find_error;
//    }



    //当执行后关闭句柄
    if (fcntl(currPorcess->channel[0], F_SETFD, FD_CLOEXEC) == -1) {
        goto find_error;
    }

    if (fcntl(currPorcess->channel[1], F_SETFD, FD_CLOEXEC) == -1) {
        goto find_error;
    }

    return slot;

find_error:
    ::close(currPorcess->channel[0]);
    ::close(currPorcess->channel[1]);
    return skp_false;
}

void SkpApplication::passOpenChannel(SkpChannel *ch)
{
    SKP_D(SkpApplication);
    int  i;

        //遍历整个进程表
    for (i = 0; i < SKP_MAX_PROCESSES; i++) {
        SkpProcess *currPorcess = &skpD->m_processes[i];

        //遇到非存活的进程就跳过
        if (i == ch->slot
            || currPorcess->pid == -1
            || currPorcess->channel[0] == -1)
        {
            continue;
        }

        skpLogDebug_g("pass channel s:%d pid:%d fd:%d to s:%d pid:%d fd:%d\n", \
                      ch->slot, ch->pid, ch->fd, \
                      i, currPorcess->pid, \
                      currPorcess->channel[0]);

        /* TODO: NGX_AGAIN */

        //传递channel给其他子进程（主要是传递句柄）
        writeChannel(currPorcess->channel[0], ch, sizeof(SkpChannel));
    }
}

void SkpApplication::masterProcess()
{
    SKP_D(SkpApplication);

    SkpSignal::instance()->signalBlock();

    pid_t pid;
    for(int i = 0; i < skpD->m_childNumber; i++) {
        pid = spawnProcess();
        if(pid == 0 || pid == -1) {
            return;
        }
    }

    masterWork();
}

void SkpApplication::masterWork()
{
    SKP_D(SkpApplication);
    while(skp_true) {
        SkpSignal::instance()->signalSuspend();
        pid_t pid = checkChild();
        if(pid > 0) {
            skpLogFatal_g("Replace child pid = %d exit\n", pid);
            break;
        }
        skpLogFatal_g("m_exitChild = %d, m_childNumber = %d\n", skpD->m_exitChild, skpD->m_childNumber);
        if(skpD->m_isQuit) {
            if(skpD->m_exitChild == skpD->m_childNumber) {
                skpLogFatal_g("master id = %d exit\n", getpid());
                break;
            }
        }
    }
}

pid_t SkpApplication::spawnProcess()
{
    SKP_D(SkpApplication);
    int slot = createProcessPipe();
    skpD->m_processSlot = slot;

    pid_t pid = fork();

    switch(pid) {
    case -1:
        skpLogError_g("fork error\n");
        break;
    case 0:
        skpD->m_processes[skpD->m_processSlot].pid = getpid();
        skpD->m_processes[skpD->m_processSlot].exited = 0;

        skpLogFatal_g("spawn child pid = %d\n", getpid());

        childInit();

        childWork();

        break;
    default:
        skpD->m_processes[skpD->m_processSlot].pid = pid;
        skpD->m_processes[skpD->m_processSlot].exited = 0;

        SkpChannel  ch;
        ch.command = SKP_CMD_OPEN_CHANNEL;
        ch.pid = skpD->m_processes[skpD->m_processSlot].pid;
        ch.slot = skpD->m_processSlot;
        ch.fd = skpD->m_processes[skpD->m_processSlot].channel[0];

        //建立通道
        passOpenChannel(&ch);

        break;
    }

   return pid;
}

pid_t SkpApplication::checkChild()
{
    SKP_D(SkpApplication);
    if(skpD->m_isQuit)
        return 0;

    for(int i = 0; i < SKP_MAX_PROCESSES; i++) {
        SkpProcess *currProcesse = &skpD->m_processes[i];
        if(currProcesse->pid == -1)
            continue;

        if(currProcesse->exited == 1) {

            skpLogDebug_g("close child pid = %d\n", currProcesse->pid);
            SkpChannel  ch;
            ch.command = SKP_CMD_CLOSE_CHANNEL;
            //ch.command = SKP_CMD_OPEN_CHANNEL;
            ch.pid = currProcesse->pid;
            ch.slot = i;
            ch.fd = currProcesse->channel[0];

            //建立通道
            passOpenChannel(&ch);

            currProcesse->pid = -1;
            currProcesse->exited = 0;

            ::close(currProcesse->channel[0]);
            ::close(currProcesse->channel[1]);

            ///创建child进程
            pid_t pid = spawnProcess();

            switch(pid) {
            case -1:
                break;
            case 0:
                return getpid();
            default :
                skpD->m_childNumber++;
                break;
            }
        }
    }

    return 0;
}


void SkpApplication::childInit()
{
    SKP_D(SkpApplication);
    skpD->m_isMaster = skp_false;

    sigset_t set;
    sigemptyset(&set);

    if (sigprocmask(SIG_SETMASK, &set, NULL) == -1) {
        skpLogFatal_g("sigprocmask error\n");
    }

    skpD->m_mainThread = new SkpThread();
    skpD->m_mainThread->updateThreadID();
    SkpThread::appendThread(skpD->m_mainThread);
    start();


    // 这里循环用户关闭其他worker进程的无用channel资源
    int n;
    for (n = 0; n < SKP_MAX_PROCESSES; n++) {
        SkpProcess *currPorcess = &skpD->m_processes[n];
        if (currPorcess->pid == -1) { //n位置的进程不存在，这里是预防性的代码
            continue;
        }
        //ngx_process_slot是创建worker进程的时候，从master进程复制过来的，此处就是指本worker进程在数组中的索引位置
        if (n == skpD->m_processSlot) {
            continue;
        }

        if (currPorcess->channel[1] == -1) { // channel不存在，跳过
            continue;
        }
        // 创建worker进程时，会将master的资源复制过来，因此需要关闭无用的channel -- 其他worker进程的读端描述符

        ::close(currPorcess->channel[1]);
    }

    // 关闭本worker进程的channel的写端描述符。
    ::close(skpD->m_processes[skpD->m_processSlot].channel[0]);

    skpD->m_readChannel_fd = skpD->m_processes[skpD->m_processSlot].channel[1];


    skpD->m_channelEventRead = new SkpEventRead(*skpD->m_mainThread);
    skpD->m_channelEventRead->connect(skpD->m_readChannel_fd,std::bind(&SkpApplication::onRead, this, std::placeholders::_1));
    skpD->m_channelEventRead->setET(skp_false);
    skpD->m_channelEventRead->start(0);
}

void SkpApplication::childWork()
{
    SKP_D(SkpApplication);
    while(skp_true) {

        skpD->m_mainThread->loop();

        if(skpD->m_isQuit) {
            skpLogFatal_g("child id = %d exit\n", getpid());
            break;
        }
    }
}


bool SkpApplication::writeChannel(int s, SkpChannel *ch, int size)
{
    int             n;
    int           err;
    struct iovec        iov[1];
    struct msghdr       msg;

#if 1

    union {
        struct cmsghdr  cm;
        char            space[CMSG_SPACE(sizeof(int))];
    } cmsg;

    if (ch->fd == -1) {
        msg.msg_control = NULL;
        msg.msg_controllen = 0;

    } else {
        msg.msg_control = (caddr_t) &cmsg;
        msg.msg_controllen = sizeof(cmsg);

        cmsg.cm.cmsg_len = CMSG_LEN(sizeof(int));
        cmsg.cm.cmsg_level = SOL_SOCKET;
        cmsg.cm.cmsg_type = SCM_RIGHTS;

        /*
         * We have to use ngx_memcpy() instead of simple
         *   *(int *) CMSG_DATA(&cmsg.cm) = ch->fd;
         * because some gcc 4.4 with -O2/3/s optimization issues the warning:
         *   dereferencing type-punned pointer will break strict-aliasing rules
         *
         * Fortunately, gcc with -O1 compiles this ngx_memcpy()
         * in the same simple assignment as in the code above
         */

        memcpy(CMSG_DATA(&cmsg.cm), &ch->fd, sizeof(int));
    }

    msg.msg_flags = 0;

#else

    if (ch->fd == -1) {
        msg.msg_accrights = NULL;
        msg.msg_accrightslen = 0;

    } else {
        msg.msg_accrights = (caddr_t) &ch->fd;
        msg.msg_accrightslen = sizeof(int);
    }

#endif

    iov[0].iov_base = (void *) ch;
    iov[0].iov_len = size;

    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    n = ::sendmsg(s, &msg, 0);

    if (n == -1) {
        err = errno;
        if (err == EWOULDBLOCK || err == EAGAIN) {
            return skp_true;
        }

        skpLogError_g("sendmsg() failed\n");
        return skp_false;
    }

    return skp_true;
}

// 读出master发过来的指令数据，是使用recvmsg实现的，详情介绍见《unix网络编程》

bool SkpApplication::readChannel(int s, SkpChannel *ch, int size)
{
    int             n;
    int           err;
    struct iovec        iov[1];
    struct msghdr       msg;

#if 1
    union {
        struct cmsghdr  cm;
        char            space[CMSG_SPACE(sizeof(int))];
    } cmsg;
#else
    int                 fd;
#endif

    iov[0].iov_base = (void *) ch;
    iov[0].iov_len = size;

    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

#if 1
    msg.msg_control = (caddr_t) &cmsg;
    msg.msg_controllen = sizeof(cmsg);
#else
    msg.msg_accrights = (caddr_t) &fd;
    msg.msg_accrightslen = sizeof(int);
#endif

    n = recvmsg(s, &msg, 0);

    if (n == -1) {
        err = errno;
        if (err == EWOULDBLOCK || err == EAGAIN) {
            return skp_true;
        }

        skpLogError_g("recvmsg() failed\n");
        return skp_false;
    }

    if (n == 0) {
        skpLogError_g("recvmsg() returned zero\n");
        return skp_true;
    }

    if ((int)n < (int)sizeof(SkpChannel)) {
        skpLogError_g("recvmsg() returned not enough data: %d\n", n);
        return skp_true;
    }

#if 1

    if (ch->command == SKP_CMD_OPEN_CHANNEL) {

        if (cmsg.cm.cmsg_len < (socklen_t) CMSG_LEN(sizeof(int))) {
            skpLogError_g("recvmsg() returned too small ancillary data\n");
            return skp_false;
        }

        if (cmsg.cm.cmsg_level != SOL_SOCKET || cmsg.cm.cmsg_type != SCM_RIGHTS)
        {
            skpLogError_g("recvmsg() returned invalid ancillary data level %d or type %d\n",
                          cmsg.cm.cmsg_level, cmsg.cm.cmsg_type);
            return skp_false;
        }

        /* ch->fd = *(int *) CMSG_DATA(&cmsg.cm); */

        skpLogDebug_g("send fd = %d\n", ch->fd);

        memcpy(&ch->fd, CMSG_DATA(&cmsg.cm), sizeof(int));
    }

    if (msg.msg_flags & (MSG_TRUNC|MSG_CTRUNC)) {
        skpLogError_g("recvmsg() truncated data\n");
    }

#else

    if (ch->command == NGX_CMD_OPEN_CHANNEL) {
        if (msg.msg_accrightslen != sizeof(int)) {
            ngx_log_error(NGX_LOG_ALERT, log, 0,
                          "recvmsg() returned no ancillary data");
            return NGX_ERROR;
        }

        ch->fd = fd;
    }

#endif

    return n;
}

void SkpApplication::channelHandler()
{
    SKP_D(SkpApplication);

    int          n;
    SkpChannel      ch;

    for ( ;; ) {

        //读取channel，赋值ch
        n = readChannel(skpD->m_readChannel_fd, &ch, sizeof(SkpChannel));

        skpLogDebug_g("channel data size: %d\n", n);

        if (n == skp_false) {
//            if(m_channelEventRead) {
//                delete m_channelEventRead;
//                m_channelEventRead = NULL;
//            }
            return;
        }

        if (n == skp_true) {
            return;
        }

        switch (ch.command) {

        case SKP_CMD_QUIT:
            skpD->m_isQuit = 1;
            break;

        case SKP_CMD_TERMINATE:
            skpD->m_isTerminate = 1;
            break;

        case SKP_CMD_REOPEN:
            skpD->m_isReload = 1;
            break;

        case SKP_CMD_OPEN_CHANNEL:

            skpLogDebug_g("get channel s:%d pid:%d fd:%d\n",
                           ch.slot, ch.pid, ch.fd);

            //对ngx_processes全局进程表进行赋值
            skpD->m_processes[ch.slot].pid = ch.pid;
            skpD->m_processes[ch.slot].channel[0] = ch.fd;
            break;

        case SKP_CMD_CLOSE_CHANNEL:

            skpLogDebug_g("close channel s:%d pid:%d our:%d fd:%d\n",
                           ch.slot, ch.pid, skpD->m_processes[ch.slot].pid,
                    skpD->m_processes[ch.slot].channel[0]);

            skpD->m_processes[ch.slot].pid = -1;
            ::close(skpD->m_processes[ch.slot].channel[0]);
            break;
        }
    }
}

void SkpApplication::onRead(bool isTimeOut)
{
    SKP_UNUSED(isTimeOut);
    channelHandler();
}

void SkpApplication::onSigno(int signo)
{
    SKP_D(SkpApplication);

    if(signo == SIGTERM) {
        skpD->m_isQuit = 1;
        skpLogFatal_g("SkpApplication SIGTERM m_isQuit = 1\n");
    }
    if(signo == SIGINT) {
        skpD->m_isQuit = 1;
        skpLogFatal_g("SkpApplication SIGINT m_isQuit = 1\n");
    }

    if(signo == SIGQUIT) {
        skpD->m_isQuit = 1;
        skpLogFatal_g("SkpApplication SIGQUIT m_isQuit = 1\n");
    }

    if(signo == SIGCHLD) {
        int status;
        pid_t pid;
        int i;
        int one;

        one = 0;

        for ( ;; ) {
            pid = waitpid(-1, &status, WNOHANG);
            skpLogFatal_g("waitpid = %d\n", pid);
            if (pid == 0) {
                break;
            }

            if (pid == -1) {

                if (errno == EINTR) {
                    if(one) {
                        break;
                    } else {
                        continue;
                    }
                }

                if (errno == ECHILD && one) {
                    break;
                }

                break;
            }

            one = 1;

            skpD->m_exitChild += 1;
            skpLogFatal_g("exitChild number = %d, fork number = %d\n",
                       skpD->m_exitChild,
                       skpD->m_childNumber);

            SkpProcess *currProcess = skpD->m_processes;
            for (i = 0; i < SKP_MAX_PROCESSES; i++) {
                if (currProcess[i].pid == pid) {
                    currProcess[i].status = status;
                    currProcess[i].exited = 1;
                    break;
                }
            }

        }
    }
}
