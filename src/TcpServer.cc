#include "../include/TcpServer.h"

namespace wd{
TcpServer::TcpServer(const char * ip, unsigned short port)
: _acceptor(ip, port)
, _epollfd(_acceptor)
{}

void TcpServer::start()
{
    _acceptor.ready();
    _epollfd.setMessageCallback(_onMessageCb);
    _epollfd.setConnectionCallback(_onConnerctionCb);
    _epollfd.setCloseCallback(_onCloseCb);
    _epollfd.loop();
}

void TcpServer::stop()
{
    _epollfd.unloop();
}

void TcpServer::setConnectionCallback(TcpServerCallback cb)
{
    _onConnerctionCb = cb;
}

void TcpServer::setMessageCallback(TcpServerCallback cb)
{
    _onMessageCb = cb;
}

void TcpServer::setCloseCallback(TcpServerCallback cb)
{
    _onCloseCb = cb;
}

}//end of wd
