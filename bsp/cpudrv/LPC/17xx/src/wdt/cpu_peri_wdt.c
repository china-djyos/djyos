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
// ģ������: WDTģ�飬ʹ����WDT����������Ź�
// ģ��汾: V1.10
// ������Ա: hm
// ����ʱ��: 28/08.2014
// =============================================================================
#include "stdint.h"
#include "int.h"
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
//component name:"cpu_peri_wdt" //CPU�Ŀ��Ź���������
//parent:"wdt"                  //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"wdt"              //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header               //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_CPUWDT_WDTCYCLE         //****�������Ƿ��Ѿ����ú�
#warning    cpu_peri_wdt�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,500000,10000000,
#define CFG_CPUWDT_WDTCYCLE     (5*1000*1000)           //"���Ź���ʱ��",
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

//#define CFG_CPUWDT_WDTCYCLE  5000*mS//200*1000      //��λΪ΢��
#define CN_WDT_DOGNAME   "LPC17xxWDT"

#define tagWdtReg        LPC_WDT
// =============================================================================
// ��������:WDT_LpcWdtFeed
//          WDT_LpcWdtFeed��ι������
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
bool_t WDT_LpcWdtFeed(void)
{
    atom_low_t atom;

    //��֤���Ź�ι�������ж�
    atom = Int_LowAtomStart();
    tagWdtReg->WDFEED = 0xAA;
    tagWdtReg->WDFEED = 0x55;
    Int_LowAtomEnd(atom);

    return true;
}

// =============================================================================
// ��������:WDT_HardWdtInit
//          WDT_HardWdtInitӲ�����Ź���ʼ��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
bool_t WDT_HardWdtInit(void)
{
    //����WDTģʽ
    tagWdtReg->WDMOD = 0;//�����λ
    tagWdtReg->WDTC = 0xBEBC20;//���ö�ʱʱ��2s = 0xBEBC20 * 4 /25M
    tagWdtReg->WDMOD |= (1<<0);//ʹ�ܿ�ʼ��

    return true;
}

// =============================================================================
// ��������:WDT_LpcWdtFeed
//          WDT_LpcWdtFeed���Ź�ע��
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
bool_t WDT_LpcInitial(u32 setcycle)
{
    bool_t result;

    result = WdtHal_RegisterWdtChip(CN_WDT_DOGNAME,CFG_CPUWDT_WDTCYCLE,\
                                    WDT_LpcWdtFeed,NULL,NULL);

    //LPC17XX��WDTģ����ʹ���˿��Ź���һ��Ҫιһ�ι�����ʱ���Ż���
//  WDT_LpcWdtFeed();
    return result;
}