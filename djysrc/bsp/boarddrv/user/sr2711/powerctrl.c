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
//����ģ��: SR2711���ģ���Դ����
//����:  zhb.
//�汾��V1.0.0
//�ļ�����: Զ������
//����˵��:
//�޶���ʷ:
//1. ����: 2017-08-12
//   ����:  zhb.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "cpu_peri.h"
#include "board-config.h"
#include "powerctrl.h"
#include "shell.h"
#include <stm32f7xx_hal_mmc.h>

void Peri_LowPowerCfg(enum BoardPeriCode PeriIdx,bool_t Flag);
extern void ADC_LowPowerCfg(bool_t Flag);


//**************************************************************************
static void Sh_PeriStatus(void);
static void Sh_SDRAM_PowerOn(void);
static void Sh_SDRAM_PowerOff(void);
static void Sh_EMMC_PowerOn(void);
static void Sh_EMMC_PowerOff(void);
static void Sh_EMAC_PowerOn(void);
static void Sh_EMAC_PowerOff(void);
static void Sh_IMAGE_PowerOn(void);
static void Sh_IMAGE_PowerOff(void);
static void Sh_CAMERA_PowerOn(void);
static void Sh_CAMERA_PowerOff(void);
static void Sh_30W_PowerOn(void);
static void Sh_30W_PowerOff(void);
static void Sh_10W_PowerOn(void);
static void Sh_10W_PowerOff(void);
static void Sh_SENSOR1_PowerOn(void);
static void Sh_SENSOR1_PowerOff(void);
static void Sh_SENSOR2_PowerOn(void);
static void Sh_SENSOR2_PowerOff(void);
static void Sh_SENSOR3_PowerOn(void);
static void Sh_SENSOR3_PowerOff(void);
static void Sh_SENSOR4_PowerOn(void);
static void Sh_SENSOR4_PowerOff(void);
static void Sh_SENSOR5_PowerOn(void);
static void Sh_SENSOR5_PowerOff(void);
static void Sh_SENSOR6_PowerOn(void);
static void Sh_SENSOR6_PowerOff(void);
static void Sh_SENSOR7_PowerOn(void);
static void Sh_SENSOR7_PowerOff(void);
static void Sh_SENSOR8_PowerOn(void);
static void Sh_SENSOR8_PowerOff(void);
static void Sh_SENSOR9_PowerOn(void);
static void Sh_SENSOR9_PowerOff(void);
static void Sh_SENSOR10_PowerOn(void);
static void Sh_SENSOR10_PowerOff(void);
static void Sh_SENSOR11_PowerOn(void);
static void Sh_SENSOR11_PowerOff(void);
static void Sh_SENSOR12_PowerOn(void);
static void Sh_SENSOR12_PowerOff(void);
static void Sh_SENSOR13_PowerOn(void);
static void Sh_SENSOR13_PowerOff(void);
static void Sh_RF1_PowerOn(void);
static void Sh_RF1_PowerOff(void);
static void Sh_RTC_PowerOn(void);
static void Sh_RTC_PowerOff(void);
static void Sh_ENCRYPTION_PowerOn(void);
static void Sh_ENCRYPTION_PowerOff(void);
static void Sh_RF2_PowerOn(void);
static void Sh_RF2_PowerOff(void);
static void Sh_BAT_PowerOn(void);
static void Sh_BAT_PowerOff(void);
static void Sh_5V_CheckPowerOn(void);
static void Sh_5V_CheckPowerOff(void);

static bool_t gs_PeriStaus[EN_PERI_END];
static char * gs_PeriName[EN_PERI_END];

//�͵�ƽ��Ч
const Pin VDD3_3CtrlPin[4]={
	{GPIO_K, PIN3, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},   //VDD3.3_KO1
	{GPIO_K, PIN4, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},   //VDD3.3_KO2
	{GPIO_K, PIN5, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},   //VDD3.3_KO3
	{GPIO_K, PIN6, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},   //VDD3.3_KO4
	//SR2711-V1.1.0ɾ��������  20170929
//	{GPIO_H, PIN12, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD3.3_KO5
};


//�ߵ�ƽ��Ч

//SR2711-V1.1.0��Ϊ�͵�ƽ��Ч  20170929
static const Pin VDD12_EN_Pin[]=
{
	{GPIO_H, PIN6, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}, //VDD12_EN1
	{GPIO_H, PIN7, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}, //VDD12_EN2
};


//�ߵ�ƽ��Ч
static const Pin VDD12CtrlPin[]={
	{GPIO_J, PIN5, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD_KO1
	{GPIO_J, PIN6, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD_KO2
	{GPIO_J, PIN7, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD_KO3
	{GPIO_J, PIN8, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD_KO4
	{GPIO_J, PIN9, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD_KO5
	{GPIO_J, PIN10, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}, //VDD_KO6
	{GPIO_J, PIN11, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}, //VDD_KO7
	{GPIO_J, PIN12, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}, //VDD_KO8
	{GPIO_J, PIN13, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}, //VDD_KO9
	{GPIO_J, PIN14, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}, //VDD_KO10
	{GPIO_J, PIN15, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}, //VDD_KO11
	{GPIO_K, PIN0, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD_KO12
	{GPIO_K, PIN1, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD_KO13
	{GPIO_K, PIN2, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD_KO14
	{GPIO_H, PIN10, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD_KO15
};

//�͵�ƽ��Ч  ���Ƶ�ص�ѹ����
static const Pin BATKoPin[]=
{
		{GPIO_A, PIN5, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},
};



void Board_PowerCtrlInit(void)
{
	 u8 cnt,i;
	 PIO_Configure(VDD12_EN_Pin,PIO_LISTSIZE(VDD12_EN_Pin));	//VDD12_EN
	 cnt=PIO_LISTSIZE(VDD12_EN_Pin);
	 //��ʼ״̬��ȫ��ʹ�ܵ�ƽ
	 for(i=0;i<cnt;i++)
	 {
		 //SR2711-V1.1.0��Ϊ�͵�ƽ��Ч  20170929
//		 PIO_Set(&VDD12_EN_Pin[i]);
		 PIO_Clear(&VDD12_EN_Pin[i]);
	 }

	 PIO_Configure(VDD12CtrlPin,PIO_LISTSIZE(VDD12CtrlPin));	//VDD12_KO
	 cnt=PIO_LISTSIZE(VDD12CtrlPin);
	 //��ʼ״̬��ȫ��ʹ�ܵ�ƽ
	 for(i=0;i<cnt;i++)
	 {
		 PIO_Set(&VDD12CtrlPin[i]);
	 }

	 PIO_Configure(VDD3_3CtrlPin,PIO_LISTSIZE(VDD3_3CtrlPin));	//VDD3.3
	 cnt=PIO_LISTSIZE(VDD3_3CtrlPin);
	 for(i=0;i<cnt;i++)
	 {
		 PIO_Clear(&VDD3_3CtrlPin[i]);
	 }
	 PIO_Configure(BATKoPin,PIO_LISTSIZE(BATKoPin));	//VDD3.3
	 PIO_Set(&BATKoPin[0]);


	 for(i=0;i<EN_PERI_END;i++)
	 {
	 	gs_PeriStaus[i]=true;
	 }
}






//**************************************************************************
struct ShellCmdTab const shell_cmd_power_ctrl_table[]=
{
	{"perista",(bool_t(*)(char*))Sh_PeriStatus,    "�����������״̬",            NULL},
	{"sdramon",(bool_t(*)(char*))Sh_SDRAM_PowerOn,   "ʹ��SDRAM����Դ",           NULL},
	{"sdramoff",(bool_t(*)(char*))Sh_SDRAM_PowerOff, "����SDRAM����Դ",           NULL},
	{"emmcon",(bool_t(*)(char*))Sh_EMMC_PowerOn,     "ʹ��EMMC����Դ",            NULL},
	{"emmcoff",(bool_t(*)(char*))Sh_EMMC_PowerOff,   "����EMMC����Դ",            NULL},
	{"emacron",(bool_t(*)(char*))Sh_EMAC_PowerOn,    "ʹ��EMAC����Դ",            NULL},
	{"emacoff",(bool_t(*)(char*))Sh_EMAC_PowerOff,   "����EMAC����Դ",            NULL},
	{"imageon",(bool_t(*)(char*))Sh_IMAGE_PowerOn,   "ʹ��ģ��ͼ����оƬ��Դ����Ӧ����",NULL},
	{"imageoff",(bool_t(*)(char*))Sh_IMAGE_PowerOff, "����ģ��ͼ����оƬ��Դ����Ӧ����",NULL},
	{"cameon",(bool_t(*)(char*))Sh_CAMERA_PowerOn,   "ʹ������ͷ���",             NULL},
	{"cameoff",(bool_t(*)(char*))Sh_CAMERA_PowerOff, "��������ͷ���",             NULL},
	{"30won",(bool_t(*)(char*))Sh_30W_PowerOn,       "����30W��Դ",              NULL},
	{"30woff",(bool_t(*)(char*))Sh_30W_PowerOff,     "�ϵ�30W��Դ",              NULL},
	{"10won",(bool_t(*)(char*))Sh_10W_PowerOn,       "����10W��Դ",              NULL},
	{"10woff",(bool_t(*)(char*))Sh_10W_PowerOff,     "�ϵ�10W��Դ",              NULL},
	{"sen1on",(bool_t(*)(char*))Sh_SENSOR1_PowerOn,  "ʹ�ܴ�������1��Դ",           NULL},
	{"sen1off",(bool_t(*)(char*))Sh_SENSOR1_PowerOff,"���ܴ�������1��Դ",           NULL},
	{"sen2on",(bool_t(*)(char*))Sh_SENSOR2_PowerOn,  "ʹ�ܴ�������2��Դ",           NULL},
	{"sen2off",(bool_t(*)(char*))Sh_SENSOR2_PowerOff,"���ܴ�������2��Դ",           NULL},
	{"sen3on",(bool_t(*)(char*))Sh_SENSOR3_PowerOn,  "ʹ�ܴ�������3��Դ",           NULL},
	{"sen3off",(bool_t(*)(char*))Sh_SENSOR3_PowerOff,"���ܴ�������3��Դ",           NULL},
	{"sen4on",(bool_t(*)(char*))Sh_SENSOR4_PowerOn,  "ʹ�ܴ�������4��Դ",           NULL},
	{"sen4off",(bool_t(*)(char*))Sh_SENSOR4_PowerOff,"���ܴ�������4��Դ",           NULL},
	{"sen5on",(bool_t(*)(char*))Sh_SENSOR5_PowerOn,  "ʹ�ܴ�������5��Դ",           NULL},
	{"sen5off",(bool_t(*)(char*))Sh_SENSOR5_PowerOff,"���ܴ�������5��Դ",           NULL},
	{"sen6on",(bool_t(*)(char*))Sh_SENSOR6_PowerOn,  "ʹ�ܴ�������6��Դ",           NULL},
	{"sen6off",(bool_t(*)(char*))Sh_SENSOR6_PowerOff,"���ܴ�������6��Դ",           NULL},
	{"sen7on",(bool_t(*)(char*))Sh_SENSOR7_PowerOn,  "ʹ�ܴ�������7��Դ",           NULL},
	{"sen7off",(bool_t(*)(char*))Sh_SENSOR7_PowerOff,"���ܴ�������7��Դ",           NULL},
	{"sen8on",(bool_t(*)(char*))Sh_SENSOR8_PowerOn,  "ʹ�ܴ�������8��Դ",           NULL},
	{"sen8off",(bool_t(*)(char*))Sh_SENSOR8_PowerOff,"���ܴ�������8��Դ",           NULL},
	{"sen9on",(bool_t(*)(char*))Sh_SENSOR9_PowerOn,  "ʹ�ܴ�������9��Դ",           NULL},
	{"sen9off",(bool_t(*)(char*))Sh_SENSOR9_PowerOff,"���ܴ�������9��Դ",           NULL},
	{"sen10on",(bool_t(*)(char*))Sh_SENSOR10_PowerOn,  "ʹ�ܴ�������10��Դ",        NULL},
	{"sen10off",(bool_t(*)(char*))Sh_SENSOR10_PowerOff,"���ܴ�������10��Դ",        NULL},
	{"sen11on",(bool_t(*)(char*))Sh_SENSOR11_PowerOn,  "ʹ�ܴ�������11��Դ",        NULL},
	{"sen11off",(bool_t(*)(char*))Sh_SENSOR11_PowerOff,"���ܴ�������11��Դ",        NULL},
	{"sen12on",(bool_t(*)(char*))Sh_SENSOR12_PowerOn,  "ʹ�ܴ�������12��Դ",        NULL},
	{"sen12off",(bool_t(*)(char*))Sh_SENSOR12_PowerOff,"���ܴ�������12��Դ",        NULL},
	{"sen13on",(bool_t(*)(char*))Sh_SENSOR13_PowerOn,  "ʹ�ܴ�������13��Դ",        NULL},
	{"sen13off",(bool_t(*)(char*))Sh_SENSOR13_PowerOff,"���ܴ�������13��Դ",        NULL},
	{"rf1on",(bool_t(*)(char*))Sh_RF1_PowerOn,         "ʹ��RFģ���Դ����Ӧ����",    NULL},
	{"rf1off",(bool_t(*)(char*))Sh_RF1_PowerOff,       "����RFģ���Դ����Ӧ����",    NULL},
	{"rtcon",(bool_t(*)(char*))Sh_RTC_PowerOn,         "ʹ��RTCоƬ����Ӧ����",      NULL},
	{"rtcoff",(bool_t(*)(char*))Sh_RTC_PowerOff,       "����RTCоƬ����Ӧ����",      NULL},
	{"ecton",(bool_t(*)(char*))Sh_ENCRYPTION_PowerOn,  "ʹ�ܼ���оƬ����Ӧ����",      NULL},
	{"ectoff",(bool_t(*)(char*))Sh_ENCRYPTION_PowerOff,"���ܼ���оƬ����Ӧ����",      NULL},
	{"rf2on",(bool_t(*)(char*))Sh_RF2_PowerOn,         "ʹ��������Ƶģ�鼰��Ӧ����",   NULL},
	{"rf2off",(bool_t(*)(char*))Sh_RF2_PowerOff,       "����������Ƶģ�鼰��Ӧ����",   NULL},
	{"baton",(bool_t(*)(char*))Sh_BAT_PowerOn,     "ʹ�ܵ�ز�������Ӧ����",          NULL},
	{"batoff",(bool_t(*)(char*))Sh_BAT_PowerOff,    "���ܵ�ز�������Ӧ����",         NULL},
	{"5vcheckon",(bool_t(*)(char*))Sh_5V_CheckPowerOn,  "ʹ��5V�Լ��������Ӧ����",   NULL},
	{"5vcheckoff",(bool_t(*)(char*))Sh_5V_CheckPowerOff,"����5V�Լ��������Ӧ����",   NULL},

};
//**************************************************************************
#define CN_POWER_CTRL_SHELL_NUM  sizeof(shell_cmd_power_ctrl_table)/sizeof(struct ShellCmdTab)
static struct ShellCmdRsc tg_power_ctrl_shell_cmd_rsc[CN_POWER_CTRL_SHELL_NUM];



void Peri_NameInit(void)
{
	gs_PeriName[0]="SDRAM";
	gs_PeriName[1]="EMMC";
	gs_PeriName[2]="EMAC";
	gs_PeriName[3]="IMAGE";
	gs_PeriName[4]="CAMERA";
	gs_PeriName[5]="30W";
	gs_PeriName[6]="10W";
	gs_PeriName[7]="SENSOR1";
	gs_PeriName[8]="SENSOR2";
	gs_PeriName[9]="SENSOR3";
	gs_PeriName[10]="SENSOR4";
	gs_PeriName[11]="SENSOR5";
	gs_PeriName[12]="SENSOR6";
	gs_PeriName[13]="SENSOR7";
	gs_PeriName[14]="SENSOR8";
	gs_PeriName[15]="SENSOR9";
	gs_PeriName[16]="SENSOR10";
	gs_PeriName[17]="SENSOR11";
	gs_PeriName[18]="SENSOR12";
	gs_PeriName[19]="SENSOR13";
	gs_PeriName[20]="RF1";
	gs_PeriName[21]="RTC";
	gs_PeriName[22]="ENCRYPTION";
	gs_PeriName[23]="RF2";
	gs_PeriName[25]="BATTERY";
	gs_PeriName[26]="5V_Check";
	gs_PeriName[27]="SAMPLE_RESERVED";
	gs_PeriName[28]="USB_EXPORT";
	gs_PeriName[29]="4G_USB";
    gs_PeriName[30]="4G_UART";
}
/*******************************************************************************
����:CAN����������shellģ��
����:��.
����ֵ:1��
*********************************************************************************/
ptu32_t Power_Ctrl_Shell_Module_Install(void)
{
	Sh_InstallCmd(shell_cmd_power_ctrl_table,tg_power_ctrl_shell_cmd_rsc,CN_POWER_CTRL_SHELL_NUM);
	return 1;
}


//**************************************************************************
static void Sh_PeriStatus(void)
{
	u8 i;
	printf("CPU MCLK(MHz):%d.\r\n",CN_CFG_MCLK/Mhz);
    for(i=0;i<EN_PERI_END;i++)
   {
	  if(gs_PeriStaus[i]==true)
	  {
        printf("%s Power on.\r\n",gs_PeriName[i]);
	  }
	  else
	  {
		printf("%s Power off.\r\n",gs_PeriName[i]);
	  }
   }
   printf("The end........................\r\n");
}




static void Sh_SDRAM_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_SDRAM,true);
	printf("SDRAM Power on.\r\n");

}
static void Sh_SDRAM_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_SDRAM,false);
	printf("SDRAM Power off.\r\n");
}



static void Sh_EMMC_PowerOn(void)
{
	bool_t ret;
	Peri_LowPowerCfg(EN_PERI_EMMC,true);
	ret=MMC_HardInit();
	if(!ret)
	{
		printf("EMMC Power on failed.\r\n");
	}
	printf("EMMC Power on.\r\n");
}
static void Sh_EMMC_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_EMMC,false);
	printf("EMMC Power off.\r\n");
}





static void Sh_EMAC_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_EMAC,true);
	MacReset(0);
	printf("EMAC Power on.\r\n");
}
static void Sh_EMAC_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_EMAC,false);
	printf("EMAC Power off.\r\n");
}



static void Sh_IMAGE_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_IMAGE_PROCESS,true);
	printf("IMAGE Power on.\r\n");
}
static void Sh_IMAGE_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_IMAGE_PROCESS,false);
	printf("IMAGE Power off.\r\n");

}



static void Sh_CAMERA_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_CAMERA_HEADER,true);
	printf("CAMERA Power on.\r\n");
}
static void Sh_CAMERA_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_CAMERA_HEADER,false);
	printf("CAMERA Power off.\r\n");
}


static void Sh_30W_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_30W_POWER,true);
	extern void UART4_GpioInit(void);
	UART4_GpioInit();
	UART_HardInit(CN_UART4);
	printf("30W Power on.\r\n");
}
static void Sh_30W_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_30W_POWER,false);
	printf("30W Power off.\r\n");
}



static void Sh_10W_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_10W_POWER,true);
	extern void UART3_GpioInit(void);
	extern void UART6_GpioInit(void);
	UART3_GpioInit();
	UART6_GpioInit();
	UART_HardInit(CN_UART6);
	UART_HardInit(CN_UART3);
	printf("10W Power on.\r\n");
}
static void Sh_10W_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_10W_POWER,false);
	printf("10W Power off.\r\n");
}



static void Sh_SENSOR1_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR1,true);
	printf("Sensor1 Power on.\r\n");
}
static void Sh_SENSOR1_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR1,false);
	printf("Sensor1 Power off.\r\n");
}

static void Sh_SENSOR2_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR2,true);
	printf("Sensor2 Power on.\r\n");
}
static void Sh_SENSOR2_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR2,false);
	printf("Sensor2 Power off.\r\n");
}

static void Sh_SENSOR3_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR3,true);
	printf("Sensor3 Power on.\r\n");
}
static void Sh_SENSOR3_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR3,false);
	printf("Sensor3 Power off.\r\n");
}

static void Sh_SENSOR4_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR4,true);
	printf("Sensor4 Power on.\r\n");
}
static void Sh_SENSOR4_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR4,false);
	printf("Sensor4 Power off.\r\n");
}
static void Sh_SENSOR5_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR5,true);
	printf("Sensor5 Power on.\r\n");
}
static void Sh_SENSOR5_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR5,false);
	printf("Sensor5 Power off.\r\n");
}

static void Sh_SENSOR6_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR6,true);
	printf("Sensor6 Power on.\r\n");
}
static void Sh_SENSOR6_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR6,false);
	printf("Sensor6 Power off.\r\n");
}
static void Sh_SENSOR7_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR7,true);
	printf("Sensor7 Power on.\r\n");
}
static void Sh_SENSOR7_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR7,false);
	printf("Sensor7 Power off.\r\n");
}

static void Sh_SENSOR8_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR8,true);
	printf("Sensor8 Power on.\r\n");
}
static void Sh_SENSOR8_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR8,false);
	printf("Sensor8 Power off.\r\n");
}

static void Sh_SENSOR9_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR9,true);
	printf("Sensor9 Power on.\r\n");
}
static void Sh_SENSOR9_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR9,false);
	printf("Sensor9 Power off.\r\n");
}
static void Sh_SENSOR10_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR10,true);
	printf("Sensor10 Power on.\r\n");
}
static void Sh_SENSOR10_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR10,false);
	printf("Sensor10 Power off.\r\n");
}

static void Sh_SENSOR11_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR11,true);
	printf("Sensor11 Power on.\r\n");
}
static void Sh_SENSOR11_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR11,false);
	printf("Sensor11 Power off.\r\n");
}


static void Sh_SENSOR12_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR12,true);
	printf("Sensor12 Power on.\r\n");
}
static void Sh_SENSOR12_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR12,false);
	printf("Sensor12 Power off.\r\n");
}

static void Sh_SENSOR13_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR13,true);
	printf("Sensor13 Power on.\r\n");
}
static void Sh_SENSOR13_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_SENSOR13,false);
	printf("Sensor13 Power off.\r\n");
}

static void Sh_RF1_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_RF1,true);
	printf("RF1 Power on.\r\n");
}
static void Sh_RF1_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_RF1,false);
	printf("RF1 Power off.\r\n");
}


static void Sh_RTC_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_RTC,true);
	printf("RTC Power on.\r\n");
}
static void Sh_RTC_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_RTC,false);
	printf("RTC Power off.\r\n");
}

static void Sh_ENCRYPTION_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_ENCRYPTION,true);
	printf("ENCRYPTION Power on.\r\n");
}
static void Sh_ENCRYPTION_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_ENCRYPTION,false);
	printf("ENCRYPTION Power off.\r\n");
}

static void Sh_RF2_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_RF2,true);
	printf("RF2 Power on.\r\n");
}
static void Sh_RF2_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_RF2,false);
	printf("RF2 Power off.\r\n");
}


static void Sh_BAT_PowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_BATTERY_SAMPLE,true);
	printf("BAT Power on.\r\n");

}
static void Sh_BAT_PowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_BATTERY_SAMPLE,false);
	printf("BAT Power off.\r\n");
}

static void Sh_5V_CheckPowerOn(void)
{
	Peri_LowPowerCfg(EN_PERI_5V_SAMPLE,true);
	printf("5V_Check Power on.\r\n");
}
static void Sh_5V_CheckPowerOff(void)
{
	Peri_LowPowerCfg(EN_PERI_5V_SAMPLE,false);
	printf("5V_Check Power off.\r\n");
}





void Module_PeriCtrlInit(void)
{
	Power_Ctrl_Shell_Module_Install();
	Peri_NameInit();
}

// =============================================================================
// ���ܣ����ģ���������Դ���ƺ���
// ������PeriIdx,������;
//     EnableFlag,ʹ�ܱ�ʶ��,true��ʹ��;false,����.
// ���أ���
// =============================================================================
void Peri_LowPowerCfg(enum BoardPeriCode PeriIdx,bool_t Flag)
{
    switch(PeriIdx)
    {
      case EN_PERI_SDRAM:
    	  if(Flag==OutLowPower)
    	  {
//    		  PIO_Clear(&VDD3_3CtrlPin[0]);
    		  SDRAM_LowPowerCfg(OutLowPower);
    		  gs_PeriStaus[EN_PERI_SDRAM]=true;
    	  }
    	  else
    	  {
    		  SDRAM_LowPowerCfg(InLowPower);
//    		  PIO_Set(&VDD3_3CtrlPin[0]);      //SDRAM��Դ���ܹرգ��رյĻ�SDRAM�����ݲ��ܱ���
    		  gs_PeriStaus[EN_PERI_SDRAM]=false;
    	  }
    	  break;
      case EN_PERI_EMMC:
//    	  if(Flag)
//		  {
//    		  PIO_Clear(&VDD3_3CtrlPin[3]);
//			  RCC->APB2ENR |=(1<<11);  //ʹ��SDMMC1ʱ��
//			  EMMC_Wakeup();
//			  gs_PeriStaus[EN_PERI_EMMC]=true;
//		  }
//		  else
//		  {
//			  RCC->APB2ENR &=~(1<<11);  //����SDMMC1ʱ��
//			  EMMC_Sleep();
//			  PIO_Set(&VDD3_3CtrlPin[3]);
//			  gs_PeriStaus[EN_PERI_EMMC]=false;
//		  }
		  break;
      case EN_PERI_EMAC:    //ok
    	  if(Flag==OutLowPower)
		  {
    		  PIO_Clear(&VDD3_3CtrlPin[1]);
    		  GMAC_LowPowerConfig(OutLowPower);
			  gs_PeriStaus[EN_PERI_EMAC]=true;
		  }
		  else
		  {
			  GMAC_LowPowerConfig(InLowPower);
			  PIO_Set(&VDD3_3CtrlPin[1]);
			  gs_PeriStaus[EN_PERI_EMAC]=false;

		  }
		  break;
		  //SR2711-V1.1.0��ɾ���ù���  20170929
      case EN_PERI_IMAGE_PROCESS:
//    	  if(Flag)
//		  {
//    		  PIO_Clear(&VDD3_3CtrlPin[4]); //VDD3.3_KO5
//    		  UART_LowPowerConfig(CN_UART7,UART_OUT_LOWPOWER);
//			  gs_PeriStaus[EN_PERI_IMAGE_PROCESS]=true;
//		  }
//		  else
//		  {
//			  UART_LowPowerConfig(CN_UART7,UART_IN_LOWPOWER);
//			  PIO_Set(&VDD3_3CtrlPin[4]); //VDD3.3_KO5
//			  gs_PeriStaus[EN_PERI_IMAGE_PROCESS]=false;
//		  }
		  break;
      case EN_PERI_CAMERA_HEADER:
		  if(Flag==OutLowPower)
		  {
			  PIO_Set(&VDD12CtrlPin[13]); //VDD12_KO14
			  UART_LowPowerConfig(CN_UART4,OutLowPower);
			  gs_PeriStaus[EN_PERI_CAMERA_HEADER]=true;
		  }
		  else
		  {
			  UART_LowPowerConfig(CN_UART4,InLowPower);
			  PIO_Clear(&VDD12CtrlPin[13]); //VDD12_KO14
			  gs_PeriStaus[EN_PERI_CAMERA_HEADER]=false;
		  }
		  break;
      case EN_PERI_30W_POWER:
    	  if(Flag==OutLowPower)
    	  {
    		  //SR2711-V1.1.0��Ϊ�͵�ƽ��Ч  20170929
//    		  PIO_Set(&VDD12_EN_Pin[1]); //VDD12_EN2
    		  PIO_Clear(&VDD12_EN_Pin[1]); //VDD12_EN2
    		  gs_PeriStaus[EN_PERI_30W_POWER]=true;
    	  }
    	  else
    	  {
    		  //SR2711-V1.1.0��Ϊ�͵�ƽ��Ч  20170929
//    		  PIO_Clear(&VDD12_EN_Pin[1]); //VDD12_EN2
    		  PIO_Set(&VDD12_EN_Pin[1]); //VDD12_EN2
    		  gs_PeriStaus[EN_PERI_30W_POWER]=false;
    	  }
    	  break;
      case EN_PERI_10W_POWER:
    	  if(Flag==OutLowPower)
		  {
    		  //SR2711-V1.1.0��Ϊ�͵�ƽ��Ч  20170929
//			  PIO_Set(&VDD12_EN_Pin[0]); //VDD12_EN1
			  PIO_Clear(&VDD12_EN_Pin[0]); //VDD12_EN
			  gs_PeriStaus[EN_PERI_10W_POWER]=true;
		  }
		  else
		  {
			  //SR2711-V1.1.0��Ϊ�͵�ƽ��Ч  20170929
//			  PIO_Clear(&VDD12_EN_Pin[0]); //VDD12_EN1
			  PIO_Set(&VDD12_EN_Pin[0]); //VDD12_EN1
			  gs_PeriStaus[EN_PERI_10W_POWER]=false;
		  }
		  break;
      case EN_PERI_SENSOR1:
		  if(Flag==OutLowPower)
		  {
			  PIO_Set(&VDD12CtrlPin[0]); //VDD12_KO14
			  UART_LowPowerConfig(CN_UART6,OutLowPower);
			  gs_PeriStaus[EN_PERI_SENSOR1]=true;
		  }
		  else
		  {
			  PIO_Clear(&VDD12CtrlPin[0]); //VDD12_KO14
			  gs_PeriStaus[EN_PERI_SENSOR1]=false;
		  }
		  break;

      case EN_PERI_SENSOR2:
		  if(Flag==OutLowPower)
		  {
			  PIO_Set(&VDD12CtrlPin[1]); //VDD12_KO14
			  UART_LowPowerConfig(CN_UART6,OutLowPower);
			  gs_PeriStaus[EN_PERI_SENSOR2]=true;
		  }
		  else
		  {
			  PIO_Clear(&VDD12CtrlPin[1]); //VDD12_KO14
			  gs_PeriStaus[EN_PERI_SENSOR2]=false;
		  }
		  break;

      case EN_PERI_SENSOR3:
		  if(Flag==OutLowPower)
		  {
			  PIO_Set(&VDD12CtrlPin[2]); //VDD12_KO14
			  UART_LowPowerConfig(CN_UART6,OutLowPower);
			  gs_PeriStaus[EN_PERI_SENSOR3]=true;
		  }
		  else
		  {
			  PIO_Clear(&VDD12CtrlPin[2]); //VDD12_KO14
			  gs_PeriStaus[EN_PERI_SENSOR3]=false;
		  }
		  break;

	   case EN_PERI_SENSOR4:
		  if(Flag==OutLowPower)
		  {
			  PIO_Set(&VDD12CtrlPin[3]); //VDD12_KO14
			  UART_LowPowerConfig(CN_UART6,OutLowPower);
			  gs_PeriStaus[EN_PERI_SENSOR4]=true;
		  }
		  else
		  {
			  PIO_Clear(&VDD12CtrlPin[3]); //VDD12_KO14
			  gs_PeriStaus[EN_PERI_SENSOR4]=false;
		  }
		  break;


	   case EN_PERI_SENSOR5:
		  if(Flag==OutLowPower)
		  {
			  PIO_Set(&VDD12CtrlPin[4]); //VDD12_KO14
			  UART_LowPowerConfig(CN_UART6,OutLowPower);
			  gs_PeriStaus[EN_PERI_SENSOR5]=true;
		  }
		  else
		  {
			  PIO_Clear(&VDD12CtrlPin[4]); //VDD12_KO14
			  gs_PeriStaus[EN_PERI_SENSOR5]=false;
		  }
		  break;

		case EN_PERI_SENSOR6:
		  if(Flag==OutLowPower)
		  {
			  PIO_Set(&VDD12CtrlPin[5]); //VDD12_KO14
			  UART_LowPowerConfig(CN_UART6,OutLowPower);
			  gs_PeriStaus[EN_PERI_SENSOR6]=true;
		  }
		  else
		  {
//			  RCC->APB2ENR &=~(1<<5);  //����UART6ʱ��
			  PIO_Clear(&VDD12CtrlPin[5]); //VDD12_KO14
			  gs_PeriStaus[EN_PERI_SENSOR6]=false;
		  }
		  break;

		case EN_PERI_SENSOR7:
		  if(Flag==OutLowPower)
		  {
			  PIO_Set(&VDD12CtrlPin[6]); //VDD12_KO14
			  UART_LowPowerConfig(CN_UART6,OutLowPower);
			  gs_PeriStaus[EN_PERI_SENSOR7]=true;
		  }
		  else
		  {
			  PIO_Clear(&VDD12CtrlPin[6]); //VDD12_KO14
			  gs_PeriStaus[EN_PERI_SENSOR7]=false;
		  }
		  break;

	   case EN_PERI_SENSOR8:
		  if(Flag==OutLowPower)
		  {
			  PIO_Set(&VDD12CtrlPin[7]); //VDD12_KO14
			  UART_LowPowerConfig(CN_UART6,OutLowPower);
			  gs_PeriStaus[EN_PERI_SENSOR8]=true;
		  }
		  else
		  {
			  PIO_Clear(&VDD12CtrlPin[7]); //VDD12_KO14
			  gs_PeriStaus[EN_PERI_SENSOR8]=false;
		  }
		  break;


	   case EN_PERI_SENSOR9:
		  if(Flag==OutLowPower)
		  {
			  PIO_Set(&VDD12CtrlPin[8]); //VDD12_KO14
			  UART_LowPowerConfig(CN_UART6,OutLowPower);
			  gs_PeriStaus[EN_PERI_SENSOR9]=true;
		  }
		  else
		  {
			  PIO_Clear(&VDD12CtrlPin[8]); //VDD12_KO14
			  gs_PeriStaus[EN_PERI_SENSOR9]=false;
		  }
		  break;

		case EN_PERI_SENSOR10:
		  if(Flag==OutLowPower)
		  {
			  PIO_Set(&VDD12CtrlPin[9]); //VDD12_KO14
			  UART_LowPowerConfig(CN_UART6,OutLowPower);
			  gs_PeriStaus[EN_PERI_SENSOR10]=true;
		  }
		  else
		  {
			  PIO_Clear(&VDD12CtrlPin[9]); //VDD12_KO14
			  gs_PeriStaus[EN_PERI_SENSOR10]=false;
		  }
		  break;

		case EN_PERI_SENSOR11:
		  if(Flag==OutLowPower)
		  {
			  PIO_Set(&VDD12CtrlPin[10]); //VDD12_KO14
			  UART_LowPowerConfig(CN_UART6,OutLowPower);
			  gs_PeriStaus[EN_PERI_SENSOR11]=true;
		  }
		  else
		  {
			  PIO_Clear(&VDD12CtrlPin[10]); //VDD12_KO14
			  gs_PeriStaus[EN_PERI_SENSOR11]=false;
		  }
		  break;

	   case EN_PERI_SENSOR12:
		  if(Flag==OutLowPower)
		  {
			  PIO_Set(&VDD12CtrlPin[11]); //VDD12_KO14
			  UART_LowPowerConfig(CN_UART6,OutLowPower);
			  gs_PeriStaus[EN_PERI_SENSOR12]=true;
		  }
		  else
		  {
			  PIO_Clear(&VDD12CtrlPin[11]); //VDD12_KO14
			  gs_PeriStaus[EN_PERI_SENSOR12]=false;
		  }
		  break;

	   case EN_PERI_SENSOR13:
		  if(Flag==OutLowPower)
		  {
			  PIO_Set(&VDD12CtrlPin[12]); //VDD12_KO14
			  UART_LowPowerConfig(CN_UART6,OutLowPower);
			  gs_PeriStaus[EN_PERI_SENSOR13]=true;
		  }
		  else
		  {
			  PIO_Clear(&VDD12CtrlPin[12]); //VDD12_KO14
			  gs_PeriStaus[EN_PERI_SENSOR13]=false;
		  }
		  break;

      case EN_PERI_RF1:
		  if(Flag==OutLowPower)
		  {
			  PIO_Set(&VDD12CtrlPin[14]); //VDD12_KO15
			  UART_LowPowerConfig(CN_UART3,OutLowPower);
			  gs_PeriStaus[EN_PERI_RF1]=true;
		  }
		  else
		  {
			  UART_LowPowerConfig(CN_UART3,InLowPower);
			  PIO_Clear(&VDD12CtrlPin[14]); //VDD12_KO15
			  gs_PeriStaus[EN_PERI_RF1]=false;
		  }
		  break;
      case EN_PERI_RTC:
		  if(Flag==OutLowPower)
		  {
			  IIC_LowPowerConfig(CN_IIC2,OutLowPower);
			  gs_PeriStaus[EN_PERI_RTC]=true;
		  }
		  else
		  {
			  IIC_LowPowerConfig(CN_IIC2,InLowPower);
			  gs_PeriStaus[EN_PERI_RTC]=false;
		  }
		  break;

      case EN_PERI_ENCRYPTION:
		  if(Flag==OutLowPower)
		  {
			  RCC->APB2ENR |=(1<<12);   //ʹ��SPI1ʱ��
			  gs_PeriStaus[EN_PERI_ENCRYPTION]=true;
		  }
		  else
		  {
			  RCC->APB2ENR &=~(1<<12);     //����SPI1ʱ��
			  gs_PeriStaus[EN_PERI_ENCRYPTION]=false;
		  }
		  break;
      case EN_PERI_BATTERY_SAMPLE:
		  if(Flag==OutLowPower)
		  {
			  PIO_Set(&BATKoPin[0]);    //BAT_KO
			  ADC_LowPowerCfg(OutLowPower);
			  gs_PeriStaus[EN_PERI_BATTERY_SAMPLE]=true;
		  }
		  else
		  {
			  ADC_LowPowerCfg(InLowPower);
			  PIO_Clear(&BATKoPin[0]);   //BAT_KO
			  gs_PeriStaus[EN_PERI_BATTERY_SAMPLE]=false;
		  }
		  break;
      case EN_PERI_5V_SAMPLE:
		  if(Flag==OutLowPower)
		  {
			  ADC_LowPowerCfg(OutLowPower);
			  gs_PeriStaus[EN_PERI_5V_SAMPLE]=true;
		  }
		  else
		  {
			  ADC_LowPowerCfg(InLowPower);
			  gs_PeriStaus[EN_PERI_5V_SAMPLE]=false;
		  }
		  break;

      case EN_PERI_RF2:
    	  if(Flag==OutLowPower)
		  {
			  PIO_Clear(&VDD3_3CtrlPin[2]); //VDD3.3_KO5
			  UART_LowPowerConfig(CN_UART7,OutLowPower);
			  gs_PeriStaus[EN_PERI_RF2]=true;
		  }
		  else
		  {
			  UART_LowPowerConfig(CN_UART7,InLowPower);
			  PIO_Set(&VDD3_3CtrlPin[2]); //VDD3.3_KO5
			  gs_PeriStaus[EN_PERI_RF2]=false;
		  }
		  break;
      case EN_PERI_DEBUG_UART:
    	  if(Flag==OutLowPower)
    	  {
    		  UART_LowPowerConfig(CN_UART1,OutLowPower);
    		  gs_PeriStaus[EN_PERI_DEBUG_UART]=true;
    	  }
    	  else
    	  {
    		  UART_LowPowerConfig(CN_UART1,InLowPower);
    		  gs_PeriStaus[EN_PERI_DEBUG_UART]=false;
    	  }
    	  break;

      default:
    	  break;
    }
}
