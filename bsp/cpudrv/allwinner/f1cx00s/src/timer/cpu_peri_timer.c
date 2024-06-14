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
// ģ������: PIC�������ϵ�TIMER������
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 1:55:12 PM/Mar 4, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri_timer.h"
#include "int.h"
#include "cpu.h"
#include "cpu_peri_int_line.h"
#include "timer_hard.h"
#include "board-config.h"
// =============================================================================
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_HardTimer(void);
//    ModuleInstall_HardTimer();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip timer"//CPU�Ķ�ʱ����������
//parent:"none"//��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"  //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_TIMER == false )
//#warning  " cpu_onchip_timer  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_TIMER    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

#define CN_TIMER_FREQ           24000000    //����Ƶ�ʹ̶�Ϊ24M
#define CN_MAX_COUNTER     0xFFFFFFFF   //32λ��ʱ����������ֵ
//��ʱ��������ʱ���ڣ�uS��
#define CN_MAX_TIME_US     (0x100000000/CN_TIMER_FREQ*1000000)

//�о�timerģ�����ġ������� Timer_Alloc ��������Ķ�ʱ��Ӳ�����
enum ENUM_TIMER
{
    EN_TIMER_0=0,
    EN_TIMER_1=1
};

struct F1cx00Timer *pg_tTimerReg = (struct F1cx00Timer *)F1C100S_TIMER_BASE;

//������ʱ��оƬ�Ķ�ʱ��Ӧ�����Լ��ľ��
struct TimerHandle
{
    u16     timerno;          //��ʱ����
    u16     irqline;          //�жϺ�
    u32     cycle;            //��ʱ����
    u32     timerstate;       //��ʱ����ʶ
    fnTimerIsr UserIsr;       //�û��ж���Ӧ����
};

#define CN_TIMER_NUM   2

static struct TimerHandle  stgTimerHandle[CN_TIMER_NUM];

//timer0..timern��irq
static u32 sgHaltimerIrq[CN_TIMER_NUM]={    CN_INT_LINE_TIMER0,
                                            CN_INT_LINE_TIMER1
                                            };

//----���ö�ʱ����ǰֵ-------------------------------------------------------
//����: ���ø���ʱ����ǰֵ��������.
//����: timer,��ʱ��Ӳ�����
//      cycle,��ʱ��Ԥ��Ƶ��,0~7��Ӧ1~128��Ƶ
//����: ��
//-----------------------------------------------------------------------------
void Timer_EnableInt(ufast_t timer)
{
    if(timer >2)    //c100��3����ʱ��
        return;
    pg_tTimerReg->IrqEn |= 1<<timer;
}
//----ȡ��ʱ����ǰֵ-------------------------------------------------------
//����:ȡ��ʱ����ǰֵ��������.
//����: timer,��ʱ��Ӳ�����
//����: ��ʱ����ǰֵ
//-----------------------------------------------------------------------------
void Timer_DisableInt(ufast_t timer)
{
    if(timer >2)    //c100��3����ʱ��
        return;
    pg_tTimerReg->IrqEn &= ~(1<<timer);
}//----���ö�ʱ��ʱ��Դ----------------------------------------------------------
//����: ���ø���ʱ��ʱ��Դ.
//����: timer,��ʱ��Ӳ�����
//      value,��ʱ������ʱ��Դ,1=24M,0=32768
//����: ��
//-----------------------------------------------------------------------------
void Timer_SetClkSource(ufast_t timer,ufast_t value)
{
    if(timer >2)    //c100��3����ʱ��
        return;
    pg_tTimerReg->Cell[timer].TimerCtrl &= ~(3<<2);
    pg_tTimerReg->Cell[timer].TimerCtrl |= (value<<2);
}
//----���ö�ʱ��Ԥ��Ƶ��-------------------------------------------------------
//����: ���ø���ʱ��Ԥ��Ƶ��.
//����: timer,��ʱ��Ӳ�����
//      value,��ʱ��Ԥ��Ƶ��,0~7��Ӧ1~128��Ƶ
//����: ��
//-----------------------------------------------------------------------------
void Timer_SetPrecale(ufast_t timer,uint16_t value)
{
    if(timer >2)    //c100��3����ʱ��
        return;
    pg_tTimerReg->Cell[timer].TimerCtrl &= ~(7<<4);
    pg_tTimerReg->Cell[timer].TimerCtrl |= (value<<4);
}
//----���ö�ʱ���жϼ��-------------------------------------------------------
//����: ���ø���ʱ���жϼ����������.
//����: timer,��ʱ��Ӳ�����
//      value,��ʱ��Ԥ��Ƶ��,0~7��Ӧ1~128��Ƶ
//����: ��
//-----------------------------------------------------------------------------
void Timer_SetInterval(ufast_t timer,u32 cycle)
{
    if(timer >2)    //c100��3����ʱ��
        return;
    pg_tTimerReg->Cell[timer].TimerInterval = cycle;
}
//----ȡ��ʱ���жϼ��-------------------------------------------------------
//����: ȡ��ʱ���жϼ����������.
//����: timer,��ʱ��Ӳ�����
//����: �жϼ����������
//-----------------------------------------------------------------------------
u32 Timer_GetInterval(ufast_t timer)
{
    if(timer >2)    //c100��3����ʱ��
        return 0;
    return pg_tTimerReg->Cell[timer].TimerInterval;
}
//----���ö�ʱ����ǰֵ-------------------------------------------------------
//����: ���ø���ʱ����ǰֵ��������.
//����: timer,��ʱ��Ӳ�����
//      cycle,��ʱ��Ԥ��Ƶ��,0~7��Ӧ1~128��Ƶ
//����: ��
//-----------------------------------------------------------------------------
void Timer_SetCounter(ufast_t timer,u32 cycle)
{
    if(timer >2)    //c100��3����ʱ��
        return;
    pg_tTimerReg->Cell[timer].TimerValue = cycle;
}
//----ȡ��ʱ����ǰֵ-------------------------------------------------------
//����:ȡ��ʱ����ǰֵ��������.
//����: timer,��ʱ��Ӳ�����
//����: ��ʱ����ǰֵ
//-----------------------------------------------------------------------------
u32 Timer_GetCounter(ufast_t timer)
{
    if(timer >2)    //c100��3����ʱ��
        return 0;
    return pg_tTimerReg->Cell[timer].TimerValue;
}

//----���ö�ʱ��Ϊ�����ж�ģʽ-------------------------------------------------------
//����: ���ö�ʱ��Ϊ�����ж�ģʽ
//����: timer,��ʱ��Ӳ�����
//����: ��
//-----------------------------------------------------------------------------
void Timer_SetAutoReload(ufast_t timer)
{
    if(timer >2)    //c100��3����ʱ��
        return;
    pg_tTimerReg->Cell[timer].TimerCtrl &= ~(1<<7);
}
//----������ʱ��----------------------------------------------------------------
//����: ������ʱ��
//����: timer,��ʱ��Ӳ�����
//����: ��
//-----------------------------------------------------------------------------
void Timer_Start(ufast_t timer)
{
    if(timer >2)    //c100��3����ʱ��
        return;
    pg_tTimerReg->Cell[timer].TimerCtrl |= 1<<0;
}
//----ֹͣ��ʱ��----------------------------------------------------------------
//����: ֹͣ��ʱ��
//����: timer,��ʱ��Ӳ�����
//����: ��
//-----------------------------------------------------------------------------
void Timer_Stop(ufast_t timer)
{
    if(timer >2)    //c100��3����ʱ��
        return;
    pg_tTimerReg->Cell[timer].TimerCtrl &= ~(1<<0);
}
//----��ȡ��ʱ���ж�״̬--------------------------------------------------------
//����: ��ȡ��ʱ���ж�״̬
//����: timer,��ʱ��Ӳ�����
//����: true = �ж��ѷ�����false = �ж�δ����
//-----------------------------------------------------------------------------
bool_t Timer_GetIntStatus(ufast_t timer)
{
    if(timer >2)    //c100��3����ʱ��
        return;
    if(pg_tTimerReg->IrqSta & (1<<timer))
        return true;
    else
        return false;
}
//----�����ʱ��Ԥ�ж�-------------------------------------------------------
//����: �����ʱ���ж�
//����: timer,��ʱ��Ӳ�����
//����: ��
//-----------------------------------------------------------------------------
void Timer_ClearInt(ufast_t timer)
{
    if(timer >2)    //c100��3����ʱ��
        return;
    pg_tTimerReg->IrqSta |= 1<<timer; // clear int
}

u8 __F1CTimer_GetFreeTimer(void)
{
    u8 i;
    for(i = 0; i < CN_TIMER_NUM; i++)
    {
        if(stgTimerHandle[i].timerstate & CN_TIMER_FREE)
            break;
    }
    return i;
}

// =============================================================================
// ��������: ���ṩ��ʱ�任��Ϊ��ʱ���ļ���������
// �������:time��Ҫ�����ʱ��(��λΪ΢��)
// �������:counter,��Ӧʱ���counter
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :time̫���ʧ�ܣ������counter���Ƕ�ʱ�������ֵ
// =============================================================================
bool_t __F1CTimer_Time2Counter(u64 time, u64 *counter)
{

    u64 counter_1=time*CN_TIMER_FREQ/1000000;
    *counter = counter_1 > CN_MAX_COUNTER ? CN_MAX_COUNTER : counter_1;

    if(*counter == counter_1)
        return true;

    return false;

}
// =============================================================================
// ��������: ����ʱ���ļ�������������Ϊʱ��
// �������:counter,��ʱ����counter��Ԫ
// �������:time�������ʱ��(��λΪ΢��)
// ����ֵ  :true�ɹ� fasleʧ��
// =============================================================================
bool_t __F1CTimer_Counter2Time(u64 counter,u64 *time)
{
    u32 time_1=counter/(CN_TIMER_FREQ/1000000);
    if(time_1 > CN_MAX_TIME_US)
    {
        *time = CN_MAX_TIME_US;
        return false;
    }
    else
    {
        *time = time_1;
        return true;
    }
}
// =============================================================================
// ��������: ��ֹͣ��ʱ
// �������: timer����ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __F1CTimer_PauseCount(struct TimerHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno >= CN_TIMER_NUM)
        {
            return false;
        }
        else
        {
            pg_tTimerReg->Cell[timerno].TimerCtrl &= ~((1)<<0);
            pg_tTimerReg->IrqSta = 1<<timerno;
            timer->timerstate = (timer->timerstate)&(~CN_TIMER_ENCOUNT);
            return true;
        }
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������: ��ʼ��ʱ
// �������:timer����ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __F1CTimer_StartCount(struct TimerHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno >= CN_TIMER_NUM)
        {
            return false;
        }
        else
        {
//            pg_tTimerReg->IrqSta = 1<<timerno;
            pg_tTimerReg->Cell[timerno].TimerCtrl |= 1<<1;
            pg_tTimerReg->Cell[timerno].TimerCtrl |= 1<<0;
            timer->timerstate = (timer->timerstate)| (CN_TIMER_ENCOUNT);

            return true;
        }
    }
    else
    {
        return false;
    }

}

// =============================================================================
// ��������:�趨�жϼ���ļ�ʱ������
// �������:timer����ʱ��
//          cycle,���ڣ���ʱ��ʱ�Ӹ�������
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :�����������̫�󣨳������ʱ����������������Ϊ��ʱ�����������
// =============================================================================
bool_t  __F1CTimer_SetCycle(struct TimerHandle  *timer, u32 cycle)
{
    u8 timerno;
    u64 counter,time_set;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno >= CN_TIMER_NUM)
        {
            return false;
        }
        else
        {
            pg_tTimerReg->Cell[timerno].TimerInterval = cycle;
            timer->cycle = cycle;
            return true;
        }
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������: ���ö�ʱ��Ϊ�Զ�����
// �������:timer����ʱ��
//         autoreload, true���Լ�reload���ڣ������ֶ�ÿ����������
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// =============================================================================
bool_t  __F1CTimer_SetAutoReload(struct TimerHandle  *timer, bool_t autoreload)
{
    bool_t result;
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno < CN_TIMER_NUM)
        {
            if(autoreload == true)
            {
                pg_tTimerReg->Cell[timerno].TimerCtrl &= ~(1<<7);
            }
            else
            {
                pg_tTimerReg->Cell[timerno].TimerCtrl |= 1<<7;
            }
        }
        else
        {
            result = false;
        }
    }
    else
    {
        result = false;
    }

    return result;
}

//-----------------------------------------------------------------------------
// ���ܣ���ʱ���ж���Ӧ������������Ҫ��Ӳ��������Ȼ���ӵ���user ISR��
// ��������ʱ�������
// ���أ�user ISR�ķ���ֵ
// ˵�����붨��Ϊ weak �������������û��Լ����� ISR��f1c100��Ҫ���в�����ʱ���ļĴ�������
//       ���жϣ��Զ���ISR��ע�⡣
//-----------------------------------------------------------------------------
__attribute__((weak)) u32 __F1CTimer_isr(ptu32_t TimerHandle)
{
    u32 timerno;
    timerno = ((struct TimerHandle  *)TimerHandle)->timerno;
    Timer_ClearInt(timerno);
    return ((struct TimerHandle  *)TimerHandle)->UserIsr(TimerHandle);
}

// =============================================================================
// ��������: ���䶨ʱ��
// �������:timerisr,��ʱ�����жϴ�����
// �������:
// ����ֵ  :����Ķ�ʱ�������NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
ptu32_t __F1CTimer_Alloc(fnTimerIsr timerisr)
{
    u8 timerno;
    u8 irqline;
    struct TimerHandle  *timer;
    ptu32_t timerhandle;
    //ԭ�Ӳ�������ֹ��Դ����
    atom_low_t  timeratom;
    timeratom = Int_LowAtomStart();

    //Ѱ�ҿ��е�timer
    timerno = __F1CTimer_GetFreeTimer();
    if(timerno < CN_TIMER_NUM)     //���п��еģ������ñ�־λ
    {
        Int_LowAtomEnd(timeratom);  //ԭ�Ӳ������
    }
    else//û�еĻ�ֱ�ӷ��ؾͿ����ˣ��ò����ن�����
    {
        Int_LowAtomEnd(timeratom);   //ԭ�Ӳ������
        return 0;
    }
    irqline = sgHaltimerIrq[timerno];
    timer = &stgTimerHandle[timerno];
    timer->cycle = 0;
    timer->timerno = timerno;
    timer->irqline = irqline;
    timer->timerstate = CN_TIMER_ENUSE;
    timer->UserIsr=timerisr;
    //���ˣ��жϺźͶ�ʱ�����붼���ˣ��ø���͸����ˡ�
    //�����úö�ʱ������
    __F1CTimer_PauseCount(timer);
    //���ö�ʱ���ж�,�Ƚ��������ж����еĹ����������
    Int_Register(irqline);
    Int_DisableLine(irqline);

    Int_IsrDisConnect(irqline);
    Int_EvttDisConnect(irqline);
    Int_SettoAsynSignal(irqline);

    Int_SetClearType(irqline,CN_INT_CLEAR_USER);
    Int_SetIsrPara(irqline,(ptu32_t)timer);

    Int_IsrConnect(irqline, __F1CTimer_isr);
    timerhandle = (ptu32_t)timer;

    return timerhandle;
}


// =============================================================================
// ��������: �ͷŶ�ʱ��
// �������:timer������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __F1CTimer_Free(ptu32_t timerhandle)
{
    u8 timerno;
    u8 irqline;
    atom_low_t  timeratom;  //����������Դ
    struct TimerHandle  *timer;
    timer = (struct TimerHandle  *)timerhandle;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        irqline = timer->irqline;
        if(timerno < CN_TIMER_NUM)
        {       //�޸�ȫ�ֱ�־һ����ԭ���Ե�
            timeratom = Int_LowAtomStart();
            //������ж�������������
            timer->timerstate = CN_TIMER_FREE;
            Int_DisableLine(irqline);
            Int_IsrDisConnect(irqline);
            Int_EvttDisConnect(irqline);
            Int_UnRegister(irqline);
            Int_LowAtomEnd(timeratom);  //ԭ�Ӳ������

            return true;
        }
        else//û�еĻ�ֱ�ӷ��ؾͿ����ˣ��ò����ن�����
        {
            return false;
        }

    }
    else
    {
        return false;
    }
}


// =============================================================================
// ��������: ���ö�ʱ�����ж�����
// �������:timer���������Ķ�ʱ��
//          real_priorΪtrue��Ϊʵʱ�źŷ���Ϊ�첽�ź�
// �������:
// ����ֵ  :����Ķ�ʱ����NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
bool_t  __F1CTimer_SetIntPro(struct TimerHandle  *timer, bool_t real_prior)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        if(real_prior)
        {
            timer->timerstate = (timer->timerstate)| (CN_TIMER_REALINT);
            Int_EvttDisConnect(timer->irqline);
            return Int_SettoReal(timer->irqline);
        }
        else
        {
            timer->timerstate = (timer->timerstate)&(~CN_TIMER_REALINT);
            return Int_SettoAsynSignal(timer->irqline);
        }
    }
    else
    {
        return false;
    }
}

// =============================================================================
// ��������: ʹ�ܶ�ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __F1CTimer_EnInt(struct TimerHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)| (CN_TIMER_ENINT);
        pg_tTimerReg->IrqEn |= 1 << timer->timerno;
        return Int_EnableLine(timer->irqline);
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������: ��ֹ��ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __F1CTimer_DisInt(struct TimerHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)&(~CN_TIMER_ENINT);
        pg_tTimerReg->IrqEn &= ~(1 << timer->timerno);
        return Int_DisableLine(timer->irqline);
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������: ��ȡ��ʱ����ʱ
// �������:timer���������Ķ�ʱ��
// �������:time����ʱ��ʱ������
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :���趨���������𣬼�cycle-ʣ��ʱ��,��ʾ�Ѿ��ߵ���ʱ��(��λ����ʱ����Ƶʱ�Ӹ���)
// =============================================================================
bool_t __F1CTimer_GetTime(struct TimerHandle  *timer, u32 *time)
{
    u8 timerno;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno >= CN_TIMER_NUM)
        {
            return false;
        }
        counter = pg_tTimerReg->Cell[timerno].TimerInterval - pg_tTimerReg->Cell[timerno].TimerValue;
        *time = counter;
        return true;
    }
    else
    {
        return false;
    }
}

// =============================================================================
// ��������:ȡ��ʱ��Ӳ�����ƼĴ�����ַ
// �������:timer���������Ķ�ʱ��
// ����ֵ  :true�ɹ� falseʧ��
// ˵��    :
// =============================================================================
bool_t __F1CTimer_GetReg(struct TimerHandle  *timer, void **reg)
{
    *reg = pg_tTimerReg;
    return true;
}
// =============================================================================
// ��������: ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timer���������Ķ�ʱ��
// �������:timeout, true��ʱ��false����ʱ
// ����ֵ  :true�ɹ� falseʧ��
// ˵��    :
// =============================================================================
bool_t __F1CTimer_CheckTimeout(struct TimerHandle  *timer, bool_t *timeout)
{
    bool_t result=true;
    u8 timerno;
    u32 temp;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno >= CN_TIMER_NUM)
        {
            result = false;
        }
        else
        {
            temp = pg_tTimerReg->Cell[timerno].TimerValue;
            if(temp == 0)
            {
                *timeout = true;
            }
            else
            {
                *timeout = false;
            }
        }
    }
    else
    {
        result = false;
    }
    return result;
}

// =============================================================================
// �������ܣ���ȡ��ʱ��ID
// ���������timer����������ʱ����
// ���������timerId,��16λΪ irqno,��16Ϊ��Ӧ�� timerno
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __F1CTimer_GetID(struct TimerHandle   *timer,u32 *timerId)
{
    u16 irqno;
    u16 timerno;

    if(NULL == timer)
    {
        return false;
    }
    else
    {
        timerno = timer->timerno;
        irqno = timer->irqline;
        *timerId = ((u32)irqno<<16)|(timerno);
        return true;
    }
}

// =============================================================================
// �������ܣ�__F1CTimer_GetCycle
//          ��ȡ��ʱ������
// ���������timer����������ʱ����
// ���������cycle����ʱ������(��λ����ʱ����Ƶʱ�Ӹ���)
// ����ֵ  ��true �ɹ� falseʧ��
// =============================================================================
bool_t __F1CTimer_GetCycle(struct TimerHandle   *timer, u32 *cycle)
{
    if(NULL == timer)
    {
        return false;
    }
    else
    {
        *cycle = timer->cycle;
        return true;
    }
}
// =============================================================================
// �������ܣ�__F1CTimer_GetState
//          ��ȡ��ʱ��״̬
// ���������timer,�������Ķ�ʱ��
// ���������timerflag����ʱ��״̬
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __F1CTimer_GetState(struct TimerHandle   *timer, u32 *timerflag)
{

    if(NULL == timer)
    {
        return false;
    }
    else
    {
        *timerflag = timer->timerstate;
        return true;
    }
}
// =============================================================================
// ��������:__F1CTimer_Ctrl
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timerhandle �������Ķ�ʱ�����
//         ctrlcmd, ����������
// �������:inoutpara,����ctrlcmd�Ĳ�ͬ����ͬ
// ����ֵ  :true�ɹ�  falseʧ��
// ˵��    :
// =============================================================================
bool_t __F1CTimer_Ctrl(ptu32_t timerhandle, \
                         enum HardTimerCmdCode ctrlcmd, \
                         ptu32_t inoutpara)
{
    bool_t result = false;
    struct TimerHandle  *timer;
    timer = (struct TimerHandle  *)timerhandle;
    if(NULL != timer)
    {
        switch(ctrlcmd)
        {
            case EN_TIMER_STARTCOUNT:
                result = __F1CTimer_StartCount(timer);
                break;
            case EN_TIMER_PAUSECOUNT:
                result = __F1CTimer_PauseCount(timer);
                break;
            case EN_TIMER_SETCYCLE:
                result = __F1CTimer_SetCycle(timer,(u32)inoutpara);
                break;
            case EN_TIMER_SETRELOAD:
                result = __F1CTimer_SetAutoReload(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_ENINT:
                result = __F1CTimer_EnInt(timer);
                break;
            case EN_TIMER_DISINT:
                result = __F1CTimer_DisInt(timer);
                break;
            case EN_TIMER_SETINTPRO:
                result = __F1CTimer_SetIntPro(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_GETTIME:
                result = __F1CTimer_GetTime(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETCYCLE:
                result = __F1CTimer_GetCycle(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETID:
                result = __F1CTimer_GetID(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETSTATE:
                result = __F1CTimer_GetState(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETREG:
                result = __F1CTimer_GetReg(timer, (void **)inoutpara);
                break;
            default:
                break;
        };
    }

    return result;
}
// =============================================================================
// ��������: ��ȡ��ʱ����Ƶ
// �������:timerhandle �������Ķ�ʱ�����
// �������:
// ����ֵ  :��ʱ����Ƶ
// ˵��    :��λ��HZ��
// =============================================================================
u32  __F1CTimer_GetFreq(ptu32_t timerhandle)
{
    return CN_TIMER_FREQ;
}
// =============================================================================
// ��������:
// �������:
// �������:
// ����ֵ  :
// ˵��    :
// =============================================================================
bool_t ModuleInstall_HardTimer(void)
{
    struct TimerChip  allwinner_timer;


    //��ʱ��TIM0~2 ѡ��ʱ��ԴΪ24M
    u8 i;

    for(i=0;i<CN_TIMER_NUM;i++)
    {
        stgTimerHandle[i].timerstate = CN_TIMER_FREE;
        Timer_SetClkSource(i,1);            //ʱ��Դ��Ϊ24M
        Timer_SetPrecale(i,0);              //Ԥ��Ƶ��Ϊ1��������Ƶ
    }

    allwinner_timer.chipname = "f1cx00-timer";
    allwinner_timer.HardTimerAlloc = __F1CTimer_Alloc;
    allwinner_timer.HardTimerFree = __F1CTimer_Free;
    allwinner_timer.HardTimerCtrl = __F1CTimer_Ctrl;
    allwinner_timer.HardTimerGetFreq = __F1CTimer_GetFreq;
    HardTimer_RegisterChip(&allwinner_timer);

    return true;
}

