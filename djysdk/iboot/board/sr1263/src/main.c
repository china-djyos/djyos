
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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下二条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、此三条件表列，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、此三条件表列，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
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
	u32 SECTORError=0;//保存出错类型信息
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

    //提取3个参数
    test_param = Sh_GetWord(param,&next_param);
    if(test_param == NULL)
    {
        printf("\r\n格式错误，正确格式是：\r\n test n \r\n");
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
// IIC资源配置          //IIC 按长度编排会影响后期升级的兼容性(参数会被全部更改) xuyj 2011-2-21 11:19上午
#define CN_IIC_SOE_START        (0)                                         // IIC中定值存放起始地址
//一个定值6个字节，目前按100个定值算 512个字节
#define CN_IIC_SET_START        (CN_IIC_SOE_START+80)   // IIC中定值存放起始地址
#define CN_IIC_WAVE_ENTRY_PNT_QUE_START (CN_IIC_SET_START+512)//录波条目指针控制，18个字节，按32字节算；
#define CN_IIC_WAVE_ENTRY_FILE_SAVE_START (CN_IIC_WAVE_ENTRY_PNT_QUE_START+32)//录波条目信息保存，共 64条，每条23个字节,按32个字节算，工:32*64=2048个字节；
#define CN_IIC_FCI_C_CODE_SIZE (CN_IIC_WAVE_ENTRY_FILE_SAVE_START+2048)//采集单元代码大小:2个字节
#define CN_IIC_FCI_C_CODE_CRC  (CN_IIC_FCI_C_CODE_SIZE+2)//检验码大小:2个字节

#define CN_IIC_COMM_PORT_PARA  (CN_IIC_FCI_C_CODE_CRC+2)//通信串口参数 : 7个 字节， 扩展为 16 个；
#define CN_IIC_COMM_PARA_CFG_FLAG (CN_IIC_COMM_PORT_PARA+70)//通信参数配置标记， 0x55:已下配置； 其它:未下配置---1个字节；
#define CN_IIC_YC_START            (CN_IIC_COMM_PARA_CFG_FLAG+2)//yc控制 指针，18个，按32来；

#define CN_IIC_FCI_H_CODE_SIZE (CN_IIC_YC_START+32)//汇集单元代码大小:4个字节
#define CN_IIC_FCI_H_CODE_CRC  (CN_IIC_FCI_H_CODE_SIZE+4)//检验码大小:4个字节

#define CN_IIC_FCI_H_IBOOT_CODE_UPGRATE_MSG  (CN_IIC_FCI_H_CODE_CRC+4)//32字节



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


