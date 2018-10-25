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
#ifndef __LCD_ILI9325_BRD_H__
#define __LCD_ILI9325_BRD_H__
#include "board-config.h"

#define LCD_ON_GPIOF10          GPIO_Pin_10
#define LCD_RST_GPIOG8          GPIO_Pin_8
#define LCD_CS_GPIOG10          GPIO_Pin_10

#define LCD_CMD     (*(volatile u16*)  0x68000000)
#define LCD_DATA    (*(volatile u16*)  0x68000002)

#define cn_coordinates      1       //0=�������꣬1=��ת���꣬��ԭ��=(239,319)

#define cn_lcd_line_size        (CN_LCD_XSIZE<<1)
#define cn_frame_buffer_size    (cn_lcd_line_size * CN_LCD_YSIZE)
#define CN_LCD_PIXEL_FORMAT               CN_SYS_PF_RGB565

#define __ili9325_write_cmd(cmd) (LCD_CMD=cmd)
#define __ili9325_write_data(data) (LCD_DATA=data)
#define __ili9325_read_data(void) (LCD_DATA)

u32 __ili9325_read_reg(void);
void __ili9325_write_reg(u32 reg_index, u32 data);
void __lcd_reset(void);
void __Lcd_BoardConfig( void );
void lcd_backlight_on(void);
void lcd_backlight_off(void);

#endif//__LCD_ILI9325_BRD_H__��ֹ�ظ�����

