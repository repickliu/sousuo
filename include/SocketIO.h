#pragma once

#include <iostream>

using std::cout;


namespace wd{

class SocketIO
{
public:
    explicit SocketIO(int fd) : _fd(fd){};

    int readn(char * buff, int len);
    int writen(const char * buff, int len);

    bool isClosed();
    int recvPeek(char * buff, int len);

    int readline(char * buff, int maxlen);

    int getFd(){return _fd;}
private:
    int _fd;
};

}


