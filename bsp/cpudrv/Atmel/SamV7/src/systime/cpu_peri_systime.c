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

// �ļ���     ��cpu_peri_systime.c
// ģ������: ϵͳ�ϵ�ʱ��ģ�飬��һ��RTT��32λ��ʱ����ʱϵͳ�ϵ�ʱ��
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 09/15.2015
// =============================================================================

#include "cpu_peri.h"
#include "systime.h"
#include "board-config.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t MoudleInit_Systime(ptu32_t para);
//    MoudleInit_Systime(0);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip systime"//�ṩ�߾��ȵ�ϵͳʱ��
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                         //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                      //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                        //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                       //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                        //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"time"  //������������������������none����ʾ�������������
                                        //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//weakdependence:"none"                 //��������������������������none����ʾ�������������
                                        //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                        //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_SYSTIME == false )
//#warning  " cpu_onchip_systime  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header                   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_SYSTIME    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,TC0,TC1,TC2,TC3,
#define CFG_SYSTIME_TIMER       (TC2)           //"��ʱ��",����systimeʹ�õĶ�ʱ��
//%$#@enum,0,1,2,
#define CFG_SYSTIME_TIMER_CH    (0)             //"��ʱ��ͨ��",����systimeʹ�õĶ�ʱ��ͨ��,��Ӳ��ƥ��
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

//#define CFG_SYSTIME_TIMER     (TC2)           //ʹ��16λ��ʱ��TC2��ͨ��0��Ϊsystime
//#define CFG_SYSTIME_TIMER_CH  (0)

#define SYSTIME_TC      CFG_SYSTIME_TIMER
#define SYSTIME_TC_CH   CFG_SYSTIME_TIMER_CH

#define SYSTIME_FRE     (CN_CFG_MCLK/ (128*2))  //��1.171875MHz
#define SYSTIME_CYCLE   (65536) //����Ϊ��󣬼�0xFFFF,���T= SYSTIME_FRE * 0xFFFF
// =============================================================================
// ���ܣ���ȡ16λ��ʱ����ǰֵ
// ��������
// ���أ�16λ�Ķ�ʱ��ʱ��ֵ
// =============================================================================
u32 Systime_GetTime(void)
{
    return SYSTIME_TC->TC_CHANNEL[SYSTIME_TC_CH].TC_CV;
}

// =============================================================================
// ���ܣ�ϵͳʱ�ӳ�ʼ����ʹ��16λ��ʱ�Ӽ�������Ϊϵͳʱ��
// ��������
// ���أ���
// =============================================================================
static void SysTime_TcConfig(void)
{
    //hemin modified 20160613
    //TC2ʱ��Դ�ĳ�TC_CMR_TCCLKS_TIMER_CLOCK4����MCK/128 = 150M/128

    PMC_EnablePeripheral(ID_TC6);//ID_TC6 - ID_TC8 ��ӦTC2��ͨ��0-2

    //����TC��Ӧͨ���Ĳ�������ֹ�жϣ����ñȽ���������
    SYSTIME_TC->TC_CHANNEL[SYSTIME_TC_CH].TC_CCR = TC_CCR_CLKDIS;
    SYSTIME_TC->TC_CHANNEL[SYSTIME_TC_CH].TC_IDR = 0xFFFFFFFF;
    SYSTIME_TC->TC_CHANNEL[SYSTIME_TC_CH].TC_SR;
    SYSTIME_TC->TC_CHANNEL[SYSTIME_TC_CH].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4;

    //ʹ�ܶ�ʱ��
    SYSTIME_TC->TC_CHANNEL[SYSTIME_TC_CH].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}
// =============================================================================
// ���ܣ�ϵͳʱ�ӳ�ʼ����ע��ϵͳʱ�ӣ�����ʼ��RTT�����ж�
// ������para
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t MoudleInit_Systime(ptu32_t para)
{
#if (!CN_USE_TICKLESS_MODE)
    //��ʼ��TC������
    SysTime_TcConfig();

    //ע��ϵͳʱ�Ӻ���
    DJY_SysTimeConnect(Systime_GetTime,NULL,SYSTIME_FRE,SYSTIME_CYCLE);
#endif
    return true;
}
