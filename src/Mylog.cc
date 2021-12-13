#include "../include/Mylog.h"
#include "../include/Config.h"

#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PatternLayout.hh>

using std::cout;

namespace wd{

Mylog * Mylog::_pInstance = NULL;

Mylog * Mylog::getInstance()
{
    if(_pInstance == nullptr)
    {
        _pInstance = new Mylog();
    }
    return _pInstance;
}

void Mylog::destroy()
{
    if(_pInstance)
    {
        delete _pInstance;
    }
}

void Mylog::warn(const char * msg)
{
    _root.warn(msg);
}

void Mylog::info(const char * msg)
{
    _root.info(msg);
}

void Mylog::error(const char * msg)
{
    _root.error(msg);
}

void Mylog::debug(const char * msg)
{
    _root.debug(msg);
}

Mylog::Mylog()
: _root(Category::getRoot().getInstance("root"))
{
    PatternLayout * ptnLyout1 = new PatternLayout();
    ptnLyout1->setConversionPattern("%d [%d] %m%n");
    PatternLayout * ptnLyout2 = new PatternLayout();
    ptnLyout2->setConversionPattern("%d [%d] %m%n");

    OstreamAppender * ostreamAppender = new OstreamAppender("ostreamAppender", &cout);
	ostreamAppender->setLayout(ptnLyout1);

    FileAppender * fileAppender = new FileAppender("fileAppender","../log/server.log" );
    fileAppender->setLayout(ptnLyout2);

    _root.addAppender(ostreamAppender);
    _root.addAppender(fileAppender);
    _root.setPriority(Priority::DEBUG);
}

Mylog::~Mylog()
{
    Category::shutdown();
}
}//end of wd


