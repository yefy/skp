#ifndef SKPSERVERTCP_H
#define SKPSERVERTCP_H

#include "skpObjectData.h"

class SkpServerTcpPrivate;
class SkpThread;

class SkpServerTcp : public SkpObjectData
{
public:
    SkpServerTcp();
    virtual ~SkpServerTcp();
protected:
    SkpServerTcp(SkpServerTcpPrivate &d);
private:
    SKP_DECLARE_PRIVATE(SkpServerTcp);

public:
    virtual int listen(uint16 port);
    virtual int listen(struct sockaddr_in *addr);

    virtual void start(std::function<void(int)> func);
    virtual void stop();
    virtual void free();

    void onAccept(bool isTimeOut);

protected:
    void removeReadReady();
};

#endif // SKPSERVERTCP_H
