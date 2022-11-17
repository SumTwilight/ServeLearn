/*************************************************************************
    > File Name: util.cpp
    > Author: zhuliang
    > Mail: ncutzl@qq.com 
    > Created Time: Sun 06 Nov 2022 11:07:58 PM PST
 ************************************************************************/
#include "util.h"
#include "ConstantDefine.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void errif(bool condition, const char *_errmsg)
{
    if( condition )
    {
        char errmsg[BUFFER_SIZE];
        bzero(errmsg, sizeof(errmsg));
        sprintf(errmsg, "ERROR INFO: %s", _errmsg);
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}

void infoif(bool condition, const char *_infomsg)
{
    if( condition )
    {
        char infomsg[BUFFER_SIZE];
        bzero(infomsg, sizeof(infomsg));
        sprintf(infomsg, "NOTICE INFO: %s\n", _infomsg);
        printf("%s", infomsg);
        exit(EXIT_FAILURE);
    }
}
