/*************************************************************************
    > File Name: server.cpp
    > Author: zhuliang
    > Mail: ncutzl@qq.com 
    > Created Time: Sun 06 Nov 2022 11:21:27 PM PST
 ************************************************************************/
#include "src/EventLoop.h"
#include "src/Server.h"

int main(void){
    EventLoop *loop = new EventLoop();
    Server *server = new Server(loop);
    loop->loop();

    return 0;
}
