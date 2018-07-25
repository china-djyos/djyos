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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
#include "cpu_peri_gpio.h"
#include "cpu_peri_iic.h"
#include "cpu_peri_uart.h"
#include "uartctrl.h"


//这里的gpio只定义了一部分 复用到不同的GPIO还需根据手册添加
//SR7363板件配置

//485-1   PA9(TX),PA10(RX),PA12(RTS)
static const Pin uart1_pin[] = {

        {GPIO_A,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_A,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_A,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF7},
};

//485-2   PD5(TX),PD6(RX),PD4(RTS)
static const Pin uart2_pin[] = {

        {GPIO_D,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_D,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_D,PIN4,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF7},
};

//485-3   PB10(RX),PB11(TX),PB14(RTS)
static const Pin uart3_pin[] = {

        {GPIO_B,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_B,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_B,PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF7},
};
//DBG  UART5
static const Pin uart5_pin[] = {

        {GPIO_C,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF5},
        {GPIO_D,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF5},
};

//IIC-1   PB8(SCL),PB9(SDA)
static const Pin iic1_pin[] = {
        {GPIO_B,PIN8,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
        {GPIO_B,PIN9,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
 };

//SPI-1
static const Pin spi1_pin[]={
        {GPIO_A,PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF_NUll},//cs1
        {GPIO_E,PIN6 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF_NUll},//cs2
        {GPIO_C,PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF_NUll},//cs3
        {GPIO_C,PIN0 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF_NUll},//cs4
        {GPIO_C,PIN3 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF_NUll},//cs5
        {GPIO_A,PIN1 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF_NUll},//cs6
        {GPIO_A,PIN5 ,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},//30 PA5SPI1_SCK
        {GPIO_A,PIN6 ,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},//31 PA6SPI1_MISO
        {GPIO_A,PIN7 ,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},//32 PA7/SPI1_MOSI
};

//SPI-2
static const Pin spi2_pin[]={
        {GPIO_B,PIN12,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF_NUll},//cs1
        {GPIO_C,PIN4 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF_NUll},//cs2
        {GPIO_B,PIN0,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF_NUll},//cs3
        {GPIO_E,PIN7 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF_NUll},//cs4
        {GPIO_E,PIN9 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF_NUll},//cs5
        {GPIO_E,PIN11 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF_NUll},//cs6
        {GPIO_B,PIN13 ,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},//30 PA5SPI1_SCK
        {GPIO_B,PIN14 ,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},//31 PA6SPI1_MISO
        {GPIO_B,PIN15 ,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},//32 PA7/SPI1_MOSI
};


//SPI-3
static const Pin spi3_pin[]={
        {GPIO_A,PIN4,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF_NUll},//cs1
        {GPIO_E,PIN15 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF_NUll},//cs2
        {GPIO_D,PIN9,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF_NUll},//cs3
        {GPIO_D,PIN11 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF_NUll},//cs4
        {GPIO_D,PIN13 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF_NUll},//cs5
        {GPIO_D,PIN15 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF_NUll},//cs6
        {GPIO_B,PIN3 ,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF6},//30 PA5SPI1_SCK       //SPI3的复用管脚设置错误，导致查了很久！！！！
        {GPIO_B,PIN4 ,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF6},//31 PA6SPI1_MISO
        {GPIO_B,PIN5 ,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF6},//32 PA7/SPI1_MOSI
};


//MAX14001-COUT管脚部分
static const Pin MAX14001_COUT[] =
{
        {GPIO_E,PIN3,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,AF_NUll},   //COUT1  PE3
        {GPIO_E,PIN5,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,AF_NUll},   //COUT2  PE5
        {GPIO_C,PIN14,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,AF_NUll},  //COUT3  PC14
        {GPIO_F,PIN10,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,AF_NUll},  //COUT4  PF10
        {GPIO_C,PIN2,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,AF_NUll},   //COUT5  PC2
        {GPIO_A,PIN0,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,AF_NUll},   //COUT6  PA0
        {GPIO_A,PIN3,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,AF_NUll},   //COUT7  PA3
        {GPIO_F,PIN4,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,AF_NUll},   //COUT8  PF4
        {GPIO_C,PIN5,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,AF_NUll},   //COUT9  PC5
        {GPIO_B,PIN1,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,AF_NUll},   //COUT10 PB1
        {GPIO_E,PIN8,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,AF_NUll},   //COUT11 PE8
        {GPIO_E,PIN10,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,AF_NUll},  //COUT12 PE10
        {GPIO_E,PIN13,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,AF_NUll},  //COUT13 PE13
        {GPIO_E,PIN14,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,AF_NUll},  //COUT14 PE14
        {GPIO_D,PIN8,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,AF_NUll},   //COUT15 PD8
        {GPIO_D,PIN10,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,AF_NUll},  //COUT16 PD10
        {GPIO_D,PIN12,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,AF_NUll},  //COUT17 PD12
        {GPIO_D,PIN14,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,AF_NUll},  //COUT18 PD14
};

// ============================================================================
// 功能：设置串口编号为SerialNo的串口为半双功发送功能
// 参数：SerialNo,串口号
// 返回：无
// ============================================================================
void Board_UartHalfDuplexSend(u8 SerialNo)
{
    switch(SerialNo)
    {
    case CN_UART1:
//        GPIO_SettoHigh(GPIO_A,PIN12);
        break;
    case CN_UART2:
//        GPIO_SettoHigh(GPIO_D,PIN4);
        break;
    case CN_UART3:
//        GPIO_SettoHigh(GPIO_B,PIN14);
        break;
    default:        break;
    }
}

// ============================================================================
// 功能：设置串口编号为SerialNo的串口为半双功接收功能
// 参数：SerialNo,串口号
// 返回：无
// ============================================================================
void Board_UartHalfDuplexRecv(u8 SerialNo)
{
//    拉低为485接收I
    switch(SerialNo)
    {
    case CN_UART1:
//        GPIO_SettoLow(GPIO_A,PIN12);
        break;
    case CN_UART2:
//        GPIO_SettoLow(GPIO_D,PIN4);
        break;
    case CN_UART3:
//        GPIO_SettoLow(GPIO_B,PIN14);
        break;
    default:        break;
    }
}

// =============================================================================
// 功能：Board_GpioInit板上所有用到的GPIO初始化
//    在这里将所有可能用到的复用模式的gpio定义在这里需要用取消注释即可
//    建议不要删除
// 参数：无
// 返回：无
// =============================================================================
void Board_GpioInit(void)
{
     PIO_Configure(MAX14001_COUT, PIO_LISTSIZE(MAX14001_COUT));  //MAX14001-COUT管脚部分

     PIO_Configure(uart1_pin, PIO_LISTSIZE(uart1_pin));  //485-1
     PIO_Configure(uart2_pin, PIO_LISTSIZE(uart2_pin));  //485-2
     PIO_Configure(uart3_pin, PIO_LISTSIZE(uart3_pin));  //485-3
     PIO_Configure(uart5_pin, PIO_LISTSIZE(uart5_pin));  //debug  UART5

     PIO_Configure(spi1_pin, PIO_LISTSIZE(spi1_pin));    //SPI1
     PIO_Configure(spi2_pin, PIO_LISTSIZE(spi2_pin));    //SPI2
     PIO_Configure(spi3_pin, PIO_LISTSIZE(spi3_pin));    //SPI3

     //初始化时把3x6块板子的片选全部拉高拉高，等到配每个芯片时，
     //它会自动拉低，然后又拉高。保证配单个器件时，其他器件都是高电平
     for(u8 i=0;i<6;i++)
         PIO_Set(&spi1_pin[i]);
     for(u8 i=0;i<6;i++)
         PIO_Set(&spi2_pin[i]);
     for(u8 i=0;i<6;i++)
         PIO_Set(&spi3_pin[i]);


     PIO_Set(&uart1_pin[2]);
     PIO_Set(&uart2_pin[2]);
     PIO_Set(&uart3_pin[2]);

     IIC_Busfree(GPIO_B,PIN9,PIN8);  //iic1,调用IIC_Busfree(u32 port,u32 sda_pin,u32 sck_pin)
     PIO_Configure(iic1_pin, PIO_LISTSIZE(iic1_pin));    //iic1
}

//level = 0 使能
#include "cpu_peri_spi.h"
bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level)
{
    switch(SPIPort)
    {
        case CN_SPI1:
            if(level)
                PIO_Set(&spi1_pin[cs]);
            else
                PIO_Clear(&spi1_pin[cs]);
            break;
        case CN_SPI2:
            if(level)
                PIO_Set(&spi2_pin[cs]);
            else
                PIO_Clear(&spi2_pin[cs]);
            break;
        case CN_SPI3:
            if(level)
                PIO_Set(&spi3_pin[cs]);
            else
                PIO_Clear(&spi3_pin[cs]);
            break;
        default:
            return false;
    }
    return true;
}

