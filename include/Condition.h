#pragma once

#include "Noncoppyable.h"

#include <pthread.h>

namespace wd
{

class MutexLock;

class Condition
:Noncoppyable
{
public:
    Condition(MutexLock & mutex);
    ~Condition();

    void wait();
    void notify();
    void notifyall();

private:
    MutexLock & _mutex;
    pthread_cond_t _cond;
};

}

