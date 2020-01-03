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
//todo: �޸�ע��\�ļ�����.
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
//#include "cfg/misc_config.h"
#include "int.h"
#include "cpu.h"
#ifdef CFG_CPU_ZQ12XX_M0
#include "cpu_peri_int_line.h"
#endif
#include "timer_hard.h"

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
//parent:"Software Timers"//��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"Software Timers","int"//������������������������none����ʾ�������������
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

#ifdef CFG_CPU_ZQ12XX_M0
typedef struct _TIMER
{
    vu32 COUNTER;
    vu32 COMPARE;
    vu32 CONTROL;
    vu32 CLKSEL;
}tagTimerReg;

static tagTimerReg volatile * const tg_TIMER_Reg[] = {
                                    (tagTimerReg *)0x41860000,      //TIMER1
                                    (tagTimerReg *)0x41860010,      //TIMER2
                                    };

enum ENUM_CPU_TIMER
{
    EN_CPU_TIMER_1=0,
    EN_CPU_TIMER_2
};

//#define CN_TIMER_DIV     (CN_CFG_PCLK1/1000000)

//������ʱ��оƬ�Ķ�ʱ��Ӧ�����Լ��ľ��
struct CPUTimerHandle
{
    bool_t  autoReloadSet;    //�Զ���װ������
    u32     timerno;          //��ʱ����
    u32     irqline;          //�жϺ�
    u32     cycle;            //��ʱ����
    u32     timerstate;       //��ʱ����ʶ
    fnTimerIsr UserIsr;      //�û��ж���Ӧ����,���ڶ����ʱ������ һ���ж���,,�������������������Զֻ��һ��
};

enum TIM_RELOAD_SET{

    EN_RELOAD_NOT_SET = 0,    //���� ���Զ���װ��
    EN_RELOAD_SET  ,          //���� �Զ���װ��

};

#define CN_CPUTIMER_NUM   (EN_CPU_TIMER_2 +1)
#define CN_CPUTIMER_MAX    EN_CPU_TIMER_2


static struct CPUTimerHandle  stgTimerHandle[CN_CPUTIMER_NUM];

//���λ����timer0 ��������
static u32  gs_dwCPUTimerBitmap;  //���ڶ�ʱ�����ֶ�����һ��Ĳ���ܶ࣬32��Ӧ���㹻
#define CN_CPUTIMER_BITMAP_MSK  (0x80000000)  //���λΪ1���������Ƽ���

//timer0..timern��irq
static u8 spTimerIrqOffset[CN_CPUTIMER_NUM]={   0,1  };

#define TIMER_IRQ_ENABLE_ADDR                (0x42030000+0x620)
#define TIMER_IRQ_STA_ADDR                      (0x42030000+0x520)
//��ȡ32λ����һ��0bitλ��(�Ӹ�λ����λ����)
u8 __CPUTimer_GetFirstZeroBit(u32 para)
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
// ��������:__CPUTimer_PauseCount
//          ֹͣ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __CPUTimer_PauseCount(struct CPUTimerHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            return false;
        }
        else
        {
            tg_TIMER_Reg[timerno]->CONTROL = 0;         //ֹͣ�����������ж�
            tg_TIMER_Reg[timerno]->COUNTER = 0;         //�������
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
// ��������:__CPUTimer_StartCount
//          ��ʼ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __CPUTimer_StartCount(struct CPUTimerHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            return false;
        }
        else
        {
            tg_TIMER_Reg[timerno]->COUNTER    = 0;
            tg_TIMER_Reg[timerno]->CONTROL &= ~(1 << 1);            //���жϱ�ʶ
            tg_TIMER_Reg[timerno]->CONTROL    = (1<<0)|(1<<2);      //timer and int
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
// ��������:__CPUTimer_SetCycle,CPU��timer,���Ϊ0xFFFFFFFF,����Timer_ModuleInit
//          �趨������ÿ��һ����Ϊ(1/108)1us����֪���ʱ�䲻��-1
//          �趨����
// �������:timer��PIC��ʱ��
//          cycle,���ڣ���ʱ��ʱ�Ӹ�������
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :�����������̫�󣨳������ʱ����������������Ϊ��ʱ�����������
// =============================================================================
bool_t  __CPUTimer_SetCycle(struct CPUTimerHandle  *timer, u32 cycle)
{
    u8 timerno;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            return false;
        }
        else
        {
            tg_TIMER_Reg[timerno]->COMPARE = cycle;
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
// ��������:__CPUTimer_SetAutoReload
//          �趨����
// �������:timer��PIC��ʱ��
//         autoreload, true���Լ�reload���ڣ������ֶ�ÿ����������
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// =============================================================================
bool_t  __CPUTimer_SetAutoReload(struct CPUTimerHandle  *timer, bool_t autoreload)
{
    if(autoreload == true)
    {
        timer->autoReloadSet = EN_RELOAD_SET;
    }
    else{
        timer->autoReloadSet = EN_RELOAD_NOT_SET;
    }


    return true;        //reload every time
}

//-----------------------------------------------------------------------------
// ���ܣ���ʱ���ж���Ӧ������������atmel m7��������Ҫ���в�����ʱ���ļĴ�������
//       ���жϵĶ�ʱ��������������ʵ��ISR��Ȼ���ӵ���user ISR��������
//       user isr�н������ʴӡ�
// ��������ʱ�������
// ���أ�user ISR�ķ���ֵ
//-----------------------------------------------------------------------------
__attribute__((weak)) u32 __CPUTimer_isr(ptu32_t TimerHandle)
{
    u32 timerno;
    struct CPUTimerHandle  *timer = (struct CPUTimerHandle  *)TimerHandle;

    timerno = timer->timerno;
//  if((((u32*)TIMER_IRQ_STA_ADDR)[0])  & (1<< spTimerIrqOffset[0]))
//  {
//      timerno = 0;
//  }
//  else if((((u32*)TIMER_IRQ_STA_ADDR )[0])&(1<<spTimerIrqOffset[1]))
//  {
//      timerno = 1;
//  }

//  timer = &stgTimerHandle[timerno];
    tg_TIMER_Reg[timerno]->CONTROL = (1<<0) | (1<<2);

    if(timer->autoReloadSet == EN_RELOAD_NOT_SET)
    {
        tg_TIMER_Reg[timerno]->CONTROL = 0;         //ֹͣ�����������ж�
//        tg_TIMER_Reg[timerno]->COUNTER = 0;         //�������
    }
    return (timer->UserIsr(TimerHandle));
}

// =============================================================================
// ��������:__CPUTimer_Alloc
//          ���䶨ʱ��
// �������:timerisr,��ʱ�����жϴ�����
// �������:
// ����ֵ  :����Ķ�ʱ�������NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
ptu32_t __CPUTimer_Alloc(fnTimerIsr timerisr)
{
    u8 timerno;
    u8 irqline;
    struct CPUTimerHandle  *timer;
    ptu32_t timerhandle;
    u8 subIntLine;

    //ԭ�Ӳ�������ֹ��Դ����
    atom_low_t  timeratom;
    timeratom = Int_LowAtomStart();

    //Ѱ�ҿ��е�timer
    timerno = __CPUTimer_GetFirstZeroBit(gs_dwCPUTimerBitmap);
    if(timerno < CN_CPUTIMER_NUM)//���п��еģ������ñ�־λ
    {
        gs_dwCPUTimerBitmap = gs_dwCPUTimerBitmap | (CN_CPUTIMER_BITMAP_MSK>> timerno);

        Int_LowAtomEnd(timeratom);  //ԭ�Ӳ������
    }
    else//û�еĻ�ֱ�ӷ��ؾͿ����ˣ��ò����ن�����
    {
        Int_LowAtomEnd(timeratom);   //ԭ�Ӳ������
        return 0;
    }

    irqline = CN_INT_LINE_TIMER;
    timer = &stgTimerHandle[timerno];
    timer->cycle = 0;
    timer->timerno = timerno;
    timer->irqline = irqline;
    timer->timerstate = CN_TIMER_ENUSE;
    timer->UserIsr=timerisr;

    //���ˣ��жϺźͶ�ʱ�����붼���ˣ��ø���͸����ˡ�
    //�����úö�ʱ������
    __CPUTimer_PauseCount(timer);
    Int_SetIsrPara(irqline,(ptu32_t)timer);

    switch(timerno)
    {
        case EN_CPU_TIMER_1:
             subIntLine  = CN_SUBID_TIMER_0;
             break;
        case EN_CPU_TIMER_2:
             subIntLine  = CN_SUBID_TIMER_1;
             break;
        default:printk("no timer to use\r\n");break;
    }

    //Ĭ������²������Զ���װ��
    timer->autoReloadSet = EN_RELOAD_NOT_SET;

    djybsp_isr_hdl_register(irqline, subIntLine, __CPUTimer_isr,0);

    timerhandle = (ptu32_t)timer;

    return timerhandle;
}


// =============================================================================
// ��������:__CPUTimer_Free
//          �ͷŶ�ʱ��
// �������:timer������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __CPUTimer_Free(ptu32_t timerhandle)
{
    u8 timerno;
    u8 irqline;
    atom_low_t  timeratom;  //����������Դ
    struct CPUTimerHandle  *timer;
    timer = (struct CPUTimerHandle  *)timerhandle;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        irqline = timer->irqline;
        if(timerno < CN_CPUTIMER_NUM)//���п��еģ������ñ�־λ
        {       //�޸�ȫ�ֱ�־һ����ԭ���Ե�
            timeratom = Int_LowAtomStart();
            gs_dwCPUTimerBitmap = gs_dwCPUTimerBitmap &(~(CN_CPUTIMER_BITMAP_MSK<< timerno));
            //������ж�������������
            timer->timerstate = 0;
            timer->autoReloadSet = EN_RELOAD_NOT_SET;
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
// ��������:__CPUTimer_SetIntPro
//          ���ö�ʱ�����ж�����
// �������:timer���������Ķ�ʱ��
//          real_priorΪtrue��Ϊʵʱ�źŷ���Ϊ�첽�ź�
// �������:
// ����ֵ  :����Ķ�ʱ����NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
bool_t  __CPUTimer_SetIntPro(struct CPUTimerHandle  *timer, bool_t real_prior)
{      //������趨ʱ�����ù����ж��ߣ����ﲻ����
//    if(timer->timerstate & CN_TIMER_ENUSE)
//    {
//        if(real_prior)
//        {
//            timer->timerstate = (timer->timerstate)| (CN_TIMER_REALINT);
//            Int_EvttDisConnect(timer->irqline);
//            return Int_SettoReal(timer->irqline);
//        }
//        else
//        {
//            timer->timerstate = (timer->timerstate)&(~CN_TIMER_REALINT);
//            return Int_SettoAsynSignal(timer->irqline);
//        }
//    }
//    else
//    {
//        return false;
//    }

    return false;
}

// =============================================================================
// ��������:__CPUTimer_EnInt
//          ʹ�ܶ�ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __CPUTimer_EnInt(struct CPUTimerHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)| (CN_TIMER_ENINT);
        *(u32*)TIMER_IRQ_ENABLE_ADDR |= (1<<spTimerIrqOffset[timer->timerno]);

        return Int_ContactLine(timer->irqline);
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:__CPUTimer_DisInt
//          ��ֹ��ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __CPUTimer_DisInt(struct CPUTimerHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        *(u32*)TIMER_IRQ_ENABLE_ADDR &= ~(1<<spTimerIrqOffset[timer->timerno]);
        timer->timerstate = (timer->timerstate)&(~CN_TIMER_ENINT);
        return Int_CutLine(timer->irqline);
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:__CPUTimer_GetTime
//          ��ȡ��ʱ����ʱ
// �������:timer���������Ķ�ʱ��
// �������:time����ʱ��΢�룩
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :���趨���������𣬼�cycle-ʣ��ʱ��,��ʾ�Ѿ��ߵ���ʱ��(��λ����ʱ����Ƶʱ�Ӹ���)
// =============================================================================
bool_t __CPUTimer_GetTime(struct CPUTimerHandle  *timer, u32 *time)
{
    u8 timerno;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            return false;
        }
        counter = tg_TIMER_Reg[timerno]->COUNTER;   //CNT����΢����
        *time = counter;
        return true;
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:__CPUTimer_CheckTimeout
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timer���������Ķ�ʱ��
// �������:timeout, true��ʱ��false����ʱ
// ����ֵ  :true�ɹ� falseʧ��
// ˵��    :
// =============================================================================
bool_t __CPUTimer_CheckTimeout(struct CPUTimerHandle  *timer, bool_t *timeout)
{
    bool_t result=true;
    u8 timerno;
    u32 temp;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            result = false;
        }
        else
        {
            temp = tg_TIMER_Reg[timerno]->COMPARE;
            counter = tg_TIMER_Reg[timerno]->COUNTER;
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
// �������ܣ�__CPUTimer_GetID
//          ��ȡ��ʱ��ID
// ���������timer����������ʱ����
// ���������timerId,��16λΪTIMERNO,��16Ϊ��Ӧ��IRQNO
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __CPUTimer_GetID(struct CPUTimerHandle   *timer,u32 *timerId)
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
// �������ܣ�__CPUTimer_GetCycle
//          ��ȡ��ʱ������
// ���������timer����������ʱ����
// ���������cycle����ʱ������(��λ����ʱ����Ƶʱ�Ӹ���)
// ����ֵ  ��true �ɹ� falseʧ��
// =============================================================================
bool_t __CPUTimer_GetCycle(struct CPUTimerHandle   *timer, u32 *cycle)
{
    u8 timerno;

    if(NULL == timer)
    {
        return false;
    }
    else
    {
        timerno =  timer->timerno;
        tg_TIMER_Reg[timerno]->COMPARE = cycle;
        *cycle = timer->cycle;
        return true;
    }
}
// =============================================================================
// �������ܣ�__CPUTimer_GetState
//          ��ȡ��ʱ��״̬
// ���������timer,�������Ķ�ʱ��
// ���������timerflag����ʱ��״̬
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __CPUTimer_GetState(struct CPUTimerHandle   *timer, u32 *timerflag)
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
// ��������:__CPUTimer_Ctrl
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timerhandle �������Ķ�ʱ�����
//         ctrlcmd, ����������
// �������:inoutpara,����ctrlcmd�Ĳ�ͬ����ͬ
// ����ֵ  :true�ɹ�  falseʧ��
// ˵��    :
// =============================================================================
bool_t __CPUTimer_Ctrl(ptu32_t timerhandle, \
                         enum HardTimerCmdCode ctrlcmd, \
                         ptu32_t inoutpara)
{
    bool_t result = false;
    struct CPUTimerHandle  *timer;
    timer = (struct CPUTimerHandle  *)timerhandle;
    if(NULL != timer)
    {
        switch(ctrlcmd)
        {
            case EN_TIMER_STARTCOUNT:
                result = __CPUTimer_StartCount(timer);
                break;
            case EN_TIMER_PAUSECOUNT:
                result = __CPUTimer_PauseCount(timer);
                break;
            case EN_TIMER_SETCYCLE:
                result = __CPUTimer_SetCycle(timer,(u32)inoutpara);
                break;
            case EN_TIMER_SETRELOAD:
                result = __CPUTimer_SetAutoReload(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_ENINT:
                result = __CPUTimer_EnInt(timer);
                break;
            case EN_TIMER_DISINT:
                result = __CPUTimer_DisInt(timer);
                break;
            case EN_TIMER_SETINTPRO:
                result = __CPUTimer_SetIntPro(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_GETTIME:
                result = __CPUTimer_GetTime(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETCYCLE:
                result = __CPUTimer_GetCycle(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETID:
                result = __CPUTimer_GetID(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETSTATE:
                result = __CPUTimer_GetState(timer, (u32 *)inoutpara);
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
u32  __CPUTimer_GetFreq(ptu32_t timerhandle)
{
    return (CN_CFG_TIMER_CLK/128);
}
// =============================================================================
// ��������:module_init_timer
//          P1020��PICtimer��ʼ��,timerʱ��ԴΪAPB2 = CN_CFG_PCLK1 = 108M(timer234567)
// �������:
// �������:
// ����ֵ  :
// ˵��    :
// =============================================================================
bool_t ModuleInstall_HardTimer(void)
{
    struct TimerChip  CPUtimer;
    u8 i;
    u8 irqline;

    for(i=0;i<CN_CPUTIMER_NUM;i++)
    {
        tg_TIMER_Reg[i]->CONTROL = 0;           //��ֹTIMER
        tg_TIMER_Reg[i]->COUNTER = 0;           //��������0
        tg_TIMER_Reg[i]->COMPARE = 0xFFFFFFFF;
        tg_TIMER_Reg[i]->CLKSEL  = 7;           //ʱ��ԴΪ80M��128��Ƶ
    }

//    //��ԭ�ȷ��䶨ʱ�����ƶ�������
//     irqline = CN_INT_LINE_TIMER;     //���Ѿ�����õ���stgTimerHandle ���г�ʼ��
//    //���ö�ʱ���ж�,�Ƚ��������ж����еĹ����������
//      Int_Register(irqline);
//      Int_CutLine(irqline);
//
//      Int_IsrDisConnect(irqline);
//      Int_EvttDisConnect(irqline);
//      Int_SettoAsynSignal(irqline);
//
//      Int_SetClearType(irqline,CN_INT_CLEAR_USER);
//     // Int_SetIsrPara(irqline,(ptu32_t)timer);//��������timer �жϷ����Ǵ���timer ��Ϊ������в���,���ﲻ������
//    //���ڶ����ʱ������һ���ж�������������жϴ��ݵĲ�����Զ��ֻ�ǵ�һ������Ķ�ʱ�������ԭ��ͨ���ж�
//    //���ݽ����Ķ�ʱ������Զ�ʱ������صĲ����������ˣ��ʸò�����������.
//    //��Ϊ���ж�ʱ������һ���ж���,timer ��ʱ��������ԶΪ���һ��timerhandle���ò����ڵ��ø��������ᴫ��
//      Int_IsrConnect(irqline, __CPUTimer_isr);

    CPUtimer.chipname = "CPUTIMER";
    CPUtimer.HardTimerAlloc = __CPUTimer_Alloc;
    CPUtimer.HardTimerFree = __CPUTimer_Free;
    CPUtimer.HardTimerCtrl = __CPUTimer_Ctrl;
    CPUtimer.HardTimerGetFreq = __CPUTimer_GetFreq;
    HardTimer_RegisterChip(&CPUtimer);

    return true;
}
#endif

#ifdef CFG_CPU_ZQ12XX_CK
typedef struct _TIMER
{
    vu32 COUNTER;
    vu32 COMPARE;
    vu32 CONTROL;
    vu32 CLKSEL;
}tagTimerReg;

static tagTimerReg volatile * const tg_TIMER_Reg[] = {
                                    (tagTimerReg *)0x41860000,      //TIMER1
                                    (tagTimerReg *)0x41860010,      //TIMER2
                                    };

enum ENUM_CPU_TIMER
{
    EN_CPU_TIMER_1=0,
    EN_CPU_TIMER_2
};

//#define CN_TIMER_DIV     (CN_CFG_PCLK1/1000000)

//������ʱ��оƬ�Ķ�ʱ��Ӧ�����Լ��ľ��
struct CPUTimerHandle
{
    u32     timerno;          //��ʱ����
    u32     irqline;          //�жϺ�
    u32     cycle;            //��ʱ����
    u32     timerstate;       //��ʱ����ʶ
    bool_t  auto_reload;      //�Զ���װ��
    fnTimerIsr UserIsr;            //�û��ж���Ӧ����,���ڶ����ʱ������ һ���ж���,,�������������������Զֻ��һ��
};

#define CN_CPUTIMER_NUM   (EN_CPU_TIMER_2 +1)
#define CN_CPUTIMER_MAX    EN_CPU_TIMER_2

static struct CPUTimerHandle  stgTimerHandle[CN_CPUTIMER_NUM];

//���λ����timer0 ��������
static u32  gs_dwCPUTimerBitmap;  //���ڶ�ʱ�����ֶ�����һ��Ĳ���ܶ࣬32��Ӧ���㹻
#define CN_CPUTIMER_BITMAP_MSK  (0x80000000)  //���λΪ1���������Ƽ���

//timer0..timern��irq
static u8 spTimerIrqOffset[CN_CPUTIMER_NUM]={   0,1  };

//#define TIMER_IRQ_ENABLE_ADDR                (0x42030000+0x620)
//#define TIMER_IRQ_STA_ADDR                      (0x42030000+0x520)
#define TIMER_IRQ_ENABLE_ADDR                (0x42030000+0x220)
#define TIMER_IRQ_STA_ADDR                      (0x42030000+0x120)

//��ȡ32λ����һ��0bitλ��(�Ӹ�λ����λ����)
u8 __CPUTimer_GetFirstZeroBit(u32 para)
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
// ��������:__CPUTimer_PauseCount
//          ֹͣ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __CPUTimer_PauseCount(struct CPUTimerHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            return false;
        }
        else
        {
            tg_TIMER_Reg[timerno]->CONTROL = 0;         //ֹͣ�����������ж�
            tg_TIMER_Reg[timerno]->COUNTER = 0;         //�������
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
// ��������:__CPUTimer_StartCount
//          ��ʼ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
bool_t __CPUTimer_StartCount(struct CPUTimerHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            return false;
        }
        else
        {
            tg_TIMER_Reg[timerno]->COUNTER    = 0;
            tg_TIMER_Reg[timerno]->CONTROL &= ~(1 << 1);               //���жϱ�ʶ
            tg_TIMER_Reg[timerno]->CONTROL    = (1<<0)|(1<<2);      //timer and int
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
// ��������:__CPUTimer_SetCycle,CPU��timer,���Ϊ0xFFFFFFFF,����Timer_ModuleInit
//          �趨������ÿ��һ����Ϊ(1/108)1us����֪���ʱ�䲻��-1
//          �趨����
// �������:timer��PIC��ʱ��
//          cycle,���ڣ���ʱ��ʱ�Ӹ�������
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :�����������̫�󣨳������ʱ����������������Ϊ��ʱ�����������
// =============================================================================
bool_t  __CPUTimer_SetCycle(struct CPUTimerHandle  *timer, u32 cycle)
{
    u8 timerno;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            return false;
        }
        else
        {
            tg_TIMER_Reg[timerno]->COMPARE = cycle;
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
// ��������:__CPUTimer_SetAutoReload
//          �趨����
// �������:timer��PIC��ʱ��
//         autoreload, true���Լ�reload���ڣ������ֶ�ÿ����������
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// =============================================================================
bool_t  __CPUTimer_SetAutoReload(struct CPUTimerHandle  *timer, bool_t autoreload)
{
    timer->auto_reload = autoreload;
    return true;        //reload every time
}

//-----------------------------------------------------------------------------
// ���ܣ���ʱ���ж���Ӧ������������atmel m7��������Ҫ���в�����ʱ���ļĴ�������
//       ���жϵĶ�ʱ��������������ʵ��ISR��Ȼ���ӵ���user ISR��������
//       user isr�н������ʴӡ�
// ��������ʱ�������
// ���أ�user ISR�ķ���ֵ
//-----------------------------------------------------------------------------
__attribute__((weak)) u32 __CPUTimer_isr(ptu32_t TimerHandle)
{
    u32 timerno = (struct CPUTimerHandle  *)TimerHandle->timerno;

    //��������˳���ܸ�
    silan_timer_irq_reset(TIMER_ADDR(timerno));
    if(!(stgTimerHandle[timerno].auto_reload))
        silan_timer_disable(TIMER_ADDR(timerno));

    return ((struct CPUTimerHandle  *)TimerHandle)->UserIsr(TimerHandle);
}

// =============================================================================
// ��������:__CPUTimer_Alloc
//          ���䶨ʱ��
// �������:timerisr,��ʱ�����жϴ�����
// �������:
// ����ֵ  :����Ķ�ʱ�������NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
ptu32_t __CPUTimer_Alloc(fnTimerIsr timerisr)
{
    u8 timerno;
    u8 irqline;
    struct CPUTimerHandle  *timer;
    ptu32_t timerhandle;
    //ԭ�Ӳ�������ֹ��Դ����
    atom_low_t  timeratom;
    timeratom = Int_LowAtomStart();

    //Ѱ�ҿ��е�timer
    timerno = __CPUTimer_GetFirstZeroBit(gs_dwCPUTimerBitmap);
    if(timerno < CN_CPUTIMER_NUM)//���п��еģ������ñ�־λ
    {
        gs_dwCPUTimerBitmap = gs_dwCPUTimerBitmap | (CN_CPUTIMER_BITMAP_MSK>> timerno);
        Int_LowAtomEnd(timeratom);  //ԭ�Ӳ������
    }
    else//û�еĻ�ֱ�ӷ��ؾͿ����ˣ��ò����ن�����
    {
        Int_LowAtomEnd(timeratom);   //ԭ�Ӳ������
        return 0;
    }

    irqline = CN_INT_LINE_TIMER;
    timer = &stgTimerHandle[timerno];
    timer->cycle = 0;
    timer->timerno = timerno;
    timer->irqline = irqline;
    timer->timerstate = CN_TIMER_ENUSE;
    timer->UserIsr=timerisr;
    //���ˣ��жϺźͶ�ʱ�����붼���ˣ��ø���͸����ˡ�
    //�����úö�ʱ������
    __CPUTimer_PauseCount(timer);
    Int_SetIsrPara(irqline,(ptu32_t)timer);
    djybsp_isr_hdl_register(CN_INT_LINE_TIMER, CN_SUBID_TIMER_0 + timer->timerno, __CPUTimer_isr,0);
    timerhandle = (ptu32_t)timer;

    return timerhandle;
}


// =============================================================================
// ��������:__CPUTimer_Free
//          �ͷŶ�ʱ��
// �������:timer������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __CPUTimer_Free(ptu32_t timerhandle)
{
    u8 timerno;
    u8 irqline;
    atom_low_t  timeratom;  //����������Դ
    struct CPUTimerHandle  *timer;
    timer = (struct CPUTimerHandle  *)timerhandle;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        irqline = timer->irqline;
        if(timerno < CN_CPUTIMER_NUM)//���п��еģ������ñ�־λ
        {       //�޸�ȫ�ֱ�־һ����ԭ���Ե�
            timeratom = Int_LowAtomStart();
            gs_dwCPUTimerBitmap = gs_dwCPUTimerBitmap &(~(CN_CPUTIMER_BITMAP_MSK<< timerno));
            //������ж�������������
            timer->timerstate = 0;
            silan_timer_disable(TIMER_ADDR(timerno));
            silan_timer_count(TIMER_ADDR(timerno), 0);
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
// ��������:__CPUTimer_SetIntPro
//          ���ö�ʱ�����ж�����
// �������:timer���������Ķ�ʱ��
//          real_priorΪtrue��Ϊʵʱ�źŷ���Ϊ�첽�ź�
// �������:
// ����ֵ  :����Ķ�ʱ����NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
bool_t  __CPUTimer_SetIntPro(struct CPUTimerHandle  *timer, bool_t real_prior)
{      //������趨ʱ�����ù����ж��ߣ����ﲻ����
    return false;
}

// =============================================================================
// ��������:__CPUTimer_EnInt
//          ʹ�ܶ�ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __CPUTimer_EnInt(struct CPUTimerHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)| (CN_TIMER_ENINT);
        *(u32*)TIMER_IRQ_ENABLE_ADDR |= (1<<spTimerIrqOffset[timer->timerno]);
        silan_timer_irq_enable(TIMER_ADDR(timer->timerno));
        return true;
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:__CPUTimer_DisInt
//          ��ֹ��ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
bool_t  __CPUTimer_DisInt(struct CPUTimerHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        *(u32*)TIMER_IRQ_ENABLE_ADDR &= ~(1<<spTimerIrqOffset[timer->timerno]);
        timer->timerstate = (timer->timerstate)&(~CN_TIMER_ENINT);
        silan_timer_irq_disable(TIMER_ADDR(timer->timerno));
        return true;
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:__CPUTimer_GetTime
//          ��ȡ��ʱ����ʱ
// �������:timer���������Ķ�ʱ��
// �������:time����ʱ��΢�룩
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :���趨���������𣬼�cycle-ʣ��ʱ��,��ʾ�Ѿ��ߵ���ʱ��(��λ����ʱ����Ƶʱ�Ӹ���)
// =============================================================================
bool_t __CPUTimer_GetTime(struct CPUTimerHandle  *timer, u32 *time)
{
    u8 timerno;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            return false;
        }
        counter = tg_TIMER_Reg[timerno]->COUNTER;   //CNT����΢����
        *time = counter;
        return true;
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:__CPUTimer_CheckTimeout
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timer���������Ķ�ʱ��
// �������:timeout, true��ʱ��false����ʱ
// ����ֵ  :true�ɹ� falseʧ��
// ˵��    :
// =============================================================================
bool_t __CPUTimer_CheckTimeout(struct CPUTimerHandle  *timer, bool_t *timeout)
{
    bool_t result=true;
    u8 timerno;
    u32 temp;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            result = false;
        }
        else
        {
            temp = tg_TIMER_Reg[timerno]->COMPARE;
            counter = tg_TIMER_Reg[timerno]->COUNTER;
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
// �������ܣ�__CPUTimer_GetID
//          ��ȡ��ʱ��ID
// ���������timer����������ʱ����
// ���������timerId,��16λΪTIMERNO,��16Ϊ��Ӧ��IRQNO
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __CPUTimer_GetID(struct CPUTimerHandle   *timer,u32 *timerId)
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
// �������ܣ�__CPUTimer_GetCycle
//          ��ȡ��ʱ������
// ���������timer����������ʱ����
// ���������cycle����ʱ������(��λ����ʱ����Ƶʱ�Ӹ���)
// ����ֵ  ��true �ɹ� falseʧ��
// =============================================================================
bool_t __CPUTimer_GetCycle(struct CPUTimerHandle   *timer, u32 *cycle)
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
// �������ܣ�__CPUTimer_GetState
//          ��ȡ��ʱ��״̬
// ���������timer,�������Ķ�ʱ��
// ���������timerflag����ʱ��״̬
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
bool_t __CPUTimer_GetState(struct CPUTimerHandle   *timer, u32 *timerflag)
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
// ��������:__CPUTimer_Ctrl
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timerhandle �������Ķ�ʱ�����
//         ctrlcmd, ����������
// �������:inoutpara,����ctrlcmd�Ĳ�ͬ����ͬ
// ����ֵ  :true�ɹ�  falseʧ��
// ˵��    :
// =============================================================================
bool_t __CPUTimer_Ctrl(ptu32_t timerhandle, \
                         enum HardTimerCmdCode ctrlcmd, \
                         ptu32_t inoutpara)
{
    bool_t result = false;
    struct CPUTimerHandle  *timer;
    timer = (struct CPUTimerHandle  *)timerhandle;
    if(NULL != timer)
    {
        switch(ctrlcmd)
        {
            case EN_TIMER_STARTCOUNT:
                result = __CPUTimer_StartCount(timer);
                break;
            case EN_TIMER_PAUSECOUNT:
                result = __CPUTimer_PauseCount(timer);
                break;
            case EN_TIMER_SETCYCLE:
                result = __CPUTimer_SetCycle(timer,(u32)inoutpara);
                break;
            case EN_TIMER_SETRELOAD:
                result = __CPUTimer_SetAutoReload(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_ENINT:
                result = __CPUTimer_EnInt(timer);
                break;
            case EN_TIMER_DISINT:
                result = __CPUTimer_DisInt(timer);
                break;
            case EN_TIMER_SETINTPRO:
                result = __CPUTimer_SetIntPro(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_GETTIME:
                result = __CPUTimer_GetTime(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETCYCLE:
                result = __CPUTimer_GetCycle(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETID:
                result = __CPUTimer_GetID(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETSTATE:
                result = __CPUTimer_GetState(timer, (u32 *)inoutpara);
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
u32  __CPUTimer_GetFreq(ptu32_t timerhandle)
{
    return (CN_CFG_TIMER_CLK/128);
}
// =============================================================================
// ��������:module_init_timer
//          P1020��PICtimer��ʼ��,timerʱ��ԴΪAPB2 = CN_CFG_PCLK1 = 108M(timer234567)
// �������:
// �������:
// ����ֵ  :
// ˵��    :
// =============================================================================
bool_t ModuleInstall_HardTimer(void)
{
    struct TimerChip  CPUtimer;
    u8 i;
    u8 irqline;

    for(i=0;i<CN_CPUTIMER_NUM;i++)
    {
        tg_TIMER_Reg[i]->CONTROL = 0;           //��ֹTIMER
        tg_TIMER_Reg[i]->COUNTER = 0;           //��������0
        tg_TIMER_Reg[i]->COMPARE = 0xFFFFFFFF;
        tg_TIMER_Reg[i]->CLKSEL  = 7;           //ʱ��ԴΪ80M��128��Ƶ
    }

//    //��ԭ�ȷ��䶨ʱ�����ƶ�������
//     irqline = CN_INT_LINE_TIMER;       //���Ѿ�����õ���stgTimerHandle ���г�ʼ��
//    //���ö�ʱ���ж�,�Ƚ��������ж����еĹ����������
//      Int_Register(irqline);
//      Int_CutLine(irqline);
//
//      Int_IsrDisConnect(irqline);
//      Int_EvttDisConnect(irqline);
//      Int_SettoAsynSignal(irqline);
//
//      Int_SetClearType(irqline,CN_INT_CLEAR_USER);
     // Int_SetIsrPara(irqline,(ptu32_t)timer);//��������timer �жϷ����Ǵ���timer ��Ϊ������в���,���ﲻ������
    //���ڶ����ʱ������һ���ж�������������жϴ��ݵĲ�����Զ��ֻ�ǵ�һ������Ķ�ʱ�������ԭ��ͨ���ж�
    //���ݽ����Ķ�ʱ������Զ�ʱ������صĲ����������ˣ��ʸò�����������.
    //��Ϊ���ж�ʱ������һ���ж���,timer ��ʱ��������ԶΪ���һ��timerhandle���ò����ڵ��ø��������ᴫ��
    //  Int_IsrConnect(irqline, __CPUTimer_isr);

    CPUtimer.chipname = "CPUTIMER";
    CPUtimer.HardTimerAlloc = __CPUTimer_Alloc;
    CPUtimer.HardTimerFree = __CPUTimer_Free;
    CPUtimer.HardTimerCtrl = __CPUTimer_Ctrl;
    CPUtimer.HardTimerGetFreq = __CPUTimer_GetFreq;
    HardTimer_RegisterChip(&CPUtimer);

    return true;
}

#endif



