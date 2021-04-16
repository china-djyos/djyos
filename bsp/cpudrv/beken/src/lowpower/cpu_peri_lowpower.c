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
#include "stdint.h"
#include "lowpower.h"
#include "cpu_peri.h"
#include "cpu.h"
#include "djyos.h"
#include "mcu_ps_pub.h"
#include "manual_ps_pub.h"
#include "wlan_ui_pub.h"
#include <dbug.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    void ModuleInstall_LowPower(void);
//    ModuleInstall_LowPower();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip lowpower control"//�͹��������������
//parent:"none"                      //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                          //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                       //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                         //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                         //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                         //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"                  //������������������������none����ʾ�������������
                                         //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                         //����������������������г����á�,���ָ�
//weakdependence:"none"                  //��������������������������none����ʾ�������������
                                         //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                         //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                         //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_LOWPOWER_CONTROL == false )
//#warning  " cpu_onchip_peripheral_lowpower_control  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header              //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_LOWPOWER_CONTROL    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure

//static struct gpio_to_wakeup_t{
//    u32 index_map;
//    u32 edge_map;
//}gGpioToWakeUpL4;

static PS_DEEP_CTRL_PARAM deep_sleep_param;


//-----------------------------------------------------------------------------
//����: ��������
//����: sleep_level,���ߵȼ�
//      pend_ticks, ����tick��
//����: ������
//-----------------------------------------------------------------------------
void __LP_BSP_EntrySleep(u8 sleep_level, u32 pend_ticks)
{
    switch(sleep_level)
    {
        case CN_SLEEP_L0:
            mcu_power_save(pend_ticks);
            break;
        case CN_SLEEP_L1:
            warning_printf("LP", "Entry sleep level_1 undefined");
            break;
        case CN_SLEEP_L2:
            warning_printf("LP", "Entry sleep level_2 undefined");
            break;
        case CN_SLEEP_L3:
            warning_printf("LP", "Entry sleep level_3 undefined");
            break;
        case CN_SLEEP_L4:
            DJY_DelayUs(100000);
            bk_enter_deep_sleep_mode(&deep_sleep_param);
            break;
    }
}

void ImmediatelyDeepSleep(void)
{
    bk_enter_deep_sleep_mode(&deep_sleep_param);
}

//-----------------------------------------------------------------------------
//���ܣ����ô����˯���л������������õ��������Ե���
//������way, ������Դ����ѡ��PS_DEEP_WAKEUP_GPIO��PS_DEEP_WAKEUP_RTC��PS_DEEP_WAKEUP_GPIO_RTC
//      gpio_index�����way == PS_DEEP_WAKEUP_GPIO��ָ��gpio��ţ�0~39����P0~P39
//          Ҫ���ö�����ţ��ɶ�ε��ñ�������way ��������ֵ��������Ч��
//      gpio_edge�����ѱ��أ�0 = �����أ�1 = �½��أ�way == PS_DEEP_WAKEUP_GPIO����Ч
//      time�����way == PS_DEEP_WAKEUP_RTC�����ڱ�ʾ����ʱ�䣬��λ = ��
//���أ���
//-----------------------------------------------------------------------------
void LP_BSP_ResigerGpioToWakeUpL4(PS_DEEP_WAKEUP_WAY way,u32 gpio_index,
                                  u32 gpio_edge, u32 time)
{
    u32 map;
    deep_sleep_param.wake_up_way    = way;
    if(way == PS_DEEP_WAKEUP_GPIO)
    {
        if(gpio_index <= 31)
        {
            map = 1 << gpio_index;
            deep_sleep_param.gpio_index_map |= map;
            map = gpio_edge << gpio_index;
            deep_sleep_param.gpio_edge_map |= map;
        }
        else
        {
            map = 1 << (gpio_index - 32);
            deep_sleep_param.gpio_last_index_map |= map;
            map = gpio_edge << (gpio_index - 32);
            deep_sleep_param.gpio_last_edge_map |= map;
        }
    }
    deep_sleep_param.sleep_time             = time;
}

bool_t __LP_BSP_SaveSleepLevel(u32 SleepLevel)
{
    return true;
}

bool_t __LP_BSP_SaveRamL3(void)
{
    return true;
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

u32 BekenEntrySleepReCall(u32 SleepLevel)
{
    return true;
}

u32 BekenExitSleepReCall(u32 SleepLevel)
{
    return true;
}
#pragma GCC diagnostic pop

//-----------------------------------------------------------------------------
//����: ��װ�͹��������Ҫ��һЩ�͹�����Ҫʹ�õ��ĺ�����ע�ᵽϵͳ��
//����: __LP_BSP_EntrySleep���������ߣ�__LP_BSP_SaveSleepLevel���������ߵȼ���__LP_BSP_SaveRamL3������������ߵȼ�3֮ǰ���ڴ棬
//      __LP_BSP_AsmSaveReg����ȡ���Լ��ķ��ص�ַ���ڵ������Ĳ����浽ջ��
//����: ��
//-----------------------------------------------------------------------------
void ModuleInstall_LowPower (void)
{
    Register_LowPower_Function(__LP_BSP_EntrySleep, __LP_BSP_SaveSleepLevel, __LP_BSP_SaveRamL3, __LP_BSP_AsmSaveReg);
}



