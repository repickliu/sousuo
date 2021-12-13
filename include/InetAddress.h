#pragma once

#include <netinet/in.h>
#include <string>

using std::string;

namespace wd{

class InetAddress
{
public:
    InetAddress(const string & ip, unsigned short port);
    InetAddress(unsigned short port);
    InetAddress(struct sockaddr_in &addr)
    : _addr(addr)
    {}
    
    string ip() const;
    unsigned short port() const;
    struct sockaddr_in * getSockaddrPtr();

private:
    struct sockaddr_in _addr; 
};

}


