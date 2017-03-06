//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
//����ģ��:ͼ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: qh_1���ili9325�ӿڳ���,��chipdrvĿ¼�µ�ili9325.c��������ص�
//          ���ݷֲ����
//����˵��:
//�޶���ʷ:
//1. ����: 2015-08-27
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"
#include "cpu_peri.h"
#include "ili9325-brd.h"
#include "ili9325.h"
#include "djyos.h"

/*---------------------------------------------------------------------------
���ܣ�  ��lcd�ڲ�0x00�Ĵ���
---------------------------------------------------------------------------*/
u32 __ili9325_read_reg(void)
{
    LCD_CMD=0x0000;
    return LCD_DATA;
}
/*---------------------------------------------------------------------------
���ܣ�  дlcd�ڲ��Ĵ���
---------------------------------------------------------------------------*/
void __ili9325_write_reg(u32 reg_index, u32 data)
{
    LCD_CMD=reg_index;
    LCD_DATA=data;
}

void __lcd_reset(void)
{
    GPIO_SettoLow(CN_GPIO_G,0x100);
    Djy_DelayUs(5000);
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

