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
#include "djyos.h"
#include "cpu.h"
#include "cpu_peri.h"
#include "board-config.h"

#if (!CN_CFG_USE_USERTIMER)
#define CN_TIME_ROUNDING    (32768U)//���������ֵ
#define TIME_GLUE           (CN_CFG_TIME_BASE_HZ>Mhz?(CN_CFG_TIME_BASE_HZ/Mhz):(Mhz/CN_CFG_TIME_BASE_HZ))
#define FAST_TIME_GLUE      ((1<<16)/TIME_GLUE)
#define TIME_BASE_MIN_GAP   (CN_CFG_TIME_BASE_HZ>Mhz?(100*TIME_GLUE):\
                            (TIME_GLUE>=100)?(4*TIME_GLUE):((4+(100/TIME_GLUE))*TIME_GLUE))
static u64 g_time_base_tick=0;
static u32 __DjyIsrTime(ptu32_t line);
//----��ʼ��tick---------------------------------------------------------------
//����: ��ʼ����ʱ��,������tick�жϺ���,������ʱ��.
//����: ��
//����: ��
//��ע: ����������ֲ���к���.
//-----------------------------------------------------------------------------
void __InitTimeBase(void)
{
    Int_Register(CN_INT_LINE_TIMER3);
    Int_CutLine(CN_INT_LINE_TIMER3);
    //����timer3��Ϊtick�ж�
    Int_IsrConnect(CN_INT_LINE_TIMER3,__DjyIsrTime);
    Int_SettoAsynSignal(CN_INT_LINE_TIMER3);          //tick�жϱ���Ϊ�첽�ź�
    //�������ö�ʱ������������board-config.h��CN_CFG_TICK_US��CN_CFG_TICK_HZ��
    //CN_CFG_FINE_US��CN_CFG_FINE_HZ�Ķ���һ��
    Timer_SetClkSource(3,0);          //pclkԤ��Ƶ����1/2��Ƶ
    Timer_SetPrecale(1,CN_CFG_TIMER_CLK/CN_CFG_TIME_BASE_HZ/2 -1);
    //���ϰѶ�ʱ������ʱ�ӵ�Ƶ����ΪCN_CFG_FINE_HZ
    //Timer_SetCounter(3,(uint16_t)((u32)CN_CFG_TICK_US*1000/CN_CFG_FINE_US));
    Timer_SetCounter(3, CN_LIMIT_UINT16);
    Timer_SetType(3,1);                //����tick��ʱ����������
    Timer_Reload(3);                    //���ض�ʱֵ
}

void __DjyStartTimeBase(void)
{
    Timer_Start(3);                     //������ʱ��
    Int_RestoreAsynLine(CN_INT_LINE_TIMER3);//����tick�ж�S
}

//��ȡ���Զ�ʱ�����ֵ
u32 __Djy_GetDelayMaxCnt(void)
{
    return CN_LIMIT_UINT16;
}

u32 __Djy_GetTimeBaseGap(void)
{
    return TIME_BASE_MIN_GAP;
}

//----��������tick�ж�ʱ��---------------------------------------------------------------
//����: �������ö�ʱ��reloadֵ.
//����: time:��λus
//����: ��
//��ע: ����������ֲ���к���.
//������ڲ����ú��������ﲻ��time��ֵ���жϣ��ɵ�����ȷ����ȷ��
//-----------------------------------------------------------------------------
void __Djy_SetTimeBaseCnt(u32 cnt)
{
    if(cnt>CN_LIMIT_UINT16)
    {
        //�����ϲ����ܳ��ִ��¼�
        return;
    }
    g_time_base_tick += pg_timer_reg->TCNTB3 - pg_timer_reg->TCNTO3;
    pg_timer_reg->TCNTB3 = cnt;
    pg_timer_reg->TCNTO3 = pg_timer_reg->TCNTB3;
}

//----��ȡtick�ж�ʱ��---------------------------------------------------------------
//����: ��ȡ����ʱ�жϷ���ʱ��reload��current�Ĳ�ֵ.
//����: ��
//����: reload��Ӧ��USֵ
//��ע: ����������ֲ���к���.
//-----------------------------------------------------------------------------
static u32 __Djy_GetTimeBaseRealCnt(void)
{
    return (pg_timer_reg->TCNTB3 - pg_timer_reg->TCNTO3);
}

//----��ȡ��ʱ����reloadֵ��Ӧ��USֵ---------------------------------------------------------------
//����: ��ȡ��ʱ��reloadֵ��Ӧ��USֵ.
//����: ��
//����: reload��Ӧ��USֵ
//��ע: ����������ֲ���к���.
//-----------------------------------------------------------------------------
u32 __Djy_GetTimeBaseReload(void)
{
    return (pg_timer_reg->TCNTB3);
}

u64 __Djy_TimeBaseUsToCnt(u64 us)
{
    return ((CN_CFG_TIME_BASE_HZ>Mhz)?
            (us*TIME_GLUE):
            (us*FAST_TIME_GLUE + CN_TIME_ROUNDING)>>16);
}

u64 __Djy_GetTimeBaseCnt(u32 cnt)
{
    g_time_base_tick += cnt;
    return g_time_base_tick;
}

u64 __DjyGetSysCnt(void)
{
    u32 cnt=0;
    cnt = __Djy_GetTimeBaseRealCnt();
//  if(pg_systick_reg->ctrl & bm_systick_ctrl_countflag)
//      return (g_time_base_tick+cnt+__Djy_GetTimeBaseReload());
//  else
        return (g_time_base_tick+cnt);
}


//----��ȡ��ǰʱ��(uS)---------------------------------------------------------
//���ܣ���ȡ��ǰʱ��(uS),�Ӽ������������������us����64λ��Ĭ�ϲ������
//      g_s64OsTime Ϊ64λ��������64λϵͳ�У���ȡ g_s64OsTime ��Ҫ����1��
//      ����,��Ҫʹ��ԭ�Ӳ�����
//��������
//���أ���ǰʱ��
//˵��: ����һ��׮����,��systime.c�ļ��� DJY_GetSysTime �������á�
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
           (temp*FAST_TIME_GLUE+(u64)CN_TIME_ROUNDING)>>16:
            (temp*TIME_GLUE));
    Int_LowAtomEnd(atom_low);
    return time;
}

static u32 __DjyIsrTime(ptu32_t line)
{
    u32 tick=0;
    tick = __Djy_GetTimeBaseReload();
    Djy_IsrTimeBase(tick);
    return 0;
}

#endif

// =============================================================================
// ���ܣ����е�ѡ��ϵͳ���з�ʽǰ������M3/M4��CPU����PC��ת��Init_CPU()
// ��������
// ���أ���
// =============================================================================
void CPU_Reboot(void)
{

}
// =============================================================================
// ���ܣ�ResetӲ��CPU���൱���ϵ�����������Ӳ��������õ���λ
// ��������
// ���أ���
// =============================================================================
void CPU_Reset(void)
{
    volatile u32 *SWRST = (u32*)0x4C000044;

#if CN_CPU_OPTIONAL_CACHE == 1
    Cache_CleanData();
    Cache_InvalidInst();
#endif
    *SWRST = 0x533C2416;
}
// =============================================================================
// ���ܣ����е�CPU���ش���ǰ����pre_load()ǰ
// ��������
// ���أ���
// =============================================================================
void CPU_RestartSystem(void)
{

}

