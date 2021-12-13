#include "DictProducer.h"

#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

namespace wd{
void DictProducer::build_en_dict()
{
    ifstream ifs(_dir);
    string line;
    if(!ifs.good()) {
        cerr << ">> ifstream open file error!\n";
        return;
    }
    while(getline(ifs, line)) {
        istringstream iss(line);
        string word;
        while(iss >> word) {
            bool flag = true;
            for(auto s : word){
                if(!isalpha(s)) flag = false;
            }
            if(flag){
                transform(word.begin(),word.end(),word.begin(),::tolower);
                ++_dict[word];
            }
        }
    } 
    ifs.close();
}
void DictProducer::set_allfilename(const string& dir)
{
    DIR *Dir;
    struct dirent *ptr;

    if((Dir=opendir(dir.data())) == NULL)
    {
        cerr << "open dir error..." << endl;
        return;
    }

    while((ptr = readdir(Dir)) != NULL)
    {
        if(strcmp(ptr->d_name, ",")==0 || strcmp(ptr->d_name, "..")==0)
            continue;
        else if(ptr->d_type == 8){
            _files.push_back(dir + ptr->d_name);
            cout << ptr->d_name << endl;
        }
        else
            continue;

    }
    closedir(Dir);
}
DictProducer::DictProducer(const string& dir,SplitToolCppJieba* splitTool)
:_dir(dir)
,_splitTool(splitTool)
{
    set_allfilename(dir);
}
void DictProducer::build_cn_dict()
{
    for(auto file : _files)
    {
       
        ifstream ifs(file);
        string line;
        if(!ifs.good()) {
            cerr << ">> ifstream open file error!\n";
            return;
        }
        while(getline(ifs, line)) {
            vector<string> results = _splitTool->cut(line);           
            for(auto c : results){
                if(c.size() > 3){
                    bool flag = true;
                    for(auto s : c)
                        if((s>='0' && s<='9') || isalpha(s)) {
                            flag = false;
                            break;
                        }
                    if(flag)
                        _dict[c]++;
                }
            }
        } 
        ifs.close();
    }

}
void DictProducer::store_dict(const char * filepath)
{
    ofstream ofs(filepath);
    if(!ofs.is_open()) {
        cerr << ">> ofstream open file error!\n";
        return;
    }
    for(auto dict : _dict) {
        ofs << dict.first << " " << dict.second << endl;
    }
    ofs.close();
}
void DictProducer::show_files()const
{
    cout << "file path is " << _dir << endl;    
    return;
}
void DictProducer::show_dict()const
{
    for(auto dict : _dict){
        cout << dict.first << " " << dict.second << endl;
    }
    return;
}
void DictProducer::push_dict(const string & word)
{
   ++_dict[word];
   return;
}
}//end of wd
