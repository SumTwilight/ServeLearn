#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include <vector>

#include "ConstantDefine.h"
#include "BaseType.h"

EventLoop::EventLoop(): ep(nullptr), quit(false)
{
    ep = new Epoll();
}

EventLoop::~EventLoop()
{
    SAFE_DELETE(ep);    
}

void EventLoop::loop()
{
    while(!quit)
    {
        std::vector<Channel*> chs;
        chs = ep->poll();
        for(auto it = chs.begin(); it != chs.end(); ++it)
        {
            (*it)->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel *ch)
{
    ep->updateChannel(ch);
}
