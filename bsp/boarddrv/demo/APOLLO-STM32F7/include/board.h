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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

#ifndef __BOARD__H__
#define __BOARD__H__

//#include "./include/ctiic.h"
#include "cpu_peri.h"

#ifdef __cplusplus
extern "C" {
#endif

/**********************************ft5x26********************************************/
//IO方向设置
#define CT_SCL_OUT() {GPIOH->MODER&=~(3<<(6*2));GPIOH->MODER|=1<<6*2;}  //PH6输出模式
#define CT_SDA_IN()  {GPIOI->MODER&=~(3<<(3*2));GPIOI->MODER|=0<<3*2;}  //PI3输入模式
#define CT_SDA_OUT() {GPIOI->MODER&=~(3<<(3*2));GPIOI->MODER|=1<<3*2;}  //PI3输出模式
//IO操作函数
#define CT_IIC_SCL(n) (n?GPIO_SettoHigh(GPIO_H,PIN6):GPIO_SettoLow(GPIO_H,PIN6))//SCL
#define CT_IIC_SDA(n) (n?GPIO_SettoHigh(GPIO_I,PIN3):GPIO_SettoLow(GPIO_I,PIN3))//SDA
#define CT_READ_SDA   (GPIO_GetData(GPIO_I)&PIN3)//输入SDA

/*触摸屏芯片接口IO*/
#define FT_RST(n)  (n?GPIO_SettoHigh(GPIO_I,PIN8):GPIO_SettoLow(GPIO_I,PIN8))
#define FT_INT      (GPIO_GetData(GPIO_H)&PIN7)
/*************************************************************************************/

/********************************PCF8574***********************************************/
//IO方向设置
#define PCF_SCL_OUT() {GPIOH->MODER&=~(3<<(4*2));GPIOH->MODER|=1<<4*2;} //PH4输出模式
#define PCF_SDA_IN()  {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=0<<5*2;} //PH5输入模式
#define PCF_SDA_OUT() {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=1<<5*2;} //PH5输出模式
//IO操作
#define PCF_IIC_SCL(n)  (n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,GPIO_PIN_RESET)) //SCL
#define PCF_IIC_SDA(n)  (n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,GPIO_PIN_RESET)) //SDA
#define PCF_READ_SDA    HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_5)  //输入SDA

#define PCF8574_INT  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)//PCF8574 INT脚
//PCF8574各个IO的功能
#define BEEP_IO         0       //蜂鸣器控制引脚   P0
#define AP_INT_IO       1       //AP3216C中断引脚   P1
#define DCMI_PWDN_IO    2       //DCMI的电源控制引脚   P2
#define USB_PWR_IO      3       //USB电源控制引脚 P3
#define EX_IO           4       //扩展IO,自定义使用    P4
#define MPU_INT_IO      5       //MPU9250中断引脚   P5
#define RS485_RE_IO     6       //RS485_RE引脚        P6
#define ETH_RESET_IO    7       //以太网复位引脚       P7
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






