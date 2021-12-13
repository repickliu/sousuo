#include "../include/EpollPoller.h"
#include "../include/TcpConnection.h"
#include "../include/Acceptor.h"

#include <unistd.h>
#include <sys/eventfd.h>

namespace wd
{

EpollPoller::EpollPoller(Acceptor & acceptor)
: _efd(createEpollfd())
, _eventfd(createEventfd())
, _acceptor(acceptor)
, _eventsList(1024)
, _isLooping(false)
, _pendingFunctors()
, _mutex()
{
    addEpollReadFd(_acceptor.fd());
    addEpollReadFd(_eventfd);
}
EpollPoller::~EpollPoller()
{
    ::close(_efd);
}

int EpollPoller::createEventfd()
{
    int fd = ::eventfd(0,0);
    if(fd == -1) {
        ::perror(">> eventfd");
    }
    return fd;
}

void EpollPoller::doPeningFunctors()
{
    vector<Functor> tmp;
    {
        MutexLockGuard autolock(_mutex);
        tmp.swap(_pendingFunctors);
    }

    for(auto & functor : tmp)
        functor();
    cout << "---------end-------------" << endl;
}

void EpollPoller::handleRead()
{
    uint64_t howmany = 0;
    int ret = ::read(_eventfd, &howmany, sizeof(howmany));
    if(ret != sizeof(howmany)) {
        ::perror(">> read");
    }
}

void EpollPoller::wakeup()
{
    uint64_t one = 1;
    int ret = ::write(_eventfd, &one, sizeof(one));
    if(ret != sizeof(one)) {
        ::perror(">> write");
    }
}

void EpollPoller::runInloop(const Functor && cb)
{
    {
        MutexLockGuard autolock(_mutex);
        _pendingFunctors.push_back(std::move(cb));
    }
    wakeup();//通知IO线程
}

void EpollPoller::loop()
{
    _isLooping = true;
    while(_isLooping) {
        waitEpollFds();
    }
}
void EpollPoller::unloop()
{
    if(_isLooping) 
        _isLooping = false;
}

int EpollPoller::createEpollfd()
{
    int fd = ::epoll_create1(0);
    if(fd == -1) {
        ::perror(">> epoll_create1");
    }
    return fd;
}

void EpollPoller::addEpollReadFd(int fd)
{
    struct epoll_event value;
    value.data.fd = fd;
    value.events = EPOLLIN;
    int ret = ::epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &value);
    if(ret == -1) {
        perror(" >> epoll_ctl");
    }
}

void EpollPoller::delEpollReadFd(int fd)
{

    struct epoll_event value;
    value.data.fd = fd;
    value.events = EPOLLIN;
    int ret = ::epoll_ctl(_efd, EPOLL_CTL_DEL, fd, &value);
    if(ret == -1) {
        perror(" >> epoll_ctl");
    }
}

void EpollPoller::waitEpollFds()
{
    int nready;
    do{
        nready = ::epoll_wait(_efd, 
                              &*_eventsList.begin(), 
                              _eventsList.size(), 
                              5000);
    }while(nready == -1 && errno == EINTR);

    if(nready == 0) {
        cout << "-------- Wait TCP Connection ! --------";
    }else if(nready == -1) {
        ::perror(">> epoll_wait");
        return;
    }else {
        if(nready == _eventsList.size()) {
            _eventsList.resize(2 * _eventsList.size());
        }
    }
    for(int idx = 0; idx < nready; ++idx){
        if(_eventsList[idx].data.fd == _acceptor.fd() &&
           _eventsList[idx].events & EPOLLIN) {
            //处理新链接
            handleNewConnection();
        }else if (_eventsList[idx].data.fd == _eventfd){
            //处理事件
            handleRead();
            cout << ">> dpPendingFunctors()" << endl;
            doPeningFunctors();
        }
        else {
            if(_eventsList[idx].events & EPOLLIN)
            {
                //读事件发生
                handleMessage(_eventsList[idx].data.fd);
            }
        }
    }
}

void EpollPoller::handleNewConnection()
{
    int peerfd = _acceptor.accept();
    addEpollReadFd(peerfd);

    shared_ptr<TcpConnection> conn(new TcpConnection(peerfd, this));//创造新的连接对象conn
    //绑定回调函数
    conn->setConnectionCallback(_onConnerctionCb);
    conn->setMessageCallback(_onMessageCb);
    conn->setCloseCallback(_onCloseCb);

    std::pair<map<int, TcpConnectionPtr>::iterator, bool> ret;
    //连接队列中添加连接对象<peerfd, conn>
    ret = _conns.insert(std::make_pair(peerfd, conn));
    assert(ret.second == true);
    (void)ret;

    //执行注册的连接函数
    conn->handleConnectionCallback();
}

void EpollPoller::handleMessage(int peerfd)
{
    auto iter = _conns.find(peerfd);
    if(iter != _conns.end()) {
        //判断链接是否断开
        bool Closed = iter->second->isClosed();
        if(Closed){//连接断开
            iter->second->handleCloseCallback();
            delEpollReadFd(peerfd);
            _conns.erase(iter);
        }else {//收到数据
            //计算线程线程中执行消息处理函数
            iter->second->handleMessageCallback();
        }
    }
}

void EpollPoller::setConnectionCallback(EpollCallback cb)
{
    _onConnerctionCb = cb;
}

void EpollPoller::setMessageCallback(EpollCallback cb)
{
    _onMessageCb = cb;
}

void EpollPoller::setCloseCallback(EpollCallback cb)
{
    _onCloseCb = cb;
}

}
