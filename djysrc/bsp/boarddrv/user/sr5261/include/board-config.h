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
//����ģ��: BSPģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ������������ļ���
//        CPU�ͺ�:STM32F103ZE
//        ����ͺ�:QH_1
//        ������ҵ:����������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-10-31
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif


#define DJY_BOARD       "SR5261"

#define USB_4G_PORT          1
#define USB_HUB_PORT         2

#define CN_CFG_TICK_US 1000  //tick�������usΪ��λ��*

#define Mhz 1000000
#define CN_CFG_MCLK (216*Mhz)  //��Ƶ���ں�Ҫ�ã����붨��
#define CN_CFG_FCLK CN_CFG_MCLK  //cm7������������ʱ��
#define BOARD_MCK   (CN_CFG_MCLK/2) //��MCK

#define SYSCLK_DIV  RCC_SYSCLK_DIV1
#define AHB1_DIV RCC_HCLK_DIV4
#define CN_CFG_PCLK1  (CN_CFG_MCLK/4)

#define AHB2_DIV    RCC_HCLK_DIV2 //RCC_SYSCLK_DIV2


#define CN_CFG_PCLK2  (CN_CFG_MCLK/2)

#define CN_CFG_LSE (32768)
#define CN_CFG_HSE (25*Mhz)


/*____���¶���tick����____*/
#define	CN_CFG_USE_USERTIMER		(0U)//�Ƿ�ʹ��LPTIMER��Ϊϵͳʱ��
#define CN_CFG_TIME_BASE_HZ			CN_CFG_MCLK//(8000U)

#define CN_CFG_STACK_FILL 'd'  //ջ���ֵ����Ϊ0�����

//CPU�ܹ��������,��ѡ���߿��ܿ�ѡ�Ĳ�����������,�����С��,�ǿ�ѡ��,����������,
//��CPU�ֳ��̶�,���ֳ���BSP��arch.h�ļ��ж���
//�洢����С�˵�����
#define CN_CFG_LITTLE_ENDIAN        0
#define CN_CFG_BIG_ENDIAN           1
#define CN_CFG_BYTE_ORDER          CN_CFG_LITTLE_ENDIAN

//#define MAX_PIO_INTERRUPT_SOURCES  5

//#define GMAC_RAW                  //�����˴˺꣬�������ܣ�������Э��ջ

#define BOARD_GMAC_PHY_ADDR        0x10
#define MAX_PIO_INTERRUPT_SOURCES  5


//------------------------------------------------------------------
//LCD�йص�����
#define CN_LCD_XSIZE 800
#define CN_LCD_YSIZE 480
//���������꣬���ڹ��F7��оƬbug������оƬ�ɶ���Ϊ��
#define PatchF7Bug_SetA0       GPIO_SettoHigh(GPIO_F,1)
#define PatchF7Bug_ClearA0     GPIO_SettoLow(GPIO_F,1)
#define SPI_BUS_NAME           "SPI5"

#define ADS8688_SPI_BUS_NAME      "SPI4"
#define ADS8688_SPI_BASE_ADDR     (SPI4)

#define FRAM_ADDR              (0x54)    //���������ַ
#define AT24_ADDRESS           (0x50)            //EEPROM��ַ

#define TAC9535_IIC_BUS_NAME    "IIC1"

#define MCP2515_SPIX                    SPI1
#define MCP2515_EXIT_INT_LINE           CN_INT_LINE_EXTI3

#define AT45_Page_Size              (512)

#define MAX98357_DMAX_StreamX               DMA1_Stream4
#define MAX98357_SPIX                       SPI2
#define MAX98357_CN_INT_LINE_DMAX_StreamX   CN_INT_LINE_DMA1_Stream4

#define NANDFLASH_NO_FS_PAGES       (512)

#define CN_CAN_NUM                    2

#define MCP2515_EXIT_INT_LINE       CN_INT_LINE_EXTI3
#ifdef __cplusplus
}
#endif
#endif // __CPU_OPTIONAL_H__



