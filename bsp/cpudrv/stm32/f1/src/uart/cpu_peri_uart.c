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
// =============================================================================

// 文件名     ：cpu_peri_uart.c
// 模块描述: DJYOS串口模块的底层驱动部分，主要实现寄存器级别的操作，如中断等
// 模块版本: V1.10
// 创建人员: hm
// 创建时间: 14/08.2014
// =============================================================================
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "errno.h"
#include "systime.h"
#include "cpu_peri.h"
#include <device/djy_uart.h>
#include "int.h"
#include "djyos.h"
#include "stdlib.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern ptu32_t ModuleInstall_UART(u32 serial_no);
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
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip uart"//CPU的uart外设驱动
//parent:"uart device file"      //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"uart device file","int","cpu onchip dma"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_UART == false )
//#warning  " cpu_onchip_uart  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_UART    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,32,512,
#define CFG_UART1_SENDBUF_LEN            32                 //"UART1发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN            32                 //"UART1接收环形缓冲区大小",
#define CFG_UART1_DMABUF_LEN             32                 //"UART1 DMA缓冲区大小",

#define CFG_UART2_SENDBUF_LEN            32                 //"UART2发送环形缓冲区大小",
#define CFG_UART2_RECVBUF_LEN            32                 //"UART2接收环形缓冲区大小",
#define CFG_UART2_DMABUF_LEN             32                 //"UART2 DMA缓冲区大小",

#define CFG_UART3_SENDBUF_LEN            32                 //"UART3发送环形缓冲区大小",
#define CFG_UART3_RECVBUF_LEN            32                 //"UART3接收环形缓冲区大小",
#define CFG_UART3_DMABUF_LEN             32                 //"UART3 DMA缓冲区大小",

#define CFG_UART4_SENDBUF_LEN            32                  //"UART4发送环形缓冲区大小",
#define CFG_UART4_RECVBUF_LEN            32                  //"UART4接收环形缓冲区大小",

#define CFG_UART5_SENDBUF_LEN            32                   //"UART5发送环形缓冲区大小",
#define CFG_UART5_RECVBUF_LEN            32                   //"UART5接收环形缓冲区大小",
//%$#@enum,true,false,
#define CFG_UART1_ENABLE                 true                 //"是否配置使用UART1",
#define CFG_UART2_ENABLE                 false                //"是否配置使用UART2",
#define CFG_UART3_ENABLE                 false                //"是否配置使用UART3",
#define CFG_UART4_ENABLE                 false                //"是否配置使用UART4",
#define CFG_UART5_ENABLE                 false                //"是否配置使用UART5",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

// =============================================================================
#define CN_UART1_BASE 0x40013800
#define CN_UART2_BASE 0x40004400
#define CN_UART3_BASE 0x40004800
#define CN_UART4_BASE 0x40004c00
#define CN_UART5_BASE 0x40005000

static u32 TxByteTime;                    //正常发送一个字节所需要的时间
static tagUartReg *PutStrDirectReg;     //用于printk发送的串口寄存器
static tagUartReg *GetCharDirectReg;     //用于直接接收的串口寄存器
static u32 TxDirectPort;                  //用于printk发送的串口号
static u32 RxDirectPort;                  //用于直接接收的串口号
// =============================================================================
static tagUartReg volatile * const tg_UART_Reg[] = {(tagUartReg *)CN_UART1_BASE,
                                                    (tagUartReg *)CN_UART2_BASE,
                                                    (tagUartReg *)CN_UART3_BASE,
                                                    (tagUartReg *)CN_UART4_BASE,
                                                    (tagUartReg *)CN_UART5_BASE};
#define CN_DMA_UNUSED   0
#define CN_DMA_USED     1

//#define CFG_UART1_SENDBUF_LEN            512
//#define CFG_UART1_RECVBUF_LEN            512
//#define CFG_UART1_DMABUF_LEN             32
static u8 * UART1_DmaSendBuf;
static u8 * UART1_DmaRecvBuf;
static bool_t UART1_FIRST_DMA_SEND;
static bool_t UART1_DMA_SENDING = false;
static bool_t s_UART1_DmaUsed = false;
uint32_t UART1_DMA1CH4_ISR(ptu32_t IntLine);
uint32_t UART1_DMA1CH5_ISR(ptu32_t IntLine);

//#define CFG_UART2_SENDBUF_LEN            32
//#define CFG_UART2_RECVBUF_LEN            32
//#define CFG_UART2_DMABUF_LEN             32
static u8 * UART2_DmaSendBuf;
static u8 * UART2_DmaRecvBuf;
static bool_t UART2_FIRST_DMA_SEND;
static bool_t UART2_DMA_SENDING = false;
static bool_t s_UART2_DmaUsed = false;
uint32_t UART2_DMA1CH7_ISR(ptu32_t IntLine);
uint32_t UART2_DMA1CH6_ISR(ptu32_t IntLine);

//#define CFG_UART3_SENDBUF_LEN            32
//#define CFG_UART3_RECVBUF_LEN            32
//#define CFG_UART3_DMABUF_LEN             32
static u8 * UART3_DmaSendBuf;
static u8 * UART3_DmaRecvBuf;
static bool_t UART3_FIRST_DMA_SEND;
static bool_t UART3_DMA_SENDING = false;
static bool_t s_UART3_DmaUsed = false;

uint32_t UART3_DMA1CH2_ISR(ptu32_t IntLine);
uint32_t UART3_DMA1CH3_ISR(ptu32_t IntLine);
//#define CFG_UART4_SENDBUF_LEN            32
//#define CFG_UART4_RECVBUF_LEN            32
//
//#define CFG_UART5_SENDBUF_LEN            32
//#define CFG_UART5_RECVBUF_LEN            32

static struct UartGeneralCB *pUartCB[CN_UART_NUM];

//用于标识串口是否初始化标记，第0位表示UART0，第一位表UART1....
//依此类推，1表示初始化，0表示未初始化
static u8 sUartInited = 0;
__attribute__((weak))  void Board_UartHalfDuplexSend(u8 SerialNo)
{
    return;
}
__attribute__((weak))  void Board_UartHalfDuplexRecv(u8 SerialNo)
{
    return ;
}
// =============================================================================
static ptu32_t UART_ISR(ufast_t port);

// =============================================================================
// 功能: 禁止uart的发送中断。
// 参数: flag,是否使用dma传输
//       port,串口号，1~5
// 返回: 无
// =============================================================================
void __UART_RxIntEnable(u8 flag,u8 port)
{
    switch(port)
    {
    case CN_UART1:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch5);
        else
            bb_uart1_cr1_rxneie = 1;
        break;
    case CN_UART2:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch6);
        else
            bb_uart2_cr1_rxneie = 1;//使能接收中断
        break;
    case CN_UART3:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch3);
        else
            bb_uart3_cr1_rxneie = 1;//使能接收中断
        break;
    case CN_UART4:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA2_Ch3);
        else
            bb_uart4_cr1_rxneie = 1;//使能接收中断
        break;
    case CN_UART5:
        bb_uart5_cr1_rxneie = 1;//使能接收中断
        break;
    default:
        break;
    }
}

// =============================================================================
// 功能: 禁止uart的接收中断
// 参数: reg,被操作的寄存器组指针
//       port,串口号
// 返回: 无
// =============================================================================
void __UART_RxIntDisable(u8 flag, u8 port)
{
    switch(port)
    {
    case CN_UART1:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch5);
        else
            bb_uart1_cr1_rxneie = 0;
        break;
    case CN_UART2:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch6);
        else
            bb_uart2_cr1_rxneie = 0;//禁止接收中断
        break;
    case CN_UART3:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch3);
        else
            bb_uart3_cr1_rxneie = 0;//禁止接收中断
        break;
    case CN_UART4:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA2_Ch3);
        else
            bb_uart4_cr1_rxneie = 0;//禁止接收中断
        break;
    case CN_UART5:
        bb_uart5_cr1_rxneie = 0;//禁止接收中断
        break;
    default:
        break;
    }
}

// =============================================================================
// 功能: 使能uart的发送中断
// 参数: reg,被操作的寄存器组指针
//       port,串口号
// 返回: 无
// =============================================================================
static void __UART_TxIntEnable(u8 flag,u8 port)
{
    switch(port)
    {
    case CN_UART1:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch4);
        else
            bb_uart1_cr1_txeie = 1;//使能发送空中断
        break;
    case CN_UART2:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch7);
        else
            bb_uart2_cr1_txeie = 1;//使能发送空中断
        break;
    case CN_UART3:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch2);
        else
            bb_uart3_cr1_txeie = 1;//使能发送空中断
        break;
    case CN_UART4:
        if(flag == CN_DMA_USED)
            Int_RestoreAsynLine(CN_INT_LINE_DMA2_Ch4_5);
        else
            bb_uart4_cr1_txeie = 1;//使能发送空中断
        break;
    case CN_UART5:
        bb_uart5_cr1_txeie = 1;//使能发送空中断
        break;
    default:
        break;
    }
}

// =============================================================================
// 功能: 禁止UART的发送中断
// 参数: Reg,被操作的寄存器组指针
//       port,串口号
// 返回: 无
// =============================================================================
static void __UART_TxIntDisable(u8 flag, u8 port)
{
    switch(port)
    {
    case CN_UART1:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch4);
        else
            bb_uart1_cr1_txeie = 0;//禁止发送空中断
        break;
    case CN_UART2:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch7);
        else
            bb_uart2_cr1_txeie = 0;//禁止发送空中断
        break;
    case CN_UART3:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch2);
        else
            bb_uart3_cr1_txeie = 0;//禁止发送空中断
        break;
    case CN_UART4:
        if(flag == CN_DMA_USED)
            Int_SaveAsynLine(CN_INT_LINE_DMA2_Ch4_5);
        else
            bb_uart4_cr1_txeie = 0;//禁止发送空中断
        break;
    case CN_UART5:
        bb_uart5_cr1_txeie = 0;//禁止发送空中断
        break;
    default:
        break;
    }
}

// =============================================================================
// 功能: 检查发送寄存器的状态，如果空就返回true，若为空，否则返回false
// 参数：reg,被操作的寄存器组指针
// 返回: true = 缓冲区空，false = 非空
// =============================================================================
static bool_t __UART_TxTranEmpty(tagUartReg volatile *reg)
{
    return (0 != (reg->SR & (1<<7)));
}

// =============================================================================
// 功能: 检查接收寄存器的状态，有数据就返回true，否则返回false
// 参数：reg,被操作的寄存器组指针
// 返回: true = 有数据，false = 无数据
// =============================================================================
static bool_t __UART_RxHadChar(tagUartReg volatile *reg)
{
    return (1 == (reg->SR & (1<<5)));
}

// =============================================================================
// 功能: 设置串口baud,PCLK为25M，CPU主频为100M计算,该驱动只提供9600、19200、57600、
//       115200四种波特率设置
// 参数: Reg,被操作的寄存器组指针
//       port,串口号
//       baud,波特率
// 返回: 无
// =============================================================================
static void __UART_BaudSet(tagUartReg volatile *Reg,u32 port,u32 baud)
{
    u32 temp1,temp2;
    if((port == CN_UART1) || (port == CN_UART5))
    {
        temp1 = CN_CFG_PCLK2/(16*baud);
        temp2 = CN_CFG_PCLK2%(16*baud);
        temp2 = temp2/baud;
    }
    else
    {
        temp1 = CN_CFG_PCLK1/(16*baud);
        temp2 = CN_CFG_PCLK1%(16*baud);
        temp2 = temp2/baud;
    }
    Reg->BRR =  ((u16)temp1<<4)+(u16)temp2;
    if(TxDirectPort == port)
    {
        TxByteTime = 11000000/baud;     //1个字节传输时间，按10bit，+10%计算
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

    switch(COM->DataBits)               // data bits
    {
    case CN_UART_DATABITS_8:    Reg->CR1 &= ~(1<<12);break;
    case CN_UART_DATABITS_9:    Reg->CR1 |= (1<<12);;break;
    default:break;
    }

    switch(COM->Parity)                 // parity
    {
    Reg->CR1 |= (1<<10);                // enable parity
    case CN_UART_PARITY_ODD:    Reg->CR1 |= (1<<9);break;
    case CN_UART_PARITY_EVEN:   Reg->CR1 &=~(1<<9);break;
    default:break;
    }

    switch(COM->StopBits)               // stop bits
    {
    Reg->CR2 &= ~(3<<12);
    case CN_UART_STOPBITS_1:    Reg->CR2 |= (0<<12);break;
    case CN_UART_STOPBITS_1_5:  Reg->CR2 |= (1<<12);break;
    case CN_UART_STOPBITS_2:    Reg->CR2 |= (2<<12);break;
    default:break;
    }
}

// =============================================================================
// 功能: 使串口开始工作，时钟和uart模块
// 参数: port,串口号
// 返回: 无
// =============================================================================
void __UART_Enable(u32 port)
{
    switch(port)
    {
    case CN_UART1:
        bb_rcc_apb2enr_uart1en = 1;    //uart1时钟使能
        bb_uart1_cr1_ue = 1;
        break;
    case CN_UART2:
        bb_rcc_apb1enr_uart2en = 1;    //uart2时钟使能
        bb_uart2_cr1_ue = 1;
        break;
    case CN_UART3:
        bb_rcc_apb1enr_uart3en = 1;    //uart3时钟使能
        bb_uart3_cr1_ue = 1;
        break;
    case CN_UART4:
        bb_rcc_apb1enr_uart4en = 1;    //uart4时钟使能
        bb_uart4_cr1_ue = 1;
        break;
    case CN_UART5:
        bb_rcc_apb1enr_uart5en = 1;    //uart5时钟使能
        bb_uart5_cr1_ue = 1;
        break;
    default:
        break;
    }
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
    case CN_UART1:
        bb_rcc_apb2enr_uart1en = 0;    //uart1时钟使能
        bb_uart1_cr1_ue = 0;
        break;
    case CN_UART2:
        bb_rcc_apb1enr_uart2en = 0;    //uart2时钟使能
        bb_uart2_cr1_ue = 0;
        break;
    case CN_UART3:
        bb_rcc_apb1enr_uart3en = 0;    //uart3时钟使能
        bb_uart3_cr1_ue = 0;
        break;
    case CN_UART4:
        bb_rcc_apb1enr_uart4en = 0;    //uart4时钟使能
        bb_uart4_cr1_ue = 0;
        break;
    case CN_UART5:
        bb_rcc_apb1enr_uart5en = 0;    //uart5时钟使能
        bb_uart5_cr1_ue = 0;
        break;
    default:
        break;
    }
}

// =============================================================================
// 功能: 初始化UART对应的中断线，并初始化中断入口函数
// 参数：SerialNo,串口号
// 返回: 无
// =============================================================================
static void __UART_IntInit(ptu32_t SerialNo)
{
    u8 IntLine;
    if(SerialNo == CN_UART1)
        IntLine = CN_INT_LINE_USART1;
    else if(SerialNo == CN_UART2)
        IntLine = CN_INT_LINE_USART2;
    else if(SerialNo == CN_UART3)
        IntLine = CN_INT_LINE_USART3;
    else if(SerialNo == CN_UART4)
        IntLine = CN_INT_LINE_UART4;
    else if(SerialNo == CN_UART4)
        IntLine = CN_INT_LINE_UART5;
    else
        return;

    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,UART_ISR);
    Int_SetIsrPara(IntLine,SerialNo);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
}

// =============================================================================
// 功能: 硬件参数配置和寄存器的初始化，包括波特率、停止位、校验位、数据位，默认情况下:
//       波特率:115200  ； 停止位:1 ; 校验:偶校验 ; 数据位:8bit
// 参数: SerialNo,串口号，0~3
// 返回: 无
// =============================================================================
static void __UART_HardInit(u8 SerialNo)
{
    if(SerialNo > CN_UART5)
        return;
    //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
   //初始化uart硬件控制数据结构
    tg_UART_Reg[SerialNo]->CR1 = 0x20ac;
    tg_UART_Reg[SerialNo]->CR2 = 0x0;
    tg_UART_Reg[SerialNo]->CR3 = 0x0000;
    __UART_BaudSet(tg_UART_Reg[SerialNo],SerialNo,115200);
}

// =============================================================================
// 功能: 查询等待串口发送完毕，超时返回
// 参数: sending，dma发送方式，串口是否已经发送完成
// 返回: 无
// =============================================================================
bool_t __uart_dma_timeout(bool_t sending)
{
    u32 timeout = 1000;
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
    u32 num,addr;

    switch(port)
    {
    case CN_UART1:
//        if(true == __uart_dma_timeout(UART1_DMA_SENDING))
        if(true == UART1_DMA_SENDING)
            break;
        num = UART_PortRead(pUartCB[CN_UART1],UART1_DmaSendBuf,CFG_UART1_DMABUF_LEN);
        if(UART1_FIRST_DMA_SEND)
        {
            tg_UART_Reg[CN_UART1]->DR = UART1_DmaSendBuf[0];
            UART1_FIRST_DMA_SEND = false;
            addr = (u32)UART1_DmaSendBuf+1;
            num--;
        }else
            addr = (u32)UART1_DmaSendBuf;

        if(num > 0)
        {
            bb_dma1_ch4_ccr_en = 0;    //暂停dma传输
            pg_dma1_channel4_reg->CMAR = addr;
            pg_dma1_channel4_reg->CNDTR = num;
            bb_dma1_ch4_ccr_en = 1;    //继续dma传输
            UART1_DMA_SENDING = true;
        }
        else
            tg_UART_Reg[CN_UART1]->CR1 |=(1<<6);
        break;
    case 2:
//        if(true == __uart_dma_timeout(UART2_DMA_SENDING))
        if(true == UART2_DMA_SENDING)
            break;
        num = UART_PortRead(pUartCB[CN_UART2],UART2_DmaSendBuf,CFG_UART2_DMABUF_LEN);
        if(UART2_FIRST_DMA_SEND)
        {
            tg_UART_Reg[CN_UART1]->DR = UART2_DmaSendBuf[0];
            UART2_FIRST_DMA_SEND = false;
            addr = (u32)UART2_DmaSendBuf+1;
            num--;
        }else
            addr = (u32)UART2_DmaSendBuf;

        if(num > 0)
        {
            bb_dma1_ch7_ccr_en = 0;    //暂停dma传输
            pg_dma1_channel7_reg->CMAR = addr;
            pg_dma1_channel7_reg->CNDTR = num;
            bb_dma1_ch7_ccr_en = 1;    //继续dma传输
            UART2_DMA_SENDING = true;
        }
        else
            tg_UART_Reg[CN_UART1]->CR1 |=(1<<6);
        break;
    case 3:
//        if(true == __uart_dma_timeout(UART3_DMA_SENDING))
        if(true == UART3_DMA_SENDING)
            break;
        num = UART_PortRead(pUartCB[CN_UART3],(u8*)UART3_DmaSendBuf,CFG_UART3_DMABUF_LEN);
        if(UART3_FIRST_DMA_SEND)
        {
            tg_UART_Reg[CN_UART1]->DR = UART3_DmaSendBuf[0];
            UART3_FIRST_DMA_SEND = false;
            addr = (u32)UART3_DmaSendBuf+1;
            num--;
        }else
            addr = (u32)UART3_DmaSendBuf;

        if(num > 0)
        {
            bb_dma1_ch2_ccr_en = 0;    //暂停dma传输
            pg_dma1_channel2_reg->CMAR = addr;
            pg_dma1_channel2_reg->CNDTR = num;
            bb_dma1_ch2_ccr_en = 1;    //继续dma传输
            UART3_DMA_SENDING = true;
        }
        else
            tg_UART_Reg[CN_UART1]->CR1 |=(1<<6);
        break;
    default:
            tg_UART_Reg[CN_UART1]->CR1 |=(1<<6);
        break;
    }
    return 0;
}
// =============================================================================
// 功能: 直接发送函数，使用轮询方式。
// 参数: Reg,寄存器基址指针.
//       send_buf,发送数据缓冲区
//       len,数据长度，字节
//       timeout,超时时间，us
// 返回: 发送字节数
// =============================================================================
//static u32 __UART_SendDirectly(tagUartReg *Reg,u8 *send_buf,u32 len,u32 timeout)
//{
//    u32  result = 0;
//
//    if(Reg == NULL)
//        return result;
//
//    Reg->CR1 &= ~(1<<7);                            //disable send INT
//    for(result=0; result < len; result ++)
//    {
//        // 超时或者发送缓冲为空时退出
//        while((false == __UART_TxTranEmpty(Reg))&& (timeout > 10))
//        {
//            timeout -=10;
//            DJY_DelayUs(10);
//        }
//        if(timeout <= 10)
//            break;
//        Reg->DR = send_buf[result];
//    }
//    Reg->CR1 |= (1<<7);                             //enable send INT
//    return result;
//}
//
// =============================================================================
// 功能: 启动串口发送，其目的是触发中断，用中断方式发送数据。
// 参数: Reg,被操作的串口寄存器指针.
// 返回: 发送的个数
// =============================================================================
static u32 __UART_SendStart (tagUartReg *Reg)
{
    u8 port,dmaused;


    switch((u32)Reg)
    {
    case CN_UART1_BASE:
        dmaused = s_UART1_DmaUsed;
        port = CN_UART1;    break;
    case CN_UART2_BASE:
        dmaused = s_UART2_DmaUsed;
        port = CN_UART2;    break;
    case CN_UART3_BASE:
        dmaused = s_UART3_DmaUsed;
        port = CN_UART3;    break;
    case CN_UART4_BASE:
        dmaused = CN_DMA_UNUSED;
        port = CN_UART4;    break;
    case CN_UART5_BASE:
        dmaused = CN_DMA_UNUSED;
        port = CN_UART5;    break;
    default:return 0;
    }
    Reg->CR1 &=~(1<<6);
    Board_UartHalfDuplexSend(port);//切换到发送
    __UART_TxIntDisable(dmaused,port);
    if(dmaused)
    {
        __UART_DMA_SendStart(port);
    }
    else
    {
        //非DMA方式，只要使能发送空中断，则触发中断
    }

    __UART_TxIntEnable(dmaused,port);
    return 1;
}

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


    switch(port)
    {
    case CN_UART1:

       if(UART1_DmaSendBuf==NULL)
           UART1_DmaSendBuf = (u8*)M_Malloc(CFG_UART1_DMABUF_LEN,0);
           if(UART1_DmaSendBuf == NULL)
               return;

       if(UART1_DmaRecvBuf==NULL)
           UART1_DmaRecvBuf = (u8*)M_Malloc(CFG_UART1_DMABUF_LEN,0);
           if(UART1_DmaRecvBuf == NULL)
           {
               free(UART1_DmaRecvBuf);
               UART1_DmaRecvBuf=NULL;
               return;
           }

        bb_rcc_ahbenr_dma1en = 1;
        bb_dma1_ch4_ccr_en = 0;    //停止dma通道
        bb_dma1_ch5_ccr_en = 0;    //停止dma通道
        bb_uart1_cr1_rxneie = 0;
        bb_uart1_cr1_txeie = 0;
        bb_uart1_cr1_idleie = 1;
        pg_dma1_reg->IFCR |= 0x000ff000;        //清所有中断状态
        pg_dma1_channel4_reg->CCR = 0x00000092; //发送dma
        pg_dma1_channel4_reg->CPAR = (u32)&(tg_UART_Reg[CN_UART1]->DR);

        pg_dma1_channel5_reg->CCR = 0x00000086; //接收dma
        pg_dma1_channel5_reg->CPAR = (u32)&(tg_UART_Reg[CN_UART1]->DR);
        pg_dma1_channel5_reg->CNDTR = 32;
        pg_dma1_channel5_reg->CMAR = (u32)UART1_DmaRecvBuf;

        Int_Register(CN_INT_LINE_DMA1_Ch4);
        Int_Register(CN_INT_LINE_DMA1_Ch5);

        Int_SetClearType(CN_INT_LINE_DMA1_Ch4,CN_INT_CLEAR_AUTO);
        Int_IsrConnect(CN_INT_LINE_DMA1_Ch4,UART1_DMA1CH4_ISR);
        Int_SetClearType(CN_INT_LINE_DMA1_Ch5,CN_INT_CLEAR_AUTO);
        Int_IsrConnect(CN_INT_LINE_DMA1_Ch5,UART1_DMA1CH5_ISR);

        Int_SettoAsynSignal(CN_INT_LINE_DMA1_Ch4);
        Int_SettoAsynSignal(CN_INT_LINE_DMA1_Ch5);
        Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch4);
        Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch5);

        tg_UART_Reg[CN_UART1]->CR3 |= 0x00c0;  //设置串口使用dma收发
        s_UART1_DmaUsed = CN_DMA_USED;
        bb_dma1_ch5_ccr_en = 1;    //启动dma通道
        UART1_FIRST_DMA_SEND = true;
        break;
    case CN_UART2:
        if(UART2_DmaSendBuf==NULL)
            UART2_DmaSendBuf = (u8*)M_Malloc(CFG_UART2_DMABUF_LEN,0);
            if(UART2_DmaSendBuf == NULL)
                return;

        if(UART2_DmaRecvBuf==NULL)
            UART2_DmaRecvBuf = (u8*)M_Malloc(CFG_UART2_DMABUF_LEN,0);
            if(UART2_DmaRecvBuf == NULL)
            {
                free(UART2_DmaRecvBuf);
                UART2_DmaRecvBuf=NULL;
                return;
            }
        bb_rcc_ahbenr_dma1en = 1;
        bb_dma1_ch7_ccr_en = 0;    //停止dma通道
        bb_dma1_ch6_ccr_en = 0;    //停止dma通道
        bb_uart2_cr1_rxneie = 0;
        bb_uart2_cr1_txeie = 0;
        bb_uart2_cr1_idleie = 1;
        pg_dma1_reg->IFCR |= 0x0ff00000;        //清所有中断状态
        pg_dma1_channel7_reg->CCR = 0x00000092; //发送dma
        pg_dma1_channel7_reg->CPAR = (u32)&(tg_UART_Reg[CN_UART2]->DR);

        pg_dma1_channel6_reg->CCR = 0x00000086; //接收dma
        pg_dma1_channel6_reg->CPAR = (u32)&(tg_UART_Reg[CN_UART2]->DR);
        pg_dma1_channel6_reg->CNDTR = 32;
        pg_dma1_channel6_reg->CMAR = (u32)UART2_DmaRecvBuf;

        Int_Register(CN_INT_LINE_DMA1_Ch6);
        Int_Register(CN_INT_LINE_DMA1_Ch7);

        Int_SetClearType(CN_INT_LINE_DMA1_Ch6,CN_INT_CLEAR_AUTO);
        Int_IsrConnect(CN_INT_LINE_DMA1_Ch6,UART2_DMA1CH6_ISR);
        Int_SetClearType(CN_INT_LINE_DMA1_Ch7,CN_INT_CLEAR_AUTO);
        Int_IsrConnect(CN_INT_LINE_DMA1_Ch7,UART2_DMA1CH7_ISR);

        Int_SettoAsynSignal(CN_INT_LINE_DMA1_Ch6);
        Int_SettoAsynSignal(CN_INT_LINE_DMA1_Ch7);
        Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch6);
        Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch7);

        tg_UART_Reg[CN_UART2]->CR3 |= 0x00c0;  //设置串口使用dma收发
        s_UART2_DmaUsed = CN_DMA_USED;
        bb_dma1_ch6_ccr_en = 1;    //启动dma通道
        UART2_FIRST_DMA_SEND = true;
        break;
    case CN_UART3:
        if(UART3_DmaSendBuf==NULL)
            UART3_DmaSendBuf = (u8*)M_Malloc(CFG_UART3_DMABUF_LEN,0);
            if(UART3_DmaSendBuf == NULL)
                return;

        if(UART3_DmaRecvBuf==NULL)
            UART3_DmaRecvBuf = (u8*)M_Malloc(CFG_UART3_DMABUF_LEN,0);
            if(UART3_DmaRecvBuf == NULL)
            {
                free(UART3_DmaSendBuf);
                UART3_DmaRecvBuf=NULL;
                return;
            }
        bb_rcc_ahbenr_dma1en = 1;
        bb_dma1_ch2_ccr_en = 0;    //停止dma通道
        bb_dma1_ch3_ccr_en = 0;    //停止dma通道
        bb_uart3_cr1_rxneie = 0;
        bb_uart3_cr1_txeie = 0;
        bb_uart3_cr1_idleie = 1;
        pg_dma1_reg->IFCR |= 0x000ff000;        //清所有中断状态
        pg_dma1_channel2_reg->CCR = 0x00000092; //发送dma
        pg_dma1_channel2_reg->CPAR = (u32)&(tg_UART_Reg[CN_UART3]->DR);

        pg_dma1_channel3_reg->CCR = 0x00000086; //接收dma
        pg_dma1_channel3_reg->CPAR = (u32)&(tg_UART_Reg[CN_UART3]->DR);
        pg_dma1_channel3_reg->CNDTR = 32;
        pg_dma1_channel3_reg->CMAR = (u32)UART3_DmaRecvBuf;

        Int_Register(CN_INT_LINE_DMA1_Ch2);
        Int_Register(CN_INT_LINE_DMA1_Ch3);

        Int_SetClearType(CN_INT_LINE_DMA1_Ch2,CN_INT_CLEAR_AUTO);
        Int_IsrConnect(CN_INT_LINE_DMA1_Ch2,UART3_DMA1CH2_ISR);
        Int_SetClearType(CN_INT_LINE_DMA1_Ch3,CN_INT_CLEAR_AUTO);
        Int_IsrConnect(CN_INT_LINE_DMA1_Ch3,UART3_DMA1CH3_ISR);

        Int_SettoAsynSignal(CN_INT_LINE_DMA1_Ch2);
        Int_SettoAsynSignal(CN_INT_LINE_DMA1_Ch3);
        Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch2);
        Int_RestoreAsynLine(CN_INT_LINE_DMA1_Ch3);

        tg_UART_Reg[CN_UART3]->CR3 |= 0x00c0;  //设置串口使用dma收发
        s_UART3_DmaUsed = CN_DMA_USED;
        bb_dma1_ch3_ccr_en = 1;    //启动dma通道
        UART3_FIRST_DMA_SEND = true;
        break;
    default:
        break;
    }

}

//=============================================================================
// 功能: 设置不使用DMA，UART不使用DMA收发，每收发一个字节就必须中断一次，不建议在高速
//      连续通信中使用。
// 参数: port,端口号，即串口号，为1~3
// 返回: 无
// =============================================================================
void __UART_SetDmaUnUsed(u32 port)
{
    switch(port)
    {
    case CN_UART1:
        free(UART1_DmaSendBuf);
        free(UART1_DmaRecvBuf);
        UART1_DmaRecvBuf=NULL;
        UART1_DmaRecvBuf=NULL;
        tg_UART_Reg[CN_UART1]->CR3 &= ~0x00c0;     //设置不使用dma传输
        Int_RestoreAsynLine(CN_INT_LINE_USART1);
        if(s_UART1_DmaUsed == CN_DMA_USED)
        {
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch4);
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch5);
            s_UART1_DmaUsed = CN_DMA_UNUSED;
        }
        bb_uart1_cr1_rxneie = 1;
        bb_uart1_cr1_txeie  = 1;
        bb_uart1_cr1_idleie = 0;
        break;
    case CN_UART2:
        free(UART2_DmaSendBuf);
        free(UART2_DmaRecvBuf);
        UART2_DmaRecvBuf=NULL;
        UART2_DmaRecvBuf=NULL;
        tg_UART_Reg[CN_UART2]->CR3 &= ~0x00c0;     //设置不使用dma传输
        Int_RestoreAsynLine(CN_INT_LINE_USART2);
        if(s_UART2_DmaUsed == CN_DMA_USED)
        {
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch6);
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch7);
            s_UART2_DmaUsed = CN_DMA_UNUSED;
        }
        bb_uart2_cr1_rxneie = 1;
        bb_uart2_cr1_txeie = 1;
        bb_uart2_cr1_idleie = 0;
        break;
    case CN_UART3:
        free(UART3_DmaSendBuf);
        free(UART3_DmaRecvBuf);
        UART3_DmaRecvBuf=NULL;
        UART3_DmaRecvBuf=NULL;
        tg_UART_Reg[CN_UART3]->CR3 &= ~0x00c0;     //设置不使用dma传输
        Int_RestoreAsynLine(CN_INT_LINE_USART3);
        if(s_UART3_DmaUsed == CN_DMA_USED)
        {
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch2);
            Int_SaveAsynLine(CN_INT_LINE_DMA1_Ch3);
            s_UART3_DmaUsed = CN_DMA_UNUSED;
        }
        bb_uart3_cr1_rxneie = 1;
        bb_uart3_cr1_txeie = 1;
        bb_uart3_cr1_idleie = 0;
        break;
    default:
        break;
    }
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
            u32 data;
            data = va_arg(*arg0, u32);
            __UART_BaudSet(Reg,port, data);
        }
            break;
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
            struct COMParam *COM;
            COM = va_arg(*arg0, void *);
            __UART_ComConfig(Reg,port,COM);
        }
            break;
        default: break;
    }
    return result;
}

// =============================================================================
// 功能: UART1的DMA中断，当uart1配置成dma方式时，dma1 ch4负责发送，ch5负责接收
// 参数: 中断函数没有参数.
// 返回: 中断函数没有返回值.
// =============================================================================
uint32_t UART1_DMA1CH4_ISR(ptu32_t tagIntLine)
{
    uint16_t num;
    pg_dma1_reg->IFCR |= 0x0000f000;
    num = UART_PortRead(pUartCB[CN_UART1],UART1_DmaSendBuf,CFG_UART1_DMABUF_LEN);
    if(num == 0)
        {
            UART1_DMA_SENDING = false;
            tg_UART_Reg[CN_UART1]->CR1 |= (1<<6);
        }
    else
    {
        bb_dma1_ch4_ccr_en = 0;    //暂停dma传输
        pg_dma1_channel4_reg->CMAR = (u32)UART1_DmaSendBuf;
        pg_dma1_channel4_reg->CNDTR = num;
        bb_dma1_ch4_ccr_en = 1;    //继续dma传输
    }
    return 0;
}

// =============================================================================
// 功能: UART1的DMA中断，当uart1配置成dma方式时，dma1 ch4负责发送，ch5负责接收
// 参数: 中断函数没有参数.
// 返回: 中断函数没有返回值.
// =============================================================================
uint32_t UART1_DMA1CH5_ISR(ptu32_t tagIntLine)
{
    u32 recvs,num;
    pg_dma1_reg->IFCR |= 0x000f0000;
    bb_dma1_ch5_ccr_en = 0;    //暂停dma传输
    recvs = CFG_UART1_DMABUF_LEN - pg_dma1_channel5_reg->CNDTR;
    if(recvs == CFG_UART1_DMABUF_LEN)     //dma缓冲区满，可能有数据丢失
    {
        UART_ErrHandle(pUartCB[CN_UART1],CN_UART_FIFO_OVER_ERR);
    }else
    {
        num = UART_PortWrite(pUartCB[CN_UART1],UART1_DmaRecvBuf,recvs);
        if(num != recvs)
        {
            UART_ErrHandle(pUartCB[CN_UART1],CN_UART_BUF_OVER_ERR);
        }
    }
    pg_dma1_channel5_reg->CNDTR = CFG_UART1_DMABUF_LEN;
    pg_dma1_channel5_reg->CMAR = (u32)UART1_DmaRecvBuf;
    bb_dma1_ch5_ccr_en = 1;    //继续dma传输
    return 0;
}

// =============================================================================
// 功能: 当uart2配置成dma方式是，dma1 ch7负责发送，ch6负责接收
// 参数: 中断函数没有参数.
// 返回: 中断函数没有返回值.
// =============================================================================
uint32_t UART2_DMA1CH7_ISR(ptu32_t tagIntLine)
{
    uint16_t num;

    pg_dma1_reg->IFCR |= 0x0f000000;
    num = UART_PortRead(pUartCB[CN_UART2],UART2_DmaSendBuf,CFG_UART2_DMABUF_LEN);
    if(num == 0)
        UART2_DMA_SENDING = false;
    else
    {
        bb_dma1_ch7_ccr_en = 0;    //暂停dma传输
        pg_dma1_channel7_reg->CMAR = (u32)UART2_DmaSendBuf;
        pg_dma1_channel7_reg->CNDTR = num;
        bb_dma1_ch7_ccr_en = 1;    //继续dma传输
    }
    return 0;
}

// =============================================================================
// 功能: 当uart2配置成dma方式是，dma1 ch7负责发送，ch6负责接收
// 参数: 中断函数没有参数.
// 返回: 中断函数没有返回值.
// =============================================================================
uint32_t UART2_DMA1CH6_ISR(ptu32_t tagIntLine)
{
    u32 recvs,num;
    pg_dma1_reg->IFCR |= 0x00f00000;
    bb_dma1_ch6_ccr_en = 0;    //暂停dma传输
    recvs = CFG_UART2_DMABUF_LEN - pg_dma1_channel6_reg->CNDTR;
    if(recvs == CFG_UART2_DMABUF_LEN)     //dma缓冲区满，可能有数据丢失
    {
        UART_ErrHandle(pUartCB[CN_UART2],CN_UART_FIFO_OVER_ERR);
    }else
    {
        num = UART_PortWrite(pUartCB[CN_UART2],UART2_DmaRecvBuf,recvs);
        if(num != recvs)
        {
            UART_ErrHandle(pUartCB[CN_UART2],CN_UART_BUF_OVER_ERR);
        }
    }
    pg_dma1_channel6_reg->CNDTR = CFG_UART2_DMABUF_LEN;
    pg_dma1_channel6_reg->CMAR = (u32)UART2_DmaRecvBuf;
    bb_dma1_ch6_ccr_en = 1;    //继续dma传输
    return 0;
}

// =============================================================================
// 功能: 当uart3配置成dma方式是，dma1 ch2负责发送，ch3负责接收
// 参数: 中断函数没有参数.
// 返回: 中断函数没有返回值.
// =============================================================================
uint32_t UART3_DMA1CH2_ISR(ptu32_t tagIntLine)
{
    uint16_t num;
    pg_dma1_reg->IFCR |= 0x000000f0;
    num = UART_PortRead(pUartCB[CN_UART3],UART3_DmaSendBuf,CFG_UART3_DMABUF_LEN);
    if(num == 0)
        UART3_DMA_SENDING = false;
    else
    {
        bb_dma1_ch2_ccr_en = 0;    //暂停dma传输
        pg_dma1_channel2_reg->CMAR = (u32)UART3_DmaSendBuf;
        pg_dma1_channel2_reg->CNDTR = num;
        bb_dma1_ch2_ccr_en = 1;    //继续dma传输
    }
    return 0;
}

// =============================================================================
//功能: 当uart3配置成dma方式是，dma1 ch2负责发送，ch3负责接收
//参数: 中断函数没有参数.
//返回: 中断函数没有返回值.
// =============================================================================
uint32_t UART3_DMA1CH3_ISR(ptu32_t tagIntLine)
{
    u32 recvs,num;
    pg_dma1_reg->IFCR |= 0x00000f00;
    bb_dma1_ch3_ccr_en = 0;    //暂停dma传输
    recvs = CFG_UART3_DMABUF_LEN - pg_dma1_channel3_reg->CNDTR;
    if(recvs == CFG_UART3_DMABUF_LEN)     //dma缓冲区满，可能有数据丢失
    {
        UART_ErrHandle(pUartCB[CN_UART3],CN_UART_FIFO_OVER_ERR);
    }else
    {
        num = UART_PortWrite(pUartCB[CN_UART3],UART3_DmaRecvBuf,recvs);
        if(num != recvs)
        {
            UART_ErrHandle(pUartCB[CN_UART3],CN_UART_BUF_OVER_ERR);
        }
    }
    pg_dma1_channel3_reg->CNDTR = CFG_UART3_DMABUF_LEN;
    pg_dma1_channel3_reg->CMAR = (u32)UART3_DmaRecvBuf;
    bb_dma1_ch3_ccr_en = 1;    //继续dma传输
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
    u32 num;
    u8 ch;

    UCB = pUartCB[port];
    Reg = (tagUartReg *)tg_UART_Reg[port];


    if((Reg->SR & (1<<5)) &&(Reg->CR1 & (1<<5))  )          //接收中断
    {
        ch = (u8)Reg->DR;
        num = UART_PortWrite(UCB,&ch,1);
        if(num != 1)
        {
            UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
        }
    }
    if((Reg->SR & (1<<6)) && (Reg->CR1&(1<<6)))
    {
        Reg->CR1 &=~(1<<6);//关TC中断
        Board_UartHalfDuplexRecv(port);
    }
    if((Reg->SR & (1<<7)) &&(Reg->CR1 & (1<<7)))    //发送中断
    {
        num = UART_PortRead(UCB,&ch,1);
        if(num != 0)
            Reg->DR = ch;
        else
        {
            Reg->CR1 &= ~(1<<7);        //txeie
            Reg->CR1 |=(1<<6);
        }
    }
    if(Reg->SR & (1<<3))        //ORE过载错误
    {
        ch = (u8)Reg->DR;
        num = Reg->SR;     //读一下sr寄存器
        UART_ErrHandle(UCB,CN_UART_FIFO_OVER_ERR);
    }
    if((Reg->SR & (1<<4)) && (Reg->CR1 & (1<<4)))
    {
        Reg->DR;
        Int_TapLine(CN_INT_LINE_DMA1_Ch5);
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

    switch(serial_no)
    {
    case CN_UART1://串口0
        UART1_DmaSendBuf=NULL ;
        UART1_DmaRecvBuf=NULL ;
        UART_Param.Name         = "UART1";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART1_BASE;
        UART_Param.TxRingBufLen = CFG_UART1_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART1_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART2://串口2
        UART2_DmaSendBuf=NULL ;
        UART2_DmaRecvBuf=NULL ;
        UART_Param.Name         = "UART2";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART2_BASE;
        UART_Param.TxRingBufLen = CFG_UART2_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART2_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART3://串口3
        UART3_DmaSendBuf=NULL ;
        UART3_DmaRecvBuf=NULL ;
        UART_Param.Name         = "UART3";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART3_BASE;
        UART_Param.TxRingBufLen = CFG_UART3_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART3_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART4://串口4
        UART_Param.Name         = "UART4";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART4_BASE;
        UART_Param.TxRingBufLen = CFG_UART4_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART4_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART5://串口5
        UART_Param.Name         = "UART5";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART5_BASE;
        UART_Param.TxRingBufLen = CFG_UART5_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART5_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    default:
        return 0;
    }
    UART_Param.mode = CN_UART_GENERAL;
    //硬件初始化
    __UART_HardInit(serial_no);
    __UART_IntInit(serial_no);

    sUartInited |= (0x01 << serial_no);
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
    PutStrDirectReg->CR1 &= ~(1<<7);                        //disable send INT
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
        PutStrDirectReg->DR = str[result];
    }
    PutStrDirectReg->CR1 = CR_Bak;                         //restore send INT
    while((PutStrDirectReg->SR &(1<<6))!=(1<<6))
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

    CR_Bak = GetCharDirectReg->CR1;                          //Save INT
    GetCharDirectReg->CR1 &= ~(1<<7);                        //disable send INT
    while(__UART_RxHadChar(GetCharDirectReg) == false) ;
    result = GetCharDirectReg->DR;
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
