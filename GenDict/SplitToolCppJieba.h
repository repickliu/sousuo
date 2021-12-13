#pragma once
#ifndef _WD_SplitToolCppJieba_H__
#define _WD_SplitToolCppJieba_H__


#include "cppjieba/Jieba.hpp"

#include <iostream>

using namespace std;



namespace wd{

class SplitToolCppJieba
{
const char * DICT_PATH = "../../dev_download/cppjieba/dict/jieba.dict.utf8";
const char * HMM_PATH = "../../dev_download/cppjieba/dict/hmm_model.utf8";
const char * USER_DICT_PATH = "../../dev_download/cppjieba/dict/user.dict.utf8";
const char * IDF_PATH = "../../dev_download/cppjieba/dict/idf.utf8";
const char * STOP_WORD_PATH = "../../dev_download/cppjieba/dict/stop_words.utf8";
public:
    SplitToolCppJieba()
    : _jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH)
    {
        cout << "cppjieba init" << endl;
    }

    vector<string> cut(const string str)
    {
        vector<string> words;
        _jieba.Cut(str, words, true);
        return words;
    }

private:
    cppjieba::Jieba _jieba;
};

}

#endif

