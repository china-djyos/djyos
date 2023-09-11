#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <systime.h>
#include <cpu_peri.h>
#include <device/djy_uart.h>
#include <int.h>
#include <djyos.h>
#include <cpu_peri_uart.h>
#include <cpu_peri_dma.h>
#include <at32f435_437_usart.h>

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

//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip uart"//CPU的uart外设驱动
//parent:"uart device file"      //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                    //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                 //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                   //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                  //初始化时机，可选值：early，medium，later, pre-main。
                                   //表示初始化时间，分别是早期、中期、后期
//dependence:"uart device file"//该组件的依赖组件名（可以是none，表示无依赖组件），
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
//%$#@num,0,512,
#define CFG_UART1_SENDBUF_LEN       32      //"UART1发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN       32      //"UART1接收环形缓冲区大小",
#define CFG_UART1_DMABUF_LEN        32      //"UART1 DMA环形缓冲区大小",

#define CFG_UART2_SENDBUF_LEN       32      //"UART2发送环形缓冲区大小",
#define CFG_UART2_RECVBUF_LEN       32      //"UART2接收环形缓冲区大小",
#define CFG_UART2_DMABUF_LEN        32      //"UART2 DMA环形缓冲区大小",

#define CFG_UART3_SENDBUF_LEN       32      //"UART3发送环形缓冲区大小",
#define CFG_UART3_RECVBUF_LEN       32      //"UART3接收环形缓冲区大小",
#define CFG_UART3_DMABUF_LEN        32      //"UART3 DMA环形缓冲区大小",

#define CFG_UART4_SENDBUF_LEN       32      //"UART4发送环形缓冲区大小",
#define CFG_UART4_RECVBUF_LEN       32      //"UART4接收环形缓冲区大小",
#define CFG_UART4_DMABUF_LEN        32      //"UART4 DMA环形缓冲区大小",

#define CFG_UART5_SENDBUF_LEN       32      //"UART5发送环形缓冲区大小",
#define CFG_UART5_RECVBUF_LEN       32      //"UART5接收环形缓冲区大小",
#define CFG_UART5_DMABUF_LEN        32      //"UART5 DMA环形缓冲区大小",

#define CFG_UART6_SENDBUF_LEN       32      //"UART6发送环形缓冲区大小",
#define CFG_UART6_RECVBUF_LEN       32      //"UART6接收环形缓冲区大小",
#define CFG_UART6_DMABUF_LEN        32      //"UART6 DMA环形缓冲区大小",

#define CFG_UART7_SENDBUF_LEN       32      //"UART7发送环形缓冲区大小",
#define CFG_UART7_RECVBUF_LEN       32      //"UART7接收环形缓冲区大小",
#define CFG_UART7_DMABUF_LEN        32      //"UART7 DMA环形缓冲区大小",

#define CFG_UART8_SENDBUF_LEN       32      //"UART8发送环形缓冲区大小",
#define CFG_UART8_RECVBUF_LEN       32      //"UART8接收环形缓冲区大小",
#define CFG_UART8_DMABUF_LEN        32      //"UART8 DMA环形缓冲区大小",

//%$#@enum,true,false
#define CFG_UART1_ENABLE           true        //"是否使用UART1",
#define CFG_UART2_ENABLE           false       //"是否使用UART2",
#define CFG_UART3_ENABLE           false       //"是否使用UART3",
#define CFG_UART4_ENABLE           false       //"是否使用UART4",
#define CFG_UART5_ENABLE           false       //"是否使用UART5",
#define CFG_UART6_ENABLE           false       //"是否使用UART6",
#define CFG_UART7_ENABLE           false       //"是否使用UART7",
#define CFG_UART8_ENABLE           false       //"是否使用UART8",
//%$#@enum,DMA_NONUSE,DMA1_C1,DMA1_C2,DMA1_C3,DMA1_C4,DMA1_C5,DMA1_C6,DMA1_C7,DMA2_C1,DMA2_C2,DMA2_C3,DMA2_C4
#define CFG_UART1_TX_DMA_CHANNEL       DMA_NONUSE   //串口1发送所使用的DMA通道，不能和其它的外设重复使用,用DMA的话要收发都配置好通道
#define CFG_UART1_RX_DMA_CHANNEL       DMA_NONUSE   //串口1接收所使用的DMA通道，不能和其它的外设重复使用,用DMA的话要收发都配置好通道
#define CFG_UART2_TX_DMA_CHANNEL       DMA_NONUSE   //串口2发送所使用的DMA通道，不能和其它的外设重复使用,用DMA的话要收发都配置好通道
#define CFG_UART2_RX_DMA_CHANNEL       DMA_NONUSE   //串口2接收所使用的DMA通道，不能和其它的外设重复使用,用DMA的话要收发都配置好通道
#define CFG_UART3_TX_DMA_CHANNEL       DMA_NONUSE   //串口3发送所使用的DMA通道，不能和其它的外设重复使用,用DMA的话要收发都配置好通道
#define CFG_UART3_RX_DMA_CHANNEL       DMA_NONUSE   //串口3接收所使用的DMA通道，不能和其它的外设重复使用,用DMA的话要收发都配置好通道
#define CFG_UART4_TX_DMA_CHANNEL       DMA_NONUSE   //串口4发送所使用的DMA通道，不能和其它的外设重复使用,用DMA的话要收发都配置好通道
#define CFG_UART4_RX_DMA_CHANNEL       DMA_NONUSE   //串口4接收所使用的DMA通道，不能和其它的外设重复使用,用DMA的话要收发都配置好通道
#define CFG_UART5_TX_DMA_CHANNEL       DMA_NONUSE   //串口5发送所使用的DMA通道，不能和其它的外设重复使用,用DMA的话要收发都配置好通道
#define CFG_UART5_RX_DMA_CHANNEL       DMA_NONUSE   //串口5接收所使用的DMA通道，不能和其它的外设重复使用,用DMA的话要收发都配置好通道
#define CFG_UART6_TX_DMA_CHANNEL       DMA_NONUSE   //串口6发送所使用的DMA通道，不能和其它的外设重复使用,用DMA的话要收发都配置好通道
#define CFG_UART6_RX_DMA_CHANNEL       DMA_NONUSE   //串口6接收所使用的DMA通道，不能和其它的外设重复使用,用DMA的话要收发都配置好通道
#define CFG_UART7_TX_DMA_CHANNEL       DMA_NONUSE   //串口7发送所使用的DMA通道，不能和其它的外设重复使用,用DMA的话要收发都配置好通道
#define CFG_UART7_RX_DMA_CHANNEL       DMA_NONUSE   //串口7接收所使用的DMA通道，不能和其它的外设重复使用,用DMA的话要收发都配置好通道
#define CFG_UART8_TX_DMA_CHANNEL       DMA_NONUSE   //串口8发送所使用的DMA通道，不能和其它的外设重复使用,用DMA的话要收发都配置好通道
#define CFG_UART8_RX_DMA_CHANNEL       DMA_NONUSE   //串口8接收所使用的DMA通道，不能和其它的外设重复使用,用DMA的话要收发都配置好通道
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束
//@#$%component end configure
// =============================================================================

#define tagUartReg              usart_type

#define CN_UART1_BASE           USART1_BASE
#define CN_UART2_BASE           USART2_BASE
#define CN_UART3_BASE           USART3_BASE
#define CN_UART4_BASE           UART4_BASE
#define CN_UART5_BASE           UART5_BASE
#define CN_UART6_BASE           USART6_BASE
#define CN_UART7_BASE           UART7_BASE
#define CN_UART8_BASE           UART8_BASE

static u8 *pUART_DmaSendBuf[CN_UART_NUM];
static u8 *pUART_DmaRecvBuf[CN_UART_NUM];
static struct UartGeneralCB *pUartCB[CN_UART_NUM];

extern struct UartGeneralCB *UART_InstallGeneral(struct UartParam *Param);

uint32_t UART_DmaRx_ISR(ptu32_t port);
uint32_t UART_DmaTx_ISR(ptu32_t port);

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

static u32 TxByteTime;                    //正常发送一个字节所需要的时间
static tagUartReg *PutStrDirectReg;     //用于printk发送的串口寄存器
static tagUartReg *GetCharDirectReg;     //用于直接接收的串口寄存器
static u32 TxDirectPort;                  //用于printk发送的串口号
static u32 RxDirectPort;                  //用于直接接收的串口号

// =============================================================================
static tagUartReg * const tg_UART_Reg[] = {(tagUartReg *)CN_UART1_BASE,
                                                    (tagUartReg *)CN_UART2_BASE,
                                                    (tagUartReg *)CN_UART3_BASE,
                                                    (tagUartReg *)CN_UART4_BASE,
                                                    (tagUartReg *)CN_UART5_BASE,
                                                    (tagUartReg *)CN_UART6_BASE,
                                                    (tagUartReg *)CN_UART7_BASE,
                                                    (tagUartReg *)CN_UART8_BASE,};

// UART对应的DMA Stream中断号
// static u8 const UartDmaRxInt[] = {DMA1_Channel1_IRQn,DMA1_Channel2_IRQn,
//                                     DMA1_Channel3_IRQn,DMA1_Channel4_IRQn,
//                                     DMA1_Channel5_IRQn,DMA1_Channel6_IRQn,
//                                     DMA1_Channel7_IRQn,DMA1_Channel1_IRQn};

// static u8 const UartDmaTxInt[] = {DMA2_Channel1_IRQn,DMA2_Channel2_IRQn,
//                                     DMA2_Channel3_IRQn,DMA2_Channel4_IRQn,
//                                     DMA2_Channel5_IRQn,DMA2_Channel6_IRQn,
//                                     DMA2_Channel7_IRQn,DMA2_Channel1_IRQn};

// static dma_channel_type * const UartDmaRxChannel[] = {DMA1_CHANNEL1,DMA1_CHANNEL2,
//                                                         DMA1_CHANNEL3,DMA1_CHANNEL4,
//                                                         DMA1_CHANNEL5,DMA1_CHANNEL6,
//                                                         DMA1_CHANNEL7,DMA1_CHANNEL1,};
// static dma_channel_type * const UartDmaTxChannel[] = {DMA2_CHANNEL1,DMA2_CHANNEL2,
//                                                         DMA2_CHANNEL3,DMA2_CHANNEL4,
//                                                         DMA2_CHANNEL5,DMA2_CHANNEL6,
//                                                         DMA2_CHANNEL7,DMA2_CHANNEL1,};

static dma_channel_type * const UartAllDmaChannel[] = {DMA1_CHANNEL1, DMA1_CHANNEL2, DMA1_CHANNEL3,
                                                    DMA1_CHANNEL4, DMA1_CHANNEL5, DMA1_CHANNEL6,
                                                    DMA1_CHANNEL7, DMA2_CHANNEL1, DMA2_CHANNEL2,
                                                    DMA2_CHANNEL3, DMA2_CHANNEL4
                                                    // ,DMA2_CHANNEL5, DMA2_CHANNEL6, DMA2_CHANNEL7
                                                    };
static u8 const UartAllDmaChannelInt[] = {DMA1_Channel1_IRQn, DMA1_Channel2_IRQn, DMA1_Channel3_IRQn,
                                                        DMA1_Channel4_IRQn, DMA1_Channel5_IRQn, DMA1_Channel6_IRQn,
                                                        DMA1_Channel7_IRQn, DMA2_Channel1_IRQn, DMA2_Channel2_IRQn,
                                                        DMA2_Channel3_IRQn, DMA2_Channel4_IRQn
                                                        // ,DMA2_Channel5_IRQn, DMA2_Channel6_IRQn, DMA2_Channel7_IRQn
                                                        };
// 一维下标对应串口编号，二维下标，0表示TX的DMA通道，1表示RX的DMA通道
#define UART_TX_DMA_CH          0
#define UART_RX_DMA_CH          1
static enum UART_DMA_CHANNEL UartTX_RX_DmaChannel[8][2];

static u8 const DMA_Tx_ch[] = {DMAMUX_DMAREQ_ID_USART1_TX,DMAMUX_DMAREQ_ID_USART2_TX,DMAMUX_DMAREQ_ID_USART3_TX,
                                DMAMUX_DMAREQ_ID_UART4_TX,DMAMUX_DMAREQ_ID_UART5_TX,DMAMUX_DMAREQ_ID_USART6_TX,
                                DMAMUX_DMAREQ_ID_UART7_TX, DMAMUX_DMAREQ_ID_UART8_TX};
static u8 const DMA_Rx_ch[] = {DMAMUX_DMAREQ_ID_USART1_RX,DMAMUX_DMAREQ_ID_USART2_RX,DMAMUX_DMAREQ_ID_USART3_RX,
                                DMAMUX_DMAREQ_ID_UART4_RX,DMAMUX_DMAREQ_ID_UART5_RX,DMAMUX_DMAREQ_ID_USART6_RX,
                                DMAMUX_DMAREQ_ID_UART7_RX, DMAMUX_DMAREQ_ID_UART8_RX};

#define CN_DMA_UNUSED   0
#define CN_DMA_USED     1

static bool_t s_UART_DmaSending[] = {false,false,false,false,false,false,false,false};
static bool_t s_UART_DmaUsed[] = {false,false,false,false,false,false,false,false};



// =============================================================================
// 功能: 禁止uart的发送中断。
// 参数: flag,是否使用dma传输
//       port,串口号，1~5
// 返回: 无
// =============================================================================
void __UART_RxIntEnable(u8 flag, u8 port)
{
    if(flag == CN_DMA_USED)
    {
        Int_RestoreAsynLine(UartAllDmaChannelInt[UartTX_RX_DmaChannel[port][UART_RX_DMA_CH]]);
    }
    else
    {
        // tg_UART_Reg[port]->CR1 |= (1<<5);
        usart_interrupt_enable(tg_UART_Reg[port], USART_RDBF_INT, TRUE);
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
    if(flag == CN_DMA_USED)
    {
        Int_SaveAsynLine(UartAllDmaChannelInt[UartTX_RX_DmaChannel[port][UART_RX_DMA_CH]]);
    }
    else
    {
        // tg_UART_Reg[port]->CR1 &= ~(1<<5);
        usart_interrupt_enable(tg_UART_Reg[port], USART_RDBF_INT, FALSE);
    }
}

// =============================================================================
// 功能: 使能uart的发送中断
// 参数: reg,被操作的寄存器组指针
//       port,串口号
// 返回: 无
// =============================================================================
static void __UART_TxIntEnable(u8 flag, u8 port)
{
    if(flag == CN_DMA_USED)
    {
        Int_RestoreAsynLine(UartAllDmaChannelInt[UartTX_RX_DmaChannel[port][UART_TX_DMA_CH]]);
    }
    else
    {
        // tg_UART_Reg[port]->CR1 |= (1<<7);
        usart_interrupt_enable(tg_UART_Reg[port], USART_TDBE_INT, TRUE);
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
    if(flag == CN_DMA_USED)
    {
        Int_SaveAsynLine(UartAllDmaChannelInt[UartTX_RX_DmaChannel[port][UART_TX_DMA_CH]]);
    }
    else
    {
        // tg_UART_Reg[port]->CR1 &= ~(1<<7);//禁止发送空中断
        usart_interrupt_enable(tg_UART_Reg[port], USART_TDBE_INT, FALSE);
    }
}

// =============================================================================
// 功能: 检查发送寄存器的状态，如果空就返回true，若为空，否则返回false
// 参数：reg,被操作的寄存器组指针
// 返回: true = 缓冲区空，false = 非空
// =============================================================================
static bool_t __UART_TxTranEmpty(tagUartReg *reg)
{
    return (RESET != usart_flag_get(reg, USART_TDBE_FLAG));
    // return (0 != (reg->SR & (1<<7)));
}

// =============================================================================
// 功能: 检查接收寄存器的状态，有数据就返回true，否则返回false
// 参数：reg,被操作的寄存器组指针
// 返回: true = 有数据，false = 无数据
// =============================================================================
static bool_t __UART_RxHadChar(tagUartReg *reg)
{
    return (RESET != usart_flag_get(reg, USART_RDBF_FLAG));
    // return ((reg->SR & (1<<5)) !=0);
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
    // u32 mantissa,fraction;
    // float temp;
    // if((port == CN_UART1) || (port == CN_UART6))
    // {
    //     temp = (float)CN_CFG_PCLK2/(16*baud);
    //     mantissa = temp;
    //     fraction = (temp - mantissa)*16;
    // }
    // else
    // {
    //     temp = (float)CN_CFG_PCLK1/(16*baud);
    //     mantissa = temp;
    //     fraction = (temp - mantissa)*16;
    // }

    // Reg->BRR =  ((u16)mantissa<<4)+(u16)fraction;

    crm_clocks_freq_type clocks_freq;
    uint32_t apb_clock = 0;
    uint32_t temp_val = 0;
    crm_clocks_freq_get(&clocks_freq);
    if((port == CN_UART1) || (port == CN_UART6))
    {
        apb_clock = clocks_freq.apb2_freq;
    }
    else
    {
        apb_clock = clocks_freq.apb1_freq;
    }
    temp_val = (apb_clock * 10 / baud);
    if((temp_val % 10) < 5)
    {
        temp_val = (temp_val / 10);
    }
    else
    {
        temp_val = (temp_val / 10) + 1;
    }
    Reg->baudr_bit.div = temp_val;

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
static void __UART_ComConfig(tagUartReg *Reg,u32 port,struct COMParam *COM)
{
    if((COM == NULL) || (Reg == NULL))
    {
        return;
    }
    __UART_BaudSet(Reg, port, COM->BaudRate);

    switch(COM->DataBits)               // data bits
    {
        case CN_UART_DATABITS_7:    Reg->ctrl1_bit.dbn_h = 1;Reg->ctrl1_bit.dbn_l = 0;   break;    
        case CN_UART_DATABITS_8:    Reg->ctrl1_bit.dbn_h = 0;Reg->ctrl1_bit.dbn_l = 0;   break;  
        case CN_UART_DATABITS_9:    Reg->ctrl1_bit.dbn_h = 0;Reg->ctrl1_bit.dbn_l = 1;   break;  
        default:break;
    }

    switch(COM->Parity)                 // parity
    {
        // Reg->CR1 |= (1<<10);                // enable parity
        // case CN_UART_PARITY_ODD:    Reg->CR1 |= (1<<9);break;
        // case CN_UART_PARITY_EVEN:   Reg->CR1 &=~(1<<9);break;
        case CN_UART_PARITY_NONE:    usart_parity_selection_config(Reg, USART_PARITY_NONE);break;
        case CN_UART_PARITY_ODD:    usart_parity_selection_config(Reg, USART_PARITY_ODD);break;
        case CN_UART_PARITY_EVEN:   usart_parity_selection_config(Reg, USART_PARITY_EVEN);break;
        default:break;
    }

    switch(COM->StopBits)               // stop bits
    {
        // Reg->CR2 &= ~(3<<12);
        // case CN_UART_STOPBITS_1:    Reg->CR2 |= (0<<12);break;
        // case CN_UART_STOPBITS_1_5:  Reg->CR2 |= (1<<12);break;
        // case CN_UART_STOPBITS_2:    Reg->CR2 |= (2<<12);break;
        case CN_UART_STOPBITS_1:    Reg->ctrl2_bit.stopbn = USART_STOP_1_BIT;break;
        case CN_UART_STOPBITS_1_5:  Reg->ctrl2_bit.stopbn = USART_STOP_1_5_BIT;break;
        case CN_UART_STOPBITS_2:    Reg->ctrl2_bit.stopbn = USART_STOP_2_BIT;break;
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
        case CN_UART1:      crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);        break;
        case CN_UART2:      crm_periph_clock_enable(CRM_USART2_PERIPH_CLOCK, TRUE);        break;
        case CN_UART3:      crm_periph_clock_enable(CRM_USART3_PERIPH_CLOCK, TRUE);        break;
        case CN_UART4:      crm_periph_clock_enable(CRM_UART4_PERIPH_CLOCK, TRUE);         break;
        case CN_UART5:      crm_periph_clock_enable(CRM_UART5_PERIPH_CLOCK, TRUE);         break;
        case CN_UART6:      crm_periph_clock_enable(CRM_USART6_PERIPH_CLOCK, TRUE);        break;
        case CN_UART7:      crm_periph_clock_enable(CRM_UART7_PERIPH_CLOCK, TRUE);         break;
        case CN_UART8:      crm_periph_clock_enable(CRM_UART8_PERIPH_CLOCK, TRUE);         break;
        default:
            break;
    }
    usart_enable(tg_UART_Reg[port], TRUE);
    // tg_UART_Reg[port]->CR1 |= (1<<13);
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
        case CN_UART1:      crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, FALSE);        break;
        case CN_UART2:      crm_periph_clock_enable(CRM_USART2_PERIPH_CLOCK, FALSE);        break;
        case CN_UART3:      crm_periph_clock_enable(CRM_USART3_PERIPH_CLOCK, FALSE);        break;
        case CN_UART4:      crm_periph_clock_enable(CRM_UART4_PERIPH_CLOCK, FALSE);         break;
        case CN_UART5:      crm_periph_clock_enable(CRM_UART5_PERIPH_CLOCK, FALSE);         break;
        case CN_UART6:      crm_periph_clock_enable(CRM_USART6_PERIPH_CLOCK, FALSE);        break;
        case CN_UART7:      crm_periph_clock_enable(CRM_UART7_PERIPH_CLOCK, FALSE);         break;
        case CN_UART8:      crm_periph_clock_enable(CRM_UART8_PERIPH_CLOCK, FALSE);         break;
        default:
            break;
    }
    // tg_UART_Reg[port]->CR1 &= ~(1<<13);
    usart_enable(tg_UART_Reg[port], FALSE);
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

    if ((Reg->ctrl1_bit.rdbfien != RESET) && (usart_flag_get(Reg, USART_RDBF_FLAG) != RESET))   //接收中断
    // if( (Reg->SR & (1<<5)) &&(Reg->CR1 &(1<<5)))           //接收中断
    {
        ch = (u8)usart_data_receive(Reg);
        num = UART_PortWrite(UCB,&ch,1);
        if(num != 1)
        {
            UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
        }
    }

    if ((Reg->ctrl1_bit.tdcien != RESET) && (usart_flag_get(Reg, USART_TDC_FLAG) != RESET))   //发送完成
    // if((Reg->SR & (1<<6))&&(Reg->CR1 & (1<<6)) )
    {
        // Reg->CR1 &= ~(1<<6);
        usart_interrupt_enable(Reg, USART_TDC_INT, FALSE);
        Board_UartHalfDuplexRecv(port);
    }


    if ((Reg->ctrl1_bit.tdbeien != RESET) && (usart_flag_get(Reg, USART_TDBE_FLAG) != RESET))   //发送中断
    // if((Reg->SR & (1<<7))&&(Reg->CR1 & (1<<7)))    //发送中断
    {
        num = UART_PortRead(UCB,&ch,1);
        if(num != 0)
        {
            usart_data_transmit(Reg, (u16)ch);
        }
        else
        {
            // Reg->CR1 &= ~(1<<7);        //txeie
            usart_interrupt_enable(Reg, USART_TDBE_INT, FALSE);
            // Reg->CR1 |= (1<<6);
            usart_interrupt_enable(Reg, USART_TDC_INT, TRUE);
        }
    }

    // if(Reg->SR & (1<<3))        //ORE过载错误
    if (usart_flag_get(Reg, USART_NERR_FLAG) != RESET)        //ORE过载错误
    {
        // ch = (u8)Reg->DR;
        ch = (u8)usart_data_receive(Reg);
        // num = Reg->SR;     //读一下sr寄存器
        usart_flag_get(Reg, USART_NERR_FLAG);
        UART_ErrHandle(UCB,CN_UART_FIFO_OVER_ERR);
    }

    // if((Reg->SR & (1<<4)) && (Reg->CR1 & (1<<4)))
    if ((Reg->ctrl1_bit.idleien != RESET) && (usart_flag_get(Reg, USART_IDLEF_FLAG) != RESET))   //总线空闲
    {
        // Reg->DR;
        ch = (u8)usart_data_receive(Reg);
        Int_TapLine(UartAllDmaChannelInt[UartTX_RX_DmaChannel[port][UART_RX_DMA_CH]]);
    }
    return 0;
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
            IntLine = CN_INT_LINE_USART1;
            break;
        case CN_UART2:
            IntLine = CN_INT_LINE_USART2;
            break;
        case CN_UART3:
            IntLine = CN_INT_LINE_USART3;
            break;
        case CN_UART4:
            IntLine = CN_INT_LINE_UART4;
            break;
        case CN_UART5:
            IntLine = CN_INT_LINE_UART5;
            break;
        case CN_UART6:
            IntLine = CN_INT_LINE_USART6;
            break;
        case CN_UART7:
            IntLine = CN_INT_LINE_UART7;
            break;
        case CN_UART8:
            IntLine = CN_INT_LINE_UART8;
            break;
        default:
            return ;
            break;
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
//       波特率:115200  ； 停止位:1 ; 校验:偶校验 ; 数据位:8bit
// 参数: SerialNo,串口号，0~3
// 返回: 无
// =============================================================================
static void __UART_HardInit(u8 SerialNo)
{
    if(SerialNo > CN_UART8)
        return;

    // 这里不开时钟的话，串口初始化可能失败
    switch(SerialNo)
    {
        case CN_UART1:      crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);        break;
        case CN_UART2:      crm_periph_clock_enable(CRM_USART2_PERIPH_CLOCK, TRUE);        break;
        case CN_UART3:      crm_periph_clock_enable(CRM_USART3_PERIPH_CLOCK, TRUE);        break;
        case CN_UART4:      crm_periph_clock_enable(CRM_UART4_PERIPH_CLOCK, TRUE);         break;
        case CN_UART5:      crm_periph_clock_enable(CRM_UART5_PERIPH_CLOCK, TRUE);         break;
        case CN_UART6:      crm_periph_clock_enable(CRM_USART6_PERIPH_CLOCK, TRUE);        break;
        case CN_UART7:      crm_periph_clock_enable(CRM_UART7_PERIPH_CLOCK, TRUE);         break;
        case CN_UART8:      crm_periph_clock_enable(CRM_UART8_PERIPH_CLOCK, TRUE);         break;
        default:
            break;
    }

    usart_init(tg_UART_Reg[SerialNo], 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
    usart_parity_selection_config(tg_UART_Reg[SerialNo], USART_PARITY_NONE);

    usart_transmitter_enable(tg_UART_Reg[SerialNo], TRUE);
    usart_receiver_enable(tg_UART_Reg[SerialNo], TRUE);
    usart_interrupt_enable(tg_UART_Reg[SerialNo], USART_RDBF_INT, TRUE);
    usart_interrupt_enable(tg_UART_Reg[SerialNo], USART_TDBE_INT, TRUE);
    __UART_Enable(SerialNo);
}

// =============================================================================
// 功能: 启动串口DMA发送，使用DMA启动，在发送前需判断上次DMA是否已经发送完成，通过
//       查看标志UARTx_DMA_SENDING来判断UARTx是否已经发送完成
// 参数: PORT,串口号.
// 返回: 发送的个数
// =============================================================================
u32 __UART_DMA_SendStart(u32 port)
{
    u32 num = 0;
    u32 addr = 0;
    u8 *DmaSendBuf = NULL;
    u32 DmaSendBufLen = 0;

    switch(port)
    {
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
        case CN_UART6:
            DmaSendBufLen = CFG_UART6_DMABUF_LEN;        break;
        case CN_UART7:
            DmaSendBufLen = CFG_UART7_DMABUF_LEN;        break;
        case CN_UART8:
            DmaSendBufLen = CFG_UART8_DMABUF_LEN;        break;
        default:
            break;
    }

    DmaSendBuf = pUART_DmaSendBuf[port];
//    if(true == __uart_dma_timeout(s_UART_DmaSending[port]))
    if(true == s_UART_DmaSending[port])
        return 0;
    num = UART_PortRead(pUartCB[port],DmaSendBuf,DmaSendBufLen);
    addr = (u32)DmaSendBuf;

    if(num > 0)
    {
        DMA_Enable(UartAllDmaChannel[UartTX_RX_DmaChannel[port][UART_TX_DMA_CH]], addr, num);
        s_UART_DmaSending[port] = true;
    }
    else
    {
        // tg_UART_Reg[port]->CR1 |= (1<<6);
        usart_interrupt_enable(tg_UART_Reg[port], USART_TDC_INT, TRUE);
    }
    return 0;
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
            DmaBufLen = CFG_UART1_DMABUF_LEN;        break;
        case CN_UART2:
            DmaBufLen = CFG_UART2_DMABUF_LEN;        break;
        case CN_UART3:
            DmaBufLen = CFG_UART3_DMABUF_LEN;        break;
        case CN_UART4:
            DmaBufLen = CFG_UART4_DMABUF_LEN;        break;
        case CN_UART5:
            DmaBufLen = CFG_UART5_DMABUF_LEN;        break;
        case CN_UART6:
            DmaBufLen = CFG_UART6_DMABUF_LEN;        break;
        case CN_UART7:
            DmaBufLen = CFG_UART7_DMABUF_LEN;        break;
        case CN_UART8:
            DmaBufLen = CFG_UART8_DMABUF_LEN;        break;
        default :        break;
    }

    if (pUART_DmaRecvBuf[port] == NULL)
    {
        pUART_DmaRecvBuf[port] = (u8*)M_Malloc(DmaBufLen,0);
        if(pUART_DmaRecvBuf[port] == NULL)
            return;
    }

    if(pUART_DmaSendBuf[port] == NULL)
    {
        pUART_DmaSendBuf[port] = (u8*)M_Malloc(DmaBufLen,0);
        if(pUART_DmaSendBuf[port] == NULL)
        {
            free(pUART_DmaRecvBuf[port]);
            pUART_DmaRecvBuf[port]=NULL;
            return;
        }
    }

    DmaRecvBuf = pUART_DmaRecvBuf[port];
    DmaSendBuf = pUART_DmaSendBuf[port];
    __UART_RxIntDisable(CN_DMA_UNUSED,port);
    __UART_TxIntDisable(CN_DMA_UNUSED,port);

    // tg_UART_Reg[port]->CR1 |= (1<<4);//enable idle int
    usart_interrupt_enable(tg_UART_Reg[port], USART_IDLE_INT, TRUE);

    DMA_Config(UartAllDmaChannel[UartTX_RX_DmaChannel[port][UART_RX_DMA_CH]],DMA_Rx_ch[port],(u32)&(tg_UART_Reg[port]->dt),
            (u32)DmaRecvBuf, DMA_DIR_P2M, DMA_MEM_DATABITS_8, DMA_PER_DATABITS_8, DmaBufLen);

    DMA_Config(UartAllDmaChannel[UartTX_RX_DmaChannel[port][UART_TX_DMA_CH]],DMA_Tx_ch[port],(u32)&(tg_UART_Reg[port]->dt),
            (u32)DmaSendBuf, DMA_DIR_M2P, DMA_MEM_DATABITS_8, DMA_PER_DATABITS_8, DmaBufLen);

    DMA_IntEnable(UartAllDmaChannel[UartTX_RX_DmaChannel[port][UART_TX_DMA_CH]],DMA_INT_TCIE);  //使能发送完成中断
    DMA_IntEnable(UartAllDmaChannel[UartTX_RX_DmaChannel[port][UART_RX_DMA_CH]],DMA_INT_HTIE);  //使能接收半完成中断

    // tg_UART_Reg[port]->CR3 |= 0x00c0;  //设置串口使用dma收发
    //设置串口使用dma收发
    usart_dma_transmitter_enable(tg_UART_Reg[port], TRUE);
    usart_dma_receiver_enable(tg_UART_Reg[port], TRUE);

    s_UART_DmaUsed[port] = CN_DMA_USED;
    DMA_Enable(UartAllDmaChannel[UartTX_RX_DmaChannel[port][UART_RX_DMA_CH]], (u32)DmaRecvBuf, DmaBufLen);//启动dma通道

    Int_Register(UartAllDmaChannelInt[UartTX_RX_DmaChannel[port][UART_TX_DMA_CH]]);
    Int_Register(UartAllDmaChannelInt[UartTX_RX_DmaChannel[port][UART_RX_DMA_CH]]);//注册接收DMA中断
    Int_SetClearType(UartAllDmaChannelInt[UartTX_RX_DmaChannel[port][UART_RX_DMA_CH]],CN_INT_CLEAR_AUTO);
    Int_IsrConnect(UartAllDmaChannelInt[UartTX_RX_DmaChannel[port][UART_RX_DMA_CH]],UART_DmaRx_ISR);
    Int_SetClearType(UartAllDmaChannelInt[UartTX_RX_DmaChannel[port][UART_TX_DMA_CH]],CN_INT_CLEAR_AUTO);
    Int_IsrConnect(UartAllDmaChannelInt[UartTX_RX_DmaChannel[port][UART_TX_DMA_CH]],UART_DmaTx_ISR);

    Int_SetIsrPara(UartAllDmaChannelInt[UartTX_RX_DmaChannel[port][UART_TX_DMA_CH]],port);
    Int_SetIsrPara(UartAllDmaChannelInt[UartTX_RX_DmaChannel[port][UART_RX_DMA_CH]],port);

    Int_SettoAsynSignal(UartAllDmaChannelInt[UartTX_RX_DmaChannel[port][UART_RX_DMA_CH]]);
    Int_SettoAsynSignal(UartAllDmaChannelInt[UartTX_RX_DmaChannel[port][UART_TX_DMA_CH]]);
    Int_RestoreAsynLine(UartAllDmaChannelInt[UartTX_RX_DmaChannel[port][UART_RX_DMA_CH]]);
    Int_RestoreAsynLine(UartAllDmaChannelInt[UartTX_RX_DmaChannel[port][UART_TX_DMA_CH]]);
}

//=============================================================================
// 功能: 设置不使用DMA，UART不使用DMA收发，每收发一个字节就必须中断一次，不建议在高速
//      连续通信中使用。
// 参数: port,端口号，即串口号，为1~3
// 返回: 无
// =============================================================================
void __UART_SetDmaUnUsed(u32 port)
{
    u8 IntLine = 0;

    if(port == CN_UART1)             IntLine = CN_INT_LINE_USART1;
    else if(port == CN_UART2)        IntLine = CN_INT_LINE_USART2;
    else if(port == CN_UART3)        IntLine = CN_INT_LINE_USART3;
    else if(port == CN_UART4)        IntLine = CN_INT_LINE_UART4;
    else if(port == CN_UART5)        IntLine = CN_INT_LINE_UART5;
    else if(port == CN_UART6)        IntLine = CN_INT_LINE_USART6;
    else if(port == CN_UART7)        IntLine = CN_INT_LINE_UART7;
    else if(port == CN_UART8)        IntLine = CN_INT_LINE_UART8;
    else
        return;

    free(pUART_DmaRecvBuf[port]);
    pUART_DmaRecvBuf[port]=NULL;
    free(pUART_DmaSendBuf[port]);
    pUART_DmaSendBuf[port]=NULL;

    // tg_UART_Reg[port]->CR3 &= ~0x00c0;     //设置不使用dma传输
    //设置不使用dma传输
    usart_dma_transmitter_enable(tg_UART_Reg[port], TRUE);
    usart_dma_receiver_enable(tg_UART_Reg[port], TRUE);

    Int_RestoreAsynLine(IntLine);

    if(s_UART_DmaUsed[port] == CN_DMA_USED)
    {
        Int_SaveAsynLine(UartAllDmaChannelInt[UartTX_RX_DmaChannel[port][UART_RX_DMA_CH]]);
        Int_SaveAsynLine(UartAllDmaChannelInt[UartTX_RX_DmaChannel[port][UART_TX_DMA_CH]]);
        s_UART_DmaUsed[port] = CN_DMA_UNUSED;
    }
    __UART_RxIntEnable(CN_DMA_UNUSED,port);
    __UART_TxIntEnable(CN_DMA_UNUSED,port);
    // tg_UART_Reg[port]->CR1 &= ~(1<<4);//disable idle int
    usart_interrupt_enable(tg_UART_Reg[port], USART_IDLE_INT, FALSE);
}

// =============================================================================
// 功能: 启动串口发送，其目的是触发中断，用中断方式发送数据。
// 参数: Reg,被操作的串口寄存器指针.
// 返回: 发送的个数
// =============================================================================
static u32 __UART_SendStart (ptu32_t Reg)
{
    u8 port;
//    u32 delay=0;
    switch(Reg)
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

    // Reg->CR1 &= ~(1<<6);
    usart_interrupt_enable((tagUartReg *)Reg, USART_TDC_INT, FALSE);

    Board_UartHalfDuplexSend(port);//切换到发送
    atom_high_t ret;
    ret = Int_HighAtomStart();

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
    Int_HighAtomEnd(ret);
    return 1;
}

// =============================================================================
// 功能: 串口设备的控制函数,与具体的硬件寄存器设置相关
// 参数: Reg,被操作的串口寄存器指针.
//       cmd,操作类型
//       data1,data2,含义依cmd而定
// 返回: 无意义.
// =============================================================================
static ptu32_t __UART_Ctrl(ptu32_t PrivateTag,u32 cmd, va_list *arg0)
{
    ptu32_t result = 0;
    u32 port;
    tagUartReg *Reg = (tagUartReg *)PrivateTag;
    if(Reg == NULL)
        return 0;

    switch((u32)Reg)
    {
        case CN_UART1_BASE:         port = CN_UART1;    break;
        case CN_UART2_BASE:         port = CN_UART2;    break;
        case CN_UART3_BASE:         port = CN_UART3;    break;
        case CN_UART4_BASE:         port = CN_UART4;    break;
        case CN_UART5_BASE:         port = CN_UART5;    break;
        case CN_UART6_BASE:         port = CN_UART6;    break;
        case CN_UART7_BASE:         port = CN_UART7;    break;
        case CN_UART8_BASE:         port = CN_UART8;    break;
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
        case CN_UART_SET_BAUD:   //设置Baud
        {
            u32 data;
            data = va_arg(*arg0, u32);
            __UART_BaudSet(Reg,port, data);
        }
            break;
        case CN_UART_EN_RTS:
//            Reg->CR3 |= 0x100;
            Reg->ctrl3_bit.rtsen = TRUE;
            break;
        case CN_UART_DIS_RTS:
//            Reg->CR3 &= ~0x100;
            Reg->ctrl3_bit.rtsen = FALSE;
            break;
        case CN_UART_EN_CTS:
//            Reg->CR3 |= 0x200;
            Reg->ctrl3_bit.ctsen = TRUE;
            break;
        case CN_UART_DIS_CTS:
//            Reg->CR3 &= ~0x200;
            Reg->ctrl3_bit.ctsen = FALSE;
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
// 功能: 当uart2配置成dma方式是，dma1 ch7负责发送，ch6负责接收
// 参数: 中断函数没有参数.
// 返回: 中断函数没有返回值.
// =============================================================================
uint32_t UART_DmaTx_ISR(ptu32_t port)
{
    uint16_t num = 0;
    u8 *DmaSendBuf = NULL;
    u32 DmaSendBufLen = 0;

    switch(port)
    {
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
        case CN_UART6:
            DmaSendBufLen = CFG_UART6_DMABUF_LEN;        break;
        case CN_UART7:
            DmaSendBufLen = CFG_UART7_DMABUF_LEN;        break;
        case CN_UART8:
            DmaSendBufLen = CFG_UART8_DMABUF_LEN;        break;
        default:        break;
    }

    DmaSendBuf = pUART_DmaSendBuf[port];
    DMA_ClearIntFlag(UartAllDmaChannel[UartTX_RX_DmaChannel[port][UART_TX_DMA_CH]]);
    num = UART_PortRead(pUartCB[port],DmaSendBuf,DmaSendBufLen);
    if(num == 0)
    {
        s_UART_DmaSending[port] = false;
        // tg_UART_Reg[port]->CR1 |= (1<<6);
        usart_interrupt_enable(tg_UART_Reg[port], USART_TDC_INT, TRUE);
        // if(dma_flag_get(DMA1_FDT4_FLAG))
        // {
        //     dma_flag_clear(DMA1_FDT4_FLAG);
        // }
    }
    else
    {
        DMA_Enable(UartAllDmaChannel[UartTX_RX_DmaChannel[port][UART_TX_DMA_CH]], (u32)DmaSendBuf, num);
    }
    return 0;
}

// =============================================================================
// 功能: 当uart2配置成dma方式是，dma1 ch7负责发送，ch6负责接收
// 参数: 中断函数没有参数.
// 返回: 中断函数没有返回值.
// =============================================================================
uint32_t UART_DmaRx_ISR(ptu32_t port)
{
    u32 recvs = 0;
    u32 num = 0;
    u32 DmaRecvBufLen = 0;
    u8 *DmaRecvBuf = NULL;

    switch(port)
    {
        case CN_UART1:
            DmaRecvBufLen = CFG_UART1_DMABUF_LEN;        break;
        case CN_UART2:
            DmaRecvBufLen = CFG_UART2_DMABUF_LEN;        break;
        case CN_UART3:
            DmaRecvBufLen = CFG_UART3_DMABUF_LEN;        break;
        case CN_UART4:
            DmaRecvBufLen = CFG_UART4_DMABUF_LEN;        break;
        case CN_UART5:
            DmaRecvBufLen = CFG_UART5_DMABUF_LEN;        break;
        case CN_UART6:
            DmaRecvBufLen = CFG_UART6_DMABUF_LEN;        break;
        case CN_UART7:
            DmaRecvBufLen = CFG_UART7_DMABUF_LEN;        break;
        case CN_UART8:
            DmaRecvBufLen = CFG_UART8_DMABUF_LEN;        break;
        default:
            break;
    }

    DmaRecvBuf = pUART_DmaRecvBuf[port];
    DMA_Disable(UartAllDmaChannel[UartTX_RX_DmaChannel[port][UART_RX_DMA_CH]]);
    DMA_ClearIntFlag(UartAllDmaChannel[UartTX_RX_DmaChannel[port][UART_RX_DMA_CH]]);
    recvs = DmaRecvBufLen - UartAllDmaChannel[UartTX_RX_DmaChannel[port][UART_RX_DMA_CH]]->dtcnt_bit.cnt;
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

    DMA_Enable(UartAllDmaChannel[UartTX_RX_DmaChannel[port][UART_RX_DMA_CH]], (u32)DmaRecvBuf, DmaRecvBufLen);
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
    enum UART_DMA_CHANNEL TX_CH = DMA_NONUSE;
    enum UART_DMA_CHANNEL RX_CH = DMA_NONUSE;
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

            TX_CH = CFG_UART1_TX_DMA_CHANNEL;
            RX_CH = CFG_UART1_RX_DMA_CHANNEL;
            dmause                  = (TX_CH == DMA_NONUSE ? FALSE : TRUE) 
                                        && (RX_CH == DMA_NONUSE ? FALSE : TRUE);
            break;
        case CN_UART2://串口2
            UART_Param.Name         = "UART2";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CN_UART2_BASE;
            UART_Param.TxRingBufLen = CFG_UART2_SENDBUF_LEN;
            UART_Param.RxRingBufLen = CFG_UART2_RECVBUF_LEN;
            UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
            UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
            TX_CH = CFG_UART2_TX_DMA_CHANNEL;
            RX_CH = CFG_UART2_RX_DMA_CHANNEL;
            dmause                  = (TX_CH == DMA_NONUSE ? FALSE : TRUE)
                                        && (RX_CH == DMA_NONUSE ? FALSE : TRUE);
            break;

        case CN_UART3://串口3
            UART_Param.Name         = "UART3";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CN_UART3_BASE;
            UART_Param.TxRingBufLen = CFG_UART3_SENDBUF_LEN;
            UART_Param.RxRingBufLen = CFG_UART3_RECVBUF_LEN;
            UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
            UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
            TX_CH = CFG_UART3_TX_DMA_CHANNEL;
            RX_CH = CFG_UART3_RX_DMA_CHANNEL;
            dmause                  = (TX_CH == DMA_NONUSE ? FALSE : TRUE)
                                        && (RX_CH == DMA_NONUSE ? FALSE : TRUE);
            break;

        case CN_UART4://串口4
            UART_Param.Name         = "UART4";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CN_UART4_BASE;
            UART_Param.TxRingBufLen = CFG_UART4_SENDBUF_LEN;
            UART_Param.RxRingBufLen = CFG_UART4_RECVBUF_LEN;
            UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
            UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
            TX_CH = CFG_UART4_TX_DMA_CHANNEL;
            RX_CH = CFG_UART4_RX_DMA_CHANNEL;
            dmause                  = (TX_CH == DMA_NONUSE ? FALSE : TRUE)
                                        && (RX_CH == DMA_NONUSE ? FALSE : TRUE);
            break;
        case CN_UART5://串口5
            UART_Param.Name         = "UART5";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CN_UART5_BASE;
            UART_Param.TxRingBufLen = CFG_UART5_SENDBUF_LEN;
            UART_Param.RxRingBufLen = CFG_UART5_RECVBUF_LEN;
            UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
            UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
            TX_CH = CFG_UART5_TX_DMA_CHANNEL;
            RX_CH = CFG_UART6_RX_DMA_CHANNEL;
            dmause                  = (TX_CH == DMA_NONUSE ? FALSE : TRUE)
                                        && (RX_CH == DMA_NONUSE ? FALSE : TRUE);
            break;
        case CN_UART6://串口6
            UART_Param.Name         = "UART6";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CN_UART6_BASE;
            UART_Param.TxRingBufLen = CFG_UART6_SENDBUF_LEN;
            UART_Param.RxRingBufLen = CFG_UART6_RECVBUF_LEN;
            UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
            UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
            TX_CH = CFG_UART6_TX_DMA_CHANNEL;
            RX_CH = CFG_UART6_RX_DMA_CHANNEL;
            dmause                  = (TX_CH == DMA_NONUSE ? FALSE : TRUE)
                                        && (RX_CH == DMA_NONUSE ? FALSE : TRUE);
            break;
        case CN_UART7://串口7
            UART_Param.Name         = "UART7";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CN_UART7_BASE;
            UART_Param.TxRingBufLen = CFG_UART7_SENDBUF_LEN;
            UART_Param.RxRingBufLen = CFG_UART7_RECVBUF_LEN;
            UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
            UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
            TX_CH = CFG_UART7_TX_DMA_CHANNEL;
            RX_CH = CFG_UART7_RX_DMA_CHANNEL;
            dmause                  = (TX_CH == DMA_NONUSE ? FALSE : TRUE)
                                        && (RX_CH == DMA_NONUSE ? FALSE : TRUE);
            break;
        case CN_UART8://串口8
            UART_Param.Name         = "UART8";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CN_UART8_BASE;
            UART_Param.TxRingBufLen = CFG_UART8_SENDBUF_LEN;
            UART_Param.RxRingBufLen = CFG_UART8_RECVBUF_LEN;
            UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
            UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
            TX_CH = CFG_UART8_TX_DMA_CHANNEL;
            RX_CH = CFG_UART8_RX_DMA_CHANNEL;
            dmause                  = (TX_CH == DMA_NONUSE ? FALSE : TRUE)
                                        && (RX_CH == DMA_NONUSE ? FALSE : TRUE);
            break;
        default:
            return 0;
    }

    UART_Param.mode = CN_UART_GENERAL;
    Board_UartHalfDuplexRecv(serial_no);
    
    //硬件初始化
    __UART_HardInit(serial_no);
    __UART_IntInit(serial_no);
    if(dmause)
    {
        UartTX_RX_DmaChannel[serial_no][UART_TX_DMA_CH] = TX_CH;
        UartTX_RX_DmaChannel[serial_no][UART_RX_DMA_CH] = RX_CH;
        __UART_SetDmaUsed(serial_no);
    }
    else
    {
        pUART_DmaSendBuf[serial_no]=NULL;
        pUART_DmaRecvBuf[serial_no]=NULL;
        UartTX_RX_DmaChannel[serial_no][UART_TX_DMA_CH] = DMA_NONUSE;
        UartTX_RX_DmaChannel[serial_no][UART_RX_DMA_CH] = DMA_NONUSE;
    }
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
    u32 CR_Bak;
    u8 port;
    for(port=0;port<CN_UART_NUM;port++)
    {
        if(PutStrDirectReg ==tg_UART_Reg[port])
            break;
    }
    Board_UartHalfDuplexSend(port);

    CR_Bak = PutStrDirectReg->ctrl1_bit.tdbeien;                          //Save INT
    // PutStrDirectReg->CR1 &= ~(1<<7);                        //disable send INT
    usart_interrupt_enable(PutStrDirectReg, USART_TDBE_INT, FALSE);
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
        // PutStrDirectReg->DR = str[result];
        usart_data_transmit(PutStrDirectReg, (u16)(str[result]));
    }
    PutStrDirectReg->ctrl1_bit.tdbeien = CR_Bak;                         //restore send INT

    // while((PutStrDirectReg->SR &(1<<6))!=(1<<6))
    while (usart_flag_get(PutStrDirectReg, USART_TDC_FLAG) == RESET)
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
    u32 CR_Bak;
    char result;

    CR_Bak = GetCharDirectReg->ctrl1_bit.tdbeien;                          //Save INT

    // GetCharDirectReg->CR1 &= ~(1<<7);                        //disable send INT
    usart_interrupt_enable(GetCharDirectReg, USART_TDBE_INT, FALSE);

    while(__UART_RxHadChar(GetCharDirectReg) == false) ;

    result = (char)usart_data_receive(GetCharDirectReg);
    GetCharDirectReg->ctrl1_bit.tdbeien = CR_Bak;                         //restore send INT
    
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
            __UART_HardInit(RxDirectPort);
        GetCharDirect = Uart_GetCharDirect;
    }
    return;
}



