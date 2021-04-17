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
#include "cpu_peri_dma.h"
#include "stdlib.h"
#include "uartctrl.h"
#include "device.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern ptu32_t ModuleInstall_UART(ptu32_t SerialNo);
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
//dependence:"device file system","lock","uart device file","heap","cpu onchip dma","Nuvoton M480"//该组件的依赖组件名（可以是none，表示无依赖组件），
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
#define CFG_UART0_SENDBUF_LEN            32                 //"UART1发送环形缓冲区大小",
#define CFG_UART0_RECVBUF_LEN            32                 //"UART1接收环形缓冲区大小",
#define CFG_UART0_DMABUF_LEN             32                 //"UART1 DMA缓冲区大小",

#define CFG_UART1_SENDBUF_LEN            32                 //"UART2发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN            32                 //"UART2接收环形缓冲区大小",
#define CFG_UART1_DMABUF_LEN             32                 //"UART2 DMA缓冲区大小",

#define CFG_UART2_SENDBUF_LEN            32                 //"UART3发送环形缓冲区大小",
#define CFG_UART2_RECVBUF_LEN            32                 //"UART3接收环形缓冲区大小",
#define CFG_UART2_DMABUF_LEN             32                 //"UART3 DMA缓冲区大小",

#define CFG_UART3_SENDBUF_LEN            32                 //"UART4发送环形缓冲区大小",
#define CFG_UART3_RECVBUF_LEN            32                 //"UART4接收环形缓冲区大小",
#define CFG_UART3_DMABUF_LEN             32                 //"UART4 DMA缓冲区大小",

#define CFG_UART4_SENDBUF_LEN            32                 //"UART5发送环形缓冲区大小",
#define CFG_UART4_RECVBUF_LEN            32                 //"UART5接收环形缓冲区大小",
#define CFG_UART4_DMABUF_LEN             32                 //"UART5 DMA缓冲区大小",

#define CFG_UART5_SENDBUF_LEN            32                 //"UART6发送环形缓冲区大小",
#define CFG_UART5_RECVBUF_LEN            32                 //"UART6接收环形缓冲区大小",
#define CFG_UART5_DMABUF_LEN             32                 //"UART6 DMA缓冲区大小",
//%$#@enum,true,false,
#define CFG_UART0_ENABLE                 true               //"是否使用UART0",
#define CFG_UART1_ENABLE                 false              //"是否使用UART1",
#define CFG_UART2_ENABLE                 false              //"是否使用UART2",
#define CFG_UART3_ENABLE                 false              //"是否使用UART3",
#define CFG_UART4_ENABLE                 false              //"是否使用UART4",
#define CFG_UART5_ENABLE                 false              //"是否使用UART5",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

#if 1

extern bool_t Board_UartGpioInit(uint8_t SerialNo);
extern void Board_UartHalfDuplexSend(uint8_t SerialNo);
extern void Board_UartHalfDuplexRecv(uint8_t SerialNo);
// =============================================================================
#define tagUartReg     UART_T

#define CN_UART0_BASE           0x40070000
#define CN_UART1_BASE           0x40071000
#define CN_UART2_BASE           0x40072000
#define CN_UART3_BASE           0x40073000
#define CN_UART4_BASE           0x40074000
#define CN_UART5_BASE           0x40075000

static uint32_t TxByteTime;                    //正常发送一个字节所需要的时间
static tagUartReg *PutStrDirectReg;     //用于printk发送的串口寄存器
static tagUartReg *GetCharDirectReg;     //用于直接接收的串口寄存器
static uint32_t TxDirectPort;                  //用于printk发送的串口号
static uint32_t RxDirectPort;                  //用于直接接收的串口号
// =============================================================================
static tagUartReg volatile * const tg_UART_Reg[] = {(tagUartReg *)CN_UART0_BASE,
                                                    (tagUartReg *)CN_UART1_BASE,
                                                    (tagUartReg *)CN_UART2_BASE,
                                                    (tagUartReg *)CN_UART3_BASE,
                                                    (tagUartReg *)CN_UART4_BASE,
                                                    (tagUartReg *)CN_UART5_BASE};

typedef uint32_t (* UartStartSend)(ptu32_t PrivateTag);
//typedef u32 (* UartDirectSend)(ptu32_t PrivateTag,u8 *send_buf,u32 len,u32 timeout);
typedef ptu32_t (*UartControl)(ptu32_t PrivateTag,uint32_t cmd, uint32_t data1,uint32_t data2);

#if 1
// 串口模块初始化结构体
struct UartParam
{
    const char *Name;                   //UART名称，如UART0
    uint32_t TxRingBufLen;                   //发送缓冲区配置字节数
    uint32_t RxRingBufLen;                   //接收缓冲区配置字节数
    uint32_t Baud;                           //默认的波特率
    uint8_t  mode;                           //串口工作模式，参见CN_UART_GENERAL说明
    ptu32_t UartPortTag;                //UART私有标签，如寄存器基址
    UartStartSend StartSend;            //启动发送回调函数指针
//    UartDirectSend DirectlySend;        //直接轮询发送回调函数指针
    UartControl UartCtrl;               //控制函数回调函数指针
};
#endif

#define CN_DMA_UNUSED   0
#define CN_DMA_USED     1

// UART对应的PDMA中断号
static u8 const UartDmaRxInt[] = {PDMA_UART0_RX,PDMA_UART1_RX,
                                  PDMA_UART2_RX,PDMA_UART3_RX,
                                  PDMA_UART4_RX,PDMA_UART5_RX};
static u8 const UartDmaTxInt[] = {PDMA_UART0_TX,PDMA_UART1_TX,
                                  PDMA_UART2_TX,PDMA_UART3_TX,
                                  PDMA_UART4_TX,PDMA_UART5_TX};

static u8 const PDMA_Tx_ch[] = {0,2,4,6,8,10};
static u8 const PDMA_Rx_ch[] = {1,3,5,7,9,11};

// DMA正在使用标记，是否使用PDMA标记
static bool_t s_UART_DmaSending[] = {false,false,false,false,false,false};
static bool_t s_UART_DmaUsed[] = {false,false,false,false,false,false};
#if 0
#define UART0_SendBufLen            32
#define UART0_RecvBufLen            32
#define UART0_DmaBufLen             32

#define UART1_SendBufLen            32
#define UART1_RecvBufLen            32
#define UART1_DmaBufLen             32

#define UART2_SendBufLen            32
#define UART2_RecvBufLen            32
#define UART2_DmaBufLen             32

#define UART3_SendBufLen            32
#define UART3_RecvBufLen            32
#define UART3_DmaBufLen             32

#define UART4_SendBufLen            32
#define UART4_RecvBufLen            32
#define UART4_DmaBufLen             32

#define UART5_SendBufLen            32
#define UART5_RecvBufLen            32
#define UART5_DmaBufLen             32
#endif
static u8 *pUART_DmaSendBuf[CN_UART_NUM];
static u8 *pUART_DmaRecvBuf[CN_UART_NUM];

static struct UartCB *pUartCB[CN_UART_NUM];

//用于标识串口是否初始化标记，第0位表示UART0，第一位表UART1....
//依此类推，1表示初始化，0表示未初始化
static uint8_t sUartInited = 0;

// =============================================================================
static ptu32_t UART_ISR(ptu32_t port);
static uint32_t PDMA_ISR(ptu32_t port);
// =============================================================================
// 功能: 禁止uart的发送中断。
// 参数: flag,是否使用dma传输
//       port,串口号，1~5
// 返回: 无
// =============================================================================
void __UART_RxIntEnable(u8 flag,u8 port)
{
    if(flag == CN_DMA_USED)
        PDMA_EnableInt(PDMA,PDMA_Rx_ch[port],PDMA_INT_TRANS_DONE);
    else
        tg_UART_Reg[port]->INTEN |= (1<<0);
}

// =============================================================================
// 功能: 禁止uart的接收中断
// 参数: reg,被操作的寄存器组指针
//       port,串口号
// 返回: 无
// =============================================================================
void __UART_RxIntDisable(u8 flag, u8 port)
{
    if(flag == CN_DMA_USED)
        PDMA_DisableInt(PDMA,PDMA_Rx_ch[port], PDMA_INT_TRANS_DONE);
    else
        tg_UART_Reg[port]->INTEN &= ~(1<<0);
}

// =============================================================================
// 功能: 使能uart的发送中断
// 参数: reg,被操作的寄存器组指针
//       port,串口号
// 返回: 无
// =============================================================================
static void __UART_TxIntEnable(u8 flag,u8 port)
{
    if(flag == CN_DMA_USED)
        PDMA_EnableInt(PDMA,PDMA_Tx_ch[port], PDMA_INT_TRANS_DONE);
    else
        tg_UART_Reg[port]->INTEN |= (1<<1);
}

// =============================================================================
// 功能: 禁止UART的发送中断
// 参数: Reg,被操作的寄存器组指针
//       port,串口号
// 返回: 无
// =============================================================================
static void __UART_TxIntDisable(u8 flag, u8 port)
{
    if(flag == CN_DMA_USED)
        PDMA_DisableInt(PDMA,PDMA_Tx_ch[port], PDMA_INT_TRANS_DONE);
    else
        tg_UART_Reg[port]->INTEN &= ~(1<<1);//禁止发送空中断
}

// =============================================================================
// 功能: 检查发送寄存器的状态，如果空就返回true，若为空，否则返回false
// 参数：reg,被操作的寄存器组指针
// 返回: true = 缓冲区空，false = 非空
// =============================================================================
static bool_t __UART_TxTranEmpty(tagUartReg volatile *reg)
{
    return (0 != (reg->FIFOSTS & (1<<22)));
}

// =============================================================================
// 功能: 检查接收寄存器的状态，有数据就返回true，否则返回false
// 参数：reg,被操作的寄存器组指针
// 返回: true = 有数据，false = 无数据
// =============================================================================
static bool_t __UART_RxHadChar(tagUartReg volatile *reg)
{
    return ((reg->FIFOSTS & (1<<14)) ==0);
}

// =============================================================================
// 功能: 设置串口baud,PCLK为25M，CPU主频为100M计算,该驱动只提供9600、19200、57600、
//       115200四种波特率设置
// 参数: Reg,被操作的寄存器组指针
//       port,串口号
//       baud,波特率
// 返回: 无
// =============================================================================
static void __UART_BaudSet(tagUartReg volatile *Reg,uint8_t port,uint32_t baud)
{
    uint32_t u32UartClkSrcSel=0ul, u32UartClkDivNum=0ul;
    uint32_t u32ClkTbl[4] = {__HXT, 0ul, __LXT, __HIRC};
    uint32_t u32Baud_Div = 0ul;

    if(port == CN_UART0)
    {
        /* Get UART clock source selection */
        u32UartClkSrcSel = ((uint32_t)(CLK->CLKSEL1 & CLK_CLKSEL1_UART0SEL_Msk)) >> CLK_CLKSEL1_UART0SEL_Pos;
        /* Get UART clock divider number */
        u32UartClkDivNum = (CLK->CLKDIV0 & CLK_CLKDIV0_UART0DIV_Msk) >> CLK_CLKDIV0_UART0DIV_Pos;
    }
    else if(port == CN_UART1)
    {
        /* Get UART clock source selection */
        u32UartClkSrcSel = (CLK->CLKSEL1 & CLK_CLKSEL1_UART1SEL_Msk) >> CLK_CLKSEL1_UART1SEL_Pos;
        /* Get UART clock divider number */
        u32UartClkDivNum = (CLK->CLKDIV0 & CLK_CLKDIV0_UART1DIV_Msk) >> CLK_CLKDIV0_UART1DIV_Pos;
    }
    else if(port == CN_UART2)
    {
        /* Get UART clock source selection */
        u32UartClkSrcSel = (CLK->CLKSEL3 & CLK_CLKSEL3_UART2SEL_Msk) >> CLK_CLKSEL3_UART2SEL_Pos;
        /* Get UART clock divider number */
        u32UartClkDivNum = (CLK->CLKDIV4 & CLK_CLKDIV4_UART2DIV_Msk) >> CLK_CLKDIV4_UART2DIV_Pos;
    }
    else if(port == CN_UART3)
    {
        /* Get UART clock source selection */
        u32UartClkSrcSel = (CLK->CLKSEL3 & CLK_CLKSEL3_UART3SEL_Msk) >> CLK_CLKSEL3_UART3SEL_Pos;
        /* Get UART clock divider number */
        u32UartClkDivNum = (CLK->CLKDIV4 & CLK_CLKDIV4_UART3DIV_Msk) >> CLK_CLKDIV4_UART3DIV_Pos;
    }
    else if(port == CN_UART4)
    {
        /* Get UART clock source selection */
        u32UartClkSrcSel = (CLK->CLKSEL3 & CLK_CLKSEL3_UART4SEL_Msk) >> CLK_CLKSEL3_UART4SEL_Pos;
        /* Get UART clock divider number */
        u32UartClkDivNum = (CLK->CLKDIV4 & CLK_CLKDIV4_UART4DIV_Msk) >> CLK_CLKDIV4_UART4DIV_Pos;
    }
    else if(port == CN_UART5)
    {
        /* Get UART clock source selection */
        u32UartClkSrcSel = (CLK->CLKSEL3 & CLK_CLKSEL3_UART5SEL_Msk) >> CLK_CLKSEL3_UART5SEL_Pos;
        /* Get UART clock divider number */
        u32UartClkDivNum = (CLK->CLKDIV4 & CLK_CLKDIV4_UART5DIV_Msk) >> CLK_CLKDIV4_UART5DIV_Pos;
    }

    /* Get PLL clock frequency if UART clock source selection is PLL */
    if(u32UartClkSrcSel == 1ul)
    {
        u32ClkTbl[u32UartClkSrcSel] = CLK_GetPLLClockFreq();
    }

    /* Set UART baud rate */
    if(baud != 0ul)
    {
        u32Baud_Div = UART_BAUD_MODE2_DIVIDER((u32ClkTbl[u32UartClkSrcSel]) / (u32UartClkDivNum + 1ul), baud);

        if(u32Baud_Div > 0xFFFFul)
        {
            Reg->BAUD = (UART_BAUD_MODE0 | UART_BAUD_MODE0_DIVIDER((u32ClkTbl[u32UartClkSrcSel]) / (u32UartClkDivNum + 1ul), baud));
        }
        else
        {
            Reg->BAUD = (UART_BAUD_MODE2 | u32Baud_Div);
        }
    }

}

// =============================================================================
// 功能: 设置对应UART的IO口，包括时钟和IO配置
// 参数: SerialNo,串口号
// 返回: 无
// =============================================================================
static void __UART_GpioConfig(uint8_t SerialNo)
{
    //UART 的GPIO初始化应根据板件来定，因此该函数在boarddrv文件夹的board.c中
    Board_UartGpioInit(SerialNo);
}

// =============================================================================
// 功能: 对串口传输参数配置，包括波特率、奇偶校验、数据位、停止位
// 参数:  Reg,被操作的寄存器组指针
//        port,串口号
//        data,结构体tagCOMParam类型的指针数值
// 返回: 无
// =============================================================================
static void __UART_ComConfig(tagUartReg volatile *Reg,uint32_t port,struct COMParam *COM)
{
    if((COM == NULL) || (Reg == NULL))
        return;
    __UART_BaudSet(Reg,port,COM->BaudRate);

    switch(COM->DataBits)               // data bits
    {
    case CN_UART_DATABITS_5:    Reg->LINE |= UART_WORD_LEN_5;break;
    case CN_UART_DATABITS_6:    Reg->LINE |= UART_WORD_LEN_6;break;
    case CN_UART_DATABITS_7:    Reg->LINE |= UART_WORD_LEN_7;break;
    case CN_UART_DATABITS_8:    Reg->LINE |= UART_WORD_LEN_8;break;
    default:break;
    }

    switch(COM->Parity)                 // parity
    {
    case CN_UART_PARITY_NONE:   Reg->LINE |= UART_PARITY_NONE;break;
    case CN_UART_PARITY_ODD:    Reg->LINE |= UART_PARITY_ODD;break;
    case CN_UART_PARITY_EVEN:   Reg->LINE |= UART_PARITY_EVEN;break;
    case CN_UART_PARITY_MARK:   Reg->LINE |= UART_PARITY_MARK;break;
    case CN_UART_PARITY_SPACE:  Reg->LINE |= UART_PARITY_SPACE;break;
    default:break;
    }

    switch(COM->StopBits)               // stop bits
    {
    case CN_UART_STOPBITS_1:    Reg->LINE |= UART_STOP_BIT_1;break;
    case CN_UART_STOPBITS_1_5:  Reg->LINE |= UART_STOP_BIT_1_5;break;
    case CN_UART_STOPBITS_2:    Reg->LINE |= UART_STOP_BIT_2;break;
    default:break;
    }
}

// =============================================================================
// 功能: 使串口开始工作，时钟和uart模块
// 参数: port,串口号
// 返回: 无
// =============================================================================
void __UART_Enable(uint32_t port)
{
    switch(port)
    {//使能串口1时钟
        case CN_UART0:
            CLK->APBCLK0 |= CLK_APBCLK0_UART0CKEN_Msk; // UART0 Clock Enable
            /* Select UART clock source is HXT */
            CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_UART0SEL_Msk) | (0x0 << CLK_CLKSEL1_UART0SEL_Pos);
            break;
        case CN_UART1:
            CLK->APBCLK0 |= CLK_APBCLK0_UART1CKEN_Msk; // UART1 Clock Enable
            /* Select UART clock source is HXT */
            CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_UART1SEL_Msk) | (0x0 << CLK_CLKSEL1_UART1SEL_Pos);
            break;
        case CN_UART2:
            CLK->APBCLK0 |= CLK_APBCLK0_UART2CKEN_Msk; // UART2 Clock Enable
            /* Select UART clock source is HXT */
            CLK->CLKSEL3 = (CLK->CLKSEL3 & ~CLK_CLKSEL3_UART2SEL_Msk) | (0x0 << CLK_CLKSEL3_UART2SEL_Pos);
            break;
        case CN_UART3:
            CLK->APBCLK0 |= CLK_APBCLK0_UART3CKEN_Msk; // UART3 Clock Enable
            /* Select UART clock source is HXT */
            CLK->CLKSEL3 = (CLK->CLKSEL3 & ~CLK_CLKSEL3_UART3SEL_Msk) | (0x0 << CLK_CLKSEL3_UART3SEL_Pos);
            break;
        case CN_UART4:
            CLK->APBCLK0 |= CLK_APBCLK0_UART4CKEN_Msk; // UART4 Clock Enable
            /* Select UART clock source is HXT */
            CLK->CLKSEL3 = (CLK->CLKSEL3 & ~CLK_CLKSEL3_UART4SEL_Msk) | (0x0 << CLK_CLKSEL3_UART4SEL_Pos);
            break;
        case CN_UART5:
            CLK->APBCLK0 |= CLK_APBCLK0_UART5CKEN_Msk; // UART5 Clock Enable
            /* Select UART clock source is HXT */
            CLK->CLKSEL3 = (CLK->CLKSEL3 & ~CLK_CLKSEL3_UART5SEL_Msk) | (0x0 << CLK_CLKSEL3_UART5SEL_Pos);
            break;
        default:
            break;
    }
    //tg_UART_Reg[port]->CR1 |= (1<<13);
    tg_UART_Reg[port]->FUNCSEL = UART_FUNCSEL_UART;
}

// =============================================================================
// 功能: 使串口停止工作，包括时钟和uart模块
// 参数: port,串口号
// 返回: 无
// =============================================================================
void __UART_Disable(uint32_t port)
{
    switch(port)
    {
        case CN_UART0:      CLK_DisableModuleClock(UART0_MODULE);        break;
        case CN_UART1:      CLK_DisableModuleClock(UART1_MODULE);        break;
        case CN_UART2:      CLK_DisableModuleClock(UART2_MODULE);        break;
        case CN_UART3:      CLK_DisableModuleClock(UART3_MODULE);        break;
        case CN_UART4:      CLK_DisableModuleClock(UART4_MODULE);        break;
        case CN_UART5:      CLK_DisableModuleClock(UART5_MODULE);        break;
            break;
        default:
            break;
    }
    //tg_UART_Reg[port]->CR1 &= ~(1<<13);
    tg_UART_Reg[port]->FUNCSEL = (1<<3);;
}

// =============================================================================
// 功能: 初始化UART对应的中断线，并初始化中断入口函数
// 参数：SerialNo,串口号
// 返回: 无
// =============================================================================
static void __UART_IntInit(uint32_t SerialNo)
{
    uint8_t IntLine;
    if(SerialNo == CN_UART0)
        IntLine = CN_INT_LINE_UART0;
    else if(SerialNo == CN_UART1)
        IntLine = CN_INT_LINE_UART1;
    else if(SerialNo == CN_UART2)
        IntLine = CN_INT_LINE_UART2;
    else if(SerialNo == CN_UART3)
        IntLine = CN_INT_LINE_UART3;
    else if(SerialNo == CN_UART4)
        IntLine = CN_INT_LINE_UART4;
    else if(SerialNo == CN_UART5)
        IntLine = CN_INT_LINE_UART5;
    else
        return;

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
//       波特率:115200  ； 停止位:1 ; 校验:无校验 ; 数据位:8bit
// 参数: SerialNo,串口号，0~3
// 返回: 无
// =============================================================================
static void __UART_HardInit(uint8_t SerialNo)
{
    if(SerialNo > CN_UART5)
        return;

    __UART_GpioConfig(SerialNo); //使能管脚

    /* Select UART function */
    tg_UART_Reg[SerialNo]->FUNCSEL = UART_FUNCSEL_UART;
     /* Set UART line configuration */
    tg_UART_Reg[SerialNo]->LINE = UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1;
     /* Set UART Rx and RTS trigger level */
    tg_UART_Reg[SerialNo]->FIFO &= ~(UART_FIFO_RFITL_Msk | UART_FIFO_RTSTRGLV_Msk);

    __UART_BaudSet(tg_UART_Reg[SerialNo],SerialNo,115200); //波特率初始化

    UART_ENABLE_INT(tg_UART_Reg[SerialNo], (UART_INTEN_RDAIEN_Msk | UART_INTEN_THREIEN_Msk));
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
// 功能: 启动串口PDMA发送，使用DMA启动，在发送前需判断上次DMA是否已经发送完成，通过
//       查看标志UARTx_DMA_SENDING来判断UARTx是否已经发送完成
// 参数: PORT,串口号.
// 返回: 发送的个数
// =============================================================================
u32 __UART_PDMA_SendStart(u32 port)
{
    u32 num,addr;
    u8 *DmaSendBuf;
    u32 DmaSendBufLen;

    switch(port)
    {
    case CN_UART0:
        DmaSendBufLen = CFG_UART0_DMABUF_LEN;        break;
    case CN_UART1:
        DmaSendBufLen = CFG_UART1_DMABUF_LEN;        break;
    case CN_UART2:
        DmaSendBufLen = CFG_UART2_DMABUF_LEN;        break;
    case CN_UART3:
        DmaSendBufLen = CFG_UART3_DMABUF_LEN;        break;
    case CN_UART4:
        DmaSendBufLen = CFG_UART4_DMABUF_LEN;        break;
    case CN_UART5:
        DmaSendBufLen = CFG_UART5_DMABUF_LEN;        break;
    default:
        break;
    }
    DmaSendBuf = pUART_DmaSendBuf[port];
//    if(true == __uart_dma_timeout(s_UART_DmaSending[port]))
    if(true ==s_UART_DmaSending[port])
        return 0;
    num = UART_PortRead(pUartCB[port],DmaSendBuf,DmaSendBufLen);
    addr = (u32)DmaSendBuf;

    if(num > 0)
    {
        //DMA_Enable(UartPdmaTxStream[port],addr,num); //启动DMA
        PDMA_EnableInt(PDMA,PDMA_Tx_ch[port], 0);
        s_UART_DmaSending[port] = true;
    }
    else
    {
        tg_UART_Reg[port]->INTEN |= (1<<22);    //使能发送完成中断;
    }
    return 0;
}

// =============================================================================
// 功能: 启动串口发送，其目的是触发中断，用中断方式发送数据。
// 参数: Reg,被操作的串口寄存器指针.
// 返回: 发送的个数
// =============================================================================
static uint32_t __UART_SendStart (tagUartReg *Reg)
{
    uint8_t port;
    uint32_t delay=0;

    switch((uint32_t)Reg)
    {
    case CN_UART0_BASE:        port = CN_UART0;    break;
    case CN_UART1_BASE:        port = CN_UART1;    break;
    case CN_UART2_BASE:        port = CN_UART2;    break;
    case CN_UART3_BASE:        port = CN_UART3;    break;
    case CN_UART4_BASE:        port = CN_UART4;    break;
    case CN_UART5_BASE:        port = CN_UART5;    break;
    default:return 0;
    }

    Reg->INTEN &= ~(1<<22);
    Board_UartHalfDuplexSend(port);//切换到发送
    __UART_TxIntDisable(s_UART_DmaUsed[port],port);
    if(s_UART_DmaUsed[port])
    {
        __UART_PDMA_SendStart(port);
    }
    else
    {
        //非DMA方式，只要使能发送空中断，则触发中断
    }

    __UART_TxIntEnable(s_UART_DmaUsed[port],port);
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
    u8 *DmaRecvBuf,*DmaSendBuf;
    u32 DmaBufLen;
    switch(port)
    {
    case CN_UART0:
        DmaBufLen = CFG_UART0_DMABUF_LEN;        break;
    case CN_UART1:
        DmaBufLen = CFG_UART1_DMABUF_LEN;        break;
    case CN_UART2:
        DmaBufLen = CFG_UART2_DMABUF_LEN;        break;
    case CN_UART3:
        DmaBufLen = CFG_UART3_DMABUF_LEN;        break;
    case CN_UART4:
        DmaBufLen = CFG_UART4_DMABUF_LEN;        break;
    case CN_UART5:
        DmaBufLen = CFG_UART5_DMABUF_LEN;        break;
    default :        break;
    }

    if(pUART_DmaRecvBuf[port]==NULL)
        pUART_DmaRecvBuf[port] = (u8*)M_Malloc(DmaBufLen,0);
        if(pUART_DmaRecvBuf[port] == NULL)
            return;

    if(pUART_DmaSendBuf[port]==NULL)
        pUART_DmaSendBuf[port] = (u8*)M_Malloc(DmaBufLen,0);
        if(pUART_DmaSendBuf[port] == NULL)
        {
            free(pUART_DmaRecvBuf[port]);
            pUART_DmaRecvBuf[port]=NULL;
            return;
        }
    DmaRecvBuf = pUART_DmaRecvBuf[port];
    DmaSendBuf = pUART_DmaSendBuf[port];
    __UART_RxIntDisable(CN_DMA_UNUSED,port);
    __UART_TxIntDisable(CN_DMA_UNUSED,port);
   // tg_UART_Reg[port]->CR1 |= (1<<4);//enable idle int
   //tg_UART_Reg[port]->INTEN |= (1<<8);//enable LINIEN int

    PDMA_Config(PDMA,PDMA_Tx_ch[port],UartDmaTxInt[port],(uint32_t)DmaSendBuf, PDMA_SAR_INC,
            (uint32_t)tg_UART_Reg[port], PDMA_DAR_FIX,PDMA_WIDTH_8,32);

    PDMA_Config(PDMA,PDMA_Rx_ch[port],UartDmaRxInt[port],(uint32_t)tg_UART_Reg[port], PDMA_SAR_FIX,
            (uint32_t)DmaRecvBuf, PDMA_DAR_INC,PDMA_WIDTH_8,32);

    //DMA_IntEnable(PDMA_Tx_ch[port],DMA_INT_TCIE);  //使能发送完成中断
    //DMA_IntEnable(PDMA_Rx_ch[port],DMA_INT_HTIE);  //使能接收半完成中断

    __UART_TxIntEnable(CN_DMA_USED,port);
    __UART_RxIntEnable(CN_DMA_USED,port);

    //tg_UART_Reg[port]->CR3 |= 0x00c0;  //设置串口使用dma收发
    tg_UART_Reg[port]->INTEN |= UART_INTEN_TXPDMAEN_Msk | UART_INTEN_RXPDMAEN_Msk;
    tg_UART_Reg[port]->INTEN = UART_INTEN_RLSIEN_Msk; // Enable Receive Line interrupt
    s_UART_DmaUsed[port] = CN_DMA_USED;

    //DMA_Enable(UartDmaRxStream[port],(u32)DmaRecvBuf,32);//启动dma通道

    Int_Register(CN_INT_LINE_PDMA);
    Int_SetClearType(CN_INT_LINE_PDMA,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(CN_INT_LINE_PDMA,PDMA_ISR);
    Int_SettoAsynSignal(CN_INT_LINE_PDMA);
    Int_RestoreAsynLine(CN_INT_LINE_PDMA);

}

//=============================================================================
// 功能: 设置不使用DMA，UART不使用DMA收发，每收发一个字节就必须中断一次，不建议在高速
//      连续通信中使用。
// 参数: port,端口号，即串口号，为1~3
// 返回: 无
// =============================================================================
void __UART_SetDmaUnUsed(uint32_t port)
{
    uint8_t IntLine;
    if(port == CN_UART0)             IntLine = CN_INT_LINE_UART0;
    else if(port == CN_UART1)        IntLine = CN_INT_LINE_UART1;
    else if(port == CN_UART2)        IntLine = CN_INT_LINE_UART2;
    else if(port == CN_UART3)        IntLine = CN_INT_LINE_UART3;
    else if(port == CN_UART4)        IntLine = CN_INT_LINE_UART4;
    else if(port == CN_UART5)        IntLine = CN_INT_LINE_UART5;
    else
        return;

    free(pUART_DmaRecvBuf[port]);
    pUART_DmaRecvBuf[port]=NULL;
    free(pUART_DmaSendBuf[port]);
    pUART_DmaSendBuf[port]=NULL;

    //tg_UART_Reg[port]->CR3 &= ~0x00c0;     //设置不使用dma传输
    tg_UART_Reg[port]->INTEN &= ~UART_INTEN_TXPDMAEN_Msk;
    tg_UART_Reg[port]->INTEN &= ~UART_INTEN_RXPDMAEN_Msk;
    tg_UART_Reg[port]->INTEN &= ~UART_INTEN_RLSIEN_Msk; // Disable Receive Line interrupt
    Int_RestoreAsynLine(IntLine);
    if(s_UART_DmaUsed[port] == CN_DMA_USED)
    {
        Int_SaveAsynLine(CN_INT_LINE_PDMA);
        s_UART_DmaUsed[port] = CN_DMA_UNUSED;
    }
    __UART_RxIntEnable(CN_DMA_UNUSED,port);
    __UART_TxIntEnable(CN_DMA_UNUSED,port);
    //tg_UART_Reg[port]->CR1 &= ~(1<<4);//disable idle int
    //tg_UART_Reg[port]->INTEN &= ~(1<<8);//disable LINIEN int
}

//----串口设备控制函数---------------------------------------------------------
//功能: 串口设备的控制函数,与具体的硬件寄存器设置相关
//参数: Reg,被操作的串口寄存器指针.
//      cmd,操作类型
//      data,含义依cmd而定
//返回: 无意义.
//-----------------------------------------------------------------------------
static ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd, va_list *arg0)
{
    ptu32_t result = 0;
    u32 port;

    if(Reg == NULL)
        return 0;

    switch((uint32_t)Reg)
    {
    case CN_UART0_BASE: port = CN_UART0;break;
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
        case CN_UART_SET_BAUD:  //设置Baud
        {
            u32 data;
            data = va_arg(*arg0, u32);
            __UART_BaudSet(Reg,port, data);
        }
            break;
        case CN_UART_EN_RTS:
            //Reg->CR3 |= 0x100;
            break;
        case CN_UART_DIS_RTS:
            //Reg->CR3 &= ~0x100;
            break;
        case CN_UART_EN_CTS:
            //Reg->CR3 |= 0x200;
            break;
        case CN_UART_DIS_CTS:
            //Reg->CR3 &= ~0x200;
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

uint32_t PDMA_ISR(ptu32_t port)
{
    uint32_t status = PDMA_GET_INT_STATUS(PDMA);

    if (status & 0x1)   /* abort */
    {
        //printf("target abort interrupt !!\n");
        if (PDMA_GET_ABORT_STS(PDMA) & 0x4)
            //u32IsTestOver = 2;
        PDMA_CLR_ABORT_FLAG(PDMA,PDMA_GET_ABORT_STS(PDMA));
    }
    else if (status & 0x2)     /* done */
    {
        if ( (PDMA_GET_TD_STS(PDMA) & (1 << 0)) && (PDMA_GET_TD_STS(PDMA) & (1 << 1)) )
        {
            //u32IsTestOver = 1;
            PDMA_CLR_TD_FLAG(PDMA,PDMA_GET_TD_STS(PDMA));
        }
    }
    else if (status & 0x300)     /* channel 2 timeout */
    {
        //printf("timeout interrupt !!\n");
        //u32IsTestOver = 3;
        PDMA_CLR_TMOUT_FLAG(PDMA,0);
        PDMA_CLR_TMOUT_FLAG(PDMA,1);
    }
//    else
//        printf("unknown interrupt !!\n");

    return 0;
}

uint32_t UART_ISR(ptu32_t port)
{
    struct UartCB *UCB;
    tagUartReg *Reg;
    uint32_t num;
    uint8_t ch;
    uint32_t u32DAT;

    UCB = pUartCB[port];
    Reg = (tagUartReg *)tg_UART_Reg[port];
    if( (Reg->INTSTS & (1<<8)) &&(Reg->INTEN &(1<<0)))           //接收中断
    {
        ch = (uint8_t)Reg->DAT;
        num = UART_PortWrite(UCB,&ch,1);
        if(num != 1)
        {
            UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
        }
    }
    if((Reg->INTSTS & (1<<22))&&(Reg->INTEN & (1<<22)) )   //发生完成中断
    {
        Reg->INTEN &= ~(1<<22);
        Board_UartHalfDuplexRecv(port);
    }
    if((Reg->INTSTS & (1<<9))&&(Reg->INTEN & (1<<1)))    //发送中断
    {
        num = UART_PortRead(UCB,&ch,1);
        if(num != 0)
            Reg->DAT = ch;
        else
        {
            Reg->INTEN &= ~(1<<1);        //关闭发送中断使能
            Reg->INTEN |= (1<<22);    //使能发送完成中断
        }
    }
    if(Reg->FIFOSTS & (1<<0))        //ORE过载错误
    {
        ch = (uint8_t)Reg->DAT;
        num = Reg->FIFOSTS;     //读一下sr寄存器
        UART_ErrHandle(UCB,CN_UART_FIFO_OVER_ERR);
    }
    if((Reg->INTSTS & (1<<15)) && (Reg->INTEN & (1<<8)))
    {
        Reg->DAT;
        //Int_TapLine(UartDmaRxInt[port]);
    }

    if((tg_UART_Reg[port]->INTSTS)& UART_INTSTS_HWRLSIF_Msk)
    {
        if(tg_UART_Reg[port]->FIFOSTS & UART_FIFOSTS_BIF_Msk)
            printf("\n BIF \n");
        if(tg_UART_Reg[port]->FIFOSTS & UART_FIFOSTS_FEF_Msk)
            printf("\n FEF \n");
        if(tg_UART_Reg[port]->FIFOSTS & UART_FIFOSTS_PEF_Msk)
            printf("\n PEF \n");

        u32DAT = tg_UART_Reg[port]->DAT; // read out data
        printf("\n Error Data is '0x%x' \n", u32DAT);
        tg_UART_Reg[port]->FIFOSTS = (UART_FIFOSTS_BIF_Msk | UART_FIFOSTS_FEF_Msk | UART_FIFOSTS_PEF_Msk);
    }

    return 0;
}// =============================================================================
// 功能：初始化uart1模块,并装载串口设备，会调用uart.c的API函数。该函数主要完成的功能
//       如下：
//       1.初始化串口的硬件部分，如GPIO、中断、配置配置的COM参数，配置baud = 115200
//         stop bits = 1,无奇偶校验, data bits = 8
//       2.调用uart.c的API函数UART_InstallPort，装入串口设备
// 参数：串口号，0~3
// 返回：1=成功，0=失败
// =============================================================================
ptu32_t ModuleInstall_UART(uint32_t serial_no)
{
    struct UartParam UART_Param;

    switch(serial_no)
    {
    case CN_UART0://串口1
        UART_Param.Name         = "UART0";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART0_BASE;
        UART_Param.TxRingBufLen = CFG_UART0_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART0_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART1://串口2
        UART_Param.Name         = "UART1";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART1_BASE;
        UART_Param.TxRingBufLen = CFG_UART1_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART1_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;

    case CN_UART2://串口3
        UART_Param.Name         = "UART2";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART2_BASE;
        UART_Param.TxRingBufLen = CFG_UART2_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART2_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;

    case CN_UART3://串口4
        UART_Param.Name         = "UART3";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART3_BASE;
        UART_Param.TxRingBufLen = CFG_UART3_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART3_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART4://串口5
        UART_Param.Name         = "UART4";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART4_BASE;
        UART_Param.TxRingBufLen = CFG_UART4_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART4_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART5://串口6
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
    pUART_DmaSendBuf[serial_no]=NULL;
    pUART_DmaRecvBuf[serial_no]=NULL;
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

#if 1
// =============================================================================
// 功能：字符终端直接发送函数，采用轮询方式，直接写寄存器，用于printk，或者stdout
//       没有初始化
// 参数：str，发送字符串指针
//      len，发送的字节数
// 返回：0，发生错误；result,发送数据长度，字节单位
// =============================================================================
s32 Uart_PutStrDirect(const char *str,uint32_t len)
{
    uint32_t result = 0;
    s32 timeout = TxByteTime * len;
    uint16_t CR_Bak;
    uint8_t port;
    for(port=0;port<CN_UART_NUM;port++)
    {
        if(PutStrDirectReg ==tg_UART_Reg[port])
            break;
    }
    Board_UartHalfDuplexSend(port);
    CR_Bak = PutStrDirectReg->INTEN;                          //Save INT
    PutStrDirectReg->INTEN &= ~(1<<1);                        //disable send INT
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
        PutStrDirectReg->DAT = str[result];
    }
    PutStrDirectReg->INTEN = CR_Bak;                         //restore send INT
    while((PutStrDirectReg->FIFO &(1<<28))!=(1<<28))
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
    uint16_t CR_Bak;
    char result;

    CR_Bak = GetCharDirectReg->INTEN;                          //Save INT
    GetCharDirectReg->INTEN &= ~(1<<1);                        //disable send INT
    while(__UART_RxHadChar(GetCharDirectReg) == false) ;
    result = GetCharDirectReg->DAT;
    PutStrDirectReg->INTEN = CR_Bak;                         //restore send INT
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
#endif
#endif
