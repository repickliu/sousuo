#include "../include/SpellCorrectServer.h"

using namespace std;

void server()
{
    wd::SpellCorrectServer server;
    server.start();
}

int main()
{
    server();
    return 0;
}

