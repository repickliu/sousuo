#pragma once

#include "Config.h"
#include "Mydict.h"
#include "Threadpool.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "Mylog.h"
#include "QueryTask.h"

#include <iostream>
#include <string>

using std::string;

namespace wd{

void onConnection(const TcpConnectionPtr& conn);
void onMessage(const TcpConnectionPtr& conn);
void onClose(const TcpConnectionPtr& conn);

class SpellCorrectServer
{
    friend void onConnection(const TcpConnectionPtr& conn);
    friend void onMessage(const TcpConnectionPtr& conn);
    friend void onClose(const TcpConnectionPtr& conn);
public:
    SpellCorrectServer()
    : _conf(*Config::getInstance("../conf/conf"))
    , _tcpserver(_conf.getConfigMap()["ServerIP"].data(), atoi(_conf.getConfigMap()["Port"].data()))  
    , _threadpool(atoi(_conf.getConfigMap()["ThreadpoolCap"].data()), atoi(_conf.getConfigMap()["TaskQueSize"].data()))  
    , _dict(Mydict::getInstance(_conf.getConfigMap()["enDictPath"].data(), _conf.getConfigMap()["cnDictPath"].data()))
    {
    }
    ~SpellCorrectServer();

    void start();
    void stop();

private:
    Config&    _conf;
    TcpServer  _tcpserver;
    Threadpool _threadpool;
    Mydict*    _dict;
};

}//end wd



