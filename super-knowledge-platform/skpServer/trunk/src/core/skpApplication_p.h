#ifndef SKPAPPLICATION_P_H
#define SKPAPPLICATION_P_H

#include "skpObjectData_p.h"
#include "skpApplication.h"

#define SKP_MAX_PROCESSES         1024



#define SKP_CMD_OPEN_CHANNEL   1
#define SKP_CMD_CLOSE_CHANNEL  2
#define SKP_CMD_QUIT           3
#define SKP_CMD_TERMINATE      4
#define SKP_CMD_REOPEN         5

typedef struct SkpChannel_s SkpChannel;

struct SkpChannel_s {
     uint  command;       //worker要操作的指令
     pid_t  pid;           //worker进程id
     int   slot;          //worker进程在ngx_process中的索引
     int    fd;            //有可能用到的文件描述符
};


typedef struct {
    // 进程ID
    pid_t           pid;
    // 由waitpid系统调用获取到的进程状态
    int                 status;
    /*
    这是由socketpair系统调用产生出的用于进程间通信的socket句柄，这一对socket句柄可以互相通信，
    目前用于master 父进程与worker子进程间的通信。
    */
    int        channel[2];

    // 子进程的循环执行方法，当父进程调用ngx_spawn_process 生成子进程时使用
    //ngx_spawn_proc_pt   proc;
    /*
    上米昂的ngx_spawn_proc_pt方法中第二个参数需要传递一个指针，它是可选的。例如worker子进程就不需要，
    而cache manage进程就需要ngx_cache_manager_ctx上下文成员。这时data一般与ngx_spawn_proc_pt方法中第二个参数是等价的。
    */
    //void               *data;
    // 进程名称。操作系统中显示的进程名称与name相同
    //char               *name;

    // 标志位，为1表示在重新生成子进程
    unsigned            respawn:1;
    // 标志位，为1表示正在生成子进程
    unsigned            just_spawn:1;
    // 标志位，为1表示在进行父，子进程分离
    unsigned            detached:1;
    // 标志位，为1表示进程正在退出
    unsigned            exiting:1;
    // 标志位，为1表示进程已经退出
    unsigned            exited:1;
} SkpProcess;


class SkpThread;
class SkpEventRead;

class SkpApplicationPrivate : public SkpObjectDataPrivate
{
public:
  SkpApplicationPrivate();
  virtual ~SkpApplicationPrivate();
private:
  SKP_DECLARE_PUBLIC(SkpApplication);

private:
  bool m_isReload;
  bool m_isQuit;
  bool m_isTerminate;

  bool m_isMaster;
  int  m_exitChild;
  int m_childNumber;

  SkpThread *m_mainThread;
  SkpProcess m_processes[SKP_MAX_PROCESSES];
  int m_processSlot;
  int m_readChannel_fd;
  SkpEventRead *m_channelEventRead;

  static SkpApplication *m_application;
};

#endif // SKPAPPLICATION_P_H
