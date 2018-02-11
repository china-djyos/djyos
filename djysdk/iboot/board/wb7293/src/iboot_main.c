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
#include "string.h"
#include "IAP.h"
#include <cfg/iboot_config.h>
#include "cpu_peri.h"

//extern u32 g_bRunModeFlag;
const Pin LedPin[] = {
		{PIO_PA27,PIOA,ID_PIOA,PIO_OUTPUT_1,PIO_DEFAULT}
};

static const Pin RUN[] = {
        {PIO_PD26, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}

};


void Led_Blink(void)
{
	PIO_Configure(RUN,1);
	while(1)
	{
		PIO_Set(RUN);
        Djy_DelayUs(1*mS);
		PIO_Clear(RUN);
		Djy_DelayUs(1*mS);
	}
}

#include <dcoutput.h>
#include <shell.h>
static char *Sh_GetWord_r(char *buf,char **next)
{
    uint32_t i=0;
    *next = NULL;
    if(buf == NULL)
        return NULL;
    while(1)
    {
        if((buf[i] == ' ') || (buf[i] == 0))
        {
            if(buf[i] == 0)
                return buf;
            else
            {
                buf[i] = '\0';
                break;
            }
        }
        i++;
    }
    i++;
    while(buf[i] != 0)
    {
        if(buf[i]!=' ')
        {
            if((buf[i] == '\n') || (buf[i] == '\r'))
                *next = NULL;
            else
                *next = &buf[i];
            break;
        }
        i++;
    }
    return buf;
}
bool_t Shell_DC(char *param)
{
	u32 channel,cycle,dc;
	char *param_ch,*param_dc,*param_cycle,*next_param;

	param_ch = Sh_GetWord_r(param,&next_param);
//	param_cycle= Sh_GetWord_r(next_param,&next_param);
	param_dc = Sh_GetWord_r(next_param,&next_param);

	channel = strtoul(param_ch, (char **)NULL, 0);
	dc = strtol(param_dc, (char **)NULL, 0);
//	cycle = strtol(param_cycle, (char **)NULL, 0);

	if(channel > 1)
		printk("channel err!\r\n");
	if( (dc < 4000) || (dc > 20000))
		printk("dc value err!\r\n");

	DC_Output(channel,dc);

	return 1;
}

static struct ShellCmdTab  gVmacDebug[] =
{
    {
        "dc",
		Shell_DC,
        "usage:Virtual_Mac",
        NULL
    }
};
#define CN_VMACDEBUG_NUM  ((sizeof(gVmacDebug))/(sizeof(struct ShellCmdTab)))
static struct ShellCmdRsc gVmacDebugCmdRsc[CN_VMACDEBUG_NUM];
//ϵͳ������
ptu32_t djy_main(void)
{
	extern void Sh_GetStatus(char *param);
	extern void Sh_GetRunMode(char *param);
	Sh_GetRunMode(NULL);
	Sh_GetStatus(NULL);

	PIO_Configure(LedPin,1);

	Sh_InstallCmd(gVmacDebug,gVmacDebugCmdRsc,CN_VMACDEBUG_NUM);
	while(1)
	{
		PIO_Set(LedPin);
        Djy_EventDelay(500*mS);
		PIO_Clear(LedPin);
        Djy_EventDelay(500*mS);
	}

	return 0;
}

