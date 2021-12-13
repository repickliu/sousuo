#pragma once
#include "MutexLock.h"
#include "TcpConnection.h"
#include "Noncoppyable.h"

#include <sys/epoll.h>
#include <vector>
#include <map>
#include <memory>
#include <functional>

using std::shared_ptr;
using std::map;
using std::vector;

namespace wd{
class Acceptor;

class EpollPoller : Noncoppyable
{
public:
    typedef TcpConnection::TcpConnectionCallback EpollCallback;
    typedef std::function<void()> Functor;
    EpollPoller(Acceptor & acceptor);
    ~EpollPoller();


    void runInloop(const Functor && cb);
    void loop();
    void unloop();
    void doPeningFunctors();
    void wakeup();

    void setConnectionCallback(EpollCallback cb);
    void setMessageCallback(EpollCallback cb);
    void setCloseCallback(EpollCallback cb);


private:
    int  createEpollfd();
    int  createEventfd();
    void addEpollReadFd(int fd);
    void delEpollReadFd(int fd);

    void waitEpollFds();
    void handleNewConnection();
    void handleMessage(int peerfd);
    void handleRead();

private:
    int _efd;
    int _eventfd;
    Acceptor & _acceptor;
    map<int, shared_ptr<TcpConnection>> _conns;
    vector<struct epoll_event> _eventsList;
    bool _isLooping;
    vector<Functor> _pendingFunctors;

    MutexLock _mutex;

    EpollCallback _onConnerctionCb;
    EpollCallback _onMessageCb;
    EpollCallback _onCloseCb;
};

}


