#pragma once
#ifndef _WD_DictProducer_H__
#define _WD_DictProducer_H__

#include "SplitToolCppJieba.h"

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>

#include <string>
#include <vector>
#include <map>

using namespace std;

namespace wd{

class DictProducer
{
public:
    DictProducer(const string& dir):_dir(dir){}
    DictProducer(const string& dir,SplitToolCppJieba* splitTool);
    void build_en_dict();
    void build_cn_dict();
    void store_dict(const char* filepath);
    void show_files()const;
    void show_dict()const;

private:
    void set_allfilename(const string& dir);
    void push_dict(const string & word);

private:
    string _dir;
    vector<string> _files;
    map<string, int> _dict;
    SplitToolCppJieba * _splitTool;
};

}
#endif

