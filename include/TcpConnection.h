#pragma once

#include "Noncoppyable.h"
#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"

#include <iostream>
#include <memory>
#include <functional>
#include <string>
#include <string.h>
using std::string;

namespace wd{

class EpollPoller;
class TcpConnection;

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

class TcpConnection : Noncoppyable,
public std::enable_shared_from_this<TcpConnection>
{
public:
    typedef std::function<void(const TcpConnectionPtr&)> TcpConnectionCallback;
    explicit
    TcpConnection(int peerfd, EpollPoller * loop);
    ~TcpConnection();

    string receive();
    void send(const string & msg);

    void sendInLoop(const string& msg);

    void setConnectionCallback(TcpConnectionCallback cb);
    void setMessageCallback(TcpConnectionCallback cb);
    void setCloseCallback(TcpConnectionCallback cb);

    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();
    
    string printLocalAddr();
    string printPeerAddr();
    string to_String();

    bool isClosed();
    int getSockfd() {return _socketIO.getFd();}
    EpollPoller* getLoop() {return _loop;}


private:
    InetAddress locallInetAddr();
    InetAddress peerInetAddr();
    
    void shutdown();

private:
    Socket      _sock;
    InetAddress _localAddr;
    InetAddress _peerAddr;
    SocketIO    _socketIO;
    bool        _isShutdownWrite;
    EpollPoller* _loop;
    
    TcpConnectionCallback    _onConnerctionCallback;
    TcpConnectionCallback    _onMessageCallback;
    TcpConnectionCallback    _onCloseCallback;

};

}


