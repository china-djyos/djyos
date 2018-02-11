//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

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
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
#include "os.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "string.h"
#include "driver.h"
#include "uartctrl.h"
#include "ymodem.h"
#include "driver/include/uart.h"
#include "driver.h"
#include "IAP.h"
#include <cfg/iboot_config.h>
static struct DjyDevice * YmodemDev;

extern bool_t Ymodem_PathSet(const char *Path);
bool_t Ymodem_UpdateProgram(void)
{
	u8 cmd[32];
	u32 crc;
    extern bool_t Sh_DownloadFile(char *param);
	YmodemDev = Driver_OpenDevice("USART0",D_RDONLY,CN_TIMEOUT_FOREVER);
	if(NULL == YmodemDev)
	{
	    return false;
	}
	Ymodem_PathSet("/iboot");
	Driver_CtrlDevice(YmodemDev,CN_UART_SET_BAUD,115200,0);
	Driver_CtrlDevice(YmodemDev,CN_UART_START,0,0);
	//read the cmd
	while(1)
	{
		Driver_ReadDevice(YmodemDev,cmd,2,0,CN_TIMEOUT_FOREVER);
		if(0 ==memcmp("DL",cmd,2))
		{
			if(true == Ymodem_DownloadFile(NULL))
			{
				//��CRCУ��
				crc=IAP_GetAPPCRC();
			}
			else
			{
				continue;
			}
		}
		else if(0==memcmp("CK",cmd,2))
		{
           u8 buf[7];
           buf[0]=0x43;
           buf[1]=0x4B;
           buf[2]=crc;
           buf[3]=crc>>8;
           buf[4]=crc>>16;
           buf[5]=crc>>24;
           buf[6]=0x43;
           Driver_WriteDevice(YmodemDev,buf,7,0,0,CN_TIMEOUT_FOREVER);
           printf("IAP Ymodem tarnsfer completed.\r\n");
           break;
		}
	}

	return true;
}


static const Pin BOOT_MCU_LED[] = {
		{PIO_PA23, PIOA, ID_PIOA, PIO_OUTPUT_0,    PIO_DEFAULT}
};

void LED_On(void)
{
	PIO_Clear(BOOT_MCU_LED);
}

void LED_Off(void)
{
	PIO_Set(BOOT_MCU_LED);
}

void LED_Init(void)
{
	PIO_Configure(BOOT_MCU_LED, PIO_LISTSIZE(BOOT_MCU_LED));
}



//ϵͳ������
ptu32_t djy_main(void)
{
	while(1)
	{
	   LED_On();
	   Ymodem_UpdateProgram();
    }

	return 0;
}

