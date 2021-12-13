#pragma once

#include "Noncoppyable.h"

#include <iostream>


using std::endl;
using std::cout;

namespace wd{
class InetAddress;
class Socket : Noncoppyable
{
public:
    Socket();
    explicit Socket(int);

    ~Socket();

    int fd() const;

    void shutdownWrite();
    void nonblock();

    static InetAddress getLocalAddr(int sockfd);
    static InetAddress getPeerAddr(int sockfd);
private:
    int _fd;
};

}


