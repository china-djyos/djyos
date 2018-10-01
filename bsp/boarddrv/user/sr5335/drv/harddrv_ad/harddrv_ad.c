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
// 这份授权条款，在使用者符合以下二条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、此三条件表列，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、此三条件表列，以及下述
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
// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：ad.c
// 模块描述: SR5335-v1.0 AD驱动
// 模块版本: V1.00
// 创建人员: zhb
// 创建时间: 06/09.2017
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
// 功能：读SPI接收寄存器，轮询方式,被__SPI_TxRxPoll调用
// 参数：Reg,寄存器基址
// 返回：读到的数据
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
// 功能：写SPI发送寄存器，轮询方式,被__SPI_TxRxPoll调用
// 参数：Reg,寄存器基址，dwNpcs,片选；wData，写数据
// 返回：无
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
// 功能：初始化ADS8664，包括引脚配置和初始化配置，接口的初始化等
// 参数：无
// 返回：无
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
    //AD1~AD6三个通道测量范围为+-2.5*4.096V  LSB:5mv
    //配置通道AD7/AD8输入范围，设置为0~2.5Vref    LSB:2.5mv
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
// 功能：从一个数组中找到最大值.
// 参数：无。
// 返回：
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
// 功能：计算CP通道采集的峰值
// 参数：无。
// 返回：
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
// 功能：获取ADS8688的采集值，不包括启动转换，转换结束后再读取AD转换的值
// 参数：无
// 返回：无
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
    //首先为AUTO_RST模式下采样第1、2、3
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
    //采集辅助通道AUX
    //辅助通道测量范围0~Vref (0~4.096)  LSB=1mV.
    AD_Buf[8]=ADS8688_GetManModeData(MAN_AUX);
    for(i = 0; i < CN_AD_CH_NUM; i++)
    {
        *(pAinBuf+i)=AD_Buf[i];
    }
    return true;
}

// =============================================================================
// 功能：获取ADS8664的采集值，不包括启动转换，转换结束后再读取AD转换的值
// 参数：无
// 返回：无
// =============================================================================
bool_t AD_GetCPChannel(uint8_t num)
{
    uint16_t dat;
    //只采样CP通道
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

