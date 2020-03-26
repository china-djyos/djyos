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
//���ߣ�djyos�Ŷ�
//�汾��V1.0.0
//�ļ�����: Ӳ�����Ź�ģ������
//����˵��:��ģ��Ϊdjyos��׼����ģ��
//�޶���ʷ:
// 1. ����: 2011-11-29
//   ����: djyos�Ŷ�
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "cpu_peri.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t WDT_SAMSUNGInit(u32 setcycle);
//    WDT_SAMSUNGInit(0);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip wdt"//CPU�Ŀ��Ź���������
//parent:"watch dog"       //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                      //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                   //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                     //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:pre-main                 //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                     //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"watch dog"   //������������������������none����ʾ�������������
                                     //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                     //����������������������г����á�,���ָ�
//weakdependence:"none"              //��������������������������none����ʾ�������������
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
//%$#@num,500000,20000000,
#define CFG_WDT_WDTCYCLE            (3*1000*1000)   //"���Ź���ʱʱ��"����λus
#define CFG_BOOT_TIME_LIMIT         30000000        //"�������س���ʱ��",�������������ع��̲���Ҫ���ô���
//%$#@enum,true,false,
#define CFG_DEFEND_ON_BOOT          false          //"������������",�������ع�������������������Ź�����λ
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

//#define CFG_WDT_WDTCYCLE  (3*1000*1000)     //��λΪ΢��=3S
#define CN_WDT_DOGNAME   "SAMSUNGKWDT"

// =============================================================================
//����: �������Ź�
//��������
//���أ���
// =============================================================================
void WDT_Start(void)
{
    //disable watchdog interrupt
    pg_wdt_reg->WTCON &= ~(3<<1);
    //enable Watchdog timer;reset signal.
    pg_wdt_reg->WTCON|=((1<<5)|(1<<0));
}

// =============================================================================
//����: �����λ��
//��������
//���أ���
//ע��: ͨ�����ø�λ����ʵ�֣������ĸ�λ����������������ʵ��
// =============================================================================
void WDT_Reboot(void)
{
    while(1);//��ʱ�򣬿��Ե��ø�λ��������������while(1)����
}

// =============================================================================
//����: ���Ź����У���Ҫ����Ӳ����λ
//��������
//���أ���
// =============================================================================
void WDT_HardReset(void)
{
    while(1);
    // 1�����Ӳ����������ܣ��������ó�����λ
    // 2���������ó��޸�timeout������λ��
}

// =============================================================================
//����: ���Ź�ι������
//��������
//���أ���
// =============================================================================
bool_t WDT_WdtFeed(void)
{
    printk("WDT FEED!\r\n");
    pg_wdt_reg->WTCNT = 15000;
    return true;
}

// =============================================================================
//����: ���Ź�Ӳ����ʼ��
//��������
//���أ���
//ע��: �����û��Ŀ��Ź���ʼ����Ȼ���djyos�ṩ����������
// =============================================================================
void WDT_HardInit(void)
{
    //Prescaler value=100;�clock division factor=128 ;PCLK=67.5MHz
    //t_watchdog=1/[PCLK/(Prescaler value+1)/Division_factor]=0.0002
    //disable watchdog
    pg_wdt_reg->WTCON=((100<<8)|(3<<3));
    //���Ź�ʱ������T=WTCNT*t_watchdog=3S
    //���Ź�ι��
    pg_wdt_reg->WTDAT = 15000;
    pg_wdt_reg->WTCNT = 15000;

    //���Ͽ��Ź���ʼ�����û����Ը��ݸ���ƽ̨���ã����棬����djyos����
    //����check_timeout����λΪus����ϵͳ��djy_timer_sync�Ĳ�����λһ��
//  wdt_set_check_timeout(3000*1000);
//    wdt_star();
}

//**************************************************//
//todo����������������ʵ�ֱ������ع��̣�ʹ�õ�ʱ���貹ȫ
u32 __FeedDog_Isr(ptu32_t intline)
{
    return false;
}
bool_t __BrdBoot_FeedStart(u32 bootfeedtime)
{
    return false;
}

bool_t __BrdBoot_FeedEnd(void)
{
    return false;
}
//**************************************************//

// =============================================================================
// ���ܣ����Ͽ��Ź�оƬ��ʼ�����˺��������Ź���������ʼ������������ˡ������������ع��̡�
//      �Ĺ��ܣ����������ú󣬽�ֹͣ�Զ�ι����
// �������:
// ����ֵ  :true�ɹ�falseʧ��
// =============================================================================
bool_t WDT_SAMSUNGInit(u32 setcycle)
{
    bool_t result;

    WDT_HardInit();
    //��ʼ��WDTģ��
//  result = WdtHal_RegisterWdtChip(CN_WDT_DOGNAME,CFG_WDT_WDTCYCLE,WDT_WdtFeed,NULL,NULL);
    result = WdtHal_RegisterWdtChip(CN_WDT_DOGNAME, CFG_WDT_WDTCYCLE, WDT_WdtFeed);
#if(CFG_DEFEND_ON_BOOT == true)
    __BrdBoot_FeedEnd();
#endif
    return result;
}

