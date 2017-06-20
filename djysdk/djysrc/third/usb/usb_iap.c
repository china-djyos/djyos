//-----------------------------------------------------------------------------
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

#include <stdio.h>
#include <string.h>
#include <djyos.h>
#include <wdt_soft.h>

#define RW_SIZE						512
#define APPLICATION_FILE_NAME		"PRS-7573.bin"

typedef enum{
	INIT = 0,
	DONE,
	ERROR
}USB_IAP_STATUS;

static USB_IAP_STATUS gRunningStatus;

//-----------------------------------------------------------------------------
//功能: 灯亮
//参数:
//返回:
//备注: 由驱动实现
//-----------------------------------------------------------------------------
__attribute__((weak)) void USB_IAP_TurnOnLED(void)
{
	;
}

//-----------------------------------------------------------------------------
//功能: 灯灭
//参数:
//返回:
//备注: 由驱动实现
//-----------------------------------------------------------------------------
__attribute__((weak)) void USB_IAP_TurnOffLED(void)
{
	;
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注: 函数500微秒被调用一次
//-----------------------------------------------------------------------------
u32 USB_IAP_StatusDeamon(struct Wdt *reg)
{
	static u8 toggle;

	switch(gRunningStatus)
	{
	case INIT:
			// 500微秒周期进行亮灭
			if(toggle)
			{
				toggle = 0;
				USB_IAP_TurnOnLED();
			}
			else
			{
				toggle = 1;
				USB_IAP_TurnOffLED();
			}

			break;

	case DONE:
			// 常亮
			USB_IAP_TurnOnLED();
			break;

	case ERROR:
			// 一长两短的亮灯
			if(toggle < 4) USB_IAP_TurnOnLED();
			else if(toggle < 5) USB_IAP_TurnOffLED();
			else if(toggle < 6) USB_IAP_TurnOnLED();
			else if(toggle < 7) USB_IAP_TurnOffLED();
			else if(toggle < 8) USB_IAP_TurnOnLED();
			else if(toggle < 9) USB_IAP_TurnOffLED();
			else if(toggle < 10) toggle = 0;

			toggle++;

			break;
	default:
		break;
	}

	return (0);
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
void USB_IAP_StatusInit(void)
{

	if(!Wdt_Create("usb iap status", 500000, USB_IAP_StatusDeamon, EN_EXP_DEAL_IGNORE, 0, 0))
	{
		printf("\r\nIAP: USB: status initial failed\r\n");
		gRunningStatus = ERROR;
		do
		{
			USB_IAP_StatusDeamon(NULL);

		}while(1);
	}

	gRunningStatus = INIT;
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static u32 USB_IAP_Thread(void)
{
	DIR *dir;
	struct dirent *structDirent;
	char *path;
	char buffer[RW_SIZE];
	s32 res;
	struct stat sourceInfo;
	u8 found = 0;
	s32 source = -1;
	s32 destination = -1;
	char *program = APPLICATION_FILE_NAME;
	extern s32 MSC_DeviceReady(u8 bID);

	do{
		if(!MSC_DeviceReady(0))
			break; // USB设备已准备好
		Djy_EventDelay(1000000); // 1s跑一次
	}while(1);


	USB_IAP_StatusInit();

	dir = opendir("/fat");
	if(!dir)
	{
		printf("IAP: USB: directory \"fat\" is not exist! maybe \"FAT\" is not installed!\r\n");
		gRunningStatus = ERROR;
		return (-1);
	}

	while((structDirent = readdir(dir)) != NULL)
	{
		if(0 == strcmp(structDirent->d_name, program))
		{
			printf("IAP: USB: file \"%s\" will be programmed.\r\n",
					structDirent->d_name);
			found = 1;
			break;
		}
	}

	if(!found)
	{
		printf("IAP: USB: file \"%s\" is not found.\r\n", program);
		gRunningStatus = ERROR;
		goto END;
	}

	path = buffer;
	strcpy(path, "/fat/");
	strcpy(path+5, structDirent->d_name);
	source = open(path, O_RDONLY);
	if(-1 == source)
	{
		printf("IAP: USB: cannot open source file \"%s\".\r\n", path);
		gRunningStatus = ERROR;
		goto END;
	}

	res = fstat(source, &sourceInfo);
	if(res)
	{
		printf("IAP: USB: cannot statistics source file \"%s\".\r\n", path);
		gRunningStatus = ERROR;
		goto END;
	}

	strcpy(path, "/iboot/");
	strcpy(path+7, structDirent->d_name);
	destination = open(path, O_RDWR | O_CREAT);
	if(-1 == destination)
	{
		printf("IAP: USB: cannot open destination file \"%s\"!\r\n", path);
		gRunningStatus = ERROR;
		goto END;
	}

	while(1)
	{
		res = read(source, buffer, RW_SIZE);
		if(!res)
		{
			printf("IAP: USB: read source file error.\r\n");
			gRunningStatus = ERROR;
			break; // 没有数据可读
		}

		sourceInfo.st_size -= res; // 剩余

		if(res != write(destination, buffer, res))
		{
			printf("IAP: USB: write destination file error.\r\n");
			gRunningStatus = ERROR;
			break;
		}

		if(!sourceInfo.st_size)
		{
			printf("IAP: USB: application update succeed.\r\n");
			gRunningStatus = DONE;
			break; // 全部读完
		}
	}

END:
	closedir(dir);
	printf("IAP: USB: thread exit ...\r\n");
	if(-1 != source)
	{
		res = close(source);
		if(res)
		{
			printf("IAP: USB: close source file failed.\r\n");
		}
	}

	if(-1 != destination)
	{
		res = close(destination);
		if(res)
		{
			printf("IAP: USB: close destination file failed.\r\n");
		}
	}

	return (0);
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回: "0" -- 成功; "-1" -- 失败;
//备注:
//-----------------------------------------------------------------------------
s32 ModuleInstall_USB_IAP(void)
{
	u16 thread;

	thread = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0,
					USB_IAP_Thread, NULL, 0x1000, "USB IAP service");
	if(thread != CN_EVTT_ID_INVALID)
	{
		Djy_EventPop(thread, NULL, 0, 0, 0, 0);
		return (0);
	}

	return (-1);
}
