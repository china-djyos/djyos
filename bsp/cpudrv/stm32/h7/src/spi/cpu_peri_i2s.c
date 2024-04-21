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

// �ļ���     ��cpu_peri_i2s.c
// ģ������: I2Sģ��ײ�Ӳ������ģ��
// ģ��汾:
// ������Ա:
// ����ʱ��:
// =============================================================================
#include "string.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "int.h"
#include "math.h"
#include "dbug.h"
// =============================================================================
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip i2s"//i2s��������
//parent:"cpu onchip spi"   //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_I2S == false )
//#warning  " cpu_onchip_spi  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_I2S    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,1048576,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������
//@#$%component end configure

// =============================================================================

static tagI2sReg volatile * const tg_I2sReg[] =
                        {(tagI2sReg *)SPI1_BASE,
                         (tagI2sReg *)SPI2_BASE,
                         (tagI2sReg *)SPI3_BASE,
                         (tagI2sReg *)SPI6_BASE};
static I2S_HandleTypeDef hI2s[CN_I2S_NUM];
static DMA_HandleTypeDef I2sDmaHandle[CN_I2S_NUM];
static DMA_Stream_TypeDef * const I2sDmaTxStream[CN_I2S_NUM] =
                                 {DMA1_Stream0,DMA1_Stream1,
                                  DMA1_Stream2,DMA1_Stream3};
static u8 const I2sDmaTxInt[CN_I2S_NUM] = {CN_INT_LINE_DMA1_Stream0,CN_INT_LINE_DMA1_Stream1,
                                  CN_INT_LINE_DMA1_Stream2,CN_INT_LINE_DMA1_Stream3};
static u8 I2sDmaTxReq[CN_I2S_NUM] = {DMA_REQUEST_SPI1_TX, DMA_REQUEST_SPI2_TX, 
                                    DMA_REQUEST_SPI3_TX, BDMA_REQUEST_SPI6_TX};
static struct SemaphoreLCB I2sSemaphore[CN_I2S_NUM];
static const char *I2sSemaphoreName[CN_I2S_NUM] = {"I2S1", "I2S2", "I2S3", "I2S6"};
static bool_t I2sState[CN_I2S_NUM] = {false, false, false, false};

#define I2S_DMA_BUFFER_SIZE 65536
static u8 DMA_TxBuf[I2S_DMA_BUFFER_SIZE];
// =============================================================================
// ����: DMAʹ��
// ����: I2S�Ķ˿ں�
// ����: ��
// =============================================================================
void I2sDMA_Enable(u8 port)
{
    __HAL_DMA_ENABLE(&I2sDmaHandle[port]); 
}

// =============================================================================
// ����: DMAʧ��
// ����: I2S�Ķ˿ں�
// ����: ��
// =============================================================================
void I2sDMA_Disable(u8 port)
{
    __HAL_DMA_DISABLE(&I2sDmaHandle[port]); 
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    u8 I2sPort = 0;
    for(I2sPort = 0; I2sPort < CN_I2S_NUM; I2sPort++)
    {
        if(hi2s == &hI2s[I2sPort])
            break;
    }

    Lock_SempPost(&I2sSemaphore[I2sPort]);
}

u32 I2S_TX_DMASx_Handle(ptu32_t port)
{
    HAL_DMA_IRQHandler(&I2sDmaHandle[port]);
    return 0;
} 

u8 I2sSend(u8 port, u8 *data, u32 len, u32 timeout)
{
    u8 ret = 0;
    if (I2sState[port] == false)
    {
        return 0;
    }

    if (Lock_SempPend(&I2sSemaphore[port], timeout))
    {
        memset(DMA_TxBuf, 0, sizeof(DMA_TxBuf));
        memcpy(DMA_TxBuf, data, len);
        ret = 1;
    }
    return ret;
}

//-----------------------------------------------------------------------------
//����: ��װI2S�˿�
//����: Port��I2S�˿ںţ�I2sMode��i2s�Ĺ���ģʽ��I2sStandard����׼��
//      I2sClockPolarity��ʱ�Ӽ��ԣ�I2sDataFormat�����ݸ�ʽ��I2sFreq��������
//����: false -- ʧ��; true -- �ɹ�;
//��ע:
//-----------------------------------------------------------------------------
bool_t I2sInit(u8 Port, u32 I2sMode, u32 I2sStandard, u32 I2sClockPolarity, u32 I2sDataFormat, u32 I2sFreq)
{
    bool_t result = false;

    if (I2sState[Port] == false)
    {
        return false;
    }

    //Ĭ�ϳ�ʼ������ģʽ��
    // �������ͣ�������Э�飬16λ��չ����ʱ��ʧ�ܣ�44k�����ʣ�����״̬ʱ�ӵ͵�ƽ��MSB
    __HAL_I2S_DISABLE(&hI2s[Port]);
    hI2s[Port].Instance = tg_I2sReg[Port];
    hI2s[Port].Init.Mode = I2sMode;
    hI2s[Port].Init.Standard = I2sStandard;
    hI2s[Port].Init.DataFormat = I2sDataFormat;
    hI2s[Port].Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
    hI2s[Port].Init.AudioFreq = I2sFreq;
    hI2s[Port].Init.CPOL = I2sClockPolarity;
    hI2s[Port].Init.FirstBit = I2S_FIRSTBIT_MSB;
    hI2s[Port].Init.WSInversion = I2S_WS_INVERSION_DISABLE;
    hI2s[Port].Init.Data24BitAlignment = I2S_DATA_24BIT_ALIGNMENT_RIGHT;
    hI2s[Port].Init.MasterKeepIOState = I2S_MASTER_KEEP_IO_STATE_DISABLE;
    if (HAL_I2S_Init(&hI2s[Port]) != HAL_OK)
    {
        error_printf("I2s", "i2s port(%d) install fail.\r\n", Port);
    }
    else
    {
        __HAL_I2S_ENABLE(&hI2s[Port]);
        I2sDmaHandle[Port].Instance = I2sDmaTxStream[Port];                             // ����I2S TX DMA������  
        I2sDmaHandle[Port].Init.Request = I2sDmaTxReq[Port];
        I2sDmaHandle[Port].Init.Direction = DMA_MEMORY_TO_PERIPH;
        I2sDmaHandle[Port].Init.PeriphInc = DMA_PINC_DISABLE;
        I2sDmaHandle[Port].Init.MemInc = DMA_MINC_ENABLE;
        I2sDmaHandle[Port].Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        I2sDmaHandle[Port].Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        I2sDmaHandle[Port].Init.Mode = DMA_CIRCULAR;
        I2sDmaHandle[Port].Init.Priority = DMA_PRIORITY_HIGH;
        I2sDmaHandle[Port].Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        I2sDmaHandle[Port].Init.MemBurst = DMA_MBURST_SINGLE;
        I2sDmaHandle[Port].Init.PeriphBurst = DMA_PBURST_SINGLE;
        if (HAL_DMA_DeInit(&I2sDmaHandle[Port]) == HAL_OK)
        {
            if (HAL_DMA_Init(&I2sDmaHandle[Port]) == HAL_OK)
            {
                memset(DMA_TxBuf, 0, I2S_DMA_BUFFER_SIZE);
                HAL_I2S_Transmit_DMA(&hI2s[Port], (uint16_t *)DMA_TxBuf, I2S_DMA_BUFFER_SIZE / 2);
                result = true;
            }
            else
            {
                error_printf("I2s", "i2s port(%d) dma init fail.\r\n", Port);
            }
        }
        else
        {
            error_printf("I2s", "i2s port(%d) dma deinit fail.\r\n", Port);
        }

    }


    return result;
}


//-----------------------------------------------------------------------------
//����: ��װI2S�˿�
//����: I2S�˿ں�
//����: -1 -- ʧ��; 0 -- �ɹ�;
//��ע:
//-----------------------------------------------------------------------------
bool_t ModuleInstall_I2S(u8 Port)
{
    bool_t result = false;

    if ((CN_SPI4 == Port) || (CN_SPI5 == Port))
    {
        return result;
    }
    if (CN_SPI6 == Port)
    {
        Port = CN_I2S6;
    }
    switch (Port)
    {
    case CN_I2S1:
        __HAL_RCC_SPI1_CLK_ENABLE();
        break;
    case CN_I2S2:
        __HAL_RCC_SPI2_CLK_ENABLE();
        break;
    case CN_I2S3:
        __HAL_RCC_SPI3_CLK_ENABLE();
        break;
    case CN_I2S6:
        __HAL_RCC_SPI6_CLK_ENABLE();
        break;
    default:
        error_printf("I2s", "port error.\r\n");
        return false;
        break;
    }
    if (Lock_SempCreate_s(&I2sSemaphore[Port], 1, 1, CN_BLOCK_PRIO, I2sSemaphoreName[Port]))
    {
        I2sState[Port] = true;
        __HAL_RCC_DMA1_CLK_ENABLE();
        __HAL_LINKDMA(&hI2s[Port], hdmatx, I2sDmaHandle[Port]);     //��DMA��I2s��ϵ����
        Int_Register(I2sDmaTxInt[Port]);
        Int_SetClearType(I2sDmaTxInt[Port],CN_INT_CLEAR_AUTO);
        Int_IsrConnect(I2sDmaTxInt[Port], I2S_TX_DMASx_Handle);
        Int_SetIsrPara(I2sDmaTxInt[Port], Port);
        Int_SettoAsynSignal(I2sDmaTxInt[Port]);
        Int_ClearLine(I2sDmaTxInt[Port]);
        Int_RestoreAsynLine(I2sDmaTxInt[Port]);
    }
    return result;
}

