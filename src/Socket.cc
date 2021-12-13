#include "../include/Socket.h"
#include "../include/InetAddress.h"

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace wd
{
Socket::Socket()
{
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(_fd == -1){
        ::perror(">>> socket");
        //记录日志
    }
}

Socket::Socket(int fd)
: _fd(fd)
{}

Socket::~Socket()
{
    ::close(_fd);
}

int Socket::fd() const
{
    return _fd;
}

void Socket::shutdownWrite()    
{
    if(::shutdown(_fd, SHUT_WR)) {
        ::perror(">> shutdown");
        //记录日志
    }    
}

void Socket::nonblock()
{
    int flags = ::fcntl(_fd, F_GETFL, 0);
    if(flags == -1) {
        ::perror(">> fcntl");
    }
    int ret = ::fcntl(_fd, F_SETFL, O_NONBLOCK | flags);
    if(ret == -1) {
        ::perror(">> fcntl");
    }

}

InetAddress Socket::getLocalAddr(int sockfd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(sockaddr_in);
    if(::getsockname(sockfd, (struct sockaddr *)&addr, &len) == -1)
    {
        perror("getsockname error");
    }
    return InetAddress(addr);
}

InetAddress Socket::getPeerAddr(int sockfd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(sockaddr_in);
    if(::getpeername(sockfd, (struct sockaddr *)&addr, &len) == -1)
    {
        perror("getpeername error");
    }
    return InetAddress(addr);
}

}


