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
// 模块描述: 定时器硬件接口层数据定义以及接口声明
// 模块版本: V1.00
// 创建人员: zhangqf_cyg
// 创建时间: 2:39:26 PM/Apr 1, 2014
// =============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// =============================================================================
// 备注

#ifndef TIMER_HARD_H_
#define TIMER_HARD_H_

#include "stdint.h"
#include "stddef.h"

//硬件定时器状态和控制位，也是EN_TIMER_GETSTATE命令返回值
#define CN_TIMER_FREE        ((u32)(1<<0))      //定时器空闲，可分配
#define CN_TIMER_ENCOUNT     ((u32)(1<<1))      //允许计数
#define CN_TIMER_ENINT       ((u32)(1<<2))      //允许产生异常
#define CN_TIMER_REALINT     ((u32)(1<<3))      //实时信号
#define CN_TIMER_RELOAD      ((u32)(1<<4))      //是否reload
#define CN_TIMER_ENUSE       ((u32)(1<<10))     //正在使用，用于硬件定时器

//TIMER的操作码
enum HardTimerCmdCode
{
    EN_TIMER_STARTCOUNT,    //使能计数，para无意义
    EN_TIMER_PAUSECOUNT,    //停止计数，para无意义
    EN_TIMER_SETCYCLE,      //设置周期，para为u32,待设置的周期（计数值）
    EN_TIMER_SETRELOAD,     //reload模式or not！，para为bool_t,true代表reload
    EN_TIMER_ENINT,         //中断使能，para无意义
    EN_TIMER_DISINT,        //中断禁止，para无意义
    EN_TIMER_SETINTPRO,     //中断属性设置，para为bool_t,true代表实时信号
    EN_TIMER_GETTIME,       //获取上一次溢出到现在的计数值，inoutpara为u32 *
    EN_TIMER_GETCYCLE,      //获取定时周期，para为u32 *,单位为周期数
    EN_TIMER_GETID,         //获取定时器ID，para为u32 *，高16位为intID，低16为timerID
    EN_TIMER_GETSTATE,      //获取定时器状态，para为u32 *
    EN_TIMER_GETREG,        //获取定时器硬件寄存器地址，para为void *
    EN_TIMER_LASTOPCODE,
};

typedef u32 (*fnTimerIsr)(ptu32_t TimerHandle);

//当注册定时器时，需要使用下述函数原型
// =============================================================================
// 函数功能：fnHardTimerAlloc
//           定时器分配
// 输入参数：
//          cycle, 指定分配定时器的定时周期，该属性可以使用API函数进行更改设定（单位为微秒）
//          timerisr,分配的定时器的中断服务函数,中断中调用
// 输出参数：无
// 返回值  ：NULL 分配不成功，否则返回定时器句柄，该句柄的结构由定时器芯片自己定义
// 说明    :（一般而言刚分配的定时器，其计时停止，中断禁止）依赖芯片驱动实现
//           刚开始分配的定时器应该是各种属性都关闭的，因此属性必须自己重新设置
//           默认：停止计数，异步中断，reload,中断禁止
// =============================================================================
typedef ptu32_t (*fnHardTimerAlloc)(fnTimerIsr timerisr);
// =============================================================================
// 函数功能：fnHardTimerFree
//           定时器释放
// 输入参数：timerhandle，待释放定时器
// 输出参数：无
// 返回值  ：true 成功 false失败
// =============================================================================
typedef bool_t  (*fnHardTimerFree)(ptu32_t timerhandle);
// =============================================================================
// 函数功能：检查定时器计数频率
// 输入参数：timerhandle，被检查的定时器
// 输出参数：无
// 返回值  ：频率，Hz为单位
// =============================================================================
typedef u32  (*fnHardTimerGetFreq)(ptu32_t timerhandle);
// =============================================================================
// 函数功能：fnHardTimerCtrl
//           操作定时器
// 输入参数：timerhard，待设置定时器，
//           ctrlcmd, 操作命令
// 输出参数：inoutpara，输入输出参数，根据不同的情况而定
// 返回值  ：true 操作成功 false操作失败
// 说明：ctrlcmd对应的inoutpara的属性定义说明参看enum HeadTimerCmdCode定义
// =============================================================================
typedef bool_t  (*fnHardTimerCtrl)(ptu32_t timerhandle,\
                                   enum HardTimerCmdCode ctrlcmd, \
                                   ptu32_t inoutpara);

struct TimerChip
{
    char               *chipname;        //chip名字，必须为静态定义
    fnHardTimerAlloc  HardTimerAlloc;   //分配定时器
    fnHardTimerFree   HardTimerFree;    //释放定时器
    fnHardTimerGetFreq HardTimerGetFreq; //获取定时器计数频率
    fnHardTimerCtrl   HardTimerCtrl;    //控制定时器
};

bool_t  HardTimer_RegisterChip(struct TimerChip *timerchip);
bool_t  HardTimer_UnRegisterChip(void);
ptu32_t HardTimer_Alloc(fnTimerIsr timerhardisr);
bool_t  HardTimer_Free(ptu32_t timerhandle);
u32  HardTimer_GetFreq(ptu32_t timerhandle);
bool_t  HardTimer_Ctrl(ptu32_t timerhandle, \
                       enum HardTimerCmdCode ctrlcmd, \
                       ptu32_t inoutpara);

#endif /* TIMER_HARD_H_ */
