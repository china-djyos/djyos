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
// ģ������: TIMER������
// ģ��汾: V1.00
// ������Ա: czz
// ����ʱ��:
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
#include "board-config.h"
#include "cpu_peri_reg.h"
#include "driver/include/arm_arch.h"
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
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
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

enum ENUM__TIMER
{
    EN_TIMER_0=0,
    EN_TIMER_1,
    EN_TIMER_2,
    EN_TIMER_3,
    EN_TIMER_4,
    EN_TIMER_5,
    EN_TIMER_MAX
};

//������ʱ��оƬ�Ķ�ʱ��Ӧ�����Լ��ľ��
struct TimerCB
{
    u32     timerno;          //��ʱ����
    u32     cycle;            //��ʱ����
    u32     timerstate;       //��ʱ����ʶ
    fnTimerIsr UserIsr;            //�û��ж���Ӧ����
};
static struct TimerCB stgTimerHandle[EN_TIMER_MAX];

//���λ����timer0 ��������
static u32  gs_dwTimerBitmap;  //���ڶ�ʱ�����ֶ�����һ��Ĳ���ܶ࣬32��Ӧ���㹻
#define CN_TIMER_BITMAP_MSK  (0x80000000)  //���λΪ1���������Ƽ���


//��ȡ32λ����һ��0bitλ��(�Ӹ�λ����λ����)
static u8 Timer_GetFirstZeroBit(u32 para)
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
// ��������:Timer_StartCount
//          ��ʼ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
static bool_t Timer_StartCount(struct TimerCB  *timer)
{
    u32 timerno;
    timerno = timer->timerno;
    if(!(timer->timerstate & CN_TIMER_ENUSE))
        return false;
    if(timerno > EN_TIMER_MAX)
        return false;

    if(timerno < 3)
    {
        REG32_WRITE_REG(REG_TIMERCTLA_PERIOD_ADDR(timerno),timer->cycle);
        REG32_SET_BIT(TIMER0_2_CTL, (1 << timerno ));
    }
    else
    {
        REG32_WRITE_REG(REG_TIMERCTLB_PERIOD_ADDR(timerno),timer->cycle);
        REG32_SET_BIT(TIMER3_5_CTL, (1 << (timerno - 3)));
    }
    //�ƺ���timer��bug��+����ģʽʱ���þ���һ��CNT�������Ƶ�0�Ժ�reload����
    //����Ч���ʳ�ֵ��Ϊ 0xffffffff
    timer->timerstate = (timer->timerstate)| (CN_TIMER_ENCOUNT);

    return true;
}

// =============================================================================
// ��������:Timer_SetCycle,��timer,���Ϊ0xFFFFFFFF,����Timer_ModuleInit
//          �趨������ÿ��һ����Ϊ(1/108)1us����֪���ʱ�䲻��-1
//          �趨����
// �������:timer��PIC��ʱ��
//          cycle,���ڣ���ʱ��ʱ�Ӹ�������
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :�����������̫�󣨳������ʱ����������������Ϊ��ʱ�����������
// =============================================================================
static bool_t  Timer_SetCycle(struct TimerCB  *timer, u32 cycle)
{
    u8 timerno;
    if(!(timer->timerstate & CN_TIMER_ENUSE))
        return false;
    timerno = timer->timerno;
    if(timerno >= EN_TIMER_MAX)
        return false;
    timer->cycle = cycle;
    return true;
}
// =============================================================================
// ��������:Timer_SetAutoReload
//          �趨����
// �������:timer��PIC��ʱ��
//         autoreload, true���Լ�reload���ڣ������ֶ�ÿ����������
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// =============================================================================
static bool_t  Timer_SetAutoReload(struct TimerCB  *timer, bool_t autoreload)
{
    //�����澯
    (void) timer;
    (void) autoreload;

    return false;
}

//-----------------------------------------------------------------------------
// ���ܣ���ʱ���ж���Ӧ������������atmel m7��������Ҫ���в�����ʱ���ļĴ�������
//       ���жϵĶ�ʱ��������������ʵ��ISR��Ȼ���ӵ���user ISR��������
//       user isr�н������ʴӡ�
// ��������ʱ�������
// ���أ�user ISR�ķ���ֵ
//-----------------------------------------------------------------------------
static  u32 Header_TimerIsr(ptu32_t TimerHandle)
{

    int i;
    UINT32 status;
    (void) TimerHandle;

    status = REG_READ(TIMER0_2_CTL);
    for(i = 0; i < 3; i++)
    {
        if(status & (1 << (i + TIMERCTLA_INT_POSI)))
        {
            if(stgTimerHandle[i].UserIsr!=NULL)
            {
                stgTimerHandle[i].UserIsr((ptu32_t)&stgTimerHandle[i]);
            }
        }
    }
    do
    {
        REG_WRITE(TIMER0_2_CTL, status);
    }
    while(REG_READ(TIMER0_2_CTL) & status & (0x7 << TIMERCTLA_INT_POSI));

    status = REG_READ(TIMER3_5_CTL);
    for(i = 0; i < 3; i++)
    {
        if(status & (1 << (i + TIMERCTLB_INT_POSI)))
        {

            if(stgTimerHandle[i+3].UserIsr!=NULL)
            {
                stgTimerHandle[i+3].UserIsr((ptu32_t)&stgTimerHandle[i+3]);
            }
        }
    }
    do
    {
        REG_WRITE(TIMER3_5_CTL, status);
    }
    while(REG_READ(TIMER3_5_CTL) & status & (0x7 << TIMERCTLB_INT_POSI));

    Int_ClearLine(CN_INT_LINE_TIMER);
    return 0;
}

// =============================================================================
// ��������:__Timer_Free
//          �ͷŶ�ʱ��
// �������:timer������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
static bool_t  __Timer_Free(ptu32_t timerhandle)
{
    u32 timerno;
    atom_low_t  timeratom;  //����������Դ
    struct TimerCB  *timer;
    timer = (struct TimerCB  *)timerhandle;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno < EN_TIMER_MAX)//���п��еģ������ñ�־λ
        {       //�޸�ȫ�ֱ�־һ����ԭ���Ե�
            timeratom = Int_LowAtomStart();
            gs_dwTimerBitmap = gs_dwTimerBitmap &(~(CN_TIMER_BITMAP_MSK>> timerno));
            //������ж�������������
            timer->timerstate = 0;

            if(timerno < 3)
            {
                REG32_CLEAR_BIT(TIMER0_2_CTL,(1 << timerno ));
            }
            else
            {
                REG32_CLEAR_BIT(TIMER3_5_CTL,(timerno - 3));
            }
            Int_LowAtomEnd(timeratom);  //ԭ�Ӳ������
            return true;
        }
    }
    return false;
}


// =============================================================================
// ��������:Timer_SetIntPro
//          ���ö�ʱ�����ж�����
// �������:timer���������Ķ�ʱ��
//          real_priorΪtrue��Ϊʵʱ�źŷ���Ϊ�첽�ź�
// �������:
// ����ֵ  :����Ķ�ʱ����NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
static bool_t  Timer_SetIntPro(struct TimerCB  *timer, bool_t real_prior)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        if(real_prior)
        {
            return false;
//            timer->timerstate = (timer->timerstate)| (CN_TIMER_REALINT);
//            Int_EvttDisConnect(CN_INT_LINE_TIMER);
//            return Int_SettoReal(CN_INT_LINE_TIMER);
        }
        else
        {
            timer->timerstate = (timer->timerstate)&(~CN_TIMER_REALINT);
            return Int_SettoAsynSignal(CN_INT_LINE_TIMER);
        }
    }
    else
    {
        return false;
    }
}

// =============================================================================
// ��������:Timer_EnInt
//          ʹ�ܶ�ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
static bool_t  Timer_EnInt(struct TimerCB  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)| (CN_TIMER_ENINT);
        return Int_EnableLine(CN_INT_LINE_TIMER);
    }
    return false;
}
// =============================================================================
// ��������:Timer_DisInt
//          ��ֹ��ʱ���ж�
// �������:timer���������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
static bool_t  Timer_DisInt(struct TimerCB  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)&(~CN_TIMER_ENINT);
        return true;
//                Int_DisableLine(timer->irqline);
    }
    else
    {
        return false;
    }
}
// =============================================================================
// ��������:Timer_GetTime
//          ��ȡ��ʱ����ʱ
// �������:timer���������Ķ�ʱ��
// �������:time����ʱ��΢�룩
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :���趨���������𣬼�cycle-ʣ��ʱ��,��ʾ�Ѿ��ߵ���ʱ��(��λ����ʱ����Ƶʱ�Ӹ���)
// =============================================================================
static bool_t Timer_GetTime(struct TimerCB  *timer, u32 *time)
{
    u32 timerno = timer->timerno;
    if(timerno >= EN_TIMER_MAX)
        return false;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        if(timerno<3)
        {
            *time = REG32_READ_REG(REG_TIMERCTLA_PERIOD_ADDR(timerno));
        }
        else
        {
            *time = REG32_READ_REG(REG_TIMERCTLB_PERIOD_ADDR(timerno));
        }
        return true;
    }
    return false;
}

// =============================================================================
// ��������:ȡ��ʱ��Ӳ�����ƼĴ�����ַ
// �������:timer���������Ķ�ʱ��
// ����ֵ  :true�ɹ� falseʧ��
// ˵��    :
// =============================================================================
static bool_t Timer_GetReg(struct TimerCB  *timer, void **reg)
{
    (void) timer;
    (void) reg;
    return false;
}
// =============================================================================
// �������ܣ�Timer_GetID
//          ��ȡ��ʱ��ID
// ���������timer����������ʱ����
// ���������timerId,��16λΪTIMERNO,��16Ϊ��Ӧ��IRQNO
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
static bool_t Timer_GetID(struct TimerCB   *timer,u32 *timerId)
{
    (void) timer;
    (void)timerId;
    return false;
}

// =============================================================================
// �������ܣ�Timer_GetCycle
//          ��ȡ��ʱ������
// ���������timer����������ʱ����
// ���������cycle����ʱ������(��λ����ʱ����Ƶʱ�Ӹ���)
// ����ֵ  ��true �ɹ� falseʧ��
// =============================================================================
static bool_t Timer_GetCycle(struct TimerCB   *timer, u32 *cycle)
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
// �������ܣ�Timer_GetState
//          ��ȡ��ʱ��״̬
// ���������timer,�������Ķ�ʱ��
// ���������timerflag����ʱ��״̬
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    : ����ʵ��
// =============================================================================
static bool_t Timer_GetState(struct TimerCB   *timer, u32 *timerflag)
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
// ��������:Timer_PauseCount
//          ֹͣ����
// �������:timer��PIC��ʱ��
// �������:
// ����ֵ  :true�ɹ� fasleʧ��
// ˵��    :
// =============================================================================
static bool_t Timer_PauseCount(struct TimerCB  *timer)
{
    u32 timerno;
    if(!(timer->timerstate & CN_TIMER_ENUSE))
        return false;

    timerno = timer->timerno;
    if(timerno > EN_TIMER_MAX)
    {
        return false;
    }

    if(timerno < 3)
    {
        REG32_CLEAR_BIT(TIMER0_2_CTL,(1<<timerno));
    }
    else
    {
        REG32_CLEAR_BIT(TIMER0_2_CTL,(1<<(timerno-3)));
    }
    timer->timerstate = (timer->timerstate)&(~CN_TIMER_ENCOUNT);
    return true;

}
// =============================================================================
// ��������:__Timer_Ctrl
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timerhandle �������Ķ�ʱ�����
//         ctrlcmd, ����������
// �������:inoutpara,����ctrlcmd�Ĳ�ͬ����ͬ
// ����ֵ  :true�ɹ�  falseʧ��
// ˵��    :
// =============================================================================
static bool_t __Timer_Ctrl(ptu32_t timerhandle, \
                         enum HardTimerCmdCode ctrlcmd, \
                         ptu32_t inoutpara)
{
    bool_t result = false;
    struct TimerCB  *timer;
    timer = (struct TimerCB  *)timerhandle;
    if(NULL != timer)
    {
        switch(ctrlcmd)
        {
            case EN_TIMER_STARTCOUNT:
                result = Timer_StartCount(timer);
                break;
            case EN_TIMER_PAUSECOUNT:
                result = Timer_PauseCount(timer);
                break;
            case EN_TIMER_SETCYCLE:
                result = Timer_SetCycle(timer,(u32)inoutpara);
                break;
            case EN_TIMER_SETRELOAD:
                result = Timer_SetAutoReload(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_ENINT:
                result = Timer_EnInt(timer);
                break;
            case EN_TIMER_DISINT:
                result = Timer_DisInt(timer);
                break;
            case EN_TIMER_SETINTPRO:
                result = Timer_SetIntPro(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_GETTIME:
                result = Timer_GetTime(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETCYCLE:
                result = Timer_GetCycle(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETID:
                result = Timer_GetID(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETSTATE:
                result = Timer_GetState(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETREG:
                result = Timer_GetReg(timer, (void **)inoutpara);
                break;
            default:
                break;
        };
    }

    return result;
}
// =============================================================================
// ��������:P1020Pic__Timer_GetFreq
//       ��ȡ��ʱ����Ƶ
// �������:timerhandle �������Ķ�ʱ�����
// �������:
// ����ֵ  :��ʱ����Ƶ
// ˵��    :��λ��HZ��
// =============================================================================
static u32  __Timer_GetFreq(ptu32_t timerhandle)
{

    struct TimerCB  *timer = (struct TimerCB*)timerhandle;
    if(timer->timerno<3)
    {
        return 26*1000*1000;

    }
    if(timer->timerno < 6)
    {
        return 32*1000;
    }
    return 0;
}



// =============================================================================
// ��������:__Timer_Alloc
//          ���䶨ʱ��
// �������:timerisr,��ʱ�����жϴ�����
// �������:
// ����ֵ  :����Ķ�ʱ�������NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
static ptu32_t __Timer_Alloc(fnTimerIsr timerisr)
{
    u32 timerno;
    struct TimerCB  *timer;
    //ԭ�Ӳ�������ֹ��Դ����
    atom_low_t  timeratom;
    timeratom = Int_LowAtomStart();

    //Ѱ�ҿ��е�timer
    timerno = Timer_GetFirstZeroBit(gs_dwTimerBitmap);
    if(timerno < EN_TIMER_MAX)//���п��еģ������ñ�־λ
    {
        gs_dwTimerBitmap = gs_dwTimerBitmap | (CN_TIMER_BITMAP_MSK<< timerno);
        Int_LowAtomEnd(timeratom);  //ԭ�Ӳ������
    }
    else//û�еĻ�ֱ�ӷ��ؾͿ����ˣ��ò����ن�����
    {
        Int_LowAtomEnd(timeratom);   //ԭ�Ӳ������
        return 0;
    }

    timer = &stgTimerHandle[timerno];
    timer->cycle = 0;
    timer->timerno = timerno;
    timer->timerstate = CN_TIMER_ENUSE;
    timer->UserIsr=timerisr;
    Timer_PauseCount(timer);

    return (ptu32_t)timer;
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
    struct TimerChip  timer;

    for(u8 i=0;i<sizeof(stgTimerHandle)/sizeof(struct TimerCB);i++)
    {
        stgTimerHandle[i].UserIsr    = NULL;
        stgTimerHandle[i].timerstate = 0;
        stgTimerHandle[i].cycle      = 0;
        stgTimerHandle[i].timerno    = i;
    }

    //���ò���Ƶ ʱ�ӣ�timer0~2 26M   time3~5 32K
    REG32_CLEAR_BIT(TIMER0_2_CTL,TIMERCTLA_CLKDIV_MASK << TIMERCTLA_CLKDIV_POSI);
    REG32_CLEAR_BIT(TIMER3_5_CTL,TIMERCTLA_CLKDIV_MASK << TIMERCTLA_CLKDIV_POSI);

    //value = (PWD_TIMER_26M_CLK);
    REG32_CLEAR_BIT(ICU_PERI_CLK_PWD,(3<<20));//ʱ��ʹ��

    timer.chipname = "TIMER";
    timer.HardTimerAlloc = __Timer_Alloc;
    timer.HardTimerFree = __Timer_Free;
    timer.HardTimerCtrl = __Timer_Ctrl;
    timer.HardTimerGetFreq = __Timer_GetFreq;
    HardTimer_RegisterChip(&timer);

    Int_EvttDisConnect(CN_INT_LINE_TIMER);
    Int_SettoAsynSignal(CN_INT_LINE_TIMER);
    Int_SetClearType(CN_INT_LINE_TIMER,CN_INT_CLEAR_USER);
    Int_IsrConnect(CN_INT_LINE_TIMER, Header_TimerIsr);
    return true;
}





