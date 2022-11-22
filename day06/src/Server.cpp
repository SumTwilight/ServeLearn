#include <functional>
#include <string.h>
#include <unistd.h>
#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"

#include "ConstantDefine.h"
#include "BaseType.h" 

Server::Server(EventLoop *_loop): loop(_loop)
{
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();
    serv_sock->setnonblocking();

    Channel *servChannel = new Channel(loop, serv_sock->getFd());
    std::function<void()> cb = std::bind(&Server::newConnection, this, serv_sock);
    servChannel->setCallback(cb);
    servChannel->enableReading();

}

Server::~Server()
{
    SAFE_DELETE(loop);
}

void Server::handleReadEvent(int sockfd)
{
    char buf[READ_BUFFER_SIZE];
    while(true)  // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完成。
    {
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, &buf, sizeof(buf));
        if(bytes_read > 0)
        {
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        }
        else if(bytes_read == INVALID_SOCKET && errno == EINTR)
        {
            printf("continue reading/n");
            continue;
        }
        else if(bytes_read == INVALID_SOCKET && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        {
            printf("finish reading once, errno: %d/n", errno);
            break;
        }
        else if(bytes_read == 0) // EOF, 客户端断开连接
        {
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd); // 关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
}

void Server::newConnection(Socket *serv_sock)
{
    InetAddress *clnt_addr = new InetAddress(); //会发生内存泄漏！ 没有delete
    Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr)); // 会发生内存泄漏
    printf("new connection fd %d ! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
    clnt_sock->setnonblocking();
    Channel *clntChannel = new Channel(loop, clnt_sock->getFd());
    std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clnt_sock->getFd());
    clntChannel->setCallback(cb);
    clntChannel->enableReading();
}