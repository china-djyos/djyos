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

// 文件名     ：cpu_peri_spi.h
// 模块描述: SPI模块底层硬件驱动头文件
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 16/10.2015
// =============================================================================

#ifndef CPU_PERI_I2S_H_
#define CPU_PERI_I2S_H_

#include <stm32h7xx_hal_i2s.h>

#ifdef __cplusplus
extern "C" {
#endif

#define tagI2sReg SPI_TypeDef

#define CN_I2S1     0
#define CN_I2S2     1
#define CN_I2S3     2
#define CN_I2S6     3
#define CN_I2S_NUM  4

// I2s的模式
#define CFG_I2S_MODE_SLAVE_TX               I2S_MODE_SLAVE_TX            
#define CFG_I2S_MODE_SLAVE_RX               I2S_MODE_SLAVE_RX         
#define CFG_I2S_MODE_MASTER_TX              I2S_MODE_MASTER_TX        
#define CFG_I2S_MODE_MASTER_RX              I2S_MODE_MASTER_RX        
#define CFG_I2S_MODE_SLAVE_FULLDUPLEX       I2S_MODE_SLAVE_FULLDUPLEX 
#define CFG_I2S_MODE_MASTER_FULLDUPLEX      I2S_MODE_MASTER_FULLDUPLEX

// i2s标准
#define CFG_I2S_STANDARD_PHILIPS            I2S_STANDARD_PHILIPS  
#define CFG_I2S_STANDARD_MSB                I2S_STANDARD_MSB      
#define CFG_I2S_STANDARD_LSB                I2S_STANDARD_LSB      
#define CFG_I2S_STANDARD_PCM_SHORT          I2S_STANDARD_PCM_SHORT
#define CFG_I2S_STANDARD_PCM_LONG           I2S_STANDARD_PCM_LONG 

// i2s数据格式
#define CFG_I2S_DATAFORMAT_16B              I2S_DATAFORMAT_16B         
#define CFG_I2S_DATAFORMAT_16B_EXTENDED     I2S_DATAFORMAT_16B_EXTENDED
#define CFG_I2S_DATAFORMAT_24B              I2S_DATAFORMAT_24B         
#define CFG_I2S_DATAFORMAT_32B              I2S_DATAFORMAT_32B         

// i2s的采样率
#define CFG_I2S_AUDIOFREQ_192K              I2S_AUDIOFREQ_192K   
#define CFG_I2S_AUDIOFREQ_96K               I2S_AUDIOFREQ_96K    
#define CFG_I2S_AUDIOFREQ_48K               I2S_AUDIOFREQ_48K    
#define CFG_I2S_AUDIOFREQ_44K               I2S_AUDIOFREQ_44K    
#define CFG_I2S_AUDIOFREQ_32K               I2S_AUDIOFREQ_32K    
#define CFG_I2S_AUDIOFREQ_22K               I2S_AUDIOFREQ_22K    
#define CFG_I2S_AUDIOFREQ_16K               I2S_AUDIOFREQ_16K    
#define CFG_I2S_AUDIOFREQ_11K               I2S_AUDIOFREQ_11K    
#define CFG_I2S_AUDIOFREQ_8K                I2S_AUDIOFREQ_8K     
#define CFG_I2S_AUDIOFREQ_DEFAULT           I2S_AUDIOFREQ_DEFAULT

// i2s的时钟极性
#define CFG_I2S_CPOL_LOW                    I2S_CPOL_LOW 
#define CFG_I2S_CPOL_HIGH                   I2S_CPOL_HIGH


void I2sDMA_Enable(u8 port);
void I2sDMA_Disable(u8 port);
u8 I2sWaitSendComplete(u8 port, u32 timeout);
bool_t I2sInit(u8 Port, u32 I2sMode, u32 I2sStandard, u32 I2sClockPolarity, u32 I2sDataFormat, u32 I2sFreq);
bool_t ModuleInstall_I2S(u8 Port);
#ifdef __cplusplus
}
#endif

#endif




