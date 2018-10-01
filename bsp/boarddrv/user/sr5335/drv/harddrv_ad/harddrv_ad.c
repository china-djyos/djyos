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
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��ad.c
// ģ������: SR5335-v1.0 AD����
// ģ��汾: V1.00
// ������Ա: zhb
// ����ʱ��: 06/09.2017
// =============================================================================
#if 0

#include "string.h"
#include "cpu_peri.h"
#include "ads8688.h"
#include "stdint.h"

extern const Pin ADCS[];
extern const Pin ADRST[];
#define CN_AD_CH_NUM          9
#define CN_CP_SAM_RATE        10

static uint16_t AD_Buf[CN_AD_CH_NUM];
static uint16_t gs_cp1_buf[CN_CP_SAM_RATE];
static uint16_t gs_cp2_buf[CN_CP_SAM_RATE];

#define CN_SPI_TIMEOUT  (200*1000)

// =============================================================================
// ���ܣ���SPI���ռĴ�������ѯ��ʽ,��__SPI_TxRxPoll����
// ������Reg,�Ĵ�����ַ
// ���أ�����������
// =============================================================================
s32 SPI_Read(void)
{
    uint32_t time = CN_SPI_TIMEOUT;
    Spi *Reg;
    Reg = (Spi *)SPI2_BASE;
    while ( ( (Reg->SR & SPI_SR_RXNE) == 0 ) && (time > 0))
    {
        time --;
        Djy_DelayUs(1);
    }

    if(time == 0)
        return -1;
    return (s32)(Reg->DR & 0xFFFF);
}

// =============================================================================
// ���ܣ�дSPI���ͼĴ�������ѯ��ʽ,��__SPI_TxRxPoll����
// ������Reg,�Ĵ�����ַ��dwNpcs,Ƭѡ��wData��д����
// ���أ���
// =============================================================================
s32 SPI_Write(uint32_t dwNpcs, uint16_t wData )
{
    uint32_t time = CN_SPI_TIMEOUT;
    Spi *Reg;
    Reg = (Spi *)SPI2_BASE;
     /* Send data */
     while ( ( (Reg->SR & SPI_SR_TXE) == 0 ) && (time > 0) )
     {
            time --;
            Djy_DelayUs(1);
     }
     if(time == 0)
        return -1;
        *((volatile uint16_t *)&Reg->DR) = wData;

     return 0;
}

// =============================================================================
// ���ܣ���ʼ��ADS8664�������������úͳ�ʼ�����ã��ӿڵĳ�ʼ����
// ��������
// ���أ���
// =============================================================================
bool_t AD_HardInit(uint8_t spi_port)
{
    uint8_t i;
    bool_t ret=true;
    ADS8688_SpiInit(spi_port);
    for(i=0;i<CN_AD_CH_NUM;i++)
        AD_Buf[i]=0;
    for(i=0;i<CN_CP_SAM_RATE;i++)
    {
        gs_cp1_buf[i]=0;
        gs_cp2_buf[i]=0;
    }
    PIO_Set(ADRST);
    PIO_Set(ADCS);
    ADS8688_EnterRstMode();
    //AD1~AD6����ͨ��������ΧΪ+-2.5*4.096V  LSB:5mv
    //����ͨ��AD7/AD8���뷶Χ������Ϊ0~2.5Vref    LSB:2.5mv
    ret=ADS8688_SetChInRange(Channel_6_Input_Range,0x5);
    if(ret==false)
        goto EXIT;
    ret=ADS8688_SetChInRange(Channel_7_Input_Range,0x5);
    if(ret==false)
        goto EXIT;
    printf("AD8688 Init success.\r\n");
    return ret;
    EXIT:
    printf("ADS8688 Init failed.\r\n");
    return false;
}

// =============================================================================
// ���ܣ���һ���������ҵ����ֵ.
// �������ޡ�
// ���أ�
// =============================================================================
static uint16_t __GetMax(uint16_t *buf,uint8_t num)
{
    uint8_t i,cnt;
    uint16_t max=0;

    for(i=0;i<num;i++)
    {
        if(*(buf+i)>max)
        {
            max=*(buf+i);
            cnt=i;
        }
    }
    buf[cnt]=0;
    return max;
}
// =============================================================================
// ���ܣ�����CPͨ���ɼ��ķ�ֵ
// �������ޡ�
// ���أ�
// =============================================================================
static u16 __GetPwmPeak(u8 index)
{
    u8 i;
    u16 ver=0;
    u16 tmp[3];
    u16 *buf;
    if(index==0)
        buf=gs_cp1_buf;
    else
        buf=gs_cp2_buf;
    for(i=0;i<3;i++)
    {
        tmp[i]=__GetMax(buf,CN_CP_SAM_RATE);
    }
    ver=(tmp[0]+tmp[1]+tmp[2])/3;
    return ver;
}

// =============================================================================
// ���ܣ���ȡADS8688�Ĳɼ�ֵ������������ת����ת���������ٶ�ȡADת����ֵ
// ��������
// ���أ���
// =============================================================================
bool_t ADS8688_GetValue(uint16_t *pAinBuf)
{
    uint8_t i;
    bool_t ret=true;
    uint16_t dat;
    if(pAinBuf == NULL)
    {
        printf("%s para invalid.\r\n",__FUNCTION__);
        return false;
    }
    //����ΪAUTO_RSTģʽ�²�����1��2��3
    ret=ADS8688_GetRstModeData(0x3F,AD_Buf,6);
    if(ret==false)
        return false;
    dat=ADS8688_GetManModeData(MAN_Ch_6);
    gs_cp1_buf[CN_CP_SAM_RATE-1]=dat;
    dat=__GetPwmPeak(0);
    AD_Buf[6]=dat;

    dat=ADS8688_GetManModeData(MAN_Ch_7);
    gs_cp2_buf[CN_CP_SAM_RATE-1]=dat;
    dat=__GetPwmPeak(1);
    AD_Buf[7]=dat;
    //�ɼ�����ͨ��AUX
    //����ͨ��������Χ0~Vref (0~4.096)  LSB=1mV.
    AD_Buf[8]=ADS8688_GetManModeData(MAN_AUX);
    for(i = 0; i < CN_AD_CH_NUM; i++)
    {
        *(pAinBuf+i)=AD_Buf[i];
    }
    return true;
}

// =============================================================================
// ���ܣ���ȡADS8664�Ĳɼ�ֵ������������ת����ת���������ٶ�ȡADת����ֵ
// ��������
// ���أ���
// =============================================================================
bool_t AD_GetCPChannel(uint8_t num)
{
    uint16_t dat;
    //ֻ����CPͨ��
    dat=ADS8688_GetManModeData(MAN_Ch_6);
    gs_cp1_buf[num]=dat;

    dat=ADS8688_GetManModeData(MAN_Ch_7);
    gs_cp2_buf[num]=dat;

    return true;
}


bool_t AD_GetALLChannel(uint16_t *pAinBuf)
{
     ADS8688_GetValue(pAinBuf);
     return true;
}
#endif

