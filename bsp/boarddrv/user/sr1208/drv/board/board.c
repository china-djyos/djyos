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
// �ļ���     ��board.c
// ģ������: sr1208�����ز��ֳ�ʼ�������õ�
// ģ��汾: V1.00
// ������Ա: YT
// ����ʱ��: 2018/5/17
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "cpu_peri_gpio.h"
#include "cpu_peri_iic.h"
#include "cpu_peri.h"
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

//pwm_out1(TIM3_CH3),pwm_out2(TIM4_CH2)4
static const Pin pwm_pin[] = {
        {GPIO_B,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,ST_AF2},
        {GPIO_B,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU,ST_AF2},
};

//485_1
static const Pin uart1_pin[] = {
        {GPIO_A,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_A,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_A,PIN8,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
};

//232
static const Pin uart3_pin[] = {
        {GPIO_D,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_D,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
};

//Ԥ��������ģ��ʹ��
static const Pin uart4_pin[] = {
        {GPIO_C,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_C,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
 };

//����ģ��ʹ��  ��Ϊͨ��ģ��
static const Pin uart5_pin[] = {
        {GPIO_C,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_D,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
 };
//485_2
static const Pin uart6_pin[] = {
        {GPIO_C,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_C,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_G,PIN8,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
 };

//DEBUG��
static const Pin uart8_pin[] = {
        {GPIO_E,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_E,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
};

//max31865  �¶�   SPI1
static const Pin spi1_pin[] = {
        {GPIO_A,PIN4,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},//cs
        {GPIO_A,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_A,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_B,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
};
static const Pin Max31865[] = {
        {GPIO_I,PIN11,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},//data ready
};



//AD8664 ADоƬ   SPI2
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


//������   SPI5
static const Pin spi5_pin[]={
        {GPIO_F,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_F,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_F,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
};
const Pin SpiCs[]={
        {GPIO_F,PIN6,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF_NUll},
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




//at45db321 flash   SPI4
static const Pin spi4_pin[]={
        {GPIO_E,PIN4,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},
        {GPIO_E,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_E,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_E,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
};
static const Pin At45Wp_pin[] ={
        {GPIO_H, PIN7, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_100M,GPIO_PUPD_NONE},
};




//IIC4 for at24128c eeprom
static const Pin iic4_pin[] = {
        {GPIO_F,PIN14,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},//scl
        {GPIO_F,PIN15,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},//sda
};



 //CAN ͨ��
static const Pin CAN1_pin[]={
    {GPIO_B,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF9}, //CAN1 RX
    {GPIO_H,PIN13,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF9}, //CAN1 TX
};

//USB����״ָ̬ʾ��
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

bool_t Max31865_DataReady(void)
{
    return PIO_Get(&Max31865[0]);
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
     PIO_Configure(uart4_pin, PIO_LISTSIZE(uart4_pin));   //uart ����ģ��
     PIO_Configure(uart5_pin, PIO_LISTSIZE(uart5_pin));   //uart ����ģ��
     PIO_Configure(uart8_pin, PIO_LISTSIZE(uart8_pin));   //debug

     PIO_Configure(spi1_pin, PIO_LISTSIZE(spi1_pin));      //SPI1 for max31865
     PIO_Configure(Max31865, PIO_LISTSIZE(Max31865));      //max3
     PIO_Set(&spi1_pin[0]);//csҪ����


     PIO_Configure(spi2_pin, PIO_LISTSIZE(spi2_pin));      //SPI2 for ads8664
     PIO_Configure(ADCS,PIO_LISTSIZE(ADCS));
     PIO_Configure(ADRST,PIO_LISTSIZE(ADRST));


     PIO_Configure(spi4_pin, PIO_LISTSIZE(spi4_pin));     //SPI4  for at45db321b
     PIO_Configure(At45Wp_pin, PIO_LISTSIZE(At45Wp_pin));
     PIO_Set(&At45Wp_pin[0]);
     PIO_Set(&spi4_pin[0]);


     PIO_Configure(spi5_pin, PIO_LISTSIZE(spi5_pin));      //SPI5 ������
     PIO_Configure(PenStatus,PIO_LISTSIZE(PenStatus));
     PIO_Configure(SpiCs,PIO_LISTSIZE(SpiCs));
     PIO_Configure(LCD_BLK,PIO_LISTSIZE(LCD_BLK));
     PIO_Configure(LCD_RST,PIO_LISTSIZE(LCD_RST));


     //IIC4 for eeprom
     extern bool_t IIC_Busfree(u32 port,u32 sda_pin,u32 sck_pin);
     IIC_Busfree(GPIO_F,PIN15,PIN14);
     PIO_Configure(iic4_pin, PIO_LISTSIZE(iic4_pin));

     //PHY оƬ
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

//     extern void KO_Init(void);
//     KO_Init();
//     extern void KI_Init(void);
//     KI_Init();
//     extern void ADC_Init(void);
//     ADC_Init();
//     extern void USB_IOs(void);
//     USB_IOs();
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
