#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <string>

using namespace std;

namespace wd{

class Config
{
public:
    static Config* getInstance(const string& rhs){
        if(!_Instance){
            _Instance = new Config(rhs); 
        }
        return _Instance;
    }
    void Delete(){
        if(_Instance)
            delete _Instance;
    }
    map<string, string> &getConfigMap() {return _Instance->_configMap; }

private:
    Config(const string&);
private:
    static Config* _Instance;
    string _filepath;
    map<string, string> _configMap;
};
}//end of wd;


