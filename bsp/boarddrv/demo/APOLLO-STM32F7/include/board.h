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

#ifndef __BOARD__H__
#define __BOARD__H__

//#include "./include/ctiic.h"
#include "cpu_peri.h"

#ifdef __cplusplus
extern "C" {
#endif

/**********************************ft5x26********************************************/
//IO��������
#define CT_SCL_OUT() {GPIOH->MODER&=~(3<<(6*2));GPIOH->MODER|=1<<6*2;}  //PH6���ģʽ
#define CT_SDA_IN()  {GPIOI->MODER&=~(3<<(3*2));GPIOI->MODER|=0<<3*2;}  //PI3����ģʽ
#define CT_SDA_OUT() {GPIOI->MODER&=~(3<<(3*2));GPIOI->MODER|=1<<3*2;}  //PI3���ģʽ
//IO��������
#define CT_IIC_SCL(n) (n?GPIO_SettoHigh(GPIO_H,PIN6):GPIO_SettoLow(GPIO_H,PIN6))//SCL
#define CT_IIC_SDA(n) (n?GPIO_SettoHigh(GPIO_I,PIN3):GPIO_SettoLow(GPIO_I,PIN3))//SDA
#define CT_READ_SDA   (GPIO_GetData(GPIO_I)&PIN3)//����SDA

/*������оƬ�ӿ�IO*/
#define FT_RST(n)  (n?GPIO_SettoHigh(GPIO_I,PIN8):GPIO_SettoLow(GPIO_I,PIN8))
#define FT_INT      (GPIO_GetData(GPIO_H)&PIN7)
/*************************************************************************************/

/********************************PCF8574***********************************************/
//IO��������
#define PCF_SCL_OUT() {GPIOH->MODER&=~(3<<(4*2));GPIOH->MODER|=1<<4*2;} //PH4���ģʽ
#define PCF_SDA_IN()  {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=0<<5*2;} //PH5����ģʽ
#define PCF_SDA_OUT() {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=1<<5*2;} //PH5���ģʽ
//IO����
#define PCF_IIC_SCL(n)  (n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,GPIO_PIN_RESET)) //SCL
#define PCF_IIC_SDA(n)  (n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,GPIO_PIN_RESET)) //SDA
#define PCF_READ_SDA    HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_5)  //����SDA

#define PCF8574_INT  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)//PCF8574 INT��
//PCF8574����IO�Ĺ���
#define BEEP_IO         0       //��������������   P0
#define AP_INT_IO       1       //AP3216C�ж�����   P1
#define DCMI_PWDN_IO    2       //DCMI�ĵ�Դ��������   P2
#define USB_PWR_IO      3       //USB��Դ�������� P3
#define EX_IO           4       //��չIO,�Զ���ʹ��    P4
#define MPU_INT_IO      5       //MPU9250�ж�����   P5
#define RS485_RE_IO     6       //RS485_RE����        P6
#define ETH_RESET_IO    7       //��̫����λ����       P7
/*************************************************************************************/
void Board_Init(void);
void FT5206_RST(bool_t flag);
void Board_UartHalfDuplexSend(u8 SerialNo);
void Board_UartHalfDuplexRecv(u8 SerialNo);
void LCD_PinInit(void );
void Lcd_BackLight_OnOff(u8 onoff);
bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level);

//void IIC_pcf8574_Init(void);
//void IIC_Start(void);
//void IIC_Stop(void);
//u8 IIC_Wait_Ack(void);
//void IIC_Ack(void);
//void IIC_NAck(void);
//void IIC_Send_Byte(u8 txd);
//u8 IIC_Read_Byte(unsigned char ack);
//void PCF8574_WriteOneByte(u8 DataToWrite);
//u8 PCF8574_Init(void);
//u8 PCF8574_ReadOneByte(void);
//void PCF8574_WriteBit(u8 bit,u8 sta);
//u8 PCF8574_ReadBit(u8 bit);
//bool_t LAN8720_RESET(void);

//bool_t ModuleInstall_Touch_FT5206(struct GkWinObj *desktop,\
//        const char *touch_dev_name );
//
//bool_t ModuleInstall_KeyBoardHal(const char *dev_name);
//u32 keyboard_scan(void);
//
//bool_t lan8720Init(void);
//
//bool_t Ltdc_Lcd_Config(struct LCD_ConFig *lcd);

void FT5X26_Pin_Init(void);
u32 IIC_IoCtrlFunc(enum IIc_Io IO,u32 tag);
unsigned char  NAND_RB_Get(void);
bool_t LAN8720_RESET(void);

#ifdef __cplusplus
}
#endif
#endif






