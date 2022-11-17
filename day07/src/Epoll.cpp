#include "Epoll.h"
#include "util.h"
#include "Channel.h"
#include <unistd.h>
#include <string.h>

#include "ConstantDefine.h"
#include "BaseType.h"




Epoll::Epoll() : epfd(INVALID_SOCKET), events(nullptr)
{
    epfd = epoll_create1(0);
    errif(epfd == INVALID_SOCKET, "Epoll::Epoll() epoll create error");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(*events) * MAX_EVENTS);
}

Epoll::~Epoll()
{
    if(epfd != INVALID_SOCKET)
    {
        close(epfd);
        epfd = INVALID_SOCKET;
    }
    SAFE_DELETE_ARRAY(events);
}

void Epoll::addFd(int fd, uint32_t op)
{
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events = op;
    int ctl_state = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    errif(ctl_state == -1, "Epoll::addFd(int, uint32_t)  epoll add event error");
}

std::vector<Channel*> Epoll::poll(int timeout)
{
    std::vector<Channel*> activeChannels;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errif(nfds == -1, "Epoll::poll(int) epoll wait error");
    for(int i = 0; i < nfds; ++i)
    {
        Channel *ch = (Channel*)events[i].data.ptr;
        ch->setRevents(events[i].events);
        activeChannels.push_back(ch);
    }
    return activeChannels;
}

void Epoll::updateChannel(Channel *channel)
{
    int fd = channel->getFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->getEvents();
    if( !channel->getInEpoll() )
    {
        int ctl_state = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
        errif(ctl_state == -1, "Epoll::updateChannel() epoll add error");
        channel->setInEpoll();
    }
    else
    {
        int ctl_state = epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
    }
}