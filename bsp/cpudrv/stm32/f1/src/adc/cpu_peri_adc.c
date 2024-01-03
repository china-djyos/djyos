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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// =============================================================================

// 文件名     ：cpu_peri_adc.c
// 模块描述:
// 模块版本:
// 创建人员:
// 创建时间:
// =============================================================================
#include <djyos.h>
#include <stdint.h>
#include "cpu_peri.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_ADC(u8 port);
//
//    #if CFG_ADC1_ENABLE==1
//    ModuleInstall_ADC(CN_ADC1);
//    #endif
//
//    #if CFG_ADC2_ENABLE==1
//    ModuleInstall_ADC(CN_ADC2);
//    #endif
//
//    #if CFG_ADC3_ENABLE==1
//    ModuleInstall_ADC(CN_ADC3);
//    #endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip adc"//CPU的ADC外设驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_ADC == false )
//#warning  " cpu_onchip_adc  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_ADC    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
#define CFG_ADC1_ENABLE             false      //"是否配置使用ADC1",
#define CFG_ADC2_ENABLE             false      //"是否配置使用ADC2",
#define CFG_ADC3_ENABLE             false      //"是否配置使用ADC3",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

DMA_HandleTypeDef hdma_adc;

#define datamax 18

// #define CN_ADC1  0
// #define CN_ADC2  1
// #define CN_ADC3  2
// #define CN_ADC_NUM  3

// #define res_12  ADC_RESOLUTION_12B
// #define res_10  ADC_RESOLUTION_10B
// #define res_8   ADC_RESOLUTION_8B
// #define res_6   ADC_RESOLUTION_6B

// #define clk2 ADC_CLOCK_SYNC_PCLK_DIV2
// #define clk4 ADC_CLOCK_SYNC_PCLK_DIV4
// #define clk6 ADC_CLOCK_SYNC_PCLK_DIV6
// #define clk8 ADC_CLOCK_SYNC_PCLK_DIV8

// #define right ADC_DATAALIGN_RIGHT
// #define left  ADC_DATAALIGN_LEFT

#define tagADC_Reg ADC_TypeDef
static tagADC_Reg volatile * const tg_ADC_Reg[] =
                        {(tagADC_Reg *)ADC1_BASE,
                         (tagADC_Reg *)ADC2_BASE,
                         (tagADC_Reg *)ADC3_BASE};

static ADC_HandleTypeDef h_adc_1;
static ADC_HandleTypeDef h_adc_2;
static ADC_HandleTypeDef h_adc_3;
// /* ADC 及引脚 定义 */
// #define ADC_ADCX_CHY_GPIO_PORT GPIOA
// #define ADC_ADCX_CHY_GPIO_PIN GPIO_PIN_6
// #define ADC_ADCX_CHY_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOA_CLK_ENABLE();\
// }while(0) /* PA 口时钟使能 */
// #define ADC_ADCX ADC1
// #define ADC_ADCX_CHY ADC_CHANNEL_6 /* 通道 Y, 0 <= Y <= 16 */
// /* ADC1 时钟使能 */
// #define ADC_ADCX_CHY_CLK_ENABLE() do{ __HAL_RCC_ADC1_CLK_ENABLE();}while(0)

// =============================================================================
// 功能: ADC初始化
// 参数: ADC端口号
// 返回: true,成功;false,失败
// =============================================================================
bool_t ModuleInstall_ADC(u8 port)
{
    HAL_StatusTypeDef ret = HAL_OK;
    ADC_HandleTypeDef *hadc = NULL;
    RCC_PeriphCLKInitTypeDef adc_clk_init = {0};
    
    switch (port)
    {
        case CN_ADC1:
            __HAL_RCC_ADC1_CLK_ENABLE();
            hadc = &h_adc_1;
            break;
        
        case CN_ADC2:
            __HAL_RCC_ADC2_CLK_ENABLE();
            hadc = &h_adc_2;
            break;

        case CN_ADC3:
            __HAL_RCC_ADC3_CLK_ENABLE();
            hadc = &h_adc_3;
            break;
    
        default:
            return false;
            break;
    }
    

    hadc->Instance = tg_ADC_Reg[port]; /* 选择哪个 ADC */
    hadc->Init.DataAlign = ADC_DATAALIGN_RIGHT; /* 数据对齐方式：右对齐 */
    hadc->Init.ScanConvMode = ADC_SCAN_DISABLE; /* 非扫描模式 */
    hadc->Init.ContinuousConvMode = DISABLE; /* 关闭连续转换模式 */
    hadc->Init.NbrOfConversion = 1; /* 范围是 1~16， 这里用到 1 个规则通道 */
    hadc->Init.DiscontinuousConvMode = DISABLE; /* 禁止规则通道组间断模式 */
    /* 配置间断模式的规则通道个数，禁止规则通道组间断模式后，此参数忽略 */
    hadc->Init.NbrOfDiscConversion = 0;
    hadc->Init.ExternalTrigConv = ADC_SOFTWARE_START; /* 软件触发 */
    ret = HAL_ADC_Init(&hadc); /* 初始化 */

    if(ret==HAL_OK)
    {
        ret = HAL_ADCEx_Calibration_Start(&hadc); /* 校准 ADC */
        if(ret==HAL_OK)
        {
            /* 设置 ADC 时钟 */
            adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;/* ADC 外设时钟 */
            /* 分频系数为 6，所以 ADC 的时钟为 72M/6=12MHz */
            adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;
            HAL_RCCEx_PeriphCLKConfig(&adc_clk_init); /* 设置 ADC 时钟 */
            return true;
        }
    }
    return false;
}

// void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
// {
//     // if(hadc->Instance == ADC_ADCX)
//     // {
//         GPIO_InitTypeDef gpio_init_struct;
//         // RCC_PeriphCLKInitTypeDef adc_clk_init = {0};
//         ADC_ADCX_CHY_CLK_ENABLE(); /* 使能 ADCx 时钟 */
//         ADC_ADCX_CHY_GPIO_CLK_ENABLE(); /* 开启 GPIO 时钟 */
//         /* 设置 ADC 时钟 */
//         // adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;/* ADC 外设时钟 */
//         // /* 分频系数为 6，所以 ADC 的时钟为 72M/6=12MHz */
//         // adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;
//         // HAL_RCCEx_PeriphCLKConfig(&adc_clk_init); /* 设置 ADC 时钟 */
//         /* 设置 AD 采集通道对应 IO 引脚工作模式 */
//         gpio_init_struct.Pin = ADC_ADCX_CHY_GPIO_PIN; /* ADC 通道 IO 引脚 */
//         gpio_init_struct.Mode = GPIO_MODE_ANALOG; /* 模拟 */
//         HAL_GPIO_Init(ADC_ADCX_CHY_GPIO_PORT, &gpio_init_struct);
//     // }
// }

// // =============================================================================
// // 功能: ADC通道配置
// // 参数:
// // 返回:
// // =============================================================================
// bool_t ADC_ConfigChannel(ADC_HandleTypeDef* hadc, ADC_ChannelConfTypeDef* sConfig)
// {
//     HAL_StatusTypeDef ret = HAL_OK;
//           bool_t flag =true;
//     ret=HAL_ADC_ConfigChannel(hadc,sConfig);
//     if(ret==HAL_OK)
//         {
//             return flag;
//         }
//         else return false;
// }

// =============================================================================
// 功能: ADC规则通道序列配置
// 参数: channelx：通道号,ADC_CHANNEL_1~ADC_CHANNEL_16
//      rank：排序,1~16S
//      samplingtime：采样时间,ADC_SAMPLETIME_3CYCLES,ADC_SAMPLETIME_15CYCLES,....ADC_SAMPLETIME_408CYCLES
// 返回:
// =============================================================================
bool_t ADC_Channel_Config(u8 port, uint32_t channelx, uint32_t rank, uint32_t samplingtime)
{
    ADC_HandleTypeDef *hadc;
    ADC_ChannelConfTypeDef ADCChannelStructure;

    switch (port)
    {
        case CN_ADC1:
            hadc = &h_adc_1;
            break;
        
        case CN_ADC2:
            hadc = &h_adc_2;
            break;

        case CN_ADC3:
            hadc = &h_adc_3;
            break;
    
        default:
            return false;
            break;
    }

    ADCChannelStructure.Channel=channelx;
    ADCChannelStructure.Rank=rank;
    ADCChannelStructure.SamplingTime=samplingtime;
//    ADCChannelStructure.Offset=0;
    if(HAL_ADC_ConfigChannel(hadc, &ADCChannelStructure) == HAL_OK)
    {
        return true;
    }
    return false;
}
// // =============================================================================
// // 功能: ADC DMA传输初始化
// // 参数: ADCx:ADC1~ADC4
// // 返回: 无
// // =============================================================================
// bool_t ADC_DMA_Config(ADC_HandleTypeDef* hadc,DMA_HandleTypeDef* hdma_adc,u32 ADCx)
// {
//     HAL_StatusTypeDef ret = HAL_OK;
//         bool_t flag =true;

//     switch(ADCx)
//     {
//         case CN_ADC1:
//         RCC->AHBENR = RCC_AHBENR_DMA1EN;        //使能DMA1时钟,Channel 1
//         hdma_adc->Instance = DMA1_Channel1;
//         break;

//         case CN_ADC2:
//         RCC->AHBENR = RCC_AHBENR_DMA2EN;        //使能DMA2时钟,Channel 1
//         hdma_adc->Instance = DMA1_Channel1;
//         break;
//         case CN_ADC3:
//         RCC->AHBENR = RCC_AHBENR_DMA2EN;        //使能DMA2时钟,Channel 5
//         hdma_adc->Instance = DMA1_Channel5;
//         break;
//         case ADC_4:
//         RCC->AHBENR = RCC_AHBENR_DMA2EN;        //使能DMA2时钟,Channel 2
//         hdma_adc->Instance = DMA1_Channel2;
//         break;

//         default:return false;
//     }

//         hdma_adc->Init.Direction = DMA_PERIPH_TO_MEMORY;            //数据从外部传到内存，不能反
//         hdma_adc->Init.PeriphInc = DMA_PINC_DISABLE;                 //外部增量模式失能
//         hdma_adc->Init.MemInc = DMA_MINC_ENABLE;                     //内存增量模式使能
//         hdma_adc->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;    //外部数据对齐方式：字
//         hdma_adc->Init.MemDataAlignment = DMA_PDATAALIGN_WORD;       //内存数据宽度：字
//         hdma_adc->Init.Mode = DMA_NORMAL;                            //循环模式,dma不停读取
//         hdma_adc->Init.Priority = DMA_PRIORITY_HIGH;                 //dma选定的通道软件优先级

// //        do{
// //          (&ADC_Handle[ADCx])->DMA_Handle= &hdma_adc;
// //          hdma_adc.Parent =ADC_Handle[ADCx];
// //          } while(0);

// //        __HAL_LINKDMA(hadc,DMA_Handle,hdma_adc);

//         ret=HAL_DMA_Init(hdma_adc);
//         if(ret!=true)
//                  flag=false;
//             return flag;
// }

// =============================================================================
// 功能: ADC开始转换
// 参数:
// 返回:
// =============================================================================
bool_t ADC_Start(u8 port)
{
    ADC_HandleTypeDef *hadc = NULL;

    switch (port)
    {
        case CN_ADC1:
            hadc = &h_adc_1;
            break;
        
        case CN_ADC2:
            hadc = &h_adc_2;
            break;

        case CN_ADC3:
            hadc = &h_adc_3;
            break;
    
        default:
            return false;
            break;
    }

    if(HAL_ADC_Start(hadc) == HAL_OK)
    {
        return true;
    }
    return false;
}
// =============================================================================
// 功能: ADC停止转换
// 参数:
// 返回:
// =============================================================================
bool_t ADC_Stop(u8 port)
{
    ADC_HandleTypeDef *hadc = NULL;

    switch (port)
    {
        case CN_ADC1:
            hadc = &h_adc_1;
            break;
        
        case CN_ADC2:
            hadc = &h_adc_2;
            break;

        case CN_ADC3:
            hadc = &h_adc_3;
            break;
    
        default:
            return false;
            break;
    }

    if(HAL_ADC_Stop(hadc) == HAL_OK)
    {
        return true;
    }
    return false;
}

// =============================================================================
// 功能: ADC等待转换结束
// 参数:
// 返回:
// =============================================================================
bool_t ADC_PollForConversion(u8 port, uint32_t Timeout)
{
    ADC_HandleTypeDef* hadc = NULL;

    switch (port)
    {
        case CN_ADC1:
            hadc = &h_adc_1;
            break;
        
        case CN_ADC2:
            hadc = &h_adc_2;
            break;

        case CN_ADC3:
            hadc = &h_adc_3;
            break;
    
        default:
            return false;
            break;
    }

    if(HAL_ADC_PollForConversion(hadc, Timeout) == HAL_OK)
    {
        return true;
    }
    return false;

}
// =============================================================================
// 功能: DMA传输ADC开始转换,并获取转换结果
// 参数: hadc,句柄
//      pData[data]:内存地址,存放DMA传输采样值
//      Length:数据量
// 返回: Length
// =============================================================================
uint32_t ADC_DMA_Start(u8 port, uint32_t* pData[datamax], uint32_t Length)
{
    ADC_HandleTypeDef* hadc = NULL;

    switch (port)
    {
        case CN_ADC1:
            hadc = &h_adc_1;
            break;
        
        case CN_ADC2:
            hadc = &h_adc_2;
            break;

        case CN_ADC3:
            hadc = &h_adc_3;
            break;
    
        default:
            return false;
            break;
    }
    if(HAL_ADC_Start_DMA(hadc,pData[datamax],Length) == HAL_OK)
    {
        return Length;
    }
    return 0;
}
// =============================================================================
// 功能: DMA传输ADC停止转换
// 参数: ADCx:ADC1~ADC4
//      num:每路ADC采集通道数
//      res:分辨率
//      clk:时钟频率,clk2 clk4 clk6 clk8
// 返回: 无
// =============================================================================
bool_t ADC_DMA_Stop(u8 port)
{
    ADC_HandleTypeDef* hadc = NULL;

    switch (port)
    {
        case CN_ADC1:
            hadc = &h_adc_1;
            break;
        
        case CN_ADC2:
            hadc = &h_adc_2;
            break;

        case CN_ADC3:
            hadc = &h_adc_3;
            break;
    
        default:
            return false;
            break;
    }

    if(HAL_ADC_Stop_DMA(hadc) == HAL_OK)
    {
        return true;
    }
    return false;
}
// =============================================================================
// 功能: DMA模式获取ADC采样值
// 参数: ADCx:ADC1~ADC4
//      num:每路ADC采集通道数
//      res:分辨率
//      clk:时钟频率,clk2 clk4 clk6 clk8
// 返回: 无
// =============================================================================
uint32_t Get_ADC_Value(u8 port)
{
    uint32_t data;
    ADC_HandleTypeDef* hadc = NULL;

    switch (port)
    {
        case CN_ADC1:
            hadc = &h_adc_1;
            break;
        
        case CN_ADC2:
            hadc = &h_adc_2;
            break;

        case CN_ADC3:
            hadc = &h_adc_3;
            break;
    
        default:
            return false;
            break;
    }

    data = HAL_ADC_GetValue(hadc);

    return data;
}
// // =============================================================================
// // 功能: 多重DMA模式配置
// // 参数:
// // 返回: 无
// // =============================================================================
// bool_t MultiModeConfig(ADC_HandleTypeDef* hadc, ADC_MultiModeTypeDef* multimode)
// {
//     HAL_StatusTypeDef ret = HAL_OK;
//                    bool_t flag =true;
//     ret=HAL_ADCEx_MultiModeConfigChannel(hadc,multimode);
//     if(ret==HAL_OK)
//     {
//         return flag;
//     }
//     else return false;
// }
// // =============================================================================
// // 功能: 多重ADC开始转换
// // 参数: hadc,句柄
// //      pData[data]:内存地址,存放DMA传输采样值                               //改，还是直接获取采样值
// //      Length:数据量
// // 返回:
// // =============================================================================
// bool_t ADC_MultiDMA_Start(ADC_HandleTypeDef* hadc, uint32_t* pData[datamax], uint32_t Length)
// {
//     HAL_StatusTypeDef ret = HAL_OK;
//          bool_t flag =true;
//     ret =HAL_ADCEx_MultiModeStart_DMA(hadc,pData[datamax],Length);
//     if(ret==HAL_OK)
//     {
//         return flag;
//     }
//     else return false;
// }
// // =============================================================================
// // 功能: DMA传输ADC停止转换
// // 参数:
// // 返回:
// // =============================================================================
// bool_t ADC_MultiDMA_Stop(ADC_HandleTypeDef* hadc)
// {
//     HAL_StatusTypeDef ret = HAL_OK;
//                    bool_t flag =true;
//     ret=HAL_ADCEx_MultiModeStop_DMA(hadc);
//     if(ret==HAL_OK)
//     {
//         return flag;
//     }
//     else return false;
// }
// // =============================================================================
// // 功能: 获取多重ADC转换值
// // 参数: hadc,句柄
// //      pData[data]:内存地址,存放DMA传输采样值
// //      Length:数据量
// // 返回:
// // =============================================================================
// uint32_t ADC_MultiDMA_GetValue(ADC_HandleTypeDef* hadc)
// {
//    return HAL_ADCEx_MultiModeGetValue(hadc);  //直接返回了CDR的值
// }

