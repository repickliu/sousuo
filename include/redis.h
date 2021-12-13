#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
 
#include <hiredis/hiredis.h>
 
class Redis
{
public:
    static Redis* getInstannce()
    {
        if(Instance == NULL)
        {
            Instance = new Redis();
            Instance->connect("127.0.0.1", 6380);
        }
        return Instance;
    }    
 
    bool connect(std::string host, int port)
    {
        this->_connect = redisConnect(host.c_str(), port);
        if(this->_connect != NULL && this->_connect->err)
        {
            printf("connect error: %s\n", this->_connect->errstr);
            return 0;
        }
        return 1;
    }
 
    std::string get(std::string key)
    {
        std::string str;

        if( existsKey(key.c_str())){
            this->_reply = (redisReply*)redisCommand(this->_connect, "GET %s", key.c_str()); 
            str = this->_reply->str;
            freeReplyObject(this->_reply);
        }else{
            str = "";
        }

        return str;
    }

    void setExpireTime(const char* key)
    {
        redisReply* reply = (redisReply*)redisCommand(this->_connect, "EXPIRE %s %d", key, 600);         
        freeReplyObject(reply);
    }

    int existsKey(const char* ID)
    {
        redisReply * reply = (redisReply*)redisCommand(this->_connect,"exists %s",ID);
        int res = 0;
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1L)
                res  = 1;
        }
        freeReplyObject(reply);
        return res;
    }
 
    void set(std::string key, std::string value)
    {
        redisCommand(this->_connect, "SET %s %s", key.c_str(), value.c_str());
        setExpireTime(key.c_str());
    }
private:
    Redis(){}
 
    ~Redis()
    {
        this->_connect = NULL;
        this->_reply = NULL;                
    }
private:
    static Redis* Instance;
    redisContext* _connect;
    redisReply* _reply;
 
};
