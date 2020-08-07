/*
 * select.c
 *
 *  Created on: 2015Äê10ÔÂ13ÈÕ
 *      Author: zqf
 */

#include <os.h>
#include "board-config.h"
#include <sys/select.h>
#include <sys/socket.h>

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


s32 select(s32 maxfd, fd_set *reads,fd_set *writes, fd_set *exps, \
           struct timeval *timeout)
{
    s32 mode = 0;
    s32 i = 0;
    s32 fd = -1;
    s32 result = 0;
    s32 waittime = 0;
    s32 forever = 0;


    if(NULL != reads)
    {
        reads->mode |= CN_SOCKET_IOREAD;
        mode |= CN_SOCKET_IOREAD;
    }
    if(NULL != writes)
    {
        writes->mode |= CN_SOCKET_IOWRITE;
        mode |= CN_SOCKET_IOWRITE;
    }
    if(NULL != exps)
    {
        exps->mode |= CN_SOCKET_IOERR;
        mode |= CN_SOCKET_IOERR;
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
                    if(issocketactive(fd,mode))
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
                    if(issocketactive(fd,mode))
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
                    if(issocketactive(fd,mode))
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
                DJY_EventDelay(CN_CFG_TICK_US);
                waittime -= (CN_CFG_TICK_US/1000);
            }
            else
            {
                break;
            }
        }
        else
        {
            //DJY_EventDelay(1*mS);
            DJY_EventDelay(CN_CFG_TICK_US);
        }

    }
    return result;
}

s32 FD_ISSET(s32 fd, fd_set *sets)
{
    s32 mode;
    s32 result = 0;

    if(NULL != sets)
    {
        mode = sets->mode;
        result = issocketactive(fd,mode);
    }

    return result;
}

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

