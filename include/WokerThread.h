/* author:liushuang */
/* email:repickliu@163.com */
#pragma once

#include "Thread.h"
#include "Threadpool.h"


namespace wd
{
class WokerThread
:public Thread
{
public:
    WokerThread(Threadpool & Threadpool); 

private:
    void run();

private:
    Threadpool & _threadpool;

};
}



