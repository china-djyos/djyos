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
// ��ע�������ṩ��timer_core.cʹ�ã����Բ������������֮���
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "int.h"
#include "cpu.h"
#include "cpu_peri_int_line.h"
#include "timer_hard.h"
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
//parent:"Software Timers"//��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"Software Timers","int"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
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

//define timer address

#define CN_TIMERA0_BASE (0x40500000)
#define CN_TIMERA1_BASE (0x40500000 + 0x40)
#define CN_TIMERB0_BASE (0x40510000)
#define CN_TIMERB1_BASE (0x40510000 + 0x40)
#define CN_TIMERC0_BASE (0x41520000)
#define CN_TIMERC1_BASE (0x41520000 + 0x40)

//#define CN_TIMERD0_BASE (0x41530000)
//#define CN_TIMERD1_BASE (0x41530000 + 0x40)

//�Ĵ�������,16Bitȫ������32Bit��ʱ��

typedef struct
{
    volatile u32 FSR;
    volatile u32 CR;
    volatile u32 TC;
    volatile u32 PR;
    volatile u32 PC;
    volatile u32 MR;
    volatile u32 MCR;
    volatile u32 EMR;
    volatile u32 EWCR;
    volatile u32 CAPTR;
    volatile u32 CCR;
    volatile u32 INTIF;
    volatile u32 INTIE;
}tagTimer16Reg;

typedef struct
{
    volatile tagTimer16Reg *mp_TimerReg_L;//��16bit��ʱ���Ĵ���
    volatile tagTimer16Reg *mp_TimerReg_H;//��16bit��ʱ���Ĵ���
}tagTimer32Reg;

//32bit ��ʱ����λ�Ĵ�������

volatile tagTimer32Reg sgpt_TimerReg[] = {
    {
        (volatile tagTimer32Reg*)CN_TIMERA0_BASE,
        (volatile tagTimer32Reg*)CN_TIMERA1_BASE,
    },
    {
        (volatile tagTimer32Reg*)CN_TIMERB0_BASE,
        (volatile tagTimer32Reg*)CN_TIMERB1_BASE,
    },
    {
        (volatile tagTimer32Reg*)CN_TIMERC0_BASE,
        (volatile tagTimer32Reg*)CN_TIMERC1_BASE,
    },

};

#define CN_PIT_MAX_COUNTER     0xFFFFFFFF
#define CN_PIT_MAX_TIME_US     (0xFFFFFFFF/375/10)  //0xFFFFFFFF/375Mhz=1us


enum ENUM_SCM6XX_TIMER
{
    EN_SCM6XXTIMER_A=0,//��˳��ֵ
    EN_SCM6XXTIMER_B,   //EN_STM32TIMER_5Ϊ1��EN_STM32TIMER_5��������һ����Ϊ2
    EN_SCM6XXTIMER_C,
//  EN_STM32TIMER_D,
};

//������ʱ��оƬ�Ķ�ʱ��Ӧ�����Լ��ľ��
struct SCM6xxTimerHandle
{
    u32     timerno;          //��ʱ����
    u32     irqline;          //�жϺ�
    u32     cycle;            //��ʱ����
    u32     timerstate;       //��ʱ����ʶ
    bool_t  autoReload;       //�Զ���װ�ر�� true ʹ��  false ʹ��
    fnTimerIsr UserIsr;      //�û��ж���Ӧ����
};
#define CN_SCM6XXTIMER_NUM   (EN_SCM6XXTIMER_C +1)
#define CN_SCM6XXTIMER_MAX    EN_SCM6XXTIMER_C

static struct SCM6xxTimerHandle  stgTimerHandle[CN_SCM6XXTIMER_NUM];

//���λ����timer0����������
static u32  gs_dwSCM6xxTimerBitmap;  //���ڶ�ʱ�����ֶ�����һ��Ĳ���ܶ࣬32��Ӧ���㹻
#define CN_SCM6XXTIMER_BITMAP_MSK  (0x80000000)  //���λΪ1���������Ƽ���

//timer0��timern��irq�ж�
static u32 sgHaltimerIrq[CN_SCM6XXTIMER_NUM]={
                                            CN_INT_LINE_TIMERA1,
                                            CN_INT_LINE_TIMERB1,
                                            CN_INT_LINE_TIMERC1,
                                            };

//��ȡ32λ����һ��0bitλ��(�Ӹ�λ����λ����)
u8 __SCM6xxTimer_GetFirstZeroBit(u32 para)
{
    u8 i;
    for(i = 0; i < 32; i++)
    {
        if(!(para & (1<<(31-i))))
            break;
    }
    return i;
}

// =============================================================================
// ��������:__SCM6xxTimer_Time2Counter
//          ���ṩ��ʱ�任��Ϊ��ʱ���ļ�����λ,
// �������:time��Ҫ�����ʱ��(��λΪ΢��)
// �������:counter,��Ӧʱ���counter
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :time̫���ʧ�ܣ������counter���Ƕ�ʱ�������ֵ
// =============================================================================
bool_t __SCM6xxTimer_Time2Counter(u64 time, u64 *counter)
{

    u64 counter_1=time*375/10;
    *counter = counter_1 > CN_PIT_MAX_COUNTER ? CN_PIT_MAX_COUNTER : counter_1;

    if(*counter == counter_1)
        return true;

    return false;

}
// =============================================================================
// ��������:__SCM6xxTimer_Counter2Time
//          ����ʱ���ļ�����λ����Ϊʱ��
// �������:counter,��ʱ����counter��Ԫ
// �������:time�������ʱ��(��λΪ΢��)
// ����ֵ  :true�ɹ� fasleʧ��
// =============================================================================
bool_t __SCM6xxTimer_Counter2Time(u64 counter,u64 *time)
{
    //�������ö�Pclʱ�ӽ��м��� - ����Ƶ- 37500000
    u32 time_1=counter/375* 10;
    if(time_1 > CN_PIT_MAX_TIME_US)
    {
        *time = CN_PIT_MAX_TIME_US;
        return false;
    }
    else
    {
        *time = time_1;
        return true;
    }
}
// =============================================================================
// ��������:__SCM6xxTimer_PauseCount
//          ֹͣ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================

bool_t __SCM6xxTimer_PauseCount(struct SCM6xxTimerHandle  *timer)
{
    u8 timerno;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_SCM6XXTIMER_MAX)
        {
            return false;
        }
        else
        {
            sgpt_TimerReg[timerno].mp_TimerReg_L->CR = 0;
            sgpt_TimerReg[timerno].mp_TimerReg_H->CR = 0;

            //���жϱ�־
            sgpt_TimerReg[timerno].mp_TimerReg_L->INTIF = 0;
            sgpt_TimerReg[timerno].mp_TimerReg_H->INTIF = 0;

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
// ��������:__SCM6xxTimer_StartCount
//          ��ʼ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __SCM6xxTimer_StartCount(struct SCM6xxTimerHandle  *timer)
{
    u8 timerno;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_SCM6XXTIMER_MAX)
        {
            return false;
        }
        else
        {
            //���жϱ�־
            sgpt_TimerReg[timerno].mp_TimerReg_L->INTIF |= 0xf;
            sgpt_TimerReg[timerno].mp_TimerReg_H->INTIF |= 0xf;

            //��λ�Լ�ֹͣ��λ TC0
            sgpt_TimerReg[timerno].mp_TimerReg_L->CR |= (1<<1);
            sgpt_TimerReg[timerno].mp_TimerReg_H->CR |= (1<<1);

            sgpt_TimerReg[timerno].mp_TimerReg_L->CR &= ~(1<<1);
            sgpt_TimerReg[timerno].mp_TimerReg_H->CR &= ~(1<<1);

            //��������
            sgpt_TimerReg[timerno].mp_TimerReg_L->CR = 0x1;
            sgpt_TimerReg[timerno].mp_TimerReg_H->CR = 0x1;

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
// ��������:__SCM6xxTimer_SetCycle,STM32��timer,���Ϊ0xFFFFFFFF,����Timer_ModuleInit
//          �趨������ÿ��һ����Ϊ(1/84)1us����֪���ʱ�䲻��-1
//          �趨����
// �������:timer��PIC��ʱ��
//          cycle,���ڣ���ʱ��ʱ�Ӹ�������
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :�����������̫�󣨳������ʱ����������������Ϊ��ʱ�����������
// =============================================================================
bool_t  __SCM6xxTimer_SetCycle(struct SCM6xxTimerHandle  *timer, u32 cycle)
{
    u8 timerno;
    u64 counter,time_set;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_SCM6XXTIMER_MAX)
        {
            return false;
        }
        else
        {
            //����ƥ��ֵ
            sgpt_TimerReg[timerno].mp_TimerReg_L->MR = (u16)cycle;
            sgpt_TimerReg[timerno].mp_TimerReg_H->MR = (u16)(cycle>>16);
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
// ��������:__SCM6xxTimer_SetAutoReload
//          �趨����
// �������:timer��PIC��ʱ��
//         autoreload, true���Լ�reload���ڣ������ֶ�ÿ����������
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// =============================================================================

bool_t  __SCM6xxTimer_SetAutoReload(struct SCM6xxTimerHandle  *timer, bool_t autoreload)
{
    bool_t result;
    u8 timerno;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;

        if(timerno < CN_SCM6XXTIMER_MAX)
        {
            if(autoreload == true)
            {
                stgTimerHandle[timerno].autoReload = true;
                //Ĭ�Ͼ����Զ���װ��
//                sgpt_TimerReg[timerno].mp_TimerReg_L->MCR &= ~(1<<2);
//                sgpt_TimerReg[timerno].mp_TimerReg_H->MCR &= ~(1<<2);
            }
            else
            {
                stgTimerHandle[timerno].autoReload = false;
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
// ���ܣ���ʱ���ж���Ӧ������������atmel m7��������Ҫ���в�����ʱ���ļĴ�������
//       ���жϵĶ�ʱ��������������ʵ��ISR��Ȼ���ӵ���user ISR��������
//       user isr�н������ʴӡ�
// ��������ʱ�������
// ���أ�user ISR�ķ���ֵ
//-----------------------------------------------------------------------------
__attribute__((weak))  u32 __SCM6xxTimer_isr(ptu32_t TimerHandle)
{
    u32 timerno;

    timerno = ((struct SCM6xxTimerHandle  *)TimerHandle)->timerno;

    if(stgTimerHandle[timerno].autoReload == false)
    {
         sgpt_TimerReg[timerno].mp_TimerReg_L->CR = 0;
         sgpt_TimerReg[timerno].mp_TimerReg_H->CR = 0;
    }
    //��ƥ���ж�
    sgpt_TimerReg[timerno].mp_TimerReg_L->INTIF |= (1<<0);
    sgpt_TimerReg[timerno].mp_TimerReg_H->INTIF |= (1<<0);

    Int_ClearLine(((struct SCM6xxTimerHandle  *)TimerHandle)->irqline);
    return ((struct SCM6xxTimerHandle  *)TimerHandle)->UserIsr(TimerHandle);
}

// =============================================================================
// ��������:__SCM6xxTimer_Alloc
//          ���䶨ʱ��
// �������:timerisr,��ʱ�����жϴ�����
// �������:
// ����ֵ  :����Ķ�ʱ�������NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
ptu32_t __SCM6xxTimer_Alloc(fnTimerIsr timerisr)
{
    u8 timerno;
    u8 irqline;
    struct SCM6xxTimerHandle  *timer;
    ptu32_t timerhandle;
    //ԭ�Ӳ�������ֹ��Դ����
    atom_low_t  timeratom;
    timeratom = Int_LowAtomStart();

    //Ѱ�ҿ��е�timer
    timerno = __SCM6xxTimer_GetFirstZeroBit(gs_dwSCM6xxTimerBitmap);
    if(timerno < CN_SCM6XXTIMER_NUM)//���п��еģ������ñ�־λ
    {
        gs_dwSCM6xxTimerBitmap = gs_dwSCM6xxTimerBitmap | (CN_SCM6XXTIMER_BITMAP_MSK<< timerno);
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
    __SCM6xxTimer_PauseCount(timer);
    //__SCM6xxTimer_SetCycle(timer,cycle);
    //���ö�ʱ���ж�,�Ƚ��������ж����еĹ����������
    Int_Register(irqline);
    Int_CutLine(irqline);

    Int_IsrDisConnect(irqline);
    Int_EvttDisConnect(irqline);
    Int_SettoAsynSignal(irqline);

    Int_SetClearType(irqline,CN_INT_CLEAR_USER);
    Int_SetIsrPara(irqline,(ptu32_t)timer);

    Int_IsrConnect(irqline, __SCM6xxTimer_isr);
    timerhandle = (ptu32_t)timer;

    return timerhandle;
}


// =============================================================================
// ��������:__SCM6xxTimer_Free
//          �ͷŶ�ʱ��
// �������:timer������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __SCM6xxTimer_Free(ptu32_t timerhandle)
{
    u8 timerno;
    u8 irqline;
    atom_low_t  timeratom;  //����������Դ
    struct SCM6xxTimerHandle  *timer;
    timer = (struct SCM6xxTimerHandle  *)timerhandle;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        irqline = timer->irqline;
        if(timerno < CN_SCM6XXTIMER_NUM)//���п��еģ������ñ�־λ
        {       //�޸�ȫ�ֱ�־һ����ԭ���Ե�
            timeratom = Int_LowAtomStart();
            gs_dwSCM6xxTimerBitmap = gs_dwSCM6xxTimerBitmap &(~(CN_SCM6XXTIMER_BITMAP_MSK<< timerno));
            //������ж�������������
            timer->timerstate = 0;
            Int_CutLine(irqline);
            Int_IsrDisConnect(irqline);
            Int_EvttDisConnect(irqline);
            Int_UnRegister(irqline);
            Int_LowAtomEnd(timeratom);  //ԭ�Ӳ�����ϣ���������

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
// ��������:__SCM6xxTimer_SetIntPro
//          ���ö�ʱ�����ж�����
// �������:timer���������Ķ�ʱ��
//          real_priorΪtrue��Ϊʵʱ�źŷ���Ϊ�첽�ź�
// �������:
// ����ֵ  :����Ķ�ʱ����NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
bool_t  __SCM6xxTimer_SetIntPro(struct SCM6xxTimerHandle  *timer, bool_t real_prior)
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
// ��������:__SCM6xxTimer_EnInt
//          ʹ�ܶ�ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __SCM6xxTimer_EnInt(struct SCM6xxTimerHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)| (CN_TIMER_ENINT);
        return Int_ContactLine(timer->irqline);
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:__SCM6xxTimer_DisInt
//          ��ֹ��ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __SCM6xxTimer_DisInt(struct SCM6xxTimerHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)&(~CN_TIMER_ENINT);
        return Int_CutLine(timer->irqline);
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:__SCM6xxTimer_GetTime
//          ��ȡ��ʱ����ʱ
// �������:timer���������Ķ�ʱ��
// �������:time����ʱ��΢�룩
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :���趨���������𣬼�cycle-ʣ��ʱ��,��ʾ�Ѿ��ߵ���ʱ��(��λ����ʱ����Ƶʱ�Ӹ���)
// =============================================================================
bool_t __SCM6xxTimer_GetTime(struct SCM6xxTimerHandle  *timer, u32 *time)
{
    u8 timerno;
    u32 counter;
    u16 cntValueH,cntValueL;
    u32 cntValue;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_SCM6XXTIMER_MAX)
        {
            return false;
        }
        cntValueL = sgpt_TimerReg[timerno].mp_TimerReg_L->TC;
        cntValueH = sgpt_TimerReg[timerno].mp_TimerReg_H->TC;
        cntValue  =  (cntValueH<<16)|cntValueL;
        counter   = cntValue;                  //CNT����΢����
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
bool_t __SCM6xxTimer_GetReg(struct SCM6xxTimerHandle  *timer, void **reg)
{
    u8 timerno;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_SCM6XXTIMER_MAX)
        {
            return false;
        }
        *reg = sgpt_TimerReg[timerno].mp_TimerReg_L;
        return true;
    }
    else
    {
        return false;
    }
}

// ========================= ====================================================
// ��������:__SCM6xxTimer_CheckTimeout
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timer���������Ķ�ʱ��
// �������:timeout, true��ʱ��false����ʱ
// ����ֵ  :true�ɹ� falseʧ��
// ˵��    :
// =============================================================================
bool_t __SCM6xxTimer_CheckTimeout(struct SCM6xxTimerHandle  *timer, bool_t *timeout)
{
    bool_t result=true;
    u8 timerno;
    u32 temp;
    u32 counter;
    u16 cntValueH,cntValueL;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_SCM6XXTIMER_MAX)
        {
            result = false;
        }
        else
        {

            cntValueL = sgpt_TimerReg[timerno].mp_TimerReg_L->TC;
            cntValueH = sgpt_TimerReg[timerno].mp_TimerReg_H->TC;
            counter   = (cntValueH << 16) | cntValueL;

            cntValueL = sgpt_TimerReg[timerno].mp_TimerReg_L->MR;
            cntValueH = sgpt_TimerReg[timerno].mp_TimerReg_H->MR;

            temp =  (cntValueH << 16) | cntValueL;

            if(temp == counter)
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
// �������ܣ�__SCM6xxTimer_GetID
//          ��ȡ��ʱ��ID
// ���������timer����������ʱ����
// ���������timerId,��16λΪTIMERNO,��16Ϊ��Ӧ��IRQNO
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __SCM6xxTimer_GetID(struct SCM6xxTimerHandle   *timer,u32 *timerId)
{
    u16 irqno;
    u16 timerno;

    if(NULL == timer)
    {
        return false;
    }
    else
    {
        irqno = (u16)timer->timerno;
        timerno = (u16)timer->irqline;
        *timerId = (timerno<<16)|(irqno);
        return true;
    }
}
// =============================================================================
// �������ܣ�__SCM6xxTimer_GetCycle
//          ��ȡ��ʱ������
// ���������timer����������ʱ����
// ���������cycle����ʱ������(��λ����ʱ����Ƶʱ�Ӹ���)
// ����ֵ  ��true �ɹ� falseʧ��
// =============================================================================
bool_t __SCM6xxTimer_GetCycle(struct SCM6xxTimerHandle   *timer, u32 *cycle)
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
// �������ܣ�__SCM6xxTimer_GetState
//          ��ȡ��ʱ��״̬
// ���������timer,�������Ķ�ʱ��
// ���������timerflag����ʱ��״̬
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __SCM6xxTimer_GetState(struct SCM6xxTimerHandle   *timer, u32 *timerflag)
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
// ��������:__SCM6xxTimer_Ctrl
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timerhandle �������Ķ�ʱ�����
//         ctrlcmd, ����������
// �������:inoutpara,����ctrlcmd�Ĳ�ͬ����ͬ
// ����ֵ  : true�ɹ�  falseʧ��
// ˵��    :
// =============================================================================
bool_t __SCM6xxTimer_Ctrl(ptu32_t timerhandle, \
                         enum HardTimerCmdCode ctrlcmd, \
                         ptu32_t inoutpara)
{
    bool_t result = false;
    struct SCM6xxTimerHandle  *timer;
    timer = (struct SCM6xxTimerHandle  *)timerhandle;
    if(NULL != timer)
    {
        switch(ctrlcmd)
        {
            case EN_TIMER_STARTCOUNT:
                result = __SCM6xxTimer_StartCount(timer);
                break;
            case EN_TIMER_PAUSECOUNT:
                result = __SCM6xxTimer_PauseCount(timer);
                break;
            case EN_TIMER_SETCYCLE:
                result = __SCM6xxTimer_SetCycle(timer,(u32)inoutpara);
                break;
            case EN_TIMER_SETRELOAD:
                result = __SCM6xxTimer_SetAutoReload(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_ENINT:
                result = __SCM6xxTimer_EnInt(timer);
                break;
            case EN_TIMER_DISINT:
                result = __SCM6xxTimer_DisInt(timer);
                break;
            case EN_TIMER_SETINTPRO:
                result = __SCM6xxTimer_SetIntPro(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_GETTIME:
                result = __SCM6xxTimer_GetTime(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETCYCLE:
                result = __SCM6xxTimer_GetCycle(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETID:
                result = __SCM6xxTimer_GetID(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETSTATE:
                result = __SCM6xxTimer_GetState(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETREG:
                result = __SCM6xxTimer_GetReg(timer, (void **)inoutpara);
                break;
            default:
                break;
        };
    }

    return result;
}
// =============================================================================
// ��������:__P1020PicTimer_GetFreq
//       ��ȡ��ʱ����Ƶ
// �������:timerhandle �������Ķ�ʱ�����
// �������:
// ����ֵ  :��ʱ����Ƶ
// ˵��    :��λ��HZ��
// =============================================================================
u32  __SCM6xxTimer_GetFreq(ptu32_t timerhandle)
{
    //��ʱ��TIM2��5 ʱ��ԴΪ��������ʱ��PCLK1(APB1ʱ��)���ٶ�Ϊ42M��
    //����APB1��Ƶϵ��Ϊ1������ͨ�ö�ʱ��ʱ��ΪAPB1��2��
    //board-config.h��������STM32F4��Ƶ168M,FCLKʱ��168M,AHB����ʱ��168M,��������ʱ��CLK2=84M���ֲ�涨������84M��
    //��������ʱ��CLK1=42M���ֲ�涨������42M�������ֲ�˵����μ�
    return 37500000;  //37.5MHz���μ��ֲ�RCC_DCKCFGR��TIMx_PSC�Ĵ���
}
// =============================================================================
// ��������:module_init_timer
//       P1020��PICtimer��ʼ��,timerʱ��ԴΪAPB1 = CN_CFG_PCLK1 = 84M(timer234567)
// �������:
// �������:
// ����ֵ  :
// ˵��    :
// =============================================================================
bool_t ModuleInstall_HardTimer(void)
{
    struct TimerChip  SCM6xxtimer;
    u8 i;

    for(i=0;i<CN_SCM6XXTIMER_NUM;i++)
    {
        sgpt_TimerReg[i].mp_TimerReg_L->CR = 0;
        sgpt_TimerReg[i].mp_TimerReg_L->CR = 0;
        //����Ϊ��ʱģʽ
        sgpt_TimerReg[i].mp_TimerReg_L->FSR =0;
        sgpt_TimerReg[i].mp_TimerReg_H->FSR =(0xE << 4) | (0x1);

        //���ö�ʱ���������ֵ
        sgpt_TimerReg[i].mp_TimerReg_L->MR = 0xffff;
        sgpt_TimerReg[i].mp_TimerReg_H->MR = 0xffff;

        //����ʱ������ȫ����ƵֵΪ����Ƶ
        sgpt_TimerReg[i].mp_TimerReg_L->PR = 0;

                                                //��ֹTIMER������ΪʲôҪ��ֹTIMER???
                                                //�����ʱ���ж�������Ϊʵʱ�жϣ����û�ʵ�ֵ��жϷ�����ISR�б�����
                                                //�жϣ��Ҳ��ܵ����κ�ϵͳ��������趨Ϊ�첽�źţ���
                                                //�������жϣ����������ȫ��ϵͳ���á�
        //ʹ��ƥ��
        sgpt_TimerReg[i].mp_TimerReg_H->MCR   = 0x3;

        //ʹ��ƥ���ж�
        sgpt_TimerReg[i].mp_TimerReg_H->INTIE = 0x1;

        //out control
        sgpt_TimerReg[i].mp_TimerReg_H->EMR   = 0x3;

        //Ĭ��������Զ���װ��ʹ��
        stgTimerHandle[i].autoReload = true;


    }

    SCM6xxtimer.chipname = "SCM610TIMER";
    SCM6xxtimer.HardTimerAlloc = __SCM6xxTimer_Alloc;     //���䶨ʱ��
    SCM6xxtimer.HardTimerFree  = __SCM6xxTimer_Free;       //�ͷŶ�ʱ��
    SCM6xxtimer.HardTimerCtrl  = __SCM6xxTimer_Ctrl;       //���ƶ�ʱ��
    SCM6xxtimer.HardTimerGetFreq = __SCM6xxTimer_GetFreq; //��ȡ��ʱ������Ƶ�ʣ�84Mhz

    HardTimer_RegisterChip(&SCM6xxtimer);                //ע�ᶨʱ��оƬ��ϵͳ��ʱ��ģ��

    return true;
}

