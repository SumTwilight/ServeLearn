/*************************************************************************
    > File Name: server.cpp
    > Author: zhuliang
    > Mail: ncutzl@qq.com 
    > Created Time: Sun 06 Nov 2022 11:21:27 PM PST
 ************************************************************************/

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <vector>
#include "Channel.h"
#include "util.h"
#include "Epoll.h"
#include "InetAddress.h"
#include "Socket.h"
#include "ConstantDefine.h"
#include "BaseType.h"

using namespace std;

void setnonblocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL)| O_NONBLOCK);
}

void handleReadEvent(int);

int main(void){
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();
    Epoll *ep = new Epoll();
    serv_sock->setnonblocking();
    Channel *servChannel = new Channel(ep, serv_sock->getFd());
    servChannel->enableReading();

    while (true)
    {
        std::vector<Channel*> activeChannels = ep->poll();
        int nfds = activeChannels.size();
        for(int i = 0; i < nfds; ++i)
        {
            int chfd = activeChannels[i]->getFd();
            if(chfd == serv_sock->getFd()) // new client connect.
            {
                InetAddress *clnt_addr = new InetAddress();  // delete ??
                Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr));
                printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
                clnt_sock->setnonblocking();
                Channel *clntChannel = new Channel(ep, clnt_sock->getFd());
                clntChannel->enableReading();

                SAFE_DELETE(clnt_addr);  // delete !!
            }
            else if(activeChannels[i]->getEvents() & EPOLLIN ) // readable
            {
                handleReadEvent(activeChannels[i]->getFd());
            }
            else
            {
                printf("something else happened\n");
            }
        }
    }
    SAFE_DELETE(serv_sock);
    SAFE_DELETE(serv_addr);
    return 0;
}


void handleReadEvent(int sockfd)
{
    char buf[BUFFER_SIZE];
    while (true)
    {
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0)
        {
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        }
        else if(bytes_read == -1 && errno == EINTR)  //客户端正常中断、继续读取
        {
            printf("continue reading");
            continue;
        }
        else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))  //非阻塞IO，这个条件表示数据全部读取完毕
        {
            printf("finish reading once, errno: %d\n", errno);
            break;
        }
        else
        {
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
    
}