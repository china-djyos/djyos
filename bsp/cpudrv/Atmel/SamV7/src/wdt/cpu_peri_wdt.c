//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��cpu_peri_wdt.c
// ģ������: ���Ź�ģ��
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 09/15.2015
// =============================================================================

#include "stdint.h"
#include "cpu_peri.h"
#include "wdt_hal.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    ModuleInstall_CpuWdt(0);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu_peri_wdt"     //CPU�Ŀ��Ź���������
//parent:"wdt"                      //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���                 //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                       //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                    //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:init                        //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                    //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
//dependence:"wdt"                  //������������������������none����ʾ�������������
                                    //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                    //����������������������г����á�,���ָ�
//weakdependence:"none"             //��������������������������none����ʾ�������������
                                    //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                    //����������������������г����á�,���ָ�
//mutex:"none"                      //������������������������none����ʾ�������������
                                    //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_CPUWDT_WDTCYCLE   //****�������Ƿ��Ѿ����ú�
#warning    cpu_peri_wdt�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,1000,20000000,
#define CFG_CPUWDT_WDTCYCLE             (5*1000*1000)   //"���Ź�ʱ��",����CPU���迴�Ź�����ʱ��
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

#define CN_WDT_DOGNAME   "SamV71HardWdt"

void WDT_Disable(void)
{
    WDT->WDT_MR |= WDT_MR_WDDIS;
}

// =============================================================================
// ��������:�趨WDT��ι�����ڣ���������û��ι�����򹷽�
// �������:cycle,���ڣ�΢��
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
void WDT_SetCycle(u32 cycle)
{
    u8 wdt_clk;

    wdt_clk = cycle*256/1000000;    //WDTʱ�� = 32768��128 = 256Hz

    WDT_Disable();
    WDT->WDT_MR = WDT_MR_WDRSTEN | WDT_MR_WDDBGHLT | WDT_MR_WDIDLEHLT |
            WDT_MR_WDV(wdt_clk) | WDT_MR_WDD(wdt_clk);
}

// =============================================================================
// ��������:WDT_WdtFeed
//          ι������
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
bool_t WDT_WdtFeed(void)
{
    atom_high_t atom;

    //UNLOCKʱ����뱣֤�������
    atom = Int_HighAtomStart();
    WDT->WDT_CR = WDT_CR_KEY_PASSWD | WDT_CR_WDRSTT;
    Int_HighAtomEnd(atom);
    return true;
}

// =============================================================================
// ��������:WDT_SamInit
//          SamV71���Ź�ע��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
bool_t ModuleInstall_CpuWdt(u32 setcycle)
{
    bool_t result;

    //��ʼ��WDTģ��
    WDT_SetCycle(CFG_CPUWDT_WDTCYCLE);

    result = WdtHal_RegisterWdtChip(CN_WDT_DOGNAME,CFG_CPUWDT_WDTCYCLE,\
                                    WDT_WdtFeed,NULL,NULL);

    WDT_WdtFeed();
    return result;
}
