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
//所属模块: BSP模块
//作者:  Liuwei
//版本：V1.0.0
//文件描述: 板件特性配置文件。
//        CPU型号:S3C2416

//其他说明:
//修订历史:
//2. ...
//1. 日期: 2015-05-11
//   作者:  Liuwei
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cpu-optional.h"

#define DJY_BOARD        "TQ2416"

/*____与硬件相关的配置____*/
#define Mhz 1000000
#define CN_CFG_MCLK (532*Mhz)  //主频，内核要用，必须定义
#define CN_CFG_HCLK (CN_CFG_MCLK/4)  //高速外设时钟
#define CN_CFG_PCLK (CN_CFG_MCLK/8)  //低速外设时钟
#define CN_CFG_TIMER_CLK CN_CFG_PCLK  //定时器时钟源

/*____以下定义tick参数____*/
#define CN_CFG_TICK_US 1000  //tick间隔，以us为单位。
#define CN_CFG_TICK_HZ 1000  //内核时钟频率，单位为hz。
#define CN_USE_TICKLESS_MODE    (0U)
#if (!CN_USE_TICKLESS_MODE)
#define CN_CFG_FINE_US 0x000007B  //1/532,tick输入时钟周期，以uS为单位，32位定点数整数、小数各占16位，这也限制了ticks最长不超过65535uS
#define CN_CFG_FINE_HZ CN_CFG_MCLK  //tick输入时钟频率，是CN_CFG_FINE_US的倒数
#else
#define CN_CFG_USE_USERTIMER        (0U)//是否使用LPTIMER作为系统时钟
#define CN_CFG_TIME_BASE_HZ         CN_CFG_MCLK//(32000U)//(8000U)
#if (!CN_CFG_USE_USERTIMER)
#define CN_CFG_USE_BYPASSTIMER      (0U)//0表示不使用旁路定时器，1表示使用旁路定时器
#define CN_CFG_TIME_PRECISION       (500U)/*精度单位：US*/
#else
#define CN_CFG_USERTIMER_PRESC      (1U)//若不使用SYSTICK定时器，需指定用户定时器的分频数
#define CN_CFG_FINE_US (0x1F4000U)  //1/32000,tick输入时钟周期，以uS为单位，32位定点数整数、小数各占16位，这也限制了ticks最长不超过65535uS
#define CN_CFG_FINE_HZ (0x831U)  //tick输入时钟频率，是CN_CFG_FINE_US的倒数
#define CN_CFG_TIME_PRECISION       (200U)/*精度单位：US*/
#endif
#endif

//CPU架构相关配置,可选才在这里配置,例如大小端,是可选的,在这里配置,
//而CPU字长固定,故字长在BSP的arch_feature.h文件中定义
//存储器大小端的配置
#define CN_CFG_LITTLE_ENDIAN        0
#define CN_CFG_BIG_ENDIAN           1
#define CN_CFG_BYTE_ORDER          CN_CFG_LITTLE_ENDIAN

//LCD参数配置
#define CN_LCD_XSIZE        480     //LCD物理像素宽度.
#define CN_LCD_YSIZE        272     //LCD物理像素高度.
#define CN_LCD_PIXEL_FORMAT CN_SYS_PF_RGB565    //LCD颜色格式.

#define  CN_LCD_DRV_FRAME_BUFFER_EN  //定义该宏则使用FrameBuffer,否则直接屏幕输出.
#undef  CN_LCD_DRV_REMOTE_EN        //定义该宏则LCD驱动使用远程显示功能

#ifdef __cplusplus
}
#endif
#endif // __BOARD_CONFIG_H__



