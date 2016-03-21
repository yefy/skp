#ifndef SKPSIGNAL_H
#define SKPSIGNAL_H

#include "skpObjectData.h"

class SkpSignalPrivate;

class SkpSignal : public SkpObjectData
{
public:
    SkpSignal();
    virtual ~SkpSignal();
private:
    SKP_DECLARE_PRIVATE(SkpSignal);
public:
    static SkpSignal *instance();
    static SkpSignal *m_signal;
    void addSigno(int sig, const char *name, std::function<void(int)> func);
    static void signalHandler(int sig);
    void signalBlock();
    void signalSuspend();
private:
    void initSignal();
    void onSignal(int sig);
    void onSignalCatchCore(int sig);
    void signalManage(int signo);
    void onSigno(bool isTime);
};

#endif // SKPSIGNAL_H
