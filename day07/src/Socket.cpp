#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "ConstantDefine.h"


Socket::Socket():fd(INVALID_SOCKET)
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == INVALID_SOCKET, "Socket::Socket() socket create error");
}

Socket::Socket(int _fd):fd(_fd)
{
    errif(fd == INVALID_SOCKET, "Socket::Socket(int) socket create error");
}

Socket::~Socket()
{
    if(fd != INVALID_SOCKET)
    {
        close(fd);
        fd = INVALID_SOCKET;
    }
}

void Socket::bind(InetAddress *addr)
{
    int bind_state = ::bind(fd, (sockaddr*)&addr->addr, addr->addr_len);
    errif(bind_state == -1, "Socket::bind(InetAddress) socket bind error");
}

void Socket::listen()
{
    int listen_state = ::listen(fd, SOMAXCONN);
    errif(listen_state == -1, "Socket::listen() socket listen error");
}

void Socket::setnonblocking()
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress *addr)
{
    int clnt_sockfd = ::accept(fd, (sockaddr*)&addr->addr, &addr->addr_len);
    errif(clnt_sockfd == -1, "Socket::accept() socket accept error");
    return clnt_sockfd;
}

int Socket::getFd()
{
    return fd;
}