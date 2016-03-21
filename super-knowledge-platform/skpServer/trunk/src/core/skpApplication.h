#ifndef SKPAPPLICATION_H
#define SKPAPPLICATION_H

#include "skpObjectData.h"

class SkpApplicationPrivate;
typedef struct SkpChannel_s SkpChannel;

class SkpApplication : public SkpObjectData
{
public:
    SkpApplication(int argc, char **argv);
    virtual ~SkpApplication();
protected:
    SkpApplication(SkpApplicationPrivate &d, int argc, char **argv);
private:
    SKP_DECLARE_PRIVATE(SkpApplication);
public:
    void init(int argc, char **argv);
    void destory();

    virtual void listen() = 0;
    virtual void start() = 0;
    int exec(int forkNumber = 0);

    void singleProcess();
    void singleWork();

    bool createProcessPipe();
    void passOpenChannel(SkpChannel *ch);

    void masterProcess();
    void masterWork();

    pid_t spawnProcess();
    pid_t checkChild();
    void childWork();
    void childInit();


    bool writeChannel(int s, SkpChannel *ch, int size);
    bool readChannel(int s, SkpChannel *ch, int size);

    void channelHandler();
    void onRead(bool isTimeOut);

    static SkpApplication *instance();

    void onSigno(int signo);
};

#endif // SKPAPPLICATION_H
