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

// �ļ���     ��cpu_peri_wdt.c
// ģ������: WDTģ�飬ʹ����WDT��������Ź�
// ģ��汾: V1.10
// ������Ա: hm
// ����ʱ��: 28/08.2014
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "systime.h"
#include "cpu_peri.h"
#include <int.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    #if CFG_IWDG_ENABLE==1
//    extern bool_t IWDG_Stm32Initial(u32 setcycle);
//    IWDG_Stm32Initial(CFG_WDT_WDTCYCLE);
//    #endif
//
//    #if CFG_WWDG_ENABLE==1
//    extern bool_t WWDG_STM32Init(u32 setcycle);
//    WWDG_STM32Init(CFG_WWDG_WDTCYCLE);
//    #endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip wdt"//CPU�Ŀ��Ź���������
//parent:"watch dog"       //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:pre-main            //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"watch dog"        //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_WDT == false )
//#warning  " cpu_onchip_wdt  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_WDT    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,,
#define CFG_IWDT_WDTCYCLE       5000000      //"IWDT���Ź���ʱʱ��",
#define CFG_WWDG_WDTCYCLE       50000        //"WWDG���Ź���ʱʱ��",
//%$#@enum,TIM6,TIM7,
#define CFG_WDT_TIM             TIM6    //���������õ��Ķ�ʱ�������CFG_DEFEND_ON_BOOTûѡ����ѡ�������õģ�
//%$#@enum,true,false,
#define CFG_WWDG_ENABLE         false            //"�Ƿ�����ʹ��WWDG",
#define CFG_IWDG_ENABLE         false            //"�Ƿ�����ʹ��IWDG",
#define CFG_DEFEND_ON_BOOT      false            //"������������",�������ع�������������������Ź�����λ�����CFG_WWDG_ENABLE��CFG_IWDG_ENABLE��ûѡ����ѡ�������õģ�
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�"s
//%$#@free,s
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

//#define CFG_WDT_WDTCYCLE  5000*mS//200*1000      //��λΪ΢��
//#define CFG_WWDG_WDTCYCLE  50*mS                 //WWDG���Ϊ58mS
#define CN_WDT_DOGNAME   "STM32IWDG"
#define CN_WWDG_DOGNAME   "STM32WWDG"

#define CN_IWDG_BASE  0x40003000
#define CN_WWDG_BASE  0x40002C00
#define STM_IWDG      ((tagIWDGReg*)CN_IWDG_BASE)
#define STM_WWDG      ((tagWWDGReg*)CN_WWDG_BASE)

#define CN_PRE_VALUE   5
static u16 s_prevalue[]={4,8,16,32,64,128,256,256};


bool_t __BrdBoot_FeedEnd(void);
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ------------------------ STM32 �������Ź� -----------------------------------
// ----------------------------------------------------------------------------
// =============================================================================
// ��������:ʹ�ܿ��Ź�
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
bool_t IWDG_Stm32Enable(void)
{
    STM_IWDG->IWDG_KR = 0xCCCC;
    return true;
}

// =============================================================================
// ��������:IWDG_Stm32WdtFeed
//          IWDG_Stm32WdtFeed��ι������
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
bool_t IWDG_Stm32WdtFeed(void)
{
    STM_IWDG->IWDG_KR = 0xAAAA;
    return true;
}
// =============================================================================
// ��������:IWDG_PreSet,IWDGʱ��ԴΪ�ڲ�RC��Ϊ40KHz
//          IWDG_PreSet����ʱ�ӷֲ�ϵ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
bool_t IWDG_Stm32PreSet(u8 Value)
{
    //����IWDGģʽ
    if(!(STM_IWDG->IWDG_SR & IWDG_SR_PVU_MASK))
    {
        STM_IWDG->IWDG_KR = 0x5555;
        STM_IWDG->IWDG_PR = IWDG_PR_PR & Value;
        //wait finish
        while((STM_IWDG->IWDG_SR & IWDG_SR_PVU_MASK));
        return true;
    }
    else
        return false;
}

// =============================================================================
// ��������:IWDG_Stm32ReloadSet,������װ��ֵ
//          IWDG_Stm32ReloadSetӲ�����Ź���ʼ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
bool_t IWDG_Stm32ReloadSet(u16 value)
{
    if(!(STM_IWDG->IWDG_SR & IWDG_SR_RVU_MASK))
    {
        STM_IWDG->IWDG_KR = 0x5555;
        STM_IWDG->IWDG_RLR = IWDG_RLR_RL & value;
        //wait finish
        while((STM_IWDG->IWDG_SR & IWDG_SR_RVU_MASK));
        return true;
    }
    else
        return false;
}

// =============================================================================
// ���ܣ����Ͽ��Ź�оƬ��ʼ�����˺��������Ź���������ʼ������������ˡ������������ع��̡�
//      �Ĺ��ܣ����������ú󣬽�ֹͣ�Զ�ι����
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵����IWDG�����ã�������PR��RL��STM_IWDG->IWDG_SR��Ӧλ����Ϊ1
// =============================================================================
bool_t IWDG_Stm32Initial(void)
{
    bool_t result;
    pg_rcc_reg->CSR |= (1<<0);//turn on LSI
    while(!(pg_rcc_reg->CSR & (1<<1)));

    IWDG_Stm32PreSet(CN_PRE_VALUE);     //����ʱ��Ԥ��Ƶ��ʱ��ԴΪ40KHZ
    //������װ��ֵ����ι����װ��ֵ,����Ϊ5��Ƶ
    IWDG_Stm32ReloadSet((40/s_prevalue[CN_PRE_VALUE]) * (CFG_IWDT_WDTCYCLE/mS));

//  result = WdtHal_RegisterWdtChip(CN_WDT_DOGNAME,CFG_IWDT_WDTCYCLE,\
//                                  IWDG_Stm32WdtFeed,NULL,NULL);

    result = WdtHal_RegisterWdtChip(CN_WDT_DOGNAME, CFG_IWDT_WDTCYCLE, IWDG_Stm32WdtFeed);
#if(CFG_DEFEND_ON_BOOT == true)
    __BrdBoot_FeedEnd();
#endif
    IWDG_Stm32Enable();
    return result;
}

bool_t WWDG_STM32WdtFeed(void)
{
    STM_WWDG->WWDG_CR |= 0x7F;
    return true;
}

//PCLK1=36Mʱ�ӣ�CK��ʱ��ʱ��(PCLK1����4096)����8��Ϊ1.09����WWDG������ÿ
//1.09mS����һ�Σ�WWDG���Ź����ι��ʱ��Ϊ58mS
bool_t WWDG_STM32Init(void)
{
    bool_t result;
    pg_rcc_reg->APB1ENR |= (1<<11);//enable wwdg
    STM_WWDG->WWDG_CR &= ~(1<<7);//WDGA
    STM_WWDG->WWDG_CFR |= (3<<7);//WDGTB0,1
    WWDG_STM32WdtFeed();

//  result = WdtHal_RegisterWdtChip(CN_WWDG_DOGNAME,CFG_WWDG_WDTCYCLE,\
//                          WWDG_STM32WdtFeed,NULL,NULL);
    result = WdtHal_RegisterWdtChip(CN_WWDG_DOGNAME, CFG_WWDG_WDTCYCLE, WWDG_STM32WdtFeed);
#if(CFG_DEFEND_ON_BOOT == true)
    __BrdBoot_FeedEnd();
#endif
    STM_WWDG->WWDG_CR |= (1<<7);//ʹ�ܿ��Ź�
    return result;
}

bool_t BrdWdt_FeedDog(void)
{
#if(CFG_IWDG_ENABLE == true)
    IWDG_Stm32WdtFeed();
#endif

#if(CFG_WWDG_ENABLE == true)
    WWDG_STM32WdtFeed();
#endif

    return (TRUE); // �պ���
}
#if(CFG_DEFEND_ON_BOOT == true)

u32 __FeedDog_Isr(ptu32_t intline)
{
    CFG_WDT_TIM->CNT = 0;
    CFG_WDT_TIM->SR = 0;//���жϱ�־
    BrdWdt_FeedDog();
    Int_ClearLine(intline);
    return 1;
}
bool_t __BrdBoot_FeedStart(u32 bootfeedtime)
{

    u8 irqline = CN_INT_LINE_TIM6;

#if(CFG_WWDG_ENABLE == true)
    pg_rcc_reg->APB1ENR |= (1<<11);//enable wwdg
    STM_WWDG->WWDG_CR &= ~(1<<7);//WDGA
    STM_WWDG->WWDG_CFR |= (3<<7);//WDGTB0,1
    WWDG_STM32WdtFeed();
    STM_WWDG->WWDG_CR |= (1<<7);//ʹ�ܿ��Ź�
#endif

#if(CFG_IWDG_ENABLE == true)
    pg_rcc_reg->CSR |= (1<<0);//turn on LSI
    while(!(pg_rcc_reg->CSR & (1<<1)));
    IWDG_Stm32PreSet(CN_PRE_VALUE);     //����ʱ��Ԥ��Ƶ��ʱ��ԴΪ40KHZ
    //������װ��ֵ����ι����װ��ֵ,����Ϊ5��Ƶ
    IWDG_Stm32ReloadSet((40/s_prevalue[CN_PRE_VALUE]) * (CFG_IWDT_WDTCYCLE/mS));

    IWDG_Stm32Enable();
#endif

    pg_rcc_reg->APB1ENR |= (1<<4)|(1<<5);//ʱ��ʹ��,timer6/7��ʹ��λΪ����4/5
    CFG_WDT_TIM->CR1 &= ~(TIM_CR1_CEN); //��ֹTIMER
    CFG_WDT_TIM->CR1 |= TIM_CR1_ARPE | TIM_CR1_DIR;//�Զ���װ
    CFG_WDT_TIM->DIER |= TIM_DIER_UIE;//ʹ�ܸ����ж�
    CFG_WDT_TIM->PSC = 4000-1;//��Ƶϵ�� Ϊ�� ����Ƶ(1/108)1uS
    CFG_WDT_TIM->ARR = 27000;//��ʱ��Ԥװ��ֵ
    Int_Register(irqline);
    Int_IsrConnect(irqline,__FeedDog_Isr);
    Int_SettoReal(irqline);
    Int_ClearLine(irqline);
    Int_RestoreRealLine(irqline);
    CFG_WDT_TIM->CR1 |= (TIM_CR1_CEN); //ʹ��TIMER
    return true;
}

bool_t __BrdBoot_FeedEnd(void)
{
    u8 irqline = CN_INT_LINE_TIM6;
    BrdWdt_FeedDog();
    CFG_WDT_TIM->CR1 &=~(TIM_CR1_CEN); //��ֹTIMER
    Int_SaveRealLine(irqline);
    Int_IsrDisConnect(irqline);
    Int_UnRegister(irqline);
//    RCC->APB1ENR &=(~RCC_APB1ENR_TIM12EN);
    pg_rcc_reg->APB1ENR &= (~((1<<4)|(1<<5)));//ʱ��ʹ��,timer6/7��ʹ��λΪ����4/5
    return true;
}
#endif
