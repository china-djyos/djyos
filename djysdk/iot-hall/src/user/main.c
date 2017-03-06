
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
#include "os.h"
#include "stdint.h"
#include "systime.h"
#include "cpu_peri.h"
#include "lowpower.h"
#include "shell.h"
#include "IOT.h"

extern ptu32_t MainTestEntry(void);
extern void GDD_Startup(void);
extern void djyip_main(void);
void IOT_Startup(void);
extern u8  g_MonitorNum;
void BootRun(char *param)
{
	volatile u8 *pAddr;
	u8 i;
	pAddr=(u8 *)(0x2001fff0);
	for(i=0;i<16;i++)
	{
		*(pAddr+i)=(0xA0+i);
	}
	reset();
}
void AppRun(char *param)
{
	volatile u8 *pAddr;
	u8 i;
	pAddr=(u8 *)(0x2001fff0);
	for(i=0;i<16;i++)
	{
		*(pAddr+i)=0x00;
	}
	reset();
}
extern u16 g_UpdatePkgNum;
extern u8  g_MonitorNum;
extern u8  g_UpdateLastPkgSize;
void UpdateMonitor(char *param)
{
	g_UpdatePkgNum=800;
	g_MonitorNum=100;
	g_UpdateLastPkgSize=100;
	TransferToMonitorUpdate();
}
static bool_t __osversion(char *param)
{
	printf("DJYOSVERSION:DATE:%s TIME:%s\n\r",__DATE__,__TIME__);
	return true;
}
struct ShellCmdTab  shell_monitordebug_table[] =
{
    {
            "bootrun",
            BootRun,
            "��λcpu����Boot����",
            NULL
    },
    {
        "apprun",
        AppRun,
        "��λcpu����App����",
        NULL
    },
    {
    	"update",
		UpdateMonitor,
		"�м����������˽ڵ�",
		NULL
    },
	{
		"version",
		__osversion,
		"usage:version",
		NULL
	}
};
#define cn_monitor_test_num  ((sizeof(shell_monitordebug_table))/(sizeof(struct ShellCmdTab)))
static struct ShellCmdRsc tg_monitorshell_cmd_rsc[cn_monitor_test_num];
void Shell_CmdAdd(void)
{
    Sh_InstallCmd(shell_monitordebug_table,tg_monitorshell_cmd_rsc,
            cn_monitor_test_num);
}

u32 QH_EntrySleepReCall(u32 SleepLevel)
{
    switch(SleepLevel)
    {
       case CN_SLEEP_L0:
           break;
       case CN_SLEEP_L1:
           break;
       case CN_SLEEP_L2:
           break;
       case CN_SLEEP_L3:
           break;
       case CN_SLEEP_L4:
    	   	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
			IWDG_SetPrescaler(IWDG_Prescaler_256); //40K/256 =6.4ms
			IWDG_SetReload(0x0FFF); //4096*6.4ms
			IWDG_ReloadCounter();
			IWDG_Enable();
    	   //����͹���ǰ����SPK,��Ƶ��Դ����������Դ
    	   IOT_VoicePowerOff();
    	   SX1278_EnterSleep();
//    	   IOT_SmogPowerOff();

    	   RTC_ClearFlag(	RTC_FLAG_TAMP1F|RTC_FLAG_TSOVF|RTC_FLAG_TSF
    	       			   |RTC_FLAG_WUTF|RTC_FLAG_ALRBF|RTC_FLAG_ALRAF|RTC_FLAG_RSF);

    	   PWR_ClearFlag(PWR_FLAG_WU);//���Wake-up ��־
    	   PWR_WakeUpPinCmd(ENABLE);
           break;
       default:
           break;
    }
    return 0;
}
u32 QH_ExitSleepReCall(u32 SleepLevel)
{
    switch(SleepLevel)
    {
       case CN_SLEEP_L0:
           break;
       case CN_SLEEP_L1:
           break;
       case CN_SLEEP_L2:
           break;
       default:
           break;
    }
    return 0;
}
void Flash_Test(void)
{
	u8 buf[64];
	u32 i;
	u32 address;
	address = 0x080fff80;
	for(i = 0; i < 64; i ++ )
	{
		buf[i] = i;
	}
	Flash_Write(address,buf,64);
	memset(buf,0x00,64);
	Flash_Read(address,buf,64);
	Flash_Erase();
	Flash_Read(address,buf,64);
}
void Flash_Test2(void)
{
	u8 wbuf[64],rbuf[64];
	u32 i;
	u32 address;
	address = 0x08080000;
	for(i = 0; i < 64; i ++ )
	{
		wbuf[i] = i;
	}
	Flash_Erase();
	for(address = 0x08080000; address < 0x080fffff;)
	{
		Flash_Write(address,wbuf,64);
		memset(rbuf,0x00,64);
		Flash_Read(address,rbuf,64);
		if(0!= memcmp(wbuf,rbuf,64))
		{
			printf("error addr = %d \r\n",address);
			break;
		}
		address += 64;
	}
	if(address == 0x08100000)
	{
		printf("write flash ok!\r\n");
	}
}
void ECC_Test(void)
{
	u32 i;
	u8 ecc_test1[256];
	u8 ecc_test2[256];
	u8 ecc_test3[3];
	u32 ecc_result1=0;
	u32 ecc_result2=0;
	u32 ecc_result3=0;
	for(i=0;i<256;i++)
	{
		ecc_test1[i]=i;
	}
	for(i=0;i<10;i++)
	{
		ecc_test2[i]=i;
	}
	for(i=0;i<246;i++)
	{
		ecc_test2[10+i]=0x00;
	}
	for(i=0;i<3;i++)
	{
		ecc_test3[i]=i;
	}
	Ecc_Make256(ecc_test1,&ecc_result1);
	Ecc_Make256(ecc_test2,&ecc_result2);
	Ecc_Make256(ecc_test3,&ecc_result3);
    printf("ECC Test1 Result %x\r\n",ecc_result1);
    printf("ECC Test2 Result %x\r\n",ecc_result2);
    printf("ECC Test3 Result %x\r\n",ecc_result3);
}
ptu32_t djy_main(void)
{
	Shell_CmdAdd();
//	djyip_main();
#ifdef APP_PRO
	printf("Running APP...\r\n");
	IOT_Startup();
#endif
#ifdef OS_BOOT
	printf("Running OS_BOOT...\r\n");
	Flash_Erase();
	IOT_Update();
#endif
	while(1)
	{
		Djy_EventDelay(1000*mS);
	}
	return 0;
}

