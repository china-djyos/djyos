//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//author:zhangqf
//date  :下午4:59:55/2017年1月4日
//usage :
//------------------------------------------------------

//standard includes
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <os.h>
//add your own specified header here
#include <driver.h>
#include <shell.h>
#include "iodev.h"


typedef struct
{
	u8  idebug;
	u8  odebug;
}tagIoDebug;
static tagIoDebug  gIoDebug = {
		.idebug = 0,
		.odebug = 0,
};
//-----------------------------------------------------------------------------
//功能:weuse,0xthis,0xfunction,0xto open the serial device
//参数:
//返回:NULL open failed else the device handle
//备注:
//作者:zhangqf@下午4:55:51/2017年1月5日
//-----------------------------------------------------------------------------
ptu32_t iodevopen(const char *name)
{
	ptu32_t result = 0;
	result = (ptu32_t)Driver_OpenDevice(name,O_RDWR,0);
	return result;
}
//#define CN_UART_RT_TIMEOUT (10*mS)
//-----------------------------------------------------------------------------
//功能:we use this function to read data from the serial device
//参数:dev,device handle;buf,data buf to read;len,data len
//返回:-1 if the read abort others  the read len
//备注:do the block read
//作者:zhangqf@下午4:57:17/2017年1月5日
//-----------------------------------------------------------------------------
int iodevread(ptu32_t dev,u8 *buf,u32 buflen,u32 timeout)
{
	int len = -1;
	if(NULL == (void *)dev)
	{
		return len;
	}
	len = Driver_ReadDevice((struct DjyDevice *)dev,buf,buflen,0,timeout);
	if((len > 0)&&(gIoDebug.idebug > 0))
	{
		time_t printtime;
		printtime = time(NULL);
		printf("[RCV:%s:%d bytes]",ctime(&printtime),len);
		for(int i = 0;i < len;i++)
		{
			if(gIoDebug.idebug == 1)
			{
				printf("%c",buf[i]);
			}
			else if(gIoDebug.idebug ==2)
			{
				printf("%02x ",buf[i]);
			}
		}
		printf("\n\r");
	}

	return len;
}
//-----------------------------------------------------------------------------
//功能:we use this function to write data to the serial device
//参数:dev,device handle;buf,data buf to write;len,data len
//返回:
//备注:do the block send
//作者:zhangqf@下午4:57:17/2017年1月5日
//-----------------------------------------------------------------------------
int iodevwrite(ptu32_t dev,u8 *buf,u32 len,u32 timeout)
{
	int result = -1;
	int sentlen;
	sentlen  =0;

	if(NULL == (void *)dev)
	{
		return result;
	}
	while(sentlen != len)
	{
		result = Driver_WriteDevice((struct DjyDevice *)dev,buf+sentlen,len-sentlen,0,CN_BLOCK_BUFFER,timeout);
		if(result > 0)
		{
			sentlen += result;
		}
		else
		{
			break;
		}
	}
	result = sentlen;

	if(gIoDebug.odebug > 0)
	{
		time_t printtime;
		printtime = time(NULL);
		printf("[SND:%s:%d bytes]",ctime(&printtime),len);
		for(int i = 0;i < len;i++)
		{
			if(gIoDebug.odebug == 1)
			{
				printf("%c",buf[i]);
			}
			else if(gIoDebug.odebug ==2)
			{
				printf("%02x ",buf[i]);
			}
		}
		printf("\n\r");
	}
	return result;
}

//-----------------------------------------------------------------------------
//功能:we use this function to flush the device,read all the exited data
//参数:dev,device handle;buf,data buf to write;len,data len
//返回:
//备注:do the block send
//作者:zhangqf@下午4:57:17/2017年1月5日
//-----------------------------------------------------------------------------
void iodevflush(ptu32_t dev)
{
	u8 buf[8];
	int len;
	while((len =iodevread(dev,buf,8,0))>0);
	return;
}


//-----------------------------------------------------------------------------
//功能:use this function to close the sio device
//参数:
//返回:
//备注:
//作者:zhangqf@下午5:05:27/2017年1月5日
//-----------------------------------------------------------------------------
bool_t iodevclose(ptu32_t dev)
{
	bool_t result;
	result = Driver_CloseDevice((struct DjyDevice *)dev);
	return result;
}
//usage:this function to set the sdev debug mode
bool_t iodebugset(char *param)
{
	char *argv[2];
	int   argc=2;
	int   v1;

	string2arg(&argc,argv,param);
	if(argc == 2)
	{
		v1 = strtol(argv[0],NULL,NULL);
		if(v1 == 0)
		{
			gIoDebug.idebug = strtol(argv[1],NULL,NULL);
		}
		else
		{
			gIoDebug.odebug = strtol(argv[1],NULL,NULL);
		}
	}
	return true;
}

struct ShellCmdTab  gIoDebugItem[] =
{
    {
        "iodebug",
		iodebugset,
        "usage:iodebug type[0/rcv 1/snd] mode[0/nodebug 1/ascii 2/hex]",
        "usage:iodebug type[0/rcv 1/snd] mode[0/nodebug 1/ascii 2/hex]",
    },
};
#define CN_IoDebug_NUM  ((sizeof(gIoDebugItem))/(sizeof(struct ShellCmdTab)))
static struct ShellCmdRsc gIoDebugCmdRsc[CN_IoDebug_NUM];
//-----------------------------------------------------------------------------
//功能:this is the ppp main function here
//参数:
//返回:
//备注:
//作者:zhangqf@下午4:06:57/2017年1月5日
//-----------------------------------------------------------------------------
bool_t PppIoInit(ptu32_t para)
{
	bool_t result;
	result = Sh_InstallCmd(gIoDebugItem,gIoDebugCmdRsc,CN_IoDebug_NUM);
	return result;
}



