#include "../include/TcpConnection.h"
#include "../include/EpollPoller.h"
#include "../include/Mylog.h"

namespace wd 
{
TcpConnection::TcpConnection(int peerfd, EpollPoller *loop)
: _sock(peerfd)
, _localAddr(locallInetAddr())
, _peerAddr(peerInetAddr())
, _socketIO(peerfd)
, _isShutdownWrite(false)
, _loop(loop)
{
    _sock.nonblock();
}

TcpConnection::~TcpConnection()
{
    if(!_isShutdownWrite)
    {
        _isShutdownWrite = true;
        shutdown();
    }
}

void TcpConnection::setConnectionCallback(TcpConnectionCallback cb)
{
    _onConnerctionCallback = cb;
}

void TcpConnection::setMessageCallback(TcpConnectionCallback cb)
{
    _onMessageCallback = cb;
}

void TcpConnection::setCloseCallback(TcpConnectionCallback cb)
{
    _onCloseCallback = cb;
}

void TcpConnection::handleConnectionCallback()
{
    if(_onConnerctionCallback)
        _onConnerctionCallback(shared_from_this());
}

void TcpConnection::handleMessageCallback()
{
    if(_onMessageCallback)
        _onMessageCallback(shared_from_this());
}

void TcpConnection::handleCloseCallback()
{
    if(_onCloseCallback)
        _onCloseCallback(shared_from_this());
}

string TcpConnection::receive()
{
    char buff[65536] = {0};
    int ret = _socketIO.readline(buff, 65536);
    
    if (ret == 0) {
        return string();
    }else {
        return string(buff);
    }
}

void TcpConnection::send(const string & msg)
{
    _socketIO.writen(msg.data(), msg.size());
    string logwarn = "Message sent through socket " + std::to_string(_socketIO.getFd());
    LogWarn(logwarn);
    std::cout << "socket = " << _socketIO.getFd() << std::endl;
}

void TcpConnection::sendInLoop(const string& msg)
{
    _loop->runInloop(std::bind(&TcpConnection::send, this, msg));   
}

string TcpConnection::printPeerAddr()
{
    InetAddress peeraddr = peerInetAddr();
    string addr = peeraddr.ip();
    int port = (int)peeraddr.port();
    string msg = "ip is: " + addr + " port is " + std::to_string(port); 
    return msg;
}

string TcpConnection::printLocalAddr()
{
    InetAddress localladdr = locallInetAddr();
    string addr = localladdr.ip();
    int port = (int)localladdr.port();
    string msg = "ip is: " + addr + " port is " + std::to_string(port); 
    return msg;
}

string TcpConnection::to_String()
{
    return printLocalAddr() + " >>>> " + printPeerAddr();
}

InetAddress TcpConnection::locallInetAddr()
{
    struct sockaddr_in localladdr;
    socklen_t len = sizeof(struct sockaddr_in);
    int ret = ::getsockname(_sock.fd(), (struct sockaddr*)&localladdr, &len); 
    if (ret == -1) {
        ::perror(">> getsockname");
    }
    return localladdr;
}

InetAddress TcpConnection::peerInetAddr()
{
    struct sockaddr_in peeraddr;
    socklen_t len = sizeof(struct sockaddr_in);
    int ret = ::getpeername(_sock.fd(), (struct sockaddr*)&peeraddr, &len);
    if (ret == -1) {
        ::perror(">> getpeername");
    }
    return peeraddr;
}

void TcpConnection::shutdown()
{
    if(!_isShutdownWrite)
    {
        _sock.shutdownWrite();
        _isShutdownWrite = true;
    }
}

bool TcpConnection::isClosed()
{
    return _socketIO.isClosed();
}

}


