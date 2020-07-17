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

// 文件名     ：board.c
// 模块描述: 板件相关部分初始化或配置等
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 5/11.2015
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "os.h"
#include "cpu_peri.h"
#include "uartctrl.h"
#include "pcf8574.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"board config"//板件特性驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                       //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:required                     //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                      //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                     //初始化时机，可选值：early，medium，later, pre-main。
                                      //表示初始化时间，分别是早期、中期、后期
//dependence:"kernel","stm32f4","cpu onchip gpio"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                      //选中该组件时，被依赖组件将强制选中，
                                      //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"               //该组件的弱依赖组件名（可以是none，表示无依赖组件），
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
//%$#@enum,true,flase,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

// =============================================================================
// 功能：根据具体的板件配置串口的GPIO的引脚功能，这是与板件相关，所以该函数放在该文件，CPU
//      串口驱动直接调用该函数来初始化串口的GPIO引脚
//      主要包括GPIO端口和串口时钟使能、GPIO配置、重映射、时钟等
// 参数：无
// 返回：true
// =============================================================================
bool_t Board_UartGpioInit(u8 SerialNo)
{
    //初始化IO端口位uart功能
    //Ports  :  GPA10 GPA9 GPA8 GPA7 GPA6 GPA5 GPA4 GPA3 GPA2 GPA1 GPA0
    //Signal :   RXD1 TXD1  xx   xx   xx   xx   xx   xx   xx   xx   xx
    switch(SerialNo)
    {
    case CN_UART1:
        RCC->AHB1ENR|=1<<0;     //使能PORTA口时钟
        RCC->APB2ENR|=1<<4;     //使能串口1时钟
        GPIO_CfgPinFunc(GPIO_A,PIN9|PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,复用功能,上拉输出
        GPIO_AFSet(GPIO_A,9,7); //PA9,AF7
        GPIO_AFSet(GPIO_A,10,7);//PA10,AF7
        break;
    case CN_UART2:
        RCC->APB1ENR |= (1<<17);
        RCC->AHB1ENR |= (1<<0);//port A clock enable
        GPIO_CfgPinFunc(GPIO_A,PIN2|PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,复用功能,上拉输出
        GPIO_AFSet(GPIO_A,2,7); //PA2,AF7
        GPIO_AFSet(GPIO_A,3,7);//PA3,AF7
        break;
    case CN_UART3:
        RCC->APB1ENR |= (1<<18);//uart3 clock enable
        RCC->AHB1ENR |= (1<<1);// port b clock enable
        GPIO_CfgPinFunc(GPIO_B,PIN10|PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);
        GPIO_AFSet(GPIO_B,10,7);    //PB10,AF7
        GPIO_AFSet(GPIO_B,11,7);//PB11,AF7
        break;
    case CN_UART4:
        RCC->APB1ENR |= (1<<19);//uart4 clock enable
        RCC->AHB1ENR |= (1<<0);//port A clock enable
        GPIO_CfgPinFunc(GPIO_A,PIN0|PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,复用功能,上拉输出
        GPIO_AFSet(GPIO_A,0,7); //PA2,AF7
        GPIO_AFSet(GPIO_A,1,7);//PA3,AF7
        break;
    case CN_UART5:
        RCC->APB1ENR |= (1<<20);//uart5 clock enable
        RCC->AHB1ENR |= (1<<2);//port C clock enable
        GPIO_CfgPinFunc(GPIO_C,PIN11|PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,复用功能,上拉输出
        GPIO_AFSet(GPIO_C,11,7);    //PA2,AF7
        GPIO_AFSet(GPIO_C,12,7);//PA3,AF7
        break;
    case CN_UART6:
        RCC->APB2ENR|= (1<<5);      //使能串口6时钟
        RCC->AHB1ENR |= (1<<2);//port C clock enable
        GPIO_CfgPinFunc(GPIO_C,PIN6|PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,复用功能,上拉输出
        GPIO_AFSet(GPIO_C,6,7); //PA2,AF7
        GPIO_AFSet(GPIO_C,7,7);//PA3,AF7
        break;
    default:
        break;
    }
    return true;
}

// =============================================================================
// 功能：根据具体的板件配置IIC的GPIO的引脚功能，这是与板件相关，所以该函数放在该文件，CPU
//      IIC驱动直接调用该函数来初始化串口的GPIO引脚
//      主要包括GPIO端口和外设时钟使能、GPIO配置、重映射、时钟等
// 参数：无
// 返回：true
// =============================================================================
bool_t Board_IicGpioInit(u8 I2Cx)
{

    return true;
}
// =============================================================================
// 功能：网络相关的gpio配置
// 参数：无
// 返回：true
// =============================================================================

bool_t Board_EthGpioInit(void)
{
    GPIO_PowerOn(GPIO_A);
    GPIO_PowerOn(GPIO_C);
    GPIO_PowerOn(GPIO_D);
    GPIO_PowerOn(GPIO_G);
    RCC->APB2ENR|=1<<14;    //使能SYSCFG时钟
    SYSCFG->PMC|=1<<23;     //使用RMII接口




    GPIO_CfgPinFunc(GPIO_A,PIN1|PIN2|PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_NONE);
    GPIO_CfgPinFunc(GPIO_D,PIN3,GPIO_MODE_OUT,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_NONE);
    GPIO_CfgPinFunc(GPIO_C,PIN1|PIN4|PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_PU);
    GPIO_CfgPinFunc(GPIO_G,PIN11|PIN13|PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_PU);

    GPIO_AFSet(GPIO_A,1,11);    //PA1,AF11
    GPIO_AFSet(GPIO_A,2,11);    //PA2,AF11
    GPIO_AFSet(GPIO_A,7,11);    //PA7,AF11

    GPIO_AFSet(GPIO_C,1,11);    //PC1,AF11
    GPIO_AFSet(GPIO_C,4,11);    //PC4,AF11
    GPIO_AFSet(GPIO_C,5,11);    //PC5,AF11

    GPIO_AFSet(GPIO_G,11,11);   //PG11,AF11
    GPIO_AFSet(GPIO_G,13,11);   //PG13,AF11
    GPIO_AFSet(GPIO_G,14,11);   //PG14,AF11

    GPIO_SettoLow(GPIO_D,PIN3);
    DJY_EventDelay(10*1000);
    GPIO_SettoHigh(GPIO_D,PIN3);
    return true;
}
// =============================================================================
// 功能：根据具体的板件配置SPI的GPIO的引脚功能，这是与板件相关，所以该函数放在该文件，CPU
//      SPI驱动直接调用该函数来初始化串口的GPIO引脚
//      主要包括GPIO端口和外设时钟使能、GPIO配置、重映射、时钟等
// 参数：无
// 返回：true
// =============================================================================
bool_t Board_SpiGpioInit(u8 SPIx)
{
    if(SPIx == CN_SPI1)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // 使能PORTB时钟
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // 使能SPI1时钟

        RCC->AHB1ENR |= 1<<6; // 使能PORTG时钟

        RCC->APB2RSTR |= RCC_APB2RSTR_SPI1; // 复位SPI1
        RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1; // 停止复位SPI1

        GPIO_CfgPinFunc(GPIO_B,PIN4|PIN5|PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU); // GPB3、4和5为SCK、MISO和MOSI
        GPIO_AFSet(GPIO_B,3,5);
        GPIO_AFSet(GPIO_B,4,5);
        GPIO_AFSet(GPIO_B,5,5);
#if 0 // CS由设备确定
        GPIO_CfgPinFunc(GPIO_B,PIN14,GPIO_MODE_OUT,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU); // GPB14为CS用于NOR

        GPIO_CfgPinFunc(GPIOG,PIN7,GPIO_MODE_OUT,GPIO_OTYPE_PP,
                GPIO_SPEED_100M,GPIO_PUPD_PU); // GPG7为CS用于无线模式
        GPIOG->ODR|=1<<7;

        GPIO_SettoHigh(GPIO_B,PIN14);// CS为高
#endif
    }
    else
    {
        return false;
    }
    return true;
}

// =============================================================================
// 功能：根据具体的板件485所需的控制管脚，控制485为发送状态该板件由硬件电路实现无需控制
// 参数：串口号如：CN_UART1
// 返回：无
// =============================================================================

void Board_UartHalfDuplexSend(u8 SerialNo)
{
    switch(SerialNo)
    {
        case CN_UART1:break;
        case CN_UART2:break;
        default:      break;
    }
}

// ============================================================================
// 功能：获取端口的类型RS_485/RS_232 RS485 返回false
// 参数：SerialNo,串口号
// 返回：true/false
// ============================================================================
u8   Board_CheckPortIsRS232(u8 SerialNo)
{

    if(SerialNo==CN_UART1||SerialNo==CN_UART2)
        return false;
    return true;
}

 // =============================================================================
 // 功能：根据具体的板件485所需的控制管脚，控制485为接受收状态
 // 参数：串口号如：CN_UART1
 // 返回：无
 // =============================================================================

void Board_UartHalfDuplexRecv(u8 SerialNo)
{

    switch(SerialNo)
    {
    case CN_UART1: break;
    case CN_UART2: break;
    default:       break;
    }
}

// =============================================================================
// 功能：根据端口号控制片选
// 参数：SPIPort端口号  level==0使能使能或  1失能
// 返回：true
// =============================================================================
#define BITBAND(addr, bitnum)       ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)              *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)      MEM_ADDR(BITBAND(addr, bitnum))
#define GPIOB_ODR_Addr                (GPIOB_BASE+20) //0x40020414
#define PAout(n)                       BIT_ADDR(GPIOA_ODR_Addr,n)  //输出
#define PBout(n)                       BIT_ADDR(GPIOB_ODR_Addr,n)  //输出

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level)
{
    switch(SPIPort)
    {
        case CN_SPI1:
            if(level)
                PBout(14) = 1;
            else
                PBout(14) = 0;
            break;
        default:  return false;
    }
    return true;
}
#pragma GCC diagnostic pop

static void Board_NORGpioInit (void )
{

    Board_SpiGpioInit(CN_SPI1);
    GPIO_CfgPinFunc(GPIO_B, PIN14, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_50M,
                    GPIO_PUPD_PU); // GPB14为CS用于NOR

    RCC->AHB1ENR |= 1<<6; // 使能PORTG时钟
    GPIO_CfgPinFunc(GPIOG,PIN7, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_100M,
                    GPIO_PUPD_PU); // GPG7为CS用于无线模式
    GPIOG->ODR |= 1<<7;

    GPIO_SettoHigh(GPIO_B,PIN14);// CS为高
}
// =============================================================================
// 功能：Board_GpioInit板上所有用到的GPIO初始化
//  在这里将所有可能用到的复用模式的gpio定义在这里需要用取消注释即可
// 参数：无
// 返回：无
// =============================================================================

void Board_Init(void)
{
    Board_EthGpioInit();//网络
    Board_UartGpioInit(CN_UART1);
//    Board_UartGpioInit(CN_UART2);
    Board_NORGpioInit();
}

////网口的 PHY 芯片：LAN8720复位
bool_t LAN8720_RESET(void)
{
//  PCF8574_WriteBit(ETH_RESET_IO,1);
//  DJY_DelayUs(100*mS);
//  PCF8574_WriteBit(ETH_RESET_IO,0);
//  DJY_DelayUs(100*mS);
    //PD3上输出一个100mS的高电平脉冲
    return true;
}

