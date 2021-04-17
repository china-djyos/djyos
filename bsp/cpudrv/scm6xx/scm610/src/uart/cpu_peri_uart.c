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
// 创建人员: huzb
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
//#include "cpu_peri_dma.h"
#include "stdlib.h"

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
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip uart"//CPU的uart外设驱动
//parent:"uart device file"      //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"device file system","lock","uart device file","heap","cpu onchip dma"//该组件的依赖组件名（可以是none，表示无依赖组件），
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
//%$#@num,16,512,
#define CFG_UART1_SENDBUF_LEN            32                 //"UART1发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN            32                 //"UART1接收环形缓冲区大小",
#define CFG_UART1_DMABUF_LEN             32                 //"UART1 DMA缓冲区大小",

#define CFG_UART2_SENDBUF_LEN            32                 //"UART2发送环形缓冲区大小",
#define CFG_UART2_RECVBUF_LEN            32                 //"UART2接收环形缓冲区大小",
#define CFG_UART2_DMABUF_LEN             32                 //"UART2 DMA缓冲区大小",

#define CFG_UART3_SENDBUF_LEN            32                 //"UART3发送环形缓冲区大小",
#define CFG_UART3_RECVBUF_LEN            32                 //"UART3接收环形缓冲区大小",
#define CFG_UART3_DMABUF_LEN             32                 //"UART3 DMA缓冲区大小",

#define CFG_UART4_SENDBUF_LEN            32                 //"UART4发送环形缓冲区大小",
#define CFG_UART4_RECVBUF_LEN            32                 //"UART4接收环形缓冲区大小",
#define CFG_UART4_DMABUF_LEN             32                 //"UART4 DMA缓冲区大小",

#define CFG_UART5_SENDBUF_LEN            32                 //"UART5发送环形缓冲区大小",
#define CFG_UART5_RECVBUF_LEN            32                 //"UART5接收环形缓冲区大小",
#define CFG_UART5_DMABUF_LEN             32                 //"UART5 DMA缓冲区大小",

#define CFG_UART6_SENDBUF_LEN            32                 //"UART6发送环形缓冲区大小",
#define CFG_UART6_RECVBUF_LEN            32                 //"UART6接收环形缓冲区大小",
#define CFG_UART6_DMABUF_LEN             32                 //"UART6 DMA缓冲区大小",

#define CFG_UART7_SENDBUF_LEN            32                 //"UART7发送环形缓冲区大小",
#define CFG_UART7_RECVBUF_LEN            32                 //"UART7接收环形缓冲区大小",
#define CFG_UART7_DMABUF_LEN             32                 //"UART7 DMA缓冲区大小",

#define CFG_UART8_SENDBUF_LEN            32                 //"UART8发送环形缓冲区大小",
#define CFG_UART8_RECVBUF_LEN            32                 //"UART8接收环形缓冲区大小",
#define CFG_UART8_DMABUF_LEN             32                 //"UART8 DMA缓冲区大小",

//%$#@enum,true,false,
#define CFG_UART1_ENABLE                 false              //"是否使用UART1",
#define CFG_UART2_ENABLE                 true              //"是否使用UART2",
#define CFG_UART3_ENABLE                 false              //"是否使用UART3",
#define CFG_UART4_ENABLE                 false              //"是否使用UART4",
#define CFG_UART5_ENABLE                 false              //"是否使用UART5",
#define CFG_UART6_ENABLE                 false              //"是否使用UART6",
#define CFG_UART7_ENABLE                 false              //"是否使用UART7",
#define CFG_UART8_ENABLE                 false              //"是否使用UART8",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================


#define CN_UART1_BASE (0x40300000)
#define CN_UART2_BASE (0x40310000)
#define CN_UART3_BASE (0x40320000)
#define CN_UART4_BASE (0x40330000)
#define CN_UART5_BASE (0x41340000)
#define CN_UART6_BASE (0x41350000)
#define CN_UART7_BASE (0x41360000)
#define CN_UART8_BASE (0x41370000)

//Uart Reg define

typedef struct
{
    volatile u32 TXDATA;
    volatile u32 RXDATA;
    volatile u32 STATUS;
    volatile u32 CTRL;
    volatile u32 INTSTA;
    volatile u32 BAUDDIV;

}tagUartReg;

static u32 TxByteTime;                    //正常发送一个字节所需要的时间
static tagUartReg *PutStrDirectReg;       //用于printk发送的串口寄存器
static tagUartReg *GetCharDirectReg;      //用于直接接收的串口寄存器
static u32 TxDirectPort;                  //用于printk发送的串口号
static u32 RxDirectPort;                  //用于直接接收的串口号
// =============================================================================
static tagUartReg volatile * const sgpt_UartReg[] = {(tagUartReg *)CN_UART1_BASE,
                                                    (tagUartReg *)CN_UART2_BASE,
                                                    (tagUartReg *)CN_UART3_BASE,
                                                    (tagUartReg *)CN_UART4_BASE,
                                                    (tagUartReg *)CN_UART5_BASE,
                                                    (tagUartReg *)CN_UART6_BASE,
                                                    (tagUartReg *)CN_UART7_BASE,
                                                    (tagUartReg *)CN_UART8_BASE};

#define CN_DMA_UNUSED   0
#define CN_DMA_USED     1
#define CN_UART_NUM     8

//#define CFG_UART1_SENDBUF_LEN            32
//#define CFG_UART1_RECVBUF_LEN            32
//#define CFG_UART1_DMABUF_LEN             32
//
//#define CFG_UART2_SENDBUF_LEN            32
//#define CFG_UART2_RECVBUF_LEN            32
//#define CFG_UART2_DMABUF_LEN             32
//
//#define CFG_UART3_SENDBUF_LEN            32
//#define CFG_UART3_RECVBUF_LEN            32
//#define CFG_UART3_DMABUF_LEN             32
//
//#define CFG_UART4_SENDBUF_LEN            32
//#define CFG_UART4_RECVBUF_LEN            32
//#define CFG_UART4_DMABUF_LEN             32
//
//#define CFG_UART5_SENDBUF_LEN            32
//#define CFG_UART5_RECVBUF_LEN            32
//#define CFG_UART5_DMABUF_LEN             32
//
//#define CFG_UART6_SENDBUF_LEN            32
//#define CFG_UART6_RECVBUF_LEN            32
//#define CFG_UART6_DMABUF_LEN             32

static u8 *pUART_DmaSendBuf[CN_UART_NUM];
static u8 *pUART_DmaRecvBuf[CN_UART_NUM];

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
static ptu32_t UART_ISR(ptu32_t port);

static uint32_t UART_DmaRx_ISR(ptu32_t port);
static uint32_t UART_DmaTx_ISR(ptu32_t port);

// =============================================================================
// 功能: 禁止uart的发送中断。
// 参数: flag,是否使用dma传输
//       port,串口号，1~5
// 返回: 无
// =============================================================================
void __UART_RxIntEnable(u8 port)
{
    if(CN_UART1 <= port && port < CN_UART_MAX)
    {
        sgpt_UartReg[port]->CTRL |= (1<<3);
    }
}

// =============================================================================
// 功能: 禁止uart的接收中断
// 参数: reg,被操作的寄存器组指针
//       port,串口号
// 返回: 无
// =============================================================================
void __UART_RxIntDisable(u8 port)
{
    if(CN_UART1 <= port && port < CN_UART_MAX)
    {
        sgpt_UartReg[port]->CTRL &= ~(1<<3);
    }
}

// =============================================================================
// 功能: 使能uart的发送中断
// 参数: reg,被操作的寄存器组指针
//       port,串口号
// 返回: 无
// =============================================================================
static void __UART_TxIntEnable(u8 port)
{
    if(CN_UART1 <= port && port < CN_UART_MAX)
    {
        sgpt_UartReg[port]->CTRL |= (1<<2);
    }
}

// =============================================================================
// 功能: 禁止UART的发送中断
// 参数: Reg,被操作的寄存器组指针
//       port,串口号
// 返回: 无
// =============================================================================
static void __UART_TxIntDisable(u8 port)
{
    if(CN_UART1 <= port && port < CN_UART_MAX)
    {
        sgpt_UartReg[port]->CTRL &= ~(1<<2);
    }
}

// =============================================================================
// 功能: 检查发送寄存器的状态，如果空就返回true，若为空，否则返回false
// 参数：reg,被操作的寄存器组指针
// 返回: true = 缓冲区空，false = 非空
// =============================================================================
static bool_t __UART_TxTranEmpty(tagUartReg volatile *reg)
{
    bool_t bRet;

    if(reg->STATUS & (1<<5))
    {
        bRet = true;
    }else{
        bRet = false;
    }
    return bRet;
}

// =============================================================================
// 功能: 检查接收寄存器的状态，有数据就返回true，否则返回false
// 参数：reg,被操作的寄存器组指针
// 返回: true = 有数据，false = 无数据
// =============================================================================

static bool_t __UART_RxTranNotEmpty(tagUartReg volatile *reg)
{
    bool_t bRet;

    if(reg->STATUS & (1<<1))
    {
        bRet = false;
    }else{
        bRet =  true;
    }

    return bRet;
}

// =============================================================================
// 功能: 设置串口baud,PCLK为25M，CPU主频为100M计算,该驱动只提供9600、19200、57600、
//       115200四种波特率设置
// 参数: Reg,被操作的寄存器组指针
//       port,串口号
//       baud,波特率
// 返回: 无
// =============================================================================

#define CN_UART_CLK (37500000UL)

static void __UART_BaudSet(tagUartReg volatile *Reg,u32 port,u32 baud)
{
    if(CN_UART1 <= port && port < CN_UART_MAX)
    {
        sgpt_UartReg[port]->BAUDDIV = (u32)(CN_UART_CLK / baud);
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
    struct COMParam *COM;
    if((COM == NULL) || (Reg == NULL))
        return;
    __UART_BaudSet(Reg,port,COM->BaudRate);

    //数据位不可配-固定为8BIT

    switch(COM->Parity)                 // parity
    {

        case CN_UART_PARITY_NONE:       //ONOE PARITY
             Reg->CTRL &= ~(1<<4);
             break;
        case CN_UART_PARITY_ODD:
             Reg->CTRL |= (1<<4);
             Reg->CTRL &= ~(1<<7);
             break;
        case CN_UART_PARITY_EVEN:
             Reg->CTRL |= (1<<4);
             Reg->CTRL |= (1<<7);
             break;
        default:Reg->CTRL &= ~(1<<4);break;
    }

    //Stop Bit Config
    Reg->CTRL |= (1<<6);
    switch(COM->StopBits)               // stop bits
    {
        case CN_UART_STOPBITS_1:
             Reg->CTRL &= ~(1<<5);
             break;
        case CN_UART_STOPBITS_1_5:     //不支持1.5个停止位
             ;break;
        case CN_UART_STOPBITS_2:
             Reg->CTRL |= (1<<5);
             break;
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
    if(CN_UART1 <= port && port < CN_UART_MAX)
    {
        //串口收发使能
        sgpt_UartReg[port]->CTRL      |= (1<<0) ;
        sgpt_UartReg[port]->CTRL      |= (1<<1) ;
    }
}

// =============================================================================
// 功能: 使串口停止工作，包括时钟和uart模块
// 参数: port,串口号
// 返回: 无
// =============================================================================

void __UART_Disable(u32 port)
{
    if(CN_UART1 <= port && port < CN_UART_MAX)
    {
        sgpt_UartReg[port]->CTRL      &= ~(1<<0) ;
        sgpt_UartReg[port]->CTRL      &= ~(1<<1) ;
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

    switch(SerialNo)
    {
        case CN_UART1:
             IntLine = CN_INT_LINE_UART0;
              break;
        case CN_UART2:
             IntLine = CN_INT_LINE_UART1;
              break;
        case CN_UART3:
             IntLine = CN_INT_LINE_UART2;
              break;
        case CN_UART4:
             IntLine = CN_INT_LINE_UART3;
              break;
        case CN_UART5:
             IntLine = CN_INT_LINE_UART4;
              break;
        case CN_UART6:
             IntLine = CN_INT_LINE_UART5;
              break;
        case CN_UART7:
             IntLine = CN_INT_LINE_UART6;
              break;
        case CN_UART8:
             IntLine = CN_INT_LINE_UART7;
              break;
    }

    u32 UART_ISR(ptu32_t port);
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
//       波特率:115200  ； 停止位:1 ; 校验:偶校验 ; 数据位:8bit
// 参数: SerialNo,串口号，0~3
// 返回: 无
// =============================================================================

static void __UART_HardInit(u8 SerialNo)
{
    if(SerialNo > CN_UART8)
        return;
    //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
   //初始化uart硬件控制数据结构
   //复位初始化
    sgpt_UartReg[SerialNo]->CTRL     = 0;
    sgpt_UartReg[SerialNo]->BAUDDIV  = 0;

    //设置无奇偶校验位
    sgpt_UartReg[SerialNo]->CTRL &= ~(1<<4);
    //设置停止位1 bit
    sgpt_UartReg[SerialNo]->CTRL &= ~(1<<5);
    //设置停止位检查
    sgpt_UartReg[SerialNo]->CTRL |= (1<<6);

    //因为不支持发送空中断，这里发送不通过中断发送,直接把发送中断禁止
    sgpt_UartReg[SerialNo]->CTRL &= ~(1<<2);

    //这里只开接收中断
    sgpt_UartReg[SerialNo]->CTRL |=  (1<<3);

    //使能收发
    sgpt_UartReg[SerialNo]->CTRL      |= (1<<0) ;
    sgpt_UartReg[SerialNo]->CTRL      |= (1<<1) ;

    //默认波特率设置115200
    //sgpt_UartReg[SerialNo]->BAUDDIV = (uint32_t)(UART_CLK/115200);
    __UART_BaudSet(sgpt_UartReg[SerialNo],SerialNo,115200);
}

// =============================================================================
// 功能: 启动串口发送，其目的是触发中断，用中断方式发送数据。
// 参数: Reg,被操作的串口寄存器指针.
// 返回: 发送的个数
// =============================================================================

static u32 __UART_SendStart (tagUartReg *Reg)
{
    u8 port;
    u32 delay=0,len;
    struct UartGeneralCB *UCB;
    u8 fifo[32];
    atom_low_t atom_low;

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
        default:return 0;
    }

    UCB = pUartCB[port];

    Board_UartHalfDuplexSend(port);//切换到发送
    __UART_TxIntDisable(port);

    //没有发送空中断，这里发送不使用中断发送

    while((len = UART_PortRead(UCB,&fifo[0],1)) != 0)
    {
         //为保证发送过程不被中断打断，加入低级原子操作
         atom_low = Int_LowAtomStart();
         //等待发送fifo 空
         while(sgpt_UartReg[port]->STATUS & (1<<0));
         sgpt_UartReg[port]->TXDATA = fifo[0];
         while(!(sgpt_UartReg[port]->STATUS & (1<<5)));
         Int_LowAtomEnd(atom_low);
    }

//    __UART_TxIntEnable(port);

    return 1;
}

// =============================================================================
// 功能: 启动串口DMA发送，使用DMA启动，在发送前需判断上次DMA是否已经发送完成，通过
//       查看标志UARTx_DMA_SENDING来判断UARTx是否已经发送完成
// 参数: PORT,串口号.
// 返回: 发送的个数
// =============================================================================

u32 __UART_DMA_SendStart(u32 port)
{

   return 0;
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
        case CN_UART_SET_BAUD:  //设置Baud
        {
            u32 data;
            data = va_arg(*arg0, u32);
            __UART_BaudSet(Reg,port, data);
        }
            break;
        case CN_UART_EN_RTS:

            break;
        case CN_UART_DIS_RTS:

            break;
        case CN_UART_EN_CTS:

            break;
        case CN_UART_DIS_CTS:

            break;
        case CN_UART_DMA_USED:

            break;
        case CN_UART_DMA_UNUSED:

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
// 功能: 设置uart使用dma收发，根据stm32各串口的收发dma通道配置寄存器。将重新初
//       始化所有dma寄存器，因为:
//       1、不使用dma期间可能有其他设备使用dma，从而更改了设置。
//       2、即使uart使用dma期间，也可能软件模块配合的问题导致其他设备使用dma.
// 参数: port,端口号，即串口号，从1~3
// 返回: 无
// =============================================================================
void __UART_SetDmaUsed(u32 port)
{

}

//=============================================================================
// 功能: 设置不使用DMA，UART不使用DMA收发，每收发一个字节就必须中断一次，不建议在高速
//      连续通信中使用。
// 参数: port,端口号，即串口号，为1~3
// 返回: 无
// =============================================================================

void __UART_SetDmaUnUsed(u32 port)
{

}



// =============================================================================
// 功能: 当uart2配置成dma方式是，dma1 ch7负责发送，ch6负责接收
// 参数: 中断函数没有参数.
// 返回: 中断函数没有返回值.
// =============================================================================
uint32_t UART_DmaTx_ISR(ptu32_t port)
{

    return 0;
}

// =============================================================================
// 功能: 当uart2配置成dma方式是，dma1 ch7负责发送，ch6负责接收
// 参数: 中断函数没有参数.
// 返回: 中断函数没有返回值.
// =============================================================================

uint32_t UART_DmaRx_ISR(ptu32_t port)
{
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
    Reg = (tagUartReg *)sgpt_UartReg[port];

    //接收中断

    if(!(Reg->STATUS & (1<<1)))//判接收FiFo
    {
        //若接收缓存非空
        while(!(Reg->STATUS & (1<<1)))
        {
            ch = (u8)Reg->RXDATA;
//            Reg->TXDATA = ch;
            num = UART_PortWrite(UCB,&ch,1);
            if(num != 1)
            {
                UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
            }
        }

        //清接收中断
        Reg->INTSTA |= (1<<1);
    }

     //因为这里没有发送空中断，固发送不用中断

    if(Reg->INTSTA & (1<<2))    //接收错误
    {
        printk("Rev Overrun\r\n");
        //清接收错误
        Reg->INTSTA |= (1<<2);
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
    case CN_UART6://串口6
        UART_Param.Name         = "UART6";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART6_BASE;
        UART_Param.TxRingBufLen = CFG_UART6_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART6_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
     case CN_UART7://串口6
        UART_Param.Name         = "UART7";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART7_BASE;
        UART_Param.TxRingBufLen = CFG_UART7_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART7_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
     case CN_UART8://串口6
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
    for(port=0;port<CN_UART_MAX;port++)
    {
        if(PutStrDirectReg ==sgpt_UartReg[port])
            break;
    }
    Board_UartHalfDuplexSend(port);
//    CR_Bak = PutStrDirectReg->CTRL;                          //Save INT
//    PutStrDirectReg->CTRL &= ~(1<<2);                        //disable send INT
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
        PutStrDirectReg->TXDATA = str[result];

        //等待发送完成
        while(!(PutStrDirectReg->STATUS & (1<<5)));

    }

//    PutStrDirectReg->CTRL = CR_Bak;  //恢复中断
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

    CR_Bak = GetCharDirectReg->CTRL;                          //Save INT
    PutStrDirectReg->CTRL &= ~(1<<2);                         //disable send INT
    while(__UART_RxTranNotEmpty(GetCharDirectReg) == false) ;
    result = GetCharDirectReg->RXDATA;
    PutStrDirectReg->CTRL = CR_Bak;                            //restore send INT
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

