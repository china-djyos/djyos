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

// �ļ���     ��board.c
// ģ������: �����ز��ֳ�ʼ�������õ�
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 5/11.2015
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "os.h"
#include "cpu_peri.h"
#include "uartctrl.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"board config"//�����������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                       //ѡ�third��system��bsp��user����������������IDE�з���
//select:required                     //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                      //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                     //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                      //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"kernel","stm32f1","cpu onchip gpio","cpu onchip peripheral lowpower control"//������������������������none����ʾ�������������
                                      //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                      //����������������������г����á�,���ָ�
//weakdependence:"none"               //��������������������������none����ʾ�������������
                                      //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                      //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                      //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_BOARD_CONFIG == false )
//#warning  " board_config  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_BOARD_CONFIG    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,flase,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

void __lcd_reset(void)
{
    GPIO_SettoLow(CN_GPIO_G,0x100);
    DJY_DelayUs(5000);
    GPIO_SettoHigh(CN_GPIO_G,0x100);
}

void lcd_backlight_on(void)
{
    GPIO_SettoLow(CN_GPIO_F,0x400);
}

void lcd_backlight_off(void)
{
    GPIO_SettoHigh(CN_GPIO_F,0x400);
}

void __Lcd_BoardConfig( void )
{
    GPIO_PowerOn(CN_GPIO_F);
    GPIO_PowerOn(CN_GPIO_G);
    GPIO_CfgPinFunc(CN_GPIO_F,10,CN_GPIO_MODE_GPIO_OUT_OD_2Mhz);
    GPIO_CfgPinFunc(CN_GPIO_G,8,CN_GPIO_MODE_GPIO_OUT_PP_2Mhz);
}

//----��ʼ������Ӳ��-----------------------------------------------------------
//����: ���⣬��ʼ�������ɨ�����
//����: ��
//����: ��
//----------------------------------------------------------------------------
bool_t key_hard_init(void)
{
    GPIO_PowerOn(CN_GPIO_C);
    GPIO_CfgPinFunc(CN_GPIO_C,10,CN_GPIO_MODE_IN_PULLUP);
    GPIO_CfgPinFunc(CN_GPIO_C,11,CN_GPIO_MODE_IN_PULLUP);
    GPIO_CfgPinFunc(CN_GPIO_C,12,CN_GPIO_MODE_IN_PULLUP);
    GPIO_CfgPinFunc(CN_GPIO_C,13,CN_GPIO_MODE_IN_PULLUP);
    return true;
}

// =============================================================================
// ���ܣ����ݾ���İ�����ô��ڵ�GPIO�����Ź��ܣ�����������أ����Ըú������ڸ��ļ���CPU
//      ��������ֱ�ӵ��øú�������ʼ�����ڵ�GPIO����
//      ��Ҫ����GPIO�˿ںʹ���ʱ��ʹ�ܡ�GPIO���á���ӳ�䡢ʱ�ӵ�
// ��������
// ���أ�true
// =============================================================================
bool_t Board_UartGpioInit(u8 SerialNo)
{
    //��ʼ��IO�˿�λuart����
    //Ports  :  GPA10 GPA9 GPA8 GPA7 GPA6 GPA5 GPA4 GPA3 GPA2 GPA1 GPA0
    //Signal :   RXD1 TXD1  xx   xx   xx   xx   xx   xx   xx   xx   xx
    switch(SerialNo)
    {
    case CN_UART1:
        bb_rcc_apb2enr_uart1en = 1;     //uart1ʱ��ʹ��
        GPIO_PowerOn(CN_GPIO_A);
        GPIO_CfgPinFunc(CN_GPIO_A,9,CN_GPIO_MODE_PERI_OUT_PP_10Mhz);
        GPIO_CfgPinFunc(CN_GPIO_A,10,CN_GPIO_MODE_IN_FLOATING);
        break;
    case CN_UART2:
        bb_rcc_apb1enr_uart2en = 1;     //uart2ʱ��ʹ��
        GPIO_PowerOn(CN_GPIO_A);
        GPIO_CfgPinFunc(CN_GPIO_A,2,CN_GPIO_MODE_PERI_OUT_PP_10Mhz);
        GPIO_CfgPinFunc(CN_GPIO_A,3,CN_GPIO_MODE_IN_FLOATING);
        break;
    case CN_UART3:
        bb_rcc_apb1enr_uart3en = 1;     //uart3ʱ��ʹ��
        GPIO_PowerOn(CN_GPIO_B);
        GPIO_CfgPinFunc(CN_GPIO_B,10,CN_GPIO_MODE_PERI_OUT_PP_10Mhz);
        GPIO_CfgPinFunc(CN_GPIO_B,11,CN_GPIO_MODE_IN_FLOATING);
        break;
    case CN_UART4:
        bb_rcc_apb1enr_uart4en = 1;     //uart4ʱ��ʹ��
        GPIO_PowerOn(CN_GPIO_C);
        GPIO_CfgPinFunc(CN_GPIO_C,10,CN_GPIO_MODE_PERI_OUT_PP_10Mhz);
        GPIO_CfgPinFunc(CN_GPIO_C,11,CN_GPIO_MODE_IN_FLOATING);
        break;
    case CN_UART5:
        bb_rcc_apb1enr_uart5en = 1;    //uart5ʱ��ʹ��
        GPIO_PowerOn(CN_GPIO_C);
        GPIO_PowerOn(CN_GPIO_D);
        GPIO_CfgPinFunc(CN_GPIO_C,12,CN_GPIO_MODE_PERI_OUT_PP_10Mhz);
        GPIO_CfgPinFunc(CN_GPIO_D,2,CN_GPIO_MODE_IN_FLOATING);
        break;
    default:
        break;
    }
 }

// =============================================================================
// ���ܣ�Board_GpioInit���������õ���GPIO��ʼ��
//  �����ｫ���п����õ��ĸ���ģʽ��gpio������������Ҫ��ȡ��ע�ͼ���
// ��������
// ���أ���
// =============================================================================

void Board_Init(void)
{
    Board_UartGpioInit(CN_UART1);
    Board_UartGpioInit(CN_UART2);
}
