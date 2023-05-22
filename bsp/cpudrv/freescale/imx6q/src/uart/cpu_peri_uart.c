// SPDX-License-Identifier: GPL-2.0+
/*
 * (c) 2022-2023 xxx <xxx@djyos.org
 *
 * drivers/serial/serial_mxc.c
 * (c) 2007 Sascha Hauer <s.hauer@pengutronix.de>
 */

#include "board-config.h"
#include <stdio.h>
#include "stdint.h"
#include "string.h"
#include "errno.h"
#include "systime.h"
#include "cpu_peri.h"
#include "int.h"
#include "djyos.h"
#include <device/djy_uart.h>
#include "string.h"
#include "cpu_peri_uart.h"
//#include "cpu_peri_dma.h"
// =============================================================================
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
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip uart"//CPU��uart��������
//parent:"uart device file"        //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                    //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                 //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                   //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                  //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                   //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"uart device file"    //������������������������none����ʾ�������������
                                   //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                   //����������������������г�
//weakdependence:"none"            //��������������������������none����ʾ�������������
                                   //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                   //����������������������г����á�,���ָ�
//mutex:"none"                     //������Ļ����������������none����ʾ�޻����������
                                   //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue  	****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_UART == false )
//#warning  " cpu_onchip_uart  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header   		//header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_UART    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,512,
#define CFG_UART1_SENDBUF_LEN       64      //"UART1���ͻ��λ�������С",
#define CFG_UART1_RECVBUF_LEN       64      //"UART1���ջ��λ�������С",
#define CFG_UART1_DMABUF_LEN        64      //"UART1 DMA���λ�������С",

#define CFG_UART2_SENDBUF_LEN       64      //"UART2���ͻ��λ�������С",
#define CFG_UART2_RECVBUF_LEN       64      //"UART2���ջ��λ�������С",
#define CFG_UART2_DMABUF_LEN        64      //"UART2 DMA���λ�������С",

#define CFG_UART3_SENDBUF_LEN       64      //"UART3���ͻ��λ�������С",
#define CFG_UART3_RECVBUF_LEN       64      //"UART3���ջ��λ�������С",
#define CFG_UART3_DMABUF_LEN        64      //"UART3 DMA���λ�������С",

#define CFG_UART4_SENDBUF_LEN       64      //"UART4���ͻ��λ�������С",
#define CFG_UART4_RECVBUF_LEN       64      //"UART4���ջ��λ�������С",
#define CFG_UART4_DMABUF_LEN        64      //"UART4 DMA���λ�������С",

#define CFG_UART5_SENDBUF_LEN       64      //"UART5���ͻ��λ�������С",
#define CFG_UART5_RECVBUF_LEN       64      //"UART5���ջ��λ�������С",
#define CFG_UART5_DMABUF_LEN        64      //"UART5 DMA���λ�������С",

#define CFG_UART6_SENDBUF_LEN       64      //"UART6���ͻ��λ�������С",
#define CFG_UART6_RECVBUF_LEN       64      //"UART6���ջ��λ�������С",
#define CFG_UART6_DMABUF_LEN        64      //"UART6 DMA���λ�������С",

#define CFG_UART7_SENDBUF_LEN       64      //"UART7���ͻ��λ�������С",
#define CFG_UART7_RECVBUF_LEN       64      //"UART7���ջ��λ�������С",
#define CFG_UART7_DMABUF_LEN        64      //"UART7 DMA���λ�������С",

#define CFG_UART8_SENDBUF_LEN       64      //"UART8���ͻ��λ�������С",
#define CFG_UART8_RECVBUF_LEN       64      //"UART8���ջ��λ�������С",
#define CFG_UART8_DMABUF_LEN        64      //"UART8 DMA���λ�������С",

//%$#@enum,true,false
#define CFG_UART1_ENABLE           true        //"�Ƿ�ʹ��UART1",
#define CFG_UART1_ENABLE_DMA       false       //"UART1ʹ��DMA",
#define CFG_UART2_ENABLE           false       //"�Ƿ�ʹ��UART2",
#define CFG_UART2_ENABLE_DMA       false       //"UART2ʹ��DMA",
#define CFG_UART3_ENABLE           false       //"�Ƿ�ʹ��UART3",
#define CFG_UART3_ENABLE_DMA       false       //"UART3ʹ��DMA",
#define CFG_UART4_ENABLE           false       //"�Ƿ�ʹ��UART4",
#define CFG_UART4_ENABLE_DMA       false       //"UART4ʹ��DMA",
#define CFG_UART5_ENABLE           false       //"�Ƿ�ʹ��UART5",
#define CFG_UART5_ENABLE_DMA       false       //"UART5ʹ��DMA",
#define CFG_UART6_ENABLE           false       //"�Ƿ�ʹ��UART6",
#define CFG_UART6_ENABLE_DMA       false       //"UART6ʹ��DMA",
#define CFG_UART7_ENABLE           false       //"�Ƿ�ʹ��UART7",
#define CFG_UART7_ENABLE_DMA       false       //"UART7ʹ��DMA",
#define CFG_UART8_ENABLE           false       //"�Ƿ�ʹ��UART8",
#define CFG_UART8_ENABLE_DMA       false       //"UART8ʹ��DMA",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������
//@#$%component end configure
// =============================================================================

//#define CFG_UART0_SENDBUF_LEN 		   256
//#define CFG_UART0_RECVBUF_LEN 		   256
//
//#define CFG_UART1_SENDBUF_LEN 		   256
//#define CFG_UART1_RECVBUF_LEN 		   256
//
//#define CFG_UART2_SENDBUF_LEN 		   256
//#define CFG_UART2_RECVBUF_LEN 		   256



/* UART Control Register Bit Fields.*/
#define URXD_CHARRDY	(1<<15)
#define URXD_ERR	(1<<14)
#define URXD_OVRRUN	(1<<13)
#define URXD_FRMERR	(1<<12)
#define URXD_BRK	(1<<11)
#define URXD_PRERR	(1<<10)
#define URXD_RX_DATA	(0xFF)
#define UCR1_ADEN	(1<<15) /* Auto dectect interrupt */
#define UCR1_ADBR	(1<<14) /* Auto detect baud rate */
#define UCR1_TRDYEN	(1<<13) /* Transmitter ready interrupt enable */
#define UCR1_IDEN	(1<<12) /* Idle condition interrupt */
#define UCR1_RRDYEN	(1<<9)	/* Recv ready interrupt enable */
#define UCR1_RDMAEN	(1<<8)	/* Recv ready DMA enable */
#define UCR1_IREN	(1<<7)	/* Infrared interface enable */
#define UCR1_TXMPTYEN	(1<<6)	/* Transimitter empty interrupt enable */
#define UCR1_RTSDEN	(1<<5)	/* RTS delta interrupt enable */
#define UCR1_SNDBRK	(1<<4)	/* Send break */
#define UCR1_TDMAEN	(1<<3)	/* Transmitter ready DMA enable */
#define UCR1_UARTCLKEN	(1<<2)	/* UART clock enabled */
#define UCR1_DOZE	(1<<1)	/* Doze */
#define UCR1_UARTEN	(1<<0)	/* UART enabled */
#define UCR2_ESCI	(1<<15) /* Escape seq interrupt enable */
#define UCR2_IRTS	(1<<14) /* Ignore RTS pin */
#define UCR2_CTSC	(1<<13) /* CTS pin control */
#define UCR2_CTS	(1<<12) /* Clear to send */
#define UCR2_ESCEN	(1<<11) /* Escape enable */
#define UCR2_PREN	(1<<8)  /* Parity enable */
#define UCR2_PROE	(1<<7)  /* Parity odd/even */
#define UCR2_STPB	(1<<6)	/* Stop */
#define UCR2_WS		(1<<5)	/* Word size */
#define UCR2_RTSEN	(1<<4)	/* Request to send interrupt enable */
#define UCR2_TXEN	(1<<2)	/* Transmitter enabled */
#define UCR2_RXEN	(1<<1)	/* Receiver enabled */
#define UCR2_SRST	(1<<0)	/* SW reset */
#define UCR3_DTREN	(1<<13) /* DTR interrupt enable */
#define UCR3_PARERREN	(1<<12) /* Parity enable */
#define UCR3_FRAERREN	(1<<11) /* Frame error interrupt enable */
#define UCR3_DSR	(1<<10) /* Data set ready */
#define UCR3_DCD	(1<<9)  /* Data carrier detect */
#define UCR3_RI		(1<<8)  /* Ring indicator */
#define UCR3_ADNIMP	(1<<7)  /* Autobaud Detection Not Improved */
#define UCR3_RXDSEN	(1<<6)  /* Receive status interrupt enable */
#define UCR3_AIRINTEN	(1<<5)  /* Async IR wake interrupt enable */
#define UCR3_AWAKEN	(1<<4)  /* Async wake interrupt enable */
#define UCR3_REF25	(1<<3)  /* Ref freq 25 MHz */
#define UCR3_REF30	(1<<2)  /* Ref Freq 30 MHz */

/* imx8 names these bitsfields instead: */
#define UCR3_DTRDEN	BIT(3)  /* bit not used in this chip */
#define UCR3_RXDMUXSEL	BIT(2)  /* RXD muxed input selected; 'should always be set' */

#define UCR3_INVT	(1<<1)  /* Inverted Infrared transmission */
#define UCR3_BPEN	(1<<0)  /* Preset registers enable */
#define UCR4_CTSTL_32	(32<<10) /* CTS trigger level (32 chars) */
#define UCR4_INVR	(1<<9)  /* Inverted infrared reception */
#define UCR4_ENIRI	(1<<8)  /* Serial infrared interrupt enable */
#define UCR4_WKEN	(1<<7)  /* Wake interrupt enable */
#define UCR4_REF16	(1<<6)  /* Ref freq 16 MHz */
#define UCR4_IRSC	(1<<5)  /* IR special case */
#define UCR4_TCEN	(1<<3)  /* Transmit complete interrupt enable */
#define UCR4_BKEN	(1<<2)  /* Break condition interrupt enable */
#define UCR4_OREN	(1<<1)  /* Receiver overrun interrupt enable */
#define UCR4_DREN	(1<<0)  /* Recv data ready interrupt enable */
#define UFCR_RXTL_SHF	0       /* Receiver trigger level shift */
#define UFCR_RFDIV	(7<<7)  /* Reference freq divider mask */
#define UFCR_RFDIV_SHF	7	/* Reference freq divider shift */
#define RFDIV		4	/* divide input clock by 2 */
#define UFCR_DCEDTE	(1<<6)  /* DTE mode select */
#define UFCR_TXTL_SHF	10      /* Transmitter trigger level shift */
#define USR1_PARITYERR	(1<<15) /* Parity error interrupt flag */
#define USR1_RTSS	(1<<14) /* RTS pin status */
#define USR1_TRDY	(1<<13) /* Transmitter ready interrupt/dma flag */
#define USR1_RTSD	(1<<12) /* RTS delta */
#define USR1_ESCF	(1<<11) /* Escape seq interrupt flag */
#define USR1_FRAMERR	(1<<10) /* Frame error interrupt flag */
#define USR1_RRDY	(1<<9)	/* Receiver ready interrupt/dma flag */
#define USR1_TIMEOUT	(1<<7)	/* Receive timeout interrupt status */
#define USR1_RXDS	(1<<6)	/* Receiver idle interrupt flag */
#define USR1_AIRINT	(1<<5)	/* Async IR wake interrupt flag */
#define USR1_AWAKE	(1<<4)	/* Aysnc wake interrupt flag */
#define USR2_ADET	(1<<15) /* Auto baud rate detect complete */
#define USR2_TXFE	(1<<14) /* Transmit buffer FIFO empty */
#define USR2_DTRF	(1<<13) /* DTR edge interrupt flag */
#define USR2_IDLE	(1<<12) /* Idle condition */
#define USR2_IRINT	(1<<8)	/* Serial infrared interrupt flag */
#define USR2_WAKE	(1<<7)	/* Wake */
#define USR2_RTSF	(1<<4)	/* RTS edge interrupt flag */
#define USR2_TXDC	(1<<3)	/* Transmitter complete */
#define USR2_BRCD	(1<<2)	/* Break condition */
#define USR2_ORE	(1<<1)	/* Overrun error */
#define USR2_RDR	(1<<0)	/* Recv data ready */
#define UTS_FRCPERR	(1<<13) /* Force parity error */
#define UTS_LOOP	(1<<12) /* Loop tx and rx */
#define UTS_TXEMPTY	(1<<6)	/* TxFIFO empty */
#define UTS_RXEMPTY	(1<<5)	/* RxFIFO empty */
#define UTS_TXFULL	(1<<4)	/* TxFIFO full */
#define UTS_RXFULL	(1<<3)	/* RxFIFO full */
#define UTS_SOFTRS	(1<<0)	/* Software reset */
#define TXTL		2  /* reset default */
#define RXTL		1  /* reset default */

struct mxc_uart {
	u32 rxd;
	u32 spare0[15];

	u32 txd;
	u32 spare1[15];

	u32 cr1;
	u32 cr2;
	u32 cr3;
	u32 cr4;

	u32 fcr;
	u32 sr1;
	u32 sr2;
	u32 esc;

	u32 tim;
	u32 bir;
	u32 bmr;
	u32 brc;

	u32 onems;
	u32 ts;
};

#define ATZ1_BASE_ADDR              0x02000000
#define UART1_BASE                  (ATZ1_BASE_ADDR + 0x20000)

static u32 imxStartSend(ptu32_t PrivateTag)
{
	return 0;
}

static ptu32_t imxUartCtrl(ptu32_t PrivateTag,u32 cmd, va_list *arg0)
{
	return 0;
}

struct imxUartPort {
	int port;
	struct UartGeneralCB *UartGeneralCB;
};

static struct imxUartPort imxUartPort[1] = {
	[0] = {
		.port = 0,
		.UartGeneralCB = NULL,
	},
};

static struct UartParam imxUartParam[1] = {
	[0] = {

		.Name          = "UART0",
		.TxRingBufLen  = 256,
		.RxRingBufLen  = 256,
		.Baud          = 115200,
		.mode          = CN_UART_GENERAL,
		.UartPortTag   = 0,
		.StartSend     = imxStartSend,
		.UartCtrl      = imxUartCtrl,
	},
};

u32 imx_get_uartclk(void)
{
    u32 clk = 80000000; /* read from jtag. */
    return clk;
}

static void mxc_serial_setbrg(void)
{
    u32 clk = imx_get_uartclk();
    volatile struct mxc_uart *mxc_base = (void *)0x02020000;
    int baudrate = 115200; // CONFIG_BAUDRATE;

    mxc_base->fcr = 4 << 7; /* divide input clock by 2 */
    mxc_base->bir = 0xf;
    mxc_base->bmr = clk / (2 * baudrate);

}

ptu32_t ModuleInstall_UART(ptu32_t serial_no)
{
	struct UartParam *Param;
	struct imxUartPort *up;

    volatile struct mxc_uart *mxc_base;
    mxc_base = (void *)0x02020000;

	/* mxc_serial_probe */
    mxc_base->cr1 = 0;
    mxc_base->cr2 = 0;
    while (mxc_base->cr2 & UCR2_SRST == 0);
    mxc_base->cr3 = 0x0704 | UCR3_ADNIMP;
    mxc_base->cr4 = 0x8000;
    mxc_base->esc = 0x002b;
    mxc_base->tim = 0;
    mxc_base->ts = 0;
	/* serial_setbrg */
    mxc_serial_setbrg();

    mxc_base->cr2 = UCR2_WS | UCR2_IRTS | UCR2_RXEN | UCR2_TXEN | UCR2_SRST;
    mxc_base->cr1 = UCR1_UARTEN;

	serial_no = 0;
	Param = &imxUartParam[serial_no];
	up = &imxUartPort[serial_no];

//	up->UartGeneralCB = UART_InstallGeneral(Param);
}

s32 imxPutStrDirect(const char *buf, u32 len)
{
    int c;
    volatile struct mxc_uart *mxc_base;

    mxc_base = (void *)0x02020000;

    while (len) {
	/* write c to hardware. */
        while (mxc_base->ts & UTS_TXEMPTY == 0);
		c = *buf++;
        mxc_base->txd = c;
        len--;
    }
}

char imxGetCharDirect(void)
{
    int c;
    volatile struct mxc_uart *mxc_base = (void *)0x02020000;

	while (mxc_base->ts & UTS_RXEMPTY);
	c = mxc_base->rxd & URXD_RX_DATA;
    return c;
}

void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut)
{
	PutStrDirect = imxPutStrDirect;
	GetCharDirect = imxGetCharDirect;
}

