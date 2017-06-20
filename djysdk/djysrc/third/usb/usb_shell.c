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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
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
//����: 
//����:
//����:
//��ע: 
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
//����:
//����:
//����:
//��ע:
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
//����:
//����:
//����:
//��ע:
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
//����:
//����:
//����:
//��ע:
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
//����:
//����:
//����:
//��ע:
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
//����:
//����:
//����:
//��ע:
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
//����:
//����:
//����:
//��ע:
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
//����:
//����:
//����:
//��ע:
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
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static bool_t SH_USB_Send_DEBUG(char *pParam)
{
	u8 *command;
	u32 len;
	char high, low;

	if(!pParam)
		return (FALSE);

	if(('-' == pParam[0]) && ('h' == pParam[1]) && (' ' == pParam[2])) // ���ַ�����ʽ����תΪHEX��ʽ
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
			high = tolower((s32)*pParam); // �ַ���Ϊcommandĳ���ַ��ĸ�4λ
			low = tolower((s32)*(pParam + 1)); // �ַ���Ϊcommandĳ���ַ��ĵ�4λ

			if ((('0' <= high && '9' >= high) || ('a' <= high && 'f' >= high)) &&
				(('0' <= low && '9' >= low) || ('a' <= low && 'f' >= low))) // ֻ����0��-��9������a�� - 'f'֮�������
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
	else // �ַ�����ʽ������
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
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static bool_t SH_USB_Send_DEBUG_Timeout(char *pParam)
{
	u8 *command;
	u32 len;
	char high, low;

	if(!pParam)
		return (FALSE);

	if(('-' == pParam[0]) && ('h' == pParam[1]) && (' ' == pParam[2])) // ���ַ�����ʽ����תΪHEX��ʽ
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
			high = tolower((s32)*pParam); // �ַ���Ϊcommandĳ���ַ��ĸ�4λ
			low = tolower((s32)*(pParam + 1)); // �ַ���Ϊcommandĳ���ַ��ĵ�4λ

			if ((('0' <= high && '9' >= high) || ('a' <= high && 'f' >= high)) &&
				(('0' <= low && '9' >= low) || ('a' <= low && 'f' >= low))) // ֻ����0��-��9������a�� - 'f'֮�������
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
	else // �ַ�����ʽ������
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
//����:
//����:
//����:
//��ע:
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
//����:
//����:
//����:
//��ע:
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
//����:
//����:
//����:
//��ע:
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
			if((runLoadLog) && (++output > 100)) // ���һ�����һ��
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

	printf("\r\nUSB TEST: load off\r\n"); // �˳�
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
//����: USB������װ
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
void USB_ShellInstall(void)
{
    u32 commands;

    pController = USBH_HostHandle(2); // #2��USB��������ģ��

    commands = sizeof(sCommandTable) / sizeof(struct ShellCmdTab);

    spCommandSpace = malloc(commands * sizeof(*spCommandSpace));
    if(NULL == spCommandSpace)
        return ;

    Sh_InstallCmd(sCommandTable, spCommandSpace, commands);
}
