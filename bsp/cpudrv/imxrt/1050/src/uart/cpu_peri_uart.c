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
// ģ��汾: V1.0.0
// ������Ա: chj
// ����ʱ��: 2018-05-07
// =============================================================================
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "errno.h"
#include "systime.h"
#include "cpu_peri.h"
#include "cpu_peri_int_line.h"
#include <device/djy_uart.h>
#include "device.h"
#include "int.h"
#include "djyos.h"
#include "stdlib.h"
#include "cpu_peri_uart.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern ptu32_t ModuleInstall_UART(u8 port);
//    #if (CFG_UART1_ENABLE == 1)
//    ModuleInstall_UART(CN_UART1);
//    #endif
//    #if (CFG_UART2_ENABLE == 1)
//    ModuleInstall_UART(CN_UART2);
//    #endif
//    #if (CFG_UART3_ENABLE == 1)
//    ModuleInstall_UART(CN_UART3);
//    #endif
//    #if (CFG_UART4_ENABLE == 1)
//    ModuleInstall_UART(CN_UART4);
//    #endif
//    #if (CFG_UART5_ENABLE == 1)
//    ModuleInstall_UART(CN_UART5);
//    #endif
//    #if (CFG_UART6_ENABLE == 1)
//    ModuleInstall_UART(CN_UART6);
//    #endif
//    #if (CFG_UART7_ENABLE == 1)
//    ModuleInstall_UART(CN_UART7);
//    #endif
//    #if (CFG_UART8_ENABLE == 1)
//    ModuleInstall_UART(CN_UART8);
//    #endif

//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip uart"//CPU��uart��������
//parent:"uart device file"      //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                  //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                 //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                 //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"uart device file","heap","lock","int"//������������������������none����ʾ�������������
                                 //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                 //����������������������г����á�,���ָ�
//weakdependence:none            //��������������������������none����ʾ�������������
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
#define CFG_UART1_SENDBUF_LEN    64             //"UART1���ͻ����С",
#define CFG_UART1_RECVBUF_LEN    64             //"UART1���ջ����С",

#define CFG_UART2_SENDBUF_LEN    64             //"UART2���ͻ����С",
#define CFG_UART2_RECVBUF_LEN    64             //"UART2���ջ����С",

#define CFG_UART3_SENDBUF_LEN    64             //"UART3���ͻ����С",
#define CFG_UART3_RECVBUF_LEN    64             //"UART3���ջ����С",

#define CFG_UART4_SENDBUF_LEN    64             //"UART4���ͻ����С",
#define CFG_UART4_RECVBUF_LEN    64             //"UART4���ջ����С",

#define CFG_UART5_SENDBUF_LEN    64             //"UART5���ͻ����С",
#define CFG_UART5_RECVBUF_LEN    64             //"UART5���ջ����С",

#define CFG_UART6_SENDBUF_LEN    64             //"UART6���ͻ����С",
#define CFG_UART6_RECVBUF_LEN    64             //"UART6���ջ����С",

#define CFG_UART7_SENDBUF_LEN    64             //"UART7���ͻ����С",
#define CFG_UART7_RECVBUF_LEN    64             //"UART7���ջ����С",

#define CFG_UART8_SENDBUF_LEN    64             //"UART8���ͻ����С",
#define CFG_UART8_RECVBUF_LEN    64             //"UART8���ջ����С",
//%$#@enum,true,false,
#define CFG_UART1_ENABLE         true           //"����ʹ��UART1",
#define CFG_UART2_ENABLE         false          //"����ʹ��UART2",
#define CFG_UART3_ENABLE         false          //"����ʹ��UART3",
#define CFG_UART4_ENABLE         false          //"����ʹ��UART4",
#define CFG_UART5_ENABLE         false          //"����ʹ��UART5",
#define CFG_UART6_ENABLE         false          //"����ʹ��UART6",
#define CFG_UART7_ENABLE         false          //"����ʹ��UART7",
#define CFG_UART8_ENABLE         false          //"����ʹ��UART8",
//%$#@string,1,30,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
//==============================================================================

//==============================================================================
extern u32 BOARD_DebugConsoleSrcFreq(void);//�ṩ�������ʱ��Ƶ��
#define SRC_FREQ    BOARD_DebugConsoleSrcFreq()
// =============================================================================
#define CN_UART1_BASE (0x40184000u)
#define CN_UART2_BASE (0x40188000u)
#define CN_UART3_BASE (0x4018C000u)
#define CN_UART4_BASE (0x40190000u)
#define CN_UART5_BASE (0x40194000u)
#define CN_UART6_BASE (0x40198000u)
#define CN_UART7_BASE (0x4019C000u)
#define CN_UART8_BASE (0x401A0000u)

static u32 TxByteTime;                    //��������һ���ֽ�����Ҫ��ʱ��
static tagUartReg *PutStrDirectReg;     //����printk���͵Ĵ��ڼĴ���
static tagUartReg *GetCharDirectReg;     //����ֱ�ӽ��յĴ��ڼĴ���
static u32 TxDirectPort;                  //����printk���͵Ĵ��ں�
static u32 RxDirectPort;                  //����ֱ�ӽ��յĴ��ں�
// =============================================================================
static tagUartReg volatile * const tg_UART_Reg[] = {(LPUART_Type *)CN_UART1_BASE,
                                                     (LPUART_Type *)CN_UART2_BASE,
                                                     (LPUART_Type *)CN_UART3_BASE,
                                                     (LPUART_Type *)CN_UART4_BASE,
                                                     (LPUART_Type *)CN_UART5_BASE,
                                                     (LPUART_Type *)CN_UART6_BASE,
                                                     (LPUART_Type *)CN_UART7_BASE,
                                                     (LPUART_Type *)CN_UART8_BASE};

static lpuart_config_t tg_UART_Config[CN_UART_NUM];

#define CN_DMA_UNUSED           0U
#define CN_DMA_USED             1U

#define CN_UART_TX_RMPTY        2U
#define CN_UART_TX_COMPLETE     3U

static struct UartGeneralCB *pUartCB[CN_UART_NUM];
static u8 sUartInited = 0;
__attribute__((weak))  void Board_UartHalfDuplexSend(u8 port)
{
    return;
}
__attribute__((weak))  void Board_UartHalfDuplexRecv(u8 port)
{
    return ;
}

//static bool_t s_UART_DmaSending[] = {false,false,false,false,false,false};
static bool_t s_UART_DmaUsed[] = {false,false,false,false,false,false};

u32 UART_ISR(ptu32_t port);
// =============================================================================
// ����: ʹ��uart�Ľ����жϡ�
// ����: flag,�Ƿ�ʹ��dma����
//       port,���ںţ�1~8
// ����: ��
// =============================================================================
void __UART_RxIntEnable(u8 flag,u8 port)
{
    if(flag == CN_DMA_USED){}
    else
        LPUART_EnableInterrupts((tagUartReg *)tg_UART_Reg[port], \
                kLPUART_RxDataRegFullInterruptEnable);
}

// =============================================================================
// ����: ��ֹuart�Ľ����ж�
// ����: flag,�Ƿ�ʹ��dma����
//       port,���ں�
// ����: ��
// =============================================================================
void __UART_RxIntDisable(u8 flag, u8 port)
{
    if(flag == CN_DMA_USED){}
    else
        LPUART_DisableInterrupts((tagUartReg *)tg_UART_Reg[port], \
                kLPUART_RxDataRegFullInterruptEnable);
}

// =============================================================================
// ����: ʹ��uart�ķ����ж�
// ����: flag,�Ƿ�ʹ��dma����
//       port,���ں�
//       int_flag,�ж����ͣ����ͼĴ������ж��Լ���������ж�
// ����: ��
// =============================================================================
static void __UART_TxIntEnable(u8 flag,u8 port,u8 int_flag)
{
    if(flag == CN_DMA_USED){}
    else
    {
        if(int_flag==CN_UART_TX_RMPTY)
            LPUART_EnableInterrupts((tagUartReg *)tg_UART_Reg[port], \
                    kLPUART_TxDataRegEmptyInterruptEnable);
        else if(int_flag==CN_UART_TX_COMPLETE)
            LPUART_EnableInterrupts((LPUART_Type *)tg_UART_Reg[port], \
                    kLPUART_TransmissionCompleteInterruptEnable);
    }
}

// =============================================================================
// ����: ��ֹUART�ķ����ж�
// ����: flag,�Ƿ�ʹ��dma����
//       port,���ں�
//      int_flag,�ж����ͣ����ͼĴ������ж��Լ���������ж�
// ����: ��
// =============================================================================
static void __UART_TxIntDisable(u8 flag, u8 port,u8 int_flag)
{
    if(flag == CN_DMA_USED){}
    else
    {
        if(int_flag==CN_UART_TX_RMPTY)
            LPUART_DisableInterrupts((tagUartReg *)tg_UART_Reg[port], \
                    kLPUART_TxDataRegEmptyInterruptEnable);
        else if(int_flag==CN_UART_TX_COMPLETE)
            LPUART_DisableInterrupts((tagUartReg *)tg_UART_Reg[port], \
                    kLPUART_TransmissionCompleteInterruptEnable);
    }
}

// =============================================================================
// ����: ��鷢�ͼĴ�����״̬������վͷ���true����Ϊ�գ����򷵻�false
// ������port,���ں�
// ����: true = �������գ�false = �ǿ�
// =============================================================================
static bool_t __UART_TxTranEmpty(tagUartReg volatile *reg)
{
    if((kLPUART_TxFifoEmptyFlag)&\
            LPUART_GetStatusFlags((tagUartReg *)reg))
    {
        return true;
    }
    else
    {
        return false;
    }
    /*
    if (!(reg->STAT & LPUART_STAT_TDRE_MASK))
        return false;
    else
        return true;
    */
}

// =============================================================================
// ����: ��鷢�ͼĴ�����״̬�����������ɾͷ���true�����򷵻�false
// ������port,���ں�
// ����: true = ��������ɣ�false = ����δ���
// =============================================================================
static bool_t __UART_TxTranComplete(tagUartReg volatile *reg)
{
    if((kLPUART_TransmissionCompleteFlag)&\
            LPUART_GetStatusFlags((tagUartReg *)reg))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// =============================================================================
// ����: �����ռĴ�����״̬�������ݾͷ���true�����򷵻�false
// ������port,���ں�
// ����: true = �����ݣ�false = ������
// =============================================================================
static bool_t __UART_RxHadChar(tagUartReg volatile *reg)
{
    if (kLPUART_RxFifoEmptyFlag & \
                LPUART_GetStatusFlags((tagUartReg *)reg))
        return false;
    else
        return true;
}

// =============================================================================
// ����: ���ô��ڲ�����
// ����:
//       port,���ں�
//       baud,������
// ����: ��
//ע��SRC_FREQ�����ṩ���������ʱ��Ƶ�ʣ�Ӧ�ɹ��̷�����ʵ������ʱ���ṩ
// =============================================================================
static void __UART_BaudSet(u8 port,u32 baud)
{
    tg_UART_Config[port].baudRate_Bps = baud;
    LPUART_SetBaudRate((tagUartReg *)tg_UART_Reg[port], baud, \
            SRC_FREQ);
    if(TxDirectPort == port)
    {
        TxByteTime = 11000000/baud;     //1���ֽڴ���ʱ�䣬��10bit��+10%����
    }
}

// =============================================================================
// ����: �Դ��ڴ���������ã����������ʡ���żУ�顢����λ��ֹͣλ
// ����:
//        port,���ں�
//        data,�ṹ��tagCOMParam���͵�ָ����ֵ
// ����: ��
// =============================================================================
static void __UART_ComConfig(tagUartReg volatile * reg,u8 port,struct COMParam *COM)
{
    struct COMParam *COM;

    if(tg_UART_Reg[port]!=reg)
        return;
    if(COM == NULL)
        return;

     LPUART_GetDefaultConfig(&tg_UART_Config[port]);
     tg_UART_Config[port].baudRate_Bps = COM->BaudRate;
     switch(COM->DataBits)               // data bits
     {
         case CN_UART_DATABITS_7:    tg_UART_Config[port].dataBitsCount = kLPUART_SevenDataBits;
                                     break;
         case CN_UART_DATABITS_8:    tg_UART_Config[port].dataBitsCount = kLPUART_EightDataBits;
                                     break;
         default:break;
     }

     switch(COM->Parity)                 // parity
     {
         case CN_UART_PARITY_ODD:    tg_UART_Config[port].parityMode = kLPUART_ParityOdd;
                                     break;
         case CN_UART_PARITY_EVEN:   tg_UART_Config[port].parityMode = kLPUART_ParityEven;
                                     break;
         default:break;
     }

     switch(COM->StopBits)               // stop bits
     {
         case CN_UART_STOPBITS_1:    tg_UART_Config[port].stopBitCount = kLPUART_OneStopBit;
                                     break;
         case CN_UART_STOPBITS_2:    tg_UART_Config[port].stopBitCount = kLPUART_TwoStopBit;
                                     break;
         default:break;
     }
}

// =============================================================================
// ����: ʹ���ڿ�ʼ������ʱ�Ӻ�uartģ��
// ����: port,���ں�
// ����: ��
// =============================================================================
void __UART_Enable(u8 port)
{
    tg_UART_Config[port].enableTx = true;
    tg_UART_Config[port].enableRx = true;

    LPUART_Init((tagUartReg *)tg_UART_Reg[port], \
                 &tg_UART_Config[port], SRC_FREQ);
}

// =============================================================================
// ����: ʹ����ֹͣ����������ʱ�Ӻ�uartģ��
// ����: port,���ں�
// ����: ��
// =============================================================================
void __UART_Disable(u8 port)
{
    tg_UART_Config[port].enableTx = false;
    tg_UART_Config[port].enableRx = false;

    LPUART_Deinit((tagUartReg *)tg_UART_Reg[port]);
}

// =============================================================================
// ����: ��ʼ��UART��Ӧ���ж��ߣ�����ʼ���ж���ں���
// ������SerialNo,���ں�
// ����: ��
// =============================================================================
static void __UART_IntInit(u8 port)
{
    u8 IntLine;
    if(port == CN_UART1)
        IntLine = CN_INT_LINE_LPUART1;
    else if(port == CN_UART2)
        IntLine = CN_INT_LINE_LRUART2;
    else if(port == CN_UART3)
        IntLine = CN_INT_LINE_LRUART3;
    else if(port == CN_UART4)
        IntLine = CN_INT_LINE_LRUART4;
    else if(port == CN_UART5)
        IntLine = CN_INT_LINE_LPUART5;
    else if(port == CN_UART6)
        IntLine = CN_INT_LINE_LPUART6;
    else if(port == CN_UART7)
        IntLine = CN_INT_LINE_LPUART7;
    else if(port == CN_UART8)
        IntLine = CN_INT_LINE_LPUART8;
    else
        return;

    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,UART_ISR);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
    Int_SetIsrPara(IntLine,port);
}

// =============================================================================
// ����: Ӳ���������úͼĴ����ĳ�ʼ�������������ʡ�ֹͣλ��У��λ������λ��Ĭ�������:
//       ������:115200  �� ֹͣλ:1 ; У��:żУ�� ; ����λ:8bit
// ����: port,���ںţ�0~7
// ����: ��
// =============================================================================
static void __UART_HardInit(u8 port)
{
    if(port > CN_UART8)
        return;
    //ϵͳ��ʼ��ʱ�Ѿ�ʹ�жϴ��ڽ�ֹ״̬�������ٽ�ֹ������жϡ�
   //��ʼ��uartӲ���������ݽṹ
   LPUART_GetDefaultConfig(&tg_UART_Config[port]);
   tg_UART_Config[port].baudRate_Bps = 115200;
   //tg_UART_Config[port].enableTx = true;
   //tg_UART_Config[port].enableRx = true;
   //LPUART_Init(tg_UART_Reg[port], &tg_UART_Config[port],
    //     SRC_FREQ);
}

// =============================================================================
// ����: �������ڷ��ͣ���Ŀ���Ǵ����жϣ����жϷ�ʽ�������ݡ�
// ����: port:���ں�.
// ����:
// =============================================================================
static u32 __UART_SendStart (tagUartReg *reg)
{
    u8 port=0;
    //u32 delay=0;

    switch((u32)reg)
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
    Board_UartHalfDuplexSend(port);//�л�������
    __UART_TxIntDisable(s_UART_DmaUsed[port],port,\
            CN_UART_TX_RMPTY);
    if(s_UART_DmaUsed[port])
    {
        /*__UART_DMA_SendStart(port);*/
    }
    else
    {
        //��DMA��ʽ��ֻҪʹ�ܷ��Ϳ��жϣ��򴥷��ж�
    }

    __UART_TxIntEnable(s_UART_DmaUsed[port],port,\
            CN_UART_TX_RMPTY);
    return 1;
}

// =============================================================================
// ����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
// ����: port,���ں�.
//       cmd,��������
//       data1,data2,������cmd����
// ����: ������.
// =============================================================================
static ptu32_t __UART_Ctrl(tagUartReg *reg,u32 cmd, va_list *arg0)
{
    u8 port=0;
    ptu32_t result = 0;

    if(reg == NULL)
        return 0;

    switch((u32)reg)
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
            __UART_RxIntEnable(CN_DMA_UNUSED,port);
            break;
        case CN_DEV_CTRL_STOP:
            __UART_RxIntDisable(CN_DMA_UNUSED,port);
            __UART_Disable(port);
            break;
        case CN_UART_SET_BAUD:  //����Baud
        {
            u32 data;
            data = va_arg(*arg0, u32);
            __UART_BaudSet(port, data);
        }
            break;
        case CN_UART_EN_RTS:
            tg_UART_Config[port].enableRxRTS = true;
            reg->MODIR |= LPUART_MODIR_RXRTSE_MASK;
            break;
        case CN_UART_DIS_RTS:
            tg_UART_Config[port].enableRxRTS = false;
            reg->MODIR &= ~LPUART_MODIR_RXRTSE_MASK;
            break;
        case CN_UART_EN_CTS:
            tg_UART_Config[port].enableTxCTS = true;
            reg->MODIR |= LPUART_MODIR_TXCTSE_MASK;
            break;
        case CN_UART_DIS_CTS:
            tg_UART_Config[port].enableTxCTS = false;
            reg->MODIR &= ~LPUART_MODIR_TXCTSE_MASK;
            break;
/*
        case CN_UART_DMA_USED:
            __UART_SetDmaUsed(port);
            break;
        case CN_UART_DMA_UNUSED:
            __UART_SetDmaUnUsed(port);
            break;
*/
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
// ����: UART���жϷ��������������Ϳա����ա����������жϡ�
//      1.�����жϣ��ӽ���FIFO�ж����ݣ�������UART_PortWriteд����ջ������У�
//      2.�����жϣ�����UART_PortRead�����ͻ����������ݣ���д�뷢��FIFO��
//      3.�����жϣ����������¼�
// ����: port:���ں�.
// ����: 0.
// =============================================================================
u32 UART_ISR(ptu32_t port)
{
    struct UartGeneralCB *UCB;
    u32 num;
    u8 ch;

    UCB = pUartCB[port];
    if ((kLPUART_RxDataRegFullFlag)&    \
            LPUART_GetStatusFlags((tagUartReg *)tg_UART_Reg[port]))//�����ж�
    {
        ch = LPUART_ReadByte((tagUartReg *)tg_UART_Reg[port]);
        num = UART_PortWrite(UCB,&ch,1);
        if(num != 1)
        {
            UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
        }
    }
    if((kLPUART_NoiseErrorFlag|kLPUART_FramingErrorFlag|kLPUART_ParityErrorFlag) &  \
            LPUART_GetStatusFlags((tagUartReg *)tg_UART_Reg[port])) /*��������*/
    {
        Board_UartHalfDuplexRecv(port);
    }
    if((kLPUART_TxDataRegEmptyFlag)&
            LPUART_GetStatusFlags((tagUartReg *)tg_UART_Reg[port]))
    {
        num = UART_PortRead(UCB,&ch,1);
        if(num != 0)
            LPUART_WriteByte((tagUartReg *)tg_UART_Reg[port], ch);
        else
        {
            LPUART_DisableInterrupts((tagUartReg *)tg_UART_Reg[port], \
                        kLPUART_TxDataRegEmptyInterruptEnable);
        }
        LPUART_ClearStatusFlags((tagUartReg *)tg_UART_Reg[port], \
                kLPUART_TxDataRegEmptyInterruptEnable);
    }
    if((kLPUART_TxFifoOverflowFlag|kLPUART_RxFifoUnderflowFlag)&    \
            LPUART_GetStatusFlags((tagUartReg *)tg_UART_Reg[port])) //ORE���ش���
    {
        UART_ErrHandle(UCB,CN_UART_FIFO_OVER_ERR);
    }
    return 0;
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
ptu32_t ModuleInstall_UART(u8 port)
{
     struct UartParam UART_Param;

     switch(port)
     {
        case CN_UART1://����1
            UART_Param.Name         = "UART1";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CN_UART1_BASE;
            UART_Param.TxRingBufLen = CFG_UART1_SENDBUF_LEN;
            UART_Param.RxRingBufLen = CFG_UART1_RECVBUF_LEN;
            UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
            UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
            break;
        case CN_UART2://����2
            UART_Param.Name         = "UART2";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CN_UART2_BASE;
            UART_Param.TxRingBufLen = CFG_UART2_SENDBUF_LEN;
            UART_Param.RxRingBufLen = CFG_UART2_RECVBUF_LEN;
            UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
            UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
            break;

        case CN_UART3://����3
            UART_Param.Name         = "UART3";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CFG_UART3_SENDBUF_LEN;
            UART_Param.TxRingBufLen = CFG_UART3_RECVBUF_LEN;
            UART_Param.RxRingBufLen = CFG_UART3_RECVBUF_LEN;
            UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
            UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
            break;

        case CN_UART4://����4
            UART_Param.Name         = "UART4";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CN_UART4_BASE;
            UART_Param.TxRingBufLen = CFG_UART4_SENDBUF_LEN;
            UART_Param.RxRingBufLen = CFG_UART4_RECVBUF_LEN;
            UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
            UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
            break;
        case CN_UART5://����5
            UART_Param.Name         = "UART5";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CN_UART5_BASE;
            UART_Param.TxRingBufLen = CFG_UART5_SENDBUF_LEN;
            UART_Param.RxRingBufLen = CFG_UART5_RECVBUF_LEN;
            UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
            UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
            break;
        case CN_UART6://����6
            UART_Param.Name         = "UART6";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CN_UART6_BASE;
            UART_Param.TxRingBufLen = CFG_UART6_SENDBUF_LEN;
            UART_Param.RxRingBufLen = CFG_UART6_RECVBUF_LEN;
            UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
            UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
            break;
        case CN_UART7://����7
            UART_Param.Name         = "UART7";
            UART_Param.Baud         = 115200;
            UART_Param.UartPortTag  = CN_UART7_BASE;
            UART_Param.TxRingBufLen = CFG_UART7_SENDBUF_LEN;
            UART_Param.RxRingBufLen = CFG_UART7_RECVBUF_LEN;
            UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
            UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
            break;
        case CN_UART8://����8
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
     /*
     pUART_DmaSendBuf[serial_no]=NULL;
     pUART_DmaRecvBuf[serial_no][0]=NULL;
     pUART_DmaRecvBuf[serial_no][1]=NULL;
     */
     Board_UartHalfDuplexRecv(port);
     //Ӳ����ʼ��
     __UART_HardInit(port);
     __UART_IntInit(port);
     __UART_Enable(port);
     __UART_RxIntEnable(CN_DMA_UNUSED,port);
     sUartInited |= (0x01 << port);
     pUartCB[port] = UART_InstallGeneral(&UART_Param);
     if( pUartCB[port] == NULL)
         return 0;
     else
         return 1;
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
    CR_Bak = LPUART_GetEnabledInterrupts(PutStrDirectReg);                      //Save INT
    LPUART_DisableInterrupts(PutStrDirectReg,\
        (kLPUART_TxDataRegEmptyInterruptEnable|kLPUART_TransmissionCompleteInterruptEnable));//disable send INT
    for(result=0; result < len+1; result ++)
    {
        // ��ʱ���߷��ͻ���Ϊ��ʱ�˳�
        while((false == __UART_TxTranEmpty(PutStrDirectReg))&& (timeout > 10))
        {
            timeout -=10;
            DJY_DelayUs(10);
        }
        if( (timeout <= 10) || (result == len))
            break;
        PutStrDirectReg->DATA = *(u8*)(str+result);
    }
    LPUART_EnableInterrupts(PutStrDirectReg,CR_Bak); //restore send INT
    while(!__UART_TxTranComplete(PutStrDirectReg))
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
// ���ܣ��ַ��ն�ֱ�ӽ��պ�����������ѯ��ʽ��ֱ�Ӷ��Ĵ���������stdin��ʼ��ǰ
// ������str�������ַ���ָ��
//      len�����͵��ֽ���
// ���أ�0����������result,�������ݳ��ȣ��ֽڵ�λ
// =============================================================================
char Uart_GetCharDirect(void)
{
    u16 CR_Bak;
    char result;

    CR_Bak = LPUART_GetEnabledInterrupts(GetCharDirectReg);               //Save INT
    LPUART_DisableInterrupts(GetCharDirectReg,\
            (kLPUART_TxDataRegEmptyInterruptEnable|kLPUART_TransmissionCompleteInterruptEnable));//disable send INT
    while(__UART_RxHadChar(GetCharDirectReg) == false)
    {
        DJY_EventDelay(5*mS);
    }
    result=LPUART_ReadByte(GetCharDirectReg);
    LPUART_EnableInterrupts(GetCharDirectReg,CR_Bak); //restore send INT
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
        __UART_Enable(TxDirectPort);
        TxByteTime = 95;      //��ʼ��Ĭ��115200������һ��byte��87uS,+10%����
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
        {
            __UART_HardInit(RxDirectPort);
            __UART_Enable(RxDirectPort);
        }
        GetCharDirect = Uart_GetCharDirect;
    }
    return;
}

