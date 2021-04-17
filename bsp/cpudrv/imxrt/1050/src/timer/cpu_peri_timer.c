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

// �ļ���     ��cpu_peri_timer.c
// ģ������: ��ʱ������ģ��
// ģ��汾: V1.0.0
// ������Ա: chj
// ����ʱ��: 2018-05-07
// =============================================================================
#include "stdio.h"
#include "cpu_peri_timer.h"
#include "cpu_peri_int_line.h"
#include "int.h"
#include "djyos.h"
#include "timer_hard.h"

#include "fsl_gpt.h"
#include "fsl_clock.h"
//#include "clock_config.h"
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
//attribute:bsp                     //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                  //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                    //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                   //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                    //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"Software Timers","int"//������������������������none����ʾ�������������
                                    //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                    //����������������������г����á�,���ָ�
//weakdependence:none               //��������������������������none����ʾ�������������
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
//%$#@num,32,512,

//%$#@enum,true,false,
//%$#@string,1,30,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Select IPG Clock as PERCLK_CLK clock source */
#define EXAMPLE_GPT_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for PERCLK_CLK clock source */
#define EXAMPLE_GPT_CLOCK_DIVIDER_SELECT (0U)
#define EXAMPLE_GPT_CLOCK_DIV            (3U)
/* Get source clock for GPT driver (GPT prescaler = 0) */
#define EXAMPLE_GPT_CLK_FREQ (CLOCK_GetFreq(kCLOCK_IpgClk) / (EXAMPLE_GPT_CLOCK_DIVIDER_SELECT + 1U))
#define EXAMPLE_GPT_CLOCK_FREQ           ((EXAMPLE_GPT_CLK_FREQ/EXAMPLE_GPT_CLOCK_DIV)/100000)  //��λ��MHZ

#define CN_PIT_MAX_COUNTER     0xFFFFFFFF
#define CN_PIT_MAX_TIME_US     (0xFFFFFFFF/EXAMPLE_GPT_CLOCK_FREQ)

#define CN_CHANNEL              kGPT_OutputCompare_Channel1
#define CN_INTEERUPT            kGPT_OutputCompare1InterruptEnable
#define CN_INTEERUPT_FLAG       kGPT_OutputCompare1Flag
typedef GPT_Type    tagTimerReg;

static tagTimerReg volatile * const tg_TIMER_Reg[] = {
                                                        (tagTimerReg *)GPT1_BASE,
                                                        (tagTimerReg *)GPT2_BASE
                                                     };

enum ENUM_TIMER
{
    EN_TIMER_1=0,//��˳��ֵ
    EN_TIMER_2
};


//������ʱ��оƬ�Ķ�ʱ��Ӧ�����Լ��ľ��
struct TimerHandle
{
    u32     timerno;          //��ʱ����
    u32     irqline;          //�жϺ�
    u32     cycle;            //��ʱ����
    u32     timerstate;       //��ʱ����ʶ
    fnTimerIsr UserIsr;      //�û��ж���Ӧ����
};

#define CN_TIMER_NUM   (EN_TIMER_2 +1)
#define CN_TIMER_MAX    EN_TIMER_2

static struct TimerHandle  stgTimerHandle[CN_TIMER_NUM];

//���λ����timer0����������
static u32  gs_dwTimerBitmap;  //���ڶ�ʱ�����ֶ�����һ��Ĳ���ܶ࣬32��Ӧ���㹻
#define CN_TIMER_BITMAP_MSK  (0x80000000)  //���λΪ1���������Ƽ���

//timer0��timern��irq�ж�
static u32 sgHaltimerIrq[CN_TIMER_NUM]={
                                                CN_INT_LINE_GPT1,
                                                CN_INT_LINE_GPT2
                                            };

//��ȡ32λ����һ��0bitλ��(�Ӹ�λ����λ����)
u8 __Timer_GetFirstZeroBit(u32 para)
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
// ��������:__Time2Counter
//          ���ṩ��ʱ�任��Ϊ��ʱ���ļ�����λ,
// �������:time��Ҫ�����ʱ��(��λΪ΢��)
// �������:counter,��Ӧʱ���counter
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :time̫���ʧ�ܣ������counter���Ƕ�ʱ�������ֵ
// =============================================================================
bool_t __Time2Counter(u64 time, u64 *counter)
{

    u64 counter_1=time*EXAMPLE_GPT_CLOCK_FREQ;
    *counter = counter_1 > CN_PIT_MAX_COUNTER ? CN_PIT_MAX_COUNTER : counter_1;

    if(*counter == counter_1)
        return true;

    return false;

}

// =============================================================================
// ��������:__Counter2Time
//          ����ʱ���ļ�����λ����Ϊʱ��
// �������:counter,��ʱ����counter��Ԫ
// �������:time�������ʱ��(��λΪ΢��)
// ����ֵ  :true�ɹ� fasleʧ��
// =============================================================================
bool_t __Counter2Time(u64 counter,u64 *time)
{
    u32 time_1=counter/EXAMPLE_GPT_CLOCK_FREQ;
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
// ��������:__Timer_PauseCount
//          ֹͣ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __Timer_PauseCount(struct TimerHandle  *timer)
{
    u32  flag=0;
    u8 timerno;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_TIMER_MAX)
        {
            return false;
        }
        else
        {
            GPT_StopTimer((tagTimerReg*)tg_TIMER_Reg[timerno]);
            flag=GPT_GetEnabledInterrupts((tagTimerReg*)tg_TIMER_Reg[timerno]);
            GPT_ClearStatusFlags((tagTimerReg*)tg_TIMER_Reg[timerno], flag);
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
// ��������:__Timer_StartCount
//          ��ʼ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __Timer_StartCount(struct TimerHandle  *timer)
{
    u32 flag=0;
    u8 timerno;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_TIMER_MAX)
        {
            return false;
        }
        else
        {
            flag=GPT_GetEnabledInterrupts((tagTimerReg*)tg_TIMER_Reg[timerno]);
            GPT_ClearStatusFlags((tagTimerReg*)tg_TIMER_Reg[timerno], flag);
            GPT_StartTimer((tagTimerReg*)tg_TIMER_Reg[timerno]);
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
// ��������:__Timer_SetCycle,timer,���Ϊ0xFFFFFFFF,����Timer_ModuleInit
//          �趨������ÿ��һ����Ϊ(1/84)1us����֪���ʱ�䲻��-1
//          �趨����
// �������:timer��PIC��ʱ��
//          cycle,���ڣ���ʱ��ʱ�Ӹ�������
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :�����������̫�󣨳������ʱ����������������Ϊ��ʱ�����������
// =============================================================================
bool_t  __Timer_SetCycle(struct TimerHandle  *timer, u32 cycle)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_TIMER_MAX)
        {
            return false;
        }
        else
        {
            GPT_SetOutputCompareValue((tagTimerReg*)tg_TIMER_Reg[timerno],\
                                                            CN_CHANNEL, cycle);
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
// ��������:__Timer_SetAutoReload
//          �趨����
// �������:timer��PIC��ʱ��
//         autoreload, true���Լ�reload���ڣ������ֶ�ÿ����������
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// =============================================================================
bool_t  __Timer_SetAutoReload(struct TimerHandle  *timer, bool_t autoreload)
{
    bool_t result;
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno <= CN_TIMER_MAX)
        {
            if(autoreload == true)
            {
                return true;
            }
            else
            {
                return false;
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
u32 __Timer_isr(ptu32_t TimerHandle)
{
    u32 timerno;
    timerno = ((struct TimerHandle  *)TimerHandle)->timerno;
    GPT_ClearStatusFlags((tagTimerReg*)tg_TIMER_Reg[timerno], CN_INTEERUPT_FLAG);
    Int_ClearLine(((struct TimerHandle  *)TimerHandle)->irqline);
    return ((struct TimerHandle  *)TimerHandle)->UserIsr(TimerHandle);
}

// =============================================================================
// ��������:__Timer_Alloc
//          ���䶨ʱ��
// �������:timerisr,��ʱ�����жϴ�����
// �������:
// ����ֵ  :����Ķ�ʱ�������NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
ptu32_t __Timer_Alloc(fnTimerIsr timerisr)
{
    u8 timerno;
    u8 irqline;
    struct TimerHandle  *timer;
    ptu32_t timerhandle;
    //ԭ�Ӳ�������ֹ��Դ����
    atom_low_t  timeratom;
    timeratom = Int_LowAtomStart();

    //Ѱ�ҿ��е�timer
    timerno = __Timer_GetFirstZeroBit(gs_dwTimerBitmap);
    if(timerno < CN_TIMER_NUM)//���п��еģ������ñ�־λ
    {
        gs_dwTimerBitmap = gs_dwTimerBitmap | (CN_TIMER_BITMAP_MSK<< timerno);
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
    __Timer_PauseCount(timer);
    //__Timer_SetCycle(timer,cycle);
    //���ö�ʱ���ж�,�Ƚ��������ж����еĹ����������
    Int_Register(irqline);
    Int_CutLine(irqline);

    Int_IsrDisConnect(irqline);
    Int_EvttDisConnect(irqline);
    Int_SettoAsynSignal(irqline);

    Int_SetClearType(irqline,CN_INT_CLEAR_USER);
    Int_SetIsrPara(irqline,(ptu32_t)timer);

    Int_IsrConnect(irqline, __Timer_isr);
    timerhandle = (ptu32_t)timer;

    return timerhandle;
}

// =============================================================================
// ��������:__Timer_Free
//          �ͷŶ�ʱ��
// �������:timer������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __Timer_Free(ptu32_t timerhandle)
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
        if(timerno < CN_TIMER_NUM)//���п��еģ������ñ�־λ
        {       //�޸�ȫ�ֱ�־һ����ԭ���Ե�
            timeratom = Int_LowAtomStart();
            gs_dwTimerBitmap = gs_dwTimerBitmap &(~(CN_TIMER_BITMAP_MSK<< timerno));
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
// ��������:__Timer_SetIntPro
//          ���ö�ʱ�����ж�����
// �������:timer���������Ķ�ʱ��
//          real_priorΪtrue��Ϊʵʱ�źŷ���Ϊ�첽�ź�
// �������:
// ����ֵ  :����Ķ�ʱ����NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
bool_t  __Timer_SetIntPro(struct TimerHandle  *timer, bool_t real_prior)
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
// ��������:__Timer_EnInt
//          ʹ�ܶ�ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __Timer_EnInt(struct TimerHandle  *timer)
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
// ��������:__Timer_DisInt
//          ��ֹ��ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __Timer_DisInt(struct TimerHandle  *timer)
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
// ��������:__Timer_GetTime
//          ��ȡ��ʱ����ʱ
// �������:timer���������Ķ�ʱ��
// �������:time����ʱ��΢�룩
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :���趨���������𣬼�cycle-ʣ��ʱ��,��ʾ�Ѿ��ߵ���ʱ��(��λ����ʱ����Ƶʱ�Ӹ���)
// =============================================================================
bool_t __Timer_GetTime(struct TimerHandle  *timer, u32 *time)
{
    u8 timerno;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_TIMER_MAX)
        {
            return false;
        }
        counter = tg_TIMER_Reg[timerno]->CNT;//CNT����΢����
        *time = counter;
        return true;
    }
    else
    {
        return false;
    }
}

// ========================= ====================================================
// ��������:__Timer_CheckTimeout
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timer���������Ķ�ʱ��
// �������:timeout, true��ʱ��false����ʱ
// ����ֵ  :true�ɹ� falseʧ��
// ˵��    :
// =============================================================================
bool_t __Timer_CheckTimeout(struct TimerHandle  *timer, bool_t *timeout)
{
    bool_t result=true;
    u8 timerno;
    u32 temp;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_TIMER_MAX)
        {
            result = false;
        }
        else
        {
            temp=GPT_GetOutputCompareValue((tagTimerReg*)tg_TIMER_Reg[timerno],\
                                                                        CN_CHANNEL);
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
// �������ܣ�__Timer_GetID
//          ��ȡ��ʱ��ID
// ���������timer����������ʱ����
// ���������timerId,��16λΪTIMERNO,��16Ϊ��Ӧ��IRQNO
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __Timer_GetID(struct TimerHandle   *timer,u32 *timerId)
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
// �������ܣ�__Timer_GetCycle
//          ��ȡ��ʱ������
// ���������timer����������ʱ����
// ���������cycle����ʱ������(��λ����ʱ����Ƶʱ�Ӹ���)
// ����ֵ  ��true �ɹ� falseʧ��
// =============================================================================
bool_t __Timer_GetCycle(struct TimerHandle   *timer, u32 *cycle)
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
// �������ܣ�__Timer_GetState
//          ��ȡ��ʱ��״̬
// ���������timer,�������Ķ�ʱ��
// ���������timerflag����ʱ��״̬
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __Timer_GetState(struct TimerHandle   *timer, u32 *timerflag)
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
// ��������:__Timer_Ctrl
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timerhandle �������Ķ�ʱ�����
//         ctrlcmd, ����������
// �������:inoutpara,����ctrlcmd�Ĳ�ͬ����ͬ
// ����ֵ  : true�ɹ�  falseʧ��
// ˵��    :
// =============================================================================
bool_t __Timer_Ctrl(ptu32_t timerhandle, \
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
                result = __Timer_StartCount(timer);
                break;
            case EN_TIMER_PAUSECOUNT:
                result = __Timer_PauseCount(timer);
                break;
            case EN_TIMER_SETCYCLE:
                result = __Timer_SetCycle(timer,(u32)inoutpara);
                break;
            case EN_TIMER_SETRELOAD:
                result = __Timer_SetAutoReload(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_ENINT:
                result = __Timer_EnInt(timer);
                break;
            case EN_TIMER_DISINT:
                result = __Timer_DisInt(timer);
                break;
            case EN_TIMER_SETINTPRO:
                result = __Timer_SetIntPro(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_GETTIME:
                result = __Timer_GetTime(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETCYCLE:
                result = __Timer_GetCycle(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETID:
                result = __Timer_GetID(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETSTATE:
                result = __Timer_GetState(timer, (u32 *)inoutpara);
                break;
            default:
                break;
        };
    }

    return result;
}

// =============================================================================
// ��������:__Timer_GetFreq
//       ��ȡ��ʱ����Ƶ
// �������:timerhandle �������Ķ�ʱ�����
// �������:
// ����ֵ  :��ʱ����Ƶ
// ˵��    :��λ��HZ��
// =============================================================================
u32  __Timer_GetFreq(ptu32_t timerhandle)
{
    return (EXAMPLE_GPT_CLK_FREQ/EXAMPLE_GPT_CLOCK_DIV);
}

// =============================================================================
// ��������:module_init_timer
// �������:
// �������:
// ����ֵ  :
// ˵��    :
// =============================================================================
bool_t ModuleInstall_HardTimer(void)
{
    struct TimerChip  timer;
    gpt_config_t gptConfig;
    u8 i;

    /*Clock setting for GPT*/
    CLOCK_SetMux(kCLOCK_PerclkMux, EXAMPLE_GPT_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_PerclkDiv, EXAMPLE_GPT_CLOCK_DIVIDER_SELECT);
    GPT_GetDefaultConfig(&gptConfig);

    for(i=0;i<CN_TIMER_NUM;i++)
    {
        GPT_Init((tagTimerReg*)tg_TIMER_Reg[i], &gptConfig);
        GPT_SetClockDivider((tagTimerReg*)tg_TIMER_Reg[i], EXAMPLE_GPT_CLOCK_DIV);
        GPT_SetOutputCompareValue((tagTimerReg*)tg_TIMER_Reg[i], CN_CHANNEL, 0XFFFFFFFF);
        GPT_EnableInterrupts((tagTimerReg*)tg_TIMER_Reg[i], CN_INTEERUPT);
        GPT_StopTimer((tagTimerReg*)tg_TIMER_Reg[i]);
    }

    timer.chipname = "TIMER";
    timer.HardTimerAlloc = __Timer_Alloc;     //���䶨ʱ��
    timer.HardTimerFree = __Timer_Free;       //�ͷŶ�ʱ��
    timer.HardTimerCtrl = __Timer_Ctrl;       //���ƶ�ʱ��
    timer.HardTimerGetFreq = __Timer_GetFreq; //��ȡ��ʱ������Ƶ��

    HardTimer_RegisterChip(&timer);                //ע�ᶨʱ��оƬ��ϵͳ��ʱ��ģ��

    return true;
}
