//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
#include <shell.h>


static bool_t lcdrst(void);
static bool_t lcdbl(char *param);
static bool_t gtav(void);
static bool_t etav(void);
static bool_t gtp(void);
static bool_t lcdsta(void);

extern void LCD_Reset(void);
extern void LCD_BackLight(bool_t OnOff);
extern void Touch_GetAdjustVal(void);
extern bool_t Touch_EraseAdjustVal(void);
extern void Touch_GetTouchPoint(void);
extern void InitLCM240128C(void);
extern u8 LCD_GetStatus(void);

//static void Sh_LCD_Reset(void)
bool_t lcdrst(void)
{
    LCD_Reset();
    InitLCM240128C( );
    return true;
}

//static void Sh_LCD_Status(void)
bool_t lcdsta(void)
{
   u8 status;
   status=LCD_GetStatus();
   printf("status:%x\r\n",status);
   return true;
}
//static bool_t Sh_LCD_BackLight(char *param)
 bool_t lcdbl(char *param)
{
    char *word_OnOff,*word_trail,*next_param;
    uint8_t byOnOff;
    //��ȡ1������
    extern char *shell_inputs(char *buf,char **next);
    word_OnOff = shell_inputs(param,&next_param);
    word_trail = shell_inputs(next_param,&next_param);
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
//static void Sh_Get_AdjustVal(void)
bool_t gtav(void)
{
    Touch_GetAdjustVal();
    return true;
}
//static void Sh_Earse_AdjustVal(void)
bool_t etav(void)
{
    Touch_EraseAdjustVal();
    return true;
}
//static void Sh_Get_TouchPoint(void)
bool_t gtp(void)
{
    Touch_GetTouchPoint();
    return true;
}
ADD_TO_ROUTINE_SHELL(gtp,gtp,"get the location of touch point  COMMAND:gtp+enter");
ADD_TO_ROUTINE_SHELL(etav,etav,"erase touch lcd adjust val  COMMAND:etav+enter");
ADD_TO_ROUTINE_SHELL(gtav,gtav,"get touch lcd adjust val  COMMAND:gtav+enter");
ADD_TO_ROUTINE_SHELL(lcdbl,lcdbl,"On/Off lcd back light  COMMAND:lcdbl+1/0(1:On 0:Off)+enter");
ADD_TO_ROUTINE_SHELL(lcdsta,lcdsta,"get lcd status  COMMAND:lcdsta+enter");
ADD_TO_ROUTINE_SHELL(lcdrst,lcdrst,"reset lcd  COMMAND:lcdrst+enter");
