#pragma once

#include "Mylog.h"
#include "InetAddress.h"
#include "Socket.h"

#include <unistd.h>
namespace wd{

class Acceptor
{
public:
    Acceptor(const string & ip, unsigned short port);
    

    void ready();
    int accept();
    int fd() const;

private:
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();
    
private:
    Socket _sock;
    InetAddress _addr;
};

}


