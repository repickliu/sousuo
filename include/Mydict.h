#pragma once

#include "trie.h"
#include "Config.h"
#include <cctype>
#include <queue>

namespace wd{


class Mydict
{

public:
    static Mydict* getInstance(const char* file_en, const char* file_cn){
        if(_pInstance == nullptr){
            _pInstance = new Mydict(file_en, file_cn);
        }
        return _pInstance;
    }
    trie getDict(){ return _dict;}
    void Delete();

    
private:
    Mydict(const char* file_en, const char* file_cn):_dict(file_en, file_cn){};
private:
    static Mydict* _pInstance;
    trie _dict;
};

}


