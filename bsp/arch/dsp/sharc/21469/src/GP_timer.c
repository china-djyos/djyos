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
// ģ������: SHARC21469 GP TIMER������
// ģ��汾: V1.00
// ������Ա: zhuhb_cyg
// ����ʱ��: 3:27:12 PM/June 11, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע�������ṩ��timer_core.cʹ�ã����Բ������������֮���
#include "timer_hard.h"
#include <cdef21469.h>
#include <def21489.h>
#include "misc_config.h"
#include "board_config.h"
#include "int.h"
#include "cpu.h"
#include "cpu_peri_int_line.h"
#include "stdint.h"

#include <stdio.h>

#define CN_GP_TIMER_COUNT_MAX  0x7FFFFFFF
#define CN_GP_TIMER_NUM        2


//������ʱ��оƬ�Ķ�ʱ��Ӧ�����Լ��ľ��
struct GPTimerHandle
{
    u32     timerno;          //��ʱ����
    u32     irqline;          //�жϺ�
    u32     cycle;            //��ʱ����
    u32     timerstate;       //��ʱ����ʶ
};


static struct GPTimerHandle  s_tGPTimerHandle[2];

static u32 sgHaltimerIrq[2]={cn_int_line_GPTMR0I,cn_int_line_GPTMR1I};

static int s_s32GPTimerHandle = 0;
static int s_s32Timer1Counter = 0;


// =============================================================================
// ��������:__GPTimer_Time2Counter
//          ���ṩ��ʱ�任��Ϊ��ʱ���ļ�����λ
// �������:time��Ҫ�����ʱ��(��λΪ΢��)
// �������:counter,��Ӧʱ���counter
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :time̫���ʧ�ܣ������counter���Ƕ�ʱ�������ֵ
// =============================================================================
bool_t __GPTimer_Time2Counter(u32 time, u32 *counter)
{
    u32 time_max;
    u32 time_set;
    time_max = (u32)(2*(2^31-1)*(10^6/CN_CFG_PCLK1));    //CN_CFG_PCLK1=200MHzʱ��time_max=21.474836s=21474836us.
    time_set = time > time_max ? time_max : time;
    if(time_max > time)
    {
        time_set = time;
        *counter =  (time_set *CN_CFG_PCLK1)/(2*10^6);
        return true;
    }
    else
    {
        time_set = time_max;
        *counter =  (time_set *CN_CFG_PCLK1)/(2*10^6);
        return false;
    }
}
// =============================================================================
// ��������:__GPTimer_Counter2Time
//          ����ʱ���ļ�����λ����Ϊʱ��
// �������:counter,��ʱ����counter��Ԫ
// �������:time�������ʱ��(��λΪ΢��)
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __GPTimer_Counter2Time(u32 counter,u32 *time)
{
    u32 time_max;

    time_max = (u32)(2*CN_GP_TIMER_COUNT_MAX *10^6)/CN_CFG_PCLK1;


    if(counter > CN_GP_TIMER_COUNT_MAX)
    {
        *time = time_max;
        return false;
    }
    else
    {
        *time = (u32)(2*counter*10^6)/(CN_CFG_PCLK1);
        return true;
    }
}
// =============================================================================
// ��������:__GPTimer_PauseCount
//          ֹͣ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================

bool_t __GPTimer_PauseCount(struct GPTimerHandle  *timer)
{
    u8 timerno;
    u32 temp;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_GP_TIMER_NUM-1)
        {
            return false;
        }
        else
        {
           if(timerno==0)
            {
                *pTMSTAT |= (~TIM0EN);
            }
            else
            {
                *pTMSTAT |= (~TIM1EN);
            }

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
// ��������:__GPTimer_StartCount
//          ��ʼ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __GPTimer_StartCount(struct GPTimerHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_GP_TIMER_NUM-1)
        {
            return false;
        }
        else

        {
            if(timerno==0)
            {
                *pTMSTAT |= TIM0EN;
            }
            else
            {
                *pTMSTAT |= TIM1EN;
            }

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
// ��������:__GPTimer_SetCycle
//          �趨����
// �������:timer��GP��ʱ��
//          cycle,���ڣ�΢�룩��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :�����������̫�󣨳������ʱ����������������Ϊ��ʱ�����������
// =============================================================================
bool_t  __GPTimer_SetCycle(struct GPTimerHandle  *timer, u32 cycle)
{
    u8 timerno;
    u32 temp;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        __GPTimer_Time2Counter(cycle,&counter);

        timerno = timer->timerno;

        if(timerno==0)
        {
            *pTM0PRD = counter&CN_GP_TIMER_COUNT_MAX;
                return true;
        }

        else if(timerno==1)
        {
            *pTM1PRD = counter&CN_GP_TIMER_COUNT_MAX;
                return true;
        }
        else
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
// ��������:__GPTimer_SetAutoReload
//          �趨����
// �������:timer��PIC��ʱ��
//         autoreload, true���Լ�reload���ڣ������ֶ�ÿ����������
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵����SHARC21469 GP_Timer0/GP_Timer1Ĭ��Ϊreload��Ҫ��ı����ڣ���Ҫ��disable��Ȼ���������������ڡ�
// =============================================================================

bool_t  __GPTimer_SetAutoReload(struct GPTimerHandle  *timer, bool_t autoreload)
{

    if(timer->timerstate&CN_TIMER_ENUSE)
    {
        if(autoreload)
        {
            timer->timerstate= timer->timerstate | CN_TIMER_RELOAD;
        }
        else
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
// ��������:__GPTimer_Alloc
//          ���䶨ʱ��
// �������:timerisr,��ʱ�����жϴ�����
// �������:
// ����ֵ  :����Ķ�ʱ�������NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
ptu32_t __GPTimer_Alloc(fnTimerIsr timerisr)
{
    u8 timerno;
    u8 i=0;
    u8 irqline;
    struct GPTimerHandle  *timer;
    ptu32_t timerhandle;
    //ԭ�Ӳ�������ֹ��Դ����
    atom_low_t  timeratom;
    timeratom = Int_LowAtomStart();

    timer=&s_tGPTimerHandle[0];
    if(!timer->timerstate)
    {
        timerno=timer->timerno;

    }
    else
    {
        timer=&s_tGPTimerHandle[1];
        if(!timer->timerstate)
        {
            timerno=timer->timerno;

        }
        else   //û�еĻ�ֱ�ӷ��ؾͿ����ˣ��ò����ن�����
        {
            return NULL;
        }
    }
    Int_LowAtomEnd(timeratom);  //ԭ�Ӳ������

    irqline = sgHaltimerIrq[timerno];
    timer = &s_tGPTimerHandle[timerno];
    timer->cycle = 0;
    timer->timerno = timerno;
    timer->irqline = irqline;
    timer->timerstate = CN_TIMER_ENUSE;
    //���ˣ��жϺźͶ�ʱ�����붼���ˣ��ø���͸����ˡ�
    //�����úö�ʱ������
    //__P1020PicTimer_PauseCount(timer);
//    __GPTimer_SetCycle(timer,cycle);
    //���ö�ʱ���ж�,�Ƚ��������ж����еĹ����������
    Int_Register(irqline);
    Int_CutLine(irqline);
    Int_IsrDisConnect(irqline);
    Int_EvttDisConnect(irqline);
    Int_SettoAsynSignal(irqline);
    Int_IsrConnect(irqline, timerisr);
    timerhandle = (ptu32_t)timer;

    return timerhandle;
}


// =============================================================================
// ��������:__GPTimer_Free
//          �ͷŶ�ʱ��
// �������:timer������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __GPTimer_Free(ptu32_t timerhandle)
{
    u8 timerno;
    u8 irqline;
    atom_low_t  timeratom;  //����������Դ
    struct GPTimerHandle  *timer;
    timer = (struct GPTimerHandle  *)timerhandle;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        irqline = timer->irqline;
        if(timerno < CN_GP_TIMER_NUM )//���п��еģ������ñ�־λ
        {       //�޸�ȫ�ֱ�־һ����ԭ���Ե�
            timeratom = Int_LowAtomStart();
            //������ж�������������
            timer->timerstate = 0;
            Int_CutLine(irqline);
            Int_IsrDisConnect(irqline);
            Int_EvttDisConnect(irqline);
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
// ��������:__P1020PicTimer_SetIntPro
//          ���ö�ʱ�����ж�����
// �������:timer���������Ķ�ʱ��
//          real_priorΪtrue��Ϊʵʱ�źŷ���Ϊ�첽�ź�
// �������:
// ����ֵ  :����Ķ�ʱ����NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
bool_t  __GPTimer_SetIntPro(struct GPTimerHandle  *timer, bool_t real_prior)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        if(real_prior)
        {
            return false;
        }
        else
        {
            timer->timerstate = (timer->timerstate)&(~CN_TIMER_REALINT);
            //return Int_SettoAsynSignal(timer->irqline);
            return true;
        }
    }
    else
    {
        return false;
    }
}

// =============================================================================
// ��������:__GPTimer_EnInt
//          ʹ�ܶ�ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __GPTimer_EnInt(struct GPTimerHandle  *timer)
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
// ��������:__GPTimer_DisInt
//          ��ֹ��ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __GPTimer_DisInt(struct GPTimerHandle  *timer)
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
// ��������:__GPTimer_GetTime
//          ��ȡ��ʱ����ʱ
// �������:timer���������Ķ�ʱ��
// �������:time����ʱ��΢�룩
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :���趨���������𣬼�cycle-ʣ��ʱ��,��ʾ�Ѿ��ߵ���ʱ��
// =============================================================================
bool_t __GPTimer_GetTime(struct GPTimerHandle  *timer, u32 *time)
{
    u8 timerno;
    u32 temp;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno >=CN_GP_TIMER_NUM)
        {
            return false;
        }
        else
        {
            if(timerno==0)
            {
                *pTMSTAT=(~TIM0EN);
                counter=*pTM0CNT;
                temp=*pTM0PRD;
                counter=counter-(0xFFFFFFFF-temp);
            }
            else
            {
                *pTMSTAT=(~TIM1EN);
                counter=*pTM1CNT;
                temp=*pTM1PRD;
                counter=counter-(0xFFFFFFFF-temp);
            }
            return __GPTimer_Counter2Time(counter, time);
        }
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:__GPTimer_CheckTimeout
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timer���������Ķ�ʱ��
// �������:timeout, true��ʱ��false����ʱ
// ����ֵ  :true�ɹ� falseʧ��
// ˵��    :
// =============================================================================
bool_t __GPTimer_CheckTimeout(struct GPTimerHandle  *timer, bool_t *timeout)
{
    bool_t result;
    u8 timerno;
    u32 temp;
    u32 counter;
    u32 *time;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno >=CN_GP_TIMER_NUM)
        {
            result = false;
        }
        else
        {
            result = true;

            __GPTimer_GetTime(timer, time);
            if(*time>timer->cycle)
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
// �������ܣ�_GPTimer_GetID
//          ��ȡ��ʱ��ID
// ���������timer����������ʱ����
// ���������timerId,��16λΪTIMERNO,��16Ϊ��Ӧ��IRQNO
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __GPTimer_GetID(struct GPTimerHandle   *timer,u32 *timerId)
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
// �������ܣ�_GPTimer_GetCycle
//          ��ȡ��ʱ������
// ���������timer����������ʱ����
// ���������cycle����ʱ������(΢��)
// ����ֵ  ��true �ɹ� falseʧ��
// =============================================================================
bool_t __GPTimer_GetCycle(struct GPTimerHandle   *timer, u32 *cycle)
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
// �������ܣ�_GPTimer_GetState
//          ��ȡ��ʱ��״̬
// ���������timer,�������Ķ�ʱ��
// ���������timerflag����ʱ��״̬
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __GPTimer_GetState(struct GPTimerHandle   *timer, u32 *timerflag)
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
// ��������:__P1020PicTimer_Ctrl
//            ��ʱ����������
// �������:timerhandle �������Ķ�ʱ�����
//          ctrlcmd, ����������
// �������:inoutpara,����ctrlcmd�Ĳ�ͬ����ͬ
// ����ֵ  :true�ɹ�  falseʧ��
// ˵��    :
// =============================================================================
bool_t __GPTimer_Ctrl(ptu32_t timerhandle, \
                         enum TimerCmdCode ctrlcmd, \
                         ptu32_t inoutpara)
{
    bool_t result;
    struct GPTimerHandle  *timer;
    timer = (struct GPTimerHandle  *)timerhandle;
    if(NULL == timer)
    {
        result = false;
    }
    else
    {
        switch(ctrlcmd)
        {
            case EN_TIMER_STARTCOUNT:
                result = __GPTimer_StartCount(timer);
                break;
            case EN_TIMER_PAUSECOUNT:
                result = __GPTimer_PauseCount(timer);
                break;
            case EN_TIMER_SETCYCLE:
                result = __GPTimer_SetCycle(timer,(u32)inoutpara);
                break;
            case EN_TIMER_SETRELOAD:
                result = __GPTimer_SetAutoReload(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_ENINT:
                result = __GPTimer_EnInt(timer);
                break;
            case EN_TIMER_DISINT:
                result = __GPTimer_DisInt(timer);
                break;
            case EN_TIMER_SETINTPRO:
                result = __GPTimer_SetIntPro(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_GETTIME:
                result = __GPTimer_GetTime(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETCYCLE:
                result = __GPTimer_GetCycle(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETID:
                result = __GPTimer_GetID(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETSTATE:
                result = __GPTimer_GetState(timer, (u32 *)inoutpara);
                break;
            default:
                break;
        };
    }

    return result;
}

// =============================================================================
// ��������:__GPTimer0_ISR
//           GP Timer0�жϷ�����������ж�״̬λ
// �������:GPTMR1I_int_line GP Timer0�жϺ�
// �������:��
// ����ֵ  :��
// ˵��    :
// =============================================================================
void __GPTimer0_ISR(ptu32_t GPTMR0I_int_line)
{
    //MUST clear timer0 interrupt status first
    *pTMSTAT = TIM0IRQ;

    asm("nop;");

    s_s32GPTimerHandle++;
    printf("GP Timer0 interrupt  \n"); //Cost too much cycle in printf, so just for demo. Need to remove it.

    //return 1;
}

// =============================================================================
// ��������:__GPTimer1_ISR
//           GP Timer1�жϷ�����������ж�״̬λ
// �������:GPTMR1I_int_line GP Timer1�жϺ�
// �������:��
// ����ֵ  :��
// ˵��    :
// =============================================================================
void __GPTimer1_ISR(ptu32_t GPTMR1I_int_line)
{
    //MUST clear timer1 interrupt status first
    *pTMSTAT = TIM1IRQ;
    asm("nop;");

    s_s32Timer1Counter++;
    printf("GP Timer1 interrupt  \n");  //Cost too much cycle in printf, so just for demo. Need to remove it.
}



// =============================================================================
// ��������:module_init_timer
//          SHARC21469��GP timer��ʼ��
// �������:
// �������:
// ����ֵ  :
// ˵��    :
// =============================================================================
void GPTimer_ModuleInit(void)
{
    struct TimerChip  Sharc21469GPtimer;

    Int_Register(cn_int_line_GPTMR0I);
    Int_IsrConnect(cn_int_line_GPTMR0I,__GPTimer0_ISR);
    Int_SettoAsynSignal(cn_int_line_GPTMR0I);
    Int_ClearLine(cn_int_line_GPTMR0I);     //�����ʼ�������ķ���fifo�յ��ж�
    Int_RestoreAsynLine(cn_int_line_GPTMR0I);

    Int_Register(cn_int_line_GPTMR1I);
    Int_IsrConnect(cn_int_line_GPTMR1I,__GPTimer1_ISR);
    Int_SettoAsynSignal(cn_int_line_GPTMR1I);
    Int_ClearLine(cn_int_line_GPTMR1I);     //�����ʼ�������ķ���fifo�յ��ж�
    Int_RestoreAsynLine(cn_int_line_GPTMR1I);


    *pTM0CTL = TIMODEPWM | PRDCNT | IRQEN;      // configure the timer0
    *pTM0W = 1;                             // timer0 width

    *pTM1CTL = TIMODEPWM | PRDCNT | IRQEN;      // configure the timer1
    *pTM1W = 1;                             // timer1 width register


    Sharc21469GPtimer.chipname = "SHARC21469GPTimer";
    Sharc21469GPtimer.HardTimerAlloc = __GPTimer_Alloc;
    Sharc21469GPtimer.HardTimerFree = __GPTimer_Free;
    Sharc21469GPtimer.HardTimerCtrl = __GPTimer_Ctrl;
    HardTimer_RegisterChip(&Sharc21469GPtimer);


    // =============��������������ʹ��===================================
/*
struct GPTimerHandle *testTimer;
//step1:����һ����ʱ��
  testTimer=__GPTimer_Alloc(30000000, __GPTimer0_ISR);
    //step2:���ö�ʱ����
   // __GPTimer_SetCycle(testTimer, 30000000);
    //step3:ʹ���ж�
    __GPTimer_EnInt(testTimer);
    //��ʼ����
    __GPTimer_StartCount(testTimer);
   */

}
