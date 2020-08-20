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
#include "lowpower.h"
#include "cpu_peri.h"
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
//parent:"none"  //��д������ĸ�������֣�none��ʾû�и����
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
//%$#@target = header                    //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_PERIPHERAL_LOWPOWER_CONTROL    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

extern   uint32_t   msp_top[ ];
#define  CN_LP_STACK_SIZE  0x100

#define Stm32SleepModel4    0x789a

//----��ʼ���͹���Ӳ��--------------------------------------------------------
//����: ��ʼ���͹��Ĺ���Ӳ��,�������Ҫ��ʼ��,����ֱ��return true.��Ϊstm32�͹���
//      ������Ҫʹ�ñ������������, ����Ҫ�򿪱���ģ���Դ��ʹ�ܱ���������.
//����: ��
//����: ��
//----------------------------------------------------------------------------
bool_t __LP_BSP_HardInit(void)
{
    RCC->APB1ENR |=RCC_APB1ENR_BKPEN|RCC_APB1ENR_PWREN;//PWRģ��ʹ��
    HAL_PWR_EnableBkUpAccess();//����ʹ��
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
            HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFE);
            break;
        case CN_SLEEP_L1:
            HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFE);
            break;
        case CN_SLEEP_L2:
            //��ֹ�ж�
            //�������ⲿ�жϱ�־��RTC���ӱ�־
            EXTI->PR=0xFFFFF;
            CLEAR_BIT(RTC->CRL, RTC_FLAG_ALRAF);
            HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFE);
            break;
        case CN_SLEEP_L3:
            HAL_PWR_EnterSTANDBYMode();
            break;
        case CN_SLEEP_L4:
            HAL_PWR_EnterSTANDBYMode( );
            break;
    }
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
        bkt_DR = HAL_RTCEx_BKUPRead(&RTC_Handler,RTC_BKP_DR1);//todo
//        bkt_DR = Stm32SleepModel4;
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
    HAL_RTCEx_BKUPWrite(&RTC_Handler,RTC_BKP_DR1,SleepLevel);
    return true;

}

//64K��Ƭ��RAM������flash���洢����64k��Χ��
//���洢��Ϊ2K/Page
static void LP_RamFlashErase(void)
{
    u8 page;
    u32 addr;
    u32 PAGEError = 0;//�洢��������
    static FLASH_EraseInitTypeDef EraseInitStruct;

    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.NbPages     = 1;
    HAL_FLASH_Unlock();
    for(page = 255; page > 255-32; page--)
    {
        addr = page*0x800 + 0x08000000;
        EraseInitStruct.PageAddress = addr;

        HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError);
    }
    HAL_FLASH_Lock();
}

static u32 LP_RamFlashProgram(u32 addr,u32 *buf,u32 len)
{
    u32 i;
    HAL_FLASH_Unlock();
    for(i = 0; i < len; i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr+4*i, buf[i]);
    }
    HAL_FLASH_Unlock();
    return len;
}

static u32 LP_RamFlashRead(u32 addr,u32 *buf,u32 len)
{
    u32 i,temp;

    for(i = 0; i < len; )
    {
        temp = *(u32*)(addr+4*i);
        buf[i] = temp;
        i++;
    }
    return len;
}
//----�ָ�RAM------------------------------------------------------------------
//����: �ӷ���ʧ�洢���лָ�RAM������,�Ա�ӽ���͹��Ĵ��������г���,������L3��
//      �͹���.������������֧��L3���͹���,��������ֱ�ӷ���false.
//����: ��
//����: true=�ɹ��ָ�,false=ʧ��
//-----------------------------------------------------------------------------
bool_t __LP_BSP_RestoreRamL3(void)
{
    u32 FlashAddr,len;
    u32 *RamAddr;
//  ModuleInit_K9f1208();       //init
//  for(i = 0; i < 256; i++)
//  {
//     NandFlash_Read(4096-300+i,0,0x60000000+0x4000*i,0x4000);
//  }
//
//  //�ڲ�SRAM�ֳ�������
//  //1.0x20000000 ~ msp_top-100 ,��ָ�
//  //2.msp_top-100 ~ msp_top ,���ָܻ�����Ϊ��ʱ��flash��������ʹ��
//  //3.msp_top - 0x20004000, ��ָ�
//  temp = ((uint32_t)msp_top-CN_LP_STACK_SIZE - 0x20000000)/(uint32_t)0x4000;
//  for(i = 0; i < temp; i++)
//  {
//     NandFlash_Read(4096-4+i,0,0x20000000+0x4000*i,0x4000);
//  }
//  temp = ((uint32_t)msp_top-CN_LP_STACK_SIZE - 0x20000000)%0x4000;
//  NandFlash_Read(4096-4+i,0,0x20000000+0x4000*i,temp);
//
//  //���������1.msp~msp-100����һ��block; 2.��������block
//  if((uint32_t)msp_top/0x4000 == ((uint32_t)msp_top-CN_LP_STACK_SIZE)/0x4000)
//      NandFlash_Read(4096-4+i,temp+CN_LP_STACK_SIZE,
//              temp+0x20000000+0x4000*i+CN_LP_STACK_SIZE,0x4000-CN_LP_STACK_SIZE-temp);
//  else
//  {
//      i++;
//      temp = ((uint32_t)msp_top- 0x20000000)%0x4000;
//      NandFlash_Read(4096-4+i,temp,0x20000000+0x4000*i+temp,0x4000-temp);
//  }
//
//  i++;
//
//  //����ʣ�ಿ��
//  for(; i <4 ; i++)
//  {
//      NandFlash_Read(4096-4+i,temp,0x20000000+0x4000*i+CN_LP_STACK_SIZE,0x4000);
//  }

    //�ڲ�SRAM�ֳ�������
    //1.0x20000000 ~ msp_top-100 ,��ָ�
    //2.msp_top-100 ~ msp_top ,���ָܻ�����Ϊ��ʱ��flash��������ʹ��
    //3.msp_top - 0x20004000, ��ָ�
    RamAddr = (u32*)0x20000000;
    FlashAddr =  224*0x800 + 0x08000000;
    len = ((uint32_t)msp_top-CN_LP_STACK_SIZE - 0x20000000)/4;

    LP_RamFlashRead(FlashAddr,RamAddr,len);

    RamAddr = (u32 *)msp_top;
    FlashAddr = FlashAddr +len*4 + CN_LP_STACK_SIZE;
    len = (64*1024 - ((uint32_t)msp_top - 0x20000000))/4;

    LP_RamFlashRead(FlashAddr,RamAddr,len);

    return true;
}

//----����RAM------------------------------------------------------------------
//����: ����L3���͹���ǰ,��RAM�����ݱ��浽����ʧ�洢����,�Ա��˳��͹���״̬ʱ,
//      �ָ��ڴ沢�ӳ����жϴ���������.������������֧��L3���͹���,��������
//      ֱ�ӷ���false.
//����: ��
//����: true=�ɹ��ָ�,false=ʧ��
//-----------------------------------------------------------------------------
bool_t __LP_BSP_SaveRamL3(void)
{
    u32 RamAddr,FlashAddr,len;
   //���ֳ������е����ݱ��浽flash
//   if(ModuleInit_K9f1208())
//   {
//     i = 300;
//     while(i--)
//     {
//         NandFlash_Erase(4096-i);
//     }
//     for(i = 0; i < 256; i++)
//     {
//         NandFlash_Write(4096-300+i,0,0x60000000+0x4000*i,0x4000);
//     }
//     for(i = 0; i < 4; i++)
//     {
//         NandFlash_Write(4096-4+i,0,0x20000000+0x4000*i,0x4000);
//     }
//   }
//   else
//     return false;
    RamAddr = 0x20000000;
    FlashAddr =  224*0x800 + 0x08000000;
    len = 64*1024/4;

    LP_RamFlashErase();

    LP_RamFlashProgram(FlashAddr,(u32*)RamAddr,len);

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

