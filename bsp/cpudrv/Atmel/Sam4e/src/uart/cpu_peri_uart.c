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
// 模块描述: UART模块底层硬件驱动，寄存器级别的操作
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 05/15.2014
// =============================================================================

#include "cpu_peri.h"
#include "stdio.h"
#include <device/djy_uart.h>
#include "string.h"
#include "project_config.h"    //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    ptu32_t ModuleInstall_UART(ptu32_t SerialNo);
//    #if CFG_UART0_ENABLE ==1
//    ModuleInstall_UART(CN_UART0);
//    #endif
//    #if CFG_UART1_ENABLE ==1
//    ModuleInstall_UART(CN_UART1);
//    #endif
//
//    #if CFG_USART0_ENABLE ==1
//    ModuleInstall_UART(CN_USART0);
//    #endif
//
//    #if CFG_USART1_ENABLE ==1
//    ModuleInstall_UART(CN_USART1);
      //#endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip uart"//CPU的uart外设驱动
//parent:"uart device file"      //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                     //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                  //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                    //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                   //初始化时机，可选值：early，medium，later, pre-main。
                                     //表示初始化时间，分别是早期、中期、后期
//dependence:"device file system","lock","uart device file","heap","cpu drver Power management"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                    //如果依赖多个组件，则依次列出
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                    //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_UART == false )
//#warning  " cpu_onchip_uart  组件参数未配置，使用默认配置"
//%$#@target = header              //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_UART    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,32,512,
#define CFG_UART0_SENDBUF_LEN   256            //"UART0发送缓冲区大小",
#define CFG_UART0_RECVBUF_LEN   256            //"UART0接收缓冲区大小",
#define CFG_UART0_DMABUF_LEN    64             //"UART0DMA缓冲区大小",

#define CFG_UART1_SENDBUF_LEN   256            //"UART1发送缓冲区大小",
#define CFG_UART1_RECVBUF_LEN   256            //"UART1接收缓冲区大小",
#define CFG_UART1_DMABUF_LEN    64             //"UART1DMA缓冲区大小",

#define CFG_USART0_SENDBUF_LEN  256           //"USART0发送缓冲区大小",
#define CFG_USART0_RECVBUF_LEN  256           //"USART0接收缓冲区大小",
#define CFG_USART0_DMABUF_LEN   64            //"USART0DMA缓冲区大小",

#define CFG_USART1_SENDBUF_LEN  256           //"USART1发送缓冲区大小",
#define CFG_USART1_RECVBUF_LEN  256           //"USART1接收缓冲区大小",
#define CFG_USART1_DMABUF_LEN   64            //"USART1DMA缓冲区大小",
//%$#@enum,true,false,
#define CFG_UART0_ENABLE        true          //"是否配置UART0",
#define CFG_UART1_ENABLE        false         //"是否配置UART1",
#define CFG_USART0_ENABLE       false         //"是否配置USART0",
#define CFG_USART1_ENABLE       false         //"是否配置USART1",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

// =============================================================================
//base address
#define CN_UART0_BASE   ((u32)UART0)
#define CN_UART1_BASE   ((u32)UART1)
#define CN_USART0_BASE  ((u32)USART0)
#define CN_USART1_BASE  ((u32)USART1)

static u32 TxByteTime;                    //正常发送一个字节所需要的时间
static tagUartReg *PutStrDirectReg;     //用于printk发送的串口寄存器
static tagUartReg *GetCharDirectReg;     //用于直接接收的串口寄存器
static u32 TxDirectPort;                  //用于printk发送的串口号
static u32 RxDirectPort;                  //用于直接接收的串口号
//==============================================================================
static tagUartReg volatile * const tg_UART_Reg[] = {(tagUartReg *)CN_UART0_BASE,
                                                    (tagUartReg *)CN_UART1_BASE,
                                                    (tagUartReg *)CN_USART0_BASE,
                                                    (tagUartReg *)CN_USART1_BASE};
bool_t s_UART_DmaUsed[4]={false,false,false,false};

//#define CFG_UART0_SENDBUF_LEN  256
//#define CFG_UART0_RECVBUF_LEN  256
//#define CFG_UART0_DMABUF_LEN  64
static u32 s_UART0_DmaRcvLen = 1;
uint8_t UART0_DmaSendBuf[CFG_UART0_DMABUF_LEN];
uint8_t UART0_DmaRecvBuf[CFG_UART0_DMABUF_LEN];


//#define CFG_UART1_SENDBUF_LEN  256
//#define CFG_UART1_RECVBUF_LEN  256
//#define CFG_UART1_DMABUF_LEN  64
static u32 s_UART1_DmaRcvLen = 1;
uint8_t UART1_DmaSendBuf[CFG_UART1_DMABUF_LEN];
uint8_t UART1_DmaRecvBuf[CFG_UART1_DMABUF_LEN];

//#define CFG_USART0_SENDBUF_LEN  256
//#define CFG_USART0_RECVBUF_LEN  256
//#define CFG_USART0_DMABUF_LEN  64
static u32 s_USART0_DmaRcvLen = 1;
uint8_t USART0_DmaSendBuf[CFG_USART0_DMABUF_LEN];
uint8_t USART0_DmaRecvBuf[CFG_USART0_DMABUF_LEN];

//#define CFG_USART1_SENDBUF_LEN  256
//#define CFG_USART1_RECVBUF_LEN  256
//#define CFG_USART1_DMABUF_LEN  64
static u32 s_USART1_DmaRcvLen = 1;
uint8_t USART1_DmaSendBuf[CFG_USART1_DMABUF_LEN];
uint8_t USART1_DmaRecvBuf[CFG_USART1_DMABUF_LEN];

ptu32_t UART_ISR(ptu32_t IntLine);

static struct Object *pUartCB[CN_UART_NUM];

extern bool_t Board_UartGpioInit(u8 SerialNo);

__attribute__((weak))  void Board_UartHalfDuplexSend(u8 SerialNo)
{
    return;
}
__attribute__((weak))  void Board_UartHalfDuplexRecv(u8 SerialNo)
{
    return ;
}
//----使能接收中断-------------------------------------------------------------
//功能: 使能uart的接收中断,DMA中断使用的是endrx
//参数: reg,被操作的寄存器组指针
//      DmaIsUsed,是否使用DMA
//返回: 无
//-----------------------------------------------------------------------------
void __UART_RecvIntEnable(tagUartReg volatile *reg,u32 DmaIsUsed)
{
    if(DmaIsUsed == cn_dma_unused)
        reg->UART_IER = (1<<0);//rxrdy int
    else
        reg->UART_IER = (1<<3);//endrx int
}

//----禁止接收中断-------------------------------------------------------------
//功能: 禁止uart的接收中断,DMA中断使用的是endrx
//参数: reg,被操作的寄存器组指针
//      DmaIsUsed,是否使用DMA
//返回: 无
//-----------------------------------------------------------------------------
void __UART_RecvIntDisable(tagUartReg volatile *reg,u32 DmaIsUsed)
{
    if(DmaIsUsed == cn_dma_unused)
        reg->UART_IDR = (1<<0);//rxrdy int
    else
        reg->UART_IDR = (1<<3);//endrx int
}

//----使能发送中断-------------------------------------------------------------
//功能: 使能uart的发送中断,DMA中断使用的是endtx 中断
//参数: reg,被操作的寄存器组指针
//      DmaIsUsed,是否使用DMA
//返回: 无
//-----------------------------------------------------------------------------
void __UART_SendIntEnable(tagUartReg volatile *reg,u32 DmaIsUsed)
{
    if(DmaIsUsed == cn_dma_unused)
        reg->UART_IER = (1<<9);//tx empty int
    else
        reg->UART_IER = (1<<11);//txbufe int
}

//----禁止发送中断-------------------------------------------------------------
//功能: 禁止uart的发送中断,DMA中断使用的是endtx 中断
//参数: reg,被操作的寄存器组指针
//      DmaIsUsed,是否使用DMA
//返回: 无
//-----------------------------------------------------------------------------
void __UART_SendIntDisable(tagUartReg volatile *reg,u32 DmaIsUsed)
{
    if(DmaIsUsed == cn_dma_unused)
        reg->UART_IDR = (1<<9);//tx empty int
    else
        reg->UART_IDR = (1<<11);//txbufe int
}

//----检查串口发送寄存器是否空-------------------------------------------------
//功能: 检查发送寄存器的状态，如果空就返回true，若为空，则移位寄存器和thr都为空
//参数：无。
//返回: true = 缓冲区空，false = 非空
//-----------------------------------------------------------------------------
bool_t __UART_TxTranEmpty(volatile tagUartReg *reg)
{
    return ((reg->UART_SR & (1<<9)) && (reg->UART_SR & (1<<11)));//TXR EMPTY
}

// =============================================================================
// 功能: 检查接收寄存器的状态，有数据就返回true，否则返回false
// 参数：reg,被操作的寄存器组指针
// 返回: true = 有数据，false = 无数据
// =============================================================================
static bool_t __UART_RxHadChar(tagUartReg volatile *reg)
{
    return (1 == (reg->UART_SR & (1<<0)));  //RXRDY,At least one byte in buf
}

//----设置baud-----------------------------------------------------------------
//功能: 设置串口baud
//参数: reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __UART_BaudSet(tagUartReg volatile *reg,u32 baud)
{
    reg->UART_BRGR = (CN_CFG_MCLK/baud)/16;
    if(tg_UART_Reg[TxDirectPort] == reg)
    {
        TxByteTime = 11000000/baud;     //1个字节传输时间，按10bit，+10%计算
    }
}

//----初始化uart对应的io口-----------------------------------------------------
//功能: 初始化uart对应的io口为输入或输出
//参数：SerialNo,串口号
//返回: 无
//-----------------------------------------------------------------------------
void __UART_GpioInit(u32 SerialNo)
{
    Board_UartGpioInit(SerialNo);
}

//----初始化uart对应的中断-----------------------------------------------------
//功能: 初始化uart对应的中断线，并初始化中断入口函数
//参数：SerialNo,串口号
//返回: 无
//-----------------------------------------------------------------------------
void __UART_IntInit(u32 SerialNo)
{
    uint32_t int_line;
    if(SerialNo == CN_UART0)
        int_line = CN_INT_LINE_UART0;
    else if(SerialNo == CN_UART1)
        int_line = CN_INT_LINE_UART1;
    else if(SerialNo == CN_USART0)
        int_line = CN_INT_LINE_USART0;
    else
        int_line = CN_INT_LINE_USART1;

    Int_Register(int_line);
    Int_SetClearType(int_line,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(int_line,UART_ISR);
    Int_SettoAsynSignal(int_line);
    Int_ClearLine(int_line);
    Int_RestoreAsynLine(int_line);
}

//----初始化uart时钟使能-----------------------------------------------------
//功能: 使能相应uart外设的时钟
//参数：SerialNo,串口号
//返回: 无
//-----------------------------------------------------------------------------
void __UART_PmcInit(u32 SerialNo)
{
    if(SerialNo == CN_UART0)
        PMC_EnablePeripheral(ID_UART0);
    else if(SerialNo == CN_UART1)
        PMC_EnablePeripheral(ID_UART1);
    else if(SerialNo == CN_USART0)
        PMC_EnablePeripheral(ID_USART0);
    else
        PMC_EnablePeripheral(ID_USART1);
}

// =============================================================================
// 功能: 配置串口采用DMA方式时，接收配置，包括 DMA接收大小和指针
// 参数：reg,寄存器指针
//      SerialNo,串口号
//      DataLen,数据大小
// 返回: 无
// =============================================================================
void __UART_dma_recv_config(tagUartReg* reg,u32 SerialNo,u32 DataLen)
{
    u32 length;
    switch(SerialNo)
    {

    case CN_UART0:
        length = CFG_UART0_DMABUF_LEN;
        length = (DataLen/length)?length:DataLen%length;
        s_UART0_DmaRcvLen = DataLen;                //剩余量
        reg->UART_RCR = length;
        reg->UART_RPR = (vu32)UART0_DmaRecvBuf;
        break;

    case CN_UART1:
        length = CFG_UART1_DMABUF_LEN;
        length = (DataLen/length)?length:DataLen%length;
        s_UART1_DmaRcvLen = DataLen;                //剩余量
        reg->UART_RCR = length;
        reg->UART_RPR = (vu32)UART1_DmaRecvBuf;
        break;

    case CN_USART0:
        length = CFG_USART0_DMABUF_LEN;
        length = (DataLen/length)?length:DataLen%length;
        s_USART0_DmaRcvLen = DataLen;               //剩余量
        reg->UART_RCR = length;
        reg->UART_RPR = (vu32)USART0_DmaRecvBuf;
        break;

    case CN_USART1:
        length = CFG_USART1_DMABUF_LEN;
        length = (DataLen/length)?length:DataLen%length;
        s_USART1_DmaRcvLen = DataLen;               //剩余量
        reg->UART_RCR = length;
        reg->UART_RPR = (vu32)USART1_DmaRecvBuf;
        break;
    default:
        break;
    }
}

// =============================================================================
// 功能: 配置uart为DMA方式或配置为非DMA方式时，配置各寄存器，
// 参数：reg,串口寄存器指针
//       cmd,命令号
//       Port,端口号，即串口号
// 返回: 无
// =============================================================================
void __UART_DMA_Config(tagUartReg* Reg,u32 cmd,u32 Port)
{
    //将串口相关的寄存器reset并除能接收和发送
    Reg->UART_CR = ((1<<2)|(1<<3)|(1<<5)|
                (1<<7)|(1<<8));         //接收发送都除能了

    if(cmd == CN_UART_DMA_UNUSED)//不使用DMA
    {
        s_UART_DmaUsed[Port] = false;

         //关闭非dma时使用的中断，开始dma使用的uart接收中断
         //发送中断只在启动发送时开启
        __UART_RecvIntDisable(Reg, cn_dma_used);
        __UART_SendIntDisable(Reg, cn_dma_used);
        __UART_RecvIntEnable(Reg, cn_dma_unused);
        Reg->UART_PTCR = ((1<<1)|(1<<9));//disable DMA rx tx
        Reg->UART_CR = ((1<<4)|(1<<6));// tx rx enable
    }
    else    //使用DMA
    {
        s_UART_DmaUsed[Port] = true;

        //关闭非dma时使用的中断，开始dma使用的uart接收中断
        __UART_RecvIntEnable(Reg, cn_dma_used);
        __UART_RecvIntDisable(Reg, cn_dma_unused);
        __UART_SendIntDisable(Reg, cn_dma_unused);

        //配置dma参数
        Reg->UART_TCR   = 0;
        Reg->UART_TNCR  = 0;
        Reg->UART_RCR   = 0;
        Reg->UART_RNCR  = 0;

        __UART_dma_recv_config(Reg,Port,1);

        Reg->UART_PTCR = ((1<<0)|(1<<8));//enable DMA rx tx
        Reg->UART_CR = ((1<<4)|(1<<6));// tx rx enable
    }
}

// =============================================================================
// 功能: 设置硬件参数，包括波特率、停止位、校验位、数据位，默认情况下:
//      波特率:115200  ； 停止位:1 ; 校验:偶校验 ; 数据位:8bit
// 参数: reg,被操作的寄存器组指针
// 返回: 无
// =============================================================================
void __UART_HardInit(u8 SerialNo)
{
    tagUartReg *reg;
    if(SerialNo > CN_UART_MAX)
        return;

    __UART_PmcInit(SerialNo);
    __UART_GpioInit(SerialNo);

    reg = (tagUartReg *)tg_UART_Reg[SerialNo];
    __UART_BaudSet(reg,115200);            //波特率设置
    reg->UART_MR = 0x000;                  //偶校验，正常模式
    reg->UART_CR = ((1<<2)|(1<<3)|(1<<5)|
                    (1<<7)|(1<<8));         //接收发送都除能了

    reg->UART_MR = 0x00C0;//停止位:1 奇偶校验:偶
    reg->UART_CR = ((1<<4)|(1<<6));// tx rx enable
}

// =============================================================================
// 功能: 串口设备的控制函数,与具体的硬件寄存器设置相关
// 参数: Reg,UART的寄存器基址.
//       cmd,操作类型
//       data,含义依cmd而定
// 返回: 无意义.
// =============================================================================
ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd, va_list *arg0)
{
    u8 Port;
    switch((u32)Reg)
    {
    case CN_UART0_BASE: Port = CN_UART0;break;
    case CN_UART1_BASE: Port = CN_UART1;break;
    case CN_USART0_BASE: Port = CN_USART0;break;
    case CN_USART1_BASE: Port = CN_USART1;break;
    default:return 0;
    }

    switch(cmd)
    {
        case CN_DEV_CTRL_START:
            __UART_RecvIntEnable(Reg,s_UART_DmaUsed[Port]);
            __UART_SendIntEnable(Reg,s_UART_DmaUsed[Port]);
            break;
        case CN_DEV_CTRL_STOP:
            __UART_RecvIntDisable(Reg,s_UART_DmaUsed[Port]);
            __UART_SendIntDisable(Reg,s_UART_DmaUsed[Port]);
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
//        case CN_UART_RX_PAUSE:      //暂停接收
//            __UART_RecvIntEnable(Reg,s_UART_DmaUsed[Port]);
//            break;
//        case CN_UART_RX_RESUME:      //恢复接收
//            __UART_RecvIntDisable(Reg,s_UART_DmaUsed[Port]);
//            break;
        case CN_UART_RECV_HARD_LEVEL:    //因为UART没有FIFO，因此配置DMA接收
        {
            u32 data;
            data = va_arg(*arg0, u32);
            __UART_dma_recv_config(Reg,Port,data);
        }
            break;
//        case CN_UART_HALF_DUPLEX_SEND:
//            Board_UartHalfDuplexSend(Port);
//            break;
//        case CN_UART_HALF_DUPLEX_RECV:
//            Board_UartHalfDuplexRecv(Port);
//            break;
        case CN_UART_DMA_USED:
        case CN_UART_DMA_UNUSED:
            __UART_DMA_Config(Reg,cmd,Port);
            break;
        default: break;
    }
    return 0;
}

// =============================================================================
// 功能: 这个是直接写串口函数，不会经过事件弹出
// 参数: Reg,UART的寄存器基址.
//      send_buf,被发送的缓冲数据
//      len,发送的数据字节数
//      timeout,超时时间，微秒
// 返回: 发送的个数
// =============================================================================
//u32 __UART_SendDirectly(tagUartReg* Reg,u8 *send_buf,u32 len,u32 timeout)
//{
//    u32  result,Port;
//    switch((u32)Reg)
//    {
//    //UART0和UART1的FIFO深度为8，而其他的为1
//    case CN_UART0_BASE:Port = CN_UART0; break;
//    case CN_UART1_BASE:Port = CN_UART1;break;
//    case CN_USART0_BASE:Port = CN_USART0;break;
//    case CN_USART1_BASE:Port = CN_USART1; break;
//    default:return 0;
//    }
//
//    __UART_SendIntDisable(Reg,s_UART_DmaUsed[Port]);
////    __UART_half_duplex_send(Port);//硬件使能发送
//
//    if(s_UART_DmaUsed[Port] == false)
//    {
//        for(result=0; result < len; result ++)
//        {
//            while((0 == __UART_TxTranEmpty(Reg))
//                && (timeout > 0))//超时或者发送缓冲为空时退出
//            {
//                timeout--;
//                DJY_DelayUs(1);
//            }
//            if(timeout == 0)
//                break;
//            Reg->UART_THR = send_buf[result];
//        }
//
////        //等待发送完再将485通信转为接收
////        while((0 == __UART_TxTranEmpty(Reg))
////                && (timeout > 0))//超时或者发送缓冲为空时退出
////        {
////            timeout--;
////            DJY_DelayUs(1);
////        }
////        if(timeout == 0)
////            result = 0;
//    }
//    else
//    {
//        Reg->UART_PTCR = (1<<9);//disable dma send first
//      if((Reg->UART_TCR==0)&&(Reg->UART_TNCR==0))
//      {
//          Reg->UART_TPR  = (uint32_t)send_buf;
//          Reg->UART_TCR  = len;
//          Reg->UART_PTCR = (1<<8);//dma tx enbale
//      }
//      else
//            result = 0;
//        //直接发送方式，采用阻塞的DMA发送
//        while((!__UART_TxTranEmpty(Reg)) && (timeout > 0))
//        {
//            timeout--;
//            DJY_DelayUs(1);
//        }
//        if(timeout == 0)
//            result = 0;
//    }
//    __UART_SendIntEnable(Reg,s_UART_DmaUsed[Port]);
////    __UART_half_duplex_recv(Port);//硬件使能接收
//    return result;
//}

// =============================================================================
// 功能: 启动串口发送，直接发送FIFO大小的数据，并产生发送空中断，在中断中将缓冲区中的
//      数据完成所有数据的发送，相当于启动了一次发送功能；分为DMA方式和非DMA方式
// 参数: Reg,被操作的串口寄存器基址
//       timeout,超时时间，微秒
// 返回: 发送的字节数
// =============================================================================
u32 __UART_SendStart(tagUartReg *Reg)
{
    u8 Port;

    switch((u32)Reg)
    {
    //UART0和UART1的FIFO深度为8，而其他的为1
    case CN_UART0_BASE: Port = CN_UART0; break;
    case CN_UART1_BASE:Port = CN_UART1;break;
    case CN_USART0_BASE:Port = CN_USART0;break;
    case CN_USART1_BASE:Port = CN_USART1; break;
    default:return 0;
    }
    Board_UartHalfDuplexSend(Port);
    __UART_SendIntEnable(Reg,s_UART_DmaUsed[Port]);
    return 1;
}

// =============================================================================
// 功能: 使用中断方式效率能够大大提升软件执行效率；包括接收、发送和异常等，接收到数据
//       后，调用通用接口模块，写入软件缓冲区；发送数据时，调用通用接口模块，从软件缓
//       冲区读出数据，写入硬件发送寄存器；此外，根据芯片，可通过DMA方式发送
// 参数: 中断号.
// 返回: 0.
// =============================================================================
uint32_t UART_ISR(ptu32_t IntLine)
{
    static struct UartGeneralCB *UCB;
    tagUartReg *Reg;
    s32 timeout = 1000;
    uint8_t ch,*puart_dma_send_buf,*puart_dma_recv_buf;
    uint32_t IIR=0,Port,DmaBufLen,DmaRcvLen,num;

    switch(IntLine)
    {
        case CN_INT_LINE_UART0:
            Port = CN_UART0;
            UCB = pUartCB[Port];
            Reg =  (tagUartReg *)CN_UART0_BASE;
            DmaBufLen = CFG_UART0_DMABUF_LEN;
            DmaRcvLen = s_UART0_DmaRcvLen;
            puart_dma_send_buf = (uint8_t*)UART0_DmaSendBuf;
            puart_dma_recv_buf = (uint8_t*)UART0_DmaRecvBuf;
            break;

        case CN_INT_LINE_UART1:
            Port = CN_UART1;
            UCB = pUartCB[Port];
            Reg =  (tagUartReg *)CN_UART1_BASE;
            DmaBufLen = CFG_UART1_DMABUF_LEN;
            DmaRcvLen = s_UART1_DmaRcvLen;
            puart_dma_send_buf = (uint8_t*)UART1_DmaSendBuf;
            puart_dma_recv_buf = (uint8_t*)UART1_DmaRecvBuf;
            break;

        case CN_INT_LINE_USART0:
            Port = CN_USART0;
            UCB = pUartCB[Port];
            Reg =  (tagUartReg *)CN_UART1_BASE;
            DmaBufLen = CFG_USART0_DMABUF_LEN;
            DmaRcvLen = s_USART0_DmaRcvLen;
            puart_dma_send_buf = (uint8_t*)USART0_DmaSendBuf;
            puart_dma_recv_buf = (uint8_t*)USART0_DmaRecvBuf;
            break;

        case CN_INT_LINE_USART1:
            Port = CN_USART1;
            UCB = pUartCB[Port];
            Reg =  (tagUartReg *)CN_UART1_BASE;
            DmaBufLen = CFG_USART1_DMABUF_LEN;
            DmaRcvLen = s_USART1_DmaRcvLen;
            puart_dma_send_buf = (uint8_t*)USART1_DmaSendBuf;
            puart_dma_recv_buf = (uint8_t*)USART1_DmaRecvBuf;
            break;
        default:
            return 0;
    }

    IIR = Reg->UART_SR;

    if(s_UART_DmaUsed[Port] == cn_dma_unused)//非DMA方式发送和接收
    {
        if((IIR & (1<<0)) && (Reg->UART_IMR &(1<<0)))//rxrdy int
        {
            ch = Reg->UART_RHR;
            num = UART_PortWrite(UCB,&ch,1);
            if(num != 1)
            {
                UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
            }
        }
        //tx empty int
        if((IIR &(1<<9)) && (Reg->UART_IMR &(1<<9)))
        {
            num = UART_PortRead(UCB,&ch,1);
            while((!__UART_TxTranEmpty(Reg)) && (timeout-- > 0));
            if(num != 0)
                Reg->UART_THR = ch;
            else
            {
                Board_UartHalfDuplexRecv(Port);
                __UART_SendIntDisable(Reg,s_UART_DmaUsed[Port]);
            }
        }
    }
    else            //DMA方式发送和接收
    {
        if((IIR & (1<<3)) && (Reg->UART_IMR &(1<<3)))//endrx int
        {
            if(DmaRcvLen > DmaBufLen)                   //计算从DMA BUF中读多少数据
                num = DmaBufLen;
            else
                num = DmaRcvLen;
            UART_PortWrite(UCB,puart_dma_recv_buf,num);

            DmaRcvLen = DmaRcvLen - num;
            if(DmaRcvLen > DmaBufLen)                   //计算下次DMA接收数据量
                Reg->UART_RCR  = DmaBufLen;
            else if(DmaRcvLen > 1)
                Reg->UART_RCR = DmaRcvLen;
            else                                        //接收DMA最少为1
            {
                Reg->UART_RCR = 1;
                DmaRcvLen = 1;
            }

            Reg->UART_RPR = (vu32)puart_dma_recv_buf;
        }
        if((IIR & (1<<11)) && (Reg->UART_IMR &(1<<11)))//txbufe int
        {
            while(!(Reg->UART_SR & (1<<9)));//wait for empty

            num = UART_PortRead(UCB,puart_dma_send_buf, DmaBufLen);
            if(num > 0)
            {
                Reg->UART_PTCR = (1<<9);//diable dma tx
                if((Reg->UART_TCR==0)&&(Reg->UART_TNCR==0))
                {
//                    num = UART_PortRead(UCB,puart_dma_send_buf,
//                          DmaBufLen,0);
                    Reg->UART_TPR  = (uint32_t)puart_dma_send_buf;
                    Reg->UART_TCR  = num;
                }
                Reg->UART_PTCR = (1<<8);//enable dma tx
            }
            else
            {
                //是否需禁止 dma tx int
                while(!__UART_TxTranEmpty(Reg));
                __UART_SendIntDisable(Reg,s_UART_DmaUsed[Port]);
            }
        }
    }

    if(IIR &(0xE0)) //其他中断
    {
        UART_ErrHandle(UCB,CN_UART_HARD_COMM_ERR);
    }

    return 0;
}

// =============================================================================
// 功能：初始化串口模块，使用DJYOS的串口标准模型，包括硬件、中断和串口设备加载
// 参数：串口初始化模块的参数为串口号，如UART0
// 返回：1=成功，0=失败
// =============================================================================
ptu32_t ModuleInstall_UART(ptu32_t SerialNo)
{
    struct UartParam UART_Param;

    switch(SerialNo)
    {
    case CN_UART0://串口1
        UART_Param.Name         = "UART0";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = (ptu32_t)CN_UART0_BASE;
        UART_Param.TxRingBufLen = CFG_UART0_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART0_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART1://串口2
        UART_Param.Name         = "UART1";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = (ptu32_t)CN_UART1_BASE;
        UART_Param.TxRingBufLen = CFG_UART1_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART1_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_USART0://串口1
        UART_Param.Name         = "USART0";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = (ptu32_t)CN_USART0_BASE;
        UART_Param.TxRingBufLen = CFG_USART0_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_USART0_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_USART1://串口2
        UART_Param.Name         = "USART1";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = (ptu32_t)CN_USART1_BASE;
        UART_Param.TxRingBufLen = CFG_USART1_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_USART1_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    default:
        return 0;
    }
    UART_Param.mode = CN_UART_GENERAL;
    //硬件初始化
    __UART_HardInit(SerialNo);
    __UART_IntInit(SerialNo);

    pUartCB[SerialNo] = UART_InstallGeneral(&UART_Param);
    if( pUartCB[SerialNo] == NULL)
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

    CR_Bak = PutStrDirectReg->UART_IER;             //Save INT
    __UART_SendIntDisable(PutStrDirectReg,0);       //disable send INT
    __UART_SendIntDisable(PutStrDirectReg,1);       //disable send INT when dma
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
        PutStrDirectReg->UART_THR = str[result];
    }
    PutStrDirectReg->UART_IER = CR_Bak;             //restore send INT
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

    CR_Bak = PutStrDirectReg->UART_IER;             //Save INT
    __UART_SendIntDisable(PutStrDirectReg,0);       //disable send INT
    __UART_SendIntDisable(PutStrDirectReg,1);       //disable send INT when dma
    while(__UART_RxHadChar(GetCharDirectReg) == false) ;
    result = GetCharDirectReg->UART_RHR;
    PutStrDirectReg->UART_IER = CR_Bak;             //restore send INT
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
    else if(!strcmp(StdioOut,"/dev/USART0"))
    {
        PutStrDirectReg = (tagUartReg*)CN_USART0_BASE;
        TxDirectPort = CN_USART0;
    }
    else if(!strcmp(StdioOut,"/dev/USART1"))
    {
        PutStrDirectReg = (tagUartReg*)CN_USART1_BASE;
        TxDirectPort = CN_USART1;
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
        GetCharDirectReg = (tagUartReg*)CN_UART0_BASE;
        RxDirectPort = CN_UART0;
    }
    else if(!strcmp(StdioIn,"/dev/UART1"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART1_BASE;
        RxDirectPort = CN_UART1;
    }
    else if(!strcmp(StdioIn,"/dev/USART0"))
    {
        GetCharDirectReg = (tagUartReg*)CN_USART0_BASE;
        RxDirectPort = CN_USART0;
    }
    else if(!strcmp(StdioIn,"/dev/USART1"))
    {
        GetCharDirectReg = (tagUartReg*)CN_USART1_BASE;
        RxDirectPort = CN_USART1;
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
