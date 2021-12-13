#pragma once

#include "Acceptor.h"
#include "EpollPoller.h"
#include "Mylog.h"


namespace wd{

class TcpServer
{
public:
    typedef EpollPoller::EpollCallback TcpServerCallback;
    TcpServer(const char * ip, unsigned short port);

    void start();
    void stop();
    void setConnectionCallback(TcpServerCallback cb);
    void setMessageCallback(TcpServerCallback cb);
    void setCloseCallback(TcpServerCallback cb);

private:
    Acceptor _acceptor;
    EpollPoller _epollfd;

    TcpServerCallback _onConnerctionCb;
    TcpServerCallback _onMessageCb;
    TcpServerCallback _onCloseCb;
};

}


