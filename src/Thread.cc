/* author liushuang */
/* email repickliu@163.com */

#include "Thread.h"

namespace wd{
Thread::Thread(ThreadCallback && callback)
:_pthid(0)
,_isrunning(false)
,_callback(callback)//绑定回调函数
{

}

void Thread::start()
{
    pthread_create(&_pthid,NULL,threadFunc,this);//第四个参数是给线程执行体传参的
    _isrunning = true;
    LogInfo("WorkerThread started");
}

void * Thread::threadFunc(void *arg)
{
    Thread * p = static_cast<Thread *>(arg);
    if(p)
        p->_callback();//回调函数表示线程执行的任务
    return NULL;

}

void Thread::join()
{
    if(_isrunning) {
        pthread_join(_pthid,NULL);
        _isrunning = false;
    }
}

Thread::~Thread()
{
    if(_isrunning) {
        pthread_detach(_pthid);//强制回收线程资源
        _isrunning = false;
    }
    cout << "~Thread()" << endl;
}

}//end of namespace wd


