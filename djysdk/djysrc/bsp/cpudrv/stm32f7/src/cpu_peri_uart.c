//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
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
#include <driver/include/uart.h>
#include "int.h"
#include "djyos.h"
#include "cpu_peri_uart.h"
#include "cpu_peri_int_line.h"
#include "stdlib.h"

//#include "cpu_peri_dma.h"


extern const char *gc_pCfgStdinName;    //标准输入设备
extern const char *gc_pCfgStdoutName;   //标准输出设备
//extern const char *gc_pCfgStderrName;   //标准错误输出设备
// =============================================================================
#define CN_UART1_BASE USART1_BASE//0x40013800
#define CN_UART2_BASE USART2_BASE
#define CN_UART3_BASE USART3_BASE
#define CN_UART4_BASE UART4_BASE
#define CN_UART5_BASE UART5_BASE
#define CN_UART6_BASE USART6_BASE
#define CN_UART7_BASE UART7_BASE
#define CN_UART8_BASE UART8_BASE

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
													(tagUartReg *)CN_UART5_BASE,
													(tagUartReg *)CN_UART6_BASE,
													(tagUartReg *)CN_UART7_BASE,
													(tagUartReg *)CN_UART8_BASE,};
#define CN_DMA_UNUSED   0
#define CN_DMA_USED     1

// UART对应的DMA Stream中断号
static u8 const UartDmaRxInt[] = {CN_INT_LINE_DMA2_Stream5,CN_INT_LINE_DMA1_Stream5,
								  CN_INT_LINE_DMA1_Stream1,CN_INT_LINE_DMA1_Stream2,
								  CN_INT_LINE_DMA1_Stream0,CN_INT_LINE_DMA2_Stream2,
								  CN_INT_LINE_DMA1_Stream3,CN_INT_LINE_DMA1_Stream6};

static u8 const UartDmaTxInt[] = {CN_INT_LINE_DMA2_Stream7,CN_INT_LINE_DMA1_Stream6,
								  CN_INT_LINE_DMA1_Stream3,CN_INT_LINE_DMA1_Stream4,
								  CN_INT_LINE_DMA1_Stream7,CN_INT_LINE_DMA2_Stream6,
								  CN_INT_LINE_DMA1_Stream1,CN_INT_LINE_DMA1_Stream0};

static DMA_Stream_TypeDef * const UartDmaRxStream[] =
 	 	 	 	 	 	 	 	 {DMA2_Stream5,DMA1_Stream5,
								  DMA1_Stream1,DMA1_Stream2,
								  DMA1_Stream0,DMA2_Stream2,
								  DMA1_Stream3,DMA1_Stream6};

static DMA_Stream_TypeDef * const UartDmaTxStream[] =
								 {DMA2_Stream7,DMA1_Stream6,
								  DMA1_Stream3,DMA1_Stream4,
								  DMA1_Stream7,DMA2_Stream6,
								  DMA1_Stream1,DMA1_Stream0};
static u8 const DMA_Tx_ch[] = {4,4,4,4,4,5,5,5};
static u8 const DMA_Rx_ch[] = {4,4,4,4,4,5,5,5};
// DMA正在使用标记，是否使用DMA标记
static bool_t s_UART_DmaSending[] = {false,false,false,false,false,false,false,false};
static bool_t s_UART_DmaUsed[]    = {false,false,false,false,false,false,false,false};
//static bool_t s_UART_DmaRcvIdle[] = {false,false,false,false,false,false,false,false};


#define UART1_SendBufLen            64
#define UART1_RecvBufLen            64
#define UART1_DmaBufLen             64

#define UART2_SendBufLen            64
#define UART2_RecvBufLen            64
#define UART2_DmaBufLen             64

#define UART3_SendBufLen            64
#define UART3_RecvBufLen            64
#define UART3_DmaBufLen             64

#define UART4_SendBufLen            64
#define UART4_RecvBufLen            64
#define UART4_DmaBufLen             64

#define UART5_SendBufLen            64
#define UART5_RecvBufLen            64
#define UART5_DmaBufLen             64

#define UART6_SendBufLen            64
#define UART6_RecvBufLen            64
#define UART6_DmaBufLen             64

#define UART7_SendBufLen            64
#define UART7_RecvBufLen            64
#define UART7_DmaBufLen             64

#define UART8_SendBufLen            64
#define UART8_RecvBufLen            64
#define UART8_DmaBufLen             64


static u8 *pUART_DmaSendBuf[CN_UART_NUM];
static u8 *pUART_DmaRecvBuf[CN_UART_NUM][2];

static struct UartCB *pUartCB[CN_UART_NUM];
static u8 *sp_DmaRecvBuf[CN_UART_NUM];
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
__attribute__((weak))  void UART_InLowPowerPinCfg(u8 SerialNo)
{
    return;
}
__attribute__((weak))  void UART_OutLowPowerPinCfg(u8 SerialNo)
{
    return ;
}
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
bool_t UART_LowPowerConfig(u8 port,u8 flag)
{
    if(port>=CN_UART_NUM)
        return false;
    if(flag == InLowPower)
    {
        switch(port)
        {
             case CN_UART1: RCC->APB2ENR &=~(1<<4); break;
             case CN_UART2: RCC->APB1ENR &=~(1<<17);break;
             case CN_UART3: RCC->APB1ENR &=~(1<<18);break;
             case CN_UART4: RCC->APB1ENR &=~(1<<19);break;
             case CN_UART5: RCC->APB1ENR &=~(1<<20);break;
             case CN_UART6: RCC->APB2ENR &=~(1<<5); break;
             case CN_UART7: RCC->APB1ENR &=~(1<<30);break;
             case CN_UART8: RCC->APB1ENR &=~(1<<31);break;
            default: break;
        }
        tg_UART_Reg[port]->CR1 &=~ (0x1);
        UART_InLowPowerPinCfg(port);
        return true;
    }
    else if (flag == OutLowPower)
    {
        switch(port)
            {
            case CN_UART1: RCC->APB2ENR |=(1<<4); break;
            case CN_UART2: RCC->APB1ENR |=(1<<17);break;
            case CN_UART3: RCC->APB1ENR |=(1<<18);break;
            case CN_UART4: RCC->APB1ENR |=(1<<19);break;
            case CN_UART5: RCC->APB1ENR |=(1<<20);break;
            case CN_UART6: RCC->APB2ENR |=(1<<5); break;
            case CN_UART7: RCC->APB1ENR |=(1<<30);break;
            case CN_UART8: RCC->APB1ENR |=(1<<31);break;
            default: break;
        }
        tg_UART_Reg[port]->CR1 |= (0x1);
        UART_OutLowPowerPinCfg(port);
        return true;
    }
   return false;
}

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

bool_t UART_TxEnd(u8 port)
{
	s32 timeout = 10000;
	volatile tagUartReg *Reg;
	Reg = (tagUartReg *)tg_UART_Reg[port];
    while((false == __UART_TxTranEmpty(Reg))&& (timeout > 10))
    {
        timeout -=10;
        Djy_DelayUs(10);
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
		RCC->APB2ENR |=(1<<4);//串口时钟使能
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
	case CN_UART6:
		RCC->APB2ENR |=(1<<5);
		break;
	case CN_UART7:
		RCC->APB1ENR |=(1<<30);
		break;
	case CN_UART8:
		RCC->APB1ENR |=(1<<31);
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
static void __UART_ComConfig(tagUartReg volatile *Reg,u32 port,ptu32_t data)
{
    struct COMParam *COM;
    if((data == 0) || (Reg == NULL))
        return;
    COM = (struct COMParam *)data;
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
			RCC->APB2ENR |=(1<<4);break;
    	case CN_UART2:
    		RCC->APB1ENR |=(1<<17);break;
    	case CN_UART3:
    		RCC->APB1ENR |=(1<<18);break;
    	case CN_UART4:
    		RCC->APB1ENR |=(1<<19);break;
    	case CN_UART5:
    		RCC->APB1ENR |=(1<<20);break;
    	case CN_UART6:
    		RCC->APB2ENR |=(1<<5);break;
    	case CN_UART7:
    		RCC->APB1ENR |=(1<<30);break;
    	case CN_UART8:
    		RCC->APB1ENR |=(1<<31);break;
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
		case CN_UART1: 	RCC->APB2ENR &=~(1<<4);  break;
		case CN_UART2:  RCC->APB1ENR &=~(1<<17); break;
		case CN_UART3:	RCC->APB1ENR &=~(1<<18); break;
		case CN_UART4:	RCC->APB1ENR &=~(1<<19); break;
		case CN_UART5:	RCC->APB1ENR &=~(1<<20); break;
		case CN_UART6:	RCC->APB2ENR &=~(1<<5);  break;
		case CN_UART7: 	RCC->APB1ENR &=~(1<<30); break;
		case CN_UART8:  RCC->APB1ENR &=~(1<<31); break;
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
    tg_UART_Reg[SerialNo]->CR3 = (1<<11);
    tg_UART_Reg[SerialNo]->CR1 = 0x202d;
    tg_UART_Reg[SerialNo]->CR2 = 0x0;
    __UART_BaudSet(tg_UART_Reg[SerialNo],SerialNo,115200);
    tg_UART_Reg[SerialNo]->ICR |= 0x40;
}


void UART_HardInit(u8 SerialNo)
{
	 if(SerialNo > CN_UART_NUM)
	        return;
	 //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
	   //初始化uart硬件控制数据结构
	tg_UART_Reg[SerialNo]->CR3 = (1<<11);
	tg_UART_Reg[SerialNo]->CR1 = 0x202d;
	tg_UART_Reg[SerialNo]->CR2 = 0x0;
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
        Djy_DelayUs(1);
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
    u8 *DmaSendBuf;
    u32 DmaSendBufLen;

    switch(port)
    {
		case CN_UART1:
			DmaSendBufLen = UART1_DmaBufLen;  break;
		case CN_UART2:
			DmaSendBufLen = UART2_DmaBufLen;  break;
		case CN_UART3:
			DmaSendBufLen = UART3_DmaBufLen;  break;
		case CN_UART4:
			DmaSendBufLen = UART4_DmaBufLen;  break;
		case CN_UART5:
			DmaSendBufLen = UART5_DmaBufLen;  break;
		case CN_UART6:
			DmaSendBufLen = UART6_DmaBufLen;  break;
		case CN_UART7:
			DmaSendBufLen = UART7_DmaBufLen;  break;
		case CN_UART8:
			DmaSendBufLen = UART8_DmaBufLen;  break;
		 default:	break;
    }
    DmaSendBuf = pUART_DmaSendBuf[port];
    if(true == __uart_dma_timeout(s_UART_DmaSending[port]))
        return 0;
    num = UART_PortRead(pUartCB[port],DmaSendBuf,DmaSendBufLen,0);
    addr = (u32)DmaSendBuf;

    if(num > 0)
    {
        DMA_Enable(UartDmaTxStream[port],addr,num); //启动DMA
        s_UART_DmaSending[port] = true;
    }
    else
        tg_UART_Reg[port]->CR1 |= (1<<6);
    return 0;
}

// =============================================================================
// 功能: 启动串口发送，其目的是触发中断，用中断方式发送数据。
// 参数: Reg,被操作的串口寄存器指针.
// 返回: 发送的个数
// =============================================================================
static u32 __UART_SendStart (tagUartReg *Reg,u32 timeout)
{
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
    default:return 0;
    }

    Reg->CR1 &=~(1<<6);    //关TC中断
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
    struct HeapCB *heap;
    u32 DmaBufLen;

    if(s_UART_DmaUsed[port] == CN_DMA_USED)
        return ;
    heap =M_FindHeap("nocache");
    if(heap==NULL)
        return;

    switch(port)
    {
    case CN_UART1:
        DmaBufLen = UART1_DmaBufLen;  break;
    case CN_UART2:
        DmaBufLen = UART2_DmaBufLen;  break;
    case CN_UART3:
        DmaBufLen = UART3_DmaBufLen;  break;
    case CN_UART4:
        DmaBufLen = UART4_DmaBufLen;  break;
    case CN_UART5:
        DmaBufLen = UART5_DmaBufLen;  break;
    case CN_UART6:
        DmaBufLen = UART6_DmaBufLen;  break;
    case CN_UART7:
        DmaBufLen = UART7_DmaBufLen;  break;
    case CN_UART8:
        DmaBufLen = UART8_DmaBufLen;  break;
    default :        return;
    }

    if((pUART_DmaRecvBuf[port][0]==NULL)||(pUART_DmaRecvBuf[port][1]==NULL) )
    {
        pUART_DmaRecvBuf[port][0] = M_MallocHeap(DmaBufLen,heap,0);

        if(pUART_DmaRecvBuf[port] != NULL)
            pUART_DmaRecvBuf[port][1] = &pUART_DmaRecvBuf[port][0][DmaBufLen/2];
        else
            return;
    }

    if(pUART_DmaSendBuf[port]==NULL)
        pUART_DmaSendBuf[port] = M_MallocHeap(DmaBufLen,heap,0);

    if(pUART_DmaSendBuf[port] == NULL)
    {
        free(pUART_DmaRecvBuf[port]);
        pUART_DmaRecvBuf[port][0]=NULL;
        pUART_DmaRecvBuf[port][1]=NULL;
        return;
    }

    sp_DmaRecvBuf[port] = pUART_DmaRecvBuf[port][0];
    __UART_RxIntDisable(CN_DMA_UNUSED,port);//串口中断失能
    __UART_TxIntDisable(CN_DMA_UNUSED,port);

    DMA_Config(UartDmaRxStream[port],DMA_Rx_ch[port],(u32)&(tg_UART_Reg[port]->RDR),true,
            (u32)pUART_DmaRecvBuf[port][0],(u32)pUART_DmaRecvBuf[port][1],DMA_DIR_P2M,\
            DMA_DATABITS_8,DMA_DATABITS_8,DmaBufLen/2);

    DMA_Config(UartDmaTxStream[port],DMA_Tx_ch[port],(u32)&(tg_UART_Reg[port]->TDR),false,
            (u32)pUART_DmaSendBuf[port],0,DMA_DIR_M2P,DMA_DATABITS_8,DMA_DATABITS_8,DmaBufLen);

    s_UART_DmaUsed[port] = CN_DMA_USED;
    DMA_Enable(UartDmaRxStream[port],(u32)pUART_DmaRecvBuf[port][0],DmaBufLen/2);//启动dma通道
    sp_DmaRecvBuf[port] = (u8 *)pUART_DmaRecvBuf[port][0];

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

    tg_UART_Reg[port]->ICR |= (1<<4);//clear  idle int
    tg_UART_Reg[port]->CR1 |= (1<<4);//enable idle int
    tg_UART_Reg[port]->CR3 |= 0x00c0;  //设置串口使用dma收发
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
	else	return;

    free(pUART_DmaSendBuf[port]);
    pUART_DmaSendBuf[port]=NULL;
    free(pUART_DmaRecvBuf[port]);
    pUART_DmaRecvBuf[port][0]=NULL;
    pUART_DmaRecvBuf[port][1]=NULL;
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
    tg_UART_Reg[port]->CR1 &= ~(1<<4);//disable idle int
}
// =============================================================================
// 功能: 串口设备的控制函数,与具体的硬件寄存器设置相关
// 参数: Reg,被操作的串口寄存器指针.
//       cmd,操作类型
//       data1,data2,含义依cmd而定
// 返回: 无意义.
// =============================================================================
static ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd, u32 data1,u32 data2)
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
        case CN_UART_START:
            __UART_Enable(port);
            break;
        case CN_UART_STOP:
            __UART_Disable(port);
            break;

        case CN_UART_SET_BAUD:  //设置Baud
             __UART_BaudSet(Reg,port, data1);
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
            __UART_ComConfig(Reg,port,data1);
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
    case CN_UART6:
        DmaSendBufLen = UART6_DmaBufLen;        break;
    case CN_UART7:
        DmaSendBufLen = UART7_DmaBufLen;        break;
    case CN_UART8:
        DmaSendBufLen = UART8_DmaBufLen;        break;
    default:        break;
    }

    DmaSendBuf = pUART_DmaSendBuf[port];
    DMA_ClearIntFlag(UartDmaTxStream[port]);
    num = UART_PortRead(pUartCB[port],DmaSendBuf,DmaSendBufLen,0);
    if(num == 0)
    {
        s_UART_DmaSending[port] = false;
        tg_UART_Reg[port]->CR1 |= (1<<6);
    }
    else
    {
        DMA_Enable(UartDmaTxStream[port],(u32)DmaSendBuf,num);
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
    u32 recvs,num,DmaRecvBufLen,newrcvlen;
    u8 *DmaRecvBufNow,*DmaRecvBufold;

    switch(port)
    {
    case CN_UART1:
        DmaRecvBufLen = UART1_DmaBufLen/2;        break;
    case CN_UART2:
        DmaRecvBufLen = UART2_DmaBufLen/2;        break;
    case CN_UART3:
        DmaRecvBufLen = UART3_DmaBufLen/2;        break;
    case CN_UART4:
        DmaRecvBufLen = UART4_DmaBufLen/2;        break;
    case CN_UART5:
        DmaRecvBufLen = UART5_DmaBufLen/2;        break;
    case CN_UART6:
        DmaRecvBufLen = UART6_DmaBufLen/2;        break;
    case CN_UART7:
        DmaRecvBufLen = UART7_DmaBufLen/2;        break;
    case CN_UART8:
        DmaRecvBufLen = UART8_DmaBufLen/2;        break;
    default:        return 0;
    }

    DMA_ClearIntFlag(UartDmaRxStream[port]);

    if(((u32)&pUART_DmaRecvBuf[port][0] <= (u32)sp_DmaRecvBuf[port]) && \
      ((u32)sp_DmaRecvBuf[port] < (u32)&pUART_DmaRecvBuf[port][0][DmaRecvBufLen]))
    {
        DmaRecvBufold=pUART_DmaRecvBuf[port][0];
    }
    else if(((u32)&pUART_DmaRecvBuf[port][1] <= (u32)sp_DmaRecvBuf[port]) && \
           ((u32)sp_DmaRecvBuf[port] < (u32)&pUART_DmaRecvBuf[port][1][DmaRecvBufLen]))
    {
        DmaRecvBufold=pUART_DmaRecvBuf[port][1];
    }
    else
    {
        UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
        DMA_Enable(UartDmaRxStream[port],(u32)pUART_DmaRecvBuf[port][0],DmaRecvBufLen);//启动dma通道
        DmaRecvBufNow=pUART_DmaRecvBuf[port][0];
    }

    if(UartDmaRxStream[port]->CR & (1<<19))
        DmaRecvBufNow =  pUART_DmaRecvBuf[port][1];
    else
        DmaRecvBufNow =  pUART_DmaRecvBuf[port][0];


    if(DmaRecvBufNow >= DmaRecvBufold)//未发生缓冲区环绕
    {
        recvs = DmaRecvBufLen - UartDmaRxStream[port]->NDTR;
        newrcvlen = &DmaRecvBufNow[recvs] - sp_DmaRecvBuf[port];
        num = UART_PortWrite(pUartCB[port],sp_DmaRecvBuf[port],newrcvlen,0);

        if(num != newrcvlen)
            UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
    }
    else//区发生缓冲区环绕
    {
        recvs = &DmaRecvBufold[DmaRecvBufLen]-sp_DmaRecvBuf[port];
        num = UART_PortWrite(pUartCB[port],sp_DmaRecvBuf[port],recvs,0);
        if(num != recvs)
            UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);

        recvs = DmaRecvBufLen - UartDmaRxStream[port]->NDTR;

        num = UART_PortWrite(pUartCB[port],DmaRecvBufNow,recvs,0);
        if(num != recvs)
            UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
    }
    sp_DmaRecvBuf[port] = &DmaRecvBufNow[recvs];

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
    struct UartCB *UCB;
    tagUartReg *Reg;
    u32 num;
    u8 ch;

    UCB = pUartCB[port];
    Reg = (tagUartReg *)tg_UART_Reg[port];
    if( (Reg->ISR & (1<<5)) &&(Reg->CR1 &(1<<5)))           //接收中断
    {
        ch = (u8)Reg->RDR;
        num = UART_PortWrite(UCB,&ch,1,0);
        if(num != 1)
        {
            UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
        }
    }
    if((Reg->ISR & (1<<6))&&(Reg->CR1&(1<<6)))//TC
    {
        Reg->CR1 &=~(1<<6);    //关TC中断
        Board_UartHalfDuplexRecv(port);
    }

    if((Reg->ISR & (1<<7))&&(Reg->CR1 & (1<<7))  )    //发送中断
    {
        num = UART_PortRead(UCB,&ch,1,0);
        if(num != 0)
            Reg->TDR = ch;
        else
        {
            Reg->CR1 &= ~(1<<7);        //txeie
            Reg->CR1 |= (1<<6);
        }
    }
    if(Reg->ISR & (1<<3))        //ORE过载错误
    {
        ch = (u8)Reg->RDR;
        num = Reg->ISR;     //读一下sr寄存器
        UART_ErrHandle(UCB,CN_UART_FIFO_OVER_ERR);
        Reg->ICR |=(1<<3);//清过载标志位
    }
    if((Reg->ISR & (1<<4)) && (Reg->CR1 & (1<<4)))
    {
        Reg->ICR |=(1<<4);
        UART_DmaRx_ISR(port);
    }
    if(Reg->ISR & (1<<1))//帧错误直接清除暂时不做处理
    {
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
        UART_Param.TxRingBufLen = UART1_SendBufLen;
        UART_Param.RxRingBufLen = UART1_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART2://串口2
        UART_Param.Name         = "UART2";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART2_BASE;
        UART_Param.TxRingBufLen = UART2_SendBufLen;
        UART_Param.RxRingBufLen = UART2_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;

    case CN_UART3://串口3
        UART_Param.Name         = "UART3";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART3_BASE;
        UART_Param.TxRingBufLen = UART3_SendBufLen;
        UART_Param.RxRingBufLen = UART3_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;

    case CN_UART4://串口4
        UART_Param.Name         = "UART4";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART4_BASE;
        UART_Param.TxRingBufLen = UART4_SendBufLen;
        UART_Param.RxRingBufLen = UART4_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART5://串口5
        UART_Param.Name         = "UART5";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART5_BASE;
        UART_Param.TxRingBufLen = UART5_SendBufLen;
        UART_Param.RxRingBufLen = UART5_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART6://串口6
        UART_Param.Name         = "UART6";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART6_BASE;
        UART_Param.TxRingBufLen = UART6_SendBufLen;
        UART_Param.RxRingBufLen = UART6_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART7://串口7
        UART_Param.Name         = "UART7";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART7_BASE;
        UART_Param.TxRingBufLen = UART7_SendBufLen;
        UART_Param.RxRingBufLen = UART7_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART8://串口8
        UART_Param.Name         = "UART8";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART8_BASE;
        UART_Param.TxRingBufLen = UART8_SendBufLen;
        UART_Param.RxRingBufLen = UART8_RecvBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    default:
        return 0;
    }
    UART_Param.mode = CN_UART_GENERAL;
    pUART_DmaSendBuf[serial_no]=NULL;
    pUART_DmaRecvBuf[serial_no][0]=NULL;
    pUART_DmaRecvBuf[serial_no][1]=NULL;
    Board_UartHalfDuplexRecv(serial_no);
    //硬件初始化
    __UART_HardInit(serial_no);
    __UART_IntInit(serial_no);
    sUartInited |= (0x01 << serial_no);
    pUartCB[serial_no] = UART_InstallPort(&UART_Param);
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
            Djy_DelayUs(10);
        }
        if( (timeout <= 10) || (result == len))
            break;
        PutStrDirectReg->TDR = str[result];
    }
    PutStrDirectReg->CR1 = CR_Bak;                         //restore send INT
    while((PutStrDirectReg->ISR &(1<<6))!=(1<<6))
    {
        timeout -=10;
        Djy_DelayUs(10);
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
    result = GetCharDirectReg->RDR;
    PutStrDirectReg->CR1 = CR_Bak;                         //restore send INT
    return result;
}

//----初始化内核级IO-----------------------------------------------------------
//功能：初始化内核级输入和输出所需的runtime函数指针。
//参数：无
//返回：无
//-----------------------------------------------------------------------------
void Stdio_KnlInOutInit(u32 para)
{
    if(!strcmp(gc_pCfgStdoutName,"/dev/UART1"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART1_BASE;
        TxDirectPort = CN_UART1;
    }
    else if(!strcmp(gc_pCfgStdoutName,"/dev/UART2"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART2_BASE;
        TxDirectPort = CN_UART2;
    }
    else if(!strcmp(gc_pCfgStdoutName,"/dev/UART3"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART3_BASE;
        TxDirectPort = CN_UART3;
    }
    else if(!strcmp(gc_pCfgStdoutName,"/dev/UART4"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART4_BASE;
        TxDirectPort = CN_UART4;
    }
    else if(!strcmp(gc_pCfgStdoutName,"/dev/UART5"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART5_BASE;
        TxDirectPort = CN_UART5;
    }
    else if(!strcmp(gc_pCfgStdoutName,"/dev/UART6"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART6_BASE;
        TxDirectPort = CN_UART6;
    }
    else if(!strcmp(gc_pCfgStdoutName,"/dev/UART7"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART7_BASE;
        TxDirectPort = CN_UART7;
    }
    else if(!strcmp(gc_pCfgStdoutName,"/dev/UART8"))
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

    if(!strcmp(gc_pCfgStdinName,"/dev/UART1"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART1_BASE;
        RxDirectPort = CN_UART1;
    }
    else if(!strcmp(gc_pCfgStdinName,"/dev/UART2"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART2_BASE;
        RxDirectPort = CN_UART2;
    }
    else if(!strcmp(gc_pCfgStdinName,"/dev/UART3"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART3_BASE;
        RxDirectPort = CN_UART3;
    }
    else if(!strcmp(gc_pCfgStdinName,"/dev/UART4"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART4_BASE;
        RxDirectPort = CN_UART4;
    }
    else if(!strcmp(gc_pCfgStdinName,"/dev/UART5"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART5_BASE;
        RxDirectPort = CN_UART5;
    }
    else if(!strcmp(gc_pCfgStdinName,"/dev/UART6"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART6_BASE;
        RxDirectPort = CN_UART6;
    }
    else if(!strcmp(gc_pCfgStdinName,"/dev/UART7"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART7_BASE;
        RxDirectPort = CN_UART7;
    }
    else if(!strcmp(gc_pCfgStdinName,"/dev/UART8"))
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

