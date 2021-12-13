#include "../include/Acceptor.h"

namespace wd {
Acceptor::Acceptor(const string & ip, unsigned short port)
: _sock()
, _addr(ip, port)
{
}

int Acceptor::accept()
{
    int peerfd = ::accept(_sock.fd(), NULL, NULL);
    if(peerfd == -1)
    {
        perror("accept error");
    }
    return peerfd;
}

void Acceptor::ready()
{
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}

void Acceptor::setReuseAddr()
{
    int on = 1;
    if(::setsockopt(_sock.fd(),
                    SOL_SOCKET,
                    SO_REUSEADDR,
                    &on,
                    static_cast<socklen_t>(sizeof(on))) == -1)
    {
        LogError("setsockoopt reuseaddr error");
        ::close(_sock.fd());
        exit(EXIT_FAILURE);

    }
}

void Acceptor::setReusePort()
{
    int on = 1;
    if(::setsockopt(_sock.fd(),
                    SOL_SOCKET,
                    SO_REUSEPORT,
                    &on,
                    static_cast<socklen_t>(sizeof(on))) == -1)
    {
        LogError("setsockoopt reuseport error");
        ::close(_sock.fd());
        exit(EXIT_FAILURE);

    }
}

int Acceptor::fd() const 
{
    return _sock.fd();
}

void Acceptor::bind()
{
    if(-1 == ::bind(_sock.fd(),
                    (const struct sockaddr*)_addr.getSockaddrPtr(),
                    sizeof(InetAddress)))
    {
        perror("bind error");
        ::close(_sock.fd());
        exit(EXIT_FAILURE);
    }
}

void Acceptor::listen()
{
    if(-1 == ::listen(_sock.fd(), 10))
    {
        perror("listen error");
        ::close(_sock.fd());
        exit(EXIT_FAILURE);
    }
}

}//end of wd


