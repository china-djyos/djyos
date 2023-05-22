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
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip uart"//CPU的uart外设驱动
//parent:"uart device file"        //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                    //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                 //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                   //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                  //初始化时机，可选值：early，medium，later, pre-main。
                                   //表示初始化时间，分别是早期、中期、后期
//dependence:"uart device file"    //该组件的依赖组件名（可以是none，表示无依赖组件），
                                   //选中该组件时，被依赖组件将强制选中，
                                   //如果依赖多个组件，则依次列出
//weakdependence:"none"            //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                   //选中该组件时，被依赖组件不会被强制选中，
                                   //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                     //该组件的互斥组件名（可以是none，表示无互斥组件），
                                   //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue  	****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_UART == false )
//#warning  " cpu_onchip_uart  组件参数未配置，使用默认配置"
//%$#@target = header   		//header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_UART    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,512,
#define CFG_UART1_SENDBUF_LEN       64      //"UART1发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN       64      //"UART1接收环形缓冲区大小",
#define CFG_UART1_DMABUF_LEN        64      //"UART1 DMA环形缓冲区大小",

#define CFG_UART2_SENDBUF_LEN       64      //"UART2发送环形缓冲区大小",
#define CFG_UART2_RECVBUF_LEN       64      //"UART2接收环形缓冲区大小",
#define CFG_UART2_DMABUF_LEN        64      //"UART2 DMA环形缓冲区大小",

#define CFG_UART3_SENDBUF_LEN       64      //"UART3发送环形缓冲区大小",
#define CFG_UART3_RECVBUF_LEN       64      //"UART3接收环形缓冲区大小",
#define CFG_UART3_DMABUF_LEN        64      //"UART3 DMA环形缓冲区大小",

#define CFG_UART4_SENDBUF_LEN       64      //"UART4发送环形缓冲区大小",
#define CFG_UART4_RECVBUF_LEN       64      //"UART4接收环形缓冲区大小",
#define CFG_UART4_DMABUF_LEN        64      //"UART4 DMA环形缓冲区大小",

#define CFG_UART5_SENDBUF_LEN       64      //"UART5发送环形缓冲区大小",
#define CFG_UART5_RECVBUF_LEN       64      //"UART5接收环形缓冲区大小",
#define CFG_UART5_DMABUF_LEN        64      //"UART5 DMA环形缓冲区大小",

#define CFG_UART6_SENDBUF_LEN       64      //"UART6发送环形缓冲区大小",
#define CFG_UART6_RECVBUF_LEN       64      //"UART6接收环形缓冲区大小",
#define CFG_UART6_DMABUF_LEN        64      //"UART6 DMA环形缓冲区大小",

#define CFG_UART7_SENDBUF_LEN       64      //"UART7发送环形缓冲区大小",
#define CFG_UART7_RECVBUF_LEN       64      //"UART7接收环形缓冲区大小",
#define CFG_UART7_DMABUF_LEN        64      //"UART7 DMA环形缓冲区大小",

#define CFG_UART8_SENDBUF_LEN       64      //"UART8发送环形缓冲区大小",
#define CFG_UART8_RECVBUF_LEN       64      //"UART8接收环形缓冲区大小",
#define CFG_UART8_DMABUF_LEN        64      //"UART8 DMA环形缓冲区大小",

//%$#@enum,true,false
#define CFG_UART1_ENABLE           true        //"是否使用UART1",
#define CFG_UART1_ENABLE_DMA       false       //"UART1使能DMA",
#define CFG_UART2_ENABLE           false       //"是否使用UART2",
#define CFG_UART2_ENABLE_DMA       false       //"UART2使能DMA",
#define CFG_UART3_ENABLE           false       //"是否使用UART3",
#define CFG_UART3_ENABLE_DMA       false       //"UART3使能DMA",
#define CFG_UART4_ENABLE           false       //"是否使用UART4",
#define CFG_UART4_ENABLE_DMA       false       //"UART4使能DMA",
#define CFG_UART5_ENABLE           false       //"是否使用UART5",
#define CFG_UART5_ENABLE_DMA       false       //"UART5使能DMA",
#define CFG_UART6_ENABLE           false       //"是否使用UART6",
#define CFG_UART6_ENABLE_DMA       false       //"UART6使能DMA",
#define CFG_UART7_ENABLE           false       //"是否使用UART7",
#define CFG_UART7_ENABLE_DMA       false       //"UART7使能DMA",
#define CFG_UART8_ENABLE           false       //"是否使用UART8",
#define CFG_UART8_ENABLE_DMA       false       //"UART8使能DMA",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束
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

