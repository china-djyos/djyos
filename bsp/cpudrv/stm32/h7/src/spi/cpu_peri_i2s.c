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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// =============================================================================

// 文件名     ：cpu_peri_i2s.c
// 模块描述: I2S模块底层硬件驱动模块
// 模块版本:
// 创建人员:
// 创建时间:
// =============================================================================
#include "string.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "int.h"
#include "math.h"
#include "dbug.h"
// =============================================================================
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。
//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip i2s"//i2s总线驱动
//parent:"cpu onchip spi"   //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_I2S == false )
//#warning  " cpu_onchip_spi  组件参数未配置，使用默认配置"
//%$#@target = header   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_I2S    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,1048576,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束
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
// 功能: DMA使能
// 参数: I2S的端口号
// 返回: 无
// =============================================================================
void I2sDMA_Enable(u8 port)
{
    __HAL_DMA_ENABLE(&I2sDmaHandle[port]); 
}

// =============================================================================
// 功能: DMA失能
// 参数: I2S的端口号
// 返回: 无
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
//功能: 安装I2S端口
//参数: Port：I2S端口号，I2sMode：i2s的工作模式，I2sStandard：标准，
//      I2sClockPolarity：时钟极性，I2sDataFormat：数据格式，I2sFreq：采样率
//返回: false -- 失败; true -- 成功;
//备注:
//-----------------------------------------------------------------------------
bool_t I2sInit(u8 Port, u32 I2sMode, u32 I2sStandard, u32 I2sClockPolarity, u32 I2sDataFormat, u32 I2sFreq)
{
    bool_t result = false;

    if (I2sState[Port] == false)
    {
        return false;
    }

    //默认初始化工作模式：
    // 主机发送，飞利浦协议，16位拓展，主时钟失能，44k采样率，空闲状态时钟低电平，MSB
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
        I2sDmaHandle[Port].Instance = I2sDmaTxStream[Port];                             // 设置I2S TX DMA数据流  
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
//功能: 安装I2S端口
//参数: I2S端口号
//返回: -1 -- 失败; 0 -- 成功;
//备注:
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
        __HAL_LINKDMA(&hI2s[Port], hdmatx, I2sDmaHandle[Port]);     //将DMA与I2s联系起来
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

