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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��cpu_peri_uart.c
// ģ������: DJYOS����ģ��ĵײ��������֣���Ҫʵ�ּĴ�������Ĳ��������жϵ�
// ģ��汾: V1.10
// ������Ա:
// ����ʱ��:
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

extern const char *gc_pCfgStdinName;    //��׼�����豸
extern const char *gc_pCfgStdoutName;   //��׼����豸
//extern const char *gc_pCfgStderrName;   //��׼��������豸

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

static u32 TxByteTime;                    //��������һ���ֽ�����Ҫ��ʱ��
static UART_HandleTypeDef *PutStrDirectH;       //����printk���͵Ĵ���
static UART_HandleTypeDef *GetCharDirectH;      //����ֱ�ӽ��յĴ���
static u32 TxDirectPort;                  //����printk���͵Ĵ��ں�
static u32 RxDirectPort;                  //����ֱ�ӽ��յĴ��ں�
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
	    {//ʹ�ܴ���1ʱ��
	    case CN_UART1:      __HAL_RCC_USART1_CLK_ENABLE();        break; //ʹ�ܴ���1ʱ��
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
// ����: ���ô���baud,PCLKΪ25M��CPU��ƵΪ216M����,������ֻ�ṩ9600��19200��57600��
//       115200���ֲ���������
// ����: Reg,�������ļĴ�����ָ��
//       port,���ں�
//       baud,������
// ����: ��
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
        TxByteTime = 11000000/baud;     //1���ֽڴ���ʱ�䣬��10bit��+10%����
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
// ����: �йش��ڲ����ĳ�ʼ���������ʣ�ֹͣλ����żУ��λ
// ������port,���ں�
//      data������
// ����: ��
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
// ����: ��ʼ��UART��Ӧ���ж��ߣ�����ʼ���ж���ں���
// ������SerialNo,���ں�
// ����: ��
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
// ����: Ӳ���������úͼĴ����ĳ�ʼ�������������ʡ�ֹͣλ��У��λ������λ��Ĭ�������:
//       ������:115200  �� ֹͣλ:1 ; У��:�� ; ����λ:8bit
// ����: SerialNo,���ںţ�0~3
// ����: ��
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
// ����: �������ڷ��ͣ��������������ʹ��DMA��ʹ��DMA
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
// ����: ���͵ĸ���
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
// ���ܣ������Ƿ�ʹ��DMA���䣬
// ������port��ʹ�õĴ��ں�
//      enable,1 or 0,ʹ�ܻ����
// ���أ���
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
// ����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
//       cmd,��������
//       data1,data2,������cmd����
// ����: ������.
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
        case CN_UART_HALF_DUPLEX_SEND: //��������
        	Board_UartHalfDuplexSend(port);
        	break;
        case CN_UART_HALF_DUPLEX_RECV: //��������
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
// ���ܣ�������ɻص���������HAL�жϺ������ã���DMA�жϵ��ô˺������ɱ�Ȼ�����õĴ��ͳ���
//      ��ɣ���������ɣ������÷���ING���Ϊfalse
// ������huart��HAL�ṩ��UARTHandler
// ���أ���
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
// ���ܣ�������ɻص�����������DMA������ɣ���HAL�жϺ������ã���DMA�жϵ��ô˺������ɱ�Ȼ
//      �����õĴ��ͳ�����ɣ�����������е�num == UART_RxBufLen
// ������huart��HAL�ṩ��UARTHandler
// ���أ���
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

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)//�봫���ж�
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
// ���ܣ���������ص����������緢����żУ�����Ӳ�������HAL����ô˺���
// ������huart��HAL�ṩ��UARTHandler
// ���أ���
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
// ���ܣ�UART�ж�,��Ϊidle�жϣ����DMA�������ж������ݣ�����������DMA���������HAL�ж�
//       �����������ջ���õ�HAL_UART_XXXXCallback����
// ���������ں�
// ���أ�1=�ɹ���0=ʧ��
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
// ���ܣ���ʼ��uart1ģ��,��װ�ش����豸�������uart.c��API�������ú�����Ҫ��ɵĹ���
//       ���£�
//       1.��ʼ�����ڵ�Ӳ�����֣���GPIO���жϡ��������õ�COM����������baud = 115200
//         stop bits = 1,����żУ��, data bits = 8
//       2.����uart.c��API����UART_InstallPort��װ�봮���豸
// ���������ںţ�0~3
// ���أ�1=�ɹ���0=ʧ��
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
			__UART_HardInit(port);				//Ӳ����ʼ��
			__UART_IntInit(port);
			Ret = true;
		}
    }
    return Ret;
}

// =============================================================================
// ���ܣ��ַ��ն�ֱ�ӷ��ͺ�����������ѯ��ʽ��ֱ��д�Ĵ���������printk������stdout
//       û�г�ʼ��
// ������str�������ַ���ָ��
//      len�����͵��ֽ���
// ���أ�0����������result,�������ݳ��ȣ��ֽڵ�λ
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
// ���ܣ��ַ��ն�ֱ�ӽ��պ�����������ѯ��ʽ��ֱ�Ӷ��Ĵ���������stdin��ʼ��ǰ
// ������str�������ַ���ָ��
//      len�����͵��ֽ���
// ���أ�0����������result,�������ݳ��ȣ��ֽڵ�λ
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

//----��ʼ���ں˼�IO-----------------------------------------------------------
//���ܣ���ʼ���ں˼��������������runtime����ָ�롣
//��������
//���أ���
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
        TxByteTime = 95;      //��ʼ��Ĭ��115200������һ��byte��87uS,+10%����
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



