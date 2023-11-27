#ifndef ETHREAD_H_
#define ETHREAD_H_

#include <iostream>
 
static void* runThread(void* arg);
 
class Thread
{
  public:
    Thread():started(false),m_tid(0) {};
    virtual ~Thread() {};
 
    int32_t start() { started = true; int32_t t = pthread_create(&m_tid, NULL, runThread, this); return t; }
    void stop() { started = false; }
    int32_t join() { return pthread_join(m_tid, NULL); }
    bool isRunning() { return started; }
    void setName(std::string name) { pthread_setname_np(m_tid, name.c_str()); }
    pthread_t getThreadPointer() {return m_tid;}
 
    virtual void* run() = 0;
  protected:
    bool started;
    pthread_t m_tid;
};
 
static void* runThread(void* arg)
{
return ((Thread*)arg)->run();
}
 
#endif