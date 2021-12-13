#include "../include/SpellCorrectServer.h"

namespace wd{

Threadpool * g_threadpool = NULL;    

void onConnection(const TcpConnectionPtr& conn)
{
    cout << conn->to_String() << endl;    
    string loginfo = conn->to_String() + "The link was established.";
    LogInfo(loginfo);
    conn->send("Connection Established.");
}

void onMessage(const TcpConnectionPtr& conn)
{
    string query(conn->receive());
    QueryTask* ptask(new QueryTask(query, *Mydict::getInstance("", ""), 5, conn));
    cout << " QueryTask Added.   sockio = " << conn->getSockfd() << " " << ptask << endl;
    g_threadpool->addTask(ptask);
}

void onClose(const TcpConnectionPtr& conn)
{
    string loginfo = conn->to_String() + " close.";
    LogInfo(loginfo);
    cout << conn->to_String() << " closed" << endl;
}


void SpellCorrectServer::start()
{
    LogInfo("Server Started");
    cout << "----------------- Server Started! ----------------" << endl;
    g_threadpool = &_threadpool;
    g_threadpool->start();
    _tcpserver.setConnectionCallback(&onConnection);
    _tcpserver.setMessageCallback(&onMessage);
    _tcpserver.setCloseCallback(&onClose);
    _tcpserver.start();
}

SpellCorrectServer::~SpellCorrectServer()
{
    cout << "SpellCorrectServer stop." << endl;
    stop();
}

void SpellCorrectServer::stop()
{
    _dict->Delete();
    Config::getInstance("abc")->Delete();
    _tcpserver.stop();
    _threadpool.stop();
}

}//end wd
