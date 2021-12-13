#pragma once

#include "Noncoppyable.h"
#include "Mylog.h"

#include <iostream>
#include <functional>
#include <pthread.h>


using std::cout;
using std::endl;


namespace wd{
class Thread
:Noncoppyable
{
public:
    using ThreadCallback = std::function<void()>;//回调函数接受执行方法
    Thread(ThreadCallback && callback);


    virtual
    ~Thread();

    void start();//开启子线程
    void join();//回收子线程

    pthread_t getThreadId(){return _pthid;}
private:
    /* virtual void run() = 0;//线程执行的任务 */
    
    static void * threadFunc(void *);
private:
    pthread_t _pthid;
    bool _isrunning;
    ThreadCallback _callback;//线程要执行的任务

};
}

