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


#include "stdint.h"
#include "stddef.h"
#include "cpu_peri_gpio.h"
#include "cpu_peri_iic.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"board config"//�����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:required               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"kernel","stm32f7","cpu onchip gpio","cpu onchip iic"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_BOARD_CONFIG == false )
//#warning  " board_config  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header            //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_BOARD_CONFIG    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


//�����gpioֻ������һ���� ���õ���ͬ��GPIO��������ֲ����

static const Pin uart1_pin[] = {

        {GPIO_A,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_A,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},


//      {GPIO_B,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
//      {GPIO_B,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},

};


static const Pin uart2_pin[] = {
        {GPIO_A,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_A,PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},

//      {GPIO_D,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
//      {GPIO_D,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
};


static const Pin uart3_pin[] = {
        {GPIO_D,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
        {GPIO_D,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},

//      {GPIO_C,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
//      {GPIO_C,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
//
//      {GPIO_B,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
//      {GPIO_B,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF7},
};


static const Pin uart4_pin[] = {
        {GPIO_A,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_A,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},

//      {GPIO_C,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
//      {GPIO_C,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
//
 };


static const Pin uart5_pin[] = {
        {GPIO_C,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_D,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
 };

static const Pin uart6_pin[] = {
        {GPIO_C,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_C,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},

//      {GPIO_G,PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
//      {GPIO_G,PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
 };

static const Pin uart7_pin[] = {
        {GPIO_F,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_F,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},

//      {GPIO_E,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
//      {GPIO_E,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
//
 };
static const Pin uart8_pin[] = {
        {GPIO_E,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
        {GPIO_E,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE,AF8},
 };


static const Pin spi1_pin[] = {
        {GPIO_B,PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_B,PIN4,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_B,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
        {GPIO_F,PIN14,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},//��ͨ�����CS����

 };

static const Pin iic1_pin[] = {
        {GPIO_B,PIN8,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
        {GPIO_B,PIN9,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
 };


static const Pin iic2_pin[] = {
        {GPIO_B,PIN11,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
        {GPIO_B,PIN10,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
 };

static const Pin iic3_pin[] = {
        {GPIO_H,PIN7,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
        {GPIO_H,PIN8,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
 };

static const Pin iic4_pin[] = {
        {GPIO_H,PIN12,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
        {GPIO_H,PIN11,GPIO_MODE_AF,GPIO_OTYPE_OD,GPIO_SPEED_VH,GPIO_PUPD_PU,AF4},
 };


static const Pin EthMiiPins[] = {
    {GPIO_A,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_A,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_A,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_C,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_C,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_C,PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_C,PIN4,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_C,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_B,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_G,PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_G,PIN13,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_G,PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},

    {GPIO_E,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_H,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
    {GPIO_H,PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF11},
};

void __lcd_backlight_onoff(u8 onoff)
{
  if(onoff)
      GPIO_SettoLow(GPIO_A,PIN8);
  else
      GPIO_SettoHigh(GPIO_A,PIN8);
}
//----��ʼ��lcdӲ��------------------------------------------------------------
//����: ��ʼ��lcdӲ����
//����: ��
//����: ��
//��ע: ����������ֲ�ؼ�����
//----------------------------------------------------------------------------
void LTDC_MspInit(void)
{
    GPIO_PowerOn(GPIO_A);//鑳屽厜鎺у埗寮曡剼
    GPIO_PowerOn(GPIO_I);
    GPIO_PowerOn(GPIO_J);
    GPIO_PowerOn(GPIO_K);

    GPIO_CfgPinFunc(GPIO_A,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_NONE);
    GPIO_CfgPinFunc(GPIO_I,
            PIN12|PIN13|PIN14|PIN15,
            GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_NONE);
    GPIO_CfgPinFunc(GPIO_J,
            PIN0|PIN1|PIN2|PIN3|PIN4|PIN5|PIN6|PIN7|PIN8|PIN9|PIN10|PIN11|PIN12|PIN13|PIN14|PIN15,
            GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_NONE);
    GPIO_CfgPinFunc(GPIO_K,
            PIN0|PIN1|PIN2|PIN3|PIN4|PIN5|PIN6|PIN7,
            GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_NONE);

    GPIO_AFSet(GPIO_I,
            PIN12|PIN13|PIN14|PIN15,AF14);
    GPIO_AFSet(GPIO_J,
            PIN0|PIN1|PIN2|PIN3|PIN4|PIN5|PIN6|PIN7|PIN8|PIN9|PIN10|PIN11|PIN12|PIN13|PIN14|PIN15
            ,AF14);
    GPIO_AFSet(GPIO_K,
            PIN0|PIN1|PIN2|PIN3|PIN4|PIN5|PIN6|PIN7,AF14);
}
// ============================================================================
// ���ܣ����ô��ڱ��ΪSerialNo�Ĵ���Ϊ��˫�����͹���
// ������SerialNo,���ں�
// ���أ���
// ============================================================================
void Board_UartHalfDuplexSend(u8 SerialNo)
{
//  switch(SerialNo)
//  {
//  case CN_UART1:
//      PIO_Set(&uart1_pin[2]);
//      break;
//  case CN_UART6:
//      PIO_Set(&uart6_pin[2]);
//      break;
//  default:
//      break;
//  }
}

// ============================================================================
// ���ܣ����ô��ڱ��ΪSerialNo�Ĵ���Ϊ��˫�����չ���
// ������SerialNo,���ں�
// ���أ���
// ============================================================================
void Board_UartHalfDuplexRecv(u8 SerialNo)
{
    //����Ϊ485����
//  switch(SerialNo)
//  {
//  case CN_UART1:
//      PIO_Clear(&uart1_pin[2]);
//      break;
//  case CN_UART6:
//      PIO_Clear(&uart6_pin[2]);
//      break;
//  default:
//      break;
//  }
}

// =============================================================================
// ���ܣ�Board_GpioInit���������õ���GPIO��ʼ��
//  �����ｫ���п����õ��ĸ���ģʽ��gpio������������Ҫ��ȡ��ע�ͼ���
//  ���鲻Ҫɾ��
// ��������
// ���أ���
// =============================================================================
void Board_Init(void)
{

     PIO_Configure(uart1_pin, PIO_LISTSIZE(uart1_pin));      //UART1



     PIO_Configure(EthMiiPins, PIO_LISTSIZE(EthMiiPins));


//=====================IIC GPIO��ʼ��===================================//
//����û��Ƭѡ��ʹ�����ŵ�IIC��������CPU��λ������IIC���Ĺ����о��п��ܷ������߱�ռ���磺qh_1��stmpe811
//   ��������������I/Oģ��ķ�ʽ���������ͷ�
        IIC_Busfree(GPIO_B,PIN9,PIN8);//iic1
        PIO_Configure(iic1_pin, PIO_LISTSIZE(iic1_pin));      //IIC
//      IIC_Busfree(GPIO_B,PIN11,PIN10);
//      IIC_Busfree(GPIO_H,PIN8,PIN7);
//      IIC_Busfree(GPIO_H,PIN12,PIN11);
        LTDC_MspInit();
}

bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level)
{
    return true;
}
