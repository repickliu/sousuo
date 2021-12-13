#include "../include/Mydict.h"



namespace wd{
Mydict* Mydict::_pInstance = NULL;

void Mydict::Delete()
{
    delete _pInstance;
}

}


