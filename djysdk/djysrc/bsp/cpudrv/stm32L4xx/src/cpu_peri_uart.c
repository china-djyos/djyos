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
#include <stdlib.h>
#include "string.h"
#include "errno.h"
#include "systime.h"
#include "cpu_peri.h"
#include <driver/include/uart.h>
#include "int.h"
#include "djyos.h"
#include "cpu_peri_uart.h"
#include "cpu_peri_int_line.h"

#include "stm32l4xx.h"
#include "stm32l4xx_hal_uart.h"
#include <stm32l4xx_hal_rcc.h>
#define tagUartReg USART_TypeDef

extern const char *gc_pCfgStdinName;    //标准输入设备
extern const char *gc_pCfgStdoutName;   //标准输出设备
//extern const char *gc_pCfgStderrName;   //标准错误输出设备

// =============================================================================
typedef struct
{
	char* Name;
	tagUartReg volatile *Reg;
	u8 TxRxIntLine;
	u8 DmaRxIntLine;
	u8 DmaTxIntLine;
	u8 DmaReq;
	DMA_Channel_TypeDef *DmaRxCh;
	DMA_Channel_TypeDef *DmaTxCh;
}tagUartCfg;

static const tagUartCfg sUartCfg[CN_UART_NUM] = {
{"UART1",(tagUartReg volatile *)USART1_BASE,CN_INT_LINE_USART1,CN_INT_LINE_DMA1_CH5,CN_INT_LINE_DMA1_CH4,DMA_REQUEST_2,DMA1_Channel5,DMA1_Channel4},
{"UART2",(tagUartReg volatile *)USART2_BASE,CN_INT_LINE_USART2,CN_INT_LINE_DMA1_CH6,CN_INT_LINE_DMA1_CH7,DMA_REQUEST_2,DMA1_Channel6,DMA1_Channel7},
{"UART3",(tagUartReg volatile *)USART3_BASE,CN_INT_LINE_USART3,CN_INT_LINE_DMA1_CH3,CN_INT_LINE_DMA1_CH2,DMA_REQUEST_2,DMA1_Channel3,DMA1_Channel2},
{"UART4",(tagUartReg volatile *)UART4_BASE,CN_INT_LINE_UART4,CN_INT_LINE_DMA2_CH3,CN_INT_LINE_DMA2_CH5,DMA_REQUEST_2,DMA2_Channel3,DMA2_Channel5},
{"UART5",(tagUartReg volatile *)UART5_BASE,CN_INT_LINE_UART5,CN_INT_LINE_DMA2_CH2,CN_INT_LINE_DMA2_CH1,DMA_REQUEST_2,DMA2_Channel2,DMA2_Channel1},
{"UART6",(tagUartReg volatile *)LPUART1_BASE,CN_INT_LINE_LPUART1,CN_INT_LINE_DMA2_CH7,CN_INT_LINE_DMA2_CH6,DMA_REQUEST_4,DMA2_Channel7,DMA2_Channel6}
};
// =============================================================================

#define UART_SndBufLen    32
#define UART_RxBufLen	  32

static u32 TxByteTime;                    //正常发送一个字节所需要的时间
static UART_HandleTypeDef *PutStrDirectH;       //用于printk发送的串口
static UART_HandleTypeDef *GetCharDirectH;      //用于直接接收的串口
static u32 TxDirectPort;                  //用于printk发送的串口号
static u32 RxDirectPort;                  //用于直接接收的串口号
// =============================================================================

static struct UartCB *pUartCB[CN_UART_NUM];
static u8* pUartSndBuf[CN_UART_NUM] = {NULL};
static u8* pUartRcvBuf[CN_UART_NUM] = {NULL};
static bool_t sUartDmaUsed[] = {false,false,false,false,false,false};
static bool_t sUartSending[] = {false,false,false,false,false,false};
static u8 sDmaRcvOff[CN_UART_NUM] = {0};

static UART_HandleTypeDef sHuart[CN_UART_NUM];
static UART_HandleTypeDef *phuart[CN_UART_NUM] = {
		&sHuart[0],&sHuart[1],&sHuart[2],&sHuart[3],&sHuart[4],&sHuart[5]};


// =============================================================================
static ptu32_t UART_ISR(ptu32_t IniLine);
__weak void Board_UartHalfDuplexSend(u8 port)
{
	//user added if need out of the file
}
__weak void Board_UartHalfDuplexRecv(u8 port)
{
	//user added if need out of the file
}

static void __UART_StartRecv(u8 port)
{
	if(sUartDmaUsed[port])
	{
		sDmaRcvOff[port] = 0;
		HAL_UART_Receive_DMA(phuart[port],pUartRcvBuf[port],UART_RxBufLen);
	}
	else
	{
		HAL_UART_Receive_IT(phuart[port],pUartRcvBuf[port],1);
	}
}


static void __UART_RCC_Enable(u8 port,u8 en)
{
	if(en)
	{
	    switch(port)
	    {//使能串口1时钟
	    case CN_UART1:      __HAL_RCC_USART1_CLK_ENABLE();        break; //使能串口1时钟
	    case CN_UART2:      __HAL_RCC_USART2_CLK_ENABLE();        break;
	    case CN_UART3:      __HAL_RCC_USART3_CLK_ENABLE();        break;
	    case CN_UART4:      __HAL_RCC_UART4_CLK_ENABLE();        break;
	    case CN_UART5:      __HAL_RCC_UART5_CLK_ENABLE();        break;
	    case CN_UART6:      __HAL_RCC_LPUART1_CLK_ENABLE();        break;
	    default:
	        break;
	    }
	}
	else
	{
	    switch(port)
	    {
	    case CN_UART1:      __HAL_RCC_USART1_CLK_DISABLE();        break;
	    case CN_UART2:      __HAL_RCC_USART2_CLK_DISABLE();        break;
	    case CN_UART3:      __HAL_RCC_USART3_CLK_DISABLE();        break;
	    case CN_UART4:      __HAL_RCC_UART4_CLK_DISABLE();        break;
	    case CN_UART5:      __HAL_RCC_UART5_CLK_DISABLE();        break;
	    case CN_UART6:      __HAL_RCC_LPUART1_CLK_DISABLE();        break;
	        break;
	    default:
	        break;
	    }
	}
}
// =============================================================================
// 功能: 设置串口baud,PCLK为25M，CPU主频为216M计算,该驱动只提供9600、19200、57600、
//       115200四种波特率设置
// 参数: Reg,被操作的寄存器组指针
//       port,串口号
//       baud,波特率
// 返回: 无
// =============================================================================
static void __UART_BaudSet(u32 port,u32 baud)
{
    if(port < CN_UART_NUM)
    {
    	phuart[port]->Init.BaudRate = baud;
    	HAL_UART_Abort(phuart[port]);
    	UART_SetConfig(phuart[port]);
    	__UART_StartRecv(port);
    }
    if(TxDirectPort == port)
    {
        TxByteTime = 11000000/baud;     //1个字节传输时间，按10bit，+10%计算
    }
}

static void __UART_Start(u8 port)
{
	__UART_RCC_Enable(port,1);
	HAL_UART_Abort(phuart[port]);
	__UART_StartRecv(port);
}
static void __UART_Stop(u8 port)
{
	HAL_UART_Abort(phuart[port]);
	__UART_RCC_Enable(port,0);
}

// =============================================================================
// 功能: 有关串口参数的初始化，波特率，停止位，奇偶校验位
// 参数：port,串口号
//      data，参数
// 返回: 无
// =============================================================================
static void __UART_ComConfig(tagUartReg volatile *Reg,u32 port,ptu32_t data)
{
    struct COMParam *COM;
    if(port < CN_UART_NUM)
    {
    	COM = (struct COMParam *)data;
    	phuart[port]->Init.BaudRate = COM->BaudRate;
        switch(COM->DataBits)               // data bits
        {
        case CN_UART_DATABITS_8:    phuart[port]->Init.WordLength = UART_WORDLENGTH_8B;break;
        case CN_UART_DATABITS_9:    phuart[port]->Init.WordLength = UART_WORDLENGTH_9B;break;
        default:break;
        }

        switch(COM->Parity)                 // parity
        {
        case CN_UART_PARITY_NONE:   phuart[port]->Init.Parity = UART_PARITY_NONE;break;
        case CN_UART_PARITY_ODD:    phuart[port]->Init.Parity = UART_PARITY_ODD;break;
        case CN_UART_PARITY_EVEN:   phuart[port]->Init.Parity = UART_PARITY_EVEN;break;
        default:break;
        }

        switch(COM->StopBits)               // stop bits
        {
        case CN_UART_STOPBITS_1:    phuart[port]->Init.StopBits = UART_STOPBITS_1;break;
        case CN_UART_STOPBITS_1_5:  phuart[port]->Init.StopBits = UART_STOPBITS_1_5;break;
        case CN_UART_STOPBITS_2:    phuart[port]->Init.StopBits = UART_STOPBITS_2;break;
        default:break;
        }
    	HAL_UART_Abort(phuart[port]);
    	UART_SetConfig(phuart[port]);
    	__UART_StartRecv(port);
    }
}


// =============================================================================
// 功能: 初始化UART对应的中断线，并初始化中断入口函数
// 参数：SerialNo,串口号
// 返回: 无
// =============================================================================
static void __UART_IntInit(u32 port)
{
    u8 IntLine;

    IntLine = sUartCfg[port].TxRxIntLine;
    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,UART_ISR);
    Int_SetIsrPara(IntLine,port);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
}

// HAL needed
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	u8 port;
	for(port = CN_UART1;port < CN_UART_NUM; port ++)
	{
		if(huart == phuart[port])
			break;
	}
	__UART_RCC_Enable(port,1);
}

// =============================================================================
// 功能: 硬件参数配置和寄存器的初始化，包括波特率、停止位、校验位、数据位，默认情况下:
//       波特率:115200  ； 停止位:1 ; 校验:无 ; 数据位:8bit
// 参数: SerialNo,串口号，0~3
// 返回: 无
// =============================================================================
static void __UART_HardInit(u8 port)
{
	if(port < CN_UART_NUM)
	{
		__UART_RCC_Enable(port,1);
		phuart[port]->Instance = sUartCfg[port].Reg;
		phuart[port]->Init.BaudRate = 115200;
		phuart[port]->Init.WordLength = UART_WORDLENGTH_8B;
		phuart[port]->Init.Parity = UART_PARITY_NONE;
		phuart[port]->Init.StopBits = UART_STOPBITS_1;
		phuart[port]->Init.Mode = UART_MODE_TX_RX;
		phuart[port]->Init.HwFlowCtl = UART_HWCONTROL_NONE;
		phuart[port]->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_ENABLE;
		phuart[port]->Init.OverSampling = UART_OVERSAMPLING_16;
		phuart[port]->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

		HAL_UART_DeInit(phuart[port]);
		HAL_UART_Init(phuart[port]);
		__UART_StartRecv(port);
	}
}

// =============================================================================
// 功能: 启动串口发送，包括两种情况，使用DMA或不使用DMA
// 参数: Reg,被操作的串口寄存器指针.
// 返回: 发送的个数
// =============================================================================
static u32 __UART_SendStart (tagUartReg *Reg,u32 timeout)
{
    u16 port,len=0;

    for(port = CN_UART1;port < CN_UART_NUM;port ++)
    {
    	if(Reg == sUartCfg[port].Reg)
    		break;
    }
    if(port < CN_UART_NUM)
    {
        if(!sUartSending[port])
        {
    		len = UART_PortRead(pUartCB[port],pUartSndBuf[port],UART_SndBufLen,0);
    		if(len > 0)
    		{
				if(sUartDmaUsed[port])
				{
					HAL_UART_Transmit_DMA(phuart[port],pUartSndBuf[port],len);
				}
				else
				{
					HAL_UART_Transmit_IT(phuart[port],pUartSndBuf[port],len);
				}
				sUartSending[port] = true;
    		}
        }
    }

    return len;
}

// =============================================================================
// 功能：配置是否使用DMA传输，
// 参数：port，使用的串口号
//      enable,1 or 0,使能或禁能
// 返回：无
// =============================================================================
void __UART_SetDmaUsed(u8 port,u8 enable)
{
	u8 IntLine;
	if( (false == sUartDmaUsed[port]) && (enable) )
	{
		__HAL_RCC_DMA1_CLK_ENABLE();
		__HAL_RCC_DMA2_CLK_ENABLE();
		HAL_UART_Abort(phuart[port]);

		if(phuart[port]->hdmarx == NULL)
		{
			phuart[port]->hdmarx = (DMA_HandleTypeDef*)malloc(2*sizeof(DMA_HandleTypeDef));
			if(NULL == phuart[port]->hdmarx)
				return;
			phuart[port]->hdmatx = phuart[port]->hdmarx + 1;
		}

		phuart[port]->hdmarx->Instance = sUartCfg[port].DmaRxCh;
		phuart[port]->hdmarx->Init.Direction = DMA_PERIPH_TO_MEMORY;
		phuart[port]->hdmarx->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		phuart[port]->hdmarx->Init.MemInc = DMA_MINC_ENABLE;
		phuart[port]->hdmarx->Init.Mode = DMA_CIRCULAR;
		phuart[port]->hdmarx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		phuart[port]->hdmarx->Init.PeriphInc = DMA_PINC_DISABLE;
		phuart[port]->hdmarx->Init.Priority = DMA_PRIORITY_HIGH;
		phuart[port]->hdmarx->Init.Request = sUartCfg[port].DmaReq;
		phuart[port]->hdmarx->XferCpltCallback = 0;
		phuart[port]->hdmarx->XferHalfCpltCallback = 0;
		phuart[port]->hdmarx->XferErrorCallback = 0;
		phuart[port]->hdmarx->Parent = (void*)phuart[port];

		phuart[port]->hdmatx->Instance = sUartCfg[port].DmaTxCh;
		phuart[port]->hdmatx->Init.Direction = DMA_MEMORY_TO_PERIPH;
		phuart[port]->hdmatx->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		phuart[port]->hdmatx->Init.MemInc = DMA_MINC_ENABLE;
		phuart[port]->hdmatx->Init.Mode = DMA_NORMAL;
		phuart[port]->hdmatx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		phuart[port]->hdmatx->Init.PeriphInc = DMA_PINC_DISABLE;
		phuart[port]->hdmatx->Init.Priority = DMA_PRIORITY_LOW;
		phuart[port]->hdmatx->Init.Request = sUartCfg[port].DmaReq;
		phuart[port]->hdmatx->XferCpltCallback = 0;
		phuart[port]->hdmatx->XferErrorCallback = 0;
		phuart[port]->hdmatx->XferHalfCpltCallback = 0;
		phuart[port]->hdmatx->Parent = (void*)phuart[port];
		HAL_DMA_Init(phuart[port]->hdmatx);
		HAL_DMA_Init(phuart[port]->hdmarx);

		sUartCfg[port].Reg->CR1 |= USART_CR1_IDLEIE;//enable idle int
		sUartDmaUsed[port] = true;

		IntLine = sUartCfg[port].DmaRxIntLine;
	    Int_Register(IntLine);
	    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
	    Int_IsrConnect(IntLine,(u32(*)(ptu32_t))HAL_DMA_IRQHandler);
	    Int_SetIsrPara(IntLine,(ptu32_t)(phuart[port]->hdmarx));
	    Int_SettoAsynSignal(IntLine);
	    Int_ClearLine(IntLine);
	    Int_RestoreAsynLine(IntLine);

	    IntLine = sUartCfg[port].DmaTxIntLine;
	    Int_Register(IntLine);
	    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
	    Int_IsrConnect(IntLine,(u32(*)(ptu32_t))HAL_DMA_IRQHandler);
	    Int_SetIsrPara(IntLine,(ptu32_t)(phuart[port]->hdmatx));
	    Int_SettoAsynSignal(IntLine);
	    Int_ClearLine(IntLine);
	    Int_RestoreAsynLine(IntLine);
	}
	else if( (sUartDmaUsed[port]) && (!enable) )
	{
		HAL_UART_Abort(phuart[port]);
		free(phuart[port]->hdmarx);
		phuart[port]->hdmarx = NULL;
		phuart[port]->hdmatx = NULL;

		sUartCfg[port].Reg->ICR = USART_ICR_IDLECF;
		sUartCfg[port].Reg->CR1 &= ~USART_CR1_IDLEIE;
		sUartDmaUsed[port] = false;

		IntLine = sUartCfg[port].DmaRxIntLine;
		Int_IsrDisConnect(IntLine);
		Int_ClearLine(IntLine);
		Int_UnRegister(IntLine);

		IntLine = sUartCfg[port].DmaTxIntLine;
		Int_IsrDisConnect(IntLine);
		Int_ClearLine(IntLine);
		Int_UnRegister(IntLine);
	}
	else
	{

	}
	__UART_StartRecv(port);
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
    u32 timeout = 100000;
    for(port = CN_UART1;port < CN_UART_NUM;port ++)
    {
    	if(Reg == sUartCfg[port].Reg)
    		break;
    }
    if(port < CN_UART_NUM)
    {
        switch(cmd)
        {
        case CN_UART_START:
        	__UART_Start(port);
        	break;
        case CN_UART_STOP:
        	__UART_Stop(port);
            break;
        case CN_UART_SET_BAUD:
        	__UART_BaudSet(port,data1);
        	break;
        case CN_UART_COM_SET:
            __UART_ComConfig(Reg,port,data1);
            break;
        case CN_UART_HALF_DUPLEX_SEND: //发送数据
        	Board_UartHalfDuplexSend(port);
        	break;
        case CN_UART_HALF_DUPLEX_RECV: //接收数据
            while((HAL_UART_STATE_READY != HAL_UART_GetState(phuart[port]))
            		&& (timeout > 10))
            {
                timeout -=10;
                Djy_DelayUs(10);
            }
            Board_UartHalfDuplexRecv(port);
        	break;
        case CN_UART_DMA_USED:
            __UART_SetDmaUsed(port,1);
            break;
        case CN_UART_DMA_UNUSED:
        	__UART_SetDmaUsed(port,0);
            break;
        default:
        	break;
        }
    }
    return result;
}

// =============================================================================
// 功能：发送完成回调函数，由HAL中断函数调用，若DMA中断调用此函数，由必然是配置的传送长度
//      完成，若传送完成，需配置发送ING标记为false
// 参数：huart，HAL提供的UARTHandler
// 返回：无
// =============================================================================
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	u16 port,len;
	for(port = CN_UART1;port < CN_UART_NUM; port ++)
	{
		if(huart == phuart[port])
			break;
	}
	if(port < CN_UART_NUM)
	{
	    len = UART_PortRead(pUartCB[port],pUartSndBuf[port],UART_SndBufLen,0);
	    if(len > 0)
	    {
	    	if(sUartDmaUsed[port])
	    		HAL_UART_Transmit_DMA(phuart[port],pUartSndBuf[port],len);
	    	else
	    		HAL_UART_Transmit_IT(phuart[port],pUartSndBuf[port],len);
	    }
	    else
	    	sUartSending[port] = false;
	}
}

// =============================================================================
// 功能：接收完成回调函数，包括DMA接收完成，由HAL中断函数调用，若DMA中断调用此函数，由必然
//      是配置的传送长度完成，即下面代码中的num == UART_RxBufLen
// 参数：huart，HAL提供的UARTHandler
// 返回：无
// =============================================================================
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	u8 port,num;
	u8 *src;
	for(port = CN_UART1;port < CN_UART_NUM; port ++)
	{
		if(huart == phuart[port])
			break;
	}
	if(port < CN_UART_NUM)
	{
		if(sUartDmaUsed[port])
		{
			num = UART_RxBufLen - sDmaRcvOff[port];
			src = pUartRcvBuf[port] + sDmaRcvOff[port];
			if(num != UART_PortWrite(pUartCB[port],src,num,0))
			{
				UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
			}
			sDmaRcvOff[port] = 0;
		}
		else
		{
			num = UART_PortWrite(pUartCB[port],pUartRcvBuf[port],1,0);
			if(num != 1)
			{
				UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
			}
			HAL_UART_Receive_IT(phuart[port],pUartRcvBuf[port],1);
		}
	}
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)//半传输中断
{
	u8 port,num;
	u8 *src;
	for(port = CN_UART1;port < CN_UART_NUM; port ++)
	{
		if(huart == phuart[port])
			break;
	}
	if(port < CN_UART_NUM)
	{
		if(sUartDmaUsed[port])
		{
			num = UART_RxBufLen/2 - sDmaRcvOff[port];
			src = pUartRcvBuf[port] + sDmaRcvOff[port];
			if(num != UART_PortWrite(pUartCB[port],src,num,0))
			{
				UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
			}
			sDmaRcvOff[port] = UART_RxBufLen/2;
		}
		else
		{
			num = UART_PortWrite(pUartCB[port],pUartRcvBuf[port],1,0);
			if(num != 1)
			{
				UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
			}
			HAL_UART_Receive_IT(phuart[port],pUartRcvBuf[port],1);
		}
	}
}
// =============================================================================
// 功能：发生错误回调函数，例如发生奇偶校验错误、硬件错误等HAL会调用此函数
// 参数：huart，HAL提供的UARTHandler
// 返回：无
// =============================================================================
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	u8 port;
	for(port = CN_UART1;port < CN_UART_NUM; port ++)
	{
		if(huart == phuart[port])
			break;
	}
	if(port < CN_UART_NUM)
	{
		UART_ErrHandle(pUartCB[port],CN_UART_HARD_COMM_ERR);
		printk("Uart hard error!\r\n");
	}
}
// =============================================================================
// 功能：UART中断,若为idle中断，则从DMA缓冲区中读出数据，并重新启动DMA，否则调用HAL中断
//       处理函数，最终会调用到HAL_UART_XXXXCallback（）
// 参数：串口号
// 返回：1=成功，0=失败
// =============================================================================
u32 UART_ISR(ptu32_t port)
{
	u32 num;
    tagUartReg volatile *Reg;
    u8 *src,temp;

    if(port < CN_UART_NUM)
    {
		Reg = sUartCfg[port].Reg;
		if( (Reg->ISR & USART_ISR_IDLE) && (sUartDmaUsed[port]) )
		{
			Reg->ICR = USART_ICR_IDLECF;	//clear the status flag

			temp = UART_RxBufLen - phuart[port]->hdmarx->Instance->CNDTR;
			num =  temp -sDmaRcvOff[port];
			src = pUartRcvBuf[port] + sDmaRcvOff[port];
			if(num != UART_PortWrite(pUartCB[port],src,num,0))
			{
				UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
			}
			sDmaRcvOff[port] = temp;
		}
		else
		{
			HAL_UART_IRQHandler(phuart[port]);
		}
    }
    return 1;
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
ptu32_t ModuleInstall_UART(u32 port)
{
	bool_t Ret = false;
    struct UartParam UART_Param;

    if(port < CN_UART_NUM)
    {
		UART_Param.Name         = sUartCfg[port].Name;
		UART_Param.UartPortTag  = (ptu32_t)sUartCfg[port].Reg;
		UART_Param.Baud         = 115200;
		UART_Param.TxRingBufLen = UART_SndBufLen;
		UART_Param.RxRingBufLen = UART_RxBufLen;
		UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
		UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;

		pUartSndBuf[port] = (u8*)malloc(UART_SndBufLen + UART_RxBufLen);
		pUartCB[port] = UART_InstallPort(&UART_Param);
		if( ( pUartCB[port] != NULL) && (pUartSndBuf[port] != NULL) )
		{
			pUartRcvBuf[port] = pUartSndBuf[port] + UART_SndBufLen;
			__UART_HardInit(port);				//硬件初始化
			__UART_IntInit(port);
			Ret = true;
		}
    }
    return Ret;
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
	u32 timeout;
	TxByteTime = 11000000/115200;

	timeout= TxByteTime * len;
	if(HAL_OK != HAL_UART_Transmit(PutStrDirectH,(u8*)str,len,timeout))
	{
		return 0;
	}
	return len;
}

// =============================================================================
// 功能：字符终端直接接收函数，采用轮询方式，直接读寄存器，用于stdin初始化前
// 参数：str，发送字符串指针
//      len，发送的字节数
// 返回：0，发生错误；result,发送数据长度，字节单位
// =============================================================================
char Uart_GetCharDirect(void)
{
	u8 result;
	while(HAL_OK != HAL_UART_Receive(GetCharDirectH,&result,1,0))
	{
		Djy_EventDelay(500);
	}
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
        TxDirectPort = CN_UART1;
    }
    else if(!strcmp(gc_pCfgStdoutName,"/dev/UART2"))
    {
        TxDirectPort = CN_UART2;
    }
    else if(!strcmp(gc_pCfgStdoutName,"/dev/UART3"))
    {
        TxDirectPort = CN_UART3;
    }
    else if(!strcmp(gc_pCfgStdoutName,"/dev/UART4"))
    {
        TxDirectPort = CN_UART4;
    }
    else if(!strcmp(gc_pCfgStdoutName,"/dev/UART5"))
    {
        TxDirectPort = CN_UART5;
    }
    else if(!strcmp(gc_pCfgStdoutName,"/dev/UART6"))
    {
        TxDirectPort = CN_UART6;
    }
    else
    {
        PutStrDirect = NULL ;
    }

    if(PutStrDirect != NULL)
    {
    	PutStrDirectH = phuart[TxDirectPort];
        __UART_HardInit(TxDirectPort);
        TxByteTime = 95;      //初始化默认115200，发送一个byte是87uS,+10%容限
        PutStrDirect = Uart_PutStrDirect;
    }

    if(!strcmp(gc_pCfgStdinName,"/dev/UART1"))
    {
        RxDirectPort = CN_UART1;
    }
    else if(!strcmp(gc_pCfgStdinName,"/dev/UART2"))
    {
        RxDirectPort = CN_UART2;
    }
    else if(!strcmp(gc_pCfgStdinName,"/dev/UART3"))
    {
        RxDirectPort = CN_UART3;
    }
    else if(!strcmp(gc_pCfgStdinName,"/dev/UART4"))
    {
        RxDirectPort = CN_UART4;
    }
    else if(!strcmp(gc_pCfgStdinName,"/dev/UART5"))
    {
        RxDirectPort = CN_UART5;
    }
    else if(!strcmp(gc_pCfgStdinName,"/dev/UART6"))
    {
        RxDirectPort = CN_UART6;
    }
    else
    {
        GetCharDirect = NULL ;
    }

    if(GetCharDirect != NULL)
    {
    	GetCharDirectH = phuart[RxDirectPort];
        if(TxDirectPort != RxDirectPort)
            __UART_HardInit(RxDirectPort);
        GetCharDirect = Uart_GetCharDirect;
    }
    return;
}



