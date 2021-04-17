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

// �ļ���     ��cpu_peri_adc.c
// ģ������:
// ģ��汾:
// ������Ա:
// ����ʱ��:
// =============================================================================
#include <djyos.h>
#include <stdint.h>
#include "cpu_peri.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ADC_Init(ADC_HandleTypeDef* hadc);
//
//    #if CFG_ADC1_ENABLE==1
//    ADC_Init(ADC1);

//    #endif
//
//    #if CFG_ADC2_ENABLE==1
//    ADC_Init(ADC2);
//    #endif
//
//    #if CFG_ADC3_ENABLE==1
//    ADC_Init(ADC3);
//    #endif
//
//    #if CFG_ADC4_ENABLE==1
//    ADC_Init(ADC4);
//    #endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip adc"//CPU��ADC��������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_ADC == false )
//#warning  " cpu_onchip_adc  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_ADC    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
#define CFG_ADC1_ENABLE             false      //"�Ƿ�����ʹ��ADC1",
#define CFG_ADC2_ENABLE             false      //"�Ƿ�����ʹ��ADC2",
#define CFG_ADC3_ENABLE             false      //"�Ƿ�����ʹ��ADC3",
#define CFG_ADC4_ENABLE             false      //"�Ƿ�����ʹ��ADC4",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

DMA_HandleTypeDef hdma_adc;

#define datamax 18

#define ADC_1  1
#define ADC_2  2
#define ADC_3  3
#define ADC_4  4

#define res_12  ADC_RESOLUTION_12B
#define res_10  ADC_RESOLUTION_10B
#define res_8   ADC_RESOLUTION_8B
#define res_6   ADC_RESOLUTION_6B

#define clk2 ADC_CLOCK_SYNC_PCLK_DIV2
#define clk4 ADC_CLOCK_SYNC_PCLK_DIV4
#define clk6 ADC_CLOCK_SYNC_PCLK_DIV6
#define clk8 ADC_CLOCK_SYNC_PCLK_DIV8

#define right ADC_DATAALIGN_RIGHT
#define left  ADC_DATAALIGN_LEFT

// =============================================================================
// ����: ADC��ʼ��
// ����: hadc,ADC���
// ����: true,�ɹ�;false,ʧ��
// =============================================================================
bool_t ADC_Init(ADC_HandleTypeDef* hadc)
{
    HAL_StatusTypeDef ret = HAL_OK;
       bool_t flag =true;

    ret = HAL_ADC_Init(hadc);
    if(ret==HAL_OK)
    {
        return flag;
    }
    else return false;
}
// =============================================================================
// ����: ADCͨ������
// ����:
// ����:
// =============================================================================
bool_t ADC_ConfigChannel(ADC_HandleTypeDef* hadc, ADC_ChannelConfTypeDef* sConfig)
{
    HAL_StatusTypeDef ret = HAL_OK;
          bool_t flag =true;
    ret=HAL_ADC_ConfigChannel(hadc,sConfig);
    if(ret==HAL_OK)
        {
            return flag;
        }
        else return false;
}

// =============================================================================
// ����: ADC����ͨ����������
// ����: channelx��ͨ����,ADC_CHANNEL_1~ADC_CHANNEL_16
//      rank������,1~16S
//      samplingtime������ʱ��,ADC_SAMPLETIME_3CYCLES,ADC_SAMPLETIME_15CYCLES,....ADC_SAMPLETIME_408CYCLES
// ����:
// =============================================================================
bool_t ADC_Channel_Config(ADC_HandleTypeDef* hadc,uint32_t channelx,uint32_t rank,uint32_t samplingtime)
{
    HAL_StatusTypeDef ret = HAL_OK;
              bool_t flag =true;
    ADC_ChannelConfTypeDef ADCChannelStructure;

    ADCChannelStructure.Channel=channelx;
    ADCChannelStructure.Rank=rank;
    ADCChannelStructure.SamplingTime=samplingtime;
    ADCChannelStructure.Offset=0;
    ret=ADC_ConfigChannel(hadc, &ADCChannelStructure);
    if(ret==HAL_OK)
        {
            return flag;
        }
        else return false;
}
// =============================================================================
// ����: ADC DMA�����ʼ��
// ����: ADCx:ADC1~ADC4
// ����: ��
// =============================================================================
bool_t ADC_DMA_Config(ADC_HandleTypeDef* hadc,DMA_HandleTypeDef* hdma_adc,u32 ADCx)
{
    HAL_StatusTypeDef ret = HAL_OK;
        bool_t flag =true;

    switch(ADCx)
    {
        case ADC_1:
        RCC->AHBENR = RCC_AHBENR_DMA1EN;        //ʹ��DMA1ʱ��,Channel 1
        hdma_adc->Instance = DMA1_Channel1;
        break;

        case ADC_2:
        RCC->AHBENR = RCC_AHBENR_DMA2EN;        //ʹ��DMA2ʱ��,Channel 1
        hdma_adc->Instance = DMA1_Channel1;
        break;
        case ADC_3:
        RCC->AHBENR = RCC_AHBENR_DMA2EN;        //ʹ��DMA2ʱ��,Channel 5
        hdma_adc->Instance = DMA1_Channel5;
        break;
        case ADC_4:
        RCC->AHBENR = RCC_AHBENR_DMA2EN;        //ʹ��DMA2ʱ��,Channel 2
        hdma_adc->Instance = DMA1_Channel2;
        break;

        default:return false;
    }

        hdma_adc->Init.Direction = DMA_PERIPH_TO_MEMORY;            //���ݴ��ⲿ�����ڴ棬���ܷ�
        hdma_adc->Init.PeriphInc = DMA_PINC_DISABLE;                 //�ⲿ����ģʽʧ��
        hdma_adc->Init.MemInc = DMA_MINC_ENABLE;                     //�ڴ�����ģʽʹ��
        hdma_adc->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;    //�ⲿ���ݶ��뷽ʽ����
        hdma_adc->Init.MemDataAlignment = DMA_PDATAALIGN_WORD;       //�ڴ����ݿ�ȣ���
        hdma_adc->Init.Mode = DMA_NORMAL;                            //ѭ��ģʽ,dma��ͣ��ȡ
        hdma_adc->Init.Priority = DMA_PRIORITY_HIGH;                 //dmaѡ����ͨ��������ȼ�

//        do{
//          (&ADC_Handle[ADCx])->DMA_Handle= &hdma_adc;
//          hdma_adc.Parent =ADC_Handle[ADCx];
//          } while(0);

//        __HAL_LINKDMA(hadc,DMA_Handle,hdma_adc);

        ret=HAL_DMA_Init(hdma_adc);
        if(ret!=true)
                 flag=false;
            return flag;
}

// =============================================================================
// ����: ADC��ʼת��
// ����:
// ����:
// =============================================================================
bool_t ADC_Start(ADC_HandleTypeDef* hadc)
{
    HAL_StatusTypeDef ret = HAL_OK;
       bool_t flag =true;

    ret=HAL_ADC_Start(hadc);
    if(ret==HAL_OK)
    {
        return flag;
    }
    else return false;
}
// =============================================================================
// ����: ADCֹͣת��
// ����:
// ����:
// =============================================================================
bool_t ADC_Stop(ADC_HandleTypeDef* hadc)
{
    HAL_StatusTypeDef ret = HAL_OK;
           bool_t flag =true;

        ret=HAL_ADC_Stop(hadc);
        if(ret==HAL_OK)
        {
            return flag;
        }
        else return false;
}

// =============================================================================
// ����: ADC�ȴ�ת������
// ����:
// ����:
// =============================================================================
bool_t ADC_PollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout)
{
    HAL_StatusTypeDef ret = HAL_OK;
           bool_t flag =true;

        ret=HAL_ADC_PollForConversion(hadc,Timeout);
        if(ret==HAL_OK)
        {
            return flag;
        }
        else return false;

}
// =============================================================================
// ����: DMA����ADC��ʼת��,����ȡת�����
// ����: hadc,���
//      pData[data]:�ڴ��ַ,���DMA�������ֵ
//      Length:������
// ����: Length
// =============================================================================
uint32_t ADC_DMA_Start(ADC_HandleTypeDef* hadc, uint32_t* pData[datamax], uint32_t Length)
{
    HAL_StatusTypeDef ret = HAL_OK;
    ret =HAL_ADC_Start_DMA(hadc,pData[datamax],Length);
    if(ret==HAL_OK)
    {
        return Length;
    }
    else return 0;
}
// =============================================================================
// ����: DMA����ADCֹͣת��
// ����: ADCx:ADC1~ADC4
//      num:ÿ·ADC�ɼ�ͨ����
//      res:�ֱ���
//      clk:ʱ��Ƶ��,clk2 clk4 clk6 clk8
// ����: ��
// =============================================================================
bool_t ADC_DMA_Stop(ADC_HandleTypeDef* hadc)
{
    HAL_StatusTypeDef ret = HAL_OK;
                   bool_t flag =true;
    ret=HAL_ADC_Stop_DMA(hadc);
    if(ret==HAL_OK)
    {
        return flag;
    }
    else return false;
}
// =============================================================================
// ����: DMAģʽ��ȡADC����ֵ
// ����: ADCx:ADC1~ADC4
//      num:ÿ·ADC�ɼ�ͨ����
//      res:�ֱ���
//      clk:ʱ��Ƶ��,clk2 clk4 clk6 clk8
// ����: ��
// =============================================================================
uint32_t Get_ADC_Value(ADC_HandleTypeDef* hadc)
{
    uint32_t data;
    data=HAL_ADC_GetValue(hadc);
    return data;
}
// =============================================================================
// ����: ����DMAģʽ����
// ����:
// ����: ��
// =============================================================================
bool_t MultiModeConfig(ADC_HandleTypeDef* hadc, ADC_MultiModeTypeDef* multimode)
{
    HAL_StatusTypeDef ret = HAL_OK;
                   bool_t flag =true;
    ret=HAL_ADCEx_MultiModeConfigChannel(hadc,multimode);
    if(ret==HAL_OK)
    {
        return flag;
    }
    else return false;
}
// =============================================================================
// ����: ����ADC��ʼת��
// ����: hadc,���
//      pData[data]:�ڴ��ַ,���DMA�������ֵ                               //�ģ�����ֱ�ӻ�ȡ����ֵ
//      Length:������
// ����:
// =============================================================================
bool_t ADC_MultiDMA_Start(ADC_HandleTypeDef* hadc, uint32_t* pData[datamax], uint32_t Length)
{
    HAL_StatusTypeDef ret = HAL_OK;
         bool_t flag =true;
    ret =HAL_ADCEx_MultiModeStart_DMA(hadc,pData[datamax],Length);
    if(ret==HAL_OK)
    {
        return flag;
    }
    else return false;
}
// =============================================================================
// ����: DMA����ADCֹͣת��
// ����:
// ����:
// =============================================================================
bool_t ADC_MultiDMA_Stop(ADC_HandleTypeDef* hadc)
{
    HAL_StatusTypeDef ret = HAL_OK;
                   bool_t flag =true;
    ret=HAL_ADCEx_MultiModeStop_DMA(hadc);
    if(ret==HAL_OK)
    {
        return flag;
    }
    else return false;
}
// =============================================================================
// ����: ��ȡ����ADCת��ֵ
// ����: hadc,���
//      pData[data]:�ڴ��ַ,���DMA�������ֵ
//      Length:������
// ����:
// =============================================================================
uint32_t ADC_MultiDMA_GetValue(ADC_HandleTypeDef* hadc)
{
   return HAL_ADCEx_MultiModeGetValue(hadc);  //ֱ�ӷ�����CDR��ֵ
}

