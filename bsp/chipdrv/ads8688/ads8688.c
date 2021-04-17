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
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
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

// �ļ���     ��ADS8668.c
// ģ������: ADS8688�ɼ�ģ��ĵײ�������������SPI�ӿ�ͨ�ź�����ȡAD�ɼ�ֵ
// ģ��汾: V1.00
// ������Ա: zhb
// ����ʱ��: 05/10.2017
// =============================================================================
#include <harddrv_ad.h>     //


//#define SPI_BASE         SPI2_BASE
//s32 SPI_Read(void);
//s32 SPI_Write(uint32_t dwNpcs, uint16_t wData );
//bool_t AD_HardInit(uint8_t spi_port);
//bool_t AD_GetCPChannel(uint8_t num);
//bool_t AD_GetALLChannel(uint16_t *pAinBuf);

#include "stdint.h"
#include "stddef.h"
#include "endian.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "shell.h"
#include "ads8688.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern void ADS8688_SpiInit(uint8_t port);
//    ADS8688_SpiInit(0);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"ADC chip ads 8688"//spi�ӿڵ�16bitADC
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"sr5333_board"    //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_ADC_CHIP_ADS_8688 == false )
//#warning  " ADC_chip_ads_8688  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_ADC_CHIP_ADS_8688    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,0,1,2,3
#define CFG_ADS8688_CS        0//"Ƭѡ",Ƭѡ��
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure



extern const Pin ADCS[];
// =============================================================================
//#define CFG_ADS8688_CS           (0)
#define WRITE                 1
#define READ                  0

extern s32 SPI_Read(void);
extern s32 SPI_Write(uint32_t dwNpcs, uint16_t wData);

// =============================================================================
// ���ܣ�ADS8688��Ӧ��SPI��������ʼ��
// ������port, SPI��������ţ���sr5333-v2.0��ADS8688���õ���SPI2;
// ���أ���.
// =============================================================================
void ADS8688_SpiInit(uint8_t port)
{
    //�Ѿ���board.c�����ʼ����SPI2������
     Spi *Reg;
     Reg = (Spi *)SPI_BASE;
     //����SPIʹ��GPIO����
     RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; // ʹ��SPI2
     RCC->APB1RSTR |= RCC_APB1RSTR_SPI2RST; // ��λSPI2
     RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI2RST; // ֹͣ��λSPI2

     Reg->CR1 |=SPI_CR1_BR_0;//����������  108M/4=27MHz
     Reg->CR1 &= ~SPI_CR1_CPOL;                      // ����ģʽ��SCKΪ1 CPOL=0
     Reg->CR1 |= SPI_CR1_CPHA;                      // ���ݲ����ӵ�2��ʱ����ؿ�ʼ,CPHA=1
     Reg->CR1 &= ~SPI_CR1_RXONLY;//ȫ˫��ͨѶģʽ
     Reg->CR1 &= ~SPI_CR1_LSBFIRST;//����֡��ʽ
     Reg->CR1 &= ~ SPI_CR1_SSM;         // ��ֹ���nss����
     Reg->CR1 |= SPI_CR1_SSI;  //SSM
     Reg->CR1 |= SPI_CR1_MSTR;       // SPI����

     Reg->CR2 |=SPI_CR2_DS_0|SPI_CR2_DS_1|SPI_CR2_DS_2|SPI_CR2_DS_3 ;//����֡��ʽΪ16bit
     Reg->CR2 |=SPI_CR2_SSOE;//SS ���ʹ��
     Reg->CR2 |=SPI_CR2_NSSP;//���� NSS ����
     Reg->CR2 &=~SPI_CR2_FRXTH;//16�ֽ�fifo
     Reg->I2SCFGR &= (uint16_t)(~SPI_I2SCFGR_I2SMOD);// ѡ��SPIģʽ
     Reg->CR1 |= SPI_CR1_SPE;
}

// =============================================================================
// ���ܣ�ADS8688��Ӧ��SPI��������ʼ��
// ������addr, Program Register��ַ;
//       data������дָ����Ч��Ϊд���мĴ�����ֵ.
// ���أ�
// =============================================================================
static bool_t __ADS8688_SpiTrans(uint16_t *wBuf, uint32_t wlen, uint16_t *rBuf,uint32_t rlen)
{
    uint32_t i;
    s32 ret;
    bool_t result = false;
    Spi *Reg;
    Reg = (Spi *)SPI_BASE;
    PIO_Clear(ADCS);
    for(i = 0; i < wlen; i++)
    {
        ret=SPI_Write(CFG_ADS8688_CS,wBuf[i]);
        if(ret==-1)
            goto SPI_EXIT;
        ret=SPI_Read();
        if(ret==-1)
            goto SPI_EXIT;
    }
    for(i = 0; i < rlen; i ++)
    {
        ret=SPI_Write(CFG_ADS8688_CS,0x0000);
        if(ret==-1)
            goto SPI_EXIT;
         ret = SPI_Read();
         if(ret==-1)
            goto SPI_EXIT;

         rBuf[i] = (uint16_t)(ret & 0xFFFF);
    }
    result = true;
SPI_EXIT:
    PIO_Set(ADCS);
    return result;
}

// =============================================================================
void ADS8688_WriteCmdReg(uint16_t command)//дADS8688����Ĵ���
{
    uint16_t tmp=0;
    __ADS8688_SpiTrans(&command,1,&tmp,1);
}

void ADS8688_EnterAutoRstMode(void)    //�����Զ�ɨ��ģʽ
{
    ADS8688_WriteCmdReg(AUTO_RST);
}

void ADS8688_EnterRstMode(void)
{
    ADS8688_WriteCmdReg(RST);
}

// =============================================================================
// ���ܣ�Program Registerд����
// ������addr, Program Register��ַ;
//       data������дָ����Ч��Ϊд���мĴ�����ֵ.
// ���أ�
// =============================================================================
static bool_t ADS8688_WRProgReg(uint8_t addr,uint8_t data)
{
    uint16_t w_dat=0x0000;
    uint16_t r_dat=0x0000;
    bool_t ret=true;
    uint8_t temp=0x00;
    temp=addr<<1;
    temp=temp|WRITE;
    w_dat=temp;
    w_dat=w_dat<<8;
    w_dat=w_dat|data;
    ret=__ADS8688_SpiTrans(&w_dat,1,&r_dat,1);
    if(ret==false)
        return ret;
    r_dat=r_dat>>8;
    if(r_dat!=data)
        return false;
    return true;
}
// =============================================================================
// ���ܣ�����ͨ�����뷶Χ
// ������seq
// ���أ�
// =============================================================================
bool_t ADS8688_SetChInRange(uint8_t ch,uint8_t range)
{
    bool_t result;
    result=ADS8688_WRProgReg(ch,range);
    return result;
}


// =============================================================================
// ���ܣ���ȡɨ��ͨ�����е�ADת������
// ������seq
// ���أ�
// =============================================================================
bool_t ADS8688_GetRstModeData(uint8_t seq,uint16_t *pdata,uint8_t chnum)
{
    uint8_t i;
    uint16_t bData = 0x0000;
    bool_t ret=true;
    uint16_t temp=0;
    ret=ADS8688_WRProgReg(AUTO_SEQ_EN,seq);
    if(!ret)
        return false;
    ADS8688_EnterRstMode();
    for (i = 0; i < chnum; i ++)
    {
        ret=__ADS8688_SpiTrans(&bData,1,pdata,1);
        if(ret==false)
            return ret;
        temp=*pdata;
        *pdata=temp;
        pdata++;
    }
    return true;
}

// =============================================================================
// ���ܣ���ȡ�ֶ�ͨ����ADת������
// �������ޡ�
// ���أ�
// =============================================================================
uint16_t ADS8688_GetManModeData(uint16_t ch)
{
    uint16_t dat=0;
    bool_t ret=true;
    uint16_t bData = 0x0000;
    //����ָ��ͨ��Ϊ�ֶ�ģʽ
    ADS8688_WriteCmdReg(ch);
    ret=__ADS8688_SpiTrans(&bData,1,&dat,1);
    if(ret==false)
        return 0;
    return dat;
}


