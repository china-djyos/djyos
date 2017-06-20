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

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <shell.h>
#include <stdlib.h>
#include <ctype.h>
#include "./stm32_usb_host_library/class/custom/inc/usbh_custom.h"
#include "usb.h"

//
//
//
#define SPACE_LENGTH					100
#define COMMAND_MAX						7

//
//
//
static USBH_HandleTypeDef  *pController;
static char sSpace[SPACE_LENGTH];

char *AT_COMMAND_TABLE[] =
{
	NULL,
	"AT +CGMI \r",
	"AT +CGMM \r",
	"ATE \r",
	"ATI \r",
	"AT +RESET\r",
	"AT ^RESET\r"
};

//
//
//
static bool_t SH_USB_Send_AT(char *Param);
static bool_t SH_USB_Reveive_AT(char *Param);
static bool_t SH_USB_Send_AT_Timeout(char *Param);
static bool_t SH_USB_Reveive_AT_Timeout(char *Param);
static bool_t SH_USB_Send_MODEM(char *Param);
static bool_t SH_USB_Reveive_MODEM(char *Param);
static bool_t SH_USB_Send_MODEM_Timeout(char *Param);
static bool_t SH_USB_Reveive_MODEM_Timeout(char *Param);
static bool_t SH_USB_Send_DEBUG(char *Param);
static bool_t SH_USB_Reveive_DEBUG(char *Param);
static bool_t SH_USB_Send_DEBUG_Timeout(char *Param);
static bool_t SH_USB_Reveive_DEBUG_Timeout(char *Param);
extern bool_t SH_USB_Log(char *pParam);
static bool_t SH_USB_Test(char *pParam);

//
//
//
static struct ShellCmdRsc *spCommandSpace;
static struct ShellCmdTab const sCommandTable[] =
{
    {
        "at-s",
        SH_USB_Send_AT,
        "for debug",
        "for debug"
    },
	{
		"at-st",
		SH_USB_Send_AT_Timeout,
		"for debug",
		"for debug"
	},
    {
        "at-r",
		SH_USB_Reveive_AT,
		"for debug",
		"for debug"
    },
	{
		"at-rt",
		SH_USB_Reveive_AT_Timeout,
		"for debug",
		"for debug"
	},
	{
		"modem-s",
		SH_USB_Send_MODEM,
		"for debug",
		"for debug"
	},
	{
		"modem-st",
		SH_USB_Send_MODEM_Timeout,
		"for debug",
		"for debug"
	},
	{
		"modem-r",
		SH_USB_Reveive_MODEM,
		"for debug",
		"for debug"
	},
	{
		"modem-rt",
		SH_USB_Reveive_MODEM_Timeout,
		"for debug",
		"for debug"
	},
	{
		"debug-s",
		SH_USB_Send_DEBUG,
		"for debug",
		"for debug"
	},
	{
		"debug-st",
		SH_USB_Send_DEBUG_Timeout,
		"for debug",
		"for debug"
	},
	{
		"debug-r",
		SH_USB_Reveive_DEBUG,
		"for debug",
		"for debug"
	},
	{
		"debug-rt",
		SH_USB_Reveive_DEBUG_Timeout,
		"for debug",
		"for debug"
	},
	{
		"diaggrab",
		SH_USB_Log,
		"for debug",
		"for debug"
	},
	{
		"utrace",
		SH_USB_Test,
		"for debug",
		"for debug"
	},
};

//-----------------------------------------------------------------------------
//功能: 
//参数:
//返回:
//备注: 
//-----------------------------------------------------------------------------
static bool_t SH_USB_Send_AT(char *pParam)
{		
	char *command;
	u32 len;
	u32 index;

	if(!pParam)
		return (FALSE);

	index = atoi(pParam);
	if((0 != index) && (index < COMMAND_MAX))
	{
		command = AT_COMMAND_TABLE[index];
	}
	else
	{
		len = strlen(pParam);
		memset(sSpace, 0x0, len);
		strcpy(sSpace, pParam);
		sSpace[len] = '\r';
		sSpace[len+1] = '\0';
		command = sSpace;
	}

	len = strlen((char*)command) + 1;

	if(USBH_CUSTOM_AT_Transmit(pController, (u8*)command, &len, -1))
        printf("AT channel command :\"%s\" error\r\n", command);

    return (TRUE);
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static bool_t SH_USB_Send_AT_Timeout(char *pParam)
{
	char *command;
	u32 len;
	u32 index;

	if(!pParam)
		return (FALSE);

	index = atoi(pParam);
	if((0 != index) && (index < COMMAND_MAX))
	{
		command = AT_COMMAND_TABLE[index];
	}
	else
	{
		len = strlen(pParam);
		memset(sSpace, 0x0, len);
		strcpy(sSpace, pParam);
		sSpace[len] = '\r';
		sSpace[len+1] = '\0';
		command = sSpace;
	}

	len = strlen(command) + 1;

	if(USBH_CUSTOM_AT_Transmit(pController, (u8*)command, &len, 30000)) // wait 30 seconds
        printf("AT channel commmand :\"%s\" error\r\n", command);
    return (TRUE);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static bool_t SH_USB_Reveive_AT(char *pParam)
{
	u32 len, left;
	u32 i;

	if(!pParam)
		len = SPACE_LENGTH; // default length
	else
		len = atoi(pParam);

	left = len;
	memset(sSpace, 0x0, SPACE_LENGTH);

	if(USBH_CUSTOM_AT_Receive(pController, (u8*)sSpace, &left, -1))
	{
		printf("AT channel receive timeout\r\n");
	}

	len -= left;
	if(len)
		printf("\r\nreceived %d bytes\r\n", len);

	for(i = 0; i < len; i++)
		printf("%c", sSpace[i]);

	return (TRUE);
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static bool_t SH_USB_Reveive_AT_Timeout(char *pParam)
{
	u32 len, left;
	u32 i;

	if(!pParam)
		len = SPACE_LENGTH; // default length
	else
		len = atoi(pParam);

	left = len;
	memset(sSpace, 0x0, SPACE_LENGTH);

	if(USBH_CUSTOM_AT_Receive(pController, (u8*)sSpace, &left, 30000))
	{
		printf("AT channel receive timeout\r\n");
	}

	len -= left;
	if(len)
		printf("\r\nreceived %d bytes\r\n", len);

	for(i = 0; i < len; i++)
		printf("%c", sSpace[i]);

	return (TRUE);
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static bool_t SH_USB_Send_MODEM(char *pParam)
{
	char *command;
	u32 len;
	u32 index;

	if(!pParam)
		return (FALSE);

	index = atoi(pParam);
	if((0 != index) && (index < COMMAND_MAX))
	{
		command = AT_COMMAND_TABLE[index];
	}
	else
	{
		len = strlen(pParam);
		memset(sSpace, 0x0, len);
		strcpy((char*)sSpace, pParam);
		sSpace[len] = '\r';
		sSpace[len+1] = '\0';
		command = sSpace;
	}

	len = strlen(command) + 1;

	if(USBH_CUSTOM_MODEM_Transmit(pController, (u8*)command, &len, -1))
		printf("MODEM channel commmand :\"%s\" error\r\n", command);

    return (TRUE);
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static bool_t SH_USB_Send_MODEM_Timeout(char *pParam)
{
	char *command;
	u32 len;
	u32 index;

	if(!pParam)
		return (FALSE);

	index = atoi(pParam);
	if((0 != index) && (index < COMMAND_MAX))
	{
		command = AT_COMMAND_TABLE[index];
	}
	else
	{
		len = strlen(pParam);
		memset(sSpace, 0x0, len);
		strcpy(sSpace, pParam);
		sSpace[len] = '\r';
		sSpace[len+1] = '\0';
		command = sSpace;
	}

	len = strlen(command) + 1;

	if(USBH_CUSTOM_MODEM_Transmit(pController, (u8*)command, &len, 30000))
		printf("MODEM channel commmand :\"%s\" error\r\n", command);
    return (TRUE);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
u8 gIntCtrl;
u32 gInts;
u64 gMax;
static bool_t SH_USB_Reveive_MODEM(char *pParam)
{
	u32 len, left;
	u32 i;
	s64 start, consume;

	if(!pParam)
		len = SPACE_LENGTH; // default length
	else
		len = atoi(pParam);

	left = len;
	memset(sSpace, 0x0, SPACE_LENGTH);

	if(gIntCtrl)
	{
		printf("\r\n%d ints happened\r\n", gInts);
		gInts = 0;
	}

	start = DjyGetSysTime();
	if(USBH_CUSTOM_MODEM_Receive(pController, (u8*)sSpace, &left, 0)) // wait forever
	consume = DjyGetSysTime() - start;
	{
		printf("MODEM channel receive timeout\r\n");
	}

	printf("\r\nmodem read cost %d us\r\n", (u32)consume);

	if(gIntCtrl)
		printf("\r\n%d ints happened during read\r\n", gInts);

	len -= left;
	if(len)
		printf("\r\nreceived %d bytes\r\n", len);

	for(i = 0; i < len; i++)
		printf("%c", sSpace[i]);

	return (TRUE);
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static bool_t SH_USB_Reveive_MODEM_Timeout(char *pParam)
{
	u32 len, left;
	u32 i;

	if(!pParam)
		len = SPACE_LENGTH; // default length
	else
		len = atoi(pParam);

	left = len;
	memset(sSpace, 0x0, SPACE_LENGTH);

	if(USBH_CUSTOM_MODEM_Receive(pController, (u8*)sSpace, &left, 30000)) // wait 30 seconds
	{
		printf("MODEM channel receive timeout\r\n");
	}

	len -= left;
	if(len)
		printf("\r\nreceived %d bytes\r\n", len);

	for(i = 0; i < len; i++)
		printf("%c", sSpace[i]);

	return (TRUE);
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static bool_t SH_USB_Send_DEBUG(char *pParam)
{
	u8 *command;
	u32 len;
	char high, low;

	if(!pParam)
		return (FALSE);

	if(('-' == pParam[0]) && ('h' == pParam[1]) && (' ' == pParam[2])) // 将字符串格式命令转为HEX格式
	{
		pParam += 3;
		len = strlen(pParam);
		if(len > COMMAND_MAX)
		{
			printf("\r\ncommand parameter is too long(max:%d)\r\n", COMMAND_MAX);
			return (TRUE);
		}

		while(*pParam != '\0' && *(pParam + 1) != '\0')
		{
			high = tolower((s32)*pParam); // 字符作为command某个字符的高4位
			low = tolower((s32)*(pParam + 1)); // 字符作为command某个字符的低4位

			if ((('0' <= high && '9' >= high) || ('a' <= high && 'f' >= high)) &&
				(('0' <= low && '9' >= low) || ('a' <= low && 'f' >= low))) // 只处理‘0’-‘9’，‘a’ - 'f'之间的数据
			{
				if ('0' <= high && '9' >= high)
				{
					sSpace[len] = (high - '0') << 4;
				}
				else
				{
					sSpace[len] = (0x0a + high - 'a') << 4;
				}

				if ('0' <= low && '9' >= low)
				{
					sSpace[len] |= (low - '0');
				}
				else
				{
					sSpace[len] |= (0x0a + low - 'a');
				}

				len++;
				pParam += 2;
			}
			else
			{
				pParam++;
			}
		}
	}
	else // 字符串格式的命令
	{
		len = strlen(pParam);
		memset(sSpace, 0x0, len);
		strcpy(sSpace, pParam);
		sSpace[len] = '\r';
		len += 1;
		sSpace[len] = '\0';

	}

	command = (u8*)sSpace;

	if(USBH_CUSTOM_DEBUG_Transmit(pController, (u8*)command, &len, -1))
		printf("DEBUG channel commmand :\"%s\" error\r\n", command);

    return (TRUE);
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static bool_t SH_USB_Send_DEBUG_Timeout(char *pParam)
{
	u8 *command;
	u32 len;
	char high, low;

	if(!pParam)
		return (FALSE);

	if(('-' == pParam[0]) && ('h' == pParam[1]) && (' ' == pParam[2])) // 将字符串格式命令转为HEX格式
	{
		pParam += 3;
		len = strlen(pParam);
		if(len > COMMAND_MAX)
		{
			printf("\r\ncommand parameter is too long(max:%d)\r\n", COMMAND_MAX);
			return (TRUE);
		}

		while(*pParam != '\0' && *(pParam + 1) != '\0')
		{
			high = tolower((s32)*pParam); // 字符作为command某个字符的高4位
			low = tolower((s32)*(pParam + 1)); // 字符作为command某个字符的低4位

			if ((('0' <= high && '9' >= high) || ('a' <= high && 'f' >= high)) &&
				(('0' <= low && '9' >= low) || ('a' <= low && 'f' >= low))) // 只处理‘0’-‘9’，‘a’ - 'f'之间的数据
			{
				if ('0' <= high && '9' >= high)
				{
					sSpace[len] = (high - '0') << 4;
				}
				else
				{
					sSpace[len] = (0x0a + high - 'a') << 4;
				}

				if ('0' <= low && '9' >= low)
				{
					sSpace[len] |= (low - '0');
				}
				else
				{
					sSpace[len] |= (0x0a + low - 'a');
				}

				len++;
				pParam += 2;
			}
			else
			{
				pParam++;
			}
		}
	}
	else // 字符串格式的命令
	{
		len = strlen(pParam);
		memset(sSpace, 0x0, len);
		strcpy(sSpace, pParam);
		sSpace[len] = '\r';
		len += 1;
		sSpace[len] = '\0';

	}

	command = (u8*)sSpace;

	if(USBH_CUSTOM_DEBUG_Transmit(pController, command, &len, 30000))
		printf("DEBUG channel commmand :\"%s\" error\r\n", command);

    return (TRUE);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static bool_t SH_USB_Reveive_DEBUG(char *pParam)
{
	u32 len, left;
	u32 i;

	if(!pParam)
		len = SPACE_LENGTH; // default length
	else
		len = atoi(pParam);

	left = len;
	memset(sSpace, 0x0, SPACE_LENGTH);

	if(USBH_CUSTOM_DEBUG_Receive(pController, (u8*)sSpace, &left, -1)) // wait forever
	{
		printf("DEBUG channel receive error\r\n");
	}

	len -= left;
	if(len)
		printf("\r\nreceived %d bytes\r\n", len);

	for(i = 0; i < len; i++)
		printf("%c", sSpace[i]);

	return (TRUE);
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static bool_t SH_USB_Reveive_DEBUG_Timeout(char *pParam)
{
	u32 len, left;
	u32 i;

	if(!pParam)
		len = SPACE_LENGTH; // default length
	else
		len = atoi(pParam);

	left = len;
	memset(sSpace, 0x0, SPACE_LENGTH);

	if(USBH_CUSTOM_DEBUG_Receive(pController, (u8*)sSpace, &left, 30000)) // wait 30 seconds
	{
		printf("DEBUG channel receive timeout\r\n");
	}

	len -= left;
	if(len)
		printf("\r\nreceived %d bytes\r\n", len);

	for(i = 0; i < len; i++)
		printf("%c", sSpace[i]);

	return (TRUE);
}
#if 0
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
u8 runSuper = 0;
u8 runSuperLog = 0;
u8 runLoad = 0;
u8 runLoadLog = 0;
u32 ram[1000];

u32 USB_TestSuper(void)
{
	s64 start, consume, log[16], max;
	u32 i, j, k;
	u32 output = 0;

	max = 0;
	j = 0;
	printf("\r\nUSB TEST: super on\r\n");
	while(runSuper)
	{

		start = DjyGetSysTime();
		for(k = 0; k < 10; k++)
		{
			for(i = 0; i < 1000; i++)
				ram[i] = i;

			for(i = 0; i < 1000; i++)
			{
				if(ram[i] != i)
					printf("\r\nUSB TEST:ram error, %d\r\n", i);
			}
		}

		consume = DjyGetSysTime() - start;
		log[j] = consume;
		if(max < consume)
			max = consume;

		j++;
		if(j == 16)
		{
			consume = 0;
			for(i = 0; i < 16; i++)
			{
				consume += log[i];
			}

			consume = consume >> 4; // average
			if((runSuperLog) && (++output > 1000))
			{
				printf("\r\nUSB TEST:ram operation cost %d us av, %d us max, \r\n", (u32)consume, (u32)max);
				printf("\r\nUSB TEST:int operation cost %d us max, \r\n", (u32)gMax);
				output = 0;
			}

			j = 0;
			max = 0;
		}
		Djy_EventDelay(1000);

	};

	printf("\r\nUSB TEST: super exit\r\n");
	return (0);
}

u32 USB_TestLoad(void)
{
	s64 start, consume, log[16], max;
	u32 len = SPACE_LENGTH;
	u32 i, j;
	u32 output = 0;

	printf("\r\nUSB TEST: load on\r\n");
	j = 0;
	max = 0;
	while(runLoad)
	{
		start = DjyGetSysTime();
		USBH_CUSTOM_MODEM_Receive(pController, (u8*)sSpace, &len, 0);
		consume = DjyGetSysTime() - start;

		log[j] = consume;
		if(max < consume)
			max = consume;

		j++;
		if(j == 16)
		{
			consume = 0;
			for(i = 0; i < 16; i++)
			{
				consume += log[i];
			}

			consume = consume >> 4; // average
			if((runLoadLog) && (++output > 100)) // 大概一秒输出一次
			{
				printf("\r\nUSB TEST:load operation cost %d us av, %d us max\r\n", (u32)consume, (u32)max);
				output = 0;
			}

			j = 0;
			max = 0;
		}

		if((len != SPACE_LENGTH) && (runLoadLog))
		{
			len = SPACE_LENGTH - len;
			printf("\r\nUSB TEST: load received %d bytes", len);
			for(i = 0; i < len; i++)
				printf("%c", sSpace[i]);
			memset(sSpace, 0x0, SPACE_LENGTH);
		}
		Djy_EventDelay(10000);
	}

	printf("\r\nUSB TEST: load off\r\n"); // 退出
	return (0);
}
#endif
static bool_t SH_USB_Test(char *pParam)
{
	u16 id;
	extern void RecordControl(char *pOperation);
#if 0
	if(!strcmp(pParam, "superon"))
	{
		runSuper = 1;

		id = Djy_EvttRegist(EN_CORRELATIVE, 20, 0, 0,
				USB_TestSuper, NULL, 0x800, "USB TEST SUPER");

		if(CN_EVTT_ID_INVALID != id)
		{
			Djy_EventPop(id, NULL, 0, 0, 0, 0);
			return (TRUE);
		}

	}

	if(!strcmp(pParam, "loadon"))
	{
		runLoad = 1;

		id = Djy_EvttRegist(EN_CORRELATIVE, 190, 0, 0,
				USB_TestLoad, NULL, 0x800, "USB TEST Load");

		if(CN_EVTT_ID_INVALID != id)
		{
			Djy_EventPop(id, NULL, 0, 0, 0, 0);
			return (TRUE);
		}
	}

	if(!strcmp(pParam, "superoff"))
	{
		runSuper = 1;
	}

	if(!strcmp(pParam, "loadoff"))
	{
		runLoad = 0;
	}

	if(!strcmp(pParam, "loadlogon"))
	{
		runLoadLog = 1;
	}

	if(!strcmp(pParam, "loadlogoff"))
	{
		runLoadLog = 0;
	}

	if(!strcmp(pParam, "superlogon"))
	{
		runSuperLog = 1;
	}

	if(!strcmp(pParam, "superlogoff"))
	{
		runSuperLog = 0;
	}
#endif
	if(!strcmp(pParam, "icon"))
	{
		gIntCtrl = 1;
		RecordControl("on");
	}

	if(!strcmp(pParam, "icoff"))
	{
		gIntCtrl = 0;
		gInts = 0;
		RecordControl("off");
	}

	if(!strcmp(pParam, "icc"))
	{
		RecordControl("clean");
	}

	if(!strcmp(pParam, "ics"))
	{
		RecordControl("show");
	}

	return (TRUE);
}

//-----------------------------------------------------------------------------
//功能: USB相关命令安装
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
void USB_ShellInstall(void)
{
    u32 commands;

    pController = USBH_HostHandle(2); // #2设USB用于无线模块

    commands = sizeof(sCommandTable) / sizeof(struct ShellCmdTab);

    spCommandSpace = malloc(commands * sizeof(*spCommandSpace));
    if(NULL == spCommandSpace)
        return ;

    Sh_InstallCmd(sCommandTable, spCommandSpace, commands);
}
