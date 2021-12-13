#include "../include/Config.h"

namespace wd{
Config* Config::_Instance = NULL;
Config::Config(const string& path)
: _filepath(path)
, _configMap()
{
    fstream ifs(path);
    string section;
    string content;
    while(ifs >> section >> content){
        _configMap.insert(make_pair(section, content));
    }
    ifs.close();
}
}//end of wd


