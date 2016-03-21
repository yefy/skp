#include "skpSignal.h"
#include "skpSignal_p.h"
#include "skpEventRead.h"
#include "skpLog.h"

#include <execinfo.h>

typedef struct SkpSignal_s SkpSignal_t;
struct SkpSignal_s{
    int     signo;
    const char   *signame;
    const char   *name;
    void  (*handler)(int signo);
};


static SkpSignal_t  g_signal[] = {
    {SIGHUP, "SIGHUP", "Hangup (POSIX)", SkpSignal::signalHandler},
    {SIGINT, "SIGINT", "Interrupt (ANSI)", SkpSignal::signalHandler},
    {SIGQUIT, "SIGQUIT", "Quit (POSIX)", SkpSignal::signalHandler },
     {SIGILL, "SIGILL", "Illegal instruction (ANSI)", SkpSignal::signalHandler },
     {SIGTRAP, "SIGTRAP", "Trace trap (POSIX)", SkpSignal::signalHandler },
    {SIGABRT, "SIGABRT", "Abort (ANSI)", SkpSignal::signalHandler },
     {SIGBUS, "SIGBUS", "BUS error (4.2 BSD)", SkpSignal::signalHandler },
     {SIGFPE, "SIGFPE", "Floating-point exception (ANSI)", SkpSignal::signalHandler },
     //{SIGKILL, "SIGKILL", "Kill, unblockable (POSIX)", SkpSignal::skp_signal_handler },
     {SIGUSR1, "SIGUSR1", "User-defined signal 1 (POSIX)", SkpSignal::signalHandler },
    {SIGSEGV, "SIGSEGV", "Segmentation violation (ANSI)", SkpSignal::signalHandler },
     {SIGUSR2, "SIGUSR2", "User-defined signal 2 (POSIX)", SkpSignal::signalHandler },
    {SIGPIPE, "SIGPIPE", "Broken pipe (POSIX)", SIG_IGN },
    {SIGALRM, "SIGALRM", "Alarm clock (POSIX)", SkpSignal::signalHandler },
    {SIGTERM, "SIGTERM", "Termination (ANSI)", SkpSignal::signalHandler },
     {SIGSTKFLT, "SIGSTKFLT", "Stack fault", SkpSignal::signalHandler },
    {SIGCHLD, "SIGCHLD", "Child status has changed (POSIX)", SkpSignal::signalHandler },
     {SIGCONT, "SIGCONT", "Continue (POSIX)", SkpSignal::signalHandler },
     {SIGSTOP, "SIGSTOP", "Stop, unblockable (POSIX)", SkpSignal::signalHandler },
     {SIGTSTP, "SIGTSTP", "Keyboard stop (POSIX)", SkpSignal::signalHandler },
     {SIGTTIN, "SIGTTIN", "Background read from tty (POSIX)", SkpSignal::signalHandler },
     {SIGTTOU, "SIGTTOU", "Background write to tty (POSIX)", SkpSignal::signalHandler },
     {SIGURG, "SIGURG", "Urgent condition on socket (4.2 BSD)", SkpSignal::signalHandler },
     {SIGXCPU, "SIGXCPU", "CPU limit exceeded (4.2 BSD)", SkpSignal::signalHandler },
     {SIGXFSZ, "SIGXFSZ", "File size limit exceeded (4.2 BSD)", SkpSignal::signalHandler },
     {SIGVTALRM, "SIGVTALRM", "Virtual alarm clock (4.2 BSD)", SkpSignal::signalHandler },
     {SIGPROF, "SIGPROF", "Profiling alarm clock (4.2 BSD)", SkpSignal::signalHandler },
     {SIGWINCH, "SIGWINCH", "Window size change (4.3 BSD, Sun)", SkpSignal::signalHandler },
    {SIGIO, "SIGIO", "I/O now possible (4.2 BSD)", SkpSignal::signalHandler },
     {SIGPWR, "SIGPWR", "Power failure restart (System V)", SkpSignal::signalHandler },
    {SIGSYS, "SIGSYS", "Bad system call", SIG_IGN },
    { 0, NULL, "", NULL }
};

typedef struct SkpAddSignal_s SkpAddSignal_t;
struct SkpAddSignal_s{
    int signo;
    std::string name;
    std::function<void(int)> func;
};



//========================SkpSignalPrivate
SkpSignalPrivate::SkpSignalPrivate() :
    SkpObjectDataPrivate(),
    m_eventRead(skp_null)
{

}

SkpSignalPrivate::~SkpSignalPrivate()
{

}


//=======================SkpSignal

SkpSignal *SkpSignal::m_signal = NULL;
SkpSignal::SkpSignal() :
    SkpObjectData(*new SkpSignalPrivate())
{
    /*
    m_eventRead = new SkpEventRead();
    m_eventRead->connect(m_pipe.notifyRead(), std::bind(&SkpSignal::onSigno, this, std::placeholders::_1));
    m_eventRead->start(0);
    */

    initSignal();
}

SkpSignal::~SkpSignal()
{

}

void SkpSignal::initSignal()
{
    SkpSignal_t      *signal;
    struct sigaction   sa;

    for (signal = g_signal; signal->signo != 0; signal++) {
        memset(&sa, 0x00, sizeof(struct sigaction));
        sa.sa_handler = signal->handler;
        sigemptyset(&sa.sa_mask);
        if (sigaction(signal->signo, &sa, NULL) == -1) {
            return;
        }
    }

#if 0
    struct sigaction  time;
    struct itimerval  itv;

    memset(&time, 0x00, sizeof(struct sigaction));
    time.sa_handler = SkpSignal::skp_signal_handler;
    sigemptyset(&time.sa_mask);

    if (sigaction(SIGALRM, &time, NULL) == -1) {
       skp_gerror("regist SIGALRM signal error\n");
       return;
    }

    itv.it_interval.tv_sec = 0;
    itv.it_interval.tv_usec = 200 * 1000;
    itv.it_value.tv_sec = 0;
    itv.it_value.tv_usec = 200 * 1000;

    if (setitimer(ITIMER_REAL, &itv, NULL) == -1) {
       skp_gerror("setitimer error\n");
       return;
    }
#endif
}

void SkpSignal::signalHandler(int sig)
{
    SkpSignal::instance()->onSignal(sig);
}

void SkpSignal::onSignal(int sig)
{
    int err;
    err = errno;

    if(sig == SIGABRT || sig == SIGSEGV)
        onSignalCatchCore(sig);
    else
        //::write(m_pipe.notifyWrite(), &sig, sizeof(int));
        signalManage(sig);

    errno = err;
}

void SkpSignal::signalManage(int signo)
{
    SKP_D(SkpSignal);
    SkpSignal_t *signal;

    for (signal = g_signal; signal->signo != 0; signal++) {
        if(signal->signo == signo && signal->signo != SIGALRM) {
            skpLogFatal_g("signo = %d, signame = %s, name = %s\n", signo, signal->signame, signal->name);
        }
    }

    if(skpD->m_signoMap.find(signo) == skpD->m_signoMap.end())
        return;

    const SkpAddSignal_t &addSigno = skpD->m_signoMap[signo];
    skpLogFatal_g("name = %s \n", addSigno.name.c_str());
    addSigno.func(signo);
}

void SkpSignal::onSignalCatchCore(int sig)
{
    SkpSignal_t *signal;

    for (signal = g_signal; signal->signo != 0; signal++) {
        if(signal->signo == sig && signal->signo != SIGALRM) {
            skpLogFatal_g("sig = %d, signame = %s, name = %s\n", sig, signal->signame, signal->name);
        }
    }

    void *callStack[1000] = {0};
    int size = backtrace(callStack,1000);
    char **symbols = backtrace_symbols(callStack,size);
    for(int i = 0; i < size; ++i)
    {
        if(symbols && symbols[i])
            skpLogFatal_g("%s\n",symbols[i]);
    }

    if(symbols)
        free(symbols);

    switch(sig)
    {
    case SIGSEGV:
        ::signal(sig,SIG_DFL);
        ::raise(sig);
        break;
    case SIGABRT:
        ::signal(sig,SIG_DFL);
        ::raise(sig);
        break;
    }
}

SkpSignal *SkpSignal::instance()
{
    if(!m_signal) {
        m_signal = new SkpSignal();
    }

    return m_signal;
}

void SkpSignal::signalBlock()
{
    sigset_t set;
    sigemptyset(&set);

    SkpSignal_t *signal;
    for (signal = g_signal; signal->signo != 0; signal++) {
        sigaddset(&set, signal->signo);

        //skpLogFatal_g("sig = %d block\n", signal->signo);
    }

    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
        skpLogFatal_g("sigprocmask error\n");
    }
}

void SkpSignal::signalSuspend()
{
    sigset_t set;
    sigemptyset(&set);
    sigsuspend(&set);
}

void SkpSignal::addSigno(int signo, const char *name, std::function<void(int)> func)
{
    SKP_D(SkpSignal);
    SkpAddSignal_t addSignal;
    addSignal.signo = signo;
    addSignal.name = name;
    addSignal.func = func;

    skpD->m_signoMap[signo] = addSignal;
}

void SkpSignal::onSigno(bool isTime)
{
    SKP_D(SkpSignal);
    SKP_UNUSED(isTime);
    int signo;
    while(::read(skpD->m_pipe.notifyRead(), &signo, sizeof(int)) > 0) {
        signalManage(signo);
    }
}
