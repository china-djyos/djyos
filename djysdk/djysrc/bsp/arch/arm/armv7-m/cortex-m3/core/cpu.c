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
//����ģ��: ������
//���ߣ�lst
//�汾��V1.0.1
//�ļ�����: ����������CPUֱ����صĴ��롣
//����˵��:
//�޶���ʷ:
//2. ����: 2009-04-24
//   ����: lst
//   �°汾��: V1.0.1
//   �޸�˵��: ɾ����һЩΪdlsp�汾׼���Ķ���
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include "align.h"
#include "stdint.h"
#include "stdlib.h"
#include "int.h"
#include "hard-exp.h"
#include "string.h"
#include "arch_feature.h"
#include "cpu.h"
#include "djyos.h"
#include <board-config.h>

// void __start_systick(void);

#if	(!CN_CFG_USE_USERTIMER)
#define	CN_CFG_SAVE_ASYN	(1U)
#define	CN_TIME_ROUNDING	(32768U)//���������ֵ
#define TIME_GLUE			(CN_CFG_TIME_BASE_HZ>Mhz ? (CN_CFG_TIME_BASE_HZ/Mhz) : (Mhz/CN_CFG_TIME_BASE_HZ))
							//((Mhz%CN_CFG_TIME_BASE_HZ==0) ? (Mhz/CN_CFG_TIME_BASE_HZ) :((float)Mhz/CN_CFG_TIME_BASE_HZ)))
#define FAST_TIME_GLUE		((1<<16)/TIME_GLUE)
#define	TIME_BASE_MIN_GAP	(CN_CFG_TIME_BASE_HZ>Mhz?(500*TIME_GLUE):\
							(TIME_GLUE>=100)?(4*TIME_GLUE):((4+(100/TIME_GLUE))*TIME_GLUE))
static u64 g_time_base_tick=0;
static u32 g_time_base_reload=0;
static bool_t bg_reload_flag = false;
extern void Int_CutTrunk(void);
extern void Int_ContactTrunk(void);
void __InitTimeBase(void)
{
    HardExp_ConnectSystick(Djy_IsrTimeBase);
    pg_systick_reg->reload = 0;
    pg_systick_reg->current = 0;
}

void __DjyStartTimeBase(void)
{
	pg_systick_reg->reload = CN_LIMIT_UINT24;
	pg_systick_reg->current = CN_LIMIT_UINT24;
	g_time_base_reload = CN_LIMIT_UINT24;
	pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //ʹ��
	                            |(1<<bo_systick_ctrl_tickint)   //��������ж�
	                            |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
}

u32 __Djy_GetDelayMaxCnt(void)
{
	return CN_LIMIT_UINT24;
}

u32 __Djy_GetTimeBaseGap(void)
{
	return TIME_BASE_MIN_GAP;
}

void __Djy_SetTimeBaseCnt(u32 cnt)
{
	u32	temp_reload = 0;
	u32 temp_cur = 0;
	if(cnt>CN_LIMIT_UINT24)
	{
		//�����ϲ����ܳ��ִ��¼�
		return;
	}
	if( bg_reload_flag )
		temp_reload = pg_systick_reg->reload;
	else
		temp_reload = g_time_base_reload;
	pg_systick_reg->reload = cnt;
	Int_CutTrunk();
	temp_cur = pg_systick_reg->current;
	pg_systick_reg->current = cnt;
	Int_ContactTrunk();
	g_time_base_tick += temp_reload - temp_cur;
	bg_reload_flag = false;
	g_time_base_reload = pg_systick_reg->reload;
#if	CN_CFG_SAVE_ASYN
	pg_systick_reg->reload = CN_LIMIT_UINT24;
#endif
}

static u32 __Djy_GetTimeBaseRealCnt(void)
{
	if( bg_reload_flag )
		return (pg_systick_reg->reload - pg_systick_reg->current);
	else
		return (g_time_base_reload - pg_systick_reg->current );
}

u32 __Djy_GetTimeBaseReload(void)
{
	return g_time_base_reload;
}

u64 __Djy_TimeBaseUsToCnt(u64 us)
{
	return ((CN_CFG_TIME_BASE_HZ>Mhz)?
			(us*TIME_GLUE):
			((u64)(us*FAST_TIME_GLUE + CN_TIME_ROUNDING))>>16);
}

//ע�⣺�˺���ֻ����SYSTICK�жϴ�������ʹ��
u64 __Djy_GetTimeBaseCnt(u32 cnt)
{
	bg_reload_flag = true;
	g_time_base_tick += cnt;
	return (g_time_base_tick + __Djy_GetTimeBaseRealCnt());
}

u64 __DjyGetSysCnt(void)
{
	u32 cnt=0;
	if(pg_systick_reg->ctrl & bm_systick_ctrl_countflag)
		bg_reload_flag = true;
	cnt = __Djy_GetTimeBaseRealCnt();
	if( bg_reload_flag )
		return (g_time_base_tick+cnt+__Djy_GetTimeBaseReload());
	else
		return (g_time_base_tick+cnt);
}

//----��ȡ��ǰʱ��(uS)---------------------------------------------------------
//���ܣ���ȡ��ǰʱ��(uS),�Ӽ������������������us����64λ��Ĭ�ϲ������
//      g_s64OsTime Ϊ64λ��������64λϵͳ�У���ȡ g_s64OsTime ��Ҫ����1��
//      ����,��Ҫʹ��ԭ�Ӳ�����
//��������
//���أ���ǰʱ��
//˵��: ����һ��׮����,��systime.c�ļ��� DjyGetSysTime �������á�
//      ���systime��ʹ��ticks��Ϊʱ�����������ɱ��ֿպ�����
//-----------------------------------------------------------------------------
u64 __DjyGetSysTime(void)
{
    u64 time;
    u64 temp=0;
    atom_low_t atom_low;

    atom_low = Int_LowAtomStart();
    temp = __DjyGetSysCnt();
    time = ((CN_CFG_TIME_BASE_HZ>Mhz)?
    		(temp/(u32)TIME_GLUE):
			(temp*TIME_GLUE));
    Int_LowAtomEnd(atom_low);
    return time;
}
#endif

extern void Init_Cpu(void);
extern void Load_Preload(void);
// =============================================================================
// ���ܣ����е�ѡ��ϵͳ���з�ʽǰ������M3/M4��CPU����PC��ת��Init_CPU()
// ��������
// ���أ���
// =============================================================================
void reboot(void)
{
    u32 InitCpu_Addr;
    InitCpu_Addr = *(u32*)0x00000004;
    ((void (*)(void))(InitCpu_Addr))();
}
// =============================================================================
// ���ܣ�ResetӲ��CPU���൱���ϵ�����������Ӳ��������õ���λ
// ��������
// ���أ���
// =============================================================================
void reset(void)
{
    pg_scb_reg->AIRCR = (0x05FA << 16)|(0x01 << bo_scb_aircr_sysresetreq);
}
// =============================================================================
// ���ܣ����е�CPU���ش���ǰ����pre_load()ǰ
// ��������
// ���أ���
// =============================================================================
void restart_system(void)
{
    Load_Preload();
}

