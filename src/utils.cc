#include "../include/utils.h"
#include <iostream>

#include <vector>

using std::vector;

utils* utils::_Instance = nullptr;
utils* utils::gettInstance()
{
    if(_Instance == nullptr)
    {
        _Instance = new utils();
    }
    return _Instance;
}

void utils::destory()
{
    if(_Instance != nullptr)
    {
        delete (_Instance);
    }
}

bool utils::isCh(const char * str)
{
    if(*(str++) & 0x80)
        if(*(str) & 0x80)
            return true;
    return false;
}


int utils::triple_min(const int &a, const int &b, const int &c)
{
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int utils::EdictLenth(string lhs, string rhs)
{
    int lhs_len = isCh(lhs.data()) ? lhs.length()/3 : lhs.length();
    int rhs_len = isCh(rhs.data()) ? rhs.length()/3 : rhs.length();
    int editDist[lhs_len + 1][rhs_len + 1];
    for(std::size_t idx = 0; idx <= lhs_len; ++idx)
    {
        editDist[idx][0] = idx;

    }

    for(std::size_t idx = 0; idx <= rhs_len; ++idx)
    {
        editDist[0][idx] = idx;

    }

    int dist_i = 0, dist_j = 0;
    std::string sublhs, subrhs;
    for(int i = 1; i <= lhs_len; ++i)//lhs_idx<=blhs_len一定要加上，防止substr处理越界，自己调试几下就清楚了
    {
        sublhs = isCh(lhs.data()) ? lhs.substr(dist_i, 3) : lhs.substr(dist_i, 1);
        for(int j = 1; j <= rhs_len; ++j)
        {
            subrhs = isCh(rhs.data()) ? rhs.substr(dist_j, 3) : rhs.substr(dist_j, 1);
            if(sublhs == subrhs)
            {
                editDist[i][j] = editDist[i - 1][j - 1];

            }
            else
            {
                editDist[i][j] = triple_min(
                            editDist[i][j - 1] + 1,
                            editDist[i - 1][j] + 1,
                            editDist[i - 1][j - 1] + 1
                           );
            }
            dist_j = isCh(rhs.data()) ? (dist_j + 3) : (dist_j + 1);
        }
        dist_j = 0;
        dist_i = isCh(lhs.data()) ? (dist_i + 3) : (dist_i + 1);
    }
    return editDist[lhs_len][rhs_len];
}

utils::utils()
{
}
