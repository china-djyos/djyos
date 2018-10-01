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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��SR5335-B0 board.c
// ģ������: �����ز��ֳ�ʼ�������õ�
// ģ��汾: V1.00
// ������Ա: zhb
// ����ʱ��: 9/09.2017
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "cpu_peri_gpio.h"
#include "cpu_peri_iic.h"
#include "cpu_peri.h"
#include "max11410.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern void Board_GpioInit(void);
//    Board_GpioInit();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"boardinit"    //�����������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���             //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"cpu_peri_gpio","cpu_peri_iic"    //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
//%$#@end configue  ****�������ý���
//@#$%component end configure

//�����gpioֻ������һ���� ���õ���ͬ��GPIO��������ֲ����
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

//Ԥ��232 232RXD4/232TXD4
static const Pin uart4_pin[] = {
        {GPIO_C,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_C,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
 };

//����ģ��ʹ��  ��Ϊͨ��ģ��
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


//DEBUG��
static const Pin uart7_pin[] = {
    {GPIO_F,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
    {GPIO_F,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
 };



//232TXD1/232RXD1 (2321T/2321R)
static const Pin uart8_pin[] = {
        {GPIO_E,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_E,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
 };





//AD8688 ADоƬ   SPI2
static const Pin spi2_pin[]={
        {GPIO_I,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_I,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_I,PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
};
const Pin ADCS[]={
    {GPIO_I,PIN0,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
};  //��ͨ�����CS����
const Pin ADRST[]={
  {GPIO_C,PIN13,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
};




//������  SPI5
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
    {Max11410_GP0_CTRL   , 0x00},//����GPIO0����Ϊ  ��ʹ�ñ���Ĭ��ֵ
    {Max11410_GP1_CTRL   , 0x00},//����GPIO1����Ϊ  ��ʹ�ñ���Ĭ��ֵ
    {Max11410_CTRL       , 0x41},//ѡ��ʱ��Դ û�����������ݸ�ʽ
    {Max11410_FILTER     , 0x04},//�˲�����Χ ѡ�� 50/60 Hz 16sps
    {Max11410_SOURCE     , 0x0D},//ѡ�����ŵ���Դ�迹ѡ�� 200ua
    {Max11410_PGA        , 0x20},//PGA �Ĵ��������ź�·��
    {Max11410_PD         , 0x00},//����ģʽ
    {Max11410_SEQ_START  , Max11410_uC0},//ѡ��������ʼ��ַ0x6E~~0x3A
};


static  struct Init_Reg16Bit  const CfguCTab[] = {

     {Max11410_uC0 ,Max11410_PGA       , 0x20},  //ѡ�� pga =1
     {Max11410_uC1 ,Max11410_MUX_CTRL1 , 0x0f},  //�Ĵ���ƥ�����ԴAIN0
     {Max11410_uC2 ,Max11410_MUX_CTRL0 , 0x12},  //ѡ��ϵͳ����
     {Max11410_uC3 ,Max11410_WAIT_START, 0xD0},  //����ȴ�ʱ��� 1.3ms    (���� WAIT_EXT = 0)
     {Max11410_uC4 ,Max11410_FILTER    , 0x04},  //ѡ�� 50/60 Hz ��16sps
     {Max11410_uC5 ,Max11410_CONV_START, 0x00},  //��������ת���ͽ����ݷ��͵� DATA0

     {Max11410_uC6 ,Max11410_PGA       , 0x20},  //ѡ�� pga =1
     {Max11410_uC7 ,Max11410_MUX_CTRL1 , 0x4f},  //�Ĵ���ƥ�����ԴAIN4
     {Max11410_uC8 ,Max11410_MUX_CTRL0 , 0x56},  //ѡ��ϵͳ����
     {Max11410_uC9 ,Max11410_WAIT_START, 0xD0},  //����ȴ�ʱ��� 1.3ms    (���� WAIT_EXT = 0)
     {Max11410_uC10,Max11410_FILTER    , 0x04},  //ѡ�� 50/60 Hz ��16sps
     {Max11410_uC11,Max11410_CONV_START, 0x10},  //��������ת���ͽ����ݷ��͵� DATA1
     {Max11410_uC12,Max11410_SEQ_START ,   Max11410_uC0},  //ѭ���ص���ַ 0x3a ��������������

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











 //CAN ͨ��
static const Pin CAN1_pin[]={
    {GPIO_B,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF9}, //CAN1 RX
    {GPIO_H,PIN13,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF9}, //CAN1 TX
};


void CAN_HalInit(void)
{
    RCC->APB1ENR |=(1<<25);//CAN1ʱ��ʹ��
    PIO_Configure(CAN1_pin, PIO_LISTSIZE(CAN1_pin));  //CAN1
}


//����ģ���Դ�������� add by zhb 20170726(����SR5333Ӳ���İ�)
const Pin M4G_PowerCtrlPin[1]={
    {GPIO_I,PIN7, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PU, AF_NUll},
};


/* ���� :------------PIN_NUM---------PIN
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
// ���ܣ����ô��ڱ��ΪSerialNo�Ĵ���Ϊ��˫�����͹���
// ������SerialNo,���ں�
// ���أ���
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
// ���ܣ����ô��ڱ��ΪSerialNo�Ĵ���Ϊ��˫�����չ���
// ������SerialNo,���ں�
// ���أ���
// ============================================================================
void Board_UartHalfDuplexRecv(u8 SerialNo)
{
    //����Ϊ485����
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
// ���ܣ�Board_GpioInit���������õ���GPIO��ʼ��
//  �����ｫ���п����õ��ĸ���ģʽ��gpio������������Ҫ��ȡ��ע�ͼ���
//  ���鲻Ҫɾ��
// ��������
// ���أ���
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



     //����ģ���Դ�������� add by zhb 20170726(����SR5333Ӳ���İ�)
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

//����ģ���Դ�������� add by zhb 20170726(����SR5333Ӳ���İ�)
void M4G_PowerSet(void)
{
     PIO_Set(&M4G_PowerCtrlPin[0]);
}
void M4G_PowerClr(void)
{
    PIO_Clear(&M4G_PowerCtrlPin[0]);
}
