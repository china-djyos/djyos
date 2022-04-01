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
//todo: ����,�޸�ע��\�ļ�����.
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
//attribute:bsp                  //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                 //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                 //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"Software Timers","int"//������������������������none����ʾ�������������
                                 //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                 //����������������������г����á�,���ָ�
//weakdependence:"none"          //��������������������������none����ʾ�������������
                                 //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                 //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                 //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_TIMER == false )
//#warning  " cpu_onchip_timer  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header             //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_TIMER    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


#define CN_PIT_MAX_COUNTER     0xFFFF
#define CN_PIT_MAX_TIME_US     (0xFFFF)

// =============================================================================
#define CN_TIMER6_BASE          0x40001000
#define CN_TIMER7_BASE          0x40001400
// =============================================================================
static tagTimerReg volatile * const tg_TIMER_Reg[] = {
                                            (tagTimerReg *)CN_TIMER6_BASE,
                                            (tagTimerReg *)CN_TIMER7_BASE};

enum ENUM_STM32_TIMER
{
    EN_STM32TIMER_6=0,
    EN_STM32TIMER_7,
};

//������ʱ��оƬ�Ķ�ʱ��Ӧ�����Լ��ľ��
struct STM32TimerHandle
{
    u32     timerno;          //��ʱ����
    u32     irqline;          //�жϺ�
    u32     cycle;            //��ʱ����
    u32     timerstate;       //��ʱ����ʶ
};
#define CN_STM32TIMER_NUM   (EN_STM32TIMER_7 +1)
#define CN_STM32TIMER_MAX    EN_STM32TIMER_7

static struct STM32TimerHandle  stgTimerHandle[CN_STM32TIMER_NUM];

//���λ����timer0 ��������
static u32  gs_dwSTM32TimerBitmap;  //���ڶ�ʱ�����ֶ�����һ��Ĳ���ܶ࣬32��Ӧ���㹻
#define CN_STM32TIMER_BITMAP_MSK  (0x80000000)  //���λΪ1���������Ƽ���

//timer0..timern��irq
static u32 sgHaltimerIrq[CN_STM32TIMER_NUM]={
                                            //CN_INT_LINE_TIM2,
                                            //CN_INT_LINE_TIM3,
                                            //CN_INT_LINE_TIM4,
                                            //CN_INT_LINE_TIM5,
                                            CN_INT_LINE_TIM6,\
                                            CN_INT_LINE_TIM7};
//��ȡ32λ����һ��0bitλ��(�Ӹ�λ����λ����)
u8 __STM32Timer_GetFirstZeroBit(u32 para)
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
// ��������:__STM32Timer_Time2Counter
//          ���ṩ��ʱ�任��Ϊ��ʱ���ļ�����λ,PITʱ����50M�Ķ�ʱƵ��
// �������:time��Ҫ�����ʱ��(��λΪ΢��)
// �������:counter,��Ӧʱ���counter
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :time̫���ʧ�ܣ������counter���Ƕ�ʱ�������ֵ
// =============================================================================
bool_t __STM32Timer_Time2Counter(u32 time, u32 *counter)
{
    u32 time_set;
    time_set = time > CN_PIT_MAX_TIME_US ? CN_PIT_MAX_TIME_US : time;
    if(CN_PIT_MAX_TIME_US > time)
    {
        time_set = time;
        *counter =  time_set;
        return true;
    }
    else
    {
        time_set = CN_PIT_MAX_TIME_US;
        *counter =  time_set;
        return false;
    }
}
// =============================================================================
// ��������:__STM32Timer_Counter2Time
//          ����ʱ���ļ�����λ����Ϊʱ��
// �������:counter,��ʱ����counter��Ԫ
// �������:time�������ʱ��(��λΪ΢��)
// ����ֵ  :true�ɹ� fasleʧ��
// =============================================================================
bool_t __STM32Timer_Counter2Time(u32 counter,u32 *time)
{
    if(counter > CN_PIT_MAX_COUNTER)
    {
        *time = CN_PIT_MAX_TIME_US;
        return false;
    }
    else
    {
        *time = counter;
        return true;
    }
}
// =============================================================================
// ��������:__STM32Timer_PauseCount
//          ֹͣ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __STM32Timer_PauseCount(struct STM32TimerHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_STM32TIMER_MAX)
        {
            return false;
        }
        else
        {
            tg_TIMER_Reg[timerno]->CR1 &= ~TIM_CR1_CEN_MASK;
            tg_TIMER_Reg[timerno]->SR = 0;//���жϱ�־
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
// ��������:__STM32Timer_StartCount
//          ��ʼ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __STM32Timer_StartCount(struct STM32TimerHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_STM32TIMER_MAX)
        {
            return false;
        }
        else
        {
            //�ƺ���timer��bug��+����ģʽʱ���þ���һ��CNT�������Ƶ�0�Ժ�reload����
            //����Ч���ʳ�ֵ��Ϊ 0xffffffff
            tg_TIMER_Reg[timerno]->CNT = 0xffffffff;
            tg_TIMER_Reg[timerno]->SR = 0;//���жϱ�־
            tg_TIMER_Reg[timerno]->CR1 |= TIM_CR1_CEN_MASK;
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
// ��������:__STM32Timer_SetCycle,STM32��timer,���Ϊ0xFFFFFFFF,����Timer_ModuleInit
//          �趨������ÿ��һ����Ϊ1us����֪���ʱ�䲻��-1
//          �趨����
// �������:timer��PIC��ʱ��
//          cycle,���ڣ���ʱ��ʱ�Ӹ�������
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :�����������̫�󣨳������ʱ����������������Ϊ��ʱ�����������
// =============================================================================
bool_t  __STM32Timer_SetCycle(struct STM32TimerHandle  *timer, u32 cycle)
{
    u8 timerno;
    u32 counter,time_set;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_STM32TIMER_MAX)
        {
            return false;
        }
        else
        {
            __STM32Timer_Time2Counter(cycle,&counter);
            tg_TIMER_Reg[timerno]->ARR = (u16)counter;
            __STM32Timer_Counter2Time(counter,&time_set);
            timer->cycle = time_set;

            return true;
        }
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:__STM32Timer_SetAutoReload
//          �趨����
// �������:timer��PIC��ʱ��
//         autoreload, true���Լ�reload���ڣ������ֶ�ÿ����������
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// =============================================================================
bool_t  __STM32Timer_SetAutoReload(struct STM32TimerHandle  *timer, bool_t autoreload)
{
    bool_t result;
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno < CN_STM32TIMER_MAX)
        {
            if(autoreload == true)
            {
                tg_TIMER_Reg[timerno]->CR1 |= TIM_CR1_ARPE_MASK;
            }
            else
            {
                tg_TIMER_Reg[timerno]->CR1 &= ~TIM_CR1_ARPE_MASK;
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
// =============================================================================
// ��������:__STM32Timer_Alloc
//          ���䶨ʱ��
// �������:timerisr,��ʱ�����жϴ�����
// �������:
// ����ֵ  :����Ķ�ʱ�������NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
ptu32_t __STM32Timer_Alloc(fnTimerIsr timerisr)
{
    u8 timerno;
    u8 irqline;
    struct STM32TimerHandle  *timer;
    ptu32_t timerhandle;
    //ԭ�Ӳ�������ֹ��Դ����
    atom_low_t  timeratom;
    timeratom = Int_LowAtomStart();

    //Ѱ�ҿ��е�timer
    timerno = __STM32Timer_GetFirstZeroBit(gs_dwSTM32TimerBitmap);
    if(timerno < CN_STM32TIMER_NUM)//���п��еģ������ñ�־λ
    {
        gs_dwSTM32TimerBitmap = gs_dwSTM32TimerBitmap | (CN_STM32TIMER_BITMAP_MSK<< timerno);
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
    //���ˣ��жϺźͶ�ʱ�����붼���ˣ��ø���͸����ˡ�
    //�����úö�ʱ������
    __STM32Timer_PauseCount(timer);
//    __STM32Timer_SetCycle(timer,cycle);
    //���ö�ʱ���ж�,�Ƚ��������ж����еĹ����������
    Int_Register(irqline);
    Int_DisableLine(irqline);

    Int_IsrDisConnect(irqline);
    Int_EvttDisConnect(irqline);
    Int_SettoAsynSignal(irqline);
    Int_IsrConnect(irqline, timerisr);
    timerhandle = (ptu32_t)timer;

    return timerhandle;
}


// =============================================================================
// ��������:__STM32Timer_Free
//          �ͷŶ�ʱ��
// �������:timer������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __STM32Timer_Free(ptu32_t timerhandle)
{
    u8 timerno;
    u8 irqline;
    atom_low_t  timeratom;  //����������Դ
    struct STM32TimerHandle  *timer;
    timer = (struct STM32TimerHandle  *)timerhandle;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        irqline = timer->irqline;
        if(timerno < CN_STM32TIMER_NUM)//���п��еģ������ñ�־λ
        {       //�޸�ȫ�ֱ�־һ����ԭ���Ե�
            timeratom = Int_LowAtomStart();
            gs_dwSTM32TimerBitmap = gs_dwSTM32TimerBitmap &(~(CN_STM32TIMER_BITMAP_MSK<< timerno));
            //������ж�������������
            timer->timerstate = 0;
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
// ��������:__STM32Timer_SetIntPro
//          ���ö�ʱ�����ж�����
// �������:timer���������Ķ�ʱ��
//          real_priorΪtrue��Ϊʵʱ�źŷ���Ϊ�첽�ź�
// �������:
// ����ֵ  :����Ķ�ʱ����NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
bool_t  __STM32Timer_SetIntPro(struct STM32TimerHandle  *timer, bool_t real_prior)
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
// ��������:__STM32Timer_EnInt
//          ʹ�ܶ�ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __STM32Timer_EnInt(struct STM32TimerHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)| (CN_TIMER_ENINT);
        return Int_EnableLine(timer->irqline);
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:__STM32Timer_DisInt
//          ��ֹ��ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __STM32Timer_DisInt(struct STM32TimerHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)&(~CN_TIMER_ENINT);
        return Int_DisableLine(timer->irqline);
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:__STM32Timer_GetTime
//          ��ȡ��ʱ����ʱ
// �������:timer���������Ķ�ʱ��
// �������:time����ʱ��΢�룩
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :���趨���������𣬼�cycle-ʣ��ʱ��,��ʾ�Ѿ��ߵ���ʱ��(��λ����ʱ����Ƶʱ�Ӹ���)
// =============================================================================
bool_t __STM32Timer_GetTime(struct STM32TimerHandle  *timer, u32 *time)
{
    u8 timerno;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_STM32TIMER_MAX)
        {
            return false;
        }
        counter = tg_TIMER_Reg[timerno]->CNT;   //CNT����΢����
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
bool_t __STM32Timer_GetReg(struct STM32TimerHandle  *timer, void **reg)
{
    u8 timerno;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_STM32TIMER_MAX)
        {
            return false;
        }
        *reg = tg_TIMER_Reg[timerno];
        return true;
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:__STM32Timer_CheckTimeout
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timer���������Ķ�ʱ��
// �������:timeout, true��ʱ��false����ʱ
// ����ֵ  :true�ɹ� falseʧ��
// ˵��    :
// =============================================================================
bool_t __STM32Timer_CheckTimeout(struct STM32TimerHandle  *timer, bool_t *timeout)
{
    bool_t result=true;
    u8 timerno;
    u32 temp;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_STM32TIMER_MAX)
        {
            result = false;
        }
        else
        {
            temp = tg_TIMER_Reg[timerno]->ARR;
            counter = tg_TIMER_Reg[timerno]->CNT;
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
// �������ܣ�__STM32Timer_GetID
//          ��ȡ��ʱ��ID
// ���������timer����������ʱ����
// ���������timerId,��16λΪ irqno,��16Ϊ��Ӧ�� timerno
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __STM32Timer_GetID(struct STM32TimerHandle   *timer,u32 *timerId)
{
    u16 irqno;
    u16 timerno;

    if(NULL == timer)
    {
        return false;
    }
    else
    {
        timerno = (u16)timer->timerno;
        irqno = (u16)timer->irqline;
        *timerId = (irqno<<16)|(timerno);
        return true;
    }
}

// =============================================================================
// �������ܣ�__STM32Timer_GetCycle
//          ��ȡ��ʱ������
// ���������timer����������ʱ����
// ���������cycle����ʱ������(��λ����ʱ����Ƶʱ�Ӹ���)
// ����ֵ  ��true �ɹ� falseʧ��
// =============================================================================
bool_t __STM32Timer_GetCycle(struct STM32TimerHandle   *timer, u32 *cycle)
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
// �������ܣ�__STM32Timer_GetState
//          ��ȡ��ʱ��״̬
// ���������timer,�������Ķ�ʱ��
// ���������timerflag����ʱ��״̬
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __STM32Timer_GetState(struct STM32TimerHandle   *timer, u32 *timerflag)
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
// ��������:__STM32Timer_Ctrl
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timerhandle �������Ķ�ʱ�����
//         ctrlcmd, ����������
// �������:inoutpara,����ctrlcmd�Ĳ�ͬ����ͬ
// ����ֵ  :true�ɹ�  falseʧ��
// ˵��    :
// =============================================================================
bool_t __STM32Timer_Ctrl(ptu32_t timerhandle, \
                         enum HardTimerCmdCode ctrlcmd, \
                         ptu32_t inoutpara)
{
    bool_t result = false;
    struct STM32TimerHandle  *timer;
    timer = (struct STM32TimerHandle  *)timerhandle;
    if(NULL != timer)
    {
        switch(ctrlcmd)
        {
            case EN_TIMER_STARTCOUNT:
                result = __STM32Timer_StartCount(timer);
                break;
            case EN_TIMER_PAUSECOUNT:
                result = __STM32Timer_PauseCount(timer);
                break;
            case EN_TIMER_SETCYCLE:
                result = __STM32Timer_SetCycle(timer,(u32)inoutpara);
                break;
            case EN_TIMER_SETRELOAD:
                result = __STM32Timer_SetAutoReload(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_ENINT:
                result = __STM32Timer_EnInt(timer);
                break;
            case EN_TIMER_DISINT:
                result = __STM32Timer_DisInt(timer);
                break;
            case EN_TIMER_SETINTPRO:
                result = __STM32Timer_SetIntPro(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_GETTIME:
                result = __STM32Timer_GetTime(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETCYCLE:
                result = __STM32Timer_GetCycle(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETID:
                result = __STM32Timer_GetID(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETSTATE:
                result = __STM32Timer_GetState(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETREG:
                result = __STM32Timer_GetReg(timer, (void **)inoutpara);
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
u32  __STM32Timer_GetFreq(ptu32_t timerhandle)
{
    //��ʱ��TIM234567ʱ��ԴΪ��������ʱ��PCLK1���ٶ�Ϊ36M
    //��ʱ����Ƶ����Ϊ36����һ��ʱ��Ϊ1uS����ʱ����ƵΪ1MHz
    return 1000000;
}
// =============================================================================
// ��������:module_init_timer
//          P1020��PICtimer��ʼ��,timerʱ��ԴΪAPB2 = CN_CFG_PCLK1 = 36M(timer234567)
// �������:
// �������:
// ����ֵ  :
// ˵��    :
// =============================================================================
bool_t ModuleInstall_HardTimer(void)
{
    struct TimerChip  STM32timer;
    u32 temp;
    pg_rcc_reg->APB1ENR |= (1<<4)|(1<<5);//ʱ��ʹ��,timer6/7��ʹ��λΪ����4/5
    //��ʱ��TIM234567ʱ��ԴΪ��������ʱ��PCLK1���ٶ�Ϊ36M
    for(temp = 0; temp < CN_STM32TIMER_NUM; temp++)
    {
        tg_TIMER_Reg[temp]->CR1 &= ~(TIM_CR1_CEN_MASK); //��ֹTIMER
        tg_TIMER_Reg[temp]->CR1 |= TIM_CR1_ARPE;//�Զ���װ

        tg_TIMER_Reg[temp]->DIER |= TIM_DIER_UIE_MASK;
        tg_TIMER_Reg[temp]->PSC = 0x23;//����Ϊ36����һ��ʱ��Ϊ1uS
        tg_TIMER_Reg[temp]->ARR = 0xFFFF;
    }

    STM32timer.chipname = "STM32TIMER";
    STM32timer.HardTimerAlloc = __STM32Timer_Alloc;
    STM32timer.HardTimerFree = __STM32Timer_Free;
    STM32timer.HardTimerCtrl = __STM32Timer_Ctrl;
    STM32timer.HardTimerGetFreq = __STM32Timer_GetFreq;
    HardTimer_RegisterChip(&STM32timer);

    return true;
}
