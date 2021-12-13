/* author liushuang */
/* email repickliu@163.com */
#include "TaskQueue.h"

#include <iostream>


using std::cout;
using std::endl;


namespace wd
{

TaskQueue::TaskQueue(size_t size)
: _queSize(size)
, _que()
, _mutex()
, _notFull(_mutex)
, _notEmpty(_mutex)
, _flag(true)
{}

bool TaskQueue::full() const
{
    return _que.size() == _queSize;
}

bool TaskQueue::empty() const
{
    return _que.empty();
}

//push方法运行在生产者线程
void TaskQueue::push(Task* t)
{
    MutexLockGuard autolock(_mutex);
    while(full()){
        _notFull.wait();//使用while可以防止被异常唤醒
    }

    _que.push(t);//生产者线程放数据
    _notEmpty.notify();//通知消费者线程取数据
}

//pop方法运行在消费者线程
Task* TaskQueue::pop()
{
    MutexLockGuard autolock(_mutex);
    /* cout << "before while " << endl; */
    while(_flag && empty()){
        _notEmpty.wait();
    }
    /* cout << " exit while " << endl; */
    if(_flag) {
        Task* ret = _que.front();
        _que.pop();
        _notFull.notify();//通知生产者线程放数据
        return ret;
    } else {
        return NULL;
    }
}

void TaskQueue::wakeup()
{
    if(_flag) _flag = false;
    _notEmpty.notifyall();//唤醒所有的等待_notEmpty条件变量的子线程
}


}

