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

// �ļ���     ��cpu_peri_gpio.h
// ģ������: scm6xx��GPIO����
// ģ��汾:
// ������Ա:
// ����ʱ��: 10/28.2016
// =============================================================================
#include "stdint.h"
#include "cpu_peri.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���

//%$#@end initcode  ****��ʼ���������
//%$#@describe      ****���������ʼ
//component name:"cpu onchip gpio"//gpio����������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                  //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                 //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                 //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                 //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence                   //������������������������none����ʾ�������������
                                 //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                 //����������������������г�
//weakdependence:"none"          //��������������������������none����ʾ�������������
                                 //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                 //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                 //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO == false )
//#warning  " cpu_onchip_gpio  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

#include "stdint.h"
#include "cpu_peri.h"
#include "stddef.h"

typedef struct
{
    volatile u32 DR;
    volatile u32 DIR;
    volatile u32 INTER;
    volatile u32 INTMR;
    volatile u32 INTSR;
    volatile u32 DBSYR;
    volatile u32 INTCR;
    volatile u32 ETPR;
    volatile u32 PULLEN;
    volatile u32 PULLTYPE;
}tagGpioReg;

#define CN_PIN_MAX (113)

// =============================================================================
// ����: GPIO ���������������
// ����: Mode
//     EN_INPUT_MODE  -����Ϊ����ģʽ
//     EN_OUTPUT_MODE -����Ϊ���ģʽ
//     PortNum - Ҫ���õĶ˿ں�
//               �˿ں�Ϊ0~113֮��
//
// ����: ���óɹ�����true ����ʧ�ܷ���false
// =============================================================================

bool_t Gpio_DirConfig(const u8 Mode,const u8 PortNum)
{
    bool_t bRet = false;

    if(Mode != EN_INPUT_MODE && Mode != EN_OUTPUT_MODE)
    {
        return bRet;
    }

    if(PortNum > CN_PIN_MAX)
    {
        return bRet;
    }

    switch(Mode)
    {
        case EN_INPUT_MODE:
             GPIO_Configure_Dir(PortNum,EN_INPUT_MODE);
             bRet = true;
             break;
        case EN_OUTPUT_MODE:
             GPIO_Configure_Dir(PortNum,EN_OUTPUT_MODE);
             bRet = true;
             break;
        default:;break;
    }

    return bRet;
}


// =============================================================================
// ����: GPIO �����ƽ����
// ����: Level
//     EN_GPIO_LEV_L  -����͵�ƽ
//     EN_GPIO_LEV_H  -����ߵ�ƽ
//     PortNum - Ҫ���õĶ˿ں�
//               �˿ں�Ϊ0~113֮��
//
// ����: ���óɹ�����true ����ʧ�ܷ���false
// =============================================================================

void GPIO_SettoHigh(u32 port,u32 msk)
{
    GPIO_Write_Out(port, EN_GPIO_LEV_H);
}


void GPIO_SettoLow(u32 port,u32 msk)
{
    GPIO_Write_Out(port, EN_GPIO_LEV_L);
}

// =============================================================================
// ����: GPIO ��ȡ�˿ڵ�ƽ
// ����:
//     PortNum - Ҫ���õĶ˿ں�
//               �˿ں�Ϊ0~113֮��
//
// ����: �ɹ�:���ض�Ӧ�˿ڵ�ƽ  ʧ��:����-1
// =============================================================================

char Gpio_ReadLevel(u8 PortNum)
{
    char byRet = CN_RORT_READ_FAILED;

    if(PortNum > CN_PIN_MAX)
    {
        return byRet;
    }

    byRet = GPIO_Read_In(PortNum);

    return byRet;
}

// =============================================================================
// ����: �ⲿ�ж�ʹ��
// ����:
//     PortNum - Ҫ���õĶ˿ں�
//               �˿ں�Ϊ0~113֮��
//
// ����:
// =============================================================================

bool_t Gpio_ExIntEnable(u8 PortNum)
{
    bool_t bRet = false;

    if(PortNum > CN_PIN_MAX)
    {
        return bRet;
    }

    GPIO_Enable_Int(PortNum,1);

    return true;
}

// =============================================================================
// ����: �ⲿ�ж�ʧ��
// ����:
//     PortNum - Ҫ���õĶ˿ں�
//               �˿ں�Ϊ0~113֮��
//
// ����:
// =============================================================================

bool_t Gpio_ExIntDisable(u8 PortNum)
{
    bool_t bRet = false;

    if(PortNum > CN_PIN_MAX)
    {
        return bRet;
    }

    GPIO_Enable_Int(PortNum,0);

    return true;
}

// =============================================================================
// ����: �ⲿ�ж����λ�����
// ����:
//     PortNum - Ҫ���õĶ˿ں�
//               �˿ں�Ϊ0~113֮��
//
// ����:
// =============================================================================

bool_t Gpio_IntMask(u8 PortNum,u8 MaskCode)
{
    bool_t bRet = false;

    if(PortNum > CN_PIN_MAX)
    {
        return bRet;
    }

    if(MaskCode != EN_GPIO_UMASK && MaskCode != EN_GPIO_MASK)
    {
        return bRet;
    }

    GPIO_Mask_Int(PortNum,MaskCode);

    return true;
}


// =============================================================================
// ����: ���жϱ�־
// ����:
//     PortNum - Ҫ���õĶ˿ں�
//               �˿ں�Ϊ0~113֮��
//
// ����:
// =============================================================================

bool_t Gpio_ClearIntFlag(u8 PortNum)
{
    bool_t bRet = false;

    if(PortNum > CN_PIN_MAX)
    {
        return bRet;
    }

    GPIO_Clear_Int_Flag(PortNum);

    return true;
}


// =============================================================================
// ����: �ⲿ�ж����λ�����
// ����:
//     PortNum - Ҫ���õĶ˿ں�
//               �˿ں�Ϊ0~113֮��
//
// ����:
// =============================================================================

bool_t Gpio_IntModeConfig(u8 PortNum,u8 IntMode)
{
    bool_t bRet = false;

    if(PortNum > CN_PIN_MAX)
    {
        return bRet;
    }

    if(IntMode > EN_MODE_END)
    {
        return bRet;
    }

    GPIO_Configure_Int_Mode(PortNum,IntMode);

    return true;
}



// =============================================================================
//����: ��ĳgpio port�������
//����: port����������port��ţ�����Ҫ����GPIOA,��port=GPIO_A
//����: ���õ�����
// =============================================================================
u32 GPIO_GetData(u32 port)
{

}

// =============================================================================
// ����: ���������������ĳgpio port
// ����: port����������port��ţ�����Ҫ����GPIOA,��port=GPIO_A
//      data�������������
// ����: ��
// =============================================================================
void GPIO_OutData(u32 port,u32 data)
{

}



// =============================================================================
// ����: ��GPIOA-GPIOI��ʱ�����
// ������port,�˿ںţ���GPIO_A
// ���أ���
// =============================================================================
void GPIO_PowerOn(u32 port)
{

}

// =============================================================================
// ����: Ϊ�˽��ܣ����͹��ģ�ֱ�ӹر�GPIOA-GPIOI��ʱ�����
// ������port,�˿ںţ���GPIO_A
// ���أ���
// =============================================================================
void GPIO_PowerOff(u32 port)
{

}















