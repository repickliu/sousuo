/* author:liushuang */
/* email:repickliu@163.com */

#pragma once 

#include "MutexLock.h"
#include "Condition.h"

#include <queue>
#include <functional>

namespace wd
{

class Task;
using TaskType = std::function<void()>;

class TaskQueue
{
public:
    TaskQueue(size_t size);

    bool full() const;
    bool empty() const;
    void push(Task* t);
    void wakeup();
    Task* pop();

private:
    size_t _queSize;
    std::queue<Task* > _que;
    MutexLock _mutex;
    Condition _notFull;
    Condition _notEmpty;
    bool _flag;
};

}



