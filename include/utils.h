#pragma once

#include <string>

using std::string;
using std::size_t;


class utils
{
public:
    static utils * gettInstance();
    void destory();
    int EdictLenth(string str1, string str2);
    
private:
    utils();
    ~utils(){}

    bool isCh(const char * str);
    size_t nBytesCode(const char ch);
    size_t length(const std::string &str);
    int triple_min(const int &a, const int &b, const int &c);
private:
    static utils* _Instance;
};



