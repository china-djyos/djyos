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

#include "stdint.h"
#include "stddef.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "wdt_soft.h"
#include <wdt_pub.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern void ModuleInstall_BrdWdt(void);
//    ModuleInstall_BrdWdt();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip iwdt"//�����
//parent:"watch dog"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:pre-main               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"watch dog"        //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_IWDT == false )
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_IWDT    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100000000,
#define CFG_WDT_FEED_CYCLE          12000000       //"���Ź���ʱʱ��"����λus
#define CFG_BOOT_TIME_LIMIT         30000000       //"�������س���ʱ��",�������������ع��̲���Ҫ���ô���
//%$#@enum,true,false,
#define CFG_DEFEND_ON_BOOT          false          //"������������",�������ع�������������������Ź�����λ��BK7251��֧�֣�ֻ��ѡfalse
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


// BOARD WDT
// =============================================================================

static u32 sBootDogFeedTime = 0;
// =============================================================================
// ���ܣ����Ͽ��Ź�ι������,max706��ι��ʱ����1.6s,�����ֲᣬι���Ǳ��ش���
// ��������
// ���أ���
// =============================================================================
bool_t BrdWdt_FeedDog(void)
{
//    printf("--hard feed dog\r\n");        //lst
    if(sddev_control(WDT_DEV_NAME, WCMD_RELOAD_PERIOD, 0) == 0)
        return true;
    else
        return false;
}

//----����boot�ڼ�ι��----------------------------------------------------------
//���ܣ�����boot�ڼ�ι�����ܣ�һ����˵��������һ����ʱ�����ж�������CFG_WDT_FEED_CYCLE
//      ��80%���ж���Ϊʵʱ�жϣ�����ISR�У�ִ��ι��������CFG_BOOT_TIME_LIMIT ʱ��֮��
//      ֹͣι����
//��������
//���أ�true
//-----------------------------------------------------------------------------
bool_t BrdBoot_FeedStart( void)
{
//    sBootDogFeedTime = bootfeedtime;
//    sddev_control(WDT_DEV_NAME, WCMD_POWER_UP, 0);      //�����Ź�
//    sddev_control(WDT_DEV_NAME, WCMD_SET_PERIOD, &sBootDogFeedTime);    //���ÿ��Ź�������ʱ��
    return true;
}

//----ֹͣ����boot�ڼ�ι��-------------------------------------------------------
//���ܣ���Ӳ�����Ź�ģ�鰲װ��ϵͳ��������ι������������ʼ���ɶ�ʱ��ISRι����������
//��������
//���أ�true
//-----------------------------------------------------------------------------
bool_t __BrdWdt_FeedDog(void)
{
    BrdWdt_FeedDog();
    return true;
}

// =============================================================================
// ���ܣ����Ͽ��Ź�оƬ��ʼ�����˺��������Ź���������ʼ������������ˡ������������ع��̡�
//      �Ĺ��ܣ����������ú󣬽�ֹͣ�Զ�ι����
// ��������
// ���أ���
// =============================================================================
bool_t ModuleInstall_BrdWdt(void)
{
    bool_t result;
    sBootDogFeedTime = CFG_WDT_FEED_CYCLE * 10 / 6;
    sddev_control(WDT_DEV_NAME, WCMD_POWER_UP, 0);      //�����Ź�
    sddev_control(WDT_DEV_NAME, WCMD_SET_PERIOD, &sBootDogFeedTime);    //���ÿ��Ź�������ʱ��
    result = WdtHal_RegisterWdtChip("Wdt_BK7251", CFG_WDT_FEED_CYCLE/4, BrdWdt_FeedDog);
#if(CFG_DEFEND_ON_BOOT == true)
    __BrdWdt_FeedDog();
#endif
    return result;
}


