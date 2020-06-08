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
#include "stdint.h"
#include "board-config.h"
#include "stdlib.h"
#include "align.h"
#include "cpu_peri.h"
#include "int.h"
#include "string.h"

#include <stdio.h>
#include <def21489.h>
#include <signal.h>
#include <21489.h>

extern "asm" unsigned int ldf_heap_space;
extern "asm" unsigned int ldf_heap_end;
extern   u32 asm_get_tick(void);

u32 *sys_heap_bottom_cnt = &ldf_heap_space;
u32 *sys_heap_top_cnt = &ldf_heap_end;


extern s64  g_s64OsTicks;             //����ϵͳ����ticks��
u32 g_u32CycleSpeed;
u32 g_u32HundreUsfor;
static volatile ufast_t uf_delay_counter;
u32 volatile  core_time;


void SetDelayHandler(int sig_int);

u32 __DjyIsrTick(ptu32_t line);

//----set the instruction delay constant varaible-------------------------------
//function:set the instruction delay to constant, which makes the djy_delay_10us
//         realy got an 10us delay,no matter what compiler and options
//para:null
//return:null
//remarks:sensitive to replant
//        use the ticktimer to test the time
//------------------------------------------------------------------------------
void __DjySetDelay(void)
{
   u32 counter,temp, narrow_cfg_mclk,clksum;
    u32 u32_fors=10000;
    volatile uint32_t u32loops;
    clksum=(u32)(CN_CFG_MCLK/1000);    //1000uS
    atom_high_t high_atom;
    high_atom = Int_HighAtomStart();//dont be interrupt by something
    Int_IsrConnect(cn_int_line_TMZLI,SetDelayHandler);  //���ж�ISR���ж��������
    Int_RestoreAsynLine(cn_int_line_TMZLI);
     while(1)
    {
      timer_set(clksum,clksum);                        //��ʼ����ֵ���Զ�����ֵ����Ϊclksum
      timer_on();                                      //�����ں�ʱ��
      uf_delay_counter=0;
      for(u32loops=u32_fors;u32loops>0;u32loops--);    //����forѭ��
      counter=timer_off();                             //�ر��ں�ʱ��
    if(uf_delay_counter!=0)             //����forѭ�������в����˶�ʱ�жϣ�����������clksum��
                                       //ֱ��forѭ�����й��̲�������ʱ�ж�
    {
        temp = (u32)(uf_delay_counter+1) * clksum;
            if(temp < 65536)
            {
                clksum = (u32)temp;
            }
            else
            {
                clksum = 65535;
                u32_fors = u32_fors / (temp / 65535 +1);
            }
        }
        else
        break;
    }
    counter = clksum - counter;    //ȡʵ����������
    narrow_cfg_mclk=CN_CFG_MCLK/1000000;
    g_u32CycleSpeed = (uint64_t)((counter * 1000) /(u32_fors*narrow_cfg_mclk));//���������64λ
    g_u32HundreUsfor = (1024<<7)/g_u32CycleSpeed;
    Int_HighAtomEnd(high_atom);
    return;
}



//----tick service routine------------------------------------------------------
//function:when the tick comes,will goto this exeption to do something
//para:null
//return:null
//remarks:sensitive to replant
//        this use the fixed timer
//        do something to the delay queque,if permiss shedule,the shedule
//        as you know,it is just soso, just do as usual,on the other hand we
//        use asm file to save the current stack,so the function does not compled
//        all the task that tick isr need
//        ppc use the timer except to do the tick ,so no need line
//        compatibal with th old while use line
//-----------------------------------------------------------------------------

void SetDelayHandler(ptu32_t sig_int)
{
    //todo: �Ż�ʱ����ʵ��ticks��
    uf_delay_counter++;
    //Djy_IsrTick( 1 )
}




u32 __DjyIsrTick(ptu32_t line)
{
    Djy_IsrTick(1);
    return 0;
}

//----init the tick-------------------------------------------------------------
//function:init the tick,we will use the decrement timer to the tick
//         which means that you can use anytime for the tick
//para:null
//return:null
//remarks:sensitive to replant
//        set the tick isr to corresponding place in the exception table, then
//        set the tick timer to appropriate value, the enable it including the
//        the timer interrupt, just soso
//        as you know,it is just soso, but annoyance that use the asm
//-----------------------------------------------------------------------------
void __DjyInitTick(void)
{
    Int_Register(cn_int_line_TMZLI);
    Int_CutLine(cn_int_line_TMZLI);
    Int_IsrConnect(cn_int_line_TMZLI,__DjyIsrTick);  //���ж�ISR���ж��������
    Int_RestoreAsynLine(cn_int_line_TMZLI);
    timer_set(CN_CFG_MCLK/1000,CN_CFG_MCLK/1000);     //��ʱ1ms
    timer_on();
    Int_RestoreAsynLine(cn_int_line_TMZLI);
    return;

}

//----��ȡ��ǰʱ��(uS)---------------------------------------------------------
//���ܣ���ȡ��ǰʱ��(uS),�Ӽ������������������us����64λ��Ĭ�ϲ������
//      g_s64OsTicks Ϊ64λ��������64λϵͳ�У���ȡ g_s64OsTicks ��Ҫ����1��
//      ����,��Ҫʹ��ԭ�Ӳ�����
//��������
//���أ���ǰʱ��
//˵��: ����һ��׮����,��rtc.c�ļ��� DJY_GetSysTime ��������
//-----------------------------------------------------------------------------
s64 __DjyGetSysTime(void)
{
    s64 time;
    u32 narrow_cfg_mclk;
    atom_low_t atom_low;
    atom_low = Int_LowAtomStart();
   //�˴�����cpu.s�л�ຯ��_asm_get_tick��õ�ǰTCOUNTֵ�����ֵ֮��
     core_time=asm_get_tick();
     narrow_cfg_mclk=CN_CFG_MCLK/1000000;
     time = g_s64OsTicks*CN_CFG_TICK_US + (u32)core_time/narrow_cfg_mclk;
     Int_LowAtomEnd(atom_low);
     return time;
}

// =============================================================================
// �������ܣ�djy_poweron_time
//          ��ȡ�ϵ翪ʼ�����ڵ�ʱ��
// �������:
// ���������time����ȡ��ʱ�䣬��λ��΢��
// ����ֵ  ��true�ɹ���falseʧ�ܣ�һ����������޷�ʵ�֣�
// =============================================================================
bool_t DjyPoweronTime(s64 *time)
{
    *time = DJY_GetSysTime();
    return true;
}

