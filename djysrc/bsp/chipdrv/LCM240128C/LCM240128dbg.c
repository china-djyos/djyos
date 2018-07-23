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
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��LCM240128dbg.c
// ģ������:  LCM240128������Ϣ
// ģ��汾: V1.00
// ������Ա: zhb
// ����ʱ��: 09/03.2017
// =============================================================================
/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include <assert.h>
#include "cpu_peri.h"
#include "ring.h"
#include "os.h"
#include "stdlib.h"
#include "shell.h"
#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "string.h"

static void Sh_LCD_Reset(void);
static bool_t Sh_LCD_BackLight(char *param);
static void Sh_Get_AdjustVal(void);
static void Sh_Earse_AdjustVal(void);
static void Sh_Get_TouchPoint(void);
static void Sh_LCD_Status(void);

extern void LCD_Reset(void);
extern void LCD_BackLight(bool_t OnOff);
extern void Touch_GetAdjustVal(void);
extern bool_t Touch_EraseAdjustVal(void);
extern void Touch_GetTouchPoint(void);
extern void InitLCM240128C(void);
extern u8 LCD_GetStatus(void);


struct ShellCmdTab const shell_cmd_lcd_table[]=
{
	{
			"lcdrst",
			Sh_LCD_Reset,
			"reset lcd",
			"COMMAND:lcdrst+enter"
	},
	{
			"lcdbl",
			Sh_LCD_BackLight,
			"On/Off lcd back light",
			"COMMAND:lcdbl+1/0(1:On 0:Off)+enter"
	},
	{
			"lcdsta",
			Sh_LCD_Status,
			"get lcd status",
			"COMMAND:lcdsta+enter"
	},
	{
			"gtav",
			Sh_Get_AdjustVal,
			"get touch lcd adjust val",
			"COMMAND:gtav+enter"
	},
	{
			"etav",
			Sh_Earse_AdjustVal,
			"erase touch lcd adjust val",
			"COMMAND:etav+enter"
	},
	{
			"gtp",
			Sh_Get_TouchPoint,
			"get the location of touch point",
			"COMMAND:gtp+enter"
	}
};

#define CN_LCD_SHELL_NUM  sizeof(shell_cmd_lcd_table)/sizeof(struct ShellCmdTab)
static struct ShellCmdRsc tg_lcd_shell_cmd_rsc[CN_LCD_SHELL_NUM];


/*******************************************************************************
����:LCD����shellģ��
����:��.
����ֵ:1��
*********************************************************************************/
ptu32_t LCD_Shell_Module_Install(void)
{
	Sh_InstallCmd(shell_cmd_lcd_table,tg_lcd_shell_cmd_rsc,CN_LCD_SHELL_NUM);
	return 1;
}


static void Sh_LCD_Reset(void)
{
	LCD_Reset();
	InitLCM240128C( );
}

static void Sh_LCD_Status(void)
{
   u8 status;
   status=LCD_GetStatus();
   printf("status:%x\r\n",status);
}
static bool_t Sh_LCD_BackLight(char *param)
{
	char *word_OnOff,*word_trail,*next_param;
	uint8_t byOnOff;
	//��ȡ1������
	extern char *Sh_GetWord(char *buf,char **next);
	word_OnOff = Sh_GetWord(param,&next_param);
	word_trail = Sh_GetWord(next_param,&next_param);
	if((word_OnOff == NULL)||(word_trail != NULL))
	{
	  printf("\r\n��ʽ������ȷ��ʽ�ǣ�\r\n>d 1:Light LCD/0:Off Light.\r\n");
	  return false;
	}
	byOnOff = strtol(word_OnOff, (char **)NULL, 0);
	if(byOnOff==1)
	{
		LCD_BackLight(1);
	}
	else if(byOnOff==0)
	{
		LCD_BackLight(0);
	}
	else
	{
		printf("Para is invalid,1:Light LCD/0:Off Light.\r\n");
	}
	return true;
}
static void Sh_Get_AdjustVal(void)
{
	Touch_GetAdjustVal();
}
static void Sh_Earse_AdjustVal(void)
{
	Touch_EraseAdjustVal();
}
static void Sh_Get_TouchPoint(void)
{
	Touch_GetTouchPoint();
}
