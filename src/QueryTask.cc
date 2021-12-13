#include "../include/QueryTask.h"
#include "../include/utils.h"

void QueryTask::process()
{
    cout << " resquesize = " << _queryWord.size() << " " << endl;
    if(_queryWord == "")
    {
        string msg = "you cin is null please cin again !!!";
        _conn->sendInLoop(msg);
        return;
    }
    string res = getFromRedis(_queryWord);
    if(res == ""){
        queryIndex(_queryWord);
        if(!_resultque.empty())
        {
            res = toJson();
            setToRedis(res);
        }else res = toJson();
    }
    cout << " json msg is " << res << endl;
    _conn->sendInLoop(res);
}

void QueryTask::setToRedis(const string& str)
{
    Redis* redis = Redis::getInstannce();
    redis->set(_queryWord, str);
}

string QueryTask::getFromRedis(const string& str)
{
    Redis* redis = Redis::getInstannce();
    return redis->get(str);
}

void QueryTask::queryIndex(const string & word)
{
    utils& util = *utils::gettInstance();
    if(isalpha(word[0])){
        vector<string> res = _dict.getDict().get_str_en_pre(word);
        for(auto iter : res)
        {
            _resultque.push(make_pair(iter, util.EdictLenth(word, iter)));
        }
    }else{
        vector<string> res = _dict.getDict().get_str_cn_pre(word);
        for(auto iter : res)
        {
            _resultque.push(make_pair(iter, util.EdictLenth(word, iter)));
        }
    }
}

string QueryTask::toJson()
{
    Json::Value root;
    Json::FastWriter writer;
    Json::Value word;
    size_t total = _resultque.size() >= _num ? _num : _resultque.size();

    while(total){
        string pend = _resultque.top().first;
        _resultque.pop();
        word = pend;
        root.append(word);
        --total;
    }

    string json_file = writer.write(root);

    return json_file;
}
