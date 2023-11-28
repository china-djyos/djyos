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
//����ģ��: BSP
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ����cortex-mϵ��CPU��systick��Ϊϵͳticksʱ��Դ��Ĭ�Ϲ�ѡ����dynamic-tick
//         ģʽ�������͹���Ҫ��ߵĳ��ϣ�Ӧʹ�õ͹��Ķ�ʱ�������֮��
//����˵��:
//1. ����: 2019-08-11
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include <stdint.h>
#include <cpu.h>
#include <int.h>
#include <cpu_peri_int_line.h>
#include <int_hard.h>
#include "cpu_peri.h"

extern void __DJY_ScheduleAsynSignal(void);
void SysTickTimer(void);

#define CFG_TICKMODE_DYNAMIC        false
#define CFG_REAL_CRITICAL           100     //�����뵱ǰtick�ж�ʱ��С�ڴ� uS �������޸�tick�ж�ʱ��

static s32 s_gCurrentTicks = 1;     //��ǰtick�������ڡ�
static s32 s_gCriticalCycle;        //CFG_REAL_CRITICAL��Ӧ�Ķ�ʱ������ֵ
static s32 s_gTicksLimit;           //24bit��ʱ�����ζ�ʱ���ticks��
extern s64  g_s64OsTicks;

//----��ʼ��tick---------------------------------------------------------------
//����: ��ʼ����ʱ��,������tick�жϺ���,������ʱ��.
//����: ��
//����: ��
//��ע: ����������ֲ���к���.
//      weak�������û�Ҳ������boarddrv������ʵ�֡�
//-----------------------------------------------------------------------------
__attribute__((weak)) void __DjyInitTick(void)
{
    Int_Register(CN_INT_LINE_TIMER2);
    Int_CutLine(CN_INT_LINE_TIMER2);
    //����timer2��Ϊtick�ж�
    Int_IsrConnect(CN_INT_LINE_TIMER2,SysTickTimer);
    Int_SettoAsynSignal(CN_INT_LINE_TIMER2);          //tick�жϱ���Ϊ�첽�ź�
    //�������ö�ʱ������������board-config.h��CN_CFG_TICK_US��CN_CFG_TICK_HZ��
    //CN_CFG_FINE_US��CN_CFG_FINE_HZ�Ķ���һ��
    Timer_SetClkSource(2,1);            //ʱ��Դ��Ϊ24M
    Timer_SetPrecale(2,0);              //Ԥ��Ƶ��Ϊ1��������Ƶ
    Timer_SetInterval(2, CN_CFG_TICK_US*24);    //24M
    Timer_SetAutoReload(2);                 //����tick��ʱ����������
    Timer_Start(2);                         //������ʱֵ
    Timer_EnableInt(2);
    Int_RestoreLine(CN_INT_LINE_TIMER2);
}

//------------------------------------------------------------------------------
//���ܣ�����CPU�����߻����¼�״̬��ʹCPU�ڹ��жϺ�ȷʵ�ܱ������ڹ��ж��ڼ�Ļ����¼�
//     ���жϻ�ദ����źţ����ѣ����ۻ����¼������� WFE��ARM��ָ�����CPU���ܲ�ͬ��
//     ֮ǰ����֮��
//��������
//���أ���
//ע�����������͵ر����ڵ͹��Ŀ��ơ�
//------------------------------------------------------------------------------
void CleanWakeupEvent(void)
{
}

// =============================================================================
// ����:��Cortex Mϵͳ�ں��У��ں�ϵͳ��ʱ��SYSTICK��ʱ�ж������쳣���������������첽�ź�
//     ��ں��������±��� g_s64OsTicks ������һ��tick�����Ϊ1��tick��
// ����:��
// ����:��
// =============================================================================
void SysTickTimer(void)
{
    s32 inc;
    Timer_ClearInt(2);
//    g_bScheduleEnable = false;
//    tg_int_global.en_asyn_signal_counter++;
//    tg_int_global.nest_asyn_signal++;
    inc = s_gCurrentTicks;
    s_gCurrentTicks = 1;
    DJY_ScheduleIsr(inc);
//    tg_int_global.nest_asyn_signal--;
//    tg_int_global.en_asyn_signal_counter--;
//    if((g_ptEventReady != g_ptEventRunning) && (tg_int_global.nest_asyn_signal==0))
//        __DJY_ScheduleAsynSignal();       //ִ���ж��ڵ���
//    g_bScheduleEnable = true;
}

// =============================================================================
// ���ܣ���ȡ��tick������ϵͳʱ�䣬���bsp���ʹ����ר�ö�ʱ���ṩϵͳʱ�ӣ�����Բ��ṩ
//      ����������ȡʱ���� g_s64OsTicks ���϶�ʱ���ĵ�ǰ����ֵ��
//      ��ȡ��ǰ����ֵ�ر�Ҫע�⣬����ڹ��첽�ź��ڼ���ñ��������ڼ䷢���˶�ʱ�����أ�
//      ����ǡ���ڱ������ڹ��жϺ����ڶ� cycle ֮ǰ�����˶�ʱ�����أ�����Ҫ����ʱ����ת
//      �����⡣
// ��������
// ���أ���ǰus��
// =============================================================================
__attribute__((weak))   u64 __DjyGetSysTime(void)
{
    s64 current;
    s32 cyclep,cyclen,interval,reload;
    bool_t ctrl;
    atom_low_t atom_low;

    atom_low = Int_LowAtomStart();
    interval = s_gCurrentTicks;
    cyclep = Timer_GetCounter(2);
    reload = Timer_GetInterval(2);
    ctrl = Timer_GetIntStatus(2);
    cyclen = Timer_GetCounter(2);
    current = g_s64OsTicks;
    Int_LowAtomEnd(atom_low);
    if(cyclen > cyclep)     //��������˵���ڶ��Ĵ��������з����˶�ʱ������
    {
        cyclen = reload - cyclen;   //����ֵ - ��ǰֵ
        current += interval;
    }
    else                    //�ڶ��Ĵ���������δ������ʱ�����أ��������ų�֮ǰ������
    {
        if(ctrl)      //˵�����Ĵ���ǰ��ʱ���Ѿ������ˣ����жϻ�û����Ӧ��
        {
            cyclen = reload - cyclen;   //����ֵ - ��ǰֵ
            current += interval;
        }
        else
        {
            cyclen =(s32)((s64)interval*24000000 - cyclen);
        }
    }
    current = current*CN_CFG_TICK_US + ((s64)cyclen*CN_CFG_FINE_US >>16);

    return (u64)current;
}

//----�����´�ticks�ж�ʱ��-----------------------------------------------------
//���ܣ��㶨tickģʽ�£�����tick��������1��tick����ֱ�ӷ���1����ʱ������ά�ֲ��䡣
//     ��̬tickģʽ�£�ͨ���޸Ķ�ʱ���������ѵ���tick�����Ϊ����Ticks������ֻ��idle
//      �¼�����ʱ���Ż�����tick�������CPU�����Ѻ�idle����һ���ܼ�ʱ�������ʱ�䣬
//      ��ÿ�ζ�ʱ����������жϼ����Ϊ1����ȷ��idleû�л������е��ڼ䣬ticks������
//      �жϡ�����Ticks�ĺ��壬�Ǵ��ϴ�tick�ж�����ģ�����ʱ���ǳ����������ߵģ����
//      ��Ҫ���Ǵ��ϴ�tick�ж�������ʱ���Ѿ��ߵ�ʱ�䣬�������ඨʱ���в�ͬ�Ĵ���
//     ��ʱ���������ࣺ
//     1����0�������趨ֵ��Ȼ�������жϣ���ʱ����0��ʼ���¼�ʱ�����ֶ�ʱ����ֻ��Ҫ�޸���
//        ��ֵ���ɣ����뿼�ǵ�ǰ�Ѿ��߹���ʱ�䡣���´��жϼ����Ϊ1�Ĺ�����tick�жϵ�ISR
//        ����ɡ�
//     2������cortex-m��24λsystick��ʱ������reload��ʼ��������0��Ȼ���reload���¿�ʼ
//        �����ģ��������굱��tick�󣬻�Ҫ��reload���õ�1��tick���ر�ע�⣬���ö�ʱ��
//        ʱ��Ҫ��ȥ���ϴ�ISR�ж�������ʱ���Ѿ��߹���ʱ�䡣
//
//������ticks,ϣ�����´��жϾ��ϴε��жϼ����
//���أ�ʵ�ʵ��´��жϾ��ϴε��жϼ����
//-----------------------------------------------------------------------------
__attribute__((weak)) u32 Tick_SetNextTimeTick(s32 Ticks)
{
#if CFG_TICKMODE_DYNAMIC == true
    u32 temp;
    atom_high_t atom_high;
    if(Ticks > s_gTicksLimit)
        Ticks = s_gTicksLimit;
    if(s_gCurrentTicks == Ticks)
        return Ticks;
    temp = pg_systick_reg->current;
    //tick�жϿ쵽�ˣ�Ϊ�ܹ������ٽ������µ����⣬������ı�tickʱ��
    //���ڵ��ñ�����ʱû�йر�ʵʱ�жϣ���ʵʱ�жϵ�ISRִ�в��ܴ���CFG_REAL_CRITICAL΢��
    if(temp >= s_gCriticalCycle)
    {
        if(s_gCurrentTicks > Ticks)
        {
            temp = (s_gCurrentTicks - Ticks )* (s64)CN_CFG_FCLK / CN_CFG_TICK_HZ;

            atom_high = Int_HighAtomStart();
            //��������ʱ���и��̶���ȱ�ݣ���������������д�ؼ���ֵ�Ĺ���ʱ�����������ʱ���
            //��Ȼ����С����������
            //�Ӽ����ľͲ��������⡣
            temp = Timer_GetCounter(2) - temp;
            //ֻ��Ҫ�ı�current�Ĵ�����Timer_GetCounter(2)�ϻ��ȥ��ticks��ԭticks�Ĳ�ֵ����
            Timer_SetCounter(2,temp);
            Int_HighAtomEnd(atom_high);
        }
        else
        {
            temp = (Ticks - s_gCurrentTicks )* (s64)CN_CFG_FCLK / CN_CFG_TICK_HZ;

            atom_high = Int_HighAtomStart();
            //��������ʱ���и��̶���ȱ�ݣ���������������д�ؼ���ֵ�Ĺ���ʱ�����������ʱ���
            //��Ȼ����С����������
            //�Ӽ����ľͲ��������⡣
            temp = Timer_GetCounter(2) + temp;
            //ֻ��Ҫ�ı�current�Ĵ�����Timer_GetCounter(2)�ϻ��ȥ��ticks��ԭticks�Ĳ�ֵ����
            Timer_SetCounter(2,temp);
            Int_HighAtomEnd(atom_high);
        }
//        pg_systick_reg->reload = 0xffffff;
        s_gCurrentTicks = Ticks;
    }
    return s_gCurrentTicks;

#else       // for CFG_TICKMODE_DYNAMIC == true
    return 1;
#endif      // for CFG_TICKMODE_DYNAMIC == true
}

