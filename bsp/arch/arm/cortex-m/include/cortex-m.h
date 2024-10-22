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
//所属模块: bsp
//作者：罗侍田
//版本：V1.0.0
//文件描述: cortex-m的公共定义
//其他说明:
//修订历史:
//2. ...
//1. 日期  : 2014/12/19
//   修改人: 罗侍田
//   新版本号:
//   修改说明: 新建
//------------------------------------------------------

#ifndef __CORTEX_M_H__
#define __CORTEX_M_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

//------------------------ SystemTick ----------------------------------------
//CTRL 中断控制及状态寄存器,0xE000_E010,bm_为位掩码,bo_为位偏移
#define bm_systick_ctrl_countflag   (1<<16) //rc,计数值变为0时自动置1?读清零
#define bm_systick_ctrl_clksource   (1<<2)  //rw,时钟选择?0=外部时钟stclk?1=内核fclk
#define bm_systick_ctrl_tickint     (1<<1)  //rw,1=SysTick 数到0 时产生SysTick 异常请求
#define bm_systick_ctrl_enable      (1<<0)  //rw,SysTick 定时器的使能位
#define bo_systick_ctrl_countflag   16   //rc,计数值变为0时自动置1?读清零
#define bo_systick_ctrl_clksource   2    //rw,时钟选择?0=外部时钟stclk?1=内核fclk
#define bo_systick_ctrl_tickint     1    //rw,1=SysTick 数到0 时产生SysTick 异常请求
#define bo_systick_ctrl_enable      0    //rw,SysTick 定时器的使能位

//CTRL SysTick校准数值寄存器 0xE000_E01c,bm_为位掩码,bo_为位偏移
#define bm_systick_calib_NOREF  (1<<31)  //r,1=STCLK 不可用,0=可用
#define bm_systick_calib_SKEW   (1<<30)  //r,1=校准值不是准确的10ms?0=是
#define bo_systick_calib_NOREF  31  //r,1=STCLK 不可用,0=可用
#define bo_systick_calib_SKEW   30  //r,1=校准值不是准确的10ms?0=是
//rw,10ms 的时间内倒计数的格数。芯片设计者应该通过Cortex‐M3 的输入信号提供该数值。
//若该值读回零?则表示无法使用校准功能
#define bm_systick_calib_TENMS  (0xffffff<<0)
#define bo_systick_calib_TENMS  0

struct SystickReg      //0xe000e010
{
    vu32 ctrl;
    vu32 reload;    //当倒数至零时?将被重装载的值
    vu32 current;   //读取时返回当前倒计数的值?写则清零?并清CTRL中的COUNTFLAG 标志
    vuc32 verify;   //SysTick校准数值寄存器
};



#ifdef __cplusplus
}
#endif

#endif /*__CORTEX_M_H__*/

