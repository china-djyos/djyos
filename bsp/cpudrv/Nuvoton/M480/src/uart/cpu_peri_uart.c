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
// ������Ա: hm
// ����ʱ��: 14/08.2014
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
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
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
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip uart"//CPU��uart��������
//parent:"uart device file"      //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","lock","uart device file","heap","cpu onchip dma","Nuvoton M480"//������������������������none����ʾ�������������
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
//%$#@num,16,512,
#define CFG_UART0_SENDBUF_LEN            32                 //"UART1���ͻ��λ�������С",
#define CFG_UART0_RECVBUF_LEN            32                 //"UART1���ջ��λ�������С",
#define CFG_UART0_DMABUF_LEN             32                 //"UART1 DMA��������С",

#define CFG_UART1_SENDBUF_LEN            32                 //"UART2���ͻ��λ�������С",
#define CFG_UART1_RECVBUF_LEN            32                 //"UART2���ջ��λ�������С",
#define CFG_UART1_DMABUF_LEN             32                 //"UART2 DMA��������С",

#define CFG_UART2_SENDBUF_LEN            32                 //"UART3���ͻ��λ�������С",
#define CFG_UART2_RECVBUF_LEN            32                 //"UART3���ջ��λ�������С",
#define CFG_UART2_DMABUF_LEN             32                 //"UART3 DMA��������С",

#define CFG_UART3_SENDBUF_LEN            32                 //"UART4���ͻ��λ�������С",
#define CFG_UART3_RECVBUF_LEN            32                 //"UART4���ջ��λ�������С",
#define CFG_UART3_DMABUF_LEN             32                 //"UART4 DMA��������С",

#define CFG_UART4_SENDBUF_LEN            32                 //"UART5���ͻ��λ�������С",
#define CFG_UART4_RECVBUF_LEN            32                 //"UART5���ջ��λ�������С",
#define CFG_UART4_DMABUF_LEN             32                 //"UART5 DMA��������С",

#define CFG_UART5_SENDBUF_LEN            32                 //"UART6���ͻ��λ�������С",
#define CFG_UART5_RECVBUF_LEN            32                 //"UART6���ջ��λ�������С",
#define CFG_UART5_DMABUF_LEN             32                 //"UART6 DMA��������С",
//%$#@enum,true,false,
#define CFG_UART0_ENABLE                 true               //"�Ƿ�ʹ��UART0",
#define CFG_UART1_ENABLE                 false              //"�Ƿ�ʹ��UART1",
#define CFG_UART2_ENABLE                 false              //"�Ƿ�ʹ��UART2",
#define CFG_UART3_ENABLE                 false              //"�Ƿ�ʹ��UART3",
#define CFG_UART4_ENABLE                 false              //"�Ƿ�ʹ��UART4",
#define CFG_UART5_ENABLE                 false              //"�Ƿ�ʹ��UART5",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
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

static uint32_t TxByteTime;                    //��������һ���ֽ�����Ҫ��ʱ��
static tagUartReg *PutStrDirectReg;     //����printk���͵Ĵ��ڼĴ���
static tagUartReg *GetCharDirectReg;     //����ֱ�ӽ��յĴ��ڼĴ���
static uint32_t TxDirectPort;                  //����printk���͵Ĵ��ں�
static uint32_t RxDirectPort;                  //����ֱ�ӽ��յĴ��ں�
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
// ����ģ���ʼ���ṹ��
struct UartParam
{
    const char *Name;                   //UART���ƣ���UART0
    uint32_t TxRingBufLen;                   //���ͻ����������ֽ���
    uint32_t RxRingBufLen;                   //���ջ����������ֽ���
    uint32_t Baud;                           //Ĭ�ϵĲ�����
    uint8_t  mode;                           //���ڹ���ģʽ���μ�CN_UART_GENERAL˵��
    ptu32_t UartPortTag;                //UART˽�б�ǩ����Ĵ�����ַ
    UartStartSend StartSend;            //�������ͻص�����ָ��
//    UartDirectSend DirectlySend;        //ֱ����ѯ���ͻص�����ָ��
    UartControl UartCtrl;               //���ƺ����ص�����ָ��
};
#endif

#define CN_DMA_UNUSED   0
#define CN_DMA_USED     1

// UART��Ӧ��PDMA�жϺ�
static u8 const UartDmaRxInt[] = {PDMA_UART0_RX,PDMA_UART1_RX,
                                  PDMA_UART2_RX,PDMA_UART3_RX,
                                  PDMA_UART4_RX,PDMA_UART5_RX};
static u8 const UartDmaTxInt[] = {PDMA_UART0_TX,PDMA_UART1_TX,
                                  PDMA_UART2_TX,PDMA_UART3_TX,
                                  PDMA_UART4_TX,PDMA_UART5_TX};

static u8 const PDMA_Tx_ch[] = {0,2,4,6,8,10};
static u8 const PDMA_Rx_ch[] = {1,3,5,7,9,11};

// DMA����ʹ�ñ�ǣ��Ƿ�ʹ��PDMA���
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

//���ڱ�ʶ�����Ƿ��ʼ����ǣ���0λ��ʾUART0����һλ��UART1....
//�������ƣ�1��ʾ��ʼ����0��ʾδ��ʼ��
static uint8_t sUartInited = 0;

// =============================================================================
static ptu32_t UART_ISR(ptu32_t port);
static uint32_t PDMA_ISR(ptu32_t port);
// =============================================================================
// ����: ��ֹuart�ķ����жϡ�
// ����: flag,�Ƿ�ʹ��dma����
//       port,���ںţ�1~5
// ����: ��
// =============================================================================
void __UART_RxIntEnable(u8 flag,u8 port)
{
    if(flag == CN_DMA_USED)
        PDMA_EnableInt(PDMA,PDMA_Rx_ch[port],PDMA_INT_TRANS_DONE);
    else
        tg_UART_Reg[port]->INTEN |= (1<<0);
}

// =============================================================================
// ����: ��ֹuart�Ľ����ж�
// ����: reg,�������ļĴ�����ָ��
//       port,���ں�
// ����: ��
// =============================================================================
void __UART_RxIntDisable(u8 flag, u8 port)
{
    if(flag == CN_DMA_USED)
        PDMA_DisableInt(PDMA,PDMA_Rx_ch[port], PDMA_INT_TRANS_DONE);
    else
        tg_UART_Reg[port]->INTEN &= ~(1<<0);
}

// =============================================================================
// ����: ʹ��uart�ķ����ж�
// ����: reg,�������ļĴ�����ָ��
//       port,���ں�
// ����: ��
// =============================================================================
static void __UART_TxIntEnable(u8 flag,u8 port)
{
    if(flag == CN_DMA_USED)
        PDMA_EnableInt(PDMA,PDMA_Tx_ch[port], PDMA_INT_TRANS_DONE);
    else
        tg_UART_Reg[port]->INTEN |= (1<<1);
}

// =============================================================================
// ����: ��ֹUART�ķ����ж�
// ����: Reg,�������ļĴ�����ָ��
//       port,���ں�
// ����: ��
// =============================================================================
static void __UART_TxIntDisable(u8 flag, u8 port)
{
    if(flag == CN_DMA_USED)
        PDMA_DisableInt(PDMA,PDMA_Tx_ch[port], PDMA_INT_TRANS_DONE);
    else
        tg_UART_Reg[port]->INTEN &= ~(1<<1);//��ֹ���Ϳ��ж�
}

// =============================================================================
// ����: ��鷢�ͼĴ�����״̬������վͷ���true����Ϊ�գ����򷵻�false
// ������reg,�������ļĴ�����ָ��
// ����: true = �������գ�false = �ǿ�
// =============================================================================
static bool_t __UART_TxTranEmpty(tagUartReg volatile *reg)
{
    return (0 != (reg->FIFOSTS & (1<<22)));
}

// =============================================================================
// ����: �����ռĴ�����״̬�������ݾͷ���true�����򷵻�false
// ������reg,�������ļĴ�����ָ��
// ����: true = �����ݣ�false = ������
// =============================================================================
static bool_t __UART_RxHadChar(tagUartReg volatile *reg)
{
    return ((reg->FIFOSTS & (1<<14)) ==0);
}

// =============================================================================
// ����: ���ô���baud,PCLKΪ25M��CPU��ƵΪ100M����,������ֻ�ṩ9600��19200��57600��
//       115200���ֲ���������
// ����: Reg,�������ļĴ�����ָ��
//       port,���ں�
//       baud,������
// ����: ��
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
// ����: ���ö�ӦUART��IO�ڣ�����ʱ�Ӻ�IO����
// ����: SerialNo,���ں�
// ����: ��
// =============================================================================
static void __UART_GpioConfig(uint8_t SerialNo)
{
    //UART ��GPIO��ʼ��Ӧ���ݰ����������˸ú�����boarddrv�ļ��е�board.c��
    Board_UartGpioInit(SerialNo);
}

// =============================================================================
// ����: �Դ��ڴ���������ã����������ʡ���żУ�顢����λ��ֹͣλ
// ����:  Reg,�������ļĴ�����ָ��
//        port,���ں�
//        data,�ṹ��tagCOMParam���͵�ָ����ֵ
// ����: ��
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
// ����: ʹ���ڿ�ʼ������ʱ�Ӻ�uartģ��
// ����: port,���ں�
// ����: ��
// =============================================================================
void __UART_Enable(uint32_t port)
{
    switch(port)
    {//ʹ�ܴ���1ʱ��
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
// ����: ʹ����ֹͣ����������ʱ�Ӻ�uartģ��
// ����: port,���ں�
// ����: ��
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
// ����: ��ʼ��UART��Ӧ���ж��ߣ�����ʼ���ж���ں���
// ������SerialNo,���ں�
// ����: ��
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
// ����: Ӳ���������úͼĴ����ĳ�ʼ�������������ʡ�ֹͣλ��У��λ������λ��Ĭ�������:
//       ������:115200  �� ֹͣλ:1 ; У��:��У�� ; ����λ:8bit
// ����: SerialNo,���ںţ�0~3
// ����: ��
// =============================================================================
static void __UART_HardInit(uint8_t SerialNo)
{
    if(SerialNo > CN_UART5)
        return;

    __UART_GpioConfig(SerialNo); //ʹ�ܹܽ�

    /* Select UART function */
    tg_UART_Reg[SerialNo]->FUNCSEL = UART_FUNCSEL_UART;
     /* Set UART line configuration */
    tg_UART_Reg[SerialNo]->LINE = UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1;
     /* Set UART Rx and RTS trigger level */
    tg_UART_Reg[SerialNo]->FIFO &= ~(UART_FIFO_RFITL_Msk | UART_FIFO_RTSTRGLV_Msk);

    __UART_BaudSet(tg_UART_Reg[SerialNo],SerialNo,115200); //�����ʳ�ʼ��

    UART_ENABLE_INT(tg_UART_Reg[SerialNo], (UART_INTEN_RDAIEN_Msk | UART_INTEN_THREIEN_Msk));
}

// =============================================================================
// ����: ��ѯ�ȴ����ڷ�����ϣ���ʱ����
// ����: sending��dma���ͷ�ʽ�������Ƿ��Ѿ��������
// ����: ��
// =============================================================================
bool_t __uart_dma_timeout(bool_t sending)
{
    s32 timeout = 1000;
    while((sending == true)&& (timeout > 0))//��ʱ
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
// ����: ��������PDMA���ͣ�ʹ��DMA�������ڷ���ǰ���ж��ϴ�DMA�Ƿ��Ѿ�������ɣ�ͨ��
//       �鿴��־UARTx_DMA_SENDING���ж�UARTx�Ƿ��Ѿ��������
// ����: PORT,���ں�.
// ����: ���͵ĸ���
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
        //DMA_Enable(UartPdmaTxStream[port],addr,num); //����DMA
        PDMA_EnableInt(PDMA,PDMA_Tx_ch[port], 0);
        s_UART_DmaSending[port] = true;
    }
    else
    {
        tg_UART_Reg[port]->INTEN |= (1<<22);    //ʹ�ܷ�������ж�;
    }
    return 0;
}

// =============================================================================
// ����: �������ڷ��ͣ���Ŀ���Ǵ����жϣ����жϷ�ʽ�������ݡ�
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
// ����: ���͵ĸ���
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
    Board_UartHalfDuplexSend(port);//�л�������
    __UART_TxIntDisable(s_UART_DmaUsed[port],port);
    if(s_UART_DmaUsed[port])
    {
        __UART_PDMA_SendStart(port);
    }
    else
    {
        //��DMA��ʽ��ֻҪʹ�ܷ��Ϳ��жϣ��򴥷��ж�
    }

    __UART_TxIntEnable(s_UART_DmaUsed[port],port);
    return 1;
}

// =============================================================================
// ����: ����uartʹ��dma�շ�������stm32�����ڵ��շ�dmaͨ�����üĴ����������³�
//       ʼ������dma�Ĵ�������Ϊ:
//       1����ʹ��dma�ڼ�����������豸ʹ��dma���Ӷ����������á�
//       2����ʹuartʹ��dma�ڼ䣬Ҳ�������ģ����ϵ����⵼�������豸ʹ��dma.
// ����: port,�˿ںţ������ںţ���1~3
// ����: ��
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

    //DMA_IntEnable(PDMA_Tx_ch[port],DMA_INT_TCIE);  //ʹ�ܷ�������ж�
    //DMA_IntEnable(PDMA_Rx_ch[port],DMA_INT_HTIE);  //ʹ�ܽ��հ�����ж�

    __UART_TxIntEnable(CN_DMA_USED,port);
    __UART_RxIntEnable(CN_DMA_USED,port);

    //tg_UART_Reg[port]->CR3 |= 0x00c0;  //���ô���ʹ��dma�շ�
    tg_UART_Reg[port]->INTEN |= UART_INTEN_TXPDMAEN_Msk | UART_INTEN_RXPDMAEN_Msk;
    tg_UART_Reg[port]->INTEN = UART_INTEN_RLSIEN_Msk; // Enable Receive Line interrupt
    s_UART_DmaUsed[port] = CN_DMA_USED;

    //DMA_Enable(UartDmaRxStream[port],(u32)DmaRecvBuf,32);//����dmaͨ��

    Int_Register(CN_INT_LINE_PDMA);
    Int_SetClearType(CN_INT_LINE_PDMA,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(CN_INT_LINE_PDMA,PDMA_ISR);
    Int_SettoAsynSignal(CN_INT_LINE_PDMA);
    Int_RestoreAsynLine(CN_INT_LINE_PDMA);

}

//=============================================================================
// ����: ���ò�ʹ��DMA��UART��ʹ��DMA�շ���ÿ�շ�һ���ֽھͱ����ж�һ�Σ��������ڸ���
//      ����ͨ����ʹ�á�
// ����: port,�˿ںţ������ںţ�Ϊ1~3
// ����: ��
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

    //tg_UART_Reg[port]->CR3 &= ~0x00c0;     //���ò�ʹ��dma����
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

//----�����豸���ƺ���---------------------------------------------------------
//����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
//����: Reg,�������Ĵ��ڼĴ���ָ��.
//      cmd,��������
//      data,������cmd����
//����: ������.
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
        case CN_UART_SET_BAUD:  //����Baud
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
    if( (Reg->INTSTS & (1<<8)) &&(Reg->INTEN &(1<<0)))           //�����ж�
    {
        ch = (uint8_t)Reg->DAT;
        num = UART_PortWrite(UCB,&ch,1);
        if(num != 1)
        {
            UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
        }
    }
    if((Reg->INTSTS & (1<<22))&&(Reg->INTEN & (1<<22)) )   //��������ж�
    {
        Reg->INTEN &= ~(1<<22);
        Board_UartHalfDuplexRecv(port);
    }
    if((Reg->INTSTS & (1<<9))&&(Reg->INTEN & (1<<1)))    //�����ж�
    {
        num = UART_PortRead(UCB,&ch,1);
        if(num != 0)
            Reg->DAT = ch;
        else
        {
            Reg->INTEN &= ~(1<<1);        //�رշ����ж�ʹ��
            Reg->INTEN |= (1<<22);    //ʹ�ܷ�������ж�
        }
    }
    if(Reg->FIFOSTS & (1<<0))        //ORE���ش���
    {
        ch = (uint8_t)Reg->DAT;
        num = Reg->FIFOSTS;     //��һ��sr�Ĵ���
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
// ���ܣ���ʼ��uart1ģ��,��װ�ش����豸�������uart.c��API�������ú�����Ҫ��ɵĹ���
//       ���£�
//       1.��ʼ�����ڵ�Ӳ�����֣���GPIO���жϡ��������õ�COM����������baud = 115200
//         stop bits = 1,����żУ��, data bits = 8
//       2.����uart.c��API����UART_InstallPort��װ�봮���豸
// ���������ںţ�0~3
// ���أ�1=�ɹ���0=ʧ��
// =============================================================================
ptu32_t ModuleInstall_UART(uint32_t serial_no)
{
    struct UartParam UART_Param;

    switch(serial_no)
    {
    case CN_UART0://����1
        UART_Param.Name         = "UART0";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART0_BASE;
        UART_Param.TxRingBufLen = CFG_UART0_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART0_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART1://����2
        UART_Param.Name         = "UART1";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART1_BASE;
        UART_Param.TxRingBufLen = CFG_UART1_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART1_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;

    case CN_UART2://����3
        UART_Param.Name         = "UART2";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART2_BASE;
        UART_Param.TxRingBufLen = CFG_UART2_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART2_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;

    case CN_UART3://����4
        UART_Param.Name         = "UART3";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART3_BASE;
        UART_Param.TxRingBufLen = CFG_UART3_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART3_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART4://����5
        UART_Param.Name         = "UART4";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART4_BASE;
        UART_Param.TxRingBufLen = CFG_UART4_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART4_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART5://����6
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
    //Ӳ����ʼ��
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
// ���ܣ��ַ��ն�ֱ�ӷ��ͺ�����������ѯ��ʽ��ֱ��д�Ĵ���������printk������stdout
//       û�г�ʼ��
// ������str�������ַ���ָ��
//      len�����͵��ֽ���
// ���أ�0����������result,�������ݳ��ȣ��ֽڵ�λ
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
        // ��ʱ���߷��ͻ���Ϊ��ʱ�˳�
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
// ���ܣ��ַ��ն�ֱ�ӽ��պ�����������ѯ��ʽ��ֱ�Ӷ��Ĵ���������stdin��ʼ��ǰ
// ������str�������ַ���ָ��
//      len�����͵��ֽ���
// ���أ�0����������result,�������ݳ��ȣ��ֽڵ�λ
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


//----��ʼ���ں˼�IO-----------------------------------------------------------
//���ܣ���ʼ���ں˼��������������runtime����ָ�롣
//��������
//���أ���
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
        TxByteTime = 95;      //��ʼ��Ĭ��115200������һ��byte��87uS,+10%����
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
