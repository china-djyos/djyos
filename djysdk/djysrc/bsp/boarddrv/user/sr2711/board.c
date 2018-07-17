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
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：board.c
// 模块描述: sr5260-v1.0板件相关部分初始化或配置等
// 模块版本: V1.00
// 创建人员: zhb
// 创建时间: 7/5.2017
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "cpu_peri_gpio.h"
#include "cpu_peri_iic.h"
#include "cpu_peri.h"

#include "stm32f7xx_hal_conf.h"
#include "stm32f7xx_ll_system.h"
#include "ad_sr2711.h"
#include "Board-config.h"
#include "cpu_peri_lptimer.h"
//这里的gpio只定义了一部分 复用到不同的GPIO还需根据手册添加


//DEBUG口   wifi调试口
static const Pin uart1_pin[] = {
		{GPIO_A,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
		{GPIO_B,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
};

void UART1_GpioInit(void)
{
	 PIO_Configure(uart1_pin, PIO_LISTSIZE(uart1_pin));
}
//232RXD3/TXD3 RF接口
const Pin uart3_pin[] = {
		{GPIO_B,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
		{GPIO_B,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
};

void UART3_GpioInit(void)
{
	 PIO_Configure(uart3_pin, PIO_LISTSIZE(uart3_pin));
}

//485 RXD4/TXD4   球机接口
const Pin uart4_pin[] = {
		{GPIO_H,PIN13,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
		{GPIO_H,PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
		{GPIO_A,PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
};

void UART4_GpioInit(void)
{
	 PIO_Configure(uart4_pin, PIO_LISTSIZE(uart4_pin));
}

//232RXD5/TXD5  4G模块
const Pin uart5_pin[] = {
		{GPIO_B,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
		{GPIO_B,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
};
void UART5_GpioInit(void)
{
	 PIO_Configure(uart5_pin, PIO_LISTSIZE(uart5_pin));
}

//485_1  扩展5个485 连接传感器
const Pin uart6_pin[] = {
		{GPIO_G,PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
		{GPIO_C,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
		{GPIO_G,PIN12,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},  //RTS1
		{GPIO_A,PIN8,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},   //RTS2
		{GPIO_A,PIN9,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},   //RTS3
		{GPIO_B,PIN0,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},   //RTS4
		{GPIO_B,PIN1,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},   //RTS5
};

void UART6_GpioInit(void)
{
	 PIO_Configure(uart6_pin, PIO_LISTSIZE(uart6_pin));
}


//485_2  图像采集单元接口
//SR2711-V1.1.0板删除该功能  20170929
//const Pin uart7_pin[] = {
//		{GPIO_F,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
//		{GPIO_F,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
//		{GPIO_F,PIN9,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
//};

const Pin MMC[6] = {
	{GPIO_C, PIN8, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_PU, AF12}, 	// SDMMC1_D0
	{GPIO_C, PIN9, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_PU, AF12}, 	// SDMMC1_D1
	{GPIO_C, PIN10, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_PU, AF12}, 	// SDMMC1_D2
	{GPIO_C, PIN11, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_PU, AF12}, 	// SDMMC1_D3
	{GPIO_C, PIN12, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_NONE, AF12}, 	// SDMMC1_CLK
	{GPIO_D, PIN2, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_PU, AF12}, 	// SDMMC1_CMD
	//{GPIO_G, PIN3, GPIO_MODE_OUT, GPIO_OTYPE_PP,  GPIO_SPEED_H, GPIO_PUPD_NONE}, 		// SDMMC1_RSTn
	//{GPIO_H, PIN15, GPIO_MODE_IN, GPIO_OTYPE_PP, GPIO_SPEED_H, GPIO_PUPD_NONE}, 		// SDMMC1_DS
};

const Pin USB_FS[2] = {
	{GPIO_A, PIN11, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_NONE, AF10},
	{GPIO_A, PIN12, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_NONE, AF10},
};

const Pin USB_HS[2] = {
	//{GPIO_H, PIN2, GPIO_MODE_IN, GPIO_OTYPE_PP, GPIO_SPEED_H, GPIO_PUPD_NONE},
	//{GPIO_H, PIN3, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_H, GPIO_PUPD_NONE},
	//{GPIO_B, PIN13, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_NONE, AF12},
	{GPIO_B, PIN14, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_NONE, AF12},
	{GPIO_B, PIN15, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_NONE, AF12},
};

const Pin M4G_RST[1]={
	{GPIO_B, PIN5, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PU, AF_NUll},
};

const Pin M4G_WAKEUP[1]={
	{GPIO_H, PIN9, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_H, GPIO_PUPD_NONE},
};

//控制4G模块电源
const Pin M4G_POWER[1]={
	{GPIO_K, PIN7, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},
};

const Pin M4G_WAKEUP_INPUT[1]={
	{GPIO_C, PIN13, GPIO_MODE_IN, GPIO_OTYPE_PP, GPIO_SPEED_H, GPIO_PUPD_NONE},
};

//控制USB电源(该USB指用于U盘拷贝EMMC里数据的USB口)
const Pin USB_VCC_EN[1]={
		{GPIO_C, PIN2, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE,AF_NUll},
};


//SR2711-V1.1.0板删除该功能  20170929
//void UART7_GpioInit(void)
//{
//	 PIO_Configure(uart7_pin, PIO_LISTSIZE(uart7_pin));
//}

// ============================================================================
// 功能：设置串口传输通道 在发送之前设置一下串口传输通道号传输完成会自动调用收发转换
// 参数：SerialNo,串口号 Cs通道号
// 返回：无
// ============================================================================
static u8 Uart6_CsNo;

void Set_UartTransportChannel(u8 SerialNo,u8 CsNo)
{
    switch(SerialNo)
   {
      case CN_UART6:
          Uart6_CsNo = CsNo;  break;
      default:                break;
   }
}
// ============================================================================
// 功能：设置串口编号为SerialNo的串口为半双功发送功能
// 参数：SerialNo,串口号
// 返回：无
// ============================================================================
void Board_SetCsSend(u8 SerialNo,u8 CsNo)
{
	switch(SerialNo)
	{
	   case CN_UART6:
	   {
		   if(CsNo==0)
		   {
			   PIO_Set(&uart6_pin[2]);
		   }
		   else if(CsNo==1)
		   {
			   PIO_Set(&uart6_pin[3]);
		   }
		   else if(CsNo==2)
		   {
			   PIO_Set(&uart6_pin[4]);
		   }
		   else if(CsNo==3)
		   {
			   PIO_Set(&uart6_pin[5]);
		   }
		   else if(CsNo==4)
		   {
			   PIO_Set(&uart6_pin[6]);
		   }
		   else
		   {
			   printf("%s para is invalid.\r\n",__FUNCTION__);
		   }
	   }
	   break;
	default:
			break;
	}
}

// ============================================================================
// 功能：设置串口编号为SerialNo的串口为半双功发送功能
// 参数：SerialNo,串口号,0~7;
//     CsNo,片选号，0~4
// 返回：无
// ============================================================================
void Board_SetCsRecv(u8 SerialNo,u8 CsNo)
{
	switch(SerialNo)
	{
	   case CN_UART6:
	   {
		   if(CsNo==0)
		   {
			   PIO_Clear(&uart6_pin[2]);
		   }
		   else if(CsNo==1)
		   {
			   PIO_Clear(&uart6_pin[3]);
		   }
		   else if(CsNo==2)
		   {
			   PIO_Clear(&uart6_pin[4]);
		   }
		   else if(CsNo==3)
		   {
			   PIO_Clear(&uart6_pin[5]);
		   }
		   else if(CsNo==4)
		   {
			   PIO_Clear(&uart6_pin[6]);
		   }
		   else
		   {
			   printf("%s para is invalid.\r\n",__FUNCTION__);
		   }
	   }
	   break;
	   default:			break;
	}
}

// ============================================================================
// 功能：设置串口编号为SerialNo的串口为半双功发送功能
// 参数：SerialNo,串口号
// 返回：无
// ============================================================================
void Board_UartHalfDuplexSend(u8 SerialNo)
{
	switch(SerialNo)
	{
	case CN_UART4:
		PIO_Set(&uart4_pin[2]);
		break;
    case CN_UART6:
        Board_SetCsSend(CN_UART6,Uart6_CsNo);
        break;

        //SR2711-V1.1.0板删除该功能  20170929
//    case CN_UART7:
//        PIO_Set(&uart7_pin[2]);
//        break;
	default:
		break;
	}
}

// ============================================================================
// 功能：设置串口编号为SerialNo的串口为半双功接收功能
// 参数：SerialNo,串口号
// 返回：无
// ============================================================================
void Board_UartHalfDuplexRecv(u8 SerialNo)
{
	//拉低为485接收
	switch(SerialNo)
	{
	case CN_UART4:
		PIO_Clear(&uart4_pin[2]);
		break;
    case CN_UART6:
        Board_SetCsRecv(CN_UART6,Uart6_CsNo);
        break;
        //SR2711-V1.1.0板删除该功能  20170929
//    case CN_UART7:
//        PIO_Clear(&uart7_pin[2]);
//        break;
	default:
		break;
	}
}



void Board_UartGpioInit(void)
{
	 PIO_Configure(uart1_pin, PIO_LISTSIZE(uart1_pin));
	 PIO_Configure(uart3_pin, PIO_LISTSIZE(uart3_pin));
	 PIO_Configure(uart4_pin, PIO_LISTSIZE(uart4_pin));
	 PIO_Configure(uart5_pin, PIO_LISTSIZE(uart5_pin));
	 PIO_Configure(uart6_pin, PIO_LISTSIZE(uart6_pin));
	 //SR2711-V1.1.0板删除该功能  20170929
//	 PIO_Configure(uart7_pin, PIO_LISTSIZE(uart7_pin));
}


/*
      RMII_REF_CLK ----------------------> PA1
      RMII_MDIO -------------------------> PA2
      RMII_MDC --------------------------> PC1
      RMII_MII_CRS_DV -------------------> PA7
      RMII_MII_RXD0 ---------------------> PC4
      RMII_MII_RXD1 ---------------------> PC5
      RMII_MII_TX_EN --------------------> PG11
      RMII_MII_TXD0 ---------------------> PG13
      RMII_MII_TXD1 ---------------------> PB13
*/
static const Pin EthRmiiPins[] = {
	{GPIO_A,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},   //ETH_CLK
	{GPIO_A,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},   //ETH_MDIO
	{GPIO_A,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},   //ETH_CRS_DV
	{GPIO_B,PIN13,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},  //ETH_TXD1
	{GPIO_C,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},   //ETH_MDC
	{GPIO_C,PIN4,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},   //ETH_RXD0
	{GPIO_C,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},   //ETH_RXD1
	{GPIO_G,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},  //ETH_TX_EN
	{GPIO_G,PIN13,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},  //ETH_TXD0
};
static const Pin phyResetPin[] = {
	{GPIO_G,PIN9,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
};



void Eth_GpioInit(void)
{
	 //PHY 芯片
	 PIO_Configure(EthRmiiPins,PIO_LISTSIZE(EthRmiiPins));	//ETH RMII
	 PIO_Configure(phyResetPin,PIO_LISTSIZE(phyResetPin));
	 PIO_Clear(phyResetPin);	//66e6060 reset delay time > 10 ms
	 Djy_EventDelay(300*mS);
	 PIO_Set(phyResetPin);
}



//IIC2 for DS3232MZ
static const Pin I2C2_pin[] = {
        {GPIO_H,PIN4,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},//scl
        {GPIO_H,PIN5,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},//sda
};

void DS3232M_GpioInit(void)
{
	extern bool_t IIC_Busfree(u32 port,u32 sda_pin,u32 sck_pin);
	IIC_Busfree(GPIO_H,PIN5,PIN4);
    PIO_Configure(I2C2_pin, PIO_LISTSIZE(I2C2_pin));
}





//SDRAM
/* 功能 :------------PIN_NUM---------PIN
   R/B-------------150------------ PD6
   WP--------------145-------------PD3
   CE--------------152-------------PG9
   ALE-------------100-------------PD12
   WE--------------147-------------PD5
   OE--------------146-------------PD4
   CLE-------------99--------------PD11
   LocalBus_D0-----104-------------PD14
   LocalBus_D1---- 105-------------PD15
   LocalBus_D2-----142-------------PD0
   LocalBus_D3-----143-------------PD1
   LocalBus_D4-----68--------------PE7
   LocalBus_D5-----69--------------PE8
   LocalBus_D6-----70--------------PE9
   LocalBus_D7-----73--------------PE10
 */
const Pin FmcSdRamPins[39] = {
	{GPIO_C,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},//nWE
    {GPIO_C,PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //CKE

	{GPIO_D,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //LocalBus_D2 142  D0
	{GPIO_D,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //LocalBus_D3 143  D1
	{GPIO_D,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //LocalBus_D13 69   E8
	{GPIO_D,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //LocalBus_D14 70   E9
	{GPIO_D,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12}, //LocalBus_D15 73   E10
	{GPIO_D,PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12}, //LocalBus_D0 104  D14
	{GPIO_D,PIN15,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12}, //LocalBus_D1 105  D15

	{GPIO_E,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12}, //LDQM
	{GPIO_E,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12}, //UDQM
	{GPIO_E,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //LocalBus_D4 68   E7
	{GPIO_E,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //LocalBus_D5 69   E8
	{GPIO_E,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //LocalBus_D6 70   E9
	{GPIO_E,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12}, //LocalBus_D7 73   E10
	{GPIO_E,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12}, //LocalBus_D8 104  D14
	{GPIO_E,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12}, //LocalBus_D9 105  D15
	{GPIO_E,PIN13,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //LocalBus_D10 142  D0
	{GPIO_E,PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //LocalBus_D11 143  D1
	{GPIO_E,PIN15,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //LocalBus_D12 68   E7

	{GPIO_F,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12}, //LocalBus_A0 104  D14
	{GPIO_F,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12}, //LocalBus_A1 105  D15
	{GPIO_F,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //LocalBus_A2 142  D0
	{GPIO_F,PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //LocalBus_A3 143  D1
	{GPIO_F,PIN4,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //LocalBus_A4 68   E7
	{GPIO_F,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //LocalBus_A5 69   E8
	{GPIO_F,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12}, //RAS 99   D11
	{GPIO_F,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //LocalBus_A6 70   E9
	{GPIO_F,PIN13,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12}, //LocalBus_A7 73   E10
	{GPIO_F,PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //LocalBus_A8 68   E7
	{GPIO_F,PIN15,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //LocalBus_A9 69   E8

	{GPIO_G,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //LocalBus_A10 70   E9
	{GPIO_G,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12}, //LocalBus_A11 73   E10
	{GPIO_G,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12}, //LocalBus_A12
	{GPIO_G,PIN4,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},  //LocalBus_BA0 70   E9
	{GPIO_G,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12}, //LocalBus_BA1 73   E10
    {GPIO_G,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},//CLK
    {GPIO_G,PIN15,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12}, //CAS 99   D11

	{GPIO_H,PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF12},
};





//NRSEC3000   SPI1
static const Pin spi1_pin[]={
		{GPIO_G,PIN10,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
		{GPIO_B,PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
		{GPIO_B,PIN4,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
		{GPIO_D,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
};



static void NRSEC3000GpioInit(void)
{
	 PIO_Configure(spi1_pin, PIO_LISTSIZE(spi1_pin));     //SPI1  for NRSEC3000
	 PIO_Set(&spi1_pin[0]);
}

bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level)
{
    switch(SPIPort)
    {
        case CN_SPI1:
        	if(level)
        	{
        		PIO_Set(&spi1_pin[0]);
        	}
        	else
        	{
        		PIO_Clear(&spi1_pin[0]);
        	}
        	break;
        default:
            return false;
    }
    return true;
}



void GMAC_InLowPowerPinCfg(void)
{
	u8 i;
	for(i=0;i<PIO_LISTSIZE(EthRmiiPins);i++)
	{
		 GPIO_CfgPinFunc(EthRmiiPins[i].PORT,EthRmiiPins[i].Pinx,GPIO_MODE_AIN,\
		                    GPIO_OTYPE_PP,GPIO_SPEED_L,GPIO_PUPD_NONE);
	}

	GPIO_CfgPinFunc(phyResetPin[i].PORT,phyResetPin[i].Pinx,GPIO_MODE_AIN,\
			                    GPIO_OTYPE_PP,GPIO_SPEED_L,GPIO_PUPD_NONE);

}


void GMAC_OutLowPowerPinCfg(void)
{
	 Eth_GpioInit();
}
// ============================================================================
// 功能：设置串口编号为SerialNo的串口进入低功耗时IO状态
// 参数：SerialNo,串口号
// 返回：无
// ============================================================================
void UART_InLowPowerPinCfg(u8 SerialNo)
{
    u8 pinnum;
    u8 i;
    switch(SerialNo)
    {
    case CN_UART1:
        for(i=0;i<PIO_LISTSIZE(uart1_pin);i++)
        {
            GPIO_CfgPinFunc(uart1_pin[i].PORT,uart1_pin[i].Pinx,GPIO_MODE_AIN,\
                    GPIO_OTYPE_PP,GPIO_SPEED_L,GPIO_PUPD_NONE);
        }
        break;
    case CN_UART3:
        for(i=0;i<PIO_LISTSIZE(uart3_pin);i++)
        {
            GPIO_CfgPinFunc(uart3_pin[i].PORT,uart3_pin[i].Pinx,GPIO_MODE_AIN,\
                    GPIO_OTYPE_PP,GPIO_SPEED_L,GPIO_PUPD_NONE);
        }
        break;
    case CN_UART4:
        for(i=0;i<PIO_LISTSIZE(uart4_pin);i++)
        {
            GPIO_CfgPinFunc(uart4_pin[i].PORT,uart4_pin[i].Pinx,GPIO_MODE_AIN,\
                    GPIO_OTYPE_PP,GPIO_SPEED_L,GPIO_PUPD_NONE);
        }
        break;
    case CN_UART5:
        for(i=0;i<PIO_LISTSIZE(uart5_pin);i++)
        {
            GPIO_CfgPinFunc(uart5_pin[i].PORT,uart5_pin[i].Pinx,GPIO_MODE_AIN,\
                    GPIO_OTYPE_PP,GPIO_SPEED_L,GPIO_PUPD_NONE);
        }
        break;
    case CN_UART6:
        for(i=0;i<PIO_LISTSIZE(uart6_pin);i++)
        {
            GPIO_CfgPinFunc(uart6_pin[i].PORT,uart6_pin[i].Pinx,GPIO_MODE_AIN,\
                    GPIO_OTYPE_PP,GPIO_SPEED_L,GPIO_PUPD_NONE);
        }
        break;
        //SR2711-V1.1.0板删除该功能  20170929
//    case CN_UART7:
//        for(i=0;i<PIO_LISTSIZE(uart7_pin);i++)
//        {
//            GPIO_CfgPinFunc(uart7_pin[i].PORT,uart7_pin[i].Pinx,GPIO_MODE_AIN,\
//                    GPIO_OTYPE_PP,GPIO_SPEED_L,GPIO_PUPD_NONE);
//        }
//        break;
    default:        break;
    }


}
// ============================================================================
// 功能：设置串口编号为SerialNo的串口退出低功耗时IO状态
// 参数：SerialNo,串口号
// 返回：无
// ============================================================================
void UART_OutLowPowerPinCfg(u8 SerialNo)
{
    switch(SerialNo)
    {
    case CN_UART1:
        PIO_Configure(uart1_pin, PIO_LISTSIZE(uart1_pin));
        break;
    case CN_UART3:
        PIO_Configure(uart3_pin, PIO_LISTSIZE(uart3_pin));
        break;
    case CN_UART4:
        PIO_Configure(uart4_pin, PIO_LISTSIZE(uart4_pin));
        break;
    case CN_UART5:
        PIO_Configure(uart5_pin, PIO_LISTSIZE(uart5_pin));
        break;
    case CN_UART6:
        PIO_Configure(uart6_pin, PIO_LISTSIZE(uart6_pin));
        break;
        //SR2711-V1.1.0板删除该功能  20170929
//    case CN_UART7:
//        PIO_Configure(uart7_pin, PIO_LISTSIZE(uart7_pin));
//        break;
    default:        break;
    }

}

// ============================================================================
// 功能：设置IO低功耗时IO状态
// 参数：port,SPI端口号
// 返回：无
// ============================================================================
void SPI_InLowPowerPinCfg(u8 port)
{
	u8 i;
	 switch(port)
	 {
	   case CN_SPI1:
		  for(i=0;i<PIO_LISTSIZE(spi1_pin);i++)
		  {
			 GPIO_CfgPinFunc(spi1_pin[i].PORT,spi1_pin[i].Pinx,GPIO_MODE_AIN,\
					GPIO_OTYPE_PP,GPIO_SPEED_L,GPIO_PUPD_NONE);
		  }
		 break;
	   default:
		   break;
	 }

}


// ============================================================================
// 功能：设置SPI端口进入低功耗时IO的状态
// 参数：SerialNo,串口号
// 返回：无
// ============================================================================
void SPI_OutLowPowerPinCfg(u8 port)
{
	 switch(port)
	 {
	   case CN_SPI1:
		   PIO_Configure(spi1_pin, PIO_LISTSIZE(spi1_pin));     //SPI1  for NRSEC3000
		   PIO_Set(&spi1_pin[0]);
		 break;
	   default:
		   break;
	 }
}

// ============================================================================
// 功能：设置IIC端口进入低功耗时IO状态
// 参数：iic_port,iic的端口号
// 返回：无
// ============================================================================
void IIC_InLowPowerPinCfg(u8 iic_port)
{
    u8 i;
    switch(iic_port)
    {
    case CN_IIC2:
        for(i=0;i<PIO_LISTSIZE(I2C2_pin);i++)
        {
            GPIO_CfgPinFunc(I2C2_pin[i].PORT,I2C2_pin[i].Pinx,GPIO_MODE_AIN,\
                    GPIO_OTYPE_PP,GPIO_SPEED_L,GPIO_PUPD_NONE);
        }
        break;
    default:        break;
    }
    return;
}

// ============================================================================
// 功能：设置IIC退出低功耗时IO的状态
// 参数：SerialNo,串口号
// 返回：无
// ============================================================================
void IIC_OutLowPowerPinCfg(u8 iic_port)
{
    switch(iic_port)
    {
    case CN_IIC2:
        PIO_Configure(I2C2_pin, PIO_LISTSIZE(I2C2_pin));
        break;
    default:        break;
    }
    return ;
}

// =============================================================================
// 功能：Board_GpioInit板上所有用到的GPIO初始化
//	在这里将所有可能用到的复用模式的gpio定义在这里需要用取消注释即可
//	建议不要删除
// 参数：无
// 返回：无
// =============================================================================
void Board_GpioInit(void)
{
	 extern void Board_PowerCtrlInit(void);
	 Board_PowerCtrlInit();
//	 PIO_Configure(wdt_pin, PIO_LISTSIZE(wdt_pin));   //wdt
	 Board_UartGpioInit();                            //uart
     //PHY 芯片
     Eth_GpioInit();                                 //phy

	 //IIC2 for DS3232MZ
     DS3232M_GpioInit();

	 NRSEC3000GpioInit();
	 extern void USB_IOs(void);
	 USB_IOs();

	 ADC_Init();

     extern void KO_Init(void);
     KO_Init();
     extern void KI_Init(void);
     KI_Init();


     PIO_Configure(USB_VCC_EN, PIO_LISTSIZE(USB_VCC_EN));   //

}

//无线模块电源控制引脚
void M4G_PowerSet(void)
{
	 PIO_Set(&M4G_POWER[0]);
}
void M4G_PowerClr(void)
{
	PIO_Clear(&M4G_POWER[0]);
}


#if	CN_CFG_USE_USERTIMER
extern struct IntMasterCtrl  tg_int_global;          //定义并初始化总中断控制结构
extern void __Djy_ScheduleAsynSignal(void);
static void __DjyIsrTimeBase(u32 param);
#define	CN_TIME_ROUNDING	(32768U)//四舍五入的值
#define TIME_GLUE			(CN_CFG_TIME_BASE_HZ>Mhz ? (CN_CFG_TIME_BASE_HZ/Mhz) : \
							((Mhz%CN_CFG_TIME_BASE_HZ==0) ? (Mhz/CN_CFG_TIME_BASE_HZ) :((float)Mhz/CN_CFG_TIME_BASE_HZ)))
#define FAST_TIME_GLUE		((1<<16)/TIME_GLUE)
#define	TIME_BASE_MIN_GAP	(CN_CFG_TIME_BASE_HZ>Mhz?(100*TIME_GLUE):\
							(TIME_GLUE>=100)?(2*TIME_GLUE):((4+(100/TIME_GLUE))*TIME_GLUE))
static u64 g_time_base_tick=0;

void __InitTimeBase(void)
{
	Lptimer1_PreInit();
}

void __DjyStartTimeBase(void)
{
	Lptimer1_Init(CN_LIMIT_UINT16,__DjyIsrTimeBase);
}

//获取可以定时的最大值
u32 __Djy_GetDelayMaxCnt(void)
{
	return CN_LIMIT_UINT16;
}

u32 __Djy_GetTimeBaseGap(void)
{
	return TIME_BASE_MIN_GAP;
}

void __Djy_SetTimeBaseCnt(u32 cnt)
{
	if(cnt>CN_LIMIT_UINT16 || cnt==0)
	{
		//理论上不可能出现此事件
		return;
	}
	Lptimer1_set_period((u16)cnt);
}

static u32 __Djy_GetTimeBaseRealCnt(void)
{
	return Lptimer1_read_cnt();
}

u32 __Djy_GetTimeBaseReload(void)
{
	return Lptimer1_read_reload();
}

u64 __Djy_TimeBaseUsToCnt(u64 us)
{
	return ((CN_CFG_TIME_BASE_HZ>Mhz)?
			(us*TIME_GLUE):
			((u64)(us*FAST_TIME_GLUE + CN_TIME_ROUNDING))>>16);
}

u64 __Djy_GetTimeBaseCnt(u32 cnt)
{
	return (g_time_base_tick + cnt);
}

u64 __DjyGetSysCnt(void)
{
	return (g_time_base_tick + __Djy_GetTimeBaseRealCnt());
}

u64 __DjyGetSysTime(void)
{
    u64 time;
    u64 temp=0;
    atom_low_t atom_low;

    atom_low = Int_LowAtomStart();
    temp = __DjyGetSysCnt();
    time = ((CN_CFG_TIME_BASE_HZ>Mhz)?
    		(temp/(u32)TIME_GLUE)://这里没有办法，只能直接使用除法，否则将引入累计误差，不能容忍	--chj
			(temp*TIME_GLUE));
    Int_LowAtomEnd(atom_low);
    return time;
}

static void __DjyIsrTimeBase(u32 param)
{
	u8 flag = 0;
	u32 tick=0;
	g_bScheduleEnable = false;
	tg_int_global.en_asyn_signal_counter = 1;
	tg_int_global.nest_asyn_signal = 1;
	flag = Lptimer1_ClearISR();
	switch(flag)
	{
		case CN_LPTIMER_NONE:
			break;
		case CN_LPTIMER_RELOAD:
			g_time_base_tick += CN_LIMIT_UINT16;
			break;
		case CN_LPTIMER_CMP:
			tick=__Djy_GetTimeBaseRealCnt();
			Djy_IsrTimeBase(tick);
			break;
		case CN_LPTIMER_RELOAD_AND_CMP:
			g_time_base_tick += CN_LIMIT_UINT16;
			tick=__Djy_GetTimeBaseRealCnt();
			Djy_IsrTimeBase(tick);
			break;
	}

	tg_int_global.nest_asyn_signal = 0;
	tg_int_global.en_asyn_signal_counter = 0;
	if(g_ptEventReady != g_ptEventRunning)
		__Djy_ScheduleAsynSignal();       //执行中断内调度
	g_bScheduleEnable = true;
}

#endif
