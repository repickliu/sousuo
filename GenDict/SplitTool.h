#pragma once
#ifndef _WD_SplitTool_H__
#define _WD_SplitTool_H__

#include <vector>
#include <string>

using namespace std;

namespace wd{

class SplitTool
{
public:
    SplitTool();
    virtual ~SplitTool();

    virtual vector<string> cut(const string & sentence) = 0;

};

}

#endif

