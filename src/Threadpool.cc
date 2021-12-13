/* author liushuang */
/* email repickliu@163.com */

#include "Threadpool.h"
#include "Thread.h"

#include <unistd.h>

#include <iostream>

using namespace std;

namespace wd
{

Threadpool::Threadpool(size_t threadNum, size_t queSize)
: _threadNum(threadNum)
, _threads()
, _queSize(queSize)
, _taskque(_queSize)
, _isExit(false)
{
    _threads.reserve(threadNum);        
}

Threadpool::~Threadpool()
{
    if(!_isExit)
        stop();
}

void Threadpool::start()
{
    for(size_t idx = 0; idx != _threadNum; ++idx){
        unique_ptr<Thread> up(new Thread(
            bind(&Threadpool::threadFunc, this)
                                        ));
        _threads.push_back(std::move(up));
    }
    for(auto & threadPtr : _threads)
        threadPtr->start();
}

void Threadpool::addTask(Task* ptask)
{
    /* cout << "addTask" << endl; */
    _taskque.push(ptask);
}

Task* Threadpool::getTask()
{
    return _taskque.pop();
}

//每一个子线程要执行的任务：不断的去任务队列之中获取任务并执行
void Threadpool::threadFunc()//注册到线程的回调函数中去执行
{
    while(!_isExit){
        Task* task = getTask();//获取任务
        if(task)
            task->process();//执行任务
        /* sleep(1); */
    }
}

void Threadpool::stop()
{
    cout << " >>> main thread Threadpool::stop() entering" << endl;
    if(!_isExit)
    {
        while(!_taskque.empty()){//退出前确保所有任务都执行完成
            /* sleep(1); */
            ::usleep(100);
        }
        _isExit = true;
        //推出while循环表示所有的任务都被子线程取走
        _taskque.wakeup();//再最后一个任务时可能存在多个线程争夺，会发生阻塞，如果不唤醒的话会发生死锁。
        //回收每一个子线程的资源
        for(auto & threadPtr : _threads) {
            threadPtr->join();//等待每一个子线程执行完毕
        }
    }
    cout << " >>> main thread Threadpool::stop() exit" << endl;
}

}

