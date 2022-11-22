/*************************************************************************
    > File Name: client.cpp
    > Author: zhuliang
    > Mail: ncutzl@qq.com 
    > Created Time: Sun 06 Nov 2022 11:51:32 PM PST
 ************************************************************************/
#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "src/util.h"

#include "src/ConstantDefine.h"


int main(void){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    int connect_state = connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    errif(connect_state == -1, "socket connect error");
    
    printf("My socket is %d\n", sockfd);
    while(true)
    {
        char buf[BUFFER_SIZE];
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        if(write_bytes == -1)
        {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if(read_bytes > 0)
        {
            printf("message from server: %s\n",buf);
        }
        else if(read_bytes == 0)
        {
            printf("server socket disconnected!\n");
            break;
        }
        else if(read_bytes == -1)
        {
            close(sockfd);
            errif(true, "socket read error");
        }
    }
    close(sockfd);

    return 0;
}
