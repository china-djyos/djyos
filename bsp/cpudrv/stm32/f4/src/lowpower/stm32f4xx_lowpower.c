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
//����ģ��: CPU��ص͹��Ĺ���
//����:  ������.
//�汾��V1.0.0
//�ļ�����: cpu��صĵ͹��Ĺ���,ʵ��lowpower.h�ļ��й涨bsp��Ҫ�ṩ�ĺ���,����
//          ��ʵ��CPU��صĺ���, �������ص�,�ɵ�boarddrvĿ¼��ȥ��
//          ʹ����stm32��42�����ݼĴ��������һ��.
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

//stm32�ϵĵ͹���Level����(�ο�lowpower.h�ļ��еĶ���)
//L0:��ִ��wfeָ��,����������.��L1һ��,������Ǵ�������˯��ģʽ.
//L1:wfeָ��,�ں�ִ�еĲ�����L0һ��,���û��Ļص���������ͣ������ʱ��
//L2:������stm32��ֹͣģʽ,����SRAM�ͼĴ���ֵ,�˳����������
//L3/L4:����stm32�Ĵ���ģʽ,SRAM�ͼĴ���ֵ����ʧֻ�б��ݼĴ���ά�ֹ���.��������
//�˳���,L3����ʱҪ����SRAM�ͼĴ���,�˳�ʱҪ�ָ������˳�������ִ��.

#include "stdint.h"
#include "stddef.h"
#include "lowpower.h"
#include "cpu_peri.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu_peri_lowpower"      //�͹��������������
//parent:"lowpower"                       //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                          //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                       //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                         //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                         //��ʼ��ʱ������ѡֵ��early��medium��later��
                                         //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lowpower"                  //������������������������none����ʾ�������������
                                         //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                         //����������������������г����á�,���ָ�
//weakdependence:"none"                  //��������������������������none����ʾ�������������
                                         //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                         //����������������������г����á�,���ָ�
//mutex:"none"                           //������������������������none����ʾ�������������
                                         //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header              //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
//%$#@end configue  ****�������ý���
//@#$%component end configure


#define Stm32SleepModel4    0x3456789a

//----��ʼ���͹���Ӳ��--------------------------------------------------------
//����: ��ʼ���͹��Ĺ���Ӳ��,�������Ҫ��ʼ��,����ֱ��return true.��Ϊstm32�͹���
//      ������Ҫʹ�ñ������������, ����Ҫ�򿪱���ģ���Դ��ʹ�ܱ���������.
//����: ��
//����: ��
//----------------------------------------------------------------------------
bool_t __LP_BSP_HardInit(void)
{
    HAL_PWR_DeInit();               //PWRģ��ʹ��
    HAL_PWR_EnableBkUpAccess();//����ʹ��
    return true;
}

//----��ȡ���߼���------------------------------------------------------------
//����: ����ϵͳ�����߻��Ѳ�����,���ñ��������Ի�û���ǰ������״̬,����ֵ��
//      ��lowpower.h�ļ��ж����CN_SLEEP_L3��CN_SLEEP_L4��CN_SLEEP_NORMAL֮һ,
//      CN_SLEEP_NORMAL��ʾ�ϵ����Ӳ��λ,��������ָ�λ.ע��,����ֵ������
//      CN_SLEEP_L0~2֮һ.
//����: ��
//����: CN_SLEEP_L3��CN_SLEEP_L4��CN_SLEEP_NORMAL֮һ
//----------------------------------------------------------------------------
u32 __LP_BSP_GetSleepLevel(void)
{

    u32 bkt_DR;
    if(__HAL_PWR_GET_FLAG(PWR_FLAG_WU+PWR_FLAG_SB)& PWR_FLAG_WU)
    {
        bkt_DR = RTC->BKP0R;//todo
//      bkt_DR = Stm32SleepModel4;
        if(bkt_DR == Stm32SleepModel4)
            return CN_SLEEP_L4;
        else
            return CN_SLEEP_L3;
    }
    else
        return CN_SLEEP_NORMAL;
}

//----�������߼���-------------------------------------------------------------
//����: �Ѽ�����������߼��𱣴浽��Ӧ��Ӳ����,ע��,ֻ��L3��L4��Ҫ.
//����: SleepLevel, ��������ĵ͹��ļ���
//����true=�ɹ�,false=ʧ��
//-----------------------------------------------------------------------------
bool_t __LP_BSP_SaveSleepLevel(u32 SleepLevel)
{
    if((SleepLevel!= CN_SLEEP_L3) && (SleepLevel!= CN_SLEEP_L4))
        return false;
    RTC->BKP0R=SleepLevel;//todo
    return true;
}

//----����L0���͹���-----------------------------------------------------------
//����: ����L0���͹���״̬,������lowpower.h������,��lowpower.c�ļ�����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void __LP_BSP_EntrySleepL0(void)
{

    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFE);
}

//----����L1���͹���-----------------------------------------------------------
//����: ����L1���͹���״̬,������lowpower.h������,��lowpower.c�ļ�����.��stm32��
//      L0��L1��ʵ��һ����,���û��Ļص��������ܲ�һ��.
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void __LP_BSP_EntrySleepL1(void)
{

    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFE);
}

//----����L2���͹���-----------------------------------------------------------
//����: ����L2���͹���״̬,������lowpower.h������,��lowpower.c�ļ�����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void __LP_BSP_EntrySleepL2(void)
{
    //��ֹ�ж�

    //�������ⲿ�жϱ�־��RTC���ӱ�־
    EXTI->PR = 0xFFFFF;
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFE);

}

//----����L3���͹���-----------------------------------------------------------
//����: ����L3���͹���״̬,������lowpower.h������,��lowpower.c�ļ�����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void __LP_BSP_EntrySleepL3(void)
{

    HAL_PWR_EnterSTANDBYMode();
}

//----����L4���͹���-----------------------------------------------------------
//����: ����L4���͹���״̬,������lowpower.h������,��lowpower.c�ļ�����,stm32��,
//      L3��L4��cpudrv����,��һ�µ�.
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void __LP_BSP_EntrySleepL4(void)
{
    HAL_PWR_EnterSTANDBYMode( );
}
