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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
#include <device/djy_uart.h>
#include "int.h"
#include "djyos.h"
#include "cpu_peri_uart.h"
#include "cpu_peri_int_line.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
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
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip uart"//CPU��uart��������
//parent:"uart device file"      //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","lock","uart device file","heap"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_UART == false )
//#warning  " cpu_onchip_uart  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_UART    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,32,512,
#define CFG_UART_SENDBUF_LEN            32                  //"���ڷ��ͻ��λ�������С",
#define CFG_UART_RECVBUF_LEN            32                  //"���ڽ��ջ��λ�������С",

//%$#@enum,,false,true
#define CFG_UART1_ENABLE                true                //"�����Ƿ�ʹ��UART1",
#define CFG_UART2_ENABLE                false               //"�����Ƿ�ʹ��UART2",
#define CFG_UART3_ENABLE                false               //"�����Ƿ�ʹ��UART3",
#define CFG_UART4_ENABLE                false               //"�����Ƿ�ʹ��UART4",
#define CFG_UART5_ENABLE                false               //"�����Ƿ�ʹ��UART5",
#define CFG_UART6_ENABLE                false               //"�����Ƿ�ʹ��UART6",
#define CFG_UART7_ENABLE                false               //"�����Ƿ�ʹ��UART7",
#define CFG_UART8_ENABLE                false               //"�����Ƿ�ʹ��UART8",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

#define tagUartReg USART_TypeDef

// =============================================================================
typedef struct
{
    char* Name;
    tagUartReg volatile *Reg;
    u8 TxRxIntLine;
    u8 DmaRxIntLine;
    u8 DmaTxIntLine;
    uint32_t DmaRxReq;
    uint32_t DmaTxReq;
    DMA_Stream_TypeDef * DmaRxCh;
    DMA_Stream_TypeDef * DmaTxCh;
}tagUartCfg;

static const tagUartCfg sUartCfg[CN_UART_NUM] = {
        {   .Name         =  "UART1",
            .Reg          =  (tagUartReg volatile *)USART1_BASE,
            .TxRxIntLine  =  CN_INT_LINE_USART1,
            .DmaRxIntLine =  CN_INT_LINE_DMA1_Stream5,
            .DmaTxIntLine =  CN_INT_LINE_DMA1_Stream7,
            .DmaRxReq     =  DMA_REQUEST_USART1_RX,
            .DmaTxReq     =  DMA_REQUEST_USART1_TX,
            .DmaRxCh      =  DMA1_Stream5,
            .DmaTxCh      =  DMA1_Stream7
        },
        {   .Name         =  "UART2",
            .Reg          =  (tagUartReg volatile *)USART2_BASE,
            .TxRxIntLine  =  CN_INT_LINE_USART2,
            .DmaRxIntLine =  CN_INT_LINE_DMA1_Stream5,
            .DmaTxIntLine =  CN_INT_LINE_DMA1_Stream7,
            .DmaRxReq     =  DMA_REQUEST_USART2_RX,
            .DmaTxReq     =  DMA_REQUEST_USART2_TX,
            .DmaRxCh      =  DMA1_Stream5,
            .DmaTxCh      =  DMA1_Stream7
        },
        {   .Name         =  "UART3",
            .Reg          =  (tagUartReg volatile *)USART3_BASE,
            .TxRxIntLine  =  CN_INT_LINE_USART3,
            .DmaRxIntLine =  CN_INT_LINE_DMA1_Stream5,
            .DmaTxIntLine =  CN_INT_LINE_DMA1_Stream7,
            .DmaRxReq     =  DMA_REQUEST_USART3_RX,
            .DmaTxReq     =  DMA_REQUEST_USART3_TX,
            .DmaRxCh      =  DMA1_Stream5,
            .DmaTxCh      =  DMA1_Stream7
        },
        {   .Name         =  "UART4",
            .Reg          =  (tagUartReg volatile *)UART4_BASE,
            .TxRxIntLine  =  CN_INT_LINE_UART4,
            .DmaRxIntLine =  CN_INT_LINE_DMA1_Stream5,
            .DmaTxIntLine =  CN_INT_LINE_DMA1_Stream7,
            .DmaRxReq     =  DMA_REQUEST_UART4_RX,
            .DmaTxReq     =  DMA_REQUEST_UART4_TX,
            .DmaRxCh      =  DMA1_Stream5,
            .DmaTxCh      =  DMA1_Stream7
        },
        {   .Name         =  "UART5",
            .Reg          =  (tagUartReg volatile *)UART5_BASE,
            .TxRxIntLine  =  CN_INT_LINE_UART5,
            .DmaRxIntLine =  CN_INT_LINE_DMA1_Stream5,
            .DmaTxIntLine =  CN_INT_LINE_DMA1_Stream7,
            .DmaRxReq     =  DMA_REQUEST_UART5_RX,
            .DmaTxReq     =  DMA_REQUEST_UART5_TX,
            .DmaRxCh      =  DMA1_Stream5,
            .DmaTxCh      =  DMA1_Stream7
        },
        {   .Name         =  "UART6",
            .Reg          =  (tagUartReg volatile *)USART6_BASE,
            .TxRxIntLine  =  CN_INT_LINE_USART6,
            .DmaRxIntLine =  CN_INT_LINE_DMA1_Stream5,
            .DmaTxIntLine =  CN_INT_LINE_DMA1_Stream7,
            .DmaRxReq     =  DMA_REQUEST_USART6_RX,
            .DmaTxReq     =  DMA_REQUEST_USART6_TX,
            .DmaRxCh      =  DMA1_Stream5,
            .DmaTxCh      =  DMA1_Stream7
        },
        {   .Name         =  "UART7",
            .Reg          =  (tagUartReg volatile *)UART7_BASE,
            .TxRxIntLine  =  CN_INT_LINE_UART7,
            .DmaRxIntLine =  CN_INT_LINE_DMA1_Stream5,
            .DmaTxIntLine =  CN_INT_LINE_DMA1_Stream7,
            .DmaRxReq     =  DMA_REQUEST_UART7_RX,
            .DmaTxReq     =  DMA_REQUEST_UART7_TX,
            .DmaRxCh      =  DMA1_Stream5,
            .DmaTxCh      =  DMA1_Stream7
        },
        {   .Name         =  "UART8",
            .Reg          =  (tagUartReg volatile *)UART8_BASE,
            .TxRxIntLine  =  CN_INT_LINE_UART8,
            .DmaRxIntLine =  CN_INT_LINE_DMA1_Stream5,
            .DmaTxIntLine =  CN_INT_LINE_DMA1_Stream7,
            .DmaRxReq     =  DMA_REQUEST_UART8_RX,
            .DmaTxReq     =  DMA_REQUEST_UART8_TX,
            .DmaRxCh      =  DMA1_Stream5,
            .DmaTxCh      =  DMA1_Stream7
        },
};
// =============================================================================

//#define CFG_UART_SENDBUF_LEN    32
//#define CFG_UART_RECVBUF_LEN     32

static u32 TxByteTime;                    //��������һ���ֽ�����Ҫ��ʱ��
static UART_HandleTypeDef *PutStrDirectH;       //����printk���͵Ĵ���
static UART_HandleTypeDef *GetCharDirectH;      //����ֱ�ӽ��յĴ���
static u32 TxDirectPort;                  //����printk���͵Ĵ��ں�
static u32 RxDirectPort;                  //����ֱ�ӽ��յĴ��ں�
// =============================================================================

static struct UartGeneralCB *pUartCB[CN_UART_NUM];
static u8* pUartSndBuf[CN_UART_NUM] = {NULL};
static u8* pUartRcvBuf[CN_UART_NUM] = {NULL};
static bool_t sUartDmaUsed[] = {false,false,false,false,false,false};
static bool_t sUartSending[] = {false,false,false,false,false,false};
static u8 sDmaRcvOff[CN_UART_NUM] = {0};

static UART_HandleTypeDef sHuart[CN_UART_NUM];
//static UART_HandleTypeDef *phuart[CN_UART_NUM] = {
//        &sHuart[0],&sHuart[1],&sHuart[2],&sHuart[3],&sHuart[4],&sHuart[5]};

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
        HAL_UART_Receive_DMA(&sHuart[port],pUartRcvBuf[port],CFG_UART_RECVBUF_LEN);
    }
    else
    {
        HAL_UART_Receive_IT(&sHuart[port],pUartRcvBuf[port],1);
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
        case CN_UART4:      __HAL_RCC_UART4_CLK_ENABLE();         break;
        case CN_UART5:      __HAL_RCC_UART5_CLK_ENABLE();         break;
        case CN_UART6:      __HAL_RCC_USART6_CLK_ENABLE();        break;
        case CN_UART7:      __HAL_RCC_UART7_CLK_ENABLE();         break;
        case CN_UART8:      __HAL_RCC_UART8_CLK_ENABLE();         break;
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
        case CN_UART6:      __HAL_RCC_USART6_CLK_DISABLE();        break;
        case CN_UART7:      __HAL_RCC_UART7_CLK_DISABLE();        break;
        case CN_UART8:      __HAL_RCC_UART8_CLK_DISABLE();        break;
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
        sHuart[port].Init.BaudRate = baud;
        HAL_UART_Abort(&sHuart[port]);
        UART_SetConfig(&sHuart[port]);
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
    HAL_UART_Abort(&sHuart[port]);
    __UART_StartRecv(port);
}
static void __UART_Stop(u8 port)
{
    HAL_UART_Abort(&sHuart[port]);
    __UART_RCC_Enable(port,0);
}

// =============================================================================
// ����: �йش��ڲ����ĳ�ʼ���������ʣ�ֹͣλ����żУ��λ
// ������port,���ں�
//      data������
// ����: ��
// =============================================================================
static void __UART_ComConfig(u32 port, struct COMParam *COM)
{
    if(port < CN_UART_NUM)
    {
        sHuart[port].Init.BaudRate = COM->BaudRate;
        switch(COM->DataBits)               // data bits
        {
        case CN_UART_DATABITS_8: sHuart[port].Init.WordLength = UART_WORDLENGTH_8B;break;
        case CN_UART_DATABITS_9: sHuart[port].Init.WordLength = UART_WORDLENGTH_9B;break;
        default:break;
        }

        switch(COM->Parity)                 // parity
        {
        case CN_UART_PARITY_NONE:sHuart[port].Init.Parity = UART_PARITY_NONE;break;
        case CN_UART_PARITY_ODD: sHuart[port].Init.Parity = UART_PARITY_ODD;break;
        case CN_UART_PARITY_EVEN:sHuart[port].Init.Parity = UART_PARITY_EVEN;break;
        default:break;
        }

        switch(COM->StopBits)               // stop bits
        {
        case CN_UART_STOPBITS_1:  sHuart[port].Init.StopBits = UART_STOPBITS_1;break;
        case CN_UART_STOPBITS_1_5:sHuart[port].Init.StopBits = UART_STOPBITS_1_5;break;
        case CN_UART_STOPBITS_2:  sHuart[port].Init.StopBits = UART_STOPBITS_2;break;
        default:break;
        }
        HAL_UART_Abort(&sHuart[port]);
        UART_SetConfig(&sHuart[port]);
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
        if(huart == &sHuart[port])
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
        sHuart[port].Instance                    = sUartCfg[port].Reg;
        sHuart[port].Init.BaudRate               = 115200;
        sHuart[port].Init.WordLength             = UART_WORDLENGTH_8B;
        sHuart[port].Init.Parity                 = UART_PARITY_NONE;
        sHuart[port].Init.StopBits               = UART_STOPBITS_1;
        sHuart[port].Init.Mode                   = UART_MODE_TX_RX;
        sHuart[port].Init.HwFlowCtl              = UART_HWCONTROL_NONE;
        sHuart[port].Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_ENABLE;
        sHuart[port].Init.OverSampling           = UART_OVERSAMPLING_16;
        sHuart[port].AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

        HAL_UART_DeInit(&sHuart[port]);
        HAL_UART_Init(&sHuart[port]);
        __UART_StartRecv(port);
    }
}

// =============================================================================
// ����: �������ڷ��ͣ��������������ʹ��DMA��ʹ��DMA
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
// ����: ���͵ĸ���
// =============================================================================
static u32 __UART_SendStart (u16 port)
{
    u16 len=0;
    HAL_StatusTypeDef Ret;

    Board_UartHalfDuplexSend(port);
    if(port < CN_UART_NUM)
    {
        if(!sUartSending[port])
        {
            len = UART_PortRead(pUartCB[port],pUartSndBuf[port],CFG_UART_SENDBUF_LEN);
            if(len > 0)
            {
                sUartSending[port] = true;
                if(sUartDmaUsed[port])
                {
                    Ret = HAL_UART_Transmit_DMA(&sHuart[port],pUartSndBuf[port],len);
                }
                else
                {
                    Ret = HAL_UART_Transmit_IT(&sHuart[port],pUartSndBuf[port],len);
                }
                if(Ret != HAL_OK)
                {
                    sUartSending[port] = false;
                    Board_UartHalfDuplexRecv(port);
                }
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
        HAL_UART_Abort(&sHuart[port]);

        if(sHuart[port].hdmarx == NULL)
        {
            sHuart[port].hdmarx = (DMA_HandleTypeDef*)malloc(2*sizeof(DMA_HandleTypeDef));
            if(NULL == sHuart[port].hdmarx)
                return;
            sHuart[port].hdmatx = sHuart[port].hdmarx + 1;
        }

        sHuart[port].hdmarx->Instance = sUartCfg[port].DmaRxCh;
        sHuart[port].hdmarx->Init.Direction = DMA_PERIPH_TO_MEMORY;
        sHuart[port].hdmarx->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        sHuart[port].hdmarx->Init.MemInc = DMA_MINC_ENABLE;
        sHuart[port].hdmarx->Init.Mode = DMA_CIRCULAR;
        sHuart[port].hdmarx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        sHuart[port].hdmarx->Init.PeriphInc = DMA_PINC_DISABLE;
        sHuart[port].hdmarx->Init.Priority = DMA_PRIORITY_HIGH;
        sHuart[port].hdmarx->Init.Request = sUartCfg[port].DmaRxReq;
        sHuart[port].hdmarx->XferCpltCallback = 0;
        sHuart[port].hdmarx->XferHalfCpltCallback = 0;
        sHuart[port].hdmarx->XferErrorCallback = 0;
        sHuart[port].hdmarx->Parent = (void*)&sHuart[port];

        sHuart[port].hdmatx->Instance = sUartCfg[port].DmaTxCh;
        sHuart[port].hdmatx->Init.Direction = DMA_MEMORY_TO_PERIPH;
        sHuart[port].hdmatx->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        sHuart[port].hdmatx->Init.MemInc = DMA_MINC_ENABLE;
        sHuart[port].hdmatx->Init.Mode = DMA_NORMAL;
        sHuart[port].hdmatx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        sHuart[port].hdmatx->Init.PeriphInc = DMA_PINC_DISABLE;
        sHuart[port].hdmatx->Init.Priority = DMA_PRIORITY_LOW;
        sHuart[port].hdmatx->Init.Request = sUartCfg[port].DmaTxReq;
        sHuart[port].hdmatx->XferCpltCallback = 0;
        sHuart[port].hdmatx->XferErrorCallback = 0;
        sHuart[port].hdmatx->XferHalfCpltCallback = 0;
        sHuart[port].hdmatx->Parent = (void*)&sHuart[port];
        HAL_DMA_Init(sHuart[port].hdmatx);
        HAL_DMA_Init(sHuart[port].hdmarx);

        sUartCfg[port].Reg->CR1 |= USART_CR1_IDLEIE;//enable idle int
        sUartDmaUsed[port] = true;

        IntLine = sUartCfg[port].DmaRxIntLine;
        Int_Register(IntLine);
        Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
        Int_IsrConnect(IntLine,(u32(*)(ptu32_t))HAL_DMA_IRQHandler);
        Int_SetIsrPara(IntLine,(ptu32_t)(sHuart[port].hdmarx));
        Int_SettoAsynSignal(IntLine);
        Int_ClearLine(IntLine);
        Int_RestoreAsynLine(IntLine);

        IntLine = sUartCfg[port].DmaTxIntLine;
        Int_Register(IntLine);
        Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
        Int_IsrConnect(IntLine,(u32(*)(ptu32_t))HAL_DMA_IRQHandler);
        Int_SetIsrPara(IntLine,(ptu32_t)(sHuart[port].hdmatx));
        Int_SettoAsynSignal(IntLine);
        Int_ClearLine(IntLine);
        Int_RestoreAsynLine(IntLine);
    }
    else if( (sUartDmaUsed[port]) && (!enable) )
    {
        HAL_UART_Abort(&sHuart[port]);
        free(sHuart[port].hdmarx);
        sHuart[port].hdmarx = NULL;
        sHuart[port].hdmatx = NULL;

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
static ptu32_t __UART_Ctrl(u32 port,u32 cmd, va_list *arg0)
{
    ptu32_t result = 0;
    u32 timeout = 100000;

    if(port < CN_UART_NUM)
    {
        switch(cmd)
        {
        case CN_DEV_CTRL_START:
            __UART_Start(port);
            break;
        case CN_DEV_CTRL_STOP:
            __UART_Stop(port);
            break;
        case CN_UART_SET_BAUD:
        {
            u32 data;
            data = va_arg(*arg0, u32);
            __UART_BaudSet(port, data);
        }
            break;
        case CN_UART_COM_SET:
        {
            struct COMParam *COM;
            COM = va_arg(*arg0, void *);
            __UART_ComConfig(port,COM);
        }
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
        if(huart == &sHuart[port])
            break;
    }
    if(port < CN_UART_NUM)
    {
        len = UART_PortRead(pUartCB[port],pUartSndBuf[port],CFG_UART_SENDBUF_LEN);
        if(len > 0)
        {
            if(sUartDmaUsed[port])
                HAL_UART_Transmit_DMA(&sHuart[port],pUartSndBuf[port],len);
            else
                HAL_UART_Transmit_IT(&sHuart[port],pUartSndBuf[port],len);
        }
        else
        {
            Board_UartHalfDuplexRecv(port);
            sUartSending[port] = false;
        }
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
        if(huart == &sHuart[port])
            break;
    }
    if(port < CN_UART_NUM)
    {
        if(sUartDmaUsed[port])
        {
            num = CFG_UART_RECVBUF_LEN - sDmaRcvOff[port];
            src = pUartRcvBuf[port] + sDmaRcvOff[port];
            if(num != UART_PortWrite(pUartCB[port],src,num))
            {
                UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
            }
            sDmaRcvOff[port] = 0;
        }
        else
        {
            num = UART_PortWrite(pUartCB[port],pUartRcvBuf[port],1);
            if(num != 1)
            {
                UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
            }
            HAL_UART_Receive_IT(&sHuart[port],pUartRcvBuf[port],1);
        }
    }
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)//�봫���ж�
{
    u8 port,num;
    u8 *src;
    for(port = CN_UART1;port < CN_UART_NUM; port ++)
    {
        if(huart == &sHuart[port])
            break;
    }
    if(port < CN_UART_NUM)
    {
        if(sUartDmaUsed[port])
        {
            num = CFG_UART_RECVBUF_LEN/2 - sDmaRcvOff[port];
            src = pUartRcvBuf[port] + sDmaRcvOff[port];
            if(num != UART_PortWrite(pUartCB[port],src,num))
            {
                UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
            }
            sDmaRcvOff[port] = CFG_UART_RECVBUF_LEN/2;
        }
        else
        {
            num = UART_PortWrite(pUartCB[port],pUartRcvBuf[port],1);
            if(num != 1)
            {
                UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
            }
            HAL_UART_Receive_IT(&sHuart[port],pUartRcvBuf[port],1);
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
        if(huart == &sHuart[port])
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

    if(port >= CN_UART_NUM)
        return 0;

    Reg = sUartCfg[port].Reg;
    if( (Reg->ISR & USART_ISR_IDLE) && (sUartDmaUsed[port]) )
    {
        Reg->ICR = USART_ICR_IDLECF;    //clear the status flag

        temp = CFG_UART_RECVBUF_LEN - ((BDMA_Channel_TypeDef *)(sHuart[port].hdmarx->Instance))->CNDTR;
        num =  temp -sDmaRcvOff[port];
        src = pUartRcvBuf[port] + sDmaRcvOff[port];
        if(num != UART_PortWrite(pUartCB[port],src,num))
        {
            UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
        }
        sDmaRcvOff[port] = temp;
    }
    else
    {
        HAL_UART_IRQHandler(&sHuart[port]);
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
    struct UartParam UART_Param;

    if(port >= CN_UART_NUM)
        return false;
    UART_Param.Name         = sUartCfg[port].Name;
    UART_Param.UartPortTag  = (ptu32_t)port;
    UART_Param.Baud         = 115200;
    UART_Param.TxRingBufLen = CFG_UART_SENDBUF_LEN;
    UART_Param.RxRingBufLen = CFG_UART_RECVBUF_LEN;
    UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
    UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
    UART_Param.mode         = CN_UART_GENERAL;

    pUartSndBuf[port] = (u8*)malloc(CFG_UART_SENDBUF_LEN + CFG_UART_RECVBUF_LEN);
    pUartCB[port] = UART_InstallGeneral(&UART_Param);

    if( ( pUartCB[port] != NULL) && (pUartSndBuf[port] != NULL) )
    {
        pUartRcvBuf[port] = pUartSndBuf[port] + CFG_UART_SENDBUF_LEN;
        __UART_HardInit(port);              //Ӳ����ʼ��
        __UART_IntInit(port);
        return true;
    }
    return false;
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
        DJY_EventDelay(500);
    }
    return result;
}

//----��ʼ���ں˼�IO-----------------------------------------------------------
//���ܣ���ʼ���ں˼��������������runtime����ָ�롣
//��������
//���أ���
//-----------------------------------------------------------------------------
void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut)
{
    if(!strcmp(StdioOut,"/dev/UART1"))
    {
        TxDirectPort = CN_UART1;
        PutStrDirectH = &sHuart[TxDirectPort];
    }
    else if(!strcmp(StdioOut,"/dev/UART2"))
    {
        TxDirectPort = CN_UART2;
        PutStrDirectH = &sHuart[TxDirectPort];
    }
    else if(!strcmp(StdioOut,"/dev/UART3"))
    {
        TxDirectPort = CN_UART3;
        PutStrDirectH = &sHuart[TxDirectPort];
    }
    else if(!strcmp(StdioOut,"/dev/UART4"))
    {
        TxDirectPort = CN_UART4;
        PutStrDirectH = &sHuart[TxDirectPort];
    }
    else if(!strcmp(StdioOut,"/dev/UART5"))
    {
        TxDirectPort = CN_UART5;
        PutStrDirectH = &sHuart[TxDirectPort];
    }
    else if(!strcmp(StdioOut,"/dev/UART6"))
    {
        TxDirectPort = CN_UART6;
        PutStrDirectH = &sHuart[TxDirectPort];
    }
    else
    {
//        PutStrDirect = NULL ;
        PutStrDirectH = NULL;
    }

    if(PutStrDirectH != NULL)
    {
//        PutStrDirectH = &sHuart[TxDirectPort];
        __UART_HardInit(TxDirectPort);
        TxByteTime = 95;      //��ʼ��Ĭ��115200������һ��byte��87uS,+10%����
        PutStrDirect = Uart_PutStrDirect;
    }

    if(!strcmp(StdioIn,"/dev/UART1"))
    {
        RxDirectPort = CN_UART1;
        GetCharDirectH = &sHuart[RxDirectPort];
    }
    else if(!strcmp(StdioIn,"/dev/UART2"))
    {
        RxDirectPort = CN_UART2;
        GetCharDirectH = &sHuart[RxDirectPort];
    }
    else if(!strcmp(StdioIn,"/dev/UART3"))
    {
        RxDirectPort = CN_UART3;
        GetCharDirectH = &sHuart[RxDirectPort];
    }
    else if(!strcmp(StdioIn,"/dev/UART4"))
    {
        RxDirectPort = CN_UART4;
        GetCharDirectH = &sHuart[RxDirectPort];
    }
    else if(!strcmp(StdioIn,"/dev/UART5"))
    {
        RxDirectPort = CN_UART5;
        GetCharDirectH = &sHuart[RxDirectPort];
    }
    else if(!strcmp(StdioIn,"/dev/UART6"))
    {
        RxDirectPort = CN_UART6;
        GetCharDirectH = &sHuart[RxDirectPort];
    }
    else
    {
//        GetCharDirect = NULL ;
        GetCharDirectH = NULL;
    }

    if(GetCharDirectH != NULL)
    {
//        GetCharDirectH = &sHuart[RxDirectPort];
        if(TxDirectPort != RxDirectPort)
            __UART_HardInit(RxDirectPort);
        GetCharDirect = Uart_GetCharDirect;
    }
    return;
}



