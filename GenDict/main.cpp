#include "DictProducer.h"
#include "../include/Config.h"

using namespace wd;
void test0()
{
    
    Config& conf = *Config::getInstance("../conf/conf");
    SplitToolCppJieba splitTool;
    DictProducer dict(conf.getConfigMap().find("cndir")->second, &splitTool);
    dict.build_cn_dict();
    dict.store_dict(conf.getConfigMap().find("cnDictPath")->second.data());
    dict.show_dict();
    dict.show_files();
}

int main()
{
    test0();
    return 0;
}

