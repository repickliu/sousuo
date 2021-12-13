/* author:liushuang */
/* email:repickliu@163.com */
#pragma once

#include "TaskQueue.h"

#include <vector>
#include <memory>
#include <functional>

using namespace std;

namespace wd
{

class Task
{
public:
    virtual
    void process() = 0;
    virtual ~Task() {};
};

class Thread;
class Threadpool
{
    friend class WokerThread;
public:
    Threadpool(size_t threadNum,size_t queSize);
    ~Threadpool();

    void addTask(Task*);
    void start();//开启线程池
    void stop();//停止线程池
private:
    void threadFunc();
    Task* getTask();

private:
    size_t _threadNum;
    vector<unique_ptr<Thread>> _threads;
    size_t _queSize;
    TaskQueue _taskque;
    bool _isExit;
};

}



