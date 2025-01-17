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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// =============================================================================

// 文件名     ：cpu_peri_uart.c
// 模块描述: DJYOS串口模块的底层驱动部分，主要实现寄存器级别的操作，如中断等
// 模块版本: V1.10
// 创建人员:
// 创建时间:
// =============================================================================
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <systime.h>
#include "cpu_peri.h"
#include <device/djy_uart.h>
#include "int.h"
#include "djyos.h"
#include "cpu_peri_uart.h"
#include "cpu_peri_int_line.h"
#include "stdlib.h"

//#include "cpu_peri_dma.h"
// =============================================================================
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern ptu32_t ModuleInstall_UART(ptu32_t SerialNo);
//
//    #if CFG_UART1_ENABLE ==1
//    ModuleInstall_UART(CN_UART1);
//    #endif
//
//    #if CFG_UART2_ENABLE ==1
//    ModuleInstall_UART(CN_UART2);
//    #endif
//
//    #if CFG_UART3_ENABLE ==1
//    ModuleInstall_UART(CN_UART3);
//    #endif
//
//    #if CFG_UART4_ENABLE ==1
//    ModuleInstall_UART(CN_UART4);
//    #endif
//
//    #if CFG_UART5_ENABLE ==1
//    ModuleInstall_UART(CN_UART5);
//    #endif
//
//    #if CFG_UART6_ENABLE ==1
//    ModuleInstall_UART(CN_UART6);
//    #endif
//
//    #if CFG_UART7_ENABLE ==1
//    ModuleInstall_UART(CN_UART7);
//    #endif
//
//    #if CFG_UART8_ENABLE ==1
//    ModuleInstall_UART(CN_UART8);
//    #endif
//
//    #if CFG_UART9_ENABLE ==1
//    ModuleInstall_UART(CN_UART9);
//    #endif
//
//    #if CFG_UART10_ENABLE ==1
//    ModuleInstall_UART(CN_UART10);
//    #endif

//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip uart"//CPU的uart外设驱动
//parent:"uart device file"      //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                    //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                 //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                   //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                  //初始化时机，可选值：early，medium，later, pre-main。
                                   //表示初始化时间，分别是早期、中期、后期
//dependence:"uart device file","cpu onchip dma"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                   //选中该组件时，被依赖组件将强制选中，
                                   //如果依赖多个组件，则依次列出
//weakdependence:"none"            //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                   //选中该组件时，被依赖组件不会被强制选中，
                                   //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                   //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_UART == false )
//#warning  " cpu_onchip_uart  组件参数未配置，使用默认配置"
//%$#@target = header    //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_UART    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,1024,
#define CFG_UART1_SENDBUF_LEN       64      //"UART1发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN       64      //"UART1接收环形缓冲区大小",
#define CFG_UART1_DMABUF_LEN        64      //"UART1 DMA环形缓冲区大小",

#define CFG_UART2_SENDBUF_LEN       64      //"UART2发送环形缓冲区大小",
#define CFG_UART2_RECVBUF_LEN       64      //"UART2接收环形缓冲区大小",
#define CFG_UART2_DMABUF_LEN        64      //"UART2 DMA环形缓冲区大小",

#define CFG_UART3_SENDBUF_LEN       64      //"UART3发送环形缓冲区大小",
#define CFG_UART3_RECVBUF_LEN       64      //"UART3接收环形缓冲区大小",
#define CFG_UART3_DMABUF_LEN        1024      //"UART3 DMA环形缓冲区大小",

#define CFG_UART4_SENDBUF_LEN       64      //"UART4发送环形缓冲区大小",
#define CFG_UART4_RECVBUF_LEN       64      //"UART4接收环形缓冲区大小",
#define CFG_UART4_DMABUF_LEN        64      //"UART4 DMA环形缓冲区大小",

#define CFG_UART5_SENDBUF_LEN       64      //"UART5发送环形缓冲区大小",
#define CFG_UART5_RECVBUF_LEN       64      //"UART5接收环形缓冲区大小",
#define CFG_UART5_DMABUF_LEN        64      //"UART5 DMA环形缓冲区大小",

#define CFG_UART6_SENDBUF_LEN       64      //"UART6发送环形缓冲区大小",
#define CFG_UART6_RECVBUF_LEN       64      //"UART6接收环形缓冲区大小",
#define CFG_UART6_DMABUF_LEN        64      //"UART6 DMA环形缓冲区大小",

#define CFG_UART7_SENDBUF_LEN       64      //"UART7发送环形缓冲区大小",
#define CFG_UART7_RECVBUF_LEN       64      //"UART7接收环形缓冲区大小",
#define CFG_UART7_DMABUF_LEN        64      //"UART7 DMA环形缓冲区大小",

#define CFG_UART8_SENDBUF_LEN       64      //"UART8发送环形缓冲区大小",
#define CFG_UART8_RECVBUF_LEN       64      //"UART8接收环形缓冲区大小",
#define CFG_UART8_DMABUF_LEN        64      //"UART8 DMA环形缓冲区大小",

#define CFG_UART9_SENDBUF_LEN       64      //"UART9发送环形缓冲区大小",
#define CFG_UART9_RECVBUF_LEN       64      //"UART9接收环形缓冲区大小",
#define CFG_UART9_DMABUF_LEN        64      //"UART9 DMA环形缓冲区大小",

#define CFG_UART10_SENDBUF_LEN      64      //"UART10发送环形缓冲区大小",
#define CFG_UART10_RECVBUF_LEN      64      //"UART10接收环形缓冲区大小",
#define CFG_UART10_DMABUF_LEN       64      //"UART10 DMA环形缓冲区大小",

//%$#@enum,true,false
#define CFG_UART1_ENABLE           false        //"是否使用UART1",
#define CFG_UART1_ENABLE_DMA       false        //"UART1使能DMA",
#define CFG_UART2_ENABLE           false       //"是否使用UART2",
#define CFG_UART2_ENABLE_DMA       false       //"UART2使能DMA",
#define CFG_UART3_ENABLE           false       //"是否使用UART3",
#define CFG_UART3_ENABLE_DMA       false       //"UART3使能DMA",
#define CFG_UART4_ENABLE           false        //"是否使用UART4",
#define CFG_UART4_ENABLE_DMA       false       //"UART4使能DMA",
#define CFG_UART5_ENABLE           false       //"是否使用UART5",
#define CFG_UART5_ENABLE_DMA       false       //"UART5使能DMA",
#define CFG_UART6_ENABLE           false       //"是否使用UART6",
#define CFG_UART6_ENABLE_DMA       false       //"UART6使能DMA",
#define CFG_UART7_ENABLE           false       //"是否使用UART7",
#define CFG_UART7_ENABLE_DMA       false       //"UART7使能DMA",
#define CFG_UART8_ENABLE           false       //"是否使用UART8",
#define CFG_UART8_ENABLE_DMA       false       //"UART8使能DMA",
#define CFG_UART9_ENABLE           false       //"是否使用UART9",
#define CFG_UART9_ENABLE_DMA       false       //"UART9使能DMA",
#define CFG_UART10_ENABLE          false       //"是否使用UART10",
#define CFG_UART10_ENABLE_DMA      false       //"UART10使能DMA",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束
//@#$%component end configure
// =============================================================================

#define CN_UART1_BASE USART1_BASE//0x40013800
#define CN_UART2_BASE USART2_BASE
#define CN_UART3_BASE USART3_BASE
#define CN_UART4_BASE UART4_BASE
#define CN_UART5_BASE UART5_BASE
#define CN_UART6_BASE USART6_BASE
#define CN_UART7_BASE UART7_BASE
#define CN_UART8_BASE UART8_BASE
#define CN_UART9_BASE UART9_BASE
#define CN_UART10_BASE USART10_BASE

static u32 TxByteTime;                    //正常发送一个字节所需要的时间
static tagUartReg *PutStrDirectReg;     //用于printk发送的串口寄存器
static tagUartReg *GetCharDirectReg;     //用于直接接收的串口寄存器
static u32 TxDirectPort;                  //用于printk发送的串口号
static u32 RxDirectPort;                  //用于直接接收的串口号
// =============================================================================
//usart和uart，寄存器结构都是 USART_TypeDef
static tagUartReg volatile * const tg_UART_Reg[] = {(tagUartReg *)CN_UART1_BASE,
                                                    (tagUartReg *)CN_UART2_BASE,
                                                    (tagUartReg *)CN_UART3_BASE,
                                                    (tagUartReg *)CN_UART4_BASE,
                                                    (tagUartReg *)CN_UART5_BASE,
                                                    (tagUartReg *)CN_UART6_BASE,
                                                    (tagUartReg *)CN_UART7_BASE,
                                                    (tagUartReg *)CN_UART8_BASE,
                                                    (tagUartReg *)CN_UART9_BASE,
                                                    (tagUartReg *)CN_UART10_BASE,};
#define CN_DMA_UNUSED   0
#define CN_DMA_USED     1

// UART对应的DMA Stream中断号
static u8 UartDmaRxInt[10];

static u8 UartDmaTxInt[10];

// DMA正在使用标记，是否使用DMA标记
static bool_t s_UART_DmaSending[10]={false,false,false,false,false,false,false,false,false,false};
static bool_t s_UART_DmaUsed[10];

static u32 s_u32DmaRecvIdleOffset[CN_UART_NUM];     //记录当前buffer中在Idle中断里已读的偏移

static DMA_HandleTypeDef *pUART_DmaSendStream[CN_UART_NUM];
static DMA_HandleTypeDef *pUART_DmaRecvStream[CN_UART_NUM];

static u8 *pUART_DmaSendBuf[CN_UART_NUM];
static u8 *pUART_DmaRecvBuf[CN_UART_NUM];
static u32 s_u32UART_DmaBufLen[CN_UART_NUM];

static struct UartGeneralCB *pUartCB[CN_UART_NUM];

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

__attribute__((weak))  void Board_UartHalfDuplexSend(u8 SerialNo)
{
    return;
}
__attribute__((weak))  void Board_UartHalfDuplexRecv(u8 SerialNo)
{
    return ;
}
__attribute__((weak))  void UART_InLowPowerPinCfg(u8 SerialNo)
{
    return;
}
__attribute__((weak))  void UART_OutLowPowerPinCfg(u8 SerialNo)
{
    return ;
}
#pragma GCC diagnostic pop

// =============================================================================
static ptu32_t UART_ISR(ptu32_t port);
static uint32_t UART_DmaRx_ISR(ptu32_t port);
static uint32_t UART_DmaTx_ISR(ptu32_t port);

// =============================================================================
// 功能： 进入或退出低功耗模式
//     时钟使能 控制器使能 GPIO设置相应的状态
// 参数： port 串口号：如 CN_UART1
//     flag 进入低功耗 或退出低功耗标志：如 UART_IN_LOWPOWER
// 返回：true/false
// =============================================================================
//bool_t UART_LowPowerConfig(u8 port,u8 flag)
//{
//    if(port>=CN_UART_NUM)
//        return false;
//    if(flag == InLowPower)
//    {
//        switch(port)
//        {
//             case CN_UART1: RCC->APB2ENR &=~(1<<4); break;
//             case CN_UART2: RCC->APB1ENR &=~(1<<17);break;
//             case CN_UART3: RCC->APB1ENR &=~(1<<18);break;
//             case CN_UART4: RCC->APB1ENR &=~(1<<19);break;
//             case CN_UART5: RCC->APB1ENR &=~(1<<20);break;
//             case CN_UART6: RCC->APB2ENR &=~(1<<5); break;
//             case CN_UART7: RCC->APB1ENR &=~(1<<30);break;
//             case CN_UART8: RCC->APB1ENR &=~(1<<31);break;
//            default: break;
//        }
//        tg_UART_Reg[port]->CR1 &=~ (0x1);
//        UART_InLowPowerPinCfg(port);
//        return true;
//    }
//    else if (flag == OutLowPower)
//    {
//        switch(port)
//            {
//            case CN_UART1: __HAL_RCC_USART1_CLK_ENABLE(); break;
//            case CN_UART2: __HAL_RCC_USART2_CLK_ENABLE();break;
//            case CN_UART3: __HAL_RCC_USART3_CLK_ENABLE();break;
//            case CN_UART4: __HAL_RCC_UART4_CLK_ENABLE();break;
//            case CN_UART5: __HAL_RCC_UART5_CLK_ENABLE();break;
//            case CN_UART6: __HAL_RCC_USART6_CLK_ENABLE(); break;
//            case CN_UART7: __HAL_RCC_UART7_CLK_ENABLE();break;
//            case CN_UART8: __HAL_RCC_UART8_CLK_ENABLE();break;
//            default: break;
//        }
//        tg_UART_Reg[port]->CR1 |= (0x1);
//        UART_OutLowPowerPinCfg(port);
//        return true;
//    }
//   return false;
//}

// =============================================================================
// 功能: 使能uart的接收中断。
// 参数: flag,是否使用dma传输
//       port,串口号，0~9，对应串口1~10
// 返回: 无
// =============================================================================
void __UART_RxIntEnable(u8 flag,u8 port)
{
    if(flag == CN_DMA_USED)
        Int_RestoreAsynLine(UartDmaRxInt[port]);
    else
        tg_UART_Reg[port]->CR1 |= (1<<5);//中断使能
}

// =============================================================================
// 功能: 禁止uart的接收中断
// 参数: reg,被操作的寄存器组指针
//       port,串口号，0~9，对应串口1~10
// 返回: 无
// =============================================================================
void __UART_RxIntDisable(u8 flag, u8 port)
{
    if(flag == CN_DMA_USED)
        Int_SaveAsynLine(UartDmaRxInt[port]);
    else
        tg_UART_Reg[port]->CR1 &= ~(1<<5);
}

// =============================================================================
// 功能: 使能uart的发送中断
// 参数: reg,被操作的寄存器组指针
//       port,串口号，0~9，对应串口1~10
// 返回: 无
// =============================================================================
static void __UART_TxIntEnable(u8 flag,u8 port)
{
    if(flag == CN_DMA_USED)
        Int_RestoreAsynLine(UartDmaTxInt[port]);
    else
        tg_UART_Reg[port]->CR1 |= USART_CR1_TXEIE_TXFNFIE;
}

// =============================================================================
// 功能: 禁止UART的发送中断
// 参数: Reg,被操作的寄存器组指针
//       port,串口号，0~9，对应串口1~10
// 返回: 无
// =============================================================================
static void __UART_TxIntDisable(u8 flag, u8 port)
{
    if(flag == CN_DMA_USED)
        Int_SaveAsynLine(UartDmaTxInt[port]);
    else
        tg_UART_Reg[port]->CR1 &= ~USART_CR1_TXEIE_TXFNFIE;//禁止发送空中断
}

// =============================================================================
// 功能: 检查发送寄存器的状态，
// 参数：reg,被操作的寄存器组指针
// 返回: true = 缓冲区空，false = 非空
// =============================================================================
static bool_t __UART_TxTranEmpty(tagUartReg volatile *reg)
{
    return ((reg->ISR & USART_ISR_TXE_TXFNF) !=0);
}

bool_t UART_WaitTxEnd(u8 port)
{
    s32 timeout = 10000;
    volatile tagUartReg *Reg;
    Reg = (tagUartReg *)tg_UART_Reg[port];
    while((false == __UART_TxTranEmpty(Reg))&& (timeout > 10))
    {
        timeout -=10;
        DJY_DelayUs(10);
    }
    return true;
}


// =============================================================================
// 功能: 检查接收寄存器的状态，有数据就返回true，否则返回false
// 参数：reg,被操作的寄存器组指针
// 返回: true = 有数据，false = 无数据
// =============================================================================
static bool_t __UART_RxHadChar(tagUartReg volatile *reg)
{
    return ((reg->ISR & (1<<5)) !=0);
}

// =============================================================================
// 功能: 设置串口baud,PCLK为25M，CPU主频为216M计算,该驱动只提供9600、19200、57600、
//       115200四种波特率设置
// 参数: Reg,被操作的寄存器组指针
//       port,串口号
//       baud,波特率
// 返回: 无
// =============================================================================
static void __UART_BaudSet(tagUartReg volatile *Reg,u32 port,u32 baud)
{
    u32 mantissa,fraction;
    float temp;
    if((port == CN_UART1) || (port == CN_UART6))
    {
        temp = (float)CN_CFG_PCLK2/(16*baud);
        mantissa = temp;
        fraction = (temp - mantissa)*16;
    }
    else
    {
        temp = (float)CN_CFG_PCLK1/(16*baud);
        mantissa = temp;
        fraction = (temp - mantissa)*16;
    }

    Reg->BRR =  ((u16)mantissa<<4)+(u16)fraction;

    if(TxDirectPort == port)
    {
        TxByteTime = 11000000/baud;     //1个字节传输时间，按10bit，+10%计算
    }
}

// =============================================================================
// 功能: 设置对应UART的IO口，包括时钟和IO配置
// 参数: SerialNo,串口号
// 注 :因串口复用引脚不确定性在需要初始化需要的引脚
// 返回: 无
// =============================================================================
static void __UART_ClkConfig(u8 SerialNo)
{

    switch(SerialNo)
    {
    case CN_UART1:
        __HAL_RCC_USART1_CLK_ENABLE();//串口时钟使能
        break;
    case CN_UART2:
        __HAL_RCC_USART2_CLK_ENABLE();
        break;
    case CN_UART3:
        __HAL_RCC_USART3_CLK_ENABLE();

        break;
    case CN_UART4:
        __HAL_RCC_UART4_CLK_ENABLE();
        break;
    case CN_UART5:
        __HAL_RCC_UART5_CLK_ENABLE();
        break;
    case CN_UART6:
        __HAL_RCC_USART6_CLK_ENABLE();
        break;
    case CN_UART7:
        __HAL_RCC_UART7_CLK_ENABLE();
        break;
    case CN_UART8:
        __HAL_RCC_UART8_CLK_ENABLE();
        break;
    case CN_UART9:
        __HAL_RCC_UART9_CLK_ENABLE();
        break;
    case CN_UART10:
        __HAL_RCC_USART10_CLK_ENABLE();
        break;
    default: break;

    }

}

// =============================================================================
// 功能: 对串口传输参数配置，包括波特率、奇偶校验、数据位、停止位
// 参数:  Reg,被操作的寄存器组指针
//        port,串口号
//        data,结构体tagCOMParam类型的指针数值
// 返回: 无
// =============================================================================
static void __UART_ComConfig(tagUartReg volatile *Reg,u32 port,struct COMParam *COM)
{
    if((COM == NULL) || (Reg == NULL))
        return;
    __UART_BaudSet(Reg,port,COM->BaudRate);

    Reg->CR1 &= ~(1);//禁止串口
    switch(COM->DataBits)               // data bits
    {
        case CN_UART_DATABITS_7:
            switch(COM->Parity)
            {
                case CN_UART_PARITY_NONE:
                    Reg->CR1 |= (1<<28);//数据效验共7位禁止奇偶效验
                    Reg->CR1 &= ~(1<<12);

                    Reg->CR1 &= ~(1<<10);
                    break;//奇效验

                case CN_UART_PARITY_ODD:
                    Reg->CR1 &= ~(1<<28);//数据效验共8位奇偶效验
                    Reg->CR1 &= ~(1<<12);

                    Reg->CR1 |= (1<<9);
                    Reg->CR1 |= (1<<10);break;//奇效验

                case CN_UART_PARITY_EVEN:
                    Reg->CR1 &= ~(1<<28);//数据效验共8位奇偶效验
                    Reg->CR1 &= ~(1<<12);

                    Reg->CR1 &=~(1<<9);
                    Reg->CR1 |= (1<<10);break;
                default:break;
            }
            break;

        case CN_UART_DATABITS_8:
            switch(COM->Parity)        //
            {
                case CN_UART_PARITY_NONE:
                    Reg->CR1 &= ~(1<<28);//数据效验共8位禁止奇偶效验
                    Reg->CR1 &= ~(1<<12);
                    Reg->CR1 &= ~(1<<10);
                    break;

                case CN_UART_PARITY_ODD:
                    Reg->CR1 &= ~(1<<28);//数据效验共9位奇偶效验
                    Reg->CR1 |= (1<<12);

                    Reg->CR1 |= (1<<9);
                    Reg->CR1 |= (1<<10);
                    break;//奇效验
                case CN_UART_PARITY_EVEN:
                    Reg->CR1 &= ~(1<<28);//数据效验共9位奇偶效验
                    Reg->CR1 |= (1<<12);

                    Reg->CR1 &=~(1<<9);
                    Reg->CR1 |= (1<<10);break;
                default:break;
            }
        break;

        case CN_UART_DATABITS_9:
            Reg->CR1 &= ~(1<<28);
            Reg->CR1 |= (1<<12);
            switch(COM->Parity)        //
            {
                case CN_UART_PARITY_NONE:
                    Reg->CR1 &= ~(1<<28);//数据效验共9位禁止奇偶效验
                    Reg->CR1 |= (1<<12);
                    Reg->CR1 &= ~(1<<10);
                    break;
                case CN_UART_PARITY_ODD:break;//不支持这种请情况
                case CN_UART_PARITY_EVEN:break;
                default:break;
            }
            break;
        default:break;
    }

    switch(COM->StopBits)  //todu:F7的 0.5个停止位没有写进来
    {
//        Reg->CR2 &= ~(3<<12);
//      case CN_UART_STOPBITS_0_5;  Reg->CR2 |= (1<<12);break;
        case CN_UART_STOPBITS_1:    Reg->CR2 |= (0<<12);break;
        case CN_UART_STOPBITS_1_5:  Reg->CR2 |= (3<<12);break;
        case CN_UART_STOPBITS_2:    Reg->CR2 |= (2<<12);break;
        default:break;
    }
    Reg->CR1 |= (1);//使能串口
}

// =============================================================================
// 功能: 使串口开始工作，时钟和uart模块
// 参数: port,串口号
// 返回: 无
// =============================================================================
void __UART_Enable(u32 port)
{
    switch(port)
    {//使能串口1时钟
        case CN_UART1:
            __HAL_RCC_USART1_CLK_ENABLE();break;
        case CN_UART2:
            __HAL_RCC_USART2_CLK_ENABLE();break;
        case CN_UART3:
            __HAL_RCC_USART3_CLK_ENABLE();break;
        case CN_UART4:
            __HAL_RCC_UART4_CLK_ENABLE();break;
        case CN_UART5:
            __HAL_RCC_UART5_CLK_ENABLE();break;
        case CN_UART6:
            __HAL_RCC_USART6_CLK_ENABLE();break;
        case CN_UART7:
            __HAL_RCC_UART7_CLK_ENABLE();break;
        case CN_UART8:
            __HAL_RCC_UART8_CLK_ENABLE();break;
        case CN_UART9:
            __HAL_RCC_UART9_CLK_ENABLE();break;
        case CN_UART10:
            __HAL_RCC_USART10_CLK_ENABLE();break;
        default:
            break;
    }
    tg_UART_Reg[port]->CR1 |= (1<<13);
}

// =============================================================================
// 功能: 使串口停止工作，包括时钟和uart模块
// 参数: port,串口号
// 返回: 无
// =============================================================================
void __UART_Disable(u32 port)
{
    switch(port)
    {
        case CN_UART1:  __HAL_RCC_USART1_CLK_DISABLE();  break;
        case CN_UART2:  __HAL_RCC_USART2_CLK_DISABLE(); break;
        case CN_UART3:  __HAL_RCC_USART3_CLK_DISABLE(); break;
        case CN_UART4:  __HAL_RCC_UART4_CLK_DISABLE(); break;
        case CN_UART5:  __HAL_RCC_UART5_CLK_DISABLE(); break;
        case CN_UART6:  __HAL_RCC_USART6_CLK_DISABLE();  break;
        case CN_UART7:  __HAL_RCC_UART7_CLK_DISABLE(); break;
        case CN_UART8:  __HAL_RCC_UART8_CLK_DISABLE(); break;
        case CN_UART9:  __HAL_RCC_UART9_CLK_DISABLE(); break;
        case CN_UART10:  __HAL_RCC_USART10_CLK_DISABLE(); break;
        default:break;
    }
    tg_UART_Reg[port]->CR1 &= ~(1);
}

// =============================================================================
// 功能: 初始化UART对应的中断线，并初始化中断入口函数
// 参数：SerialNo,串口号
// 返回: 无
// =============================================================================
static void __UART_IntInit(u32 SerialNo)
{
    u8 IntLine;
    switch (SerialNo)
    {
        case CN_UART1:
                    IntLine = CN_INT_LINE_USART1;break;
        case CN_UART2:
                    IntLine = CN_INT_LINE_USART2;break;
        case CN_UART3:
                    IntLine = CN_INT_LINE_USART3;break;
        case CN_UART4:
                    IntLine = CN_INT_LINE_UART4; break;
        case CN_UART5:
                    IntLine = CN_INT_LINE_UART5; break;
        case CN_UART6:
                    IntLine = CN_INT_LINE_USART6;break;
        case CN_UART7:
                    IntLine = CN_INT_LINE_UART7; break;
        case CN_UART8:
                    IntLine = CN_INT_LINE_UART8; break;
        case CN_UART9:
                    IntLine = CN_INT_LINE_UART9; break;
        case CN_UART10:
                    IntLine = CN_INT_LINE_USART10; break;
                default:
                    return;
    }
    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,UART_ISR);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
    Int_SetIsrPara(IntLine,SerialNo);
}



// =============================================================================
// 功能: 硬件参数配置和寄存器的初始化，包括波特率、停止位、校验位、数据位，默认情况下:
//       波特率:115200  ； 停止位:1 ; 校验:无 ; 数据位:8bit
// 参数: SerialNo,串口号，0~3
// 返回: 无
// =============================================================================
static void __UART_HardInit(u8 SerialNo)
{
    if(SerialNo > CN_UART_NUM)
        return;
    __UART_ClkConfig(SerialNo);
    //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
   //初始化uart硬件控制数据结构
    tg_UART_Reg[SerialNo]->CR1 = 0;
    tg_UART_Reg[SerialNo]->CR3 = USART_CR3_ONEBIT | UART_TXFIFO_THRESHOLD_7_8
                                | UART_RXFIFO_THRESHOLD_1_2     // | USART_CR3_TXFTIE
                                | USART_CR3_RXFTIE;
    tg_UART_Reg[SerialNo]->CR2 = 0x0;
    //  tg_UART_Reg[SerialNo]->CR1 = 0x202d;
    tg_UART_Reg[SerialNo]->CR1 = USART_CR1_UE | USART_CR1_RE | USART_CR1_TE
                                | USART_CR1_MME    //| USART_CR1_RXNEIE_RXFNEIE
                                | USART_CR1_IDLEIE | USART_CR1_FIFOEN
                                | USART_CR1_TXFEIE | USART_CR1_RXFFIE
                                | USART_CR1_TCIE;
    __UART_BaudSet(tg_UART_Reg[SerialNo],SerialNo,115200);
    tg_UART_Reg[SerialNo]->ICR |= 0x40;
}

// =============================================================================
// 功能: 查询等待串口发送完毕，超时返回
// 参数: sending，dma发送方式，串口是否已经发送完成
// 返回: 无
// =============================================================================
bool_t __uart_dma_timeout(bool_t sending)
{
    s32 timeout = 1000;
    while((sending == true)&& (timeout > 0))//超时
    {
        timeout--;
        DJY_DelayUs(1);
    }
    if(timeout == 0)
        return true;
    else
        return false;
}
// =============================================================================
// 功能: 启动串口DMA发送，使用DMA启动，在发送前需判断上次DMA是否已经发送完成，通过
//       查看标志UARTx_DMA_SENDING来判断UARTx是否已经发送完成
// 参数: PORT,串口号.
// 返回: 发送的个数
// =============================================================================
u32 __UART_DMA_SendStart(u32 port)
{
    u32 num;
    u8 *DmaSendBuf;
    u32 DmaSendBufLen;
    DMA_Stream_TypeDef *DMA_Stream;
    DMA_Stream = ((DMA_Stream_TypeDef   *)pUART_DmaSendStream[port]->Instance);
    DmaSendBufLen = s_u32UART_DmaBufLen[port];
    DmaSendBuf = pUART_DmaSendBuf[port];
//    if(true == __uart_dma_timeout(s_UART_DmaSending[port]))
    if(true == s_UART_DmaSending[port])
        return 0;
    num = UART_PortRead(pUartCB[port],DmaSendBuf,DmaSendBufLen);

    if(num > 0)
    {
        s_UART_DmaSending[port] = true;
        /* Configure DMA Stream data length */
        DMA_Stream->NDTR = num;
        /* Configure DMA Stream destination address */
        DMA_Stream->PAR = (uint32_t)&tg_UART_Reg[port]->TDR;
        /* Configure DMA Stream source address */
        DMA_Stream->M0AR = DmaSendBuf;
//        DMA_SetConfig(pUART_DmaSendStream[port],
//                        DmaSendBuf, (uint32_t)&tg_UART_Reg[port]->TDR,num);
        __HAL_DMA_ENABLE(pUART_DmaSendStream[port]);
//        HAL_DMA_Start_IT(pUART_DmaSendStream[port],
//                        DmaSendBuf, (uint32_t)&tg_UART_Reg[port]->TDR,num);
    }
    else
        tg_UART_Reg[port]->CR1 |= USART_CR1_TCIE;   //用于485切换
    return 0;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// =============================================================================
// 功能: 启动串口发送，其目的是触发中断，用中断方式发送数据。
// 参数: Reg,被操作的串口寄存器指针.
// 返回: 发送的个数
// =============================================================================
static u32 __UART_SendStart (ptu32_t MyReg)
{
    tagUartReg *Reg = (tagUartReg *)MyReg;
    u8 port;

    switch((u32)Reg)
    {
    case CN_UART1_BASE:        port = CN_UART1;    break;
    case CN_UART2_BASE:        port = CN_UART2;    break;
    case CN_UART3_BASE:        port = CN_UART3;    break;
    case CN_UART4_BASE:        port = CN_UART4;    break;
    case CN_UART5_BASE:        port = CN_UART5;    break;
    case CN_UART6_BASE:        port = CN_UART6;    break;
    case CN_UART7_BASE:        port = CN_UART7;    break;
    case CN_UART8_BASE:        port = CN_UART8;    break;
    case CN_UART9_BASE:        port = CN_UART9;    break;
    case CN_UART10_BASE:        port = CN_UART10;    break;
    default:return 0;
    }

    Reg->CR1 &= ~USART_CR1_TCIE;    //关TC中断
    Board_UartHalfDuplexSend(port);//切换到发送
//  __UART_TxIntDisable(s_UART_DmaUsed[port],port);
    if(s_UART_DmaUsed[port])
    {
        __UART_DMA_SendStart(port);
    }
    else
    {
        //非DMA方式，只要使能发送空中断，则触发中断
        Reg->CR1 |=USART_CR1_TXFEIE;    //使能 tx fifo 空中断
    }

//  __UART_TxIntEnable(s_UART_DmaUsed[port],port);
    return 1;
}
#pragma GCC diagnostic pop
DMA_HandleTypeDef hdma_usart3_rx,hdma_usart3_tx;
// =============================================================================
// 功能: 设置uart使用dma收发，根据stm32各串口的收发dma通道配置寄存器。将重新初
//       始化所有dma寄存器，因为:
//       1、不使用dma期间可能有其他设备使用dma，从而更改了设置。
//       2、即使uart使用dma期间，也可能软件模块配合的问题导致其他设备使用dma.
// 参数: port,端口号，即串口号，从1~3
// 返回: 无
// =============================================================================
void __UART_SetDmaUsed(u32 port)
{
    struct HeapCB *heap;
    u32 DmaBufLen;

    if(s_UART_DmaUsed[port] == CN_DMA_UNUSED)
        return ;
    heap =Heap_FindHeap("nocache");
    if(heap==NULL)
        return;

    DmaBufLen = s_u32UART_DmaBufLen[port];
    pUART_DmaRecvStream[port] = DMA_Alloc(&UartDmaRxInt[port]);
    pUART_DmaSendStream[port] = DMA_Alloc(&UartDmaTxInt[port]);

    if((pUART_DmaRecvStream[port]==NULL) || (pUART_DmaSendStream[port]==NULL))
    {
        DMA_free(pUART_DmaRecvStream);
        DMA_free(pUART_DmaSendStream);
    }
//  pUART_DmaRecvStream[port] = &hdma_usart3_rx;
//  pUART_DmaSendStream[port] = &hdma_usart3_tx;

    if(pUART_DmaSendBuf[port]==NULL)
    {
        pUART_DmaSendBuf[port] = M_MallocHeap(DmaBufLen,heap,0);
        if(pUART_DmaSendBuf[port] != NULL)
        {
            pUART_DmaRecvBuf[port] = pUART_DmaSendBuf[port] + DmaBufLen;
        }
        else
            return;

    }

    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* USART3_RX Init */
//  pUART_DmaRecvStream[port]->Instance = UartDmaRxStream[port];
    pUART_DmaRecvStream[port]->Init.Request = DMA_REQUEST_USART3_RX;
    pUART_DmaRecvStream[port]->Init.Direction = DMA_PERIPH_TO_MEMORY;
    pUART_DmaRecvStream[port]->Init.PeriphInc = DMA_PINC_DISABLE;
    pUART_DmaRecvStream[port]->Init.MemInc = DMA_MINC_ENABLE;
    pUART_DmaRecvStream[port]->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    pUART_DmaRecvStream[port]->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    pUART_DmaRecvStream[port]->Init.Mode = DMA_CIRCULAR;
    pUART_DmaRecvStream[port]->Init.Priority = DMA_PRIORITY_LOW;
    pUART_DmaRecvStream[port]->Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    //给个非NULL值，欺骗HAL_DMA_Init，打开半传输完成中断。
    pUART_DmaRecvStream[port]->XferHalfCpltCallback = 1;
    if (HAL_DMA_Init(pUART_DmaRecvStream[port]) != HAL_OK)
    {
      Error_Handler();
    }
    HAL_DMA_Start_IT(pUART_DmaRecvStream[port], (uint32_t)&tg_UART_Reg[port]->RDR,
                    pUART_DmaRecvBuf[port], DmaBufLen);
//  pUART_DmaSendStream[port]->Instance = UartDmaTxStream[port];
    pUART_DmaSendStream[port]->Init.Request = DMA_REQUEST_USART3_TX;
    pUART_DmaSendStream[port]->Init.Direction = DMA_MEMORY_TO_PERIPH;
    pUART_DmaSendStream[port]->Init.PeriphInc = DMA_PINC_DISABLE;
    pUART_DmaSendStream[port]->Init.MemInc = DMA_MINC_ENABLE;
    pUART_DmaSendStream[port]->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    pUART_DmaSendStream[port]->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    pUART_DmaSendStream[port]->Init.Mode = DMA_NORMAL;
    pUART_DmaSendStream[port]->Init.Priority = DMA_PRIORITY_LOW;
    pUART_DmaSendStream[port]->Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(pUART_DmaSendStream[port]) != HAL_OK)
    {
      Error_Handler();
    }
    //发送0个字节，实际上是利用 hal 库初始化 DMA 通道
    HAL_DMA_Start_IT(pUART_DmaSendStream[port],pUART_DmaSendBuf[port],
                            (uint32_t)&tg_UART_Reg[port]->TDR,0);

//  __HAL_LINKDMA(huart,hdmatx,hdma_usart3_tx);

//    DMA_Config(UartDmaRxStream[port],DMA_Rx_ch[port],(u32)&(tg_UART_Reg[port]->RDR),true,
//            (u32)pUART_DmaRecvBuf[port][0],(u32)pUART_DmaRecvBuf[port][1],DMA_DIR_P2M,\
//            DMA_DATABITS_8,DMA_DATABITS_8,DmaBufLen/2);

//    DMA_Config(UartDmaTxStream[port],DMA_Tx_ch[port],(u32)&(tg_UART_Reg[port]->TDR),false,
//            (u32)pUART_DmaSendBuf[port],0,DMA_DIR_M2P,DMA_DATABITS_8,DMA_DATABITS_8,DmaBufLen);

    s_UART_DmaUsed[port] = CN_DMA_USED;

    Int_Register(UartDmaTxInt[port]);//注册发送DMA中断
    Int_Register(UartDmaRxInt[port]);//注册接收DMA中断

    Int_SetClearType(UartDmaRxInt[port],CN_INT_CLEAR_AUTO);//关联中断
    Int_IsrConnect(UartDmaRxInt[port],UART_DmaRx_ISR);
    Int_SetClearType(UartDmaTxInt[port],CN_INT_CLEAR_AUTO);
    Int_IsrConnect(UartDmaTxInt[port],UART_DmaTx_ISR);

    Int_SetIsrPara(UartDmaTxInt[port],port);
    Int_SetIsrPara(UartDmaRxInt[port],port);

    Int_SettoAsynSignal(UartDmaRxInt[port]);
    Int_SettoAsynSignal(UartDmaTxInt[port]);
    Int_RestoreAsynLine(UartDmaRxInt[port]);
    Int_RestoreAsynLine(UartDmaTxInt[port]);

    tg_UART_Reg[port]->ICR |= USART_ICR_IDLECF;//clear  idle int
    tg_UART_Reg[port]->CR1 |= USART_CR1_IDLEIE;//enable idle int
    tg_UART_Reg[port]->CR3 |= USART_CR3_DMAR|USART_CR3_DMAT;  //设置串口使用dma收发
}

//=============================================================================
// 功能: 设置不使用DMA，UART不使用DMA收发，每收发一个字节就必须中断一次，不建议在高速
//      连续通信中使用。
// 参数: port,端口号，即串口号，为1~3
// 返回: 无
// =============================================================================
void __UART_SetDmaUnUsed(u32 port)
{
    u8 IntLine;
    if(port == CN_UART1)             IntLine = CN_INT_LINE_USART1;
    else if(port == CN_UART2)        IntLine = CN_INT_LINE_USART2;
    else if(port == CN_UART3)        IntLine = CN_INT_LINE_USART3;
    else if(port == CN_UART4)        IntLine = CN_INT_LINE_UART4;
    else if(port == CN_UART5)        IntLine = CN_INT_LINE_UART5;
    else if(port == CN_UART6)        IntLine = CN_INT_LINE_USART6;
    else if(port == CN_UART7)        IntLine = CN_INT_LINE_UART7;
    else if(port == CN_UART8)        IntLine = CN_INT_LINE_UART8;
    else if(port == CN_UART9)        IntLine = CN_INT_LINE_UART9;
    else if(port == CN_UART10)        IntLine = CN_INT_LINE_USART10;
    else    return;

    free(pUART_DmaSendBuf[port]);
    pUART_DmaSendBuf[port]=NULL;
    free(pUART_DmaRecvBuf[port]);
    pUART_DmaRecvBuf[port]=NULL;
    s_UART_DmaSending[port]=false;
    tg_UART_Reg[port]->CR3 &= ~0x00c0;     //设置不使用dma传输
    Int_RestoreAsynLine(IntLine);
    if(s_UART_DmaUsed[port] == CN_DMA_USED)
    {
        Int_SaveAsynLine(UartDmaRxInt[port]);
        Int_SaveAsynLine(UartDmaTxInt[port]);
        s_UART_DmaUsed[port] = CN_DMA_UNUSED;
    }
    __UART_RxIntEnable(CN_DMA_UNUSED,port);
    __UART_TxIntEnable(CN_DMA_UNUSED,port);
}
// =============================================================================
// 功能: 串口设备的控制函数,与具体的硬件寄存器设置相关
// 参数: Reg,被操作的串口寄存器指针.
//       cmd,操作类型
//       data1,data2,含义依cmd而定
// 返回: 无意义.
// =============================================================================
static ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd, va_list *arg0)
{
    ptu32_t result = 0;
    u32 port;

    if(Reg == NULL)
        return 0;

    switch((u32)Reg)
    {
    case CN_UART1_BASE: port = CN_UART1;break;
    case CN_UART2_BASE: port = CN_UART2;break;
    case CN_UART3_BASE: port = CN_UART3;break;
    case CN_UART4_BASE: port = CN_UART4;break;
    case CN_UART5_BASE: port = CN_UART5;break;
    case CN_UART6_BASE: port = CN_UART6;break;
    case CN_UART7_BASE: port = CN_UART7;break;
    case CN_UART8_BASE: port = CN_UART8;break;
    case CN_UART9_BASE: port = CN_UART9;break;
    case CN_UART10_BASE: port = CN_UART10;break;
    default:return 0;
    }

    switch(cmd)
    {
        case CN_DEV_CTRL_START:
            __UART_Enable(port);
            break;
        case CN_DEV_CTRL_STOP:
            __UART_Disable(port);
            break;

        case CN_DEV_CTRL_SHUTDOWN:
            break;
        case CN_DEV_CTRL_SLEEP:
            break;
        case CN_DEV_CTRL_RESUME:
            break;
        case CN_UART_SET_BAUD:  //设置Baud
        {
            u32 baud = va_arg(*arg0, u32);
             __UART_BaudSet(Reg,port, baud);
            break;
        }
        case CN_UART_EN_RTS:
            Reg->CR3 |= 0x100;
            break;
        case CN_UART_DIS_RTS:
            Reg->CR3 &= ~0x100;
            break;
        case CN_UART_EN_CTS:
            Reg->CR3 |= 0x200;
            break;
        case CN_UART_DIS_CTS:
            Reg->CR3 &= ~0x200;
            break;
        case CN_UART_DMA_USED:
            __UART_SetDmaUsed(port);
            break;
        case CN_UART_DMA_UNUSED:
            __UART_SetDmaUnUsed(port);
            break;
        case CN_UART_COM_SET:
        {
            struct COMParam *COM = va_arg(*arg0, struct COMParam *);
            __UART_ComConfig(Reg,port,COM);
            break;
        }
        default: break;
    }
    return result;
}

// =============================================================================
// 功能: 当uart2配置成dma方式是，dma1 ch7负责发送，ch6负责接收
// 参数: 中断函数没有参数.
// 返回: 中断函数没有返回值.
// =============================================================================
uint32_t UART_DmaTx_ISR(ptu32_t port)
{
    uint16_t num;
    u8 *DmaSendBuf;
    u32 DmaSendBufLen;
    DMA_Stream_TypeDef *DMA_Stream;
    DMA_Stream = ((DMA_Stream_TypeDef   *)pUART_DmaSendStream[port]->Instance);

    DmaSendBufLen = s_u32UART_DmaBufLen[port];
    DmaSendBuf = pUART_DmaSendBuf[port];
    DMA_ClearIntFlag(DMA_Stream);
    num = UART_PortRead(pUartCB[port],DmaSendBuf,DmaSendBufLen);
    if(num == 0)
    {
        s_UART_DmaSending[port] = false;
        tg_UART_Reg[port]->CR1 |= USART_CR1_TCIE;   //用于485切换
    }
    else
    {
        /* Configure DMA Stream data length */
        DMA_Stream->NDTR = num;
        /* Configure DMA Stream destination address */
        DMA_Stream->PAR = (uint32_t)&tg_UART_Reg[port]->TDR;
        /* Configure DMA Stream source address */
        DMA_Stream->M0AR = DmaSendBuf;

//        DMA_SetConfig(pUART_DmaSendStream[port],
//                        DmaSendBuf, (uint32_t)&tg_UART_Reg[port]->TDR,num);
        __HAL_DMA_ENABLE(pUART_DmaSendStream[port]);
    }
    return 0;
}

// =============================================================================
// 功能: 当uart1配置成dma方式是，dma2 ch2负责发送，ch7负责接收
// 参数: 中断函数没有参数.
// 返回: 中断函数没有返回值.
// =============================================================================
uint32_t UART_DmaRx_ISR(ptu32_t port)
{
    u32 num,DmaRecvBufLen,newrcvlen;
    u32 DmaRecvBufNow,DmaRecvBufold;

    DmaRecvBufLen = s_u32UART_DmaBufLen[port];
    DMA_ClearIntFlag(((DMA_Stream_TypeDef *)pUART_DmaRecvStream[port]->Instance));

    DmaRecvBufold = s_u32DmaRecvIdleOffset[port];
    DmaRecvBufNow = DmaRecvBufLen - ((DMA_Stream_TypeDef *)pUART_DmaRecvStream[port]->Instance)->NDTR;
    s_u32DmaRecvIdleOffset[port] = DmaRecvBufNow;
    if(DmaRecvBufNow > DmaRecvBufold)      //未发生缓冲区环绕
    {
        newrcvlen = DmaRecvBufNow - DmaRecvBufold;
        num = UART_PortWrite(pUartCB[port],pUART_DmaRecvBuf[port]+DmaRecvBufold,
                            newrcvlen);

        if(num != newrcvlen)
            UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
    }
    else if(DmaRecvBufNow < DmaRecvBufold)//区发生缓冲区环绕
    {
        newrcvlen = DmaRecvBufNow + DmaRecvBufLen- DmaRecvBufold;
        num = UART_PortWrite(pUartCB[port],pUART_DmaRecvBuf[port]+DmaRecvBufold,
                            DmaRecvBufLen- DmaRecvBufold);
        num += UART_PortWrite(pUartCB[port],pUART_DmaRecvBuf[port],
                            DmaRecvBufNow);

        if(num != newrcvlen)
            UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
    }

    return 0;
}

// =============================================================================
// 功能: UART的中断服务函数，包括发送空、接收、错误三种中断。
//      1.接收中断，从接收FIFO中读数据，并调用UART_PortWrite写入接收缓冲区中；
//      2.发送中断，调用UART_PortRead读发送缓冲区中数据，并写入发送FIFO；
//      3.错误中断，弹出错误事件
// 参数: 中断号.
// 返回: 0.
// =============================================================================
u32 UART_ISR(ptu32_t port)
{
    struct UartGeneralCB *UCB;
    tagUartReg *Reg;
    u32 num,loop;
    u8 ch;
    u8 fifobuf[20];

    UCB = pUartCB[port];
    Reg = (tagUartReg *)tg_UART_Reg[port];


    if(((Reg->ISR & USART_ISR_RXFF) && (Reg->CR1 & USART_CR1_RXFFIE))   // rx fifo full
        || ((Reg->ISR & USART_ISR_RXFT) && (Reg->CR3 & USART_CR3_RXFTIE))  //rx fifo threshold
        || ((Reg->ISR & USART_ISR_IDLE) && (Reg->CR1 & USART_CR1_IDLEIE))) //idle
    {
        if(s_UART_DmaUsed[port] && (Reg->ISR & USART_ISR_IDLE))
            Int_TapLine(UartDmaRxInt[port]);
        else
        {
            num = 0;
            while ((Reg->ISR & USART_ISR_RXNE_RXFNE) && (num < 20))
            {
                fifobuf[num] = (u8)Reg->RDR;
                num++;
            }
            if(num != 0)
            {
                num = UART_PortWrite(UCB,fifobuf,num);
                if(num != 1)
                {
                    UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
                }
            }
        }
        Reg->ICR = USART_ICR_IDLECF;  //清 USART_ISR_IDLE
    }

    if(//((Reg->ISR & USART_ISR_TXFT) && (Reg->CR3 & USART_CR3_TXFTIE))   //tx fifo threshold
        ((Reg->ISR & USART_ISR_TXFE) && (Reg->CR1 & USART_CR1_TXFEIE)))  //tx fifo empty
    {
        num = UART_PortRead(UCB,fifobuf,14);    //fifo size is 16,threshold be set to 2
        Reg->ICR = USART_ICR_TXFECF;    //这句话清不了发送fifo空中断，不知为何
        if(num == 0)
        {
            Reg->CR1 &= ~USART_CR1_TXFEIE;  //因清不了，故发完后禁止掉它，
            Reg->CR1 |= USART_CR1_TCIE;     //使能发送完成中断
        }
        else
        {
            for(loop = 0; loop < num; loop++)
            {
                Reg->TDR = fifobuf[loop];
            }
        }
    }

//  if( (Reg->ISR & USART_ISR_RXNE_RXFNE) &&(Reg->CR1 & USART_CR1_RXNEIE_RXFNEIE))           //接收中断
//  {
//      ch = (u8)Reg->RDR;
//      num = UART_PortWrite(UCB,&ch,1);
//      if(num != 1)
//      {
//          UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
//      }
//  }
    if((Reg->ISR & USART_ISR_TC)&&(Reg->CR1 & USART_CR1_TCIE))//TC
    {
        Reg->CR1 &=~USART_CR1_TCIE;    //关TC中断
        Board_UartHalfDuplexRecv(port);     //不管是DMA还是中断传输，发送完了数据后都走这里，进入接收状态
    }

    if((Reg->ISR & USART_ISR_TXE_TXFNF)&&(Reg->CR1 & USART_CR1_TXEIE_TXFNFIE)  )    //发送中断
    {
        num = UART_PortRead(UCB,&ch,1);
        if(num != 0)
            Reg->TDR = ch;
        else
        {
            Reg->CR1 &= ~USART_CR1_TXEIE_TXFNFIE;        //txeie
            Reg->CR1 |= USART_CR1_TCIE;
        }
    }
    if(Reg->ISR & USART_ISR_ORE)        //ORE过载错误
    {
        ch = (u8)Reg->RDR;
        num = Reg->ISR;     //读一下sr寄存器
        UART_ErrHandle(UCB,CN_UART_FIFO_OVER_ERR);
        Reg->ICR |=USART_ICR_ORECF;//清过载标志位
    }
//  if((Reg->ISR & USART_ISR_IDLE) && (Reg->CR1 & USART_CR1_IDLEIE))
//  {
//      Reg->ICR |=USART_ICR_IDLECF;
//      UART_DmaRx_ISR(port);
//  }
    if(Reg->ISR & USART_ISR_FE)//帧错误直接清除暂时不做处理
    {
        Reg->ICR |= USART_ICR_FECF;
    }
    return 0;
}

// =============================================================================
// 功能：初始化uart1模块,并装载串口设备，会调用uart.c的API函数。该函数主要完成的功能
//       如下：
//       1.初始化串口的硬件部分，如GPIO、中断、配置配置的COM参数，配置baud = 115200
//         stop bits = 1,无奇偶校验, data bits = 8
//       2.调用uart.c的API函数UART_InstallPort，装入串口设备
// 参数：串口号，0~3
// 返回：1=成功，0=失败
// =============================================================================
ptu32_t ModuleInstall_UART(u32 serial_no)
{
    struct UartParam UART_Param;
    bool_t dmause;
    switch(serial_no)
    {
    case CN_UART1://串口1
        UART_Param.Name         = "UART1";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART1_BASE;
        UART_Param.TxRingBufLen = CFG_UART1_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART1_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        s_UART_DmaUsed[serial_no] = CFG_UART1_ENABLE_DMA;
        s_u32UART_DmaBufLen[serial_no] = CFG_UART1_DMABUF_LEN;
        break;
    case CN_UART2://串口2
        UART_Param.Name         = "UART2";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART2_BASE;
        UART_Param.TxRingBufLen = CFG_UART2_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART2_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        s_UART_DmaUsed[serial_no] = CFG_UART2_ENABLE_DMA;
        s_u32UART_DmaBufLen[serial_no] = CFG_UART2_DMABUF_LEN;
        break;

    case CN_UART3://串口3
        UART_Param.Name         = "UART3";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART3_BASE;
        UART_Param.TxRingBufLen = CFG_UART3_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART3_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        s_UART_DmaUsed[serial_no] = CFG_UART3_ENABLE_DMA;
        s_u32UART_DmaBufLen[serial_no] = CFG_UART3_DMABUF_LEN;
        break;

    case CN_UART4://串口4
        UART_Param.Name         = "UART4";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART4_BASE;
        UART_Param.TxRingBufLen = CFG_UART4_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART4_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        s_UART_DmaUsed[serial_no] = CFG_UART4_ENABLE_DMA;
        s_u32UART_DmaBufLen[serial_no] = CFG_UART4_DMABUF_LEN;
        break;
    case CN_UART5://串口5
        UART_Param.Name         = "UART5";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART5_BASE;
        UART_Param.TxRingBufLen = CFG_UART5_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART5_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        s_UART_DmaUsed[serial_no] = CFG_UART5_ENABLE_DMA;
        s_u32UART_DmaBufLen[serial_no] = CFG_UART5_DMABUF_LEN;
        break;
    case CN_UART6://串口6
        UART_Param.Name         = "UART6";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART6_BASE;
        UART_Param.TxRingBufLen = CFG_UART6_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART6_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        s_UART_DmaUsed[serial_no] = CFG_UART6_ENABLE_DMA;
        s_u32UART_DmaBufLen[serial_no] = CFG_UART6_DMABUF_LEN;
        break;
    case CN_UART7://串口7
        UART_Param.Name         = "UART7";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART7_BASE;
        UART_Param.TxRingBufLen = CFG_UART7_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART7_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        s_UART_DmaUsed[serial_no] = CFG_UART7_ENABLE_DMA;
        s_u32UART_DmaBufLen[serial_no] = CFG_UART7_DMABUF_LEN;
        break;
    case CN_UART8://串口8
        UART_Param.Name         = "UART8";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART8_BASE;
        UART_Param.TxRingBufLen = CFG_UART8_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART8_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        s_UART_DmaUsed[serial_no] = CFG_UART8_ENABLE_DMA;
        s_u32UART_DmaBufLen[serial_no] = CFG_UART8_DMABUF_LEN;
        break;
    case CN_UART9://串口9
        UART_Param.Name         = "UART9";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART9_BASE;
        UART_Param.TxRingBufLen = CFG_UART9_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART9_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        s_UART_DmaUsed[serial_no] = CFG_UART9_ENABLE_DMA;
        s_u32UART_DmaBufLen[serial_no] = CFG_UART9_DMABUF_LEN;
        break;
    case CN_UART10://串口10
        UART_Param.Name         = "UART10";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART10_BASE;
        UART_Param.TxRingBufLen = CFG_UART10_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART10_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        s_UART_DmaUsed[serial_no] = CFG_UART10_ENABLE_DMA;
        s_u32UART_DmaBufLen[serial_no] = CFG_UART10_DMABUF_LEN;
        break;
    default:
        return 0;
    }
    UART_Param.mode = CN_UART_GENERAL;
    Board_UartHalfDuplexRecv(serial_no);
    //硬件初始化
    __UART_HardInit(serial_no);
    __UART_IntInit(serial_no);
    if(s_UART_DmaUsed[serial_no])
    {
        __UART_SetDmaUsed(serial_no);
    }
    else
    {
        pUART_DmaSendBuf[serial_no]=NULL;
        pUART_DmaRecvBuf[serial_no]=NULL;
    }
    pUartCB[serial_no] = UART_InstallGeneral(&UART_Param);
    if( pUartCB[serial_no] == NULL)
        return 0;
    else
        return 1;
}

// =============================================================================
// 功能：字符终端直接发送函数，采用轮询方式，直接写寄存器，用于printk，或者stdout
//       没有初始化
// 参数：str，发送字符串指针
//      len，发送的字节数
// 返回：0，发生错误；result,发送数据长度，字节单位
// =============================================================================
s32 Uart_PutStrDirect(const char *str,u32 len)
{
    u32 result = 0;
    s32 timeout = TxByteTime * len;
    u16 CR_Bak;
    u8 port;
    for(port=0;port<CN_UART_NUM;port++)
    {
        if(PutStrDirectReg ==tg_UART_Reg[port])
            break;
    }
    Board_UartHalfDuplexSend(port);
    CR_Bak = PutStrDirectReg->CR1;                          //Save INT
    PutStrDirectReg->CR1 &= ~USART_CR1_TXEIE_TXFNFIE;                        //disable send INT
    for(result=0; result < len+1; result ++)
    {
        // 超时或者发送缓冲为空时退出
        while((false == __UART_TxTranEmpty(PutStrDirectReg))&& (timeout > 10))
        {
            timeout -=10;
            DJY_DelayUs(10);
        }
        if( (timeout <= 10) || (result == len))
            break;
        PutStrDirectReg->TDR = str[result];
    }
    PutStrDirectReg->CR1 = CR_Bak;                         //restore send INT
    while((PutStrDirectReg->ISR &USART_ISR_TC)!=USART_ISR_TC)
    {
        timeout -=10;
        DJY_DelayUs(10);
        if(timeout < 10)
           break;
    }
    Board_UartHalfDuplexRecv(port);
    return result;
}

// =============================================================================
// 功能：字符终端直接接收函数，采用轮询方式，直接读寄存器，用于stdin初始化前
// 参数：str，发送字符串指针
//      len，发送的字节数
// 返回：0，发生错误；result,发送数据长度，字节单位
// =============================================================================
char Uart_GetCharDirect(void)
{
    u16 CR_Bak;
    char result;

    CR_Bak = GetCharDirectReg->CR1;     //Save INT
    GetCharDirectReg->CR1 &= ~(USART_CR1_TXEIE_TXFNFIE); //disable send INT
    GetCharDirectReg->ICR |= USART_ICR_ORECF;    //清溢出错误标志：轮询接收有可能接收不及时产生溢出错误
    while(__UART_RxHadChar(GetCharDirectReg) == false)
        DJY_EventDelay(1000);

    result = GetCharDirectReg->RDR;
    PutStrDirectReg->CR1 = CR_Bak;                         //restore send INT
    return result;
}

//----初始化内核级IO-----------------------------------------------------------
//功能：初始化内核级输入和输出所需的runtime函数指针。
//参数：无
//返回：无
//-----------------------------------------------------------------------------
void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut)
{
    if(!strcmp(StdioOut,"/dev/UART1"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART1_BASE;
        TxDirectPort = CN_UART1;
    }
    else if(!strcmp(StdioOut,"/dev/UART2"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART2_BASE;
        TxDirectPort = CN_UART2;
    }
    else if(!strcmp(StdioOut,"/dev/UART3"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART3_BASE;
        TxDirectPort = CN_UART3;
    }
    else if(!strcmp(StdioOut,"/dev/UART4"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART4_BASE;
        TxDirectPort = CN_UART4;
    }
    else if(!strcmp(StdioOut,"/dev/UART5"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART5_BASE;
        TxDirectPort = CN_UART5;
    }
    else if(!strcmp(StdioOut,"/dev/UART6"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART6_BASE;
        TxDirectPort = CN_UART6;
    }
    else if(!strcmp(StdioOut,"/dev/UART7"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART7_BASE;
        TxDirectPort = CN_UART7;
    }
    else if(!strcmp(StdioOut,"/dev/UART8"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART8_BASE;
        TxDirectPort = CN_UART8;
    }
    else if(!strcmp(StdioOut,"/dev/UART9"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART9_BASE;
        TxDirectPort = CN_UART9;
    }
    else if(!strcmp(StdioOut,"/dev/UART10"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART10_BASE;
        TxDirectPort = CN_UART10;
    }
    else
    {
        PutStrDirectReg = NULL ;
    }

    if(PutStrDirectReg != NULL)
    {
        __UART_HardInit(TxDirectPort);
        TxByteTime = 95;      //初始化默认115200，发送一个byte是87uS,+10%容限
        PutStrDirect = Uart_PutStrDirect;
    }

    if(!strcmp(StdioIn,"/dev/UART1"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART1_BASE;
        RxDirectPort = CN_UART1;
    }
    else if(!strcmp(StdioIn,"/dev/UART2"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART2_BASE;
        RxDirectPort = CN_UART2;
    }
    else if(!strcmp(StdioIn,"/dev/UART3"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART3_BASE;
        RxDirectPort = CN_UART3;
    }
    else if(!strcmp(StdioIn,"/dev/UART4"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART4_BASE;
        RxDirectPort = CN_UART4;
    }
    else if(!strcmp(StdioIn,"/dev/UART5"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART5_BASE;
        RxDirectPort = CN_UART5;
    }
    else if(!strcmp(StdioIn,"/dev/UART6"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART6_BASE;
        RxDirectPort = CN_UART6;
    }
    else if(!strcmp(StdioIn,"/dev/UART7"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART7_BASE;
        RxDirectPort = CN_UART7;
    }
    else if(!strcmp(StdioIn,"/dev/UART8"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART8_BASE;
        RxDirectPort = CN_UART8;
    }
    else if(!strcmp(StdioIn,"/dev/UART9"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART9_BASE;
        RxDirectPort = CN_UART9;
    }
    else if(!strcmp(StdioIn,"/dev/UART10"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART10_BASE;
        RxDirectPort = CN_UART10;
    }
    else
    {
        GetCharDirectReg = NULL ;
    }
    if(GetCharDirectReg != NULL)
    {
        if(TxDirectPort != RxDirectPort)
            __UART_HardInit(RxDirectPort);
        GetCharDirect = Uart_GetCharDirect;
    }
    return;
}

