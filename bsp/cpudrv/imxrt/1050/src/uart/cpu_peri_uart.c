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
// 模块版本: V1.0.0
// 创建人员: chj
// 创建时间: 2018-05-07
// =============================================================================
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "errno.h"
#include "systime.h"
#include "cpu_peri.h"
#include "cpu_peri_int_line.h"
#include <device/djy_uart.h>
#include "device.h"
#include "int.h"
#include "djyos.h"
#include "stdlib.h"
#include "cpu_peri_uart.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern ptu32_t ModuleInstall_UART(u8 port);
//    #if (CFG_UART1_ENABLE == 1)
//    ModuleInstall_UART(CN_UART1);
//    #endif
//    #if (CFG_UART2_ENABLE == 1)
//    ModuleInstall_UART(CN_UART2);
//    #endif
//    #if (CFG_UART3_ENABLE == 1)
//    ModuleInstall_UART(CN_UART3);
//    #endif
//    #if (CFG_UART4_ENABLE == 1)
//    ModuleInstall_UART(CN_UART4);
//    #endif
//    #if (CFG_UART5_ENABLE == 1)
//    ModuleInstall_UART(CN_UART5);
//    #endif
//    #if (CFG_UART6_ENABLE == 1)
//    ModuleInstall_UART(CN_UART6);
//    #endif
//    #if (CFG_UART7_ENABLE == 1)
//    ModuleInstall_UART(CN_UART7);
//    #endif
//    #if (CFG_UART8_ENABLE == 1)
//    ModuleInstall_UART(CN_UART8);
//    #endif

//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip uart"//CPU的uart外设驱动
//parent:"uart device file"      //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                  //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable               //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                 //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                //初始化时机，可选值：early，medium，later, pre-main。
                                 //表示初始化时间，分别是早期、中期、后期
//dependence:"uart device file","heap","lock","int"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                 //选中该组件时，被依赖组件将强制选中，
                                 //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:none            //该组件的弱依赖组件名（可以是none，表示无依赖组件），
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
#define CFG_UART1_SENDBUF_LEN    64             //"UART1发送缓冲大小",
#define CFG_UART1_RECVBUF_LEN    64             //"UART1接收缓冲大小",

#define CFG_UART2_SENDBUF_LEN    64             //"UART2发送缓冲大小",
#define CFG_UART2_RECVBUF_LEN    64             //"UART2接收缓冲大小",

#define CFG_UART3_SENDBUF_LEN    64             //"UART3发送缓冲大小",
#define CFG_UART3_RECVBUF_LEN    64             //"UART3接收缓冲大小",

#define CFG_UART4_SENDBUF_LEN    64             //"UART4发送缓冲大小",
#define CFG_UART4_RECVBUF_LEN    64             //"UART4接收缓冲大小",

#define CFG_UART5_SENDBUF_LEN    64             //"UART5发送缓冲大小",
#define CFG_UART5_RECVBUF_LEN    64             //"UART5接收缓冲大小",

#define CFG_UART6_SENDBUF_LEN    64             //"UART6发送缓冲大小",
#define CFG_UART6_RECVBUF_LEN    64             //"UART6接收缓冲大小",

#define CFG_UART7_SENDBUF_LEN    64             //"UART7发送缓冲大小",
#define CFG_UART7_RECVBUF_LEN    64             //"UART7接收缓冲大小",

#define CFG_UART8_SENDBUF_LEN    64             //"UART8发送缓冲大小",
#define CFG_UART8_RECVBUF_LEN    64             //"UART8接收缓冲大小",
//%$#@enum,true,false,
#define CFG_UART1_ENABLE         true           //"配置使用UART1",
#define CFG_UART2_ENABLE         false          //"配置使用UART2",
#define CFG_UART3_ENABLE         false          //"配置使用UART3",
#define CFG_UART4_ENABLE         false          //"配置使用UART4",
#define CFG_UART5_ENABLE         false          //"配置使用UART5",
#define CFG_UART6_ENABLE         false          //"配置使用UART6",
#define CFG_UART7_ENABLE         false          //"配置使用UART7",
#define CFG_UART8_ENABLE         false          //"配置使用UART8",
//%$#@string,1,30,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
//==============================================================================

//==============================================================================
extern u32 BOARD_DebugConsoleSrcFreq(void);//提供板件运行时钟频率
#define SRC_FREQ    BOARD_DebugConsoleSrcFreq()
// =============================================================================
#define CN_UART1_BASE (0x40184000u)
#define CN_UART2_BASE (0x40188000u)
#define CN_UART3_BASE (0x4018C000u)
#define CN_UART4_BASE (0x40190000u)
#define CN_UART5_BASE (0x40194000u)
#define CN_UART6_BASE (0x40198000u)
#define CN_UART7_BASE (0x4019C000u)
#define CN_UART8_BASE (0x401A0000u)

static u32 TxByteTime;                    //正常发送一个字节所需要的时间
static tagUartReg *PutStrDirectReg;     //用于printk发送的串口寄存器
static tagUartReg *GetCharDirectReg;     //用于直接接收的串口寄存器
static u32 TxDirectPort;                  //用于printk发送的串口号
static u32 RxDirectPort;                  //用于直接接收的串口号
// =============================================================================
static tagUartReg volatile * const tg_UART_Reg[] = {(LPUART_Type *)CN_UART1_BASE,
                                                     (LPUART_Type *)CN_UART2_BASE,
                                                     (LPUART_Type *)CN_UART3_BASE,
                                                     (LPUART_Type *)CN_UART4_BASE,
                                                     (LPUART_Type *)CN_UART5_BASE,
                                                     (LPUART_Type *)CN_UART6_BASE,
                                                     (LPUART_Type *)CN_UART7_BASE,
                                                     (LPUART_Type *)CN_UART8_BASE};

static lpuart_config_t tg_UART_Config[CN_UART_NUM];

#define CN_DMA_UNUSED           0U
#define CN_DMA_USED             1U

#define CN_UART_TX_RMPTY        2U
#define CN_UART_TX_COMPLETE     3U

static struct UartGeneralCB *pUartCB[CN_UART_NUM];
static u8 sUartInited = 0;
__attribute__((weak))  void Board_UartHalfDuplexSend(u8 port)
{
    return;
}
__attribute__((weak))  void Board_UartHalfDuplexRecv(u8 port)
{
    return ;
}

//static bool_t s_UART_DmaSending[] = {false,false,false,false,false,false};
static bool_t s_UART_DmaUsed[] = {false,false,false,false,false,false};

u32 UART_ISR(ptu32_t port);
// =============================================================================
// 功能: 使能uart的接受中断。
// 参数: flag,是否使用dma传输
//       port,串口号，1~8
// 返回: 无
// =============================================================================
void __UART_RxIntEnable(u8 flag,u8 port)
{
    if(flag == CN_DMA_USED){}
    else
        LPUART_EnableInterrupts((tagUartReg *)tg_UART_Reg[port], \
                kLPUART_RxDataRegFullInterruptEnable);
}

// =============================================================================
// 功能: 禁止uart的接收中断
// 参数: flag,是否使用dma传输
//       port,串口号
// 返回: 无
// =============================================================================
void __UART_RxIntDisable(u8 flag, u8 port)
{
    if(flag == CN_DMA_USED){}
    else
        LPUART_DisableInterrupts((tagUartReg *)tg_UART_Reg[port], \
                kLPUART_RxDataRegFullInterruptEnable);
}

// =============================================================================
// 功能: 使能uart的发送中断
// 参数: flag,是否使用dma传输
//       port,串口号
//       int_flag,中断类型：发送寄存器空中断以及发送完成中断
// 返回: 无
// =============================================================================
static void __UART_TxIntEnable(u8 flag,u8 port,u8 int_flag)
{
    if(flag == CN_DMA_USED){}
    else
    {
        if(int_flag==CN_UART_TX_RMPTY)
            LPUART_EnableInterrupts((tagUartReg *)tg_UART_Reg[port], \
                    kLPUART_TxDataRegEmptyInterruptEnable);
        else if(int_flag==CN_UART_TX_COMPLETE)
            LPUART_EnableInterrupts((LPUART_Type *)tg_UART_Reg[port], \
                    kLPUART_TransmissionCompleteInterruptEnable);
    }
}

// =============================================================================
// 功能: 禁止UART的发送中断
// 参数: flag,是否使用dma传输
//       port,串口号
//      int_flag,中断类型：发送寄存器空中断以及发送完成中断
// 返回: 无
// =============================================================================
static void __UART_TxIntDisable(u8 flag, u8 port,u8 int_flag)
{
    if(flag == CN_DMA_USED){}
    else
    {
        if(int_flag==CN_UART_TX_RMPTY)
            LPUART_DisableInterrupts((tagUartReg *)tg_UART_Reg[port], \
                    kLPUART_TxDataRegEmptyInterruptEnable);
        else if(int_flag==CN_UART_TX_COMPLETE)
            LPUART_DisableInterrupts((tagUartReg *)tg_UART_Reg[port], \
                    kLPUART_TransmissionCompleteInterruptEnable);
    }
}

// =============================================================================
// 功能: 检查发送寄存器的状态，如果空就返回true，若为空，否则返回false
// 参数：port,串口号
// 返回: true = 缓冲区空，false = 非空
// =============================================================================
static bool_t __UART_TxTranEmpty(tagUartReg volatile *reg)
{
    if((kLPUART_TxFifoEmptyFlag)&\
            LPUART_GetStatusFlags((tagUartReg *)reg))
    {
        return true;
    }
    else
    {
        return false;
    }
    /*
    if (!(reg->STAT & LPUART_STAT_TDRE_MASK))
        return false;
    else
        return true;
    */
}

// =============================================================================
// 功能: 检查发送寄存器的状态，如果发送完成就返回true，否则返回false
// 参数：port,串口号
// 返回: true = 发送已完成，false = 发送未完成
// =============================================================================
static bool_t __UART_TxTranComplete(tagUartReg volatile *reg)
{
    if((kLPUART_TransmissionCompleteFlag)&\
            LPUART_GetStatusFlags((tagUartReg *)reg))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// =============================================================================
// 功能: 检查接收寄存器的状态，有数据就返回true，否则返回false
// 参数：port,串口号
// 返回: true = 有数据，false = 无数据
// =============================================================================
static bool_t __UART_RxHadChar(tagUartReg volatile *reg)
{
    if (kLPUART_RxFifoEmptyFlag & \
                LPUART_GetStatusFlags((tagUartReg *)reg))
        return false;
    else
        return true;
}

// =============================================================================
// 功能: 设置串口波特率
// 参数:
//       port,串口号
//       baud,波特率
// 返回: 无
//注：SRC_FREQ函数提供板件的运行时钟频率，应由工程方根据实际运行时钟提供
// =============================================================================
static void __UART_BaudSet(u8 port,u32 baud)
{
    tg_UART_Config[port].baudRate_Bps = baud;
    LPUART_SetBaudRate((tagUartReg *)tg_UART_Reg[port], baud, \
            SRC_FREQ);
    if(TxDirectPort == port)
    {
        TxByteTime = 11000000/baud;     //1个字节传输时间，按10bit，+10%计算
    }
}

// =============================================================================
// 功能: 对串口传输参数配置，包括波特率、奇偶校验、数据位、停止位
// 参数:
//        port,串口号
//        data,结构体tagCOMParam类型的指针数值
// 返回: 无
// =============================================================================
static void __UART_ComConfig(tagUartReg volatile * reg,u8 port,struct COMParam *COM)
{
    struct COMParam *COM;

    if(tg_UART_Reg[port]!=reg)
        return;
    if(COM == NULL)
        return;

     LPUART_GetDefaultConfig(&tg_UART_Config[port]);
     tg_UART_Config[port].baudRate_Bps = COM->BaudRate;
     switch(COM->DataBits)               // data bits
     {
         case CN_UART_DATABITS_7:    tg_UART_Config[port].dataBitsCount = kLPUART_SevenDataBits;
                                     break;
         case CN_UART_DATABITS_8:    tg_UART_Config[port].dataBitsCount = kLPUART_EightDataBits;
                                     break;
         default:break;
     }

     switch(COM->Parity)                 // parity
     {
         case CN_UART_PARITY_ODD:    tg_UART_Config[port].parityMode = kLPUART_ParityOdd;
                                     break;
         case CN_UART_PARITY_EVEN:   tg_UART_Config[port].parityMode = kLPUART_ParityEven;
                                     break;
         default:break;
     }

     switch(COM->StopBits)               // stop bits
     {
         case CN_UART_STOPBITS_1:    tg_UART_Config[port].stopBitCount = kLPUART_OneStopBit;
                                     break;
         case CN_UART_STOPBITS_2:    tg_UART_Config[port].stopBitCount = kLPUART_TwoStopBit;
                                     break;
         default:break;
     }
}

// =============================================================================
// 功能: 使串口开始工作，时钟和uart模块
// 参数: port,串口号
// 返回: 无
// =============================================================================
void __UART_Enable(u8 port)
{
    tg_UART_Config[port].enableTx = true;
    tg_UART_Config[port].enableRx = true;

    LPUART_Init((tagUartReg *)tg_UART_Reg[port], \
                 &tg_UART_Config[port], SRC_FREQ);
}

// =============================================================================
// 功能: 使串口停止工作，包括时钟和uart模块
// 参数: port,串口号
// 返回: 无
// =============================================================================
void __UART_Disable(u8 port)
{
    tg_UART_Config[port].enableTx = false;
    tg_UART_Config[port].enableRx = false;

    LPUART_Deinit((tagUartReg *)tg_UART_Reg[port]);
}

// =============================================================================
// 功能: 初始化UART对应的中断线，并初始化中断入口函数
// 参数：SerialNo,串口号
// 返回: 无
// =============================================================================
static void __UART_IntInit(u8 port)
{
    u8 IntLine;
    if(port == CN_UART1)
        IntLine = CN_INT_LINE_LPUART1;
    else if(port == CN_UART2)
        IntLine = CN_INT_LINE_LRUART2;
    else if(port == CN_UART3)
        IntLine = CN_INT_LINE_LRUART3;
    else if(port == CN_UART4)
        IntLine = CN_INT_LINE_LRUART4;
    else if(port == CN_UART5)
        IntLine = CN_INT_LINE_LPUART5;
    else if(port == CN_UART6)
        IntLine = CN_INT_LINE_LPUART6;
    else if(port == CN_UART7)
        IntLine = CN_INT_LINE_LPUART7;
    else if(port == CN_UART8)
        IntLine = CN_INT_LINE_LPUART8;
    else
        return;

    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,UART_ISR);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
    Int_SetIsrPara(IntLine,port);
}

// =============================================================================
// 功能: 硬件参数配置和寄存器的初始化，包括波特率、停止位、校验位、数据位，默认情况下:
//       波特率:115200  ； 停止位:1 ; 校验:偶校验 ; 数据位:8bit
// 参数: port,串口号，0~7
// 返回: 无
// =============================================================================
static void __UART_HardInit(u8 port)
{
    if(port > CN_UART8)
        return;
    //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
   //初始化uart硬件控制数据结构
   LPUART_GetDefaultConfig(&tg_UART_Config[port]);
   tg_UART_Config[port].baudRate_Bps = 115200;
   //tg_UART_Config[port].enableTx = true;
   //tg_UART_Config[port].enableRx = true;
   //LPUART_Init(tg_UART_Reg[port], &tg_UART_Config[port],
    //     SRC_FREQ);
}

// =============================================================================
// 功能: 启动串口发送，其目的是触发中断，用中断方式发送数据。
// 参数: port:串口号.
// 返回:
// =============================================================================
static u32 __UART_SendStart (tagUartReg *reg)
{
    u8 port=0;
    //u32 delay=0;

    switch((u32)reg)
    {
        case CN_UART1_BASE:        port = CN_UART1;    break;
        case CN_UART2_BASE:        port = CN_UART2;    break;
        case CN_UART3_BASE:        port = CN_UART3;    break;
        case CN_UART4_BASE:        port = CN_UART4;    break;
        case CN_UART5_BASE:        port = CN_UART5;    break;
        case CN_UART6_BASE:        port = CN_UART6;    break;
        case CN_UART7_BASE:        port = CN_UART7;    break;
        case CN_UART8_BASE:        port = CN_UART8;    break;
        default:return 0;
    }
    Board_UartHalfDuplexSend(port);//切换到发送
    __UART_TxIntDisable(s_UART_DmaUsed[port],port,\
            CN_UART_TX_RMPTY);
    if(s_UART_DmaUsed[port])
    {
        /*__UART_DMA_SendStart(port);*/
    }
    else
    {
        //非DMA方式，只要使能发送空中断，则触发中断
    }

    __UART_TxIntEnable(s_UART_DmaUsed[port],port,\
            CN_UART_TX_RMPTY);
    return 1;
}

// =============================================================================
// 功能: 串口设备的控制函数,与具体的硬件寄存器设置相关
// 参数: port,串口号.
//       cmd,操作类型
//       data1,data2,含义依cmd而定
// 返回: 无意义.
// =============================================================================
static ptu32_t __UART_Ctrl(tagUartReg *reg,u32 cmd, va_list *arg0)
{
    u8 port=0;
    ptu32_t result = 0;

    if(reg == NULL)
        return 0;

    switch((u32)reg)
    {
        case CN_UART1_BASE: port = CN_UART1;break;
        case CN_UART2_BASE: port = CN_UART2;break;
        case CN_UART3_BASE: port = CN_UART3;break;
        case CN_UART4_BASE: port = CN_UART4;break;
        case CN_UART5_BASE: port = CN_UART5;break;
        case CN_UART6_BASE: port = CN_UART6;break;
        case CN_UART7_BASE: port = CN_UART7;break;
        case CN_UART8_BASE: port = CN_UART8;break;
        default:return 0;
    }

    switch(cmd)
    {
        case CN_DEV_CTRL_START:
            __UART_Enable(port);
            __UART_RxIntEnable(CN_DMA_UNUSED,port);
            break;
        case CN_DEV_CTRL_STOP:
            __UART_RxIntDisable(CN_DMA_UNUSED,port);
            __UART_Disable(port);
            break;
        case CN_UART_SET_BAUD:  //设置Baud
        {
            u32 data;
            data = va_arg(*arg0, u32);
            __UART_BaudSet(port, data);
        }
            break;
        case CN_UART_EN_RTS:
            tg_UART_Config[port].enableRxRTS = true;
            reg->MODIR |= LPUART_MODIR_RXRTSE_MASK;
            break;
        case CN_UART_DIS_RTS:
            tg_UART_Config[port].enableRxRTS = false;
            reg->MODIR &= ~LPUART_MODIR_RXRTSE_MASK;
            break;
        case CN_UART_EN_CTS:
            tg_UART_Config[port].enableTxCTS = true;
            reg->MODIR |= LPUART_MODIR_TXCTSE_MASK;
            break;
        case CN_UART_DIS_CTS:
            tg_UART_Config[port].enableTxCTS = false;
            reg->MODIR &= ~LPUART_MODIR_TXCTSE_MASK;
            break;
/*
        case CN_UART_DMA_USED:
            __UART_SetDmaUsed(port);
            break;
        case CN_UART_DMA_UNUSED:
            __UART_SetDmaUnUsed(port);
            break;
*/
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
// 功能: UART的中断服务函数，包括发送空、接收、错误三种中断。
//      1.接收中断，从接收FIFO中读数据，并调用UART_PortWrite写入接收缓冲区中；
//      2.发送中断，调用UART_PortRead读发送缓冲区中数据，并写入发送FIFO；
//      3.错误中断，弹出错误事件
// 参数: port:串口号.
// 返回: 0.
// =============================================================================
u32 UART_ISR(ptu32_t port)
{
    struct UartGeneralCB *UCB;
    u32 num;
    u8 ch;

    UCB = pUartCB[port];
    if ((kLPUART_RxDataRegFullFlag)&    \
            LPUART_GetStatusFlags((tagUartReg *)tg_UART_Reg[port]))//接收中断
    {
        ch = LPUART_ReadByte((tagUartReg *)tg_UART_Reg[port]);
        num = UART_PortWrite(UCB,&ch,1);
        if(num != 1)
        {
            UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
        }
    }
    if((kLPUART_NoiseErrorFlag|kLPUART_FramingErrorFlag|kLPUART_ParityErrorFlag) &  \
            LPUART_GetStatusFlags((tagUartReg *)tg_UART_Reg[port])) /*发生错误*/
    {
        Board_UartHalfDuplexRecv(port);
    }
    if((kLPUART_TxDataRegEmptyFlag)&
            LPUART_GetStatusFlags((tagUartReg *)tg_UART_Reg[port]))
    {
        num = UART_PortRead(UCB,&ch,1);
        if(num != 0)
            LPUART_WriteByte((tagUartReg *)tg_UART_Reg[port], ch);
        else
        {
            LPUART_DisableInterrupts((tagUartReg *)tg_UART_Reg[port], \
                        kLPUART_TxDataRegEmptyInterruptEnable);
        }
        LPUART_ClearStatusFlags((tagUartReg *)tg_UART_Reg[port], \
                kLPUART_TxDataRegEmptyInterruptEnable);
    }
    if((kLPUART_TxFifoOverflowFlag|kLPUART_RxFifoUnderflowFlag)&    \
            LPUART_GetStatusFlags((tagUartReg *)tg_UART_Reg[port])) //ORE过载错误
    {
        UART_ErrHandle(UCB,CN_UART_FIFO_OVER_ERR);
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
ptu32_t ModuleInstall_UART(u8 port)
{
     struct UartParam UART_Param;

     switch(port)
     {
        case CN_UART1://串口1
            UART_Param.Name         = "UART1";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CN_UART1_BASE;
            UART_Param.TxRingBufLen = CFG_UART1_SENDBUF_LEN;
            UART_Param.RxRingBufLen = CFG_UART1_RECVBUF_LEN;
            UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
            UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
            break;
        case CN_UART2://串口2
            UART_Param.Name         = "UART2";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CN_UART2_BASE;
            UART_Param.TxRingBufLen = CFG_UART2_SENDBUF_LEN;
            UART_Param.RxRingBufLen = CFG_UART2_RECVBUF_LEN;
            UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
            UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
            break;

        case CN_UART3://串口3
            UART_Param.Name         = "UART3";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CFG_UART3_SENDBUF_LEN;
            UART_Param.TxRingBufLen = CFG_UART3_RECVBUF_LEN;
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
        case CN_UART6://串口6
            UART_Param.Name         = "UART6";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CN_UART6_BASE;
            UART_Param.TxRingBufLen = CFG_UART6_SENDBUF_LEN;
            UART_Param.RxRingBufLen = CFG_UART6_RECVBUF_LEN;
            UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
            UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
            break;
        case CN_UART7://串口7
            UART_Param.Name         = "UART7";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CN_UART7_BASE;
            UART_Param.TxRingBufLen = CFG_UART7_SENDBUF_LEN;
            UART_Param.RxRingBufLen = CFG_UART7_RECVBUF_LEN;
            UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
            UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
            break;
        case CN_UART8://串口8
            UART_Param.Name         = "UART8";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CN_UART8_BASE;
            UART_Param.TxRingBufLen = CFG_UART8_SENDBUF_LEN;
            UART_Param.RxRingBufLen = CFG_UART8_RECVBUF_LEN;
            UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
            UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
            break;
        default:
            return 0;
     }
     UART_Param.mode = CN_UART_GENERAL;
     /*
     pUART_DmaSendBuf[serial_no]=NULL;
     pUART_DmaRecvBuf[serial_no][0]=NULL;
     pUART_DmaRecvBuf[serial_no][1]=NULL;
     */
     Board_UartHalfDuplexRecv(port);
     //硬件初始化
     __UART_HardInit(port);
     __UART_IntInit(port);
     __UART_Enable(port);
     __UART_RxIntEnable(CN_DMA_UNUSED,port);
     sUartInited |= (0x01 << port);
     pUartCB[port] = UART_InstallGeneral(&UART_Param);
     if( pUartCB[port] == NULL)
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
    u32 CR_Bak;
    u8 port;
    for(port=0;port<CN_UART_NUM;port++)
    {
        if(PutStrDirectReg ==tg_UART_Reg[port])
            break;
    }
    Board_UartHalfDuplexSend(port);
    CR_Bak = LPUART_GetEnabledInterrupts(PutStrDirectReg);                      //Save INT
    LPUART_DisableInterrupts(PutStrDirectReg,\
        (kLPUART_TxDataRegEmptyInterruptEnable|kLPUART_TransmissionCompleteInterruptEnable));//disable send INT
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
        PutStrDirectReg->DATA = *(u8*)(str+result);
    }
    LPUART_EnableInterrupts(PutStrDirectReg,CR_Bak); //restore send INT
    while(!__UART_TxTranComplete(PutStrDirectReg))
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

    CR_Bak = LPUART_GetEnabledInterrupts(GetCharDirectReg);               //Save INT
    LPUART_DisableInterrupts(GetCharDirectReg,\
            (kLPUART_TxDataRegEmptyInterruptEnable|kLPUART_TransmissionCompleteInterruptEnable));//disable send INT
    while(__UART_RxHadChar(GetCharDirectReg) == false)
    {
        DJY_EventDelay(5*mS);
    }
    result=LPUART_ReadByte(GetCharDirectReg);
    LPUART_EnableInterrupts(GetCharDirectReg,CR_Bak); //restore send INT
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
    else
    {
        PutStrDirectReg = NULL ;
    }

    if(PutStrDirectReg != NULL)
    {
        __UART_HardInit(TxDirectPort);
        __UART_Enable(TxDirectPort);
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
    else
    {
        GetCharDirectReg = NULL ;
    }
    if(GetCharDirectReg != NULL)
    {
        if(TxDirectPort != RxDirectPort)
        {
            __UART_HardInit(RxDirectPort);
            __UART_Enable(RxDirectPort);
        }
        GetCharDirect = Uart_GetCharDirect;
    }
    return;
}

