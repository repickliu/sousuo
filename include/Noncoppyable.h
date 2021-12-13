#pragma once

namespace wd
{
class Noncoppyable
{
public:
    Noncoppyable() {};
    ~Noncoppyable() {};

    Noncoppyable(const Noncoppyable &) = delete ;
    Noncoppyable & operator=(const Noncoppyable &) = delete; 
};
}

