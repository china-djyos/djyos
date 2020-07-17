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

#include "stdint.h"
#include "stddef.h"
//#include "cpu_peri_gpio.h"
//#include "cpu_peri_iic.h"
#include "IoIicBus.h"
#include "pcf8574.h"
#include "board.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中

//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"board config"//组件名
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:required               //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"kernel","stm32h7","cpu onchip gpio"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_BOARD_CONFIG == false )
//#warning  " board_config  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_BOARD_CONFIG    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


//这里的gpio只定义了一部分 复用到不同的GPIO还需根据手册添加
static const Pin uart1_pin[] = {

        {GPIO_A,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_A,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},


//        {GPIO_B,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
//        {GPIO_B,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},

};


static const Pin uart2_pin[] = {
        {GPIO_A,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_A,PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},

//        {GPIO_D,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
//        {GPIO_D,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
};


static const Pin uart3_pin[] = {
//        {GPIO_D,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
//        {GPIO_D,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},

//        {GPIO_C,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
//        {GPIO_C,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},

        {GPIO_B,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_B,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
};


static const Pin uart4_pin[] = {
        {GPIO_A,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_A,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},

//        {GPIO_C,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
//        {GPIO_C,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
//
 };


static const Pin uart5_pin[] = {
        {GPIO_C,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_D,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
 };

static const Pin uart6_pin[] = {
        {GPIO_C,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_C,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},

//        {GPIO_G,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
//        {GPIO_G,PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
 };

static const Pin uart7_pin[] = {
        {GPIO_F,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_F,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},

//        {GPIO_E,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
//        {GPIO_E,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
//
 };
static const Pin uart8_pin[] = {
        {GPIO_E,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_E,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
 };


static const Pin spi1_pin[] = {
        {GPIO_B,PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_B,PIN4,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_B,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_F,PIN14,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},//普通输出做CS引脚

 };

static const Pin iic1_pin[] = {
        {GPIO_B,PIN8,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
        {GPIO_B,PIN9,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
 };


static const Pin iic2_pin[] = {
//        {GPIO_B,PIN11,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
//        {GPIO_B,PIN11,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
        {GPIO_H,PIN4,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
        {GPIO_H,PIN5,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},

 };

static const Pin iic3_pin[] = {
        {GPIO_H,PIN7,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
        {GPIO_H,PIN8,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
 };

static const Pin iic4_pin[] = {
        {GPIO_H,PIN12,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
        {GPIO_H,PIN11,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
 };

static const Pin Ft_pin[] = {
        {GPIO_I,PIN8,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU},
        {GPIO_H,PIN7,GPIO_MODE_IN,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU},
        {GPIO_H,PIN6,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
        {GPIO_I,PIN3,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
 };

static const Pin Key_pin[] = {
        {GPIO_H,PIN3,GPIO_MODE_IN,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU},//KEY_0
        {GPIO_H,PIN2,GPIO_MODE_IN,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU},//KEY_1
        {GPIO_C,PIN13,GPIO_MODE_IN,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU},//KEY_2
        {GPIO_A,PIN0,GPIO_MODE_IN,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PD},//KEY_up

 };


/*
      RMII_REF_CLK ----------------------> PA1
      RMII_MDIO -------------------------> PA2
      RMII_MDC --------------------------> PC1
      RMII_MII_CRS_DV -------------------> PA7
      RMII_MII_RXD0 ---------------------> PC4
      RMII_MII_RXD1 ---------------------> PC5
      RMII_MII_RXER ---------------------> PG2  //(not used)
      RMII_MII_TX_EN --------------------> PB11
      RMII_MII_TXD0 ---------------------> PG13
      RMII_MII_TXD1 ---------------------> PG14
*/
static const Pin EthRmiiPins[] = {
    {GPIO_A,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_A,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_A,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_C,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_C,PIN4,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_C,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_B,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_G,PIN13,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_G,PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
};



static const Pin SDMMC_Pins[] = {
    {GPIO_C,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF12},
    {GPIO_C,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF12},
    {GPIO_C,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF12},
    {GPIO_C,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF12},
    {GPIO_C,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF12},
    {GPIO_D,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF12},

};

static const Pin FmcNandPins[] = {
    {GPIO_D,PIN6,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF_NUll},       //R/B
    {GPIO_G,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},//CE
    {GPIO_D,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},//D2
    {GPIO_D,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},//D3
    {GPIO_D,PIN4,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},//OE
    {GPIO_D,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},//WE
    {GPIO_D,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},//CLE
    {GPIO_D,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},//ALE
    {GPIO_D,PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},//D0
    {GPIO_D,PIN15,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},//D1

    {GPIO_E,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},//D4
    {GPIO_E,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},//D5
    {GPIO_E,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},//D6
    {GPIO_E,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},
};

void FT5206_RST(bool_t flag)
{
    if(flag)
        PIO_Clear(&uart1_pin[0]);
    else
        PIO_Set(&uart6_pin[0]);
}
// ============================================================================
// 功能：设置串口编号为SerialNo的串口为半双功发送功能
// 参数：SerialNo,串口号
// 返回：无
// ============================================================================
void Board_UartHalfDuplexSend(u8 SerialNo)
{
    switch(SerialNo)
    {
//    case CN_UART1:
//        PIO_Set(&uart1_pin[2]);
//        break;
//    case CN_UART6:
//        PIO_Set(&uart6_pin[2]);
//        break;
    default:
        break;
    }
}

// ============================================================================
// 功能：设置串口编号为SerialNo的串口为半双功接收功能
// 参数：SerialNo,串口号
// 返回：无
// ============================================================================
void Board_UartHalfDuplexRecv(u8 SerialNo)
{
    //拉低为485接收
    switch(SerialNo)
    {
//    case CN_UART1:
//        PIO_Clear(&uart1_pin[2]);
//        break;
//    case CN_UART6:
//        PIO_Clear(&uart6_pin[2]);
//        break;
    default:
        break;
    }
}
void LCD_PinInit(void )
{
    u32 Pin;
    GPIO_PowerOn(GPIO_B);//背光控制引脚
    GPIO_PowerOn(GPIO_F);
    GPIO_PowerOn(GPIO_G);
    GPIO_PowerOn(GPIO_H);
    GPIO_PowerOn(GPIO_I);

    Pin=PIN5;//控制背光的引脚
    GPIO_CfgPinFunc(GPIO_B,Pin,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU);

    Pin=PIN10;
    GPIO_AFSet(GPIO_F,Pin,AF14);
    GPIO_CfgPinFunc(GPIO_F,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE);

    Pin=PIN10;
    GPIO_AFSet(GPIO_F,Pin,AF14);
    GPIO_CfgPinFunc(GPIO_F,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE);

    Pin=PIN6|PIN7|PIN11;
    GPIO_AFSet(GPIO_G,Pin,AF14);
    GPIO_CfgPinFunc(GPIO_G,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE);

    Pin=PIN9|PIN10|PIN11|PIN12|PIN13|PIN14|PIN15;
    GPIO_AFSet(GPIO_H,Pin,AF14);
    GPIO_CfgPinFunc(GPIO_H,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE);

    Pin=PIN0|PIN1|PIN2|PIN3|PIN4|PIN5|PIN6|PIN7|PIN8|PIN9|PIN10;
    GPIO_AFSet(GPIO_I,Pin,AF14);
    GPIO_CfgPinFunc(GPIO_I,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE);

}

//----------------------------------------------------------------
//功能: 开关背光
//参数:  0关背光 非零开背光
//----------------------------------------------------------------
void Lcd_BackLight_OnOff(u8 onoff)
{
  if(onoff)
     GPIO_SettoHigh(GPIO_B,PIN5);
  else
     GPIO_SettoLow(GPIO_B,PIN5);

}

//电容触摸芯片IIC接口初始化
void FT5X26_Pin_Init(void)
{
    GPIO_PowerOn(GPIO_H);
    GPIO_PowerOn(GPIO_I);
    GPIO_CfgPinFunc(GPIO_H,PIN6 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU);
    GPIO_CfgPinFunc(GPIO_I,PIN3 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU);

    GPIO_CfgPinFunc(GPIO_H,PIN7 ,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU);
    GPIO_CfgPinFunc(GPIO_I,PIN8 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU);

}

void PCF8574_Pin_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //使能GPIOB时钟

    GPIO_Initure.Pin=GPIO_PIN_12;           //PB12
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //初始化

    //IIC初始化
    GPIO_PowerOn(GPIO_H);   //使能GPIOH时钟
    //PH4,5初始化设置
    GPIO_Initure.Pin=GPIO_PIN_4|GPIO_PIN_5;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //快速
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);


    //检查PCF8574是否在位
//    IIC_Start();
//    IIC_Send_Byte(PCF8574_ADDR);            //写地址
//    temp=IIC_Wait_Ack();                    //等待应答,通过判断是否有ACK应答,来判断PCF8574的状态
//    IIC_Stop();                             //产生一个停止条件
//    PCF8574_WriteOneByte(0XFF);             //默认情况下所有IO输出高电平
//    return temp;
}

u32 IIC_IoCtrlFunc(enum IIc_Io IO,u32 tag)
{
    switch(tag)
    {
        case 1 :
            switch(IO)
            {
            case scl_set_High : CT_IIC_SCL(1); break;
            case scl_set_Low  : CT_IIC_SCL(0); break;
            case scl_set_out  : CT_SCL_OUT();  break;
            case sda_set_High : CT_IIC_SDA(1); break;
            case sda_get      : return CT_READ_SDA;
            case sda_set_Low  : CT_IIC_SDA(0); break;
            case sda_set_out  : CT_SDA_OUT();  break;
            case sda_set_in   : CT_SDA_IN();   break;
            default:
                break;
            }
        break;
        case 2 :
            switch(IO)
            {
            case scl_set_High : PCF_IIC_SCL(1); break;
            case scl_set_Low  : PCF_IIC_SCL(0); break;
            case scl_set_out  : PCF_SCL_OUT();  break;
            case sda_set_High : PCF_IIC_SDA(1); break;
            case sda_get      : return PCF_READ_SDA;
            case sda_set_Low  : PCF_IIC_SDA(0); break;
            case sda_set_out  : PCF_SDA_OUT();  break;
            case sda_set_in   : PCF_SDA_IN();   break;
            default:
                break;
            }
        break;
        default:
            break;
    }
    return 0;
}

// =============================================================================
// 功能：Board_GpioInit板上所有用到的GPIO初始化
//    在这里将所有可能用到的复用模式的gpio定义在这里需要用取消注释即可
//    建议不要删除
// 参数：无
// 返回：无
// =============================================================================
void Board_Init(void)
{
    bool_t LAN8720_RESET(void);
#if 0
    LAN8720_RESET( );//网口复位
#endif
//    PIO_Configure(uart1_pin, PIO_LISTSIZE(uart1_pin));
   PIO_Configure(uart2_pin, PIO_LISTSIZE(uart2_pin));
//    PIO_Configure(uart3_pin, PIO_LISTSIZE(uart3_pin));
//     PIO_Configure(uart8_pin, PIO_LISTSIZE(uart8_pin));
//     PIO_Configure(SDMMC_Pins, PIO_LISTSIZE(SDMMC_Pins));

//=====================IIC GPIO初始化===================================//
//对于没有片选和使能引脚的IIC从器件当CPU复位发生在IIC读的过程中就有可能发生总线被占死如：qh_1的stmpe811
//     ，所以在这里用I/O模拟的方式让总线是释放
//        IIC_Busfree(GPIO_B,PIN9,PIN8);//iic1
//        PIO_Configure(iic1_pin, PIO_LISTSIZE(iic1_pin));   //IIC

//    PIO_Configure(iic2_pin, PIO_LISTSIZE(iic2_pin));      //IIC
//    PIO_Configure(Ft_pin, PIO_LISTSIZE(Ft_pin));      //IIC

//    IIC_Busfree(GPIO_H,PIN8,PIN7);
//    IIC_Busfree(GPIO_H,PIN12,PIN11);
//I/O模拟IIC 触摸屏用到
//    PIO_Configure(Key_pin,PIO_LISTSIZE(Key_pin));  //KEY
//    PIO_Configure(EthRmiiPins,PIO_LISTSIZE(EthRmiiPins));  //ETH RMII
    PIO_Configure(FmcNandPins,PIO_LISTSIZE(FmcNandPins)); //nand flash
    LCD_PinInit();
}

bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level)
{
    return true;
}
//NANDFLASH的R/B引脚状态获取
unsigned char  NAND_RB_Get(void)
{
    return PIO_Get(&FmcNandPins[0]);
}

//网口的 PHY 芯片：LAN8720复位
bool_t LAN8720_RESET(void)
{
    PCF8574_WriteBit(ETH_RESET_IO,1);
    DJY_DelayUs(100*mS);
    PCF8574_WriteBit(ETH_RESET_IO,0);
    DJY_DelayUs(100*mS);
    return true;
}

