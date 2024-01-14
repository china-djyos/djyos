#include <stdint.h>
#include <stddef.h>
#include <lowpower.h>
#include <cpu_peri.h>
#include <core_cm4.h>
#include <dbug.h>

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    void ModuleInstall_LowPower (void)
//    ModuleInstall_LowPower();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip peripheral lowpower control"//�͹��������������
//parent:"none"                      //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                          //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                       //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                         //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                         //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                         //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"//������������������������none����ʾ�������������
                                         //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                         //����������������������г����á�,���ָ�
//weakdependence:"none"                  //��������������������������none����ʾ�������������
                                         //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                         //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                         //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_PERIPHERAL_LOWPOWER_CONTROL == false )
//#warning  " cpu_onchip_peripheral_lowpower_control  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header              //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_PERIPHERAL_LOWPOWER_CONTROL    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
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


//----��ʼ���͹���Ӳ��--------------------------------------------------------
//����: ��ʼ���͹��Ĺ���Ӳ��,�������Ҫ��ʼ��,����ֱ��return true.
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
//      CN_SLEEP_NORMAL��ʾ�ϵ����Ӳ��λ,�������ָ�λ.ע��,����ֵ������
//      CN_SLEEP_L0~2֮һ.
//����: ��
//����: CN_SLEEP_L3��CN_SLEEP_L4��CN_SLEEP_NORMAL֮һ
//----------------------------------------------------------------------------
u32 __LP_BSP_GetSleepLevel(void)
{
    u32 bkt_DR;
    RTC_HandleTypeDef RTC_Handler;  //RTC���
    RTC_Handler.Instance=RTC;
    if(__HAL_PWR_GET_FLAG(PWR_FLAG_WU+PWR_FLAG_SB)& PWR_FLAG_WU)
    {
        bkt_DR = HAL_RTCEx_BKUPRead(&RTC_Handler,RTC_BKP_DR0);//todo
        if( (bkt_DR == CN_SLEEP_L3) || (bkt_DR == CN_SLEEP_L4) )
            return bkt_DR;
        else
            return CN_SLEEP_NORMAL;
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
    RTC_HandleTypeDef RTC_Handler;  //RTC���
    RTC_Handler.Instance=RTC;

    if((SleepLevel!= CN_SLEEP_L3) && (SleepLevel!= CN_SLEEP_L4))
        return false;
    HAL_RTCEx_BKUPWrite(&RTC_Handler,RTC_BKP_DR0,SleepLevel);
    return true;

}

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
            SCB->SCR |= SCB_SCR_SEVONPEND_Msk;
            SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
            __WFE();
            break;
        case CN_SLEEP_L1:
            SCB->SCR |= SCB_SCR_SEVONPEND_Msk;
            SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
            __WFE();
            break;
        case CN_SLEEP_L2:
            //��ֹ�ж�

            //�������ⲿ�жϱ�־��RTC���ӱ�־
            EXTI->PR = 0xFFFFF;
            HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
            break;
        case CN_SLEEP_L3:
            HAL_PWR_EnterSTANDBYMode();
            break;
        case CN_SLEEP_L4:
            HAL_PWR_EnterSTANDBYMode( );
            break;
    }
}

bool_t __LP_BSP_RestoreRamL3(void)
{
    return true;
}
bool_t __LP_BSP_SaveRamL3(void)
{
   return true;
}

//-----------------------------------------------------------------------------
//����: ��װ�͹��������Ҫ��һЩ�͹�����Ҫʹ�õ��ĺ�����ע�ᵽϵͳ��
//����: __LP_BSP_EntrySleep���������ߣ�__LP_BSP_SaveSleepLevel���������ߵȼ���__LP_BSP_SaveRamL3������������ߵȼ�3֮ǰ���ڴ棬
//		__LP_BSP_AsmSaveReg����ȡ���Լ��ķ��ص�ַ���ڵ������Ĳ����浽ջ��
//����: ��
//-----------------------------------------------------------------------------
void ModuleInstall_LowPower (void)
{
    Register_LowPower_Function(__LP_BSP_EntrySleep, __LP_BSP_SaveSleepLevel, __LP_BSP_SaveRamL3, __LP_BSP_AsmSaveReg);
}
