#pragma once

#include "Mylog.h"
#include "Config.h"

#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <cstring>

using namespace std;

namespace wd{


struct trie_node_en{
    int count;
    trie_node_en * child[26];
    string str;
    trie_node_en() : count(0){memset(child, 0, sizeof(child)); }
};

struct trie_node_cn{
    trie_node_cn()
    {
        count = 0;
    }
    int count;
    map<string, trie_node_cn *> child;
};

class trie
{
public:
typedef trie_node_en node_type_en;
typedef trie_node_en* link_type_en;
typedef trie_node_cn node_type_cn;
typedef trie_node_cn* link_type_cn;
#define CH_SIZE 3//汉字大小，linux为3

trie(const char * file_en, const char* file_cn)
{
    /* construct_trie_en(Config::getInstance("../conf/conf")->getConfigMap().find("enDictPath")->second.c_str()); */
    /* construct_trie_cn(Config::getInstance("../conf/conf")->getConfigMap().find("cnDictPath")->second.c_str()); */
    construct_trie_en(file_en);
    construct_trie_cn(file_cn);
}


void construct_trie_cn(const char* filename_cn)
{
    ifstream ifs(filename_cn);
    if(!ifs.good())
    {
        ifs.close();
        LogError("文件打开失败!");
        ::exit(0);
    }
    LogInfo("开始构造字典树!");
    
    string line;
    while(getline(ifs, line)){
        istringstream iss(line);
        if(!iss.good())
        {
            LogError("字符串解析失败!");
            ::exit(0);
        }
        string word,freq;
        iss >> word >> freq;
        insert_cn(word, atoi(freq.data()));
    }
    LogInfo("字典树构造成功!");
    ifs.close();
}

void construct_trie_en(const char* filename_en)
{
    ifstream ifs(filename_en);
    if(!ifs.good())
    {
        ifs.close();
        LogError("文件打开失败!");
        ::exit(0);
    }
    LogInfo("开始构造字典树!");

    string line;
    while(getline(ifs, line)){
        istringstream iss(line);
        if(!iss.good())
        {
            LogError("字符串解析失败!");
            ::exit(0);
        }
        string word,freq;
        iss >> word >> freq;
        insert_en(word, atoi(freq.data()));
    }//end of while
    LogInfo("字典树构造成功!");
    ifs.close();
}

void insert_cn(const string & word, int freq)
{
    link_type_cn cur = &_root_cn;
    for(int i = 0; i< word.size();)
    {
        string sub_str = word.substr(i, CH_SIZE);
        auto iter = cur->child.find(sub_str);
        if(iter == cur->child.end()){
            link_type_cn tmp_node = new node_type_cn;
            cur->child.insert(pair<string, link_type_cn>(sub_str, tmp_node));
            cur = tmp_node;
            
        }else{
            cur = iter->second;
        }
        i += CH_SIZE;
    }
    cur->count = freq;
}

void addFreq_en(const string& word)
{
    link_type_en cur = &_root_en;
    for(int i = 0; i < word.length(); ++i)
    {
        if(cur->child[word[i] - 'a']){
            cur = cur->child[word[i] - 'a'];
        }else {
            LogError(word + " dosn't exit!");
        }
    }
    cur->count++;
}

void add_Freq_cn(const string& word)
{
    link_type_en cur = &_root_en;
    for(int i = 0; i < word.length(); ++i)
    {
        if(cur->child[word[i] - 'a']){
            cur = cur->child[word[i] - 'a'];
        }else {
            LogError(word + " dosn't exit!");
        }
    } 
    cur->count++;
}

void insert_en(const string & word, int freq)
{
    link_type_en cur = &_root_en;
    for(int i = 0; i < word.length(); ++i)
    {
        if(!cur->child[word[i] - 'a'])
        {
            cur->child[word[i] - 'a'] = new node_type_en;
        }
        cur = cur->child[word[i] - 'a'];
    }
    cur->count = freq;
    cur->str = word;
}

int find_word_cn(const string & word)
{
    link_type_cn cur = &_root_cn;
    for(int i = 0; i < word.size();)
    {
        string sub_str = word.substr(i, CH_SIZE);
        auto iter = cur->child.find(sub_str);
        if(iter != cur->child.end())
        {
            cur = iter->second; 
        }else{
            LogError(word + "dosn't exit!");
            return 0;
        }
        i += CH_SIZE;
    }
    return cur->count;
}

int find_word_en(const string & word)
{
    link_type_en cur = &_root_en;
    for(int i = 0; i < word.length(); ++i)
    {
        if(cur->child[word[i] - 'a']){
            cur = cur->child[word[i] - 'a'];
        }else {
            LogError(word + " dosn't exit!");
            return 0;
        }
    }
    return cur->count;
}

vector<string> get_str_cn_pre(string word)
{
    vector<string> ret;
    link_type_cn cur = &_root_cn;
    for(int i = 0; i < word.size();)
    {
        string sub_str = word.substr(i, CH_SIZE);
        auto iter = cur->child.find(sub_str);
        if(iter != cur->child.end())
        {
            cur = iter->second; 
        }else{
            LogError(word + " dosn't exit!");
            return ret;
        }
        i += CH_SIZE;
    }
    add_str_cn(cur, word, ret);
    LogInfo("get_str_pre succeed!");
    return ret;
}

vector<string> get_str_en_pre(string str)
{
    vector<string> ret;
    link_type_en cur = &_root_en;
    for(int i = 0; i < str.length(); ++i){
        if(cur->child[str[i] - 'a']){
            cur = cur->child[str[i] - 'a'];
        }else {
            LogError("word dosn't exit!");
            return ret;
        }
    }
    add_str_en(cur, ret);
    LogInfo("get_str_pre succeed!");
    return ret;
}

void add_str_cn(link_type_cn root,string word,  vector<string>& ret)
{
    for(auto node : root->child)
    {
        add_str_cn(node.second, word + node.first, ret);
    }
    if(root->count != 0)
    {
        ret.push_back(word);
    }
}

void add_str_en(link_type_en root, vector<string>& ret)
{
    for(auto node : root->child)
    {
       if(node)
       {
            if(node->count != 0)
                ret.push_back(node->str);
            add_str_en(node,ret);
       }
    }
}


private:
    node_type_en _root_en;
    node_type_cn _root_cn;
};

}


