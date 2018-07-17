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
#include "board-config.h"
#include "max11410.h"

//这里的gpio只定义了一部分 复用到不同的GPIO还需根据手册添加
// USB 相关
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

const Pin HUB_RST[1] = {
    {GPIO_C, PIN13, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_H, GPIO_PUPD_PD, AF_NUll}, // HUB reset
};

//USB运行状态指示灯
const Pin USB_LED1[1]={
    {GPIO_F, PIN9, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH, GPIO_PUPD_NONE, AF_NUll},
};

const Pin M4G_RST[1]={
    {GPIO_B,PIN5, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PU, AF_NUll},
};


const Pin HUB_POWER[1] = {
    {GPIO_C,PIN8, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_H, GPIO_PUPD_PD, AF_NUll},
};

const Pin M4G_POWER[1] = {
    {GPIO_J, PIN9, GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
};

//wdt
const Pin wdt_pin[] = {
	{GPIO_I,PIN12,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE,AF_NUll},
};

//DEBUG口
static const Pin uart1_pin[] = {
		{GPIO_A,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
		{GPIO_B,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
};

//232RXD3/TXD3
static const Pin uart3_pin[] = {
		{GPIO_B,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
		{GPIO_B,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
};

//232RXD4/TXD4
static const Pin uart4_pin[] = {
		{GPIO_A,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
		{GPIO_I,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
};

//232RXD5/TXD5
static const Pin uart5_pin[] = {
		{GPIO_B,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
		{GPIO_B,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
};


//485_1
static const Pin uart6_pin[] = {
		{GPIO_C,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
		{GPIO_G,PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
		{GPIO_G,PIN12,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
};


//485_2
static const Pin uart7_pin[] = {
		{GPIO_F,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
		{GPIO_F,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
		{GPIO_F,PIN8,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
};

// ============================================================================
// 功能：设置串口编号为SerialNo的串口为半双功发送功能
// 参数：SerialNo,串口号
// 返回：无
// ============================================================================
void Board_UartHalfDuplexSend(u8 SerialNo)
{
	switch(SerialNo)
	{
	case CN_UART6:
		PIO_Set(&uart6_pin[2]);
		break;
	case CN_UART7:
		PIO_Set(&uart7_pin[2]);
		break;
	default:
		break;
	}
    Djy_DelayUs(50);
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
	case CN_UART6:
		PIO_Clear(&uart6_pin[2]);
		break;
	case CN_UART7:
		PIO_Clear(&uart7_pin[2]);
		break;
	default:
		break;
	}
}

//**********************************************************************************
//CAN 通信(2路)

static const Pin CAN1_pin[]={
	{GPIO_H,PIN13,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF9}, //CAN1 TX
	{GPIO_H,PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF9}, //CAN1 RX
};

static const Pin CAN2_pin[]={
	{GPIO_B,PIN4,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF11}, //CAN2 TX
	{GPIO_A,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF11}, //CAN2 RX
};


void CAN_HalInit(void)
{
	RCC->APB1ENR |=(1<<25);//CAN1时钟使能
	RCC->APB1ENR |=(1<<13);//CAN3时钟使能
	PIO_Configure(CAN1_pin, PIO_LISTSIZE(CAN1_pin));  //CAN1
	PIO_Configure(CAN2_pin, PIO_LISTSIZE(CAN2_pin));  //CAN3
}

//**********************************************************************************

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
	{GPIO_K,PIN7,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
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

//**********************************************************************************
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
static const Pin FmcNandPins[] = {
    {GPIO_A,PIN9,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},//nWP
	{GPIO_D,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},  //FMC_D2
	{GPIO_D,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},  //FMC_D3
	{GPIO_D,PIN4,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},  //nNANDOE
	{GPIO_D,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},  //nNANDWE
    {GPIO_D,PIN6,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU},         //nRD_BY
	{GPIO_D,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12}, //NANDCLE
	{GPIO_D,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12}, //NANDALE
	{GPIO_D,PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12}, //FMC_D0
	{GPIO_D,PIN15,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12}, //FMC_D1
	{GPIO_E,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},  //FMC_D4 68
	{GPIO_E,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},  //FMC_D5 69
	{GPIO_E,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},  //FMC_D6 70
	{GPIO_E,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12}, //FMC_D7 73
    {GPIO_G,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12}, //nLocalBus_CS3
};


void HAL_NAND_MspInit(NAND_HandleTypeDef *hnand)
{
	 RCC->AHB3ENR|=1<<0;  //使能FMC时钟
	 PIO_Configure(FmcNandPins, PIO_LISTSIZE(FmcNandPins));
	 PIO_Set(&FmcNandPins[0]);
}

//**********************************************************************************

//LCD
/* 功能 :---------------------------PIN---------PIN-------AF
   LTDC_R0----------D0--------------46---------H2--------14
   LTDC_R1----------D1--------------47---------H3--------14
   LTDC_R2----------D2--------------66---------J1--------14
   LTDC_R3----------D3--------------61---------B0--------9
   LTDC_R4----------D4--------------68---------J3--------14
   LTDC_R5----------D6--------------69---------J4--------14
   LTDC_R6----------D27-------------62---------B1--------9
   LTDC_R7----------D5--------------65---------J0--------9
   LTDC_G0----------D7--------------119--------J7--------14
   LTDC_G1----------D8--------------120--------J8--------14
   LTDC_G2----------D9--------------64---------I15-------9
   LTDC_G3----------D12-------------122--------J10-------14
   LTDC_G4----------D13-------------123--------J11-------14
   LTDC_G5----------D14-------------126--------K0--------14
   LTDC_G6----------D10-------------13---------I11-------9
   LTDC_G7----------D11-------------127--------K2--------14
   LTDC_B0----------D15-------------174--------J12-------14
   LTDC_B1----------D18-------------175--------J13-------14
   LTDC_B2----------D19-------------176--------J14-------14
   LTDC_B3----------D20-------------177--------J15-------14
   LTDC_B4----------D21-------------186--------K3--------14
   LTDC_B5----------D22-------------50---------A3--------14
   LTDC_B6----------D16-------------188--------K5--------14
   LTDC_B7----------D17-------------189--------K6--------14
   LTDC_HSYNC-------D24-------------12---------I10-------14
   LTDC_VSYNC-------D25-------------20---------I13-------14
   LTDC_DE----------D26-------------31---------F10-------14
   LTDC_CLK---------CLKIN-----------21---------I14-------14
 */
static const Pin LCD_Pin[]={
	{GPIO_A, PIN3, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},  //LTDC_B5
	{GPIO_B, PIN0, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF9},  //LTDC_R3
	{GPIO_B, PIN1, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF9},  //LTDC_R6
	{GPIO_F, PIN10, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},  //LTDC_DE
	{GPIO_H, PIN2, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},  //LTDC_R0
	{GPIO_H, PIN3, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},  //LTDC_R1
	{GPIO_I, PIN10, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},  //LTDC_HSYNC
	{GPIO_I, PIN11, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF9},  //LTDC_G6
	{GPIO_I, PIN13, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},  //LTDC_VSYNC

	{GPIO_I, PIN14, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},  //LTDC_CLK
	{GPIO_I, PIN15, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF9},  //LTDC_G2
	{GPIO_J, PIN0, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF9},  //LTDC_R7
	{GPIO_J, PIN1, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},  //LTDC_R2
	{GPIO_J, PIN3, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},  //LTDC_R4
	{GPIO_J, PIN4, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},   //LTDC_R5
	{GPIO_J, PIN7, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},  //LTDC_G0
	{GPIO_J, PIN8, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},  //LTDC_G1

	{GPIO_J, PIN10, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14}, //LTDC_G3
	{GPIO_J, PIN11, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14}, //LTDC_G4
	{GPIO_J, PIN12, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14}, //LTDC_B0
	{GPIO_J, PIN13, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},  //LTDC_B1
	{GPIO_J, PIN14, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},  //LTDC_B2
	{GPIO_J, PIN15, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},  //LTDC_B3
	{GPIO_K, PIN0, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},   //LTDC_G5
	{GPIO_K, PIN2, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},   //LTDC_G7

	{GPIO_K, PIN3, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},   //LTDC_B4
	{GPIO_K, PIN5, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},   //LTDC_B6
	{GPIO_K, PIN6, GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PULLUP,AF14},    //LTDC_B7

    {GPIO_H,PIN10,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},//背光
    {GPIO_C,PIN9 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},//power
};

const Pin TOUCH_POWER[1] = {
    {GPIO_C,PIN9 ,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll} // 触摸屏电源与LCD公用
};
//===============================================================
//测试先拉高拉低电平后续会做成pwm输出
// 0关背光断电    非零：开背光供电
//==============================================================
void Lcd_BackLight_OnOff(u8 onoff)
{
  if(onoff)
  {
      GPIO_SettoHigh(GPIO_H,PIN10);
      GPIO_SettoLow(GPIO_C,PIN9 );
  }
  else
  {
      GPIO_SettoHigh(GPIO_C,PIN9 );
      GPIO_SettoLow(GPIO_H,PIN10);
  }

}


//**********************************************************************************
//max11410   SPI3
//PA15->SPI3_NSS PB2->SPI3_MOSI  PB3->SPI3_SCK  PC11->SPI3_MISO
static const Pin spi3_pin[]={
        {GPIO_A,PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
        {GPIO_B,PIN2 ,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF7},
        {GPIO_B,PIN3 ,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF6},
        {GPIO_C,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF6},
};
const u32 Max11410_Ref = (4000);
const bool_t Max11410_RxAadjust = true;
static  struct Init_Reg8Bit  const  Init8BitTab[] =
{
        {Max11410_GP0_CTRL   , 0x00},
        {Max11410_GP1_CTRL   , 0x00},
        {Max11410_CTRL       , 0x41},//选择REF1
        {Max11410_FILTER     , 0x04},// 50/60 Hz 16sps
        {Max11410_SOURCE     , 0x0D},//选择励磁电流源阻抗选择 200ua
        {Max11410_PGA        , 0x20},//PGA 寄存器控制信号路径
        {Max11410_PD         , 0x00},//正常模式
        {Max11410_SEQ_START  , Max11410_uC0},//选择序列起始地址Max11410_uC0
};

static  struct Init_Reg16Bit const CfguCTab[] = {
        {Max11410_uC0 , Max11410_PGA       , 0x20}, //选择 PGA，增益 = 1
        {Max11410_uC1 , Max11410_MUX_CTRL1 , 0x0f}, //寄存器匹配电流源AIN0
        {Max11410_uC2 , Max11410_MUX_CTRL0 , 0x01}, //选择 AINP = AIN0，AINN = AIN1
        {Max11410_uC3 , Max11410_FILTER    , 0x04}, //选择 50/60 Hz ，16sps
        {Max11410_uC4 , Max11410_WAIT_START, 0xD0}, //插入等待时间的 1.3ms (假设 WAIT_EXT = 0)
        {Max11410_uC5 , Max11410_CONV_START, 0x00}, //启动单个转换和将数据发送到 DATA0

        {Max11410_uC6 , Max11410_MUX_CTRL1 , 0x1f}, //寄存器匹配电流源AIN1
        {Max11410_uC7 , Max11410_MUX_CTRL0 , 0x18}, //选择 AINP = AIN1，AINN = AIN8
        {Max11410_uC8 , Max11410_FILTER    , 0x04}, //选择 50/60 Hz ，16sps
        {Max11410_uC9 , Max11410_WAIT_START, 0xD0}, //插入等待时间的 1.3ms (假设 WAIT_EXT = 0)
        {Max11410_uC10, Max11410_CONV_START, 0x40}, //启动单个转换和将数据发送到 DATA0+4
        //**********************************************************************************
        {Max11410_uC11, Max11410_PGA       , 0x20}, //选择 PGA，增益 = 1
        {Max11410_uC12, Max11410_MUX_CTRL1 , 0x2f}, //寄存器匹配电流源AIN2
        {Max11410_uC13, Max11410_MUX_CTRL0 , 0x23}, //选择 AINP = AIN2，AINN = AIN3
        {Max11410_uC14, Max11410_FILTER    , 0x04}, //选择 50/60 Hz ，16sps
        {Max11410_uC15, Max11410_WAIT_START, 0xD0}, //插入等待时间的 1.3ms (假设 WAIT_EXT = 0)
        {Max11410_uC16, Max11410_CONV_START, 0x10}, //启动单个转换和将数据发送到 DATA1

        {Max11410_uC17, Max11410_MUX_CTRL1 , 0x3f}, //寄存器匹配电流源AIN1
        {Max11410_uC18, Max11410_MUX_CTRL0 , 0x38}, //选择 AINP = AIN1，AINN = AIN8
        {Max11410_uC19, Max11410_FILTER    , 0x04}, //选择 50/60 Hz ，16sps
        {Max11410_uC20, Max11410_WAIT_START, 0xD0}, //插入等待时间的 1.3ms (假设 WAIT_EXT = 0)
        {Max11410_uC21, Max11410_CONV_START, 0x50}, //启动单个转换和将数据发送到 DATA1+4
        //**********************************************************************************
        {Max11410_uC22, Max11410_PGA       , 0x20}, //选择 PGA，增益 = 1
        {Max11410_uC23, Max11410_MUX_CTRL1 , 0x4f}, //寄存器匹配电流源AIN4
        {Max11410_uC24, Max11410_MUX_CTRL0 , 0x45}, //选择 AINP = AIN4，AINN = AIN5
        {Max11410_uC25, Max11410_FILTER    , 0x04}, //选择 50/60 Hz ，16sps
        {Max11410_uC26, Max11410_WAIT_START, 0xD0}, //插入等待时间的 1.3ms (假设 WAIT_EXT = 0)
        {Max11410_uC27, Max11410_CONV_START, 0x20}, //启动单个转换和将数据发送到 DATA2

        {Max11410_uC28, Max11410_MUX_CTRL1 , 0x5f}, //寄存器匹配电流源AIN1
        {Max11410_uC29, Max11410_MUX_CTRL0 , 0x58}, //选择 AINP = AIN1，AINN = AIN8
        {Max11410_uC30, Max11410_FILTER    , 0x04}, //选择 50/60 Hz ，16sps
        {Max11410_uC31, Max11410_WAIT_START, 0xD0}, //插入等待时间的 1.3ms (假设 WAIT_EXT = 0)
        {Max11410_uC32, Max11410_CONV_START, 0x60}, //启动单个转换和将数据发送到 DATA2+4
        //**********************************************************************************
        {Max11410_uC33, Max11410_PGA       , 0x20}, //选择 PGA，增益 = 1
        {Max11410_uC34, Max11410_MUX_CTRL1 , 0x6f}, //寄存器匹配电流源AIN4
        {Max11410_uC35, Max11410_MUX_CTRL0 , 0x67}, //选择 AINP = AIN0，AINN = AIN1
        {Max11410_uC36, Max11410_FILTER    , 0x04}, //选择 50/60 Hz ，16sps
        {Max11410_uC37, Max11410_WAIT_START, 0xD0}, //插入等待时间的 1.3ms (假设 WAIT_EXT = 0)
        {Max11410_uC38, Max11410_CONV_START, 0x30}, //启动单个转换和将数据发送到 DATA3

        {Max11410_uC39, Max11410_MUX_CTRL1 , 0x7f}, //寄存器匹配电流源AIN1
        {Max11410_uC40, Max11410_MUX_CTRL0 , 0x78}, //选择 AINP = AIN1，AINN = AIN8
        {Max11410_uC41, Max11410_FILTER    , 0x04}, //选择 50/60 Hz ，16sps
        {Max11410_uC42, Max11410_WAIT_START, 0xD0}, //插入等待时间的 1.3ms (假设 WAIT_EXT = 0)
        {Max11410_uC43, Max11410_CONV_START, 0x70}, //启动单个转换和将数据发送到 DATA3+4

        {Max11410_uC44, Max11410_SEQ_START,  Max11410_uC0},
};

bool_t Max11410_Init(struct Max11410_CfgTab *config)
{
    config->pt_Reg8Bit  = Init8BitTab;
    config->Reg8BitNum  = sizeof(Init8BitTab)/sizeof(struct Init_Reg8Bit);
    config->pt_Reg16Bit = CfguCTab;
    config->Reg16BitNum = sizeof(CfguCTab)/sizeof(struct Init_Reg16Bit);
    return true;
}

//AD8688 AD芯片   SPI4
static const Pin spi4_pin[]={
		{GPIO_E,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
		{GPIO_E,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
		{GPIO_E,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
};
const Pin ADCS[]={
	{GPIO_E,PIN4,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
};  //普通输出做CS引脚
const Pin ADRST[]={
  {GPIO_D,PIN13,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
};

const Pin ADBUSY[]={
		{GPIO_E,PIN3,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
};

void ADS8688_GpioInit(void)
{
	 PIO_Configure(spi4_pin, PIO_LISTSIZE(spi4_pin));      //SPI4 for ads8688
	 PIO_Configure(ADCS,PIO_LISTSIZE(ADCS));
	 PIO_Configure(ADRST,PIO_LISTSIZE(ADRST));
	 PIO_Configure(ADBUSY,PIO_LISTSIZE(ADBUSY));
	 PIO_Clear(ADCS);
}




static const Pin iic1_pin[] = {
		{GPIO_B,PIN7,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},//scl
		{GPIO_B,PIN8,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},//sda
};


static const Pin iic2_pin[]={
		{GPIO_H,PIN4,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
		{GPIO_H,PIN5,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
};

//MCP2515(SPI转CAN)芯片   SPI1
static const Pin spi1_pin[]={
		{GPIO_A,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
		{GPIO_A,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
		{GPIO_D,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
		{GPIO_A,PIN4,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},//普通输出做CS引脚
};

const Pin CAN_RstPin[]={
	{GPIO_D,PIN2,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
};

//const Pin CAN_IntPin[]={
//		{GPIO_D,PIN3,GPIO_MODE_IT_FALLING,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
//};





void MCP2515_GpioInit(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	PIO_Configure(spi1_pin,PIO_LISTSIZE(spi1_pin));
	PIO_Configure(CAN_RstPin,PIO_LISTSIZE(CAN_RstPin));
//	PIO_Configure(CAN_IntPin,PIO_LISTSIZE(CAN_IntPin));
	PIO_Clear(&spi1_pin[4]);
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_Initure.Pin=GPIO_PIN_3;
	GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;
	GPIO_Initure.Pull=GPIO_PULLUP;
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);
	Int_Register(CN_INT_LINE_EXTI3);
	Int_IsrConnect(CN_INT_LINE_EXTI3,(void(*))HAL_GPIO_EXTI_IRQHandler);//中断触发会调用该函数（位于hal_gpio.c）
	Int_SetIsrPara(CN_INT_LINE_EXTI3,GPIO_PIN_3);//参数就是引脚3（外部中断相同引脚号的gpio口共用一个中断）
	Int_SettoAsynSignal(CN_INT_LINE_EXTI3);
	Int_ClearLine(CN_INT_LINE_EXTI3);
	Int_RestoreAsynLine(CN_INT_LINE_EXTI3);
}

//**********************************************************************************
// =============================================================================
// 功能：Board_GpioInit板上所有用到的GPIO初始化
//	在这里将所有可能用到的复用模式的gpio定义在这里需要用取消注释即可
//	建议不要删除
// 参数：无
// 返回：无
// =============================================================================
void Board_GpioInit(void)
{
	 PIO_Configure(wdt_pin, PIO_LISTSIZE(wdt_pin));

	 PIO_Configure(uart1_pin, PIO_LISTSIZE(uart1_pin));
	 PIO_Configure(uart3_pin, PIO_LISTSIZE(uart3_pin));
	 PIO_Configure(uart4_pin, PIO_LISTSIZE(uart4_pin));
	 PIO_Configure(uart5_pin, PIO_LISTSIZE(uart5_pin));
	 PIO_Configure(uart6_pin, PIO_LISTSIZE(uart6_pin));
	 PIO_Configure(uart7_pin, PIO_LISTSIZE(uart7_pin));

	 CAN_HalInit();

     //PHY 芯片
     Eth_GpioInit();

	 extern void USB_IOs(void);
	 USB_IOs();
	 ADS8688_GpioInit();

	 PIO_Configure(spi3_pin, PIO_LISTSIZE(spi3_pin));      //SPI3 max11410
	 MCP2515_GpioInit();

	 PIO_Configure(iic1_pin, PIO_LISTSIZE(iic1_pin));      //IIC1 for TCAP535
	 PIO_Configure(iic2_pin, PIO_LISTSIZE(iic2_pin));      //IIC2 for FRAM and EEPROM

     extern void KO_Init(void);
     KO_Init();
     extern void KI_Init(void);
     KI_Init();
	 PIO_Configure(LCD_Pin, PIO_LISTSIZE(LCD_Pin));

}

bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level)
{
    switch(SPIPort)
    {
        case CN_SPI4:
        	if(level)
        	{
        		PIO_Set(&spi4_pin[0]);
        	}
        	else
        	{
        		PIO_Clear(&spi4_pin[0]);
        	}
            break;
        case CN_SPI3:
        	if(level)
        	{
        		PIO_Set(&spi3_pin[0]);
        	}
        	else
        	{
        		PIO_Clear(&spi3_pin[0]);
        	}
        	break;
        default:
            return false;
    }
    return true;
}

