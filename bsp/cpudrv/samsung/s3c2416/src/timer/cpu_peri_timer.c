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
//����ģ��: Ӳ����ʱ��
//���ߣ�lst
//�汾��V1.1.0
//�ļ�����: ����44b0Ӳ����ʱ������
//����˵��:
//�޶���ʷ:
//2. ����: 2009-04-24
//   ����: lst
//   �°汾��: V1.1.0
//   �޸�˵��: ԭdjyos.c�ж�ʱ������Ӳ����صĲ���ת�Ƶ����
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ��ֲ��44b0�汾
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "board-config.h"
#include "cpu_peri.h"
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
//attribute:bsp                      //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                   //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                     //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                    //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                     //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"Software Timers","int"//������������������������none����ʾ�������������
                                     //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                     //����������������������г����á�,���ָ�
//weakdependence:"none"              //��������������������������none����ʾ�������������
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

#define    TIMER_NUM    3   //3���û�Ӳ����ʱ��(��4��������ΪOS tick)
#define    TIMER_CLK_HZ 1000000

typedef struct{
    u8 idx;
    u8 intr_line;
    u16 state;
    u32 cycle;
    fnTimerIsr isr;
}tagTIMER_CTX;

static tagTIMER_CTX timer_ctx[TIMER_NUM];

//2440�Ķ�ʱ�����ܼ���:
//1����������ʽ����
//2��ÿ����ʱ����ѡ5��ʱ��Դ���ֱ�Ϊ��Ƶ��1/2��Ƶ~1/16��Ƶ����EXTCLK��
//3��������8λԤ��Ƶ����01����һ����234����һ����
//4��ÿ����ʱ�����бȽϼĴ��������ڲ���PWM���Ρ�
//5��ÿ����ʱ�������趨Ϊ�������к��ظ����У������ֶ�������ֹͣ��

static void Timer_Init(void)
{
    //DMA:No select(All Interrupt),
    pg_timer_reg->TCFG1 = 0;
}

//----���ö�ʱ��ʱ��Դ----------------------------------------------------------
//����: ���ø���ʱ��ʱ��Դ.
//����: timer,��ʱ����
//      value,��ʱ������ʱ��Դ,timer��value�Ķ�Ӧ������:
//      value:  0           1           2            3          4
//      timer0: 1/2��Ƶ     1/4��Ƶ     1/8��Ƶ      1/16��Ƶ   EXTCLK0
//      timer1: 1/2��Ƶ     1/4��Ƶ     1/8��Ƶ      1/16��Ƶ   EXTCLK0
//      timer2: 1/2��Ƶ     1/4��Ƶ     1/8��Ƶ      1/16��Ƶ   EXTCLK1
//      timer3: 1/2��Ƶ     1/4��Ƶ     1/8��Ƶ      1/16��Ƶ   EXTCLK1
//      timer4: 1/2��Ƶ     1/4��Ƶ     1/8��Ƶ      1/16��Ƶ   EXTCLK1
//����: ��
//-----------------------------------------------------------------------------
void Timer_SetClkSource(ufast_t timer,ufast_t value)
{
    pg_timer_reg->TCFG1 &= ~(0xf << (timer<<2));
    pg_timer_reg->TCFG1 |= value << (timer<<2);
}

//----���ö�ʱ��Ԥ��Ƶ��-------------------------------------------------------
//����: ���ø���ʱ��Ԥ��Ƶ��.
//����: group,��ʱ�����,01Ϊ1��,234Ϊ1��
//      value,��ʱ��Ԥ��Ƶ��,0~255��Ӧ1~256��Ƶ
//����: ��
//-----------------------------------------------------------------------------
void Timer_SetPrecale(ufast_t group,uint16_t value)
{
    pg_timer_reg->TCFG0 &= ~(0xff << (group<<3));
    pg_timer_reg->TCFG0 |= value << (group<<3);
}

//----���ö�ʱ������ֵ----------------------------------------------------------
//����: ���ø���ʱ������ֵ.��ʱ��������ж�ʱ��Ϊ:value*(Ԥ��Ƶ+1)/ʱ��Դ��Ƶ��
//����: timer,��ʱ����
//      value,����ֵ
//����: ��
//-----------------------------------------------------------------------------
void Timer_SetCounter(ufast_t timer,uint16_t value)
{
    switch (timer)
    {
        case 0:
            pg_timer_reg->TCNTB0 = value;
            break;
        case 1:
            pg_timer_reg->TCNTB1 = value;
            break;
        case 2:
            pg_timer_reg->TCNTB2 = value;
            break;
        case 3:
            pg_timer_reg->TCNTB3 = value;
            break;
        case 4:
            pg_timer_reg->TCNTB4 = value;
            break;
        default:break;
    }
}

u16 Timer_GetReload(ufast_t timer)
{
    switch (timer)
    {
        case 0:
            return (u16)(pg_timer_reg->TCNTB0);
        case 1:
            return (u16)(pg_timer_reg->TCNTB1);
        case 2:
            return (u16)(pg_timer_reg->TCNTB2);
        case 3:
            return (u16)(pg_timer_reg->TCNTB3);
        case 4:
            return (u16)(pg_timer_reg->TCNTB4);
        default:break;
    }
    return 0;
}
//----���ö�ʱ���Ƚ�ֵ----------------------------------------------------------
//����: ����pwmռ�ձ����
//����: timer,��ʱ����
//      value,�Ƚ�ֵ
//����: ��
//-----------------------------------------------------------------------------
void Timer_SetCompare(ufast_t timer,uint16_t value)
{
    switch (timer)
    {
        case 0:
            pg_timer_reg->TCMPB0 = value;
            break;
        case 1:
            pg_timer_reg->TCMPB1 = value;
            break;
        case 2:
            pg_timer_reg->TCMPB2 = value;
            break;
        case 3:
            pg_timer_reg->TCMPB3 = value;
            break;
        default:break;
    }
}

//----���ö�ʱ��������ʽ-------------------------------------------------------
//����: �趨��ʱ���������������ǵ��ι���
//����: timer,��ʱ����
//      type,0=���ι���,1=�Զ�������������
//����: ��
//-----------------------------------------------------------------------------
bool_t Timer_SetType(ufast_t timer,ufast_t type)
{
    type &= 0x1;
    switch (timer)
    {
        case 0:
            pg_timer_reg->TCON &= ~(1<<3);
            pg_timer_reg->TCON |= type<<3;
            return true;
        case 1:
            pg_timer_reg->TCON &= ~(1<<11);
            pg_timer_reg->TCON |= type<<11;
            return true;
        case 2:
            pg_timer_reg->TCON &= ~(1<<15);
            pg_timer_reg->TCON |= type<<15;
            return true;
        case 3:
            pg_timer_reg->TCON &= ~(1<<19);
            pg_timer_reg->TCON |= type<<19;
            return true;
        case 4:
            pg_timer_reg->TCON &= ~(1<<22);
            pg_timer_reg->TCON |= type<<22;
            return true;
        default:
            return false;
    }
}

//----���ض�ʱ�������ͱȽ�ֵ---------------------------------------------------
//����: �ֶ����¼��ض�ʱ���ļ����ͱȽϼĴ���ֵ
//����: timer,��ʱ����
//����: ��
//-----------------------------------------------------------------------------
void Timer_Reload(ufast_t timer)
{
    switch (timer)
    {
        case 0:
            pg_timer_reg->TCON |= 1<<1;
            pg_timer_reg->TCON &= ~(1<<1);
            break;
        case 1:
            pg_timer_reg->TCON |= 1<<9;
            pg_timer_reg->TCON &= ~(1<<9);
            break;
        case 2:
            pg_timer_reg->TCON |= 1<<13;
            pg_timer_reg->TCON &= ~(1<<13);
            break;
        case 3:
            pg_timer_reg->TCON |= 1<<17;
            pg_timer_reg->TCON &= ~(1<<17);
            break;
        case 4:
            pg_timer_reg->TCON |= 1<<21;
            pg_timer_reg->TCON &= ~(1<<21);
            break;
        default:break;
    }
}

//----������ʱ��--------------------------------------------------------
//����: ������ʱ��
//����: timer,��ʱ����
//����: ��
//-----------------------------------------------------------------------------
bool_t Timer_Start(ufast_t timer)
{
    switch (timer)
    {
        case 0:
            pg_timer_reg->TCON |=1;
            return true;
        case 1:
            pg_timer_reg->TCON |=1<<8;
            return true;
        case 2:
            pg_timer_reg->TCON |=1<<12;
            return true;
        case 3:
            pg_timer_reg->TCON |=1<<16;
            return true;
        case 4:
            pg_timer_reg->TCON |=1<<20;
            return true;
        default:
            return false;
    }
}

//----ֹͣ��ʱ��--------------------------------------------------------
//����: ֹͣ��ʱ��
//����: timer,��ʱ����
//����: ��
//-----------------------------------------------------------------------------
bool_t Timer_Stop(ufast_t timer)
{
    switch (timer)
    {
        case 0:
            pg_timer_reg->TCON &=~1;
            return true;
        case 1:
            pg_timer_reg->TCON &=~(1<<8);
            return true;
        case 2:
            pg_timer_reg->TCON &=~(1<<12);
            return true;
        case 3:
            pg_timer_reg->TCON &=~(1<<16);
            return true;
        case 4:
            pg_timer_reg->TCON &=~(1<<20);
            return true;
        default:
            return false;
    }
}

//----����ʱ����ǰֵ--------------------------------------------------------
//����: ������ʱ���ĵ�ǰ����ֵ
//����: timer,��ʱ����
//����: ��ǰ����ֵ
//-----------------------------------------------------------------------------
uint16_t Timer_Read(ufast_t timer)
{
    switch (timer)
    {
        case 0:
            return pg_timer_reg->TCNTO0;
            break;
        case 1:
            return pg_timer_reg->TCNTO1;
            break;
        case 2:
            return pg_timer_reg->TCNTO2;
            break;
        case 3:
            return pg_timer_reg->TCNTO3;
            break;
        case 4:
            return pg_timer_reg->TCNTO4;
            break;
        default:break;
    }
    return 0;
}

// =============================================================================
// ��������:__timer_alloc
//          ���䶨ʱ��
// �������:timerisr,��ʱ�����жϴ�����
// �������:
// ����ֵ  :����Ķ�ʱ�������NULL����䲻�ɹ�
// ˵��    :
// =============================================================================
static ptu32_t __timer_alloc(fnTimerIsr timerisr)
{
    s32 i;
    tagTIMER_CTX *tmr_ctx=NULL;
    atom_low_t atom;

    for(i=0;i<TIMER_NUM;i++)
    {
        atom = Int_LowAtomStart();
        if(!(timer_ctx[i].state&CN_TIMER_ENUSE))
        {
            timer_ctx[i].state |= CN_TIMER_ENUSE;
            break;
        }
        Int_LowAtomEnd(atom);
    }

    if(i  < TIMER_NUM)
    {
        tmr_ctx = &timer_ctx[i];
        tmr_ctx->cycle  = 0;
        tmr_ctx->isr    = timerisr;
     Int_Register(tmr_ctx->intr_line);
        Int_CutLine(tmr_ctx->intr_line);
        Int_IsrConnect(tmr_ctx->intr_line,timerisr);
        Int_SettoAsynSignal(tmr_ctx->intr_line);

        Timer_SetType(i,1);
//      Timer_SetCounter(i,TIMER_CLK_HZ/cycle);
        Timer_Reload(i);
    }

    return (ptu32_t)tmr_ctx;
}

// =============================================================================
// ��������:__timer_free
//          �ͷŶ�ʱ��
// �������:timer������Ķ�ʱ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵��    :
// =============================================================================
static bool_t  __timer_free(ptu32_t timerhandle)
{
    tagTIMER_CTX *tmr_ctx;
    atom_low_t atom;

    tmr_ctx =(tagTIMER_CTX*)timerhandle;

    if(tmr_ctx!=NULL)
    {
        atom = Int_LowAtomStart();

        tmr_ctx->state &= ~CN_TIMER_ENUSE;
        Timer_Stop(tmr_ctx->idx);
        Int_CutLine(tmr_ctx->intr_line);
        Int_IsrDisConnect(tmr_ctx->intr_line);
        Int_EvttDisConnect(tmr_ctx->intr_line);
        Int_UnRegister(tmr_ctx->intr_line);
        Int_LowAtomEnd(atom);
        return true;
    }
    return false;

}

static bool_t Timer_ENCOUNT(tagTIMER_CTX *tmr,bool_t en)
{
    if(en)
    {
        tmr->state |= CN_TIMER_ENCOUNT;
        Timer_Start(tmr->idx);
    }
    else
    {
        tmr->state &= ~CN_TIMER_ENCOUNT;
        Timer_Stop(tmr->idx);
    }
    return true;
}

static    bool_t Timer_SetCycle(tagTIMER_CTX *tmr,u32 cycle)
{
    u32 count;

    tmr->cycle =cycle;
    count =TIMER_CLK_HZ/cycle;
    if(count<=0xFFFF)
    {
        Timer_SetCounter(tmr->idx,count);
        return true;
    }
    return false;
}

static bool_t Timer_SetReload(tagTIMER_CTX *tmr,bool_t en)
{
    if(en)
    {
        tmr->state |= CN_TIMER_RELOAD;
        __Timer_SetType(tmr->idx,1);
    }
    else
    {
        tmr->state &= ~CN_TIMER_RELOAD;
        __Timer_SetType(tmr->idx,0);
    }

    return true;
}

static bool_t Timer_ENINT(tagTIMER_CTX *tmr)
{
    tmr->state |= CN_TIMER_ENINT;
    Int_ContactLine(tmr->intr_line);
    return true;
}

static bool_t Timer_DISINT(tagTIMER_CTX *tmr)
{
    tmr->state &= ~CN_TIMER_ENINT;
    Int_CutLine(tmr->intr_line);
    return true;
}

static bool_t  Timer_SETINTPRO(tagTIMER_CTX *timer, bool_t en)
{
    if(timer->state & CN_TIMER_ENUSE)
    {
        if(en)
        {
            timer->state |= CN_TIMER_REALINT;
            Int_EvttDisConnect(timer->intr_line);
            return Int_SettoReal(timer->intr_line);
        }
        else
        {
            timer->state &= ~CN_TIMER_REALINT;
            return Int_SettoAsynSignal(timer->intr_line);
        }
    }

    return false;

}
// =============================================================================
// ��������:__timer_ctrl
//          ����ʱ���Ķ�ʱʱ���Ƿ��Ѿ�����
// �������:timerhandle �������Ķ�ʱ�����
//         ctrlcmd, ����������
// �������:inoutpara,����ctrlcmd�Ĳ�ͬ����ͬ
// ����ֵ  :true�ɹ�  falseʧ��
// ˵��    :
// =============================================================================
static bool_t __timer_ctrl(ptu32_t timerhandle, enum HardTimerCmdCode ctrlcmd,ptu32_t inoutpara)
{

    bool_t result = false;
    tagTIMER_CTX  *timer;
    timer = (tagTIMER_CTX  *)timerhandle;

    if(NULL != timer)
    {
        switch(ctrlcmd)
        {
            case EN_TIMER_STARTCOUNT:
                result = Timer_ENCOUNT(timer,true);
                break;

            case EN_TIMER_PAUSECOUNT:
                result = Timer_ENCOUNT(timer,false);
                break;

            case EN_TIMER_SETCYCLE:
                result = Timer_SetCycle(timer,(u32)inoutpara);
                break;

            case EN_TIMER_SETRELOAD:
                result = Timer_SetReload(timer, *(u32*)inoutpara);
                break;

            case EN_TIMER_ENINT:
                result = Timer_ENINT(timer);
                break;

            case EN_TIMER_DISINT:
                result = Timer_DISINT(timer);
                break;

            case EN_TIMER_SETINTPRO:
                result = Timer_SETINTPRO(timer, *(u32*)inoutpara);
                break;

            case EN_TIMER_GETTIME:
                if((u32*)inoutpara!=NULL)
                {
                    *(u32*)inoutpara = Timer_Read(timer->idx);
                    return true;
                }
                return false;

            case EN_TIMER_GETCYCLE:
            {
                if((u32*)inoutpara!=NULL)
                {
                    *(u32*)inoutpara =timer->cycle;
                    return true;
                }
                return false;
            }

            case EN_TIMER_GETID:
            {
                u32 id;
                if((u32*)inoutpara!=NULL)
                {
                    id =timer->intr_line<<16|timer->idx;
                    *(u32*)inoutpara =id;
                    return true;
                }
                return false;
            }

            case EN_TIMER_GETSTATE:
            {
                if((u32*)inoutpara!=NULL)
                {
                    *(u32*)inoutpara =timer->state;
                    return true;
                }
                return false;
            }
            default:
                break;
        };
    }

    return result;
}

// =============================================================================
// ��������:__timer_get_freq
//       ��ȡ��ʱ����Ƶ
// �������:timerhandle �������Ķ�ʱ�����
// �������:
// ����ֵ  :��ʱ����Ƶ
// ˵��    :��λ��HZ��
// =============================================================================
static u32  __timer_get_freq(ptu32_t timerhandle)
{
    return TIMER_CLK_HZ;
}

// =============================================================================
// ��������:ModuleInstall_HardTimer
//
// �������:
// �������:
// ����ֵ  :
// ˵��    :
// =============================================================================
bool_t ModuleInstall_HardTimer(void)
{
    struct TimerChip  s3c_timer;
    s32 i,pre_div;

    pre_div =(CN_CFG_PCLK/2)/TIMER_CLK_HZ;

    Timer_Init();
    Timer_SetPrecale(0,pre_div); //Timer0,1.
    Timer_SetPrecale(1,pre_div); //Timer2,3,4.

    for(i=0;i<TIMER_NUM;i++)
    {
        timer_ctx[i].idx =i;
        timer_ctx[i].intr_line =i+CN_INT_LINE_TIMER0;
        timer_ctx[i].state =0;
        timer_ctx[i].cycle =100;
        timer_ctx[i].isr =NULL;
        Timer_SetClkSource(i,0); //ClkDiv = 1/2
    }

    s3c_timer.chipname = "S3C_TIMER";
    s3c_timer.HardTimerAlloc   = __timer_alloc;
    s3c_timer.HardTimerFree    = __timer_free;
    s3c_timer.HardTimerCtrl    = __timer_ctrl;
    s3c_timer.HardTimerGetFreq = __timer_get_freq;

    return HardTimer_RegisterChip(&s3c_timer);
}
