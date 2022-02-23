/*
 * select.c
 *
 *  Created on: 2015年10月13日
 *      Author: zqf
 */

#include <os.h>
#include "board-config.h"
#include <sys/select.h>
#include <sys/socket.h>

//-----------------------------------------------------------------------------
//功能：在 Sets 中添加 fd 条目
//参数：fd，待操作的 fd
//      sets，fd所在的 sets
//返回：1=成功添加，0=sets 中没有空位
//------------------------------------------------------------------------------
s32 FD_SET(s32 fd, fd_set *sets)
{
    s32 i = 0;
    s32 result = 0;
    if(NULL != sets)
    {
        for(i =0;i<FD_SETSIZE;i++)
        {
            if(sets->fd[i] == CN_INVALID_FD)
            {
                sets->fd[i] = fd;
                result = 1;
                break;
            }
        }
    }

    return result;
}

//-----------------------------------------------------------------------------
//功能：清除 Sets 中的 fd 条目，设为非法ID即可
//参数：fd，待操作的 fd
//      sets，fd所在的 sets
//返回：1=成功清除，0=sets 中没有找到fd条目
//------------------------------------------------------------------------------
s32 FD_CLR(s32 fd, fd_set *sets)
{

    s32 i = 0;
    s32 result = 0;
    if(NULL != sets)
    {
        for(i =0;i<FD_SETSIZE;i++)
        {
            if(sets->fd[i] == fd)
            {
                sets->fd[i] = CN_INVALID_FD;
                result = 1;
            }
        }
    }

    return result;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

s32 select(s32 maxfd, fd_set *reads,fd_set *writes, fd_set *exps, \
           struct timeval *timeout)
{
//    s32 mode = 0;
    s32 i = 0;
    s32 fd = -1;
    s32 result = 0;
    s32 waittime = 0;
    s32 forever = 0;


    if(NULL != reads)
    {
        reads->mode = CN_SOCKET_IOREAD;
//      mode |= CN_SOCKET_IOREAD;
    }
    if(NULL != writes)
    {
        writes->mode = CN_SOCKET_IOWRITE;
//      mode |= CN_SOCKET_IOWRITE;
    }
    if(NULL != exps)
    {
        exps->mode = CN_SOCKET_IOERR;
//      mode |= CN_SOCKET_IOERR;
    }

    if(NULL != timeout)
    {
        waittime = timeout->tv_sec*1000 + timeout->tv_usec/1000;
    }
    else
    {
        //waittime = CN_SELECT_TIMEDEFAULT;
        forever = 1;
    }

    while(result == 0)
    {
        if(NULL != reads)
        {
            for(i = 0;i < FD_SETSIZE;i++)
            {
                fd = reads->fd[i];
                if(CN_INVALID_FD != fd)
                {
                    if(issocketactive(fd,CN_SOCKET_IOREAD))
                    {
                        result++;
                    }
                }
            }
        }
        if(NULL != writes)
        {
            for(i = 0;i < FD_SETSIZE;i++)
            {
                fd = writes->fd[i];
                if(-1 != fd)
                {
                    if(issocketactive(fd,CN_SOCKET_IOWRITE))
                    {
                        result++;
                    }
                }
            }
        }
        if(NULL != exps)
        {
            for(i = 0;i < FD_SETSIZE;i++)
            {
                fd = exps->fd[i];
                if(-1 != fd)
                {
                    if(issocketactive(fd,CN_SOCKET_IOERR))
                    {
                        result++;
                    }
                }
            }
        }

        if(forever == 0)
        {
            if(waittime > 0)//do the delay
            {
//                DJY_EventDelay(1*mS);
//                waittime--;
                DJY_EventDelay(10*mS);
                waittime -= 10;
            }
            else
            {
                break;
            }
        }
        else
        {
            //DJY_EventDelay(1*mS);
            DJY_EventDelay(10*mS);
        }

    }
    return result;
}
#pragma GCC diagnostic pop

//-----------------------------------------------------------------------------
//功能：判断 Sets 中的 fd 是否已经激活，首先查看 fd 是否在sets中，在的话判断是否激活
//参数：fd，待判断的 fd
//      sets，fd所在的 sets
//返回：1=actived，0=unactive
//------------------------------------------------------------------------------
s32 FD_ISSET(s32 fd, fd_set *sets)
{
    s32 i;
    s32 result = 0;

    if(NULL != sets)
    {
        for(i =0;i<FD_SETSIZE;i++)
        {
            if(sets->fd[i] == fd)
            {
                result = issocketactive(fd,sets->mode);
                break;
            }
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
//功能：清空 Sets 中的 fd 表，全部设为非法ID即可
//参数：sets，待操作的 sets
//返回：1
//------------------------------------------------------------------------------
s32 FD_ZERO(fd_set *sets)
{
    s32 result = 0;
    s32 i = 0;
    s32 fd = CN_INVALID_FD;

    if(NULL != sets)
    {
        sets->mode = 0;
        for (i =0;i < FD_SETSIZE ; i++)
        {
            sets->fd[i] = fd;
        }
        result = 1;
    }

    return result;;
}

