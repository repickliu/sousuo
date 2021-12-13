#pragma once

#include "Mydict.h"
#include "Threadpool.h"
#include "TcpServer.h"
#include "Noncoppyable.h"
#include "redis.h"

#include <queue>
#include <json/json.h>

struct MyResult
{
    string _word;
    int _freq;
    int _dist;
};
struct MyCompare{
    bool operator()(pair<string, int> left,pair<string, int>  right){
       return left.second > right.second; 
    }
};
class QueryTask
: wd::Noncoppyable, public wd::Task
{
public:
    QueryTask(const string& query, wd::Mydict &dict, int num,
              wd::TcpConnectionPtr conn)
    : _queryWord(query.substr(0, query.size() - 1))
    , _dict(dict)
    , _resultque()
    , _num(num), _conn(conn)
    {
    } 
    void process();

private:
    void setToRedis(const string& str);
    string getFromRedis(const string& str);
    void queryIndex(const string& sin);
    string toJson();

private:
    string _queryWord;
    wd::Mydict _dict;
    priority_queue<pair<string, int>, vector<pair<string, int>>,MyCompare> _resultque;
    size_t _num;
    wd::TcpConnectionPtr _conn;
};



