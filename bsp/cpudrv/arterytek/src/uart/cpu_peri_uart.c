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

#include <at32f435_437_usart.h>

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
//%$#@num,16,512,
#define CFG_UART1_SENDBUF_LEN            32                 //"UART1���ͻ��λ�������С",
#define CFG_UART1_RECVBUF_LEN            32                 //"UART1���ջ��λ�������С",
#define CFG_UART1_DMABUF_LEN             32                 //"UART1 DMA��������С",

#define CFG_UART2_SENDBUF_LEN            32                 //"UART2���ͻ��λ�������С",
#define CFG_UART2_RECVBUF_LEN            32                 //"UART2���ջ��λ�������С",
#define CFG_UART2_DMABUF_LEN             32                 //"UART2 DMA��������С",

#define CFG_UART3_SENDBUF_LEN            32                 //"UART3���ͻ��λ�������С",
#define CFG_UART3_RECVBUF_LEN            32                 //"UART3���ջ��λ�������С",
#define CFG_UART3_DMABUF_LEN             32                 //"UART3 DMA��������С",

#define CFG_UART4_SENDBUF_LEN            32                 //"UART4���ͻ��λ�������С",
#define CFG_UART4_RECVBUF_LEN            32                 //"UART4���ջ��λ�������С",
#define CFG_UART4_DMABUF_LEN             32                 //"UART4 DMA��������С",

#define CFG_UART5_SENDBUF_LEN            32                 //"UART5���ͻ��λ�������С",
#define CFG_UART5_RECVBUF_LEN            32                 //"UART5���ջ��λ�������С",
#define CFG_UART5_DMABUF_LEN             32                 //"UART5 DMA��������С",

#define CFG_UART6_SENDBUF_LEN            32                 //"UART6���ͻ��λ�������С",
#define CFG_UART6_RECVBUF_LEN            32                 //"UART6���ջ��λ�������С",
#define CFG_UART6_DMABUF_LEN             32                 //"UART6 DMA��������С",
//%$#@enum,true,false,
#define CFG_UART1_ENABLE                 true               //"�Ƿ�ʹ��UART1",
#define CFG_UART2_ENABLE                 false              //"�Ƿ�ʹ��UART2",
#define CFG_UART3_ENABLE                 false              //"�Ƿ�ʹ��UART3",
#define CFG_UART4_ENABLE                 false              //"�Ƿ�ʹ��UART4",
#define CFG_UART5_ENABLE                 false              //"�Ƿ�ʹ��UART5",
#define CFG_UART6_ENABLE                 false              //"�Ƿ�ʹ��UART6",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

#define CN_UART1_BASE 0x40011000//0x40013800
#define CN_UART2_BASE 0x40004400
#define CN_UART3_BASE 0x40004800
#define CN_UART4_BASE 0x40004c00
#define CN_UART5_BASE 0x40005000
#define CN_UART6_BASE 0X40011400



static u32 art43x_StartSend(ptu32_t PrivateTag);
static ptu32_t art43x_UartCtrl(ptu32_t PrivateTag, u32 cmd, va_list* arg0);

struct UartParam art43x_UART_Param[] = {
#ifdef CN_UART1
	[0] = {
		.Name = "UART1",
		.Baud = 115200,
		.UartPortTag = 0,
		.TxRingBufLen = CFG_UART1_SENDBUF_LEN,
		.RxRingBufLen = CFG_UART1_RECVBUF_LEN,
		.StartSend = art43x_StartSend,
		.UartCtrl = art43x_UartCtrl,
	.mode = CN_UART_GENERAL,
	},
#endif
#ifdef CN_UART2
	[1] = {
		.Name = "UART2",
		.Baud = 115200,
		.UartPortTag = 0,
		.TxRingBufLen = CFG_UART2_SENDBUF_LEN,
		.RxRingBufLen = CFG_UART2_RECVBUF_LEN,
		.StartSend = art43x_StartSend,
		.UartCtrl = art43x_UartCtrl,
	.mode = CN_UART_GENERAL,
	},
#endif
#ifdef CN_UART3
	[2] = {
		.Name = "UART3",
		.Baud = 115200,
		.UartPortTag = 0,
		.TxRingBufLen = CFG_UART3_SENDBUF_LEN,
		.RxRingBufLen = CFG_UART3_RECVBUF_LEN,
		.StartSend = art43x_StartSend,
		.UartCtrl = art43x_UartCtrl,
	.mode = CN_UART_GENERAL,
	},
#endif
#ifdef CN_UART4
	[3] = {
		.Name = "UART4",
		.Baud = 115200,
		.UartPortTag = 0,
		.TxRingBufLen = CFG_UART4_SENDBUF_LEN,
		.RxRingBufLen = CFG_UART4_RECVBUF_LEN,
		.StartSend = art43x_StartSend,
		.UartCtrl = art43x_UartCtrl,
	.mode = CN_UART_GENERAL,
	},
#endif
#ifdef CN_UART5
	[4] = {
		.Name = "UART5",
		.Baud = 115200,
		.UartPortTag = 0,
		.TxRingBufLen = CFG_UART5_SENDBUF_LEN,
		.RxRingBufLen = CFG_UART5_RECVBUF_LEN,
		.StartSend = art43x_StartSend,
		.UartCtrl = art43x_UartCtrl,
	.mode = CN_UART_GENERAL,
	},
#endif
#ifdef CN_UART6
	[5] = {
		.Name = "UART6",
		.Baud = 115200,
		.UartPortTag = 0,
		.TxRingBufLen = CFG_UART6_SENDBUF_LEN,
		.RxRingBufLen = CFG_UART6_RECVBUF_LEN,
		.StartSend = art43x_StartSend,
		.UartCtrl = art43x_UartCtrl,
	.mode = CN_UART_GENERAL,
	},
#endif
#ifdef CN_UART7
	[6] = {
		.Name = "UART6",
		.Baud = 115200,
		.UartPortTag = 0,
		.TxRingBufLen = CFG_UART7_SENDBUF_LEN,
		.RxRingBufLen = CFG_UART7_RECVBUF_LEN,
		.StartSend = art43x_StartSend,
		.UartCtrl = art43x_UartCtrl,
	.mode = CN_UART_GENERAL,
	},
#endif
};

static u32 TxByteTime;                    //��������һ���ֽ�����Ҫ��ʱ��
static u32 TxDirectPort;                  //����printk���͵Ĵ��ں�
static u32 RxDirectPort;                  //����ֱ�ӽ��յĴ��ں�

// =============================================================================

/* we need a debug uart at the very beginning. */

struct uart_priv {
	int port;
	void* TxRingBuf;
	void* RxRingBuf;

	struct UartGeneralCB* pUartCB;
	char* dma_rx_buf;
	char* dma_tx_buf;
	int dmabuf_len;
};

/*TODO: this macro should be public accessable. */
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

usart_type* to_usart_type(u32 port)
{
	usart_type* usart_x[] = {
		USART1, USART2, USART3, UART4, UART5, USART6, UART7, UART8,
	};

	if (port >= 0 && port < ARRAY_SIZE(usart_x)) return usart_x[port];
	return NULL;
}

int to_irq(int port)
{
	int irq = -1;
	int uart_irq[] = {
		CN_INT_LINE_USART1, CN_INT_LINE_USART2, CN_INT_LINE_USART3,
		CN_INT_LINE_UART4, CN_INT_LINE_UART5,
		CN_INT_LINE_USART6,
		CN_INT_LINE_UART7, CN_INT_LINE_UART8,
	};

	if (port >= 0 && port < ARRAY_SIZE(uart_irq)) irq = uart_irq[port];
	return irq;
}

#define CN_DMA_UNUSED   0
#define CN_DMA_USED     1

// UART��Ӧ��DMA Stream�жϺ�
static u8 const UartDmaRxInt[16] = { 0 };
static u8 const UartDmaTxInt[16] = { 0 };

typedef void DMA_Stream_TypeDef;


#define DMA1_Stream0 (void *)0
#define DMA1_Stream1 (void *)0
#define DMA1_Stream2 (void *)0
#define DMA1_Stream3 (void *)0
#define DMA1_Stream4 (void *)0
#define DMA1_Stream5 (void *)0
#define DMA1_Stream6 (void *)0
#define DMA1_Stream7 (void *)0
#define DMA2_Stream2 (void *)0
#define DMA2_Stream6 (void *)0
#define DMA2_Stream7 (void *)0

static DMA_Stream_TypeDef * const UartDmaRxStream[] = {
	DMA2_Stream2,
	DMA1_Stream5,
	DMA1_Stream1,
	DMA1_Stream2,
	DMA1_Stream0,
	DMA2_Stream2
};
static DMA_Stream_TypeDef * const UartDmaTxStream[] = {
	DMA2_Stream7,
	DMA1_Stream6,
	DMA1_Stream3,
	DMA1_Stream4,
	DMA1_Stream7,
	DMA2_Stream6
};

static u8 const DMA_Tx_ch[] = {4,4,4,4,4,5};
static u8 const DMA_Rx_ch[] = {4,4,4,4,4,5};
// DMA����ʹ�ñ�ǣ��Ƿ�ʹ��DMA���
static bool_t s_UART_DmaSending[] = {false,false,false,false,false,false};
static bool_t s_UART_DmaUsed[] = {false,false,false,false,false,false};

//#define CFG_UART1_SENDBUF_LEN            32
//#define CFG_UART1_RECVBUF_LEN            32
//#define CFG_UART1_DMABUF_LEN             32
//
//#define CFG_UART2_SENDBUF_LEN            32
//#define CFG_UART2_RECVBUF_LEN            32
//#define CFG_UART2_DMABUF_LEN             32
//
//#define CFG_UART3_SENDBUF_LEN            32
//#define CFG_UART3_RECVBUF_LEN            32
//#define CFG_UART3_DMABUF_LEN             32
//
//#define CFG_UART4_SENDBUF_LEN            32
//#define CFG_UART4_RECVBUF_LEN            32
//#define CFG_UART4_DMABUF_LEN             32
//
//#define CFG_UART5_SENDBUF_LEN            32
//#define CFG_UART5_RECVBUF_LEN            32
//#define CFG_UART5_DMABUF_LEN             32
//
//#define CFG_UART6_SENDBUF_LEN            32
//#define CFG_UART6_RECVBUF_LEN            32
//#define CFG_UART6_DMABUF_LEN             32

static u8 *pUART_DmaSendBuf[CN_UART_NUM];
static u8 *pUART_DmaRecvBuf[CN_UART_NUM];

// static struct UartGeneralCB *pUartCB[CN_UART_NUM];

__attribute__((weak))  void Board_UartHalfDuplexSend(u8 SerialNo)
{
    return;
}
__attribute__((weak))  void Board_UartHalfDuplexRecv(u8 SerialNo)
{
    return ;
}
// =============================================================================
static ptu32_t UART_ISR(ptu32_t port);

static uint32_t UART_DmaRx_ISR(ptu32_t port);
static uint32_t UART_DmaTx_ISR(ptu32_t port);

// =============================================================================
// ����: ��ֹuart�ķ����жϡ�
// ����: flag,�Ƿ�ʹ��dma����
//       port,���ںţ�1~5
// ����: ��
// =============================================================================
void __UART_RxIntEnable(u8 flag,u8 port)
{
    if(flag == CN_DMA_USED)
	Int_RestoreAsynLine(0); /* disable dma's irq */
    else
	/* ##WIP## disable irq. */;
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
	Int_SaveAsynLine(0);
    else
        /* ##WIP## disable irq. */;
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
	Int_RestoreAsynLine(0);
    else
        /* ##WIP## disable this uart's irq. */;
}



// =============================================================================
// ����: ��ֹUART�ķ����ж�
// ����: Reg,�������ļĴ�����ָ��
//       port,���ں�
// ����: ��
// =============================================================================
static void __UART_TxIntDisable(u8 flag, u8 port)
{
//    if(flag == CN_DMA_USED)
//	Int_SaveAsynLine(UartDmaTxInt[port]);
//    else
//	tg_UART_Reg[port]->CR1 &= ~(1<<7);//��ֹ���Ϳ��ж�
}

// =============================================================================
// ����: ��ʼ��UART��Ӧ���ж��ߣ�����ʼ���ж���ں���
// ������SerialNo,���ں�
// ����: ��
// =============================================================================
static void __UART_IntInit(u32 SerialNo)
{
    u8 IntLine;
    int port, irq;

    port = SerialNo;
    irq = to_irq(port);
    if (irq < 0) return;

    IntLine = irq;
    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,UART_ISR);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
    Int_SetIsrPara(IntLine,SerialNo);
}
// =============================================================================
// ����: ��ѯ�ȴ����ڷ�����ϣ���ʱ����
// ����: sending��dma���ͷ�ʽ�������Ƿ��Ѿ��������
// ����: ��
// =============================================================================
bool_t __uart_dma_timeout(bool_t sending)
{
    int timeout = 1000;
	while (sending && timeout) {
		timeout--;
		DJY_DelayUs(1);
	}

	return !timeout;
}
// =============================================================================
// ����: ��������DMA���ͣ�ʹ��DMA�������ڷ���ǰ���ж��ϴ�DMA�Ƿ��Ѿ�������ɣ�ͨ��
//       �鿴��־UARTx_DMA_SENDING���ж�UARTx�Ƿ��Ѿ��������
// ����: PORT,���ں�.
// ����: ���͵ĸ���
// =============================================================================
u32 __UART_DMA_SendStart(u32 port)
{
    u32 num,addr;
    u8 *DmaSendBuf;
    u32 DmaSendBufLen;

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
    default:
        break;
    }
    DmaSendBuf = pUART_DmaSendBuf[port];
//    if(true == __uart_dma_timeout(s_UART_DmaSending[port]))
    if(true == s_UART_DmaSending[port])
	return 0;
//    num = UART_PortRead(pUartCB[port],DmaSendBuf,DmaSendBufLen);
    addr = (u32)DmaSendBuf;

    if(num > 0)
    {
	DMA_Enable(UartDmaTxStream[port],addr,num); //����DMA
	s_UART_DmaSending[port] = true;
    }
    else
    {
//##WIP##	tg_UART_Reg[port]->CR1 |= (1<<6);
    }
    return 0;
}
// =============================================================================
// ����: ֱ�ӷ��ͺ�����ʹ����ѯ��ʽ��
// ����: Reg,�Ĵ�����ַָ��.
//       send_buf,�������ݻ�����
//       len,���ݳ��ȣ��ֽ�
//       timeout,��ʱʱ�䣬us
// ����: �����ֽ���
// =============================================================================
//static u32 __UART_SendDirectly(tagUartReg *Reg,u8 *send_buf,u32 len,u32 timeout)
//{
//    u32  result = 0;
//
//    if(Reg == NULL)
//        return result;
//
//    Reg->CR1 &= ~(1<<7);                            //disable send INT
//    for(result=0; result < len; result ++)
//    {
//        // ��ʱ���߷��ͻ���Ϊ��ʱ�˳�
//        while((false == __UART_TxTranEmpty(Reg))&& (timeout > 0))
//        {
//            timeout--;
//            DJY_DelayUs(1);
//        }
//        if(timeout == 0)
//            break;
//        Reg->DR = send_buf[result];
//    }
//    Reg->CR1 |= (1<<7);                             //enable send INT
//    return result;
//}

// =============================================================================
// ����: �������ڷ��ͣ���Ŀ���Ǵ����жϣ����жϷ�ʽ�������ݡ�
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
// ����: ���͵ĸ���
// =============================================================================

static u32 art43x_StartSend (ptu32_t PrivateTag)
{
	struct UartParam *param;
	struct uart_priv *priv;
	struct usart_type *usart_x;

	priv = (void*)PrivateTag;
	param = art43x_UART_Param + priv->port;

	usart_x = to_usart_type(priv->port);
	usart_interrupt_enable(usart_x, USART_IDLE_INT, FALSE);
	if (priv->dmabuf_len) {
		/* dma not supported yet. */
		while (1);
	}
	usart_interrupt_enable(usart_x, USART_IDLE_INT, TRUE);
	return 0;
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
#if 0
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
    tg_UART_Reg[port]->CR1 |= (1<<4);//enable idle int

    DMA_Config(UartDmaRxStream[port],DMA_Rx_ch[port],(u32)&(tg_UART_Reg[port]->DR),
            (u32)DmaRecvBuf,DMA_DIR_P2M,DMA_DATABITS_8,DMA_DATABITS_8,32);

    DMA_Config(UartDmaTxStream[port],DMA_Tx_ch[port],(u32)&(tg_UART_Reg[port]->DR),
            (u32)DmaSendBuf,DMA_DIR_M2P,DMA_DATABITS_8,DMA_DATABITS_8,32);

    DMA_IntEnable(UartDmaTxStream[port],DMA_INT_TCIE);  //ʹ�ܷ�������ж�
    DMA_IntEnable(UartDmaRxStream[port],DMA_INT_HTIE);  //ʹ�ܽ��հ�����ж�

    tg_UART_Reg[port]->CR3 |= 0x00c0;  //���ô���ʹ��dma�շ�
    s_UART_DmaUsed[port] = CN_DMA_USED;
    DMA_Enable(UartDmaRxStream[port],(u32)DmaRecvBuf,32);//����dmaͨ��

    Int_Register(UartDmaTxInt[port]);
    Int_Register(UartDmaRxInt[port]);//ע�����DMA�ж�
    Int_SetClearType(UartDmaRxInt[port],CN_INT_CLEAR_AUTO);
    Int_IsrConnect(UartDmaRxInt[port],UART_DmaRx_ISR);
    Int_SetClearType(UartDmaTxInt[port],CN_INT_CLEAR_AUTO);
    Int_IsrConnect(UartDmaTxInt[port],UART_DmaTx_ISR);

    Int_SetIsrPara(UartDmaTxInt[port],port);
    Int_SetIsrPara(UartDmaRxInt[port],port);

    Int_SettoAsynSignal(UartDmaRxInt[port]);
    Int_SettoAsynSignal(UartDmaTxInt[port]);
    Int_RestoreAsynLine(UartDmaRxInt[port]);
    Int_RestoreAsynLine(UartDmaTxInt[port]);
#endif
}

//=============================================================================
// ����: ���ò�ʹ��DMA��UART��ʹ��DMA�շ���ÿ�շ�һ���ֽھͱ����ж�һ�Σ��������ڸ���
//      ����ͨ����ʹ�á�
// ����: port,�˿ںţ������ںţ�Ϊ1~3
// ����: ��
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
    else
        return;

    free(pUART_DmaRecvBuf[port]);
    pUART_DmaRecvBuf[port]=NULL;
    free(pUART_DmaSendBuf[port]);
    pUART_DmaSendBuf[port]=NULL;

#if 0
    tg_UART_Reg[port]->CR3 &= ~0x00c0;     //���ò�ʹ��dma����
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
#endif
}
// =============================================================================
// ����: �����豸�Ŀ��ƺ���,������Ӳ���Ĵ����������
// ����: Reg,�������Ĵ��ڼĴ���ָ��.
//       cmd,��������
//       data1,data2,������cmd����
// ����: ������.
// =============================================================================

ptu32_t art43x_UartCtrl(ptu32_t PrivateTag, u32 cmd, va_list* arg0)
{
	struct uart_priv *priv;
	usart_type *usart_x;
	uint32_t baud;


	priv = (void *)PrivateTag;
	usart_x = NULL;
	if(priv) usart_x = to_usart_type(priv->port);
	if (!usart_x) return -1;

	switch (cmd) {
	case CN_DEV_CTRL_START:
		usart_init(usart_x, 115200 , USART_DATA_8BITS, USART_STOP_1_BIT);
		usart_transmitter_enable(usart_x, TRUE);
		usart_receiver_enable(usart_x, TRUE);
		usart_enable(usart_x, TRUE);
		break;
	case CN_UART_SET_BAUD:
		baud = va_arg(*arg0, uint32_t);
		usart_init(usart_x, baud, USART_DATA_8BITS, USART_STOP_1_BIT);
		break;
	case CN_UART_COM_SET:
		break;
	default:
		break;
	}

	return 0;
}

// =============================================================================
// ����: ��uart2���ó�dma��ʽ�ǣ�dma1 ch7�����ͣ�ch6�������
// ����: �жϺ���û�в���.
// ����: �жϺ���û�з���ֵ.
// =============================================================================
uint32_t UART_DmaTx_ISR(ptu32_t port)
{
    uint16_t num;
    u8 *DmaSendBuf;
    u32 DmaSendBufLen;

//    switch(port)
//    {
//    case CN_UART1:
//	DmaSendBufLen = CFG_UART1_DMABUF_LEN;        break;
//    case CN_UART2:
//	DmaSendBufLen = CFG_UART2_DMABUF_LEN;        break;
//    case CN_UART3:
//	DmaSendBufLen = CFG_UART3_DMABUF_LEN;        break;
//    case CN_UART4:
//	DmaSendBufLen = CFG_UART4_DMABUF_LEN;        break;
//    case CN_UART5:
//	DmaSendBufLen = CFG_UART5_DMABUF_LEN;        break;
//    case CN_UART6:
//	DmaSendBufLen = CFG_UART6_DMABUF_LEN;        break;
//    default:        break;
//    }
//
//    DmaSendBuf = pUART_DmaSendBuf[port];
//    DMA_ClearIntFlag(UartDmaTxStream[port]);
//    num = UART_PortRead(pUartCB[port],DmaSendBuf,DmaSendBufLen);
//    if(num == 0)
//    {
//	s_UART_DmaSending[port] = false;
//	tg_UART_Reg[port]->CR1 |= (1<<6);
//    }
//    else
//    {
//	DMA_Enable(UartDmaTxStream[port],(u32)DmaSendBuf,num);
//    }
    return 0;
}

// =============================================================================
// ����: ��uart2���ó�dma��ʽ�ǣ�dma1 ch7�����ͣ�ch6�������
// ����: �жϺ���û�в���.
// ����: �жϺ���û�з���ֵ.
// =============================================================================
uint32_t UART_DmaRx_ISR(ptu32_t port)
{
    u32 recvs,num,DmaRecvBufLen;
    u8 *DmaRecvBuf;

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
    default:
        break;
    }

    DmaRecvBuf = pUART_DmaRecvBuf[port];
    DMA_Disable(UartDmaRxStream[port]);
    DMA_ClearIntFlag(UartDmaRxStream[port]);
    // recvs = DmaRecvBufLen - UartDmaRxStream[port]->NDTR;
    if(recvs == DmaRecvBufLen)     //dma�������������������ݶ�ʧ
    {
//	UART_ErrHandle(pUartCB[port],CN_UART_FIFO_OVER_ERR);
    }else
    {
//	num = UART_PortWrite(pUartCB[port],DmaRecvBuf,recvs);
	if(num != recvs)
	{
//	    UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
	}
    }

    DMA_Enable(UartDmaRxStream[port],(u32)DmaRecvBuf,DmaRecvBufLen);
    return 0;
}

// =============================================================================
// ����: UART���жϷ��������������Ϳա����ա����������жϡ�
//      1.�����жϣ��ӽ���FIFO�ж����ݣ�������UART_PortWriteд����ջ������У�
//      2.�����жϣ�����UART_PortRead�����ͻ����������ݣ���д�뷢��FIFO��
//      3.�����жϣ����������¼�
// ����: �жϺ�.
// ����: 0.
// =============================================================================




u32 UART_ISR(ptu32_t port)
{
    struct UartGeneralCB *UCB;
    int intno = EXINT_LINE_0;

    struct UartParam *param;
    struct uart_priv *priv;
    usart_type* usart_x;
    flag_status flag;
    u8 c[1];
    uint16_t data;
    int rc;

    uint32_t sts;

    if (port > ARRAY_SIZE(art43x_UART_Param))
	    return -1;
    param = art43x_UART_Param + port;

    priv = param->UartPortTag;
    if (!priv) {
	    /* port error. */
	    return -1;
    }

    usart_x = to_usart_type(priv->port);
    sts = usart_x->sts;

    if (sts & USART_RDBF_FLAG) {
	    data = usart_data_receive(usart_x);
	    c[0] = data;
	    rc = UART_PortWrite(priv->pUartCB, c, 1U);
    }

    if (sts & USART_TDBE_FLAG) {
	    rc = UART_PortRead(priv->pUartCB, c, 1U);
	    data = c[0];

	    if (rc) usart_data_transmit(usart_x, data);
	    else usart_interrupt_enable(usart_x, USART_IDLE_INT, FALSE);

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


ptu32_t ModuleInstall_UART(u32 serial_no)
{
    struct UartParam *param;
    struct uart_priv* priv;
    int port = serial_no;

    if (port < 0 || port >= ARRAY_SIZE(art43x_UART_Param))
	    return -1;

    param = art43x_UART_Param + port;
    priv = (void *)param->UartPortTag;
    if (priv)
	    return -1;	/* �Ѿ���ʼ���ˡ� */

    priv = malloc(sizeof(struct uart_priv));
    priv->port = port;
    param = art43x_UART_Param + priv->port;

    if (param->TxRingBufLen) priv->TxRingBuf = malloc(param->TxRingBufLen);
    else priv->TxRingBuf = NULL;

    if (param->RxRingBufLen) priv->RxRingBuf = malloc(param->RxRingBufLen);
    else priv->RxRingBuf = NULL;

    priv->dma_rx_buf = NULL;
    priv->dma_tx_buf = NULL;
    priv->dmabuf_len = 0;

    param->UartPortTag = (ptu32_t)priv;
    priv->pUartCB = UART_InstallGeneral(param);

    param->UartCtrl(param->UartPortTag, CN_DEV_CTRL_START, NULL);

    /* TODO: mod... */
    __UART_IntInit(port);
    usart_interrupt_enable(to_usart_type(priv->port), USART_RDBF_INT, TRUE);
    usart_interrupt_enable(to_usart_type(priv->port), USART_IDLE_INT, TRUE);

    if (param->UartCtrl) {
	    param->UartCtrl(param->UartPortTag, 0, NULL);
    }

    return priv->pUartCB ? 1 : 0;
}

// =============================================================================
// ���ܣ��ַ��ն�ֱ�ӷ��ͺ�����������ѯ��ʽ��ֱ��д�Ĵ���������printk������stdout
//       û�г�ʼ��
// ������str�������ַ���ָ��
//      len�����͵��ֽ���
// ���أ�0����������result,�������ݳ��ȣ��ֽڵ�λ
// =============================================================================
static s32 art43x_PutStrDirect(const char *str, u32 len)
{
	int i, port = TxDirectPort;
	usart_type *usart_x;

	if (port < 0 || port >= ARRAY_SIZE(art43x_UART_Param)) port = 0;
	usart_x = to_usart_type(port);

	for (i = 0; i < len; i++) {
		uint16_t ch;
		ch = str[i];
		while (usart_flag_get(usart_x, USART_TDBE_FLAG) == RESET);
		usart_data_transmit(usart_x, ch);
	}
	return i;
}

// =============================================================================
// ���ܣ��ַ��ն�ֱ�ӽ��պ�����������ѯ��ʽ��ֱ�Ӷ��Ĵ���������stdin��ʼ��ǰ
// ������str�������ַ���ָ��
//      len�����͵��ֽ���
// ���أ�0����������result,�������ݳ��ȣ��ֽڵ�λ
// =============================================================================

static char art43x_GetCharDirect(void)
{
	usart_type *usart_x;
	u32 port = RxDirectPort;
	uint16_t ch;

	usart_x = to_usart_type(port);
	while (usart_flag_get(usart_x, USART_RDBF_FLAG) == RESET);
	ch = usart_data_receive(usart_x);
	return ch;
}


int devname_to_port(char* str)
{
	int port;
	const char *devname[] = {
		"/dev/UART1",
		"/dev/UART2",
		"/dev/UART3",
		"/dev/UART4",
		"/dev/UART5",
		"/dev/UART6",
		"/dev/UART7",
		"/dev/UART8",
		"/dev/UART9",
		"/dev/UART10",
		"/dev/UART11",
		"/dev/UART12",
	};

	for (port = 0; port < ARRAY_SIZE(devname); port++)
		if (strcmp(str, devname[port]) == 0) return port;
	return -1;
}

//----��ʼ���ں˼�IO-----------------------------------------------------------
//���ܣ���ʼ���ں˼��������������runtime����ָ�롣
//��������
//���أ���
//-----------------------------------------------------------------------------
void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut)
{
	struct UartParam* param;
	int port;

	port = devname_to_port(StdioOut);
	if (port < 0) TxDirectPort = 0; /* default to UART1 */

	param = art43x_UART_Param + port;

	if(param && param->UartCtrl)
		param->UartCtrl(param->UartPortTag, CN_DEV_CTRL_START, NULL);
	PutStrDirect = art43x_PutStrDirect;
	TxByteTime = 95;

	port = devname_to_port(StdioIn);
	if (port < 0) RxDirectPort = 0; /* default to UART1. */
	GetCharDirect = art43x_GetCharDirect;
}
