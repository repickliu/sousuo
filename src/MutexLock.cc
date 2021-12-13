/* author liushuang */
/* email repickliu@163.com */

#include "MutexLock.h"

#include <stdio.h>
#include <errno.h>

namespace wd
{

MutexLock::MutexLock()
{
    if(pthread_mutex_init(&_mutex,NULL))
    {
        perror("pthread_mutex_init");
    }
}

MutexLock::~MutexLock()
{
    if(pthread_mutex_destroy(&_mutex))
    {
        perror("pthread_mutex_destroy");
    }
}

void MutexLock::lock()
{
    if(pthread_mutex_lock(&_mutex))
    {
        perror("pthread_mutex_lock");
    }
}

void MutexLock::unlock()
{
    if(pthread_mutex_unlock(&_mutex))
    {
        perror("pthread_mutex_unlock");
    }
}

}

