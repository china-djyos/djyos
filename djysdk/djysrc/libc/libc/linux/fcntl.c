/*
 * fcntl.c
 *
 *  Created on: 2015Äê10ÔÂ15ÈÕ
 *      Author: zqf
 */
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <fcntl.h>
#include <stdio.h>

int fcntl(int fd,int cmd,...)
{
    va_list args;
    u32 opcode;
    u32 optvalue;
    int result = 0;
	if(cmd == F_SETFL)
	{
	    va_start (args, cmd);
        opcode = va_arg(args, u32);
	    va_end (args);
	    if(opcode&O_NONBLOCK)
	    {
	    	optvalue = 1;
	    	result = setsockopt(fd,SOL_SOCKET,SO_NOBLOCK,&optvalue,sizeof(optvalue));
	    }
	}
	return result;
}



off_t lseek(int fd,off_t offset, int whence)
{
	return 0;
}


