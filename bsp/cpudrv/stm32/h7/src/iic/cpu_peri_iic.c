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

// �ļ���     ��stm32_iic_type1.c
// ����         ��czz
// ģ������: IICģ��ײ�Ӳ���������Ĵ�������Ĳ���
// ����ʱ��: 2018/08/16
// =============================================================================
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <cpu_peri_iic.h>
#include "cpu_peri.h"
#include "cpu_peri_uart.h"
#include "stdlib.h"
#include "iicbus.h"
#include "board-config.h"
//==============================================================================
//=========================DIDE config==========================================
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//bool_t ModuleInstall_IIC(ptu32_t port);
//    #if CFG_IIC1_ENABLE== true
//     ModuleInstall_IIC(CN_IIC1);
//    #endif
//    #if CFG_IIC2_ENABLE== true
//    ModuleInstall_IIC(CN_IIC2);
//    #endif
//    #if CFG_IIC3_ENABLE== true
//    ModuleInstall_IIC(CN_IIC3);
//    #endif
//    #if CFG_IIC4_ENABLE== true
//    ModuleInstall_IIC(CN_IIC4);
//    #endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip iic"//CPU��iic��������
//parent:"iicbus","cpu onchip iic"//��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                          //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                       //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                         //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                        //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                         //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"iicbus"//������������������������none����ʾ�������������
                                         //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                         //����������������������г�
//weakdependence:"none"                  //��������������������������none����ʾ�������������
                                         //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                         //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                         //�������������⣬�������г�
//%$#@end describe  ****�����������
//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_IIC == false )
//#warning  " cpu_onchip_iic  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_IIC    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,16,512,
#define CFG_IIC_BUF_LEN           32       //"IIC��������С",����IIC��������С
//%$#@enum,true,false,
#define CFG_IIC1_ENABLE           false     //"�Ƿ�ʹ��IIC1",
#define CFG_IIC2_ENABLE           false     //"�Ƿ�ʹ��IIC2",
#define CFG_IIC3_ENABLE           false     //"�Ƿ�ʹ��IIC3",
#define CFG_IIC4_ENABLE           false     //"�Ƿ�ʹ��IIC4",

//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,

#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure
//=====================DIDE config end ======================================================

// =============================================================================
// ����: ʹ����ֹͣ����������ʱ�Ӻ�uartģ��
// ����: port,���ں�
// ����: ��
// =============================================================================
//============================================================================================================
//============================================================================================================
typedef struct
{
  vu32 CR1;      /*!< I2C Control register 1,            Address offset: 0x00 */
  vu32 CR2;      /*!< I2C Control register 2,            Address offset: 0x04 */
  vu32 OAR1;     /*!< I2C Own address 1 register,        Address offset: 0x08 */
  vu32 OAR2;     /*!< I2C Own address 2 register,        Address offset: 0x0C */
  vu32 TIMINGR;  /*!< I2C Timing register,               Address offset: 0x10 */
  vu32 TIMEOUTR; /*!< I2C Timeout register,              Address offset: 0x14 */
  vu32 ISR;      /*!< I2C Interrupt and status register, Address offset: 0x18 */
  vu32 ICR;      /*!< I2C Interrupt clear register,      Address offset: 0x1C */
  vu32 PECR;     /*!< I2C PEC register,                  Address offset: 0x20 */
  vu32 RXDR;     /*!< I2C Receive data register,         Address offset: 0x24 */
  vu32 TXDR;     /*!< I2C Transmit data register,        Address offset: 0x28 */
} I2CType;
//============bitdef===========
/* STM32F7 I2C control 1 */
#define TYPE1_I2C_CR1_RXDMAEN         (1<<15)
#define TYPE1_I2C_CR1_TXDMAEN         (1<<14)
#define TYPE1_I2C_CR1_ANFOFF          (1<<12)
#define TYPE1_I2C_CR1_ERRIE           (1<<7)
#define TYPE1_I2C_CR1_TCIE            (1<<6)
#define TYPE1_I2C_CR1_STOPIE          (1<<5)
#define TYPE1_I2C_CR1_NACKIE          (1<<4)
#define TYPE1_I2C_CR1_ADDRIE          (1<<3)
#define TYPE1_I2C_CR1_RXIE            (1<<2)
#define TYPE1_I2C_CR1_TXIE            (1<<1)
#define TYPE1_I2C_CR1_PE              (1<<0)
#define TYPE1_I2C_ALL_IRQ_MASK      ( TYPE1_I2C_CR1_ERRIE \
                                    | TYPE1_I2C_CR1_TCIE \
                                    | TYPE1_I2C_CR1_STOPIE \
                                    | TYPE1_I2C_CR1_NACKIE \
                                    | TYPE1_I2C_CR1_RXIE \
                                    | TYPE1_I2C_CR1_TXIE)

/* STM32F7 I2C control 2 */
#define TYPE1_I2C_CR2_RELOAD          (1<<24)
#define TYPE1_I2C_CR2_AUTOEND          (1<<25)
#define TYPE1_I2C_CR2_NBYTES_MASK     (0xff<<16)
#define TYPE1_I2C_CR2_NBYTES(n)       (((n) & 0xff) << 16)
#define TYPE1_I2C_CR2_NACK            (1<<15)
#define TYPE1_I2C_CR2_STOP            (1<<14)
#define TYPE1_I2C_CR2_START           (1<<13)
#define TYPE1_I2C_CR2_HEAD10R         (1<<12)
#define TYPE1_I2C_CR2_ADD10           (1<<11)
#define TYPE1_I2C_CR2_RD_WRN          (1<<10)
#define TYPE1_I2C_CR2_SADD7_MASK      (0xfe)
#define TYPE1_I2C_CR2_SADD7(n)        (((n) & 0x7f) << 1)

/* STM32F7 I2C Interrupt Status */
#define TYPE1_I2C_ISR_BUSY            (1<<15)
#define TYPE1_I2C_ISR_ARLO            (1<<9)
#define TYPE1_I2C_ISR_BERR            (1<<8)
#define TYPE1_I2C_ISR_TCR             (1<<7)
#define TYPE1_I2C_ISR_TC              (1<<6)
#define TYPE1_I2C_ISR_STOPF           (1<<5)
#define TYPE1_I2C_ISR_NACKF           (1<<4)
#define TYPE1_I2C_ISR_RXNE            (1<<2)
#define TYPE1_I2C_ISR_TXIS            (1<<1)
#define TYPE1_I2C_ISR_ERRORS     (TYPE1_I2C_ISR_BERR|TYPE1_I2C_ISR_ARLO)

/* STM32F7 I2C Interrupt Clear */
#define TYPE1_I2C_ICR_ARLOCF          (1<<9)
#define TYPE1_I2C_ICR_BERRCF          (1<<8)
#define TYPE1_I2C_ICR_STOPCF          (1<<5)
#define TYPE1_I2C_ICR_NACKCF          (1<<4)

/* STM32F7 I2C Timing */
#define TYPE1_I2C_TIMINGR_PRESC(n)        (((n) & 0xf) << 28)
#define TYPE1_I2C_TIMINGR_SCLDEL(n)       (((n) & 0xf) << 20)
#define TYPE1_I2C_TIMINGR_SDADEL(n)       (((n) & 0xf) << 16)
#define TYPE1_I2C_TIMINGR_SCLH(n)         (((n) & 0xff) << 8)
#define TYPE1_I2C_TIMINGR_SCLL(n)         ((n) & 0xff)

#define TYPE1_I2C_MAX_LEN         0xff
#define TYPE1_I2C_DNF_DEFAULT     0
#define TYPE1_I2C_DNF_MAX         16

#define TYPE1_I2C_ANALOG_FILTER_ENABLE    1
#define TYPE1_I2C_ANALOG_FILTER_DELAY_MIN 50  /* ns */
#define TYPE1_I2C_ANALOG_FILTER_DELAY_MAX 260 /* ns */

#define TYPE1_I2C_RISE_TIME_DEFAULT       25  /* ns */
#define TYPE1_I2C_FALL_TIME_DEFAULT       10  /* ns */

#define TYPE1_PRESC_MAX           (1<<4)
#define TYPE1_SCLDEL_MAX          (1<<4)
#define TYPE1_SDADEL_MAX          (1<<4)
#define TYPE1_SCLH_MAX            (1<<8)
#define TYPE1_SCLL_MAX            (1<<8)

#define DIV_ROUND_CLOSEST(x, divisor)(          \
{                           \
    typeof(x) __x = x;              \
    typeof(divisor) __d = divisor;          \
    (((typeof(x))-1) > 0 ||             \
     ((typeof(divisor))-1) > 0 || (__x) > 0) ?  \
        (((__x) + ((__d) / 2)) / (__d)) :   \
        (((__x) - ((__d) / 2)) / (__d));    \
}                           \
)
#define NSEC_PER_SEC 1000000000L

struct TYPE1_i2c_spec {
    u32 rate;//*@���ʣ�I2C�����ٶȣ�Hz��
    u32 rate_min;//*@ rtIt min��I2C�����ٶȣ�Hz����80%
    u32 rate_max;//*@ RATEXMAX��I2C�����ٶȣ�Hz����100%
    u32 fall_max;//*@ FaluxMax��SDA��SCL�źţ�NS�������˹�½�ʱ��
    u32 rise_max;//*@ RISEXMAX��SDA��SCL�źţ�NS�������˹����ʱ��
    u32 hddat_min;//* @ HDDATA���ӣ���С���ݱ���ʱ�䣨NS��
    u32 vddat_max;//*@ VDDATYMAX�����˹������Чʱ�䣨NS��
    u32 sudat_min;//*@ SUDATYMIN����С���ݽ���ʱ�䣨NS��
    u32 l_min;//*@ Lmin Min��SCLʱ�ӵ���С�����ڣ�ns��
    u32 h_min;//*@ Hmin Min��SCLʱ�ӣ�NS����min������
};


struct TYPE1_i2c_setup {
    u8 speed;//*@�ٶȣ�I2C�ٶ�ģʽ����׼�����ټӣ�
    u32 speed_freq;//*@ SuffyFRIQ:I2C�ٶ�Ƶ�ʣ�Hz��
    u32 clock_src;//*@ CcLogySRC��I2Cʱ��ԴƵ�ʣ�Hz��
    u32 rise_time;//*@����ʱ�䣺����ʱ�䣨NS��
    u32 fall_time;//*@ʱ�䣺�½�ʱ�䣨ns��
    u8 dnf;//*@ DNF�������˲�ϵ����0��16��
    bool analog_filter;//*@��ȹ�������ģ���˲����ӳ٣���/�أ�
};

struct TYPE1_i2c_timings {
    u8 presc;//*PREC��Ԥ��Ƶ��ֵ
    u8 scldel;//*sCdel:��������ʱ��
    u8 sdadel;//*@ SDADEL�����ݱ���ʱ��
    u8 sclh;//*@ SCLH��SCL�����ڣ���ģʽ��
    u8 scll;//*@ SCLH��SCL�����ڣ���ģʽ��
};

enum TYPE1_I2C_speed {
    TYPE1_I2C_SPEED_STANDARD, /* 100 kHz */
    TYPE1_I2C_SPEED_FAST, /* 400 kHz */
    TYPE1_I2C_SPEED_FAST_PLUS, /* 1 MHz */
    TYPE1_I2C_SPEED_END,/*����Ƶ��*/
};

//==ģ���ڲ�ʹ�õ����ݽṹ
enum DmaState
{
    dma_sending,//���ڷ���
    dma_recveing,//���ڽ���
    dma_idle,   //����״̬
    DmaUnused,  //δʹ��DMAģʽ
};
enum IicError
{
    noError,
    Bus_error,
    ARlocf_error,
    buf_over,
};
enum  IICstate
{
    idle_state,
    Tx_send_memoryaddr,
    Rx_send_memoryaddr,
    Tx_buf,
    Rx_buf,
};


struct stm32_iicConfig
{
    I2CType * iicBaseAddr;   //iic�Ĵ����Ļ���ַ
    char* iicname  ;         //����
    u32   speed;             //Ĭ���ٶ������ϵ��ٶ�
    u16   iicbufLen;         //IICbus����������
    u32    iicEvIntLine;     //IIC�¼��ж��жϺ�
    u32    iicErIntLine;     //�����ж��жϺ�
    u32    DmaRxIntLine;     //DMA�����к�
    u32    DmaTxIntLine;     //DMA�����жϺ�
    void* DmaRxTag;          //���տ��ƼĴ�������ַ
    void* DmaTxTag;          //���Ϳ��ƼĴ�������ַ
    u16   DmaBufLen ;        //DMA����������
    u8    DmaSendchsel;      //����DMA�����������ػ�Ҫ����ͨ�������û����������
    u8    DmaRecvchsel;      //����DMA�����������ػ�Ҫ����ͨ�������û����������
};

static const struct stm32_iicConfig config[] = {
[CN_IIC1]={
 .iicBaseAddr    = (I2CType *)I2C1,         //iic�Ĵ����Ļ���ַ
 .iicname        = "IIC1",                  //����
 .speed          = 400*1000,                //Ĭ���ٶ������ϵ��ٶ�
 .iicbufLen      = CFG_IIC_BUF_LEN,         //IICbus����������
 .iicEvIntLine   = CN_INT_LINE_I2C1_EV,     //IIC�¼��жϺ�
 .iicErIntLine   = CN_INT_LINE_I2C1_ER,     //IIC�����жϺ�
 .DmaRxIntLine   = CN_INT_LINE_DMA1_Stream0,//DMA�����к�
 .DmaTxIntLine   = CN_INT_LINE_DMA1_Stream6,//DMA�����жϺ�
 .DmaRxTag       = DMA1_Stream0,            //���տ��ƼĴ�������ַ
 .DmaTxTag       = DMA1_Stream6,            //���Ϳ��ƼĴ�������ַ
 .DmaBufLen      = 32,                      //DMA����������
 .DmaSendchsel   = 1,                       //����DMA�����������ػ�Ҫ
 .DmaRecvchsel   = 1,                       //����ͨ�������û����������
},
[CN_IIC2]={
 .iicBaseAddr    = (I2CType *)I2C2,         //iic�Ĵ����Ļ���ַ
 .iicname        = "IIC2",                  //����
 .speed          = 400*1000,                //Ĭ���ٶ������ϵ��ٶ�
 .iicbufLen      = CFG_IIC_BUF_LEN,         //IICbus����������
 .iicEvIntLine   = CN_INT_LINE_I2C2_EV,     //IIC�¼��жϺ�
 .iicErIntLine   = CN_INT_LINE_I2C2_ER,     //IIC�����жϺ�
 .DmaRxIntLine   = CN_INT_LINE_DMA1_Stream3,//DMA�����к�
 .DmaTxIntLine   = CN_INT_LINE_DMA1_Stream7,//DMA�����жϺ�
 .DmaRxTag       = DMA1_Stream3,            //���տ��ƼĴ�������ַ
 .DmaTxTag       = DMA1_Stream7,            //���Ϳ��ƼĴ�������ַ
 .DmaBufLen      = 32,                      //DMA����������
 .DmaSendchsel   = 7,                       //����DMA�����������ػ�Ҫ
 .DmaRecvchsel   = 7,                       //����ͨ�������û����������
},
[CN_IIC3]={
 .iicBaseAddr    = (I2CType *)I2C3,         //iic�Ĵ����Ļ���ַ
 .iicname        = "IIC3",                  //����
 .speed          = 400*1000,                //Ĭ���ٶ������ϵ��ٶ�
 .iicbufLen      = CFG_IIC_BUF_LEN,         //IICbus����������
 .iicEvIntLine   = CN_INT_LINE_I2C3_EV,     //IIC�¼��жϺ�
 .iicErIntLine   = CN_INT_LINE_I2C3_ER,     //IIC�����жϺ�
 .DmaRxIntLine   = CN_INT_LINE_DMA1_Stream1,//DMA�����к�
 .DmaTxIntLine   = CN_INT_LINE_DMA1_Stream4,//DMA�����жϺ�
 .DmaRxTag       = DMA1_Stream1,            //���տ��ƼĴ�������ַ
 .DmaTxTag       = DMA1_Stream4,            //���Ϳ��ƼĴ�������ַ
 .DmaBufLen      = 32,                      //DMA����������
 .DmaSendchsel   = 1,                       //����DMA�����������ػ�Ҫ
 .DmaRecvchsel   = 3,                       //����ͨ�������û����������
},
[CN_IIC4]={
 .iicBaseAddr    = (I2CType *)I2C4,         //iic�Ĵ����Ļ���ַ
 .iicname        = "IIC4",                  //����
 .speed          = 400*1000,                //Ĭ���ٶ������ϵ��ٶ�
 .iicbufLen      = CFG_IIC_BUF_LEN,         //IICbus����������
 .iicEvIntLine   = CN_INT_LINE_I2C4_EV,     //IIC�¼��жϺ�
 .iicErIntLine   = CN_INT_LINE_I2C4_ER,     //IIC�����жϺ�
 .DmaRxIntLine   = CN_INT_LINE_DMA1_Stream2,//DMA�����к�
 .DmaTxIntLine   = CN_INT_LINE_DMA1_Stream5,//DMA�����жϺ�
 .DmaRxTag       = DMA1_Stream2,            //���տ��ƼĴ�������ַ
 .DmaTxTag       = DMA1_Stream5,            //���Ϳ��ƼĴ�������ַ
 .DmaBufLen      = 32,                      //DMA����������
 .DmaSendchsel   = 2,                       //����DMA�����������ػ�Ҫ
 .DmaRecvchsel   = 2,                       //����ͨ�������û����������
}};
static struct
{
    struct SemaphoreLCB *pDrvPostSemp;   //�ź���
    u8  DevAddr;                         //���������ϵ�ַ
    u8  MemAddr[4];                      //�����ڲ���ַ����
    u32 Addrlenbak;                      //�����ڲ���ַ���ȱ���
    u32 Addrlen;                         //�����ڲ���ַ���ȼ�������ʱ��ı�
    u32 len;                             //�շ�������������
    enum  IICstate IICstate;             //IIC����״̬
    enum DmaState  pDmaState;            //DMA����״̬
    struct IIC_CB * pIIC_CB;             //ָ��IIC_CB���
    enum IicError error;                 //��¼���һ�γ�������
    u8* Dmabuf;
    u32 DmaRcvlen;
} pStm32Icb[CN_IIC_NUM];
//==============================================================================

/**
  * All these values are coming from I2C Specification, Version 6.0, 4th of
  * April 2014.
  *
  * Table10. Characteristics of the SDA and SCL bus lines for Standard, Fast,
  * and Fast-mode Plus I2C-bus devices
  */
const static struct TYPE1_i2c_spec i2c_specs[] = {
    [TYPE1_I2C_SPEED_STANDARD] = {
        .rate = 100000,
        .rate_min = 80000,
        .rate_max = 100000,
        .fall_max = 300,
        .rise_max = 1000,
        .hddat_min = 0,
        .vddat_max = 3450,
        .sudat_min = 250,
        .l_min = 4700,
        .h_min = 4000,
    },
    [TYPE1_I2C_SPEED_FAST] = {
        .rate = 400000,
        .rate_min = 320000,
        .rate_max = 400000,
        .fall_max = 300,
        .rise_max = 300,
        .hddat_min = 0,
        .vddat_max = 900,
        .sudat_min = 100,
        .l_min = 1300,
        .h_min = 600,
    },
    [TYPE1_I2C_SPEED_FAST_PLUS] = {
        .rate = 1000000,
        .rate_min = 800000,
        .rate_max = 1000000,
        .fall_max = 100,
        .rise_max = 120,
        .hddat_min = 0,
        .vddat_max = 450,
        .sudat_min = 50,
        .l_min = 500,
        .h_min = 260,
    },
    [TYPE1_I2C_SPEED_END] = {
        .rate = 100000000,//*@���ʣ�I2C�����ٶȣ�Hz��
        .rate_min = 100,//*@ rtIt min��I2C�����ٶȣ�Hz����80%
        .rate_max = 100000000,//*@ RATEXMAX��I2C�����ٶȣ�Hz����100%
        .fall_max = 300,//*@ FaluxMax��SDA��SCL�źţ�NS�������˹�½�ʱ��
        .rise_max = 120,//*@ RISEXMAX��SDA��SCL�źţ�NS�������˹����ʱ��
        .hddat_min = 0,//* @ HDDATA���ӣ���С���ݱ���ʱ�䣨NS��
        .vddat_max = 450,//*@ VDDATYMAX�����˹������Чʱ�䣨NS��
        .sudat_min = 50,//*@ SUDATYMIN����С���ݽ���ʱ�䣨NS��
        .l_min = 500,//*@ Lmin Min��SCLʱ�ӵ���С�����ڣ�ns��
        .h_min = 260,//*@ Hmin Min��SCLʱ�ӣ�NS����min������
    },
};

//==============================================================================
static u32 iic_dmarx_isr(ptu32_t port);//IIC DMA�����жϷ�����
static u32 iic_dmatx_isr(ptu32_t port);//IIC DMA�����жϷ�����
static bool_t IIC_IntEnable(ptu32_t port);
//==============================================================================
//�жϷ�ʽ�շ���������
//
//==============================================================================
static bool_t IIC_RxTxInt_Init(ptu32_t port)
{
    u32 cr1, cr2;
    bool_t Rdflag = false;
    u32 len;
    I2CType*reg = (I2CType*) config[port].iicBaseAddr;
    reg->CR1&=~TYPE1_I2C_CR1_PE;
    switch (pStm32Icb[port].IICstate)
    {
        case Tx_send_memoryaddr:
            len = pStm32Icb[port].Addrlen + pStm32Icb[port].len;
            break;
        case Rx_send_memoryaddr:
            len = pStm32Icb[port].Addrlen;
            break;
//        case Tx_buf:            break; //д��ʱ���ַ������һ�η���
        case Rx_buf:
            len = pStm32Icb[port].len;
            Rdflag = true;
            break;
        default:   return false;
    }

    cr1 = reg->CR1;
    cr2 = reg->CR2;
    /* Set transfer direction */
    cr2 &= ~TYPE1_I2C_CR2_RD_WRN;
    if (Rdflag)
        cr2 |= TYPE1_I2C_CR2_RD_WRN;

    /* Set slave address */
    cr2 &= ~TYPE1_I2C_CR2_SADD7_MASK;
    cr2 |= TYPE1_I2C_CR2_SADD7(pStm32Icb[port].DevAddr);

    /* Set nb bytes to transfer and reload if needed */
    cr2 &= ~(TYPE1_I2C_CR2_NBYTES_MASK | TYPE1_I2C_CR2_RELOAD);
    if (len > TYPE1_I2C_MAX_LEN) {
        cr2 |= TYPE1_I2C_CR2_NBYTES(TYPE1_I2C_MAX_LEN);
        cr2 |= TYPE1_I2C_CR2_RELOAD;
    }
    else
    {
        cr2 |= TYPE1_I2C_CR2_NBYTES(len);
    }

    /* Enable NACK, STOP, error and transfer complete interrupts */
    cr1 |= TYPE1_I2C_CR1_ERRIE | TYPE1_I2C_CR1_TCIE |
        TYPE1_I2C_CR1_STOPIE | TYPE1_I2C_CR1_NACKIE;

    /* Clear TX/RX interrupt */
    cr1 &= ~(TYPE1_I2C_CR1_RXIE | TYPE1_I2C_CR1_TXIE);

    /* Enable RX/TX interrupt according to msg direction */
    if (Rdflag)
        cr1 |= TYPE1_I2C_CR1_RXIE;
    else
        cr1 |= TYPE1_I2C_CR1_TXIE;

    /* Write configurations registers */
    reg->CR1 = cr1;
    reg->CR2 = cr2;
    /* Configure Start/Repeated Start */
    reg->CR1 |=TYPE1_I2C_CR1_PE;
    reg->CR2 |= TYPE1_I2C_CR2_START;
    return true;
}

//==============================================================================
// DMA��ʽ�շ���������
//
//==============================================================================
static bool_t IIC_RxTxDMA_Init(ptu32_t port)
{
    u32 cr1, cr2;
    bool_t Rdflag = false;
    u32 len,num;
    I2CType*reg = (I2CType*) config[port].iicBaseAddr;
    reg->CR1&=~TYPE1_I2C_CR1_PE;
    switch (pStm32Icb[port].IICstate)
    {
        case Tx_send_memoryaddr:
            len = pStm32Icb[port].Addrlen + pStm32Icb[port].len;
            break;
        case Rx_send_memoryaddr:
            len = pStm32Icb[port].Addrlen;
            break;
        case Tx_buf:
            break;
        case Rx_buf:
            len = pStm32Icb[port].len;
            Rdflag = true;
            break;
        default:   return false;
    }

    cr1 = reg->CR1;
    cr2 = reg->CR2;
    /* Set transfer direction */
    cr2 &= ~TYPE1_I2C_CR2_RD_WRN;
    if (Rdflag)
        cr2 |= TYPE1_I2C_CR2_RD_WRN;

    /* Set slave address */
    cr2 &= ~TYPE1_I2C_CR2_SADD7_MASK;
    cr2 |= TYPE1_I2C_CR2_SADD7(pStm32Icb[port].DevAddr);

    /* Set nb bytes to transfer and reload if needed */
    cr2 &= ~(TYPE1_I2C_CR2_NBYTES_MASK | TYPE1_I2C_CR2_RELOAD);
    if (len > TYPE1_I2C_MAX_LEN) {
        cr2 |= TYPE1_I2C_CR2_NBYTES(TYPE1_I2C_MAX_LEN);
        cr2 |= TYPE1_I2C_CR2_RELOAD;
    }
    else
    {
        cr2 |= TYPE1_I2C_CR2_NBYTES(len);
    }

    /* Enable NACK, STOP, error and transfer complete interrupts */
    cr1 |= TYPE1_I2C_CR1_ERRIE | TYPE1_I2C_CR1_TCIE |TYPE1_I2C_CR1_STOPIE|\
           TYPE1_I2C_CR1_NACKIE|TYPE1_I2C_ISR_TCR|TYPE1_I2C_ISR_TC;

    /* Clear TX/RX interrupt */
    cr1 &= ~(TYPE1_I2C_CR1_RXIE | TYPE1_I2C_CR1_TXIE);

    /* Enable RX/TX DMA interrupt according to msg direction */
    if (Rdflag)
    {
        num = len>config[port].DmaBufLen ? \
                config[port].DmaBufLen:len;
        DMA_Enable(config[port].DmaRxTag,(u32)pStm32Icb[port].Dmabuf,num);
        pStm32Icb[port].pDmaState = dma_recveing;
        pStm32Icb[port].DmaRcvlen = num;
        pStm32Icb[port].len-=num;
        cr1 |= TYPE1_I2C_CR1_RXDMAEN;
    }
    else
    {
        if(pStm32Icb[port].Addrlen != 0)
        {
            memcpy(pStm32Icb[port].Dmabuf,&pStm32Icb[port].MemAddr,pStm32Icb[port].Addrlen);
        }
        if((pStm32Icb[port].IICstate == Rx_send_memoryaddr))
        {
            num = pStm32Icb[port].Addrlen;
        }
        else
        {
            num = IIC_PortRead(pStm32Icb[port].pIIC_CB ,\
                            (u8*)((u32)pStm32Icb[port].Dmabuf+pStm32Icb[port].Addrlen),\
                             config[port].DmaBufLen-pStm32Icb[port].Addrlen);
            if(num > 0)
            {
                pStm32Icb[port].len-=num;
                pStm32Icb[port].IICstate = Tx_buf;
                num+=pStm32Icb[port].Addrlen;
            }
        }
        pStm32Icb[port].pDmaState = dma_sending;
        DMA_Enable(config[port].DmaTxTag,(u32)pStm32Icb[port].Dmabuf,num);
        cr1 |= TYPE1_I2C_CR1_TXDMAEN;
    }

    /* Write configurations registers */
    reg->CR1 = cr1;
    reg->CR2 = cr2;
    reg->CR1 |=TYPE1_I2C_CR1_PE;
    reg->CR2 |= TYPE1_I2C_CR2_START;
    return true;
}

// =============================================================================
// ����: ����дʱ������дʱ��Ĺ���Ϊ��������ַ��д�����洢��ַ��д�������洢��ַ��
//       ��ʱ������жϣ��������üĴ���Ϊ����ģʽ��֮�󽫻ᷢ�������жϣ����жϷ���
//       �����У�ÿ�η���һ�����ֽ�����ֱ��len��������post�ź���iic_bus_semp��������
//       ֹͣʱ��
// ����: SpecificFlag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//      DevAddr,������ַ��ǰ7���أ��ѽ��ڲ���ַ��ռ��bitλ���£��ú����轫�õ�ַ��
//               ��һλ�����������һλ��/д����;
//      MemAddr,�洢���ڲ���ַ�������͵������ϵĵ�ַ���õ�ַδ��������������ַ�ϵ�
//               ����λ��
//      MenAddrLen,�洢���ڲ���ַ�ĳ��ȣ��ֽڵ�λ��δ������������ַ����ı���λ��
//      Length,���յ������������������ݵĵ�����һ�ֽڣ���count-1��ֹͣ����ACK�źţ�
//          �����յ��ֽ���Ϊcountʱ������ֹͣʱ�򣬲��ͷ��ź���iic_semp;
//      IIC_BusSemp,�������ʱ�������ͷŵ��ź��������ͳ����IIC_PortReadʱ������������
//          �����ͷŸ��ź�������������ʱ��
// ����: TRUE��������ʱ��ɹ���FALSEʧ��
// =============================================================================
static bool_t IIC_WriteStartFunc(ptu32_t port,u8 DevAddr,\
                                u32 MemAddr,u8 MemAddrLen, u32 Length,\
                                struct SemaphoreLCB *IIC_BusSemp)
{
    pStm32Icb[port].pDrvPostSemp    = IIC_BusSemp;
    pStm32Icb[port].len             = Length;
    pStm32Icb[port].Addrlen         = MemAddrLen;
    pStm32Icb[port].Addrlenbak      = MemAddrLen;
    pStm32Icb[port].DevAddr         = DevAddr;
    for(u8 i=0;i<MemAddrLen;i++)
        pStm32Icb[port].MemAddr[(MemAddrLen-1)-i]=((MemAddr>>(8*i))&0xff);
    if(MemAddrLen!=0)
        pStm32Icb[port].IICstate = Tx_send_memoryaddr;
    else
        pStm32Icb[port].IICstate = Tx_buf;
    if(pStm32Icb[port].pDmaState ==  DmaUnused)
        return IIC_RxTxInt_Init(port);
    else
        return IIC_RxTxDMA_Init(port);
}

// =============================================================================
// ����: ������ʱ��������ʱ��Ĺ���Ϊ��������ַ��д�����洢��ַ��д����������ַ������
//       ��������ַ���������ʱ������жϣ��������üĴ���Ϊ����ģʽ��֮�󽫻ᷢ��
//       ���������жϣ����ж��н����յ������ݵ���IIC_PortWriteд�뻺�壬���յ�len��
//       ���������ݺ��ͷ��ź���iic_semp
// ����: SpecificFlag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//      DevAddr,������ַ��ǰ7���أ��ѽ��ڲ���ַ��ռ��bitλ���£��ú����轫�õ�ַ��
//               ��һλ�����������һλ��/д����;
//     MemAddr,�洢���ڲ���ַ�������͵������ϵĵ�ַ���õ�ַδ��������������ַ�ϵ�
//               ����λ��
//      MenAddrLen,�洢���ڲ���ַ�ĳ��ȣ��ֽڵ�λ��δ������������ַ����ı���λ��
//      Length,���յ������������������ݵĵ�����һ�ֽڣ���count-1��ֹͣ����ACK�źţ�����
//          �յ��ֽ���Ϊcountʱ������ֹͣʱ�򣬲��ͷ��ź���iic_semp;
//     IIC_BusSemp,�����ʱ���������ͷŵ��ź������������ź�����
// ����: TRUE��������ʱ��ɹ���FALSEʧ��
// =============================================================================
static  bool_t  IIC_ReadStartFunc (ptu32_t port,u8 DevAddr,\
                                u32 MemAddr,u8 MemAddrLen, u32 Length,\
                                struct SemaphoreLCB *IIC_BusSemp)
{
    pStm32Icb[port].pDrvPostSemp    = IIC_BusSemp;
    pStm32Icb[port].len             = Length;
    pStm32Icb[port].Addrlen         = MemAddrLen;
    pStm32Icb[port].Addrlenbak      = MemAddrLen;
    pStm32Icb[port].DevAddr         = DevAddr;
    for(u8 i=0;i<MemAddrLen;i++)
        pStm32Icb[port].MemAddr[(MemAddrLen-1)-i]=((MemAddr>>(8*i))&0xff);
    if(MemAddrLen!=0)
        pStm32Icb[port].IICstate = Rx_send_memoryaddr;
    else
        pStm32Icb[port].IICstate = Rx_buf;
    if(pStm32Icb[port].pDmaState == DmaUnused)
        return IIC_RxTxInt_Init( port);
    else
        return IIC_RxTxDMA_Init(port);
}

// =============================================================================
// ����: �������ζ�д�ص����������ֶ�д�Ĳ�ֹͬͣʱ�򣬵����ڷ���ʱ����ֱ��ֹͣʱ��
//      ��Ϊ��������ֹͣ�ظ�ACK����ֹͣ��
// ����: SpecificFlag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ��
// ����: ��
// =============================================================================
static  void IIC_GenerateEndFunc(ptu32_t port)
{
    I2CType*reg = (I2CType*) config[port].iicBaseAddr;
    switch (pStm32Icb[port].pDmaState)
    {
        case DmaUnused:
             pStm32Icb[port].IICstate = DmaUnused;
             reg->CR1 &=~TYPE1_I2C_ALL_IRQ_MASK;
             reg->CR1 &=~TYPE1_I2C_CR1_PE;
             break;
        case dma_sending:
            DMA_Disable(config[port].DmaTxTag);
            break;
        case dma_recveing:
            DMA_Disable(config[port].DmaRxTag);
            break;
        case dma_idle:            break;
        default:                 break;
    }

}

//==============================================================================
// ���ܣ�����Ҫ���õ�ʱ��Ƶ�ʼ���Ҫ���õļĴ���,�������п����õ�ֵȡ�����С��ֵ
// ������setup �������
//     output ���Ҫ���üĴ�����ֵ
// ���أ�true /false
//==============================================================================
static bool_t compute_timing(struct TYPE1_i2c_setup *setup,struct TYPE1_i2c_timings *output)
{
    u32 clk_error_prev, af_delay_min  ,af_delay_max ,dnf_delay;
    int sdadel_max,scldel_min ,sdadel_min;
    u32 clk_max,clk_min,tsync;

    bool_t flag  = false;
    u32 p_prev = TYPE1_PRESC_MAX;
    float i2cclk = DIV_ROUND_CLOSEST(NSEC_PER_SEC,
                       setup->clock_src);
    float i2cbus = DIV_ROUND_CLOSEST(NSEC_PER_SEC,
                       setup->speed_freq);

    if ((setup->rise_time > i2c_specs[setup->speed].rise_max) ||\
        (setup->fall_time > i2c_specs[setup->speed].fall_max) ||\
        (setup->dnf > TYPE1_I2C_DNF_MAX) ||\
        (setup->speed_freq > i2c_specs[setup->speed].rate))
        return false;

    clk_error_prev = i2cbus;
    /*  Analog and Digital Filters ģ��������˲��� */
    af_delay_min =(setup->analog_filter ?TYPE1_I2C_ANALOG_FILTER_DELAY_MIN : 0);
    af_delay_max =(setup->analog_filter ?TYPE1_I2C_ANALOG_FILTER_DELAY_MAX : 0);
    dnf_delay = setup->dnf * i2cclk;

    //���ݱ���ʱ����С�����ֵ
    sdadel_min  =  (setup->fall_time - i2c_specs[setup->speed].hddat_min) ;
    sdadel_min -=  af_delay_min;
    sdadel_min -=  (setup->dnf + 3) * i2cclk;

    sdadel_max = i2c_specs[setup->speed].vddat_max - setup->rise_time ;
    sdadel_max-= af_delay_max;
    sdadel_max-= (setup->dnf + 4) * i2cclk;

    if (sdadel_min < 0)
        sdadel_min = 0;
    if (sdadel_max < 0)
        sdadel_max = 0;

    scldel_min = setup->rise_time + i2c_specs[setup->speed].sudat_min;

    clk_max = NSEC_PER_SEC / i2c_specs[setup->speed].rate_min;
    clk_min = NSEC_PER_SEC / i2c_specs[setup->speed].rate_max;
    tsync = af_delay_min + dnf_delay + (2 * i2cclk);

    /* Compute possible values for PRESC, SCLDEL and SDADEL*/
    for (u16 p = 0; p < TYPE1_PRESC_MAX; p++) ///Ԥ��Ƶֵ
    {
        for (u16 l = 0; l < TYPE1_SCLDEL_MAX; l++)
        {
            u32 scldel = (l + 1) * (p + 1) * i2cclk;//���ݽ���ʱ��
            if (scldel < scldel_min)
                continue;

            for (u16 a = 0; a < TYPE1_SDADEL_MAX; a++)
            {
                u32 sdadel = (a * (p + 1) + 1) * i2cclk;//���ݱ���ʱ��
                if (((sdadel >= sdadel_min) &&(sdadel <= sdadel_max)) &&(p != p_prev))
                {
                    p_prev = p;
                    u32 prescaler = (p + 1) * i2cclk;//��Ƶ��ʱ������
                    for (u16 scll = 0; scll < TYPE1_SCLL_MAX; scll++)
                    {
                        u32 tscl_l = (scll + 1) * prescaler + tsync;//ʱ�ӵ�ά��ʱ��
                        if ((tscl_l < i2c_specs[setup->speed].l_min) ||\
                              (i2cclk >=((tscl_l - af_delay_min - dnf_delay) / 4)))
                            continue;
                        for (u16 sclh = 0; sclh < TYPE1_SCLH_MAX; sclh++)
                        {
                            u32 tscl_h = (sclh + 1) * prescaler + tsync;//ʱ�Ӹ�ά��ʱ��
                            u32 tscl = tscl_l + tscl_h +\
                                    setup->rise_time + setup->fall_time;

                            if ((tscl_h >= i2c_specs[setup->speed].h_min) &&\
                                (tscl >= clk_min) && (tscl <= clk_max) &&\
                                (i2cclk < tscl_h))//������׼��Ƶ��
                            {
                                s32 clk_error = tscl - i2cbus;
                                if (clk_error < 0)
                                    clk_error = -clk_error;

                                if (clk_error < clk_error_prev)
                                {
                                    clk_error_prev = clk_error;
                                    output->presc = p;
                                    output->scll = scll;
                                    output->sclh = sclh;
                                    output->scldel = l;
                                    output->sdadel = a;
                                    flag  = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return flag;
}

//=============================================================================
// ����: ����ʱ��Ƶ��
// ����: port,�˿ںţ���IIC�ţ�Ϊ1~3
//      clk_rate Ҫ���õ�Ƶ��
// ����: true/false
// =============================================================================
static bool_t IIC_ClkSet(ptu32_t port,ptu32_t clk_rate)
{
    struct TYPE1_i2c_setup setup ;
    struct TYPE1_i2c_timings STiming;
    u32 timingr = 0;
    bool_t flag = false;
    I2CType*reg = (I2CType*) config[port].iicBaseAddr;

    setup.rise_time     = TYPE1_I2C_RISE_TIME_DEFAULT;
    setup.fall_time     = TYPE1_I2C_FALL_TIME_DEFAULT;
    setup.dnf           = TYPE1_I2C_DNF_DEFAULT;
    setup.analog_filter = TYPE1_I2C_ANALOG_FILTER_ENABLE;
    setup.analog_filter = TYPE1_I2C_ANALOG_FILTER_ENABLE;
    setup.speed_freq    = clk_rate;
    setup.clock_src     = CN_CFG_PCLK1;
    setup.speed = TYPE1_I2C_SPEED_STANDARD;
    do {
        flag = compute_timing(&setup,&STiming);
        if (flag == false)
        {
            if (setup.speed < TYPE1_I2C_SPEED_FAST_PLUS)
            {
                setup.speed ++;
            }
            else
            {
                break;
            }
        }
    } while (!flag);

    if(flag == true)
    {
        printf("IIC%d ����ʱ��  %d hz�ɹ� speed = %d ����\n\r ",port+1,clk_rate,setup.speed);
        reg->CR1 &=~ TYPE1_I2C_CR1_PE;
        timingr |= TYPE1_I2C_TIMINGR_PRESC(STiming.presc);
        timingr |= TYPE1_I2C_TIMINGR_SCLDEL(STiming.scldel);
        timingr |= TYPE1_I2C_TIMINGR_SDADEL(STiming.sdadel);
        timingr |= TYPE1_I2C_TIMINGR_SCLH(STiming.sclh);
        timingr |= TYPE1_I2C_TIMINGR_SCLL(STiming.scll);
        reg->TIMINGR = timingr;

        if (setup.analog_filter) /* Enable I2C ģ���˲�*/
            reg->CR1 &=~ TYPE1_I2C_CR1_ANFOFF;
        else
            reg->CR1 |=  TYPE1_I2C_CR1_ANFOFF;
        reg->CR1 &=~ TYPE1_I2C_CR1_PE;
        return true;
    }
    printf("IIC%d ����ʱ��  %d hzʧ�� ����\n\r ",port+1,clk_rate);
    return false;
}

//=============================================================================
// ����: ����ʹ��DMA��IICʹ��DMA�շ���ÿ�շ�һ���ֽھͱ����ж�һ�Σ��������ڸ���
//      ����ͨ����ʹ�á�
// ����: port,�˿ںţ���IIC�ţ�Ϊ1~3
// ����: ��
// =============================================================================
static bool_t Set_IIC_DMA_USED(ptu32_t port)
{
    struct HeapCB *heap;
    u32 DmaBufLen;
    ufast_t ufl_line;
    I2CType*reg = (I2CType*) config[port].iicBaseAddr;
    pStm32Icb[port].IICstate = idle_state;
    if (pStm32Icb[port].pDmaState != DmaUnused)//�Ѿ���DMAģʽ��ֱ�ӷ���
        return false;
    if(pStm32Icb[port].Dmabuf == NULL)
    {
        heap =Heap_FindHeap("nocache");
        if(heap==NULL)
        {
            heap =Heap_FindHeap("sys"); //���û�С�nocache���Ѿ�ʹ��ϵͳ��
            printf("����UART%d:����DMA ģʽû���ҵ� nocache Heap",port+1);;
        }
        DmaBufLen =config[port].DmaBufLen;

        //��ЩDMA������Ҫ32�ֽڶ���
        pStm32Icb[port].Dmabuf = M_MallocHeap(DmaBufLen+32,heap,0);
        if(pStm32Icb[port].Dmabuf == NULL)
        {
            printf("�ڴ治������DMAģʽʧ�� \n\r");
            return false;
        }
        pStm32Icb[port].Dmabuf = (u8 *)((u32)(pStm32Icb[port].Dmabuf+32)&(~31));
    }
    reg->CR1 &= ~(TYPE1_I2C_CR1_TXIE|TYPE1_I2C_CR1_TXIE|TYPE1_I2C_CR1_ERRIE|\
                  TYPE1_I2C_CR1_TCIE|TYPE1_I2C_CR1_STOPIE|TYPE1_I2C_CR1_NACKIE);
    pStm32Icb[port].pDmaState = dma_idle;

    //���ý���DMA
    DMA_Config(config[port].DmaRxTag,config[port].DmaRecvchsel,\
                (u32)(&reg->RXDR),false,(u32)pStm32Icb[port].Dmabuf,0,DMA_DIR_P2M,\
                DMA_DATABITS_8,DMA_DATABITS_8,config[port].DmaBufLen);
    DMA_SetIntModeComplete(config[port].DmaRxTag);
    //����
    DMA_Config(config[port].DmaTxTag,config[port].DmaSendchsel,\
                 (u32)(&reg->TXDR),false,(u32)pStm32Icb[port].Dmabuf,0,DMA_DIR_M2P,\
                 DMA_DATABITS_8,DMA_DATABITS_8,config[port].DmaBufLen);

    ufl_line = config[port].DmaRxIntLine;
    Int_Register(ufl_line);//ע�ᷢ��DMA�ж�
    Int_SetClearType(ufl_line,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(ufl_line,iic_dmarx_isr);
    Int_SetIsrPara(ufl_line,port);
    Int_ClearLine(ufl_line);
    Int_SettoAsynSignal(ufl_line);
    Int_RestoreAsynLine(ufl_line);

    ufl_line = config[port].DmaTxIntLine;
    Int_Register(ufl_line);//ע�����DMA�ж�
    Int_SetClearType(ufl_line,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(ufl_line,iic_dmatx_isr);//�����ж�
    Int_SetIsrPara(ufl_line,port);
    Int_ClearLine(ufl_line);
    Int_SettoAsynSignal(ufl_line);
    Int_RestoreAsynLine(ufl_line);

    return true;
}
// =============================================================================
// ����: ����IICʹ��dma�շ�������stm32��IIC���շ�dmaͨ�����üĴ����������³�
//       ʼ������dma�Ĵ�������Ϊ:
//       1����ʹ��dma�ڼ�����������豸ʹ��dma���Ӷ����������á�
//       2����ʹIICʹ��dma�ڼ䣬Ҳ�������ģ����ϵ����⵼�������豸ʹ��dma.
// ����: port,�˿ںţ���IIC�ţ���1~3
// ����: ��
// =============================================================================
static bool_t Set_IIC_DMA_UNUSED(ptu32_t port)
{
    I2CType*reg = (I2CType*) config[port].iicBaseAddr;
    pStm32Icb[port].IICstate = idle_state;

    reg->CR1 &= ~(TYPE1_I2C_CR1_RXDMAEN|TYPE1_I2C_CR1_TXDMAEN);

    if(pStm32Icb[port].pDmaState != DmaUnused)
    {
        Int_SaveAsynLine(config[port].DmaRxIntLine);
        Int_SaveAsynLine(config[port].DmaTxIntLine);
        pStm32Icb[port].pDmaState = DmaUnused;
    }
    IIC_IntEnable(port);
    return true;
}
// =============================================================================
// ����: ʹIIC��ʼ������ʱ�Ӻ�IICģ��
// ����: port,IIC��
// ����: ��
// =============================================================================
static bool_t IIC_IntDisable(ptu32_t port)
{
    I2CType*reg = (I2CType*) config[port].iicBaseAddr;
    Int_SaveAsynLine(config[port].iicErIntLine);
    Int_SaveAsynLine(config[port].iicEvIntLine);
    reg->CR1 &= ~TYPE1_I2C_ALL_IRQ_MASK;
    pStm32Icb[port].IICstate = idle_state;
    return true;
}
// =============================================================================
// ����: ʹIIC��ʼ������ʱ�Ӻ�IICģ��
// ����: port,IIC��
// ����: ��
// =============================================================================
static bool_t IIC_IntEnable(ptu32_t port)
{
    I2CType*reg = (I2CType*) config[port].iicBaseAddr;
    Int_RestoreAsynLine(config[port].iicErIntLine);
    Int_RestoreAsynLine(config[port].iicEvIntLine);
    reg->CR1 |= TYPE1_I2C_ALL_IRQ_MASK;
    pStm32Icb[port].IICstate = idle_state;
    return true;
}

// =============================================================================
// ���ܣ�IIC���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�IICʱ��Ƶ������
// ������SpecificFlag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       cmd,����
//       data1,data2,���ݣ�������������
// ���أ���
// =============================================================================
static s32 IIC_BusCtrlFunc (ptu32_t port,u32 cmd, ptu32_t data1,ptu32_t data2)
{
    switch(cmd)
    {
         case  CN_IIC_SET_CLK:
             IIC_ClkSet(port,data1);
             break;    //����ʱ��Ƶ��
         case  CN_IIC_DMA_USED :
             Set_IIC_DMA_USED(port);
             break;    //ʹ��dma����
         case  CN_IIC_DMA_UNUSED:
             Set_IIC_DMA_UNUSED(port);
             break;    //��ֹdma����
         case  CN_IIC_SET_POLL  :
             IIC_IntDisable(port);
             break;    //ʹ����ѯ��ʽ���ͽ���
         case  CN_IIC_SET_INT :
             IIC_IntEnable(port);
             break;     //ʹ���жϷ�ʽ���ͽ���
         case  CN_IIC_OS_USED :

             break;
         default:            return 0;
    }
    return 1;
}
// =============================================================================
// ���ܣ���ѯ��ʽ��IIC���豸�շ�����
// ������reg,�Ĵ�����ַ
//       devaddr,�豸��ַ
//       memaddr,�豸�ڲ���ַ
//       maddrlen,�豸�ڲ���ַ����
//       buf,�洢����
//       len,���ݳ��ȣ��ֽ�
// ���أ�len,��ȡ�ɹ�;-1,��ȡʧ��
// =============================================================================
static bool_t IIC_wait_flags(I2CType *reg,u32 flags,u32 *status)
{
    u32 timeout = 1000;
     *status = reg->ISR;
    while ((!(*status & flags))&&timeout)
    {
        timeout--;
        DJY_DelayUs(100);
        *status = reg->ISR;
    }
    if(timeout == 0)
        return false;
    return true;
}

// =============================================================================
// ���ܣ���ѯ��ʽ��IIC���豸�շ�����
// ������reg,�Ĵ�����ַ
//       devaddr,�豸��ַ
//       memaddr,�豸�ڲ���ַ
//       maddrlen,�豸�ڲ���ַ����
//       buf,�洢����
//       len,���ݳ��ȣ��ֽ�
// ���أ�len,��ȡ�ɹ�;-1,��ȡʧ��
// =============================================================================
//(I2CType *reg,u8 devaddr,u8 *buf, u32 len ,bool_t Rdflag)
static u32 IIC_RxTxPoll(ptu32_t port,u8 devaddr,u8 *buf)
{
    I2CType* reg = config[port].iicBaseAddr;
    u32  status,cr2,num,mask,bytes_to_rw,len;
    bool_t Rdflag;
    u8 * bufbak = buf;
    cr2 = reg->CR2;
    num = 0;

    switch (pStm32Icb[port].IICstate)
    {
        case Tx_send_memoryaddr:
            len = pStm32Icb[port].Addrlen + pStm32Icb[port].len;
            buf = pStm32Icb[port].MemAddr;
            Rdflag = false;
            break;
        case Rx_send_memoryaddr:
            len = pStm32Icb[port].Addrlen;
            buf = pStm32Icb[port].MemAddr;
            Rdflag = false;
            break;
        case Rx_buf:
            len = pStm32Icb[port].len;
            Rdflag = true;
            break;
        default:     return 0;
    }
    mask = Rdflag ? TYPE1_I2C_ISR_RXNE : (TYPE1_I2C_ISR_TXIS | TYPE1_I2C_ISR_NACKF);

    reg->ICR |=(TYPE1_I2C_ICR_STOPCF|TYPE1_I2C_ICR_NACKCF);
    /* �����Ҫ���ı�־λ */
    mask |= TYPE1_I2C_ISR_ERRORS;
    //������λ
    cr2 &= ~(TYPE1_I2C_CR2_RD_WRN|TYPE1_I2C_CR2_SADD7_MASK| TYPE1_I2C_CR2_ADD10|\
            TYPE1_I2C_CR2_NBYTES_MASK|TYPE1_I2C_CR2_RELOAD|TYPE1_I2C_CR2_AUTOEND);
    if(Rdflag)
        cr2 |=TYPE1_I2C_CR2_RD_WRN;
    //�������ߵ�ַ
    cr2 |= TYPE1_I2C_CR2_SADD7(devaddr);
    if (len > TYPE1_I2C_MAX_LEN)
    {
        bytes_to_rw = TYPE1_I2C_MAX_LEN;
        cr2 |= TYPE1_I2C_CR2_NBYTES(TYPE1_I2C_MAX_LEN);
        cr2 |= TYPE1_I2C_CR2_RELOAD;
    }
    else
    {
        bytes_to_rw = len;
        cr2 |= TYPE1_I2C_CR2_NBYTES(len);
    }
    reg->CR2 = cr2;
    reg->CR1 |= TYPE1_I2C_CR1_PE;
    reg->CR2 |= TYPE1_I2C_CR2_START;

    while (len)
    {
        if(IIC_wait_flags(reg,mask,&status)==false)
            break;

        if (status & (TYPE1_I2C_ISR_NACKF | TYPE1_I2C_ISR_ERRORS))
            break;
        if (status & TYPE1_I2C_ISR_RXNE) //
        {
            *buf++ = reg->RXDR;
             len--;
             num++;
             bytes_to_rw--;
        }
        if (status & TYPE1_I2C_ISR_TXIS)
        {
            *((volatile u8 *)&reg->TXDR) = *buf++;
            len--;
            num++;
            bytes_to_rw--;
            if(pStm32Icb[port].Addrlen == num)
            {
                pStm32Icb[port].IICstate =Tx_buf;
                buf = bufbak;
            }
        }
        if (!bytes_to_rw && len)
        {
            /* Wait until TCR flag is set */
            if(IIC_wait_flags(reg,TYPE1_I2C_ISR_TCR,&status)==false)
                break;
            bytes_to_rw = len > TYPE1_I2C_MAX_LEN ?TYPE1_I2C_MAX_LEN :len;
            mask = Rdflag ? TYPE1_I2C_ISR_RXNE : TYPE1_I2C_ISR_TXIS | TYPE1_I2C_ISR_NACKF;

            reg->CR2 &= ~TYPE1_I2C_CR2_NBYTES_MASK;
            if (len > TYPE1_I2C_MAX_LEN) {
                reg->CR2 |= TYPE1_I2C_CR2_NBYTES(TYPE1_I2C_MAX_LEN);
            }
            else
            {
                reg->CR2 &= ~TYPE1_I2C_CR2_RELOAD;
                reg->CR2 |= TYPE1_I2C_CR2_NBYTES(len);
            }
        }
        else if (!bytes_to_rw)
        {
            /* Wait until TC flag is set */
            if(IIC_wait_flags(reg,TYPE1_I2C_ISR_TC,&status)==false)
                break;
        }
    }
    reg->CR2 |=TYPE1_I2C_CR2_STOP;/* End of transfer, send stop condition */
    return num;
}

// =============================================================================
// ���ܣ���ѯ��ʽ��дIIC�豸
// ������SpecificFlag,�Ĵ�����ַ
//       DevAddr,�豸��ַ
//       MemAddr,�豸�ڲ���ַ
//       MemAddrLen,�豸�ڲ���ַ����
//       Buf,�洢����
//       Length,���ݳ��ȣ��ֽ�
//       WrRdFlag,��д��ǣ�Ϊ0ʱд��1ʱΪ��
// ���أ�len,��ȡ�ɹ�;-1,��ȡʧ��
// =============================================================================
static bool_t IIC_WriteReadPoll(ptu32_t port,u8 DevAddr,u32 MemAddr,\
                                u8 MemAddrLen,u8* Buf, u32 Length,u8 WrRdFlag)
{
    u8 i;
    bool_t flag=false;
    I2CType* reg = config[port].iicBaseAddr;
    reg->CR1 &=~TYPE1_I2C_CR1_PE;
    for(i=0;i<MemAddrLen;i++)
        pStm32Icb[port].MemAddr[(MemAddrLen-1)-i]=((MemAddr>>(8*i))&0xff);

    pStm32Icb[port].Addrlen = MemAddrLen;
    pStm32Icb[port].len     = Length;

    //��ѯ��ʽ�ȹ��ж�
    reg->CR1 &= ~(TYPE1_I2C_CR1_ERRIE | TYPE1_I2C_CR1_TCIE |\
            TYPE1_I2C_CR1_STOPIE | TYPE1_I2C_CR1_NACKIE);
    if(!WrRdFlag)//д
    {
        pStm32Icb[port].IICstate = Tx_send_memoryaddr;
        if((MemAddrLen+Length) ==IIC_RxTxPoll(port,DevAddr,Buf))
            flag = true;
    }
    else  //��
    {
        pStm32Icb[port].IICstate = Rx_send_memoryaddr;
        if(MemAddrLen == IIC_RxTxPoll(port,DevAddr,Buf))
        {
            pStm32Icb[port].IICstate = Rx_buf;
            if(Length == IIC_RxTxPoll(port,DevAddr,Buf))
                flag = true;
        }
    }
    return flag;
}
// =============================================================================
// ����:  IIC�ж����ú��� �ײ�Ӳ����ʼ�� ������ ʱ�� ����״̬
// ����: port�˿ں�
// ����: ��
// =============================================================================
static void IIC_HardInit(ptu32_t port)
{
    I2CType* reg = config[port].iicBaseAddr;
#if defined IICCLKOLD
    RCC->DCKCFGR2 &=~(3<<((2*port)+16));//���ԭ��������
    RCC->APB1LENR |=(1<<(21+port));//����ʱ��ʹ��
    reg->TIMINGR = 0x40953c38;//ʱ��Ĵ�����д��
    reg->CR1     = TYPE1_I2C_CR1_PE;//PEʹ��
#else
    RCC->APB1LENR |=(1<<(21+port));//����ʱ��ʹ��
//    RCC->DCKCFGR2 &=~(3<<((2*port)+16));//���ԭ��������
//    RCC->DCKCFGR2 |=(0<<((2*port)+16));
    IIC_ClkSet(port,config[port].speed);
    reg->CR1     = TYPE1_I2C_CR1_PE;//PEʹ��
#endif
}

//
// =============================================================================
// ���ܣ�IIC DMA�����жϷ�����
// ������port  IIC��� CN_IIC1
//       Add_mode Ѱַģʽ
// ���أ���
// =============================================================================
static u32 iic_dmarx_isr(ptu32_t port)
{
    u32 num;
    num = IIC_PortWrite(pStm32Icb[port].pIIC_CB,pStm32Icb[port].Dmabuf,\
                                            pStm32Icb[port].DmaRcvlen);

    num = pStm32Icb[port].len>config[port].DmaBufLen ? \
                config[port].DmaBufLen:pStm32Icb[port].len;
    DMA_ClearIntFlag(config[port].DmaRxTag);

    if(num>0)
    {
        DMA_Enable(config[port].DmaRxTag,(u32)pStm32Icb[port].Dmabuf,num);
        pStm32Icb[port].DmaRcvlen = num;
        pStm32Icb[port].len-=num;
    }
    else
    {
        pStm32Icb[port].pDmaState = dma_idle;
        DMA_Disable(config[port].DmaRxTag);
    }
    return 0;
}

// =============================================================================
// ���ܣ�IIC DMA�����жϰ������͵�ַ��������
// ������iic_port  IIC��� CN_IIC1
//       Add_mode Ѱַģʽ
// ���أ���
// =============================================================================
static u32 iic_dmatx_isr(ptu32_t port)
{
    u32 num;
    DMA_ClearIntFlag(config[port].DmaTxTag);
    if(pStm32Icb[port].IICstate == Rx_send_memoryaddr)
    {
        num=0;
    }
    else
    {
        num = pStm32Icb[port].len>config[port].DmaBufLen ? \
                config[port].DmaBufLen:pStm32Icb[port].len;
        num = IIC_PortRead(pStm32Icb[port].pIIC_CB ,pStm32Icb[port].Dmabuf,num);
    }

    if(num > 0)
    {
        DMA_Enable(config[port].DmaTxTag,(u32)pStm32Icb[port].Dmabuf,num);
        pStm32Icb[port].len-=num;
    }
    else
    {
        pStm32Icb[port].pDmaState = dma_idle;
        DMA_Disable(config[port].DmaTxTag);
    }

    return 0;
}
// =============================================================================
// ���ܣ� ������ͻ���յ����ݳ���255�ֽڻ�ʹ��reloadģʽ
//     �����������Ҫ�ٴδ��������ֵ��������Ĵ�����
// ������ port  IIC��� CN_IIC1
// ���أ���
// =============================================================================
static void iic_reload(ptu32_t port)
{
    u32 cr2;
    I2CType*reg = (I2CType*) config[port].iicBaseAddr;

    cr2 = reg->CR2;
    cr2 &= ~TYPE1_I2C_CR2_NBYTES_MASK;
    if (pStm32Icb[port].len > TYPE1_I2C_MAX_LEN)
    {
        cr2 |= TYPE1_I2C_CR2_NBYTES(TYPE1_I2C_MAX_LEN);
    }
    else
    {
        cr2 &= ~TYPE1_I2C_CR2_RELOAD;
        cr2 |= TYPE1_I2C_CR2_NBYTES(pStm32Icb[port].len);
    }
    reg->CR2 = cr2;
}

// =============================================================================
// ���ܣ�I2C�жϷ������������¼��жϺͳ����ж�
// ������port  IIC��� CN_IIC1
// ���أ���
// =============================================================================
static u32 iic_event_error_isr(ptu32_t port)
{
    I2CType* reg = (I2CType*) config[port].iicBaseAddr;
    s32 num;
    u8 ch;

    if (reg->ISR & TYPE1_I2C_ISR_TC) /* ������� */
    {
        if((pStm32Icb[port].IICstate==Tx_buf)||\
           (pStm32Icb[port].IICstate==Rx_buf))//�������ڲ���ַ��������
        {
            reg->CR2 |= TYPE1_I2C_CR2_STOP;
            pStm32Icb[port].IICstate = idle_state;
            Lock_SempPost(pStm32Icb[port].pDrvPostSemp);//�������δ���
            if( pStm32Icb[port].pDmaState != DmaUnused)
                pStm32Icb[port].pDmaState =dma_idle;
        }
        else
        {
            if(pStm32Icb[port].IICstate == Rx_send_memoryaddr)
                pStm32Icb[port].IICstate = Rx_buf;
            if(pStm32Icb[port].pDmaState == DmaUnused)
                return IIC_RxTxInt_Init( port);
            else
                return IIC_RxTxDMA_Init(port);
        }
    }
    if (reg->ISR & TYPE1_I2C_ISR_TXIS) /* ���ͼĴ����� */
    {
        if((pStm32Icb[port].IICstate==Tx_send_memoryaddr)||\
           (pStm32Icb[port].IICstate==Rx_send_memoryaddr))//�������ڲ���ַ��������
        {
            ch = pStm32Icb[port].MemAddr[pStm32Icb[port].Addrlenbak-pStm32Icb[port].Addrlen];
            pStm32Icb[port].Addrlen--;
            *((volatile u8 *)&reg->TXDR) = ch;
            if(pStm32Icb[port].Addrlen==0 && pStm32Icb[port].IICstate==Tx_send_memoryaddr )
                pStm32Icb[port].IICstate=Tx_buf;
        }
        else
        {
            num = IIC_PortRead( pStm32Icb[port].pIIC_CB,&ch,1);
            if(num != 0)
            {
                pStm32Icb[port].len--;
                *((volatile u8 *)&reg->TXDR) = ch;
            }
            else
            {
                pStm32Icb[port].IICstate = idle_state;
                Lock_SempPost(pStm32Icb[port].pDrvPostSemp);
            }
        }
    }

    if (reg->ISR & TYPE1_I2C_ISR_RXNE)/* ���ռĴ����ǿ� */
    {
        ch = reg->RXDR;
        pStm32Icb[port].len--;
        num = IIC_PortWrite(pStm32Icb[port].pIIC_CB,&ch,1);
        if(num != 1)
            pStm32Icb[port].error = buf_over;//���������
    }

    if (reg->ISR & TYPE1_I2C_ISR_NACKF)/* ���յ���Ӧ���־ */
    {
        reg->ICR |=TYPE1_I2C_ICR_NACKCF;//���������������δ���
        Lock_SempPost(pStm32Icb[port].pDrvPostSemp);
    }

    if (reg->ISR & TYPE1_I2C_ISR_STOPF)/* ֹͣλ����־  */
    {
        pStm32Icb[port].IICstate = idle_state;
        /* Disable interrupts */
        reg->CR1 &=~TYPE1_I2C_ALL_IRQ_MASK;
        /* Clear STOP flag */
        reg->ICR |=TYPE1_I2C_ICR_STOPCF;//��ֹͣλ
    }

    /*
     * Transfer Complete Reload: 255 data bytes have been transferred
     * We have to prepare the I2C controller to transfer the remaining
     * data.
     */
    if (reg->ISR & TYPE1_I2C_ISR_TCR)/*������ɵȴ�����*/
        iic_reload(port);

    if (reg->ISR & TYPE1_I2C_ISR_BERR) /* ���ߴ��� */
    {
        printf( "<%s>: Bus error\n", __func__);
        IIC_ErrPop(pStm32Icb[port].pIIC_CB,CN_IIC_POP_UNKNOW_ERR);
        reg->ICR |= TYPE1_I2C_ICR_BERRCF;
        pStm32Icb[port].error = Bus_error;
        Lock_SempPost(pStm32Icb[port].pDrvPostSemp);//�������δ���
        reg->CR1 &=~TYPE1_I2C_CR1_PE;
    }

    if (reg->ISR & TYPE1_I2C_ISR_ARLO) /* �ٲö�ʧ*/
    {
        printf("<%s>: Arbitration loss\n", __func__);
        IIC_ErrPop(pStm32Icb[port].pIIC_CB,CN_IIC_POP_MAL_LOST_ERR);
        reg->ICR |= TYPE1_I2C_ICR_ARLOCF;
        pStm32Icb[port].error = ARlocf_error;
        Lock_SempPost(pStm32Icb[port].pDrvPostSemp);//�������δ���
        reg->CR1 &=~TYPE1_I2C_CR1_PE;
    }
    return 0;
}

// =============================================================================
// ����: IIC�ж����ú���
// ����: port�˿ں�
// ����: ��
// =============================================================================
static void IIC_Int_Init(ptu32_t port)
{
    u8 IntLine;
    //ע���¼��жϷ�����
    IntLine = config[port].iicEvIntLine;
    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,iic_event_error_isr);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_SetIsrPara(IntLine,port);
    Int_RestoreAsynLine(IntLine);

    IntLine = config[port].iicErIntLine;
    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,iic_event_error_isr);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_SetIsrPara(IntLine,port);
    Int_RestoreAsynLine(IntLine);
}

// =============================================================================
// ����: I/Oģ��ķ�ʽ �ͷ�û�и�λ��ʹ�����ŵ�IIC����
// ����: port I/O�� �� CN_GPIO_B
//      TS_SDA:���ź�
//      TS_SCK:���ź�
// ����: true/false
// =============================================================================
bool_t IIC_Busfree(u32 port,u32 sda_pin,u32 sck_pin)
{
     u32 timeout=0;
     GPIO_PowerOn(port);
     GPIO_CfgPinFunc(port,sda_pin,GPIO_MODE_IN, //TS_SDA
                     GPIO_OTYPE_OD,GPIO_SPEED_100M,GPIO_PUPD_PU);
     GPIO_CfgPinFunc(port,sck_pin,GPIO_MODE_OUT,  //TS_SCK
                     GPIO_OTYPE_OD,GPIO_SPEED_100M,GPIO_PUPD_PU);
     while(1)
     {
          timeout++;
          GPIO_SettoLow(port,sck_pin);
          DJY_DelayUs(10);

          GPIO_SettoHigh(port,sck_pin);
          DJY_DelayUs(10);

          if(timeout>=100)
              return false;
          if( GPIO_GetData(port)&(sda_pin))
              break;
    }
     GPIO_CfgPinFunc(port,sda_pin,GPIO_MODE_OUT,  //TS_SCK
                         GPIO_OTYPE_OD,GPIO_SPEED_100M,GPIO_PUPD_PU);
    //����ֹͣ�ź� iic�����ͷ�
    GPIO_SettoLow(port,sda_pin);
    DJY_DelayUs(10);
    GPIO_SettoHigh(port,sda_pin);
    DJY_DelayUs(10);

    return true;
}

// =============================================================================
// ���ܣ�IIC�ײ������ĳ�ʼ�����������IIC���ߵĳ�ʼ��������Ҫ�������£�
//       1.��ʼ�����߿��ƿ�IIC_CB���ص������ͻ������ĳ�ʼ����ֵ��
//       2.Ĭ�ϵ�Ӳ����ʼ������GPIO��IIC�Ĵ����ȣ�
//       3.�жϳ�ʼ������ɶ�д�ж����ã�
//       4.����IICBusAdd��IICBusAdd_r�������߽�㣻
// ������iic_port  IIC��� CN_IIC1
//       Add_mode Ѱַģʽ
// ���أ���
// =============================================================================
bool_t ModuleInstall_IIC(ptu32_t port)
{
    struct IIC_Param IIC_Config;
    u8 *buf;
    if(port >= CN_IIC_NUM)
        return false;

    buf = M_Malloc(config[port].iicbufLen,0);
    if(buf == NULL)
        return false;

    pStm32Icb[port].pDmaState  = DmaUnused;
    pStm32Icb[port].error      = noError;
    pStm32Icb[port].IICstate   = idle_state;
    pStm32Icb[port].Dmabuf     = NULL;
    pStm32Icb[port].IICstate   = idle_state;
    pStm32Icb[port].pDmaState  = DmaUnused;
    pStm32Icb[port].Dmabuf     = NULL;
    pStm32Icb[port].DmaRcvlen  = 0;;

    IIC_Config.BusName             = config[port].iicname;
    IIC_Config.IICBuf              = buf;
    IIC_Config.IICBufLen           = config[port].iicbufLen;
    IIC_Config.SpecificFlag        = (ptu32_t)port;
    IIC_Config.pGenerateWriteStart = IIC_WriteStartFunc;
    IIC_Config.pGenerateReadStart  = IIC_ReadStartFunc;
    IIC_Config.pGenerateEnd        = IIC_GenerateEndFunc;
    IIC_Config.pBusCtrl            = IIC_BusCtrlFunc;
    IIC_Config.pWriteReadPoll      = IIC_WriteReadPoll;

    IIC_HardInit(port);
    IIC_Int_Init(port);

    pStm32Icb[port].pIIC_CB = IIC_BusAdd(&IIC_Config);
    if(NULL ==  pStm32Icb[port].pIIC_CB )
         return false;
    return true;
}


