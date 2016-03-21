#ifndef SKPTHREAD_P_H
#define SKPTHREAD_P_H

#include "skpEvent_p.h"
#include "skpThread.h"
#include "skpMutex.h"
#include "skpPipe.h"


class SkpEventRead;
class SkpTimer;

class SkpThreadPrivate : public SkpEventPrivate
{
public:
  SkpThreadPrivate();
  virtual ~SkpThreadPrivate();
  static SkpThread *currThread(pthread_t threadID = pthread_self());
  static void appendThread(SkpThread * thread);
  static void removeThread(SkpThread * thread);
private:
  SKP_DECLARE_PUBLIC(SkpThread);

private:
  //static std::vector<SkpThread *> m_eventThreads;
  static SkpMutex m_threadmutex;
  static std::map<pthread_t, SkpThread *> m_threadMap;

private:
  bool m_isRequestStop;
  bool m_isRunning;
  bool m_isRequestExit;
  bool m_isExit;
  bool m_isStart;
  bool m_isJoin;
  pthread_t m_threadID;
private:
  SkpEventRead *m_eventRead;
  SkpMutex m_pipeMutex;
  SkpPipe m_pipe;
  std::vector<SkpEventObject*> m_eventIn;
  std::vector<SkpEventObject*> m_eventOut;
private:
  std::vector<SkpObject *> m_deleteLaterObjects;
  std::vector<SkpObject *> m_waitObjects;
  SkpMutex m_deleteLatermutex;
  SkpTimer *m_timer;
};

#endif // SKPTHREAD_P_H
