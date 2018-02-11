
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
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
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
/*
 * app_main.c
 *
 *  Created on: 2014-5-28
 *      Author: Administrator
 */

#include "stdint.h"
#include "cpu_peri.h"
#include "systime.h"
#include "stddef.h"

void LED_Init(void)
{
	//cpu_run:PA15
	GPIO_InitTypeDef  GPIO_InitStruct;

	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
void LED_On (void)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET);
}
void LED_Off (void)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET);
}


ptu32_t FlashEraseTest(u32 num)
{
	s32 Ret;
	u32 SectorNo;
	u32 SECTORError=0;//�������������Ϣ
	static FLASH_EraseInitTypeDef EraseInitStruct;

	SectorNo = num + 64;
	HAL_FLASH_Unlock();

	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Banks       = FLASH_BANK_2;
	EraseInitStruct.Page        = SectorNo;
	EraseInitStruct.NbPages     = 1;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	 	Ret=-1;
	else
		Ret=0;
	HAL_FLASH_Lock();

	return Ret;
}

s32 Flash_PageProgram( u8 *Data)
{
	u32 Ret,i,ipage;
	u64 *pData = (u64*)Data;

	u32 Addr = 0x08020000;

	if(!Data)
		return (-1);

	HAL_FLASH_Unlock();

	for(i = 0; i < 512;)
	{
    	if(*(u64*)Addr != *pData)
    	{
    		Ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,Addr,*pData);
    		if(Ret != HAL_OK)
    			break;
    	}
		pData++;
		i += 8;
		Addr += 8;
	}

	HAL_FLASH_Lock();

	return i;
}

static u8 test = 0;
bool_t testmodify(char *param)
{
    char *test_param,*next_param;

    //��ȡ3������
    test_param = Sh_GetWord(param,&next_param);
    if(test_param == NULL)
    {
        printf("\r\n��ʽ������ȷ��ʽ�ǣ�\r\n test n \r\n");
        return false;
    }
    test = strtoul(test_param, (char **)NULL, 0);


	return true;
}

#include <shell.h>
struct ShellCmdTab const maintest_cmd_table[] =
{
    {
        "test",
		testmodify,
        NULL,
        "TEST NUM"
    }
};

#define CN_MAINTEST_NUM (sizeof(maintest_cmd_table)/sizeof(struct ShellCmdTab))
static struct ShellCmdRsc tg_maintest_cmd_rsc[CN_MAINTEST_NUM];

u8 buf[512];
int len;
#include <driver.h>
#include <uartctrl.h>
extern void PPP_StartEvent( void );


static u8 Uart2Buf[256];


//copy from Const_Sys.h
// IIC��Դ����          //IIC �����ȱ��Ż�Ӱ����������ļ�����(�����ᱻȫ������) xuyj 2011-2-21 11:19����
#define CN_IIC_SOE_START        (0)                                         // IIC�ж�ֵ�����ʼ��ַ
//һ����ֵ6���ֽڣ�Ŀǰ��100����ֵ�� 512���ֽ�
#define CN_IIC_SET_START        (CN_IIC_SOE_START+80)   // IIC�ж�ֵ�����ʼ��ַ
#define CN_IIC_WAVE_ENTRY_PNT_QUE_START (CN_IIC_SET_START+512)//¼����Ŀָ����ƣ�18���ֽڣ���32�ֽ��㣻
#define CN_IIC_WAVE_ENTRY_FILE_SAVE_START (CN_IIC_WAVE_ENTRY_PNT_QUE_START+32)//¼����Ŀ��Ϣ���棬�� 64����ÿ��23���ֽ�,��32���ֽ��㣬��:32*64=2048���ֽڣ�
#define CN_IIC_FCI_C_CODE_SIZE (CN_IIC_WAVE_ENTRY_FILE_SAVE_START+2048)//�ɼ���Ԫ�����С:2���ֽ�
#define CN_IIC_FCI_C_CODE_CRC  (CN_IIC_FCI_C_CODE_SIZE+2)//�������С:2���ֽ�

#define CN_IIC_COMM_PORT_PARA  (CN_IIC_FCI_C_CODE_CRC+2)//ͨ�Ŵ��ڲ��� : 7�� �ֽڣ� ��չΪ 16 ����
#define CN_IIC_COMM_PARA_CFG_FLAG (CN_IIC_COMM_PORT_PARA+70)//ͨ�Ų������ñ�ǣ� 0x55:�������ã� ����:δ������---1���ֽڣ�
#define CN_IIC_YC_START            (CN_IIC_COMM_PARA_CFG_FLAG+2)//yc���� ָ�룬18������32����

#define CN_IIC_FCI_H_CODE_SIZE (CN_IIC_YC_START+32)//�㼯��Ԫ�����С:4���ֽ�
#define CN_IIC_FCI_H_CODE_CRC  (CN_IIC_FCI_H_CODE_SIZE+4)//�������С:4���ֽ�

#define CN_IIC_FCI_H_IBOOT_CODE_UPGRATE_MSG  (CN_IIC_FCI_H_CODE_CRC+4)//32�ֽ�



ptu32_t djy_main(void)
{
	extern void Sh_GetStatus(char *param);
	extern void Sh_GetRunMode(char *param);
	Sh_GetRunMode(NULL);
	Sh_GetStatus(NULL);
	LED_Init();
	Sh_InstallCmd(maintest_cmd_table,tg_maintest_cmd_rsc,CN_MAINTEST_NUM);

	ModuleInstall_AppUpdate(CN_IIC_FCI_H_IBOOT_CODE_UPGRATE_MSG);

	while(1)
	{
		Djy_EventDelay(500*mS);
		LED_On();
		Djy_EventDelay(500*mS);
		LED_Off();

	    if(test == 100)			//just for test
	    {
	    	Update_Test(0);
	    	test = 0;
	    }
	}
}
ptu32_t djy_main_r(void)
{

	u32 temp;
	extern void Sh_GetStatus(char *param);
	extern void Sh_GetRunMode(char *param);
	Sh_GetRunMode(NULL);
	Sh_GetStatus(NULL);
	LED_Init();

	Sh_InstallCmd(maintest_cmd_table,tg_maintest_cmd_rsc,CN_MAINTEST_NUM);

//	GPRS_StartEvent();
	Simcom_UartStart("UART4");

//	Tcp_Init("106.3.225.253",4500);

	GPIO_InitTypeDef  GPIO_InitStruct;

	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);

	while(1)
	{
		Djy_EventDelay(500*mS);
		LED_On();
		Djy_EventDelay(500*mS);
		LED_Off();
//		len = Driver_ReadDevice(UartDev,Uart2Buf,256,0,0);

		if(len >0)
		{
//			Driver_WriteDevice(UartDev,Uart2Buf,len,0,1,0xFFFFFFFF);
//			memset(Uart2Buf,0x00,256);
		}
//		Djy_EventDelay(1*mS);
		if(test == 1)
		{
//			sgc115x_test();
			printf("gprs sleep enable!\r\n");
			SimCom_SleepEnable();
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, SET);
			test = 0;
		}
		if(test == 2)
		{
			printf("gprs sleep disable!\r\n");
			SimCom_SleepDisable();
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, RESET);
			test = 0;
		}

		if(test == 3)
		{
//			GPRS_Reset();
			test = 0;
		}

		if(test == 4)
		{
			AtCmdShell("+++");
			test = 0;
		}

		if(test == 5)
		{
			SimCom_PwrVccOff();
			test = 0;
		}

		if(test == 6)
		{
//			SimCom_Data2AtMode("pppnet","UART4",16);
			test = 0;
		}
		if(test == 7)
		{
//			SimCom_At2DataMode("pppnet","UART4",16);
			test = 0;
		}

		if(test == 10)
		{
			temp = GPRS_NetCsq();
			printf("GPRS_CSQ = %d\r\n",temp);
			test = 0;
		}

		if(test == 11)
		{
			test = 0;
			GPRS_LowPwerEn(1);
		}
		if(test == 12)
		{
			test = 0;
			GPRS_LowPwerEn(0);
		}
//		len = Tcp_Read(buf,512);
//		if(len)
//		{
//			buf[len - 1] = buf[len - 1]+1;
//			Tcp_Send(buf,len);
//		}
	}

	return 0;
}


