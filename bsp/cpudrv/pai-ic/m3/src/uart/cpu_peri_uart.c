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
#include "pt32z192xx_uart.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern ptu32_t ModuleInstall_UART(u32 serial_no);
//
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
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip uart"//CPU的uart外设驱动
//parent:"uart device file"      //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"uart device file" //该组件的依赖组件名（可以是none，表示无依赖组件），
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
#define CFG_UART0_SENDBUF_LEN            32                 //"UART0发送环形缓冲区大小",
#define CFG_UART0_RECVBUF_LEN            32                 //"UART0接收环形缓冲区大小",

#define CFG_UART1_SENDBUF_LEN            32                 //"UART1发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN            32                 //"UART1接收环形缓冲区大小",

#define CFG_UART2_SENDBUF_LEN            32                 //"UART2发送环形缓冲区大小",
#define CFG_UART2_RECVBUF_LEN            32                 //"UART2接收环形缓冲区大小",
//%$#@enum,true,false,
#define CFG_UART0_ENABLE                 false              //"是否配置使用UART0",
#define CFG_UART1_ENABLE                 false              //"是否配置使用UART1",
#define CFG_UART2_ENABLE                 true               //"是否配置使用UART2",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

static u32 TxByteTime;                    //正常发送一个字节所需要的时间
static CMSDK_UART_TypeDef *PutStrDirectReg;     //用于printk发送的串口寄存器
static CMSDK_UART_TypeDef *GetCharDirectReg;     //用于直接接收的串口寄存器
static u32 TxDirectPort;                  //用于printk发送的串口号
static u32 RxDirectPort;                  //用于直接接收的串口号
// =============================================================================
static CMSDK_UART_TypeDef volatile * const tg_UART_Reg[] = {UART0, UART1, UART2};

static struct UartGeneralCB *pUartCB[CN_UART_NUM];

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
// 功能: 获取uart端口的中断使能控制位
// 参数: port,串口号，1~5
// 返回: 无
// =============================================================================
u32 __UART_GetIEReg(u8 port)
{
    switch(port)
    {
    case CN_UART0:
        return UART0->IER;
        break;
    case CN_UART1:
        return UART1->IER;
        break;
    case CN_UART2:
        return UART2->IER;
        break;
    default:
        break;
    }
    return 0;
}

// =============================================================================
// 功能: 禁止uart的发送中断。
// 参数: port,串口号，1~5
// 返回: 无
// 特别说明：PT192的UART_IER_TOIDLEIE中断不能清除，故不能用，只能使用 UART_IER_RXNEIE中断
// =============================================================================
void __UART_RxIntEnable(u8 port)
{
    switch(port)
    {
    case CN_UART0:
        UART_ITConfig(UART0, UART_IER_RXNEIE,ENABLE);
        break;
    case CN_UART1:
        UART_ITConfig(UART1, UART_IER_RXNEIE,ENABLE);
        break;
    case CN_UART2:
        UART_ITConfig(UART2, UART_IER_RXNEIE,ENABLE);
        break;
    default:
        break;
    }
}

// =============================================================================
// 功能: 禁止uart的接收中断
// 参数: port,串口号
// 返回: 无
// =============================================================================
void __UART_RxIntDisable(u8 port)
{
    switch(port)
    {
    case CN_UART0:
        UART_ITConfig(UART0, UART_IER_RXNEIE,DISABLE);
        break;
    case CN_UART1:
        UART_ITConfig(UART1, UART_IER_RXNEIE,DISABLE);
        break;
    case CN_UART2:
        UART_ITConfig(UART2, UART_IER_RXNEIE,DISABLE);
        break;
    default:
        break;
    }
}

// =============================================================================
// 功能: 使能uart的发送中断
// 参数: port,串口号
// 返回: 无
// =============================================================================
static void __UART_TxIntEnable(u8 port)
{
    switch(port)
    {
    case CN_UART0:
        UART_ITConfig(UART0, UART_IER_TXEIE+UART_IER_TXHEIE+UART_IER_TXENDIE,ENABLE);
        break;
    case CN_UART1:
        UART_ITConfig(UART1, UART_IER_TXEIE+UART_IER_TXHEIE+UART_IER_TXENDIE,ENABLE);
        break;
    case CN_UART2:
        UART_ITConfig(UART2, UART_IER_TXEIE+UART_IER_TXHEIE+UART_IER_TXENDIE,ENABLE);
        break;
    default:
        break;
    }
}

// =============================================================================
// 功能: 禁止UART的发送中断
// 参数: port,串口号
// 返回: 无
// =============================================================================
static void __UART_TxIntDisable(u8 port)
{
    switch(port)
    {
    case CN_UART0:
        UART_ITConfig(UART0, UART_IER_TXEIE+UART_IER_TXHEIE+UART_IER_TXENDIE,DISABLE);
        break;
    case CN_UART1:
        UART_ITConfig(UART1, UART_IER_TXEIE+UART_IER_TXHEIE+UART_IER_TXENDIE,DISABLE);
        break;
    case CN_UART2:
        UART_ITConfig(UART2, UART_IER_TXEIE+UART_IER_TXHEIE+UART_IER_TXENDIE,DISABLE);
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
static bool_t __UART_TxTranEmpty(CMSDK_UART_TypeDef volatile *reg)
{
    return (0 != (reg->SR & UART_FLAG_TXE));
}

// =============================================================================
// 功能: 检查发送寄存器的状态，如果空就返回true，若为空，否则返回false
// 参数：reg,被操作的寄存器组指针
// 返回: true = 缓冲区空，false = 非空
// =============================================================================
static bool_t __UART_TxFifoFull(CMSDK_UART_TypeDef volatile *reg)
{
    return (0 != (reg->SR & UART_FLAG_TXF));
}

// =============================================================================
// 功能: 检查接收寄存器的状态，有数据就返回true，否则返回false
// 参数：reg,被操作的寄存器组指针
// 返回: true = 有数据，false = 无数据
// =============================================================================
static bool_t __UART_RxHadChar(CMSDK_UART_TypeDef volatile *reg)
{
    return (1 == (reg->SR & UART_FLAG_RXNE));
}

// =============================================================================
// 功能: 设置串口baud,PCLK为25M，CPU主频为100M计算,该驱动只提供9600、19200、57600、
//       115200四种波特率设置
// 参数: Reg,被操作的寄存器组指针
//       baud,波特率
// 返回: 无
// =============================================================================
static void __UART_BaudSet(CMSDK_UART_TypeDef volatile *Reg,u32 baud)
{
    uint32_t  tmpreg = 0 ,divider = 0;

    divider = CN_CFG_PCLK2/(16*baud);
    tmpreg = CN_CFG_PCLK2%(16*baud);

    if (tmpreg > ((16/2) * baud))
    {
       Reg->BRR = (uint16_t) (divider + 1);
    }
    else
    {
        Reg->BRR = (uint16_t) divider;
    }
    Reg->BRR &= ~UART_BRR_SR;
    Reg->BRR |= UART_BRR_SRX16;

    if(Reg == PutStrDirectReg)
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
static void __UART_ComConfig(CMSDK_UART_TypeDef volatile *Reg,struct COMParam *COM)
{
    if((COM == NULL) || (Reg == NULL))
        return;
    __UART_BaudSet(Reg,COM->BaudRate);

    switch(COM->DataBits)               // data bits
    {
//  case CN_UART_DATABITS_8:    Reg->CR1 &= ~(1<<12);break;
//  case CN_UART_DATABITS_9:    Reg->CR1 |= (1<<12);;break;
    default:break;
    }

    switch(COM->Parity)                 // parity
    {
//  Reg->CR1 |= (1<<10);                // enable parity
//  case CN_UART_PARITY_ODD:    Reg->CR1 |= (1<<9);break;
//  case CN_UART_PARITY_EVEN:   Reg->CR1 &=~(1<<9);break;
    default:break;
    }

    switch(COM->StopBits)               // stop bits
    {
//  Reg->CR2 &= ~(3<<12);
//  case CN_UART_STOPBITS_1:    Reg->CR2 |= (0<<12);break;
//  case CN_UART_STOPBITS_1_5:  Reg->CR2 |= (1<<12);break;
//  case CN_UART_STOPBITS_2:    Reg->CR2 |= (2<<12);break;
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
    case CN_UART0:
        UART0->CR |=UART_CR_RXEN;
        break;
    case CN_UART1:
        UART1->CR |=UART_CR_RXEN;
        break;
    case CN_UART2:
        UART2->CR |=UART_CR_RXEN;
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
    case CN_UART0:
        UART0->CR &=~UART_CR_RXEN;
        break;
    case CN_UART1:
        UART1->CR &=~UART_CR_RXEN;
        break;
    case CN_UART2:
        UART2->CR &=~UART_CR_RXEN;
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
    if(SerialNo == CN_UART0)
        IntLine = CN_INT_LINE_USART0;
    else if(SerialNo == CN_UART1)
        IntLine = CN_INT_LINE_USART1;
    else if(SerialNo == CN_UART2)
        IntLine = CN_INT_LINE_USART2;
    else
        return;

    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,UART_ISR);
    Int_SetIsrPara(IntLine,SerialNo);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
    __UART_RxIntEnable(SerialNo);
}

// =============================================================================
// 功能: 硬件参数配置和寄存器的初始化，包括波特率、停止位、校验位、数据位，默认情况下:
//       波特率:115200  ； 停止位:1 ; 校验:偶校验 ; 数据位:8bit
// 参数: SerialNo,串口号，0~3
// 返回: 无
// =============================================================================
static void __UART_HardInit(u8 SerialNo)
{
    CMSDK_UART_TypeDef *uart;
    if(SerialNo == CN_UART0)
        uart = UART0;
    else if(SerialNo == CN_UART1)
        uart = UART1;
    else if(SerialNo == CN_UART2)
        uart = UART2;
    else
        return;

    UART_InitTypeDef UART_InitStruct;
    UART_InitStruct.UART_BaudRate = 115200;
    UART_InitStruct.UART_WordLength = UARTM_8D;
    UART_InitStruct.UART_StopBit = UART1StopBit;
    UART_InitStruct.UART_Parity = UART_ODD_PARITY;
    UART_InitStruct.UART_RXEN = ENABLE;
    UART_InitStruct.UART_SampleRate = UART_SAMPLERATEX16;
    UART_InitStruct.FCPU = CN_CFG_AHBCLK;
    //此函数 baud 比较高时似乎不准，澎湃微的示范工程有注释
    UART_Init(uart, &UART_InitStruct);
    UART_Cmd(uart, ENABLE);

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
static u32 __UART_SendStart (CMSDK_UART_TypeDef *Reg)
{
    u8 port;


    switch((u32)Reg)
    {
    case (u32)UART0:
        port = CN_UART0;
        break;
    case (u32)UART1:
        port = CN_UART1;
        break;
    case (u32)UART2:
        port = CN_UART2;
        break;
    default:
        return 0;
    }
    Board_UartHalfDuplexSend(port);//切换到发送
//  __UART_TxIntDisable(port);
    //只要使能发送空中断，则触发中断
    __UART_TxIntEnable(port);
    return 1;
}

// =============================================================================
// 功能: 串口设备的控制函数,与具体的硬件寄存器设置相关
// 参数: Reg,被操作的串口寄存器指针.
//       cmd,操作类型
//       data1,data2,含义依cmd而定
// 返回: 无意义.
// =============================================================================
static ptu32_t __UART_Ctrl(CMSDK_UART_TypeDef *Reg,u32 cmd, va_list *arg0)
{
    ptu32_t result = 0;
    u32 port;
    if(Reg == NULL)
        return 0;

    switch((u32)Reg)
    {
        case (u32)UART0: port = CN_UART0;break;
        case (u32)UART1: port = CN_UART1;break;
        case (u32)UART2: port = CN_UART2;break;
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
            __UART_BaudSet(Reg, data);
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
            __UART_ComConfig(Reg,COM);
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
// 参数: 中断号.
// 返回: 0.
// =============================================================================
u32 UART_ISR(ptu32_t port)
{
    struct UartGeneralCB *UCB;
    CMSDK_UART_TypeDef *Reg;
    u32 num;
    u8 ch;
    u8 revbuf[16];

    UCB = pUartCB[port];
    Reg = (CMSDK_UART_TypeDef *)tg_UART_Reg[port];

    num = 0;
    while( Reg->SR & UART_ISR_RXNE)         //接收中断
    {
        revbuf[num++] = (u8)Reg->BR;
    }

    if(num != 0)
    {
        if(num != UART_PortWrite(UCB,revbuf,num))
        {
            UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
        }
    }

    if(Reg->SR & UART_ISR_TXEND)        //发送全部完成中断，当此中断发生时，所有跟
                                        //发送FIFO相关中断标志都必然置位
    {
        num = UART_PortRead(UCB,&ch,1);
        if(num != 0)
        {
            Reg->BR = ch;
            while(!(Reg->SR & UART_ISR_TXF))        //直到FIFO全满
            {
                num = UART_PortRead(UCB,&ch,1);
                if(num != 0)
                    Reg->BR = ch;
                else
                    break;
            }
        }
        else
        {
            __UART_TxIntDisable(port);
            Board_UartHalfDuplexRecv(port);
        }
    }
    else
    {
        while(!(Reg->SR & UART_ISR_TXF))        //直到FIFO全满
        {
            num = UART_PortRead(UCB,&ch,1);
            if(num != 0)
                Reg->BR = ch;
            else
                break;
        }
    }
    if(Reg->SR & (UART_ISR_PERR+UART_ISR_FERR+UART_ISR_OVERR))        //各种错误
    {
        ch = (u8)Reg->BR;
        Reg->SR = UART_ISR_PERR+UART_ISR_FERR+UART_ISR_OVERR;
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
ptu32_t ModuleInstall_UART(u32 serial_no)
{
    struct UartParam UART_Param;

    switch(serial_no)
    {
    case CN_UART0://串口0
        UART_Param.Name         = "UART0";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = (ptu32_t)UART0;
        UART_Param.TxRingBufLen = CFG_UART0_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART0_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART1://串口1
        UART_Param.Name         = "UART1";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = (ptu32_t)UART1;
        UART_Param.TxRingBufLen = CFG_UART1_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART1_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART2://串口2
        UART_Param.Name         = "UART2";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = (ptu32_t)UART2;
        UART_Param.TxRingBufLen = CFG_UART2_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART2_RECVBUF_LEN;
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
    u32 timeout = TxByteTime * len+10;
    u8 port;
    for(port=0;port<CN_UART_NUM;port++)
    {
        if(PutStrDirectReg ==tg_UART_Reg[port])
            break;
    }
    Board_UartHalfDuplexSend(port);
    __UART_TxIntDisable(port);
    for(result=0; result <= len; result ++)
    {
        // 超时或者发送FIFO非满时退出
        while((true == __UART_TxFifoFull(PutStrDirectReg)) && (timeout > TxByteTime))
        {
            timeout -=TxByteTime;
            DJY_DelayUs(TxByteTime);
        }
        if(timeout > TxByteTime)
            PutStrDirectReg->BR = str[result];
        else
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
//  u16 rxier;
    char result;
//
//  rxier = __UART_GetIEReg(port);
//  __UART_RxIntDisable(port);
    if(__UART_RxHadChar(GetCharDirectReg) == true)
        result = GetCharDirectReg->BR;
    else
        result = EOF;
//  PutStrDirectReg->CR1 = CR_Bak;                         //restore send INT
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
        PutStrDirectReg = UART0;
        TxDirectPort = CN_UART0;
    }
    else if(!strcmp(StdioOut,"/dev/UART1"))
    {
        PutStrDirectReg = UART1;
        TxDirectPort = CN_UART1;
    }
    else if(!strcmp(StdioOut,"/dev/UART2"))
    {
        PutStrDirectReg = UART2;
        TxDirectPort = CN_UART2;
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

    if(!strcmp(StdioIn,"/dev/UART0"))
    {
        GetCharDirectReg = UART0;
        RxDirectPort = CN_UART0;
    }
    else if(!strcmp(StdioIn,"/dev/UART1"))
    {
        GetCharDirectReg = UART1;
        RxDirectPort = CN_UART1;
    }
    else if(!strcmp(StdioIn,"/dev/UART2"))
    {
        GetCharDirectReg = UART2;
        RxDirectPort = CN_UART2;
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
