//----------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 著作权由都江堰操作系统开源团队所有。著作权人保留一切权利。
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
// 文件名     ：SR5335-B0 board.c
// 模块描述: 板件相关部分初始化或配置等
// 模块版本: V1.00
// 创建人员: zhb
// 创建时间: 9/09.2017
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "cpu_peri_gpio.h"
#include "cpu_peri_iic.h"
#include "cpu_peri.h"
#include "max11410.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern void Board_GpioInit(void);
//    Board_GpioInit();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"boardinit"    //板件特性配置
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp组件             //选填“第三方组件、核心组件、bsp组件、用户组件”，本属性用于在IDE中分组
//select:必选                //选填“必选、可选、不可选”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early              //初始化时机，可选值：early，medium，later。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"cpu_peri_gpio","cpu_peri_iic"    //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
//%$#@end configue  ****参数配置结束
//@#$%component end configure

//这里的gpio只定义了一部分 复用到不同的GPIO还需根据手册添加
static const Pin pwm_pin[] = {
    {GPIO_B,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,ST_AF2},  //pwm_out1  TIM4
    {GPIO_C,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,ST_AF2},  //pwm_out2  TIM3
};
//485_1
static const Pin uart1_pin[] = {
    {GPIO_A,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
    {GPIO_A,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
    {GPIO_A,PIN8,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
};

//232TXD2/232RXD2 (2322T/2322R)
static const Pin uart3_pin[] = {
        {GPIO_D,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_D,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
};

//预留232 232RXD4/232TXD4
static const Pin uart4_pin[] = {
        {GPIO_C,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_C,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
 };

//无线模块使用  华为通信模块
//static const Pin uart5_pin[] = {
//      {GPIO_C,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
//      {GPIO_D,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
// };
//485_2
static const Pin uart6_pin[] = {
        {GPIO_C,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_C,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_G,PIN8,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
 };


//DEBUG口
static const Pin uart7_pin[] = {
    {GPIO_F,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
    {GPIO_F,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
 };



//232TXD1/232RXD1 (2321T/2321R)
static const Pin uart8_pin[] = {
        {GPIO_E,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_E,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
 };





//AD8688 AD芯片   SPI2
static const Pin spi2_pin[]={
        {GPIO_I,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_I,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_I,PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
};
const Pin ADCS[]={
    {GPIO_I,PIN0,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
};  //普通输出做CS引脚
const Pin ADRST[]={
  {GPIO_C,PIN13,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
};




//触摸屏  SPI5
static const Pin spi5_pin[]={
        {GPIO_H,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_F,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_F,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
};
const Pin SpiCs[]={
        {GPIO_F,PIN10,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF_NUll},
};
const Pin PenStatus[] = {
    {GPIO_E,PIN3,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,AF_NUll},
};
const Pin LCD_BLK[1] = {
    {GPIO_G, PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP, GPIO_SPEED_100M,GPIO_PUPD_NONE,AF_NUll},
};
const Pin LCD_RST[1]={
    {GPIO_I,PIN4,GPIO_MODE_OUT,GPIO_OTYPE_PP, GPIO_SPEED_100M,GPIO_PUPD_PU,AF_NUll},
};

//max11410   SPI1
//PA4->SPI1_NSS  PA5->SPI1_SCK PA6->SPI1_MISO  PB5->SPI1_MOSI
static const Pin spi1_pin[]={
        {GPIO_A,PIN4,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
        {GPIO_A,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_A,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_B,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
};

const u32  Max11410_Ref = (4000);
const bool_t Max11410_RxAadjust = false;;
static  struct Init_Reg8Bit  const  Init8BitTab[] = {
    {Max11410_GP0_CTRL   , 0x00},//控制GPIO0的行为  不使用保持默认值
    {Max11410_GP1_CTRL   , 0x00},//控制GPIO1的行为  不使用保持默认值
    {Max11410_CTRL       , 0x41},//选择时钟源 没有正负的数据格式
    {Max11410_FILTER     , 0x04},//滤波器范围 选择 50/60 Hz 16sps
    {Max11410_SOURCE     , 0x0D},//选择励磁电流源阻抗选择 200ua
    {Max11410_PGA        , 0x20},//PGA 寄存器控制信号路径
    {Max11410_PD         , 0x00},//正常模式
    {Max11410_SEQ_START  , Max11410_uC0},//选择序列起始地址0x6E~~0x3A
};


static  struct Init_Reg16Bit  const CfguCTab[] = {

     {Max11410_uC0 ,Max11410_PGA       , 0x20},  //选择 pga =1
     {Max11410_uC1 ,Max11410_MUX_CTRL1 , 0x0f},  //寄存器匹配电流源AIN0
     {Max11410_uC2 ,Max11410_MUX_CTRL0 , 0x12},  //选择系统输入
     {Max11410_uC3 ,Max11410_WAIT_START, 0xD0},  //插入等待时间的 1.3ms    (假设 WAIT_EXT = 0)
     {Max11410_uC4 ,Max11410_FILTER    , 0x04},  //选择 50/60 Hz ，16sps
     {Max11410_uC5 ,Max11410_CONV_START, 0x00},  //启动单个转换和将数据发送到 DATA0

     {Max11410_uC6 ,Max11410_PGA       , 0x20},  //选择 pga =1
     {Max11410_uC7 ,Max11410_MUX_CTRL1 , 0x4f},  //寄存器匹配电流源AIN4
     {Max11410_uC8 ,Max11410_MUX_CTRL0 , 0x56},  //选择系统输入
     {Max11410_uC9 ,Max11410_WAIT_START, 0xD0},  //插入等待时间的 1.3ms    (假设 WAIT_EXT = 0)
     {Max11410_uC10,Max11410_FILTER    , 0x04},  //选择 50/60 Hz ，16sps
     {Max11410_uC11,Max11410_CONV_START, 0x10},  //启动单个转换和将数据发送到 DATA1
     {Max11410_uC12,Max11410_SEQ_START ,   Max11410_uC0},  //循环回到地址 0x3a 并重新启动序列

};


bool_t Max11410_Init(struct Max11410_CfgTab *config)
{
    config->pt_Reg8Bit  = Init8BitTab;
    config->Reg8BitNum  = sizeof(Init8BitTab)/sizeof(struct Init_Reg8Bit);
    config->pt_Reg16Bit = CfguCTab;
    config->Reg16BitNum = sizeof(CfguCTab)/sizeof(struct Init_Reg16Bit);
    return true;
}
//==================================End Max11410 config ==========================
//at45db321 flash   SPI4
static const Pin spi4_pin[]={
        {GPIO_E,PIN4,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
        {GPIO_E,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_E,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_E,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
};
//at45db321 W/R Protect Pin
static const Pin At45Wp_pin[] ={
        {GPIO_H, PIN7, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_100M,GPIO_PUPD_NONE},
};




//iic4 for at24c128b
static const Pin iic4_pin[] = {
    {GPIO_F,PIN14,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},//scl
    {GPIO_F,PIN15,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},//sda
};




/**I2S3 GPIO Configuration
        PA15     ------> I2S3_WS
        PD6     ------> I2S3_SD
        PB3     ------> I2S3_CK
        PB4     ------> SD_MODE
        */
static const Pin AUDIO_I2S3_pin[]={
    {GPIO_A,PIN15,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_H,GPIO_PUPD_PU,AF6}, //I2S3 FSYNC
    {GPIO_B,PIN3,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_H,GPIO_PUPD_PU,AF6},  //I2S3 CLK
    {GPIO_D,PIN6,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},  //I2S3 SD
    {GPIO_B,PIN4,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll}, //SD_MODE
};



void HAL_I2S_MspInit(I2S_HandleTypeDef *hi2s)
{
     RCC_PeriphCLKInitTypeDef RCC_ExCLKInitStruct;
     GPIO_InitTypeDef  GPIO_InitStruct;
     RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
     if(hi2s->Instance==SPI3)
     {
       __HAL_RCC_SPI3_CLK_ENABLE();
       PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
       PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
       PeriphClkInitStruct.PLLI2S.PLLI2SP = RCC_PLLP_DIV2;
       PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
       PeriphClkInitStruct.PLLI2S.PLLI2SQ = 2;
       PeriphClkInitStruct.PLLI2SDivQ = 1;
       PeriphClkInitStruct.I2sClockSelection = RCC_I2SCLKSOURCE_PLLI2S;
       if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
       {
           printf("I2S CLK init failed.\r\n");
       }

     }
}

void HAL_I2S_MspDeInit(I2S_HandleTypeDef* hi2s)
{

  if(hi2s->Instance==SPI3)
  {
    /* Peripheral clock disable */
    __HAL_RCC_SPI3_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15);
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_6);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_4);
  }
}











 //CAN 通信
static const Pin CAN1_pin[]={
    {GPIO_B,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF9}, //CAN1 RX
    {GPIO_H,PIN13,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF9}, //CAN1 TX
};


void CAN_HalInit(void)
{
    RCC->APB1ENR |=(1<<25);//CAN1时钟使能
    PIO_Configure(CAN1_pin, PIO_LISTSIZE(CAN1_pin));  //CAN1
}


//无线模块电源控制引脚 add by zhb 20170726(依据SR5333硬件改版)
const Pin M4G_PowerCtrlPin[1]={
    {GPIO_I,PIN7, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PU, AF_NUll},
};


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
    {GPIO_D,PIN6,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU},         //R/B 150  D6
    {GPIO_D,PIN3,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},//WP  145  D3

    {GPIO_G,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12}, //CE  152  PG9
    {GPIO_D,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12}, //ALE 100  D12
    {GPIO_D,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},  //WE  147  D5
    {GPIO_D,PIN4,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},  //OE  146  D4
    {GPIO_D,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12}, //CLE 99   D11

    {GPIO_D,PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12}, //LocalBus_D0 104  D14
    {GPIO_D,PIN15,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12}, //LocalBus_D1 105  D15
    {GPIO_D,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},  //LocalBus_D2 142  D0
    {GPIO_D,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},  //LocalBus_D3 143  D1
    {GPIO_E,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},  //LocalBus_D4 68   E7
    {GPIO_E,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},  //LocalBus_D5 69   E8
    {GPIO_E,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12},  //LocalBus_D6 70   E9
    {GPIO_E,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF12}, //LocalBus_D7 73   E10
};

/*
      RMII_REF_CLK ----------------------> PA1
      RMII_MDIO -------------------------> PA2
      RMII_MDC --------------------------> PC1
      RMII_MII_CRS_DV -------------------> PA7
      RMII_MII_RXD0 ---------------------> PC4
      RMII_MII_RXD1 ---------------------> PC5
      RMII_MII_RXER ---------------------> PG2  //(not used)
      RMII_MII_TX_EN --------------------> PB11
      RMII_MII_TXD0 ---------------------> PG13
      RMII_MII_TXD1 ---------------------> PG14
*/
static const Pin EthRmiiPins[] = {
    {GPIO_A,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_A,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_A,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_C,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_C,PIN4,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_C,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_B,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_G,PIN13,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_G,PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
};
static const Pin phyResetPin[] = {
    {GPIO_H,PIN11,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
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
    case CN_UART1:
        PIO_Set(&uart1_pin[2]);
        break;
    case CN_UART6:
        PIO_Set(&uart6_pin[2]);
        break;
    default:
        break;
    }
    Djy_DelayUs(10);
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
    case CN_UART1:
        PIO_Clear(&uart1_pin[2]);
        break;
    case CN_UART6:
        PIO_Clear(&uart6_pin[2]);
        break;
    default:
        break;
    }
    Djy_DelayUs(10);
}



// =============================================================================
// 功能：Board_GpioInit板上所有用到的GPIO初始化
//  在这里将所有可能用到的复用模式的gpio定义在这里需要用取消注释即可
//  建议不要删除
// 参数：无
// 返回：无
// =============================================================================
void Board_GpioInit(void)
{
     PIO_Configure(pwm_pin,PIO_LISTSIZE(pwm_pin));          //PWM_OUT1,2

     PIO_Configure(uart1_pin, PIO_LISTSIZE(uart1_pin));  //485_1
     PIO_Configure(uart6_pin, PIO_LISTSIZE(uart6_pin));  //485_2

     PIO_Configure(uart3_pin, PIO_LISTSIZE(uart3_pin));   //232
     PIO_Configure(uart4_pin, PIO_LISTSIZE(uart4_pin));   //232 wireless modem
//   PIO_Configure(uart5_pin, PIO_LISTSIZE(uart5_pin));   //232 wireless modem
     PIO_Configure(uart7_pin, PIO_LISTSIZE(uart7_pin));   //debug
     PIO_Configure(uart8_pin, PIO_LISTSIZE(uart8_pin));   //232

     PIO_Configure(spi1_pin, PIO_LISTSIZE(spi1_pin));      //SPI1 for max11410
     PIO_Set(&spi1_pin[0]);
     PIO_Configure(spi2_pin, PIO_LISTSIZE(spi2_pin));      //SPI2 for ads8688
     PIO_Configure(ADCS,PIO_LISTSIZE(ADCS));
     PIO_Configure(ADRST,PIO_LISTSIZE(ADRST));


     PIO_Configure(spi4_pin, PIO_LISTSIZE(spi4_pin));     //SPI4  for at45db321b
     PIO_Configure(At45Wp_pin, PIO_LISTSIZE(At45Wp_pin));
     PIO_Set(&At45Wp_pin[0]);
     PIO_Set(&spi4_pin[0]);


     PIO_Configure(spi5_pin, PIO_LISTSIZE(spi5_pin));   //SPI5 for LCM240128C touch
     PIO_Configure(PenStatus,PIO_LISTSIZE(PenStatus));
     PIO_Configure(SpiCs,PIO_LISTSIZE(SpiCs));
     PIO_Configure(LCD_BLK,PIO_LISTSIZE(LCD_BLK));
     PIO_Configure(LCD_RST,PIO_LISTSIZE(LCD_RST));



     extern bool_t IIC_Busfree(u32 port,u32 sda_pin,u32 sck_pin);
     IIC_Busfree(GPIO_F,PIN15,PIN14);
     PIO_Configure(iic4_pin, PIO_LISTSIZE(iic4_pin));      //iic4 for at24c128b



     PIO_Configure(EthRmiiPins,PIO_LISTSIZE(EthRmiiPins));  //ETH RMII
     PIO_Configure(phyResetPin,PIO_LISTSIZE(phyResetPin));
     PIO_Clear(phyResetPin);    //66e6060 reset delay time > 10 ms
     Djy_EventDelay(300*mS);
     PIO_Set(phyResetPin);



     CAN_HalInit();



     //无线模块电源控制引脚 add by zhb 20170726(依据SR5333硬件改版)
     PIO_Configure(M4G_PowerCtrlPin, PIO_LISTSIZE(M4G_PowerCtrlPin));  //M4G_PowerCtrlPin
     PIO_Set(&M4G_PowerCtrlPin[0]);



     PIO_Configure(FmcNandPins, PIO_LISTSIZE(FmcNandPins));
     PIO_Set(&FmcNandPins[1]);

     //
     PIO_Configure(AUDIO_I2S3_pin, PIO_LISTSIZE(AUDIO_I2S3_pin));
     PIO_Set(&AUDIO_I2S3_pin[3]);


     extern void KO_Init(void);
     KO_Init();
     extern void KI_Init(void);
     KI_Init();

     extern void ADC_Init(void);
     ADC_Init();
     extern void USB_IOs(void);
     USB_IOs();

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

//无线模块电源控制引脚 add by zhb 20170726(依据SR5333硬件改版)
void M4G_PowerSet(void)
{
     PIO_Set(&M4G_PowerCtrlPin[0]);
}
void M4G_PowerClr(void)
{
    PIO_Clear(&M4G_PowerCtrlPin[0]);
}
