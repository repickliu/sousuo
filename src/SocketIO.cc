#include "../include/SocketIO.h"

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

namespace wd 
{



int SocketIO::readn(char * buff, int len)
{
    char * p = buff;
    int left = len;
    while(left > 0){
        int ret = ::read(_fd, p, left);

        if(ret == -1 && errno == EINTR)
            continue;
        else if (ret == -1) {
            ::perror(">> read");
            return len -left;
        }else if(ret == 0) {
            return len - left;
        }else{
            left -= ret;
            p += ret;
        }
    }
    return len - left;
}

int SocketIO::writen(const char * buff, int len)
{

    const char * p = buff;
    int left = len;
    while(left > 0){
        int ret = ::write(_fd, p, left);
        if(ret == -1 && errno == EINTR)
            continue;
        else if (ret == -1) {
            ::perror(">> write");
            return len -left;
        }else{
            left -= ret;
            p += ret;
        }
    }
    return len - left;
}

int SocketIO::recvPeek(char * buff, int len)
{
    int ret = 0;
    do {
        ret = ::recv(_fd, buff, len, MSG_PEEK);//并未移走缓冲区的数据，只是查看
    } while(ret == -1 && errno == EINTR);
    return ret;
}

int SocketIO::readline(char * buff, int maxlen)
{
    int left = maxlen - 1;
    char * p = buff;
    int total = 0;
    while(left > 0) {
        int ret = recvPeek(p, left);
        for(int idx = 0; idx < ret; ++ idx) {
            if(p[idx] == '\n') {
                int size = idx + 1;
                readn(p, idx + 1);//从内核缓冲区中移走数据
                total += size;
                p += size;
                *p = '\0';
                return total;
            }
        }
        readn(p, ret);
        p += ret;
        left -= ret;
        total += ret;
    }
    *p = '\0';
    return  total;
}

bool SocketIO::isClosed()
{
    int nret = 0;
    char buff[1024];
    ::memset(buff, 0, sizeof(buff));
    do{
        nret = ::recv(_fd, buff, sizeof(buff), MSG_PEEK);
    }while(nret == -1 && errno == EINTR);
    return (nret == 0);
}

}


