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

#include "cpu_peri.h"
#include "stdio.h"
#include <device/djy_uart.h>
#include "string.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    ptu32_t ModuleInstall_UART(ptu32_t SerialNo);
//    #if CFG_UART0_ENABLE ==1
//    ModuleInstall_UART(CN_UART0);
//    #endif
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

//    #if CFG_UART4_ENABLE ==1
//    ModuleInstall_UART(CN_UART4);
//    #endif

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
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"device file system","lock","uart device file","cpu onchip xdma","heap"//该组件的依赖组件名（可以是none，表示无依赖组件），
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
#define CFG_UART0_SENDBUF_LEN    32             //"UART0发送缓冲区大小",
#define CFG_UART0_RECVBUF_LEN    32             //"UART0接收缓冲区大小",

#define CFG_UART1_SENDBUF_LEN    32             //"UART1发送缓冲大小",
#define CFG_UART1_RECVBUF_LEN    32             //"UART1接收缓冲大小",

#define CFG_UART2_SENDBUF_LEN    32             //"UART2发送缓冲大小",
#define CFG_UART2_RECVBUF_LEN    32             //"UART2接收缓冲大小",

#define CFG_UART3_SENDBUF_LEN    32             //"UART3发送缓冲大小",
#define CFG_UART3_RECVBUF_LEN    32             //"UART3接收缓冲大小",

#define CFG_UART4_SENDBUF_LEN    32             //"UART4发送缓冲大小",
#define CFG_UART4_RECVBUF_LEN    32             //"UART4接收缓冲大小",

#define CFG_UART5_SENDBUF_LEN    32             //"UART5发送缓冲大小",
#define CFG_UART5_RECVBUF_LEN    32             //"UART5接收缓冲大小",


//%$#@enum,true,false,
#define CFG_UART0_ENABLE         true           //"是否配置UART0",
#define CFG_UART1_ENABLE         false          //"是否配置UART1",
#define CFG_UART2_ENABLE         false          //"是否配置UART2",
#define CFG_UART3_ENABLE         false          //"是否配置UART3",
#define CFG_UART4_ENABLE         false          //"是否配置UART4",
#define CFG_UART5_ENABLE         false          //"是否配置UART4",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

// =============================================================================
#define tagUartReg     UART_Type

static tagUartReg volatile * const tg_UART_Reg[] = {(tagUartReg *)CN_UART0_BASE,
                                                    (tagUartReg *)CN_UART1_BASE,
                                                    (tagUartReg *)CN_UART2_BASE,
                                                    (tagUartReg *)CN_UART3_BASE,
                                                    (tagUartReg *)CN_UART4_BASE,
                                                    (tagUartReg *)CN_UART5_BASE};

static u32 TxByteTime;                    //正常发送一个字节所需要的时间
static tagUartReg *PutStrDirectReg;     //用于printk发送的串口寄存器
static tagUartReg *GetCharDirectReg;     //用于直接接收的串口寄存器
static u32 TxDirectPort;                  //用于printk发送的串口号
static u32 RxDirectPort;                  //用于直接接收的串口号
// =============================================================================

//#define UART0_SendBufLen            32
//#define UART0_RecvBufLen            32

//#define UART1_SendBufLen            32
//#define UART1_RecvBufLen            32

//#define UART2_SendBufLen            32
//#define UART2_RecvBufLen            32

//#define UART3_SendBufLen            32
//#define UART3_RecvBufLen            32

//#define UART4_SendBufLen            32
//#define UART4_RecvBufLen            32

//#define UART5_SendBufLen            32
//#define UART5_RecvBufLen            32
static struct UartGeneralCB *pUartCB[CN_UART_NUM];

//用于标识串口是否初始化标记，第0位表示UART0，第一位表UART1....
//依此类推，1表示初始化，0表示未初始化
static u8 sUartInited = 0;
// =============================================================================
static ptu32_t UART_ISR(ptu32_t IniLine);

__attribute__((weak))  void Board_UartHalfDuplexSend(u8 SerialNo)
{
    return;
}
__attribute__((weak))  void Board_UartHalfDuplexRecv(u8 SerialNo)
{
    switch (SerialNo)
    {
        case CN_UART0:
            break;
        default:
            break;
    }
}
// ----使能接收中断-------------------------------------------------------------
// 功能: 使能UART的接收中断
// 参数: reg,被操作的寄存器组指针
// 返回: 无
//-----------------------------------------------------------------------------
static void __UART_RxIntEnable(tagUartReg volatile *reg)
{
    reg->C2 |= UART_C2_RIE_MASK;
}

// ----禁止接收中断-------------------------------------------------------------
// 功能: 禁止UART的接收中断
// 参数: reg,被操作的寄存器组指针
// 返回: 无
//-----------------------------------------------------------------------------
static void __UART_RxIntDisable(tagUartReg volatile *reg)
{
    reg->C2 &= ~UART_C2_RIE_MASK;
}

// ----使能发送中断-------------------------------------------------------------
// 功能: 使能UART的发送中断
// 参数: reg,被操作的寄存器组指针
// 返回: 无
// -----------------------------------------------------------------------------
static void __UART_TxIntEnable(tagUartReg volatile *reg)
{
    reg->C2 |= UART_C2_TIE_MASK;
}

// ----禁止发送中断-------------------------------------------------------------
// 功能: 禁止UART的发送中断
// 参数: reg,被操作的寄存器组指针
// 返回: 无
// -----------------------------------------------------------------------------
static void __UART_TxIntDisable(tagUartReg volatile *reg)
{
    reg->C2 &= ~UART_C2_TIE_MASK;
}

// ----检查串口发送寄存器是否空--------------------------------------------------
// 功能: 检查发送寄存器的状态，如果空就返回true，相反，则返回false
// 参数：无。
// 返回: true = 缓冲区空，false = 非空
// -----------------------------------------------------------------------------
static bool_t __UART_TxTranEmpty(tagUartReg volatile *reg)
{
    return (0 != (reg->S1 & UART_S1_TDRE_MASK));
}

// =============================================================================
// 功能: 检查接收寄存器的状态，有数据就返回true，否则返回false
// 参数：reg,被操作的寄存器组指针
// 返回: true = 有数据，false = 无数据
// =============================================================================
static bool_t __UART_RxHadChar(tagUartReg volatile *reg)
{
    return reg->RCFIFO; //接收数据量，如果有数据返回true,否则false
}

// =============================================================================
// 功能: 设置串口串口的波特率，FreescaleKxx的UART0和UART1时钟源为CORE，其他的时钟源
//       为BUS时钟
// 参数: Reg,被操作的寄存器组指针
//       baud,波特率
// 返回: 无
// =============================================================================
static void __UART_BaudSet(tagUartReg volatile *Reg,u32 baud)
{
    u32 UartClk;
    u16 Sbr,Brfa;
    u8 temp;

    if(Reg == NULL)     return;

    if(((u32)Reg == CN_UART0_BASE) || ((u32)Reg == CN_UART1_BASE))
        UartClk = CN_CFG_MCLK;
    else
        UartClk = CN_CFG_MCLK/2;

    Sbr = (u16)(UartClk/(baud * 16));
    Brfa = (((UartClk*32)/(baud * 16)) - (Sbr * 32));
    //配置波特率
    temp = Reg->BDH & ~(UART_BDH_SBR(0x1F));
    Reg->BDH = temp | UART_BDH_SBR(((Sbr & 0x1F00) >> 8));
    Reg->BDL = (u8)(Sbr & UART_BDL_SBR_MASK);
    //用于校正
    temp = Reg->C4 & ~(UART_C4_BRFA(0x1F));
    Reg->C4 = temp |  UART_C4_BRFA(Brfa);

    if(tg_UART_Reg[TxDirectPort] == Reg)
    {
        TxByteTime = 11000000/baud;     //1个字节传输时间，按10bit，+10%计算
    }
}

// =============================================================================
// 功能: 设置对应UART的IO口，包括时钟和IO配置
// 参数: SerialNo,串口号
// 返回: 无
// =============================================================================
static void __UART_GpioConfig(u8 SerialNo)
{
    GPIO_PowerOn(1);
    switch(SerialNo)
    {
    case CN_UART0:
        SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
        PORT_MuxConfig(PORT_PORT_A,PORT_PIN(1),PORT_PINMUX_ALT2);
        PORT_MuxConfig(PORT_PORT_A,PORT_PIN(2),PORT_PINMUX_ALT2);
        break;
    case CN_UART1:
        SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(0),PORT_PINMUX_ALT3);
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(1),PORT_PINMUX_ALT3);
        break;
    case CN_UART2:
        SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(16),PORT_PINMUX_ALT3);
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(17),PORT_PINMUX_ALT3);
        break;
    case CN_UART3:
        SIM->SCGC4 |= SIM_SCGC4_UART3_MASK;
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(4),PORT_PINMUX_ALT3);
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(5),PORT_PINMUX_ALT3);
        break;
    case CN_UART4:
        SIM->SCGC1 |= SIM_SCGC1_UART4_MASK;
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(24),PORT_PINMUX_ALT3);
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(25),PORT_PINMUX_ALT3);
        break;
    case CN_UART5:
        SIM->SCGC1 |= SIM_SCGC1_UART5_MASK;
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(8),PORT_PINMUX_ALT3);
        PORT_MuxConfig(PORT_PORT_E,PORT_PIN(9),PORT_PINMUX_ALT3);
        break;
    default:
        break;
    }
}

// =============================================================================
// 功能: 对串口传输参数配置，包括波特率、奇偶校验、数据位、停止位
// 参数:  Reg,被操作的寄存器组指针
//        data,结构体tagCOMParam类型的指针数值
// 返回: 无
// =============================================================================
static void __UART_ComConfig(tagUartReg volatile *Reg,struct COMParam *COM)
{
    if((COM == NULL) || (Reg == NULL))
        return;
    __UART_BaudSet(Reg,COM->BaudRate);

    switch(COM->DataBits)               // data bits
    {
    case CN_UART_DATABITS_8:    Reg->C1 &= ~UART_C1_M_MASK;break;
    case CN_UART_DATABITS_9:    Reg->C1 = UART_C1_M_MASK; break;
    default:break;
    }

    switch(COM->Parity)                 // parity
    {
    Reg->C1 |= UART_C1_PE_MASK;
    case CN_UART_PARITY_ODD:    Reg->C1 |= UART_C1_PT_MASK;break;
    case CN_UART_PARITY_EVEN:   Reg->C1 &=~UART_C1_PT_MASK;break;
    default:break;
    }

    switch(COM->StopBits)               // stop bits
    {
    default:break;                      //k60没找到停止位配置
    }
}

// =============================================================================
// 功能: 初始化UART对应的中断线，并初始化中断入口函数
// 参数：SerialNo,串口号
// 返回: 无
// =============================================================================
static void __UART_IntInit(u32 SerialNo)
{
    u8 IntLine;
    if(SerialNo == CN_UART0)
        IntLine = CN_INT_LINE_UART0_RX_TX;
    else if(SerialNo == CN_UART1)
        IntLine = CN_INT_LINE_UART1_RX_TX;
    else if(SerialNo == CN_UART2)
        IntLine = CN_INT_LINE_UART2_RX_TX;
    else if(SerialNo == CN_UART3)
        IntLine = CN_INT_LINE_UART3_RX_TX;
    else if(SerialNo == CN_UART4)
        IntLine = CN_INT_LINE_UART4_RX_TX;
    else if(SerialNo == CN_UART5)
        IntLine = CN_INT_LINE_UART5_RX_TX;
    else
        return;

    if(true == Int_Register(IntLine))
    {
        Int_IsrConnect(IntLine,UART_ISR);
        Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
        Int_SettoAsynSignal(IntLine);
        Int_ClearLine(IntLine);
        Int_RestoreAsynLine(IntLine);
    }
}

// =============================================================================
// 功能: 硬件参数配置和寄存器的初始化，包括波特率、停止位、校验位、数据位，默认情况下:
//       波特率:115200  ； 停止位:1 ; 校验:偶校验 ; 数据位:8bit
// 参数: SerialNo,串口号，0~5
// 返回: 无
// =============================================================================
static void __UART_HardInit(u8 SerialNo)
{
    if(SerialNo > CN_UART5)
        return;

    //GPIO的配置
    __UART_GpioConfig(SerialNo);
    __UART_BaudSet(tg_UART_Reg[SerialNo],115200);

    tg_UART_Reg[SerialNo]->C2 |= UART_C2_TIE_MASK;      //使能TIE中断
    tg_UART_Reg[SerialNo]->C2 |= UART_C2_RIE_MASK;      //使能RIE中断
//  tg_UART_Reg[SerialNo]->C2 |= UART_C2_ILIE_MASK;     //使能ILIE中断
    tg_UART_Reg[SerialNo]->PFIFO = UART_PFIFO_TXFE_MASK | UART_PFIFO_RXFE_MASK;
    tg_UART_Reg[SerialNo]->C2 |= UART_C2_RE_MASK;       //使能接收
    tg_UART_Reg[SerialNo]->C2 |= UART_C2_TE_MASK;       //使能发送
}

// =============================================================================
// 功能: 直接发送函数，使用轮询方式。
// 参数: PrivateTag,被操作的串口设备指针.
//       send_buf,发送数据缓冲区
//       len,数据长度，字节
//       timeout,超时时间，us
// 返回: 发送字节数
// =============================================================================
//static u32 __UART_SendDirectly(tagUartReg *Reg,u8 *send_buf,u32 len,u32 timeout)
//{
//    u32  result;
//
//    if(NULL == Reg)     return 0;
//    __UART_TxIntDisable(Reg);
//    for(result=0; result < len; result ++)
//    {
//        // 超时或者发送缓冲为空时退出
//        while((false == __UART_TxTranEmpty(Reg))&& (timeout > 0))
//        {
//            timeout--;
//            DJY_DelayUs(1);
//        }
//        if(timeout == 0)
//            break;
//        Reg->D = send_buf[result];
//    }
//    __UART_TxIntEnable(Reg);
//    return result;
//}

// =============================================================================
// 功能: 启动串口发送，其目的是触发中断，用中断方式发送数据。
// 参数: PrivateTag,被操作的串口设备指针数值.
// 返回: 发送的个数
// =============================================================================
static u32 __UART_SendStart (tagUartReg *Reg)
{
    u8 fifodep=1,num,ch[8],port;

    switch((u32)Reg)
    {
    //UART0和UART1的FIFO深度为8，而其他的为1
    case CN_UART0_BASE:   port = CN_UART0;   fifodep = 8;  break;
    case CN_UART1_BASE:   port = CN_UART1;   fifodep = 8;  break;
    case CN_UART2_BASE:   port = CN_UART2;   break;
    case CN_UART3_BASE:   port = CN_UART3;   break;
    case CN_UART4_BASE:   port = CN_UART4;   break;
    case CN_UART5_BASE:   port = CN_UART5;   break;
    default:return 0;
    }
    Board_UartHalfDuplexSend(port);
    __UART_TxIntDisable(Reg);
    if(__UART_TxTranEmpty(Reg))
    {
        fifodep = UART_PortRead(pUartCB[port],ch,fifodep);// UART的FIFO大小为8字节
        for(num = 0; num < fifodep; num++)
        {
            Reg->D = ch[num];
        }
    }
    __UART_TxIntEnable(Reg);
    return num;
}

//----串口设备控制函数---------------------------------------------------------
//功能: 串口设备的控制函数,与具体的硬件寄存器设置相关
//参数: PrivateTag,被操作的串口设备指针数值.
//      cmd,操作类型
//      data,含义依cmd而定
//返回: 无意义.
//-----------------------------------------------------------------------------
static ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd, va_list *arg0)
{

    switch(cmd)
    {
        case CN_DEV_CTRL_START:
            Reg->C2 |= UART_C2_TE_MASK;
            Reg->C2 |= UART_C2_RE_MASK;
            break;
        case CN_DEV_CTRL_STOP:
            Reg->C2 &= ~UART_C2_TE_MASK;
            Reg->C2 &= ~UART_C2_RE_MASK;
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
            __UART_BaudSet(Reg, data);
        }
            break;
        case CN_UART_RX_PAUSE:      //暂停接收
            __UART_RxIntDisable(Reg);
            break;
        case CN_UART_RX_RESUME:      //恢复接收
            __UART_RxIntEnable(Reg);
            break;
        case CN_UART_RECV_HARD_LEVEL:    //设置接收fifo触发水平
            Reg->RWFIFO = data1;    //读触发水平
            Reg->TWFIFO = data2;    //写触发水平
            break;
        case CN_UART_COM_SET:
        {
            struct COMParam *COM;
            COM = va_arg(*arg0, void *);
            __UART_ComConfig(Reg,COM);
        }
            break;
        default: break;
    }
    return 0;
}

// =============================================================================
// 功能: UART的中断服务函数，包括发送空、接收、错误三种中断。
//      1.接收中断，从接收FIFO中读数据，并调用UART_PortWrite写入接收缓冲区中；
//      2.发送中断，调用UART_PortRead读发送缓冲区中数据，并写入发送FIFO；
//      3.错误中断，弹出错误事件
// 参数: 中断线号.
// 返回: 0.
// =============================================================================
static u32 UART_ISR(ptu32_t IntLine)
{
    struct UartGeneralCB *UCB;
    volatile tagUartReg *Reg;
    u32 i,fifodep=1,num=0;
    u8 ch[16];
    u8 port;

    switch(IntLine)
    {
    case CN_INT_LINE_UART0_RX_TX:
    case CN_INT_LINE_UART0_ERR:
        UCB = pUartCB[CN_UART0];
        Reg = tg_UART_Reg[CN_UART0];
        fifodep = 8;
        port = CN_UART0;
        break;
    case CN_INT_LINE_UART1_RX_TX:
    case CN_INT_LINE_UART1_ERR:
        UCB = pUartCB[CN_UART1];
        Reg = tg_UART_Reg[CN_UART1];
        fifodep = 8;
        port = CN_UART1;
        break;
    case CN_INT_LINE_UART2_RX_TX:
    case CN_INT_LINE_UART2_ERR:
        UCB = pUartCB[CN_UART2];
        Reg = tg_UART_Reg[CN_UART2];
        port = CN_UART2;
        break;
    case CN_INT_LINE_UART3_RX_TX:
    case CN_INT_LINE_UART3_ERR:
        UCB = pUartCB[CN_UART3];
        Reg = tg_UART_Reg[CN_UART3];
        port = CN_UART3;
        break;
    case CN_INT_LINE_UART4_RX_TX:
    case CN_INT_LINE_UART4_ERR:
        UCB = pUartCB[CN_UART4];
        Reg = tg_UART_Reg[CN_UART4];
        port = CN_UART4;
        break;
    case CN_INT_LINE_UART5_RX_TX:
    case CN_INT_LINE_UART5_ERR:
        UCB = pUartCB[CN_UART5];
        Reg = tg_UART_Reg[CN_UART5];
        port = CN_UART5;
        break;
    default:return 0;
    }

    if(Reg->S1 & UART_S1_RDRF_MASK)         //接收中断标志
    {
        for(num = 0; num < fifodep; num++)
        {
            if(!(Reg->SFIFO & UART_SFIFO_RXEMPT_MASK))
                ch[num] = Reg->D;
            else
                break;
        }
        UART_PortWrite(UCB,ch,num);
    }

    if(Reg->S1 & UART_S1_TDRE_MASK)         //发送中断标志
    {
        num = UART_PortRead(UCB,ch,fifodep);
        if(num>0)
        {
            for(i = 0; i < num; i++)
                Reg->D = ch[i++];
        }
        else
        {
            __UART_TxIntDisable(Reg);
            Board_UartHalfDuplexRecv(port);
        }
    }

    if(Reg->S1 & (UART_S1_FE_MASK | UART_S1_PF_MASK))   //硬件错误
    {
        UART_ErrHandle(UCB,CN_UART_HARD_COMM_ERR);
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
    case CN_UART0://串口0
        UART_Param.Name         = "UART0";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART0_BASE;
        UART_Param.TxRingBufLen = CFG_UART0_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART0_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
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

    CR_Bak = GetCharDirectReg->C2;                          //Save INT
    __UART_TxIntDisable(GetCharDirectReg);                  //disable send INT
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
        PutStrDirectReg->D = str[result];
    }
    PutStrDirectReg->C2 = CR_Bak;                         //restore send INT
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

    CR_Bak = GetCharDirectReg->C2;                          //Save INT
    __UART_TxIntDisable(GetCharDirectReg);                  //disable send INT
    while(__UART_RxHadChar(GetCharDirectReg) == false) ;
    result = GetCharDirectReg->D;
    PutStrDirectReg->C2 = CR_Bak;                         //restore send INT
    return result;
}

//----初始化内核级IO-----------------------------------------------------------
//功能：初始化内核级输入和输出所需的runtime函数指针。
//参数：无
//返回：无
//-----------------------------------------------------------------------------
void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut)
{
    if(!strcmp(StdioOut,"/dev/UART0"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART0_BASE;
        TxDirectPort = CN_UART0;
    }
    else if(!strcmp(StdioOut,"/dev/UART1"))
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

    if(!strcmp(StdioOut,"/dev/UART0"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART0_BASE;
        RxDirectPort = CN_UART0;
    }
    else if(!strcmp(StdioOut,"/dev/UART1"))
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
