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
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// =============================================================================

// �ļ���     ��cpu_peri_wdt.c
// ģ������: scm610оƬ�Դ��Ŀ��Ź���ʼ��
// ����ʱ��: 14/2.2019
// ������    : huzb
// =============================================================================
#include <stdlib.h>
#include <stdint.h>
#include "cpu_peri.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_WWDG(u32 time);
//    ModuleInstall_WWDG(0);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu peri wdt"//CPU�Ĵ��ڿ��Ź�����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                  //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                 //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium               //��ʼ��ʱ������ѡֵ��early��medium��later��
                                 //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"component wdt"   //������������������������none����ʾ�������������
                                 //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                 //����������������������г����á�,���ָ�
//weakdependence:"none"          //��������������������������none����ʾ�������������
                                 //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                 //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                 //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header              //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

typedef struct
{
    volatile u32 CTRL;
    volatile u32 CLR;
}tagWatReg;

#define CN_WAT_REG_BASE (0x40700000)

static volatile tagWatReg* sgpt_WatReg = (volatile tagWatReg*)CN_WAT_REG_BASE;

#define CN_WDT_WDTCYCLE   1000*mS//              //��λΪ΢��
static u32 sgWdtFeedCycle = 1000*mS;

#define CN_WWDG_WDTCYCLE  32000*mS               //WWDG���Ϊ58mS

#define CN_WDT_DOGNAME   "SCMWDG"


// =============================================================================
// ��������:ʹ�ܿ��Ź�
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
bool_t Wdt_Enable(void)
{
    sgpt_WatReg->CTRL |= (1<<0);

    return true;
}

// =============================================================================
// ��������:IWDG_Stm32WdtFeed
//          IWDG_Stm32WdtFeed��ι������
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
bool_t Wdt_Feed(void)
{
    //����Passadд��5A �ڽ��������
    sgpt_WatReg->CTRL &= ~(0xff << 8);
    sgpt_WatReg->CTRL |= (0x5A << 8);
    //����ι������
    sgpt_WatReg->CLR = 0xa5;

    return true;
}

// =============================================================================
// ��������:IWDG_PreSet,IWDGʱ��ԴΪ�ڲ�RC��Ϊ40KHz
//          IWDG_PreSet����ʱ�ӷֲ�ϵ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
bool_t Wdt_PreSet(u8 Value)
{
    return true;
}

// =============================================================================
// ��������:   WatDog_ReloadSet
//          ���ÿ��Ź������λ����
// �������:   value
//          оƬ����֧�����¼�������:
//          SYSWDT_OTSET_3906us
//          SYSWDT_OTSET_7812us
//          SYSWDT_OTSET_15625us
//          SYSWDT_OTSET_31250us
//          SYSWDT_OTSET_62500us
//          SYSWDT_OTSET_125ms
//          SYSWDT_OTSET_250ms
//          SYSWDT_OTSET_500ms
//          SYSWDT_OTSET_1s
//          SYSWDT_OTSET_2s
//          SYSWDT_OTSET_4s
//          SYSWDT_OTSET_8s
//          SYSWDT_OTSET_12s
//          SYSWDT_OTSET_16s
//          SYSWDT_OTSET_24s
//          SYSWDT_OTSET_32s
//
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================

bool_t Wdt_ReloadSet(u16 value)
{
    sgpt_WatReg->CTRL &= ~(0xf << 4);
    sgpt_WatReg->CTRL |= value;

}

// =============================================================================
// ��������:WatDog_Initial
//          WatDog_Initial���Ź�ע��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// ˵����
// =============================================================================

bool_t Wdt_Initial(u32 setcycle)
{
    bool_t result;

    //��λʹ��
    sgpt_WatReg->CTRL |= (1<<1);

    //������װ��ֵ����ι����װ��ֵ,����Ϊ5��Ƶ
    Wdt_ReloadSet(SYSWDT_OTSET_1s);
    result = WdtHal_RegisterWdtChip(CN_WDT_DOGNAME,CN_WDT_WDTCYCLE,\
                                    Wdt_Feed,NULL,NULL);

    Wdt_Enable();
    return result;
}