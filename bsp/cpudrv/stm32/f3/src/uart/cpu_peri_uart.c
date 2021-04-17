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
// 创建人员:
// 创建时间:
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
#include "cpu_peri_uart.h"
#include "cpu_peri_int_line.h"
//#include "cpu_peri_dma.h"
#include "stm32f303xc.h"
#include "stdlib.h"
#include "math.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。
//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern ptu32_t ModuleInstall_UART(u32 serial_no,u32 SendBufLen, u32 RecvBufLen,u8 mode);
//
//    #if CFG_UART1_ENABLE ==1
//    ModuleInstall_UART(CN_UART1,CFG_UART1_SENDBUF_LEN,CFG_UART1_RECVBUF_LEN,CFG_UART1_MODE);
//    #endif
//
//    #if CFG_UART2_ENABLE ==1
//    ModuleInstall_UART(CN_UART2,CFG_UART2_SENDBUF_LEN,CFG_UART2_RECVBUF_LEN,CFG_UART2_MODE);
//    #endif
//
//    #if CFG_UART3_ENABLE ==1
//    ModuleInstall_UART(CN_UART3,CFG_UART3_SENDBUF_LEN,CFG_UART3_RECVBUF_LEN,CFG_UART3_MODE);
//    #endif
//
//    #if CFG_UART4_ENABLE ==1
//    ModuleInstall_UART(CN_UART4,CFG_UART4_SENDBUF_LEN,CFG_UART4_RECVBUF_LEN,CFG_UART4_MODE);
//    #endif
//
//    #if CFG_UART5_ENABLE ==1
//    ModuleInstall_UART(CN_UART5,CFG_UART5_SENDBUF_LEN,CFG_UART5_RECVBUF_LEN,CFG_UART5_MODE);
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
//dependence:"device file system","uart device file","heap","cpu onchip dma"//该组件的依赖组件名（可以是none，表示无依赖组件），
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
//%$#@target = header              //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_UART    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,16,512,
#define CFG_UART1_SENDBUF_LEN            32                 //"UART1发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN            32                 //"UART1接收环形缓冲区大小",
#define CFG_UART1_DMABUF_LEN             32                 //"UART1 DMA缓冲区大小"

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
//%$#@enum,true,false,
#define CFG_UART1_MODE                   1                  //"UART1模式",CN_UART_POLL or CN_UART_GENERAL
#define CFG_UART2_MODE                   1                  //"UART2模式",CN_UART_POLL or CN_UART_GENERAL
#define CFG_UART3_MODE                   1                  //"UART3模式",CN_UART_POLL or CN_UART_GENERAL
#define CFG_UART4_MODE                   1                  //"UART4模式",CN_UART_POLL or CN_UART_GENERAL
#define CFG_UART5_MODE                   1                  //"UART5模式",CN_UART_POLL or CN_UART_GENERAL
//%$#@enum,true,false,
#define CFG_UART1_ENABLE                 true               //"是否配置使用UART1",
#define CFG_UART2_ENABLE                 false              //"是否配置使用UART2",
#define CFG_UART3_ENABLE                 false              //"是否配置使用UART3",
#define CFG_UART4_ENABLE                 false              //"是否配置使用UART4",
#define CFG_UART5_ENABLE                 false              //"是否配置使用UART5",

//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

extern void Board_UartHalfDuplexSend(u8 SerialNo);
extern void Board_UartHalfDuplexRecv(u8 SerialNo);


// =============================================================================
#define CN_UART1_BASE USART1_BASE
#define CN_UART2_BASE USART2_BASE
#define CN_UART3_BASE USART3_BASE
#define CN_UART4_BASE UART4_BASE
#define CN_UART5_BASE UART5_BASE


static u32 TxByteTime;                    //正常发送一个字节所需要的时间
static tagUartReg *PutStrDirectReg;       //用于printk发送的串口寄存器
static tagUartReg *GetCharDirectReg;      //用于直接接收的串口寄存器
static u32 TxDirectPort;                  //用于printk发送的串口号
static u32 RxDirectPort;                  //用于直接接收的串口号
// =============================================================================
static tagUartReg volatile * const tg_UART_Reg[] = {(tagUartReg *)CN_UART1_BASE,
                                                    (tagUartReg *)CN_UART2_BASE,
                                                    (tagUartReg *)CN_UART3_BASE,
                                                    (tagUartReg *)CN_UART4_BASE,
                                                    (tagUartReg *)CN_UART5_BASE,
                                                    };
#define CN_DMA_UNUSED   0
#define CN_DMA_USED     1

// UART对应的DMA Channel中断号,F3没有Stream中断
static u8 const UartDmaRxInt[] = {CN_INT_LINE_DMA1_Channel5,CN_INT_LINE_DMA1_Channel6,
                                  CN_INT_LINE_DMA1_Channel3,CN_INT_LINE_DMA2_Channel3,
                                  };

static u8 const UartDmaTxInt[] = {CN_INT_LINE_DMA1_Channel4,CN_INT_LINE_DMA1_Channel7,
                                  CN_INT_LINE_DMA1_Channel2,CN_INT_LINE_DMA2_Channel5,
                                 };

static DMA_Channel_TypeDef * const UartDmaRxChannel[] =
                                 {DMA1_Channel5,DMA1_Channel6,
                                 DMA1_Channel3,DMA2_Channel3,};

static DMA_Channel_TypeDef * const UartDmaTxChannel[] =
                                 {DMA1_Channel4,DMA1_Channel7,
                                  DMA1_Channel2,DMA2_Channel5,};

// DMA正在使用标记，是否使用DMA标记,F3 DMA 没有UART5
static bool_t s_UART_DmaSending[] = {false,false,false,false};
static bool_t s_UART_DmaUsed[]    = {false,false,false,false};

//默认DMA缓冲区Poll模式将会重新设置这几个变量
static u32 UART1_DmaBufLen = CFG_UART1_DMABUF_LEN;
static u32 UART2_DmaBufLen = CFG_UART2_DMABUF_LEN;
static u32 UART3_DmaBufLen = CFG_UART3_DMABUF_LEN;
static u32 UART4_DmaBufLen = CFG_UART4_DMABUF_LEN;
static u32 UART5_DmaBufLen = CFG_UART5_DMABUF_LEN;

static u8 *pUART_DmaSendBuf[CN_UART_NUM];
static u8 *pUART_DmaRecvBuf[CN_UART_NUM]={NULL,NULL,NULL,NULL};

struct DivPollCB
{
    u8 *DmaRecvBuf[5];
    u32 RcvBufLen[5]; //缓冲区有效数据长度
    u8 NowRecvbuf;    //当前使用的缓冲区
    u8 NoCopyBufNum;//未拷贝缓冲区数量
} *pUartPollCB[CN_UART_NUM];

static u8 UART_Mode[CN_UART_NUM];
static struct UartGeneralCB *pUartCB[CN_UART_NUM];

//用于标识串口是否初始化标记，第0位表示UART0，第一位表UART1....
//依此类推，1表示初始化，0表示未初始化
static u8 sUartInited = 0;

// =============================================================================
static ptu32_t UART_ISR(ptu32_t port);
static uint32_t UART_DmaRx_ISR(ptu32_t port);
static uint32_t UART_DmaTx_ISR(ptu32_t port);
uint32_t UART_Poll_DmaTx_ISR(ptu32_t port);
uint32_t UART_Poll_DmaRx_ISR(ptu32_t port);
// =============================================================================
// 功能: 禁止uart的接收中断。
// 参数: flag,是否使用dma传输
//       port,串口号，1~8
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
//       port,串口号
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
//       port,串口号
// 返回: 无
// =============================================================================
static void __UART_TxIntEnable(u8 flag,u8 port)
{
    if(flag == CN_DMA_USED)
        Int_RestoreAsynLine(UartDmaTxInt[port]);
    else
        tg_UART_Reg[port]->CR1 |= (1<<7);
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
        Int_SaveAsynLine(UartDmaTxInt[port]);
    else
        tg_UART_Reg[port]->CR1 &= ~(1<<7);//禁止发送空中断
}

// =============================================================================
// 功能: 检查发送寄存器的状态，
// 参数：reg,被操作的寄存器组指针
// 返回: true = 缓冲区空，false = 非空
// =============================================================================
static bool_t __UART_TxTranEmpty(tagUartReg volatile *reg)
{
    return ((reg->ISR & (1<<7)) !=0);
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

    if(port == CN_UART1) //UART1时钟源为PCLK2,高速72M
    {
        temp = (float)2*CN_CFG_PCLK2/(16*baud);
        mantissa = temp;
        fraction = (temp - mantissa)*16;
    }
    else
    {
        temp = (float)  2*CN_CFG_PCLK1/(16*baud);  //UART2 3 4 5时钟源为PCLK1,高速36M
        mantissa = temp;
        fraction = (temp - mantissa)*16;
    }

    Reg->BRR =  ((u16)mantissa<<4)+((u16)fraction>>1);
//      Reg->BRR =  ((u16)mantissa<<4)+(u16)fraction;

    if(TxDirectPort == port)
    {
        TxByteTime = 11000000/baud;  //1个字节传输时间，按10bit，+10%计算
    }
}

// =============================================================================
// 功能: 设置对应UART的IO口，包括时钟和IO配置
// 参数: SerialNo,串口号
// 注 :因串口复用引脚不确定性在需要初始化需要的引脚
// 返回: 无
// =============================================================================
static void __UART_GpioConfig(u8 SerialNo)
{

    switch(SerialNo)
    {
    case CN_UART1:
        RCC->APB2ENR |=(1<<14); //串口时钟使能
        break;
    case CN_UART2:
        RCC->APB1ENR |=(1<<17);
        break;
    case CN_UART3:
        RCC->APB1ENR |=(1<<18);

        break;
    case CN_UART4:
        RCC->APB1ENR |=(1<<19);
        break;
    case CN_UART5:
        RCC->APB1ENR |=(1<<20);
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
            switch(COM->Parity)
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
        Reg->CR2 &= ~(3<<12);
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
            RCC->APB2ENR |=(1<<14);break;
        case CN_UART2:
            RCC->APB1ENR |=(1<<17);break;
        case CN_UART3:
            RCC->APB1ENR |=(1<<18);break;
        case CN_UART4:
            RCC->APB1ENR |=(1<<19);break;
        case CN_UART5:
            RCC->APB1ENR |=(1<<20);break;
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
        case CN_UART1:  RCC->APB2ENR &=~(1<<14);  break;
        case CN_UART2:  RCC->APB1ENR &=~(1<<17); break;
        case CN_UART3:  RCC->APB1ENR &=~(1<<18); break;
        case CN_UART4:  RCC->APB1ENR &=~(1<<19); break;
        case CN_UART5:  RCC->APB1ENR &=~(1<<20); break;
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
                default:
                    return;
    }
    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,UART_ISR);
    Int_SettoAsynSignal(IntLine);
//    Int_SettoReal(IntLine);
    Int_ClearLine(IntLine);
    Int_SetIsrPara(IntLine,SerialNo);
    Int_RestoreAsynLine(IntLine);
    Int_SetIsrPara(IntLine,SerialNo);   //todo:严滔检查这句是否需要
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
    __UART_GpioConfig(SerialNo);  //时钟使能
   //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
   //初始化uart硬件控制数据结构
    tg_UART_Reg[SerialNo]->CR1 = 0;
    tg_UART_Reg[SerialNo]->CR3 |= (1<<14);
    tg_UART_Reg[SerialNo]->CR1 = 0x802d;
    tg_UART_Reg[SerialNo]->CR2 = 0x0;
    tg_UART_Reg[SerialNo]->CR3 = 0x0000;
    __UART_BaudSet(tg_UART_Reg[SerialNo],SerialNo,115200);
    DJY_DelayUs(100);//
    tg_UART_Reg[SerialNo]->ICR|=(1<<4);
    Board_UartHalfDuplexRecv(SerialNo);
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
u32 __UART_DMA_SendStart(u32 port)     //uart5没有对应DMA
{
    u32 num,addr;
    u8 *DmaSendBuf;
    u32 DmaSendBufLen;

    switch(port)
    {
        case CN_UART1:
            DmaSendBufLen = UART1_DmaBufLen; break;
        case CN_UART2:
            DmaSendBufLen = UART2_DmaBufLen; break;
        case CN_UART3:
            DmaSendBufLen = UART3_DmaBufLen; break;
        case CN_UART4:
            DmaSendBufLen = UART4_DmaBufLen; break;
        case CN_UART5:
            DmaSendBufLen = UART5_DmaBufLen; break;
         default:   break;
    }

    DmaSendBuf = pUART_DmaSendBuf[port];
//    if(true == __uart_dma_timeout(s_UART_DmaSending[port]))
    if(true ==s_UART_DmaSending[port])
        return 0;
    num = UART_PortRead(pUartCB[port],DmaSendBuf,DmaSendBufLen);
    addr = (u32)DmaSendBuf;

    if(num > 0)
    {
        DMA_Enable(UartDmaTxChannel[port],addr,num); //启动DMA
        s_UART_DmaSending[port] = true;
    }
    else
    {
        tg_UART_Reg[port]->CR1 |= (1<<6);
    }
    return 0;
}

// =============================================================================
// 功能: 启动串口发送，其目的是触发中断，用中断方式发送数据。
// 参数: Reg,被操作的串口寄存器指针.
// 返回: 发送的个数
// =============================================================================
static u32 __UART_SendStart (tagUartReg *Reg)
{
    u8 port;

    switch((u32)Reg)
    {
        case CN_UART1_BASE:        port = CN_UART1;    break;
        case CN_UART2_BASE:        port = CN_UART2;    break;
        case CN_UART3_BASE:        port = CN_UART3;    break;
        case CN_UART4_BASE:        port = CN_UART4;    break;
        case CN_UART5_BASE:        port = CN_UART5;    break;
        default:return 0;
    }
    Reg->CR1 &=~(1<<6);
    Board_UartHalfDuplexSend(port);//切换到发送
    __UART_TxIntDisable(s_UART_DmaUsed[port],port);
    if(s_UART_DmaUsed[port])
    {
        __UART_DMA_SendStart(port);
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
    case CN_UART1:
        DmaBufLen = UART1_DmaBufLen;   break;
    case CN_UART2:
        DmaBufLen = UART2_DmaBufLen;   break;
    case CN_UART3:
        DmaBufLen = UART3_DmaBufLen;   break;
    case CN_UART4:
        DmaBufLen = UART4_DmaBufLen;   break;
    case CN_UART5:
        DmaBufLen = UART5_DmaBufLen;   break;
    default :        break;
    }

    if(pUART_DmaRecvBuf[port]==NULL)  //分配内存
    {
        pUART_DmaRecvBuf[port] = (u8*)M_Malloc(DmaBufLen,0);
        if(pUART_DmaRecvBuf[port] == NULL)
            goto exit_from_Rcvbuf;
    }
    if(pUART_DmaSendBuf[port]==NULL)
    {
        pUART_DmaSendBuf[port] = (u8*)M_Malloc(DmaBufLen,0);
        if(pUART_DmaSendBuf[port] == NULL)
            goto exit_from_Sendbuf;
    }
    if(UART_Mode[port] == CN_UART_POLL)
    {
        if(pUartPollCB[port]==NULL)
        {
            pUartPollCB[port]= (struct DivPollCB *)M_Malloc(sizeof( struct DivPollCB),0);
            if(pUartPollCB[port]==NULL)
                goto exit_from_Poll;
            memset(pUartPollCB[port],0x00,sizeof(struct DivPollCB));
        }
        pUartPollCB[port]->DmaRecvBuf[0]=pUART_DmaRecvBuf[port];

        if(pUartPollCB[port]->DmaRecvBuf[1]==NULL)
        {
            pUartPollCB[port]->DmaRecvBuf[1] = (u8*)M_Malloc(DmaBufLen,0);
            if(pUartPollCB[port]->DmaRecvBuf[1] == NULL)
                goto exit_from_Rcvbuf2;
        }
        if(pUartPollCB[port]->DmaRecvBuf[2]==NULL)
        {
            pUartPollCB[port]->DmaRecvBuf[2] = (u8*)M_Malloc(DmaBufLen,0);
            if(pUartPollCB[port]->DmaRecvBuf[2] == NULL)
                goto exit_from_Rcvbuf2;
        }
        if(pUartPollCB[port]->DmaRecvBuf[3]==NULL)
        {
            pUartPollCB[port]->DmaRecvBuf[3] = (u8*)M_Malloc(DmaBufLen,0);
            if(pUartPollCB[port]->DmaRecvBuf[3] == NULL)
                goto exit_from_Rcvbuf2;
        }
        if(pUartPollCB[port]->DmaRecvBuf[4]==NULL)
        {
           pUartPollCB[port]->DmaRecvBuf[4] = (u8*)M_Malloc(DmaBufLen,0);
           if(pUartPollCB[port]->DmaRecvBuf[4] == NULL)
               goto exit_from_Rcvbuf2;
        }
        pUartPollCB[port]->NowRecvbuf = 0;
        pUartPollCB[port]->NoCopyBufNum = 0;
        free(pUART_DmaSendBuf[port]); //poll模式发送缓冲区会在发送时定位到应用层的数据缓冲区
    }

    DmaRecvBuf = pUART_DmaRecvBuf[port];
    DmaSendBuf = pUART_DmaSendBuf[port];
    __UART_RxIntDisable(CN_DMA_UNUSED,port); //串口中断失能
    __UART_TxIntDisable(CN_DMA_UNUSED,port);
    tg_UART_Reg[port]->CR1 |= (1<<4);        //使能IDLE中断

    DMA_Config(UartDmaRxChannel[port],(u32)&(tg_UART_Reg[port]->RDR),  //DMA配置
            (u32)DmaRecvBuf,DMA_DIR_P2M,DMA_DATABITS_8,DMA_DATABITS_8,DmaBufLen);

    DMA_Config(UartDmaTxChannel[port],(u32)&(tg_UART_Reg[port]->TDR),
            (u32)DmaSendBuf,DMA_DIR_M2P,DMA_DATABITS_8,DMA_DATABITS_8,DmaBufLen);

    tg_UART_Reg[port]->CR3 |= 0x00c0;  //设置串口使用DMA收发
    s_UART_DmaUsed[port] = CN_DMA_USED;

    Int_Register(UartDmaTxInt[port]);  //注册发送DMA中断
    Int_Register(UartDmaRxInt[port]);  //注册接收DMA中断

    Int_SetClearType(UartDmaRxInt[port],CN_INT_CLEAR_AUTO);  //关联中断
    Int_SetClearType(UartDmaTxInt[port],CN_INT_CLEAR_AUTO);

    if(UART_Mode[port] == CN_UART_GENERAL)
    {
        Int_IsrConnect(UartDmaRxInt[port],UART_DmaRx_ISR);
        Int_IsrConnect(UartDmaTxInt[port],UART_DmaTx_ISR);
    }
    else
    {
        Int_IsrConnect(UartDmaRxInt[port],UART_Poll_DmaRx_ISR);
        Int_IsrConnect(UartDmaTxInt[port],UART_Poll_DmaTx_ISR);
    }

    Int_SettoAsynSignal(UartDmaRxInt[port]);
    Int_SettoAsynSignal(UartDmaTxInt[port]);

//    Int_SettoReal(UartDmaRxInt[port]);
//    Int_SettoReal(UartDmaTxInt[port]);

    Int_SetIsrPara(UartDmaRxInt[port],port);
    Int_SetIsrPara(UartDmaTxInt[port],port);

    Int_RestoreAsynLine(UartDmaRxInt[port]);
    Int_RestoreAsynLine(UartDmaTxInt[port]);

    Int_ClearLine(UartDmaTxInt[port]);
    Int_ClearLine(UartDmaRxInt[port]);

    DMA_Enable(UartDmaRxChannel[port],(u32)DmaRecvBuf,DmaBufLen);  //开启DMA传输
    return ;

    exit_from_Rcvbuf2:
        free(pUartPollCB[port]);
    exit_from_Poll:
        free(pUART_DmaSendBuf[port]);
        pUART_DmaSendBuf[port]=NULL;
    exit_from_Sendbuf:
        free(pUART_DmaRecvBuf[port]);
        pUART_DmaRecvBuf[port]=NULL;
    exit_from_Rcvbuf:
        return;
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
    else
    return;

    free(pUART_DmaRecvBuf[port]);
    pUART_DmaRecvBuf[port]=NULL;
    free(pUART_DmaSendBuf[port]);
    pUART_DmaSendBuf[port]=NULL;

    tg_UART_Reg[port]->CR3 &= ~0x00c0;  //设置不使用dma传输
    Int_RestoreAsynLine(IntLine);
    if(s_UART_DmaUsed[port] == CN_DMA_USED)
    {
        Int_SaveAsynLine(UartDmaRxInt[port]);
        Int_SaveAsynLine(UartDmaTxInt[port]);
        s_UART_DmaUsed[port] = CN_DMA_UNUSED;
    }
    __UART_RxIntEnable(CN_DMA_UNUSED,port);
    __UART_TxIntEnable(CN_DMA_UNUSED,port);
    tg_UART_Reg[port]->CR1 &= ~(1<<4); //disable idle int
}
// =============================================================================
// 功能: 串口设备的控制函数,与具体的硬件寄存器设置相关
// 参数: Reg,被操作的串口寄存器指针.
//       cmd,操作类型
//       data1,data2,含义依cmd而定
// 返回: 无意义.
// =============================================================================
static ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd, va_list *args)
{
    ptu32_t result = 0;
    u32 port;
    u32 len,len1,len2,len3;
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
            data = va_arg(*args, u32);
             __UART_BaudSet(Reg,port, data);
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
            struct COMParam *com;
            com = va_arg(*args, struct COMParam *);
            __UART_ComConfig(Reg,port,com);
            break;
        }
        case CN_UART_SEND_BUF:
        {
            u32 mar;
            u16 ndtr;
            mar = va_arg(*args, u32);
            ndtr = va_arg(*args, u16);
            Board_UartHalfDuplexSend(port);
            DMA_Enable(UartDmaTxChannel[port],mar,ndtr);
            result = ndtr;
            break;
        }
        case CN_UART_RECV_BUF:
            switch (pUartPollCB[port]->NoCopyBufNum)
            {
                u8 *buf;
                s32 rcvlen;
                buf = va_arg(*args, char *);
                rcvlen = va_arg(*args, s32);
                case 1:
                    switch (pUartPollCB[port]->NowRecvbuf)
                    {
                       case 0:memcpy(buf,pUartPollCB[port]->DmaRecvBuf[4],rcvlen);
                              break;
                       case 1:memcpy(buf,pUartPollCB[port]->DmaRecvBuf[0],rcvlen);
                        break;
                       case 2:memcpy(buf,pUartPollCB[port]->DmaRecvBuf[1],rcvlen);
                              break;
                       case 3:memcpy(buf,pUartPollCB[port]->DmaRecvBuf[2],rcvlen);
                              break;
                       case 4:memcpy(buf,pUartPollCB[port]->DmaRecvBuf[3],rcvlen);
                              break;
                       default:  break;
                    }
                    break;
                case 2:
                    switch (pUartPollCB[port]->NowRecvbuf)
                        {
                       case 0: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[3]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[3],len);
                               memcpy((buf+pUartPollCB[port]->RcvBufLen[3]),pUartPollCB[port]->DmaRecvBuf[4],rcvlen-len);
                               break;
                       case 1: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[4]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[4],len);
                               memcpy((buf+pUartPollCB[port]->RcvBufLen[4]),pUartPollCB[port]->DmaRecvBuf[0],rcvlen-len);
                               break;
                       case 2: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[0]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[0],len);
                               memcpy((buf+pUartPollCB[port]->RcvBufLen[0]),pUartPollCB[port]->DmaRecvBuf[1],rcvlen-len);
                               break;
                       case 3: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[1]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[1],len);
                               memcpy((buf+pUartPollCB[port]->RcvBufLen[1]),pUartPollCB[port]->DmaRecvBuf[2],rcvlen-len);
                               break;
                       case 4: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[2]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[2],len);
                               memcpy((buf+pUartPollCB[port]->RcvBufLen[2]),pUartPollCB[port]->DmaRecvBuf[3],rcvlen-len);
                               break;
                       default:  break;
                    }
                    break;
                case 3:
                    switch (pUartPollCB[port]->NowRecvbuf)
                    {
                       case 0: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[2]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[3]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[2],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[3],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[4],rcvlen-len-len1);
                               break;
                       case 1: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[3]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[4]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[3],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[4],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[0],rcvlen-len-len1);
                               break;
                       case 2: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[4]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[0]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[4],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[0],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[1],rcvlen-len-len1);
                               break;
                       case 3: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[0]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[1]);
                            memcpy(buf,pUartPollCB[port]->DmaRecvBuf[0],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[1],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[2],rcvlen-len-len1);
                               break;
                       case 4: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[1]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[2]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[1],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[2],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[3],rcvlen-len-len1);
                               break;
                       default:  break;
                        }
                    break;
                case 4:
                    switch (pUartPollCB[port]->NowRecvbuf)
                        {
                       case 0: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[1]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[2]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[3]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[1],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[2],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[3],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[4],rcvlen-len-len1-len2);
                               break;
                       case 1: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[2]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[3]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[4]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[2],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[3],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[4],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[0],rcvlen-len-len1-len2);
                               break;
                       case 2: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[3]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[4]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[0]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[3],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[4],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[0],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[1],rcvlen-len-len1-len2);
                               break;
                       case 3: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[4]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[0]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[1]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[4],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[0],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[1],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[2],rcvlen-len-len1-len2);
                               break;
                       case 4: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[0]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[1]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[2]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[0],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[1],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[2],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[3],rcvlen-len-len1-len2);
                               break;
                       default:  break;
                        }
                        break;
                case 5:
                    switch (pUartPollCB[port]->NowRecvbuf)
                    {
                       case 0: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[0]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[1]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[2]);
                               len3 = MIN(rcvlen-len-len1-len2,pUartPollCB[port]->RcvBufLen[3]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[0],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[1],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[2],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[3],len3);
                               memcpy((buf+len+len1+len2+len3),pUartPollCB[port]->DmaRecvBuf[4],rcvlen-len-len1-len2-len3);
                               break;
                       case 1: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[1]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[2]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[3]);
                               len3 = MIN(rcvlen-len-len1-len2,pUartPollCB[port]->RcvBufLen[4]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[1],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[2],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[3],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[4],len3);
                               memcpy((buf+len+len1+len2+len3),pUartPollCB[port]->DmaRecvBuf[0],rcvlen-len-len1-len2-len3);
                               break;
                       case 2: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[2]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[3]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[4]);
                               len3 = MIN(rcvlen-len-len1-len2,pUartPollCB[port]->RcvBufLen[0]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[2],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[3],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[4],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[0],len3);
                               memcpy((buf+len+len1+len2+len3),pUartPollCB[port]->DmaRecvBuf[1],rcvlen-len-len1-len2-len3);
                               break;
                       case 3: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[3]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[4]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[0]);
                               len3 = MIN(rcvlen-len-len1-len2,pUartPollCB[port]->RcvBufLen[1]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[3],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[4],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[0],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[1],len3);
                               memcpy((buf+len+len1+len2+len3),pUartPollCB[port]->DmaRecvBuf[2],rcvlen-len-len1-len2-len3);
                               break;
                       case 4: len = MIN(rcvlen,pUartPollCB[port]->RcvBufLen[4]);
                               len1 = MIN(rcvlen-len,pUartPollCB[port]->RcvBufLen[0]);
                               len2 = MIN(rcvlen-len-len1,pUartPollCB[port]->RcvBufLen[1]);
                               len3 = MIN(rcvlen-len-len1-len2,pUartPollCB[port]->RcvBufLen[2]);
                               memcpy(buf,pUartPollCB[port]->DmaRecvBuf[4],len);
                               memcpy((buf+len),pUartPollCB[port]->DmaRecvBuf[0],len1);
                               memcpy((buf+len+len1),pUartPollCB[port]->DmaRecvBuf[1],len2);
                               memcpy((buf+len+len1+len2),pUartPollCB[port]->DmaRecvBuf[2],len3);
                               memcpy((buf+len+len1+len2+len3),pUartPollCB[port]->DmaRecvBuf[3],rcvlen-len-len1-len2-len3);
                               break;
                    default:                break;
                }
                break;
                default:    break;
            }
            pUartPollCB[port]->NoCopyBufNum = 0;  //清0
//            pUartPollCB[port]->RcvBufLen[0] = 0;
//            pUartPollCB[port]->RcvBufLen[1] = 0;
            break;
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

    switch(port)
    {
    case CN_UART1:
        DmaSendBufLen = UART1_DmaBufLen;        break;
    case CN_UART2:
        DmaSendBufLen = UART2_DmaBufLen;        break;
    case CN_UART3:
        DmaSendBufLen = UART3_DmaBufLen;        break;
    case CN_UART4:
        DmaSendBufLen = UART4_DmaBufLen;        break;
    case CN_UART5:
        DmaSendBufLen = UART5_DmaBufLen;        break;
    default:        break;
    }

    DmaSendBuf = pUART_DmaSendBuf[port];
    DMA_ClearIntFlag(UartDmaTxChannel[port]);
    num = UART_PortRead(pUartCB[port],DmaSendBuf,DmaSendBufLen);
    if(num == 0)
    {
        s_UART_DmaSending[port] = false;
        tg_UART_Reg[port]->CR1 |= (1<<6);
    }
    else
    {
        DMA_Enable(UartDmaTxChannel[port],(u32)DmaSendBuf,num);
    }
    return 0;
}
// =============================================================================
// 功能: 当uart2配置成dma方式是，dma1 ch7负责发送，ch6负责接收
// 参数: 中断函数没有参数.
// 返回: 中断函数没有返回值.
// =============================================================================
uint32_t UART_Poll_DmaTx_ISR(ptu32_t port)
{

    DMA_ClearIntFlag(UartDmaTxChannel[port]);
    s_UART_DmaSending[port] = false;
    UART_PollPortRead(pUartCB[port]);
    //todo:为了提高切换的速度这里不判断TC中断完成
    //进IDLE中断的时间可满足4M波特率一个字节
    Board_UartHalfDuplexRecv(port);
    return 0;
}
// =============================================================================
// 功能: CN_UART_POLL模式下DMA接收中断处理函数
//      接收中断有两个缓冲区采用乒乓缓存的模式
// 参数:
// 返回:
// =============================================================================
uint32_t UART_Poll_DmaRx_ISR(ptu32_t port)
{
    u32 recvs,num,DmaRecvBufLen;
    DMA_Channel_TypeDef *DMA_Channelx;
    u8 channelx;
    u32 ifcr;
    DMA_TypeDef *DMAx;
    u8 RcvBufNum;
    switch(port)
    {
    case CN_UART1:
        DmaRecvBufLen = UART1_DmaBufLen;        break;
    case CN_UART2:
        DmaRecvBufLen = UART2_DmaBufLen;        break;
    case CN_UART3:
        DmaRecvBufLen = UART3_DmaBufLen;        break;
    case CN_UART4:
        DmaRecvBufLen = UART4_DmaBufLen;        break;
    case CN_UART5:
        DmaRecvBufLen = UART5_DmaBufLen;        break;
    default:        break;
    }

    DMA_Channelx = UartDmaRxChannel[port];
    if((u32)DMA_Channelx>(u32)DMA2)   //得到当前stream是属于DMA2还是DMA1
        DMAx=DMA2;
    else
        DMAx=DMA1;
    channelx=((((u32)DMA_Channelx-(u32)DMAx)-0X08)/0X14)+1;  //得到通道号
    ifcr = 0x0000000f<<((channelx-1)*4);

    if(0 == DmaRecvBufLen - UartDmaRxChannel[port]->CNDTR)
        return 0;


    RcvBufNum = pUartPollCB[port]->NowRecvbuf;


    switch (RcvBufNum)  //缓冲区切换
    {
       case 0:pUartPollCB[port]->NowRecvbuf = 1;
              break;
       case 1:pUartPollCB[port]->NowRecvbuf = 2;
              break;
       case 2:pUartPollCB[port]->NowRecvbuf = 3;
              break;
       case 3:pUartPollCB[port]->NowRecvbuf = 4;
              break;
       case 4:pUartPollCB[port]->NowRecvbuf = 0;
              break;
       default:  break;
    }

    atom_low_t atom_low = Int_LowAtomStart(); //进入原子操作状态
    DMA_Channelx->CCR&=~(1<<0);               //关闭DMA传输
    while(DMA_Channelx->CCR&0X1);             //确保DMA可以被设置
    DMAx->IFCR|=ifcr;     //前4位置1清零，其他不变
    recvs = DmaRecvBufLen - UartDmaRxChannel[port]->CNDTR;
    DMA_Channelx->CNDTR=DmaRecvBufLen;        //DMA 数据量
    DMA_Channelx->CMAR =(u32)pUartPollCB[port]->DmaRecvBuf[pUartPollCB[port]->NowRecvbuf]; //DMA内存地址
    DMA_Channelx->CCR|= (1<<0);     //开启DMA传输
    Int_LowAtomEnd(atom_low);

    if(pUartPollCB[port]->NoCopyBufNum < 5)
    {
        pUartPollCB[port]->NoCopyBufNum++;
    }
    else
    {
        UART_ErrHandle(pUartCB[port],CN_UART_FIFO_OVER_ERR);
    }

    pUartPollCB[port]->RcvBufLen[RcvBufNum] = recvs;
    num = UART_PollPortWrite(pUartCB[port],recvs);
    if(num != recvs)
    {
        UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
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
    u32 recvs,num,DmaRecvBufLen;
    u8 *DmaRecvBuf;

    switch(port)
    {
    case CN_UART1:
        DmaRecvBufLen = UART1_DmaBufLen;        break;
    case CN_UART2:
        DmaRecvBufLen = UART2_DmaBufLen;        break;
    case CN_UART3:
        DmaRecvBufLen = UART3_DmaBufLen;        break;
    case CN_UART4:
        DmaRecvBufLen = UART4_DmaBufLen;        break;
    case CN_UART5:
        DmaRecvBufLen = UART5_DmaBufLen;        break;
    default:        break;
    }

    DmaRecvBuf = pUART_DmaRecvBuf[port];
    DMA_Disable(UartDmaRxChannel[port]);
    DMA_ClearIntFlag(UartDmaRxChannel[port]);
    recvs = DmaRecvBufLen - UartDmaRxChannel[port]->CNDTR;
    if(recvs == DmaRecvBufLen)     //dma缓冲区满，可能有数据丢失
    {
        UART_ErrHandle(pUartCB[port],CN_UART_FIFO_OVER_ERR);
    }else
    {
        num = UART_PortWrite(pUartCB[port],DmaRecvBuf,recvs);
        if(num != recvs)
        {
            UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
        }
    }

    DMA_Enable(UartDmaRxChannel[port],(u32)DmaRecvBuf,DmaRecvBufLen);
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
u32 CNT1=0,CNT2=0,CNT3=0;
u32 UART_ISR(ptu32_t port)
{
    struct UartGeneralCB *UCB;
    tagUartReg *Reg;
    u32 num;
    u8 ch;

    UCB = (struct UartGeneralCB*)pUartCB[port];
    Reg = (tagUartReg *)tg_UART_Reg[port];
    if( (Reg->ISR & (1<<5)) &&(Reg->CR1 &(1<<5))) //接收中断
    {
        ch = (u8)Reg->RDR;

//        extern void ReceiveData(u8 data);
//        ReceiveData(ch);

        num = UART_PortWrite(UCB,&ch,1);
        if(num != 1)
        {
            UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
        }
    }
    if((Reg->ISR & (1<<6)) && (Reg->CR1&(1<<6)))
    {
        Reg->CR1 &=~(1<<6);//关TC中断
        Board_UartHalfDuplexRecv(port);
    }

    if((Reg->ISR & (1<<7)) && (Reg->CR1 & (1<<7))) //发送中断
    {
        num = UART_PortRead(UCB,&ch,1);
        if(num != 0)
            Reg->TDR = ch;
        else
        {
            Reg->CR1 &= ~(1<<7);
            Reg->CR1 |= (1<<6);
        }
    }

    if(Reg->ISR & (1<<3) )   //ORE过载错误
    {
        CNT3++;
        ch = (u8)Reg->RDR;
        num = Reg->ISR;      //读一下sr寄存器
        UART_ErrHandle(UCB,CN_UART_FIFO_OVER_ERR);
        Reg->ICR |=(1<<3);   //清过载标志位
    }

    if((Reg->ISR & (1<<4)) && (Reg->CR1 & (1<<4)))  //IDLE
    {

        CNT1++;
        Reg->ICR |=(1<<4);//todo
        if(UART_Mode[port] == CN_UART_POLL)
        {
            UART_Poll_DmaRx_ISR(port);
        }
        else
            UART_DmaRx_ISR(port);
    }

    if(Reg->ISR & (1<<1))//帧错误直接清除暂时不做处理
    {
        CNT2++;
        Reg->ICR |=(1<<1);
    }
    return 0;
}

// =============================================================================
// 功能：初始化uart1模块,并装载串口设备，会调用uart.c的API函数。该函数主要完成的功能
//       如下：
//       1.初始化串口的硬件部分，如GPIO、中断、配置配置的COM参数，配置baud = 115200
//         stop bits = 1,无奇偶校验, data bits = 8
//       2.调用uart.c的API函数UART_InstallPort，装入串口设备
// 参数：serial_no，串口号，1~4
//       SendBufLen，若mode==CN_UART_GENERAL，则是发送环形缓冲区长度
//                   若mode==CN_UART_POLL，则是发送数据包的最大长度
//       RecvBufLen，若mode==CN_UART_GENERAL，则是接收环形缓冲区长度
//                   若mode==CN_UART_POLL，则是接收数据包的最大长度
//       mode :串口工作模式，参看CN_UART_GENERAL和CN_UART_POLL的说明
// 返回：1=成功，0=失败
// =============================================================================
ptu32_t ModuleInstall_UART(u32 serial_no,u32 SendBufLen, u32 RecvBufLen,u8 mode)
{
    struct UartParam UART_Param;
    u32 Maxbuflen = 2*MAX(SendBufLen,RecvBufLen);

    switch(serial_no)
    {
        case CN_UART1://串口1
            UART_Param.Name         = "UART1";
            if(mode == CN_UART_POLL)
                UART1_DmaBufLen     = Maxbuflen;
            break;
        case CN_UART2://串口2
            UART_Param.Name         = "UART2";
            if(mode == CN_UART_POLL)
                UART2_DmaBufLen     = Maxbuflen;
            break;
        case CN_UART3://串口3
            UART_Param.Name         = "UART3";
            if(mode == CN_UART_POLL)
                UART3_DmaBufLen     = Maxbuflen;
            break;
        case CN_UART4://串口4
            UART_Param.Name         = "UART4";
            if(mode == CN_UART_POLL)
                UART4_DmaBufLen     = Maxbuflen;
            break;
        case CN_UART5://串口5
            UART_Param.Name         = "UART5";
            if(mode == CN_UART_POLL)
                UART5_DmaBufLen     = Maxbuflen;
            break;
        default:
            return 0;
    }
    UART_Param.TxRingBufLen = SendBufLen;
    UART_Param.RxRingBufLen = RecvBufLen;
    UART_Param.mode         = mode;
    UART_Param.Baud         = 115200;
    UART_Param.UartPortTag  = (ptu32_t)tg_UART_Reg[serial_no];
    UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
    UART_Param.StartSend    = (UartStartSend)__UART_SendStart;

    UART_Mode[serial_no]       = mode;
    pUART_DmaSendBuf[serial_no]= NULL;
    pUART_DmaRecvBuf[serial_no]= NULL;
    //硬件初始化
    __UART_HardInit(serial_no);
    if(UART_Mode[serial_no] == CN_UART_POLL)
    {
        __UART_SetDmaUsed(serial_no);
    }

    __UART_IntInit(serial_no);  //中断初始化，将UART_ISR设置成实时中断，UART_ISR中有发送、接收、IDLE、帧错误

    sUartInited |= (0x01 << serial_no);
    if(CN_UART_POLL == mode)
        pUartCB[serial_no] = UART_InstallPoll(&UART_Param);
    else if(CN_UART_GENERAL == mode)
        pUartCB[serial_no] = UART_InstallGeneral(&UART_Param);

    if( pUartCB[serial_no] == NULL)
        return 0;

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
    u32 result = 0,timeout = TxByteTime * len;
    u16 CR_Bak;
    u8 port;
    for(port=0;port<CN_UART_NUM;port++)
    {
        if(PutStrDirectReg ==tg_UART_Reg[port])
            break;
    }
    Board_UartHalfDuplexSend(port);
    CR_Bak = PutStrDirectReg->CR1;  //Save INT
    PutStrDirectReg->CR1 &= ~(1<<7);   //disable send INT
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
    PutStrDirectReg->CR1 = CR_Bak;  //restore send INT
    while((PutStrDirectReg->ISR &(1<<6))!=(1<<6))
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

    CR_Bak = GetCharDirectReg->CR1;  //Save INT
    GetCharDirectReg->CR1 &= ~(1<<7);  //disable send INT
    while(__UART_RxHadChar(GetCharDirectReg) == false) ;
    result = GetCharDirectReg->RDR;
    PutStrDirectReg->CR1 = CR_Bak;   //restore send INT
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

