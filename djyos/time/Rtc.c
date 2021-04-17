
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
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块:日历模块
//作者:  zqf
//版本：V1.0.0
//文件描述:为系统维护日历时间;提供RTC设备注册接口
//其他说明:
//修订历史:
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "int.h"
#include "systime.h"
#include <time.h>
#include "component_config_time.h"
//手动设置或从RTC中读取的时间，距离1970/1/1的微秒数，初始值=2000/1/1/0时
static s64 sgRtcTimeSet = 946684800000000;
static s64               sgRtcUpdateTime2SysTime;  //读取或者设置RTC时间时系统的运行时刻
static fnRtc_GetTime    fnRtcGetTime = NULL;      //获取RTC设备RTC时间
static fnRtc_SetTime    fnRtcSetTime = NULL;      //设置RTC设备RTC时间
// =============================================================================
//功能：Rtc_RegisterDev
//        注册RTC设备
//参数：fnGetTime，获取RTC设备时间的函数
//        fnSetTime,设置RTC设备时间的函数
// 返回值  ：fnTimerIsr
// =============================================================================
bool_t Rtc_RegisterDev(fnRtc_GetTime fnGetTime,
                       fnRtc_SetTime fnSetTime)
{
    bool_t result = false;

    fnRtcGetTime = fnGetTime;

    if( (NULL != fnSetTime))
    {
        fnRtcSetTime = fnSetTime;
        result = true;
    }

    return result;
}

#define CN_RTC_UNIT_SECOND        1000000
// =============================================================================
//功能：获取距离1970年的秒数
//参数：
//输出参数：rtctime,存储获取的RTC时间，秒数
//返回值  ：获取的RTC时间，秒数
// =============================================================================
s64 __Rtc_Time(s64 *rtctime)
{
    s64 result;
    s64 systime;
    atom_low_t atom;

    systime = DJY_GetSysTime();

    if((sgRtcUpdateTime2SysTime/CN_RTC_UNIT_SECOND) ==(systime/CN_RTC_UNIT_SECOND))
    {
        //which means that we get the RTC time at the same second
        //we could return the same time as last time
#if (64 > CN_CPU_BITS)
        atom = Int_LowAtomStart();
        result = sgRtcTimeSet;
        Int_LowAtomEnd(atom);
#else
        result = sgRtcTimeSet;
#endif
    }
    else
    {
        //we'd better to get the RTC time now
        if(NULL == fnRtcGetTime)
        {
            //no rtc dev yet,we could cal the rtc time use the sysrunning time
#if (64 > CN_CPU_BITS)
            atom = Int_LowAtomStart();
            result = sgRtcTimeSet + systime - sgRtcUpdateTime2SysTime;
            sgRtcUpdateTime2SysTime = systime;
            sgRtcTimeSet = result;
            Int_LowAtomEnd(atom);
#else
            result = sgRtcTimeSet + systime - sgRtcUpdateTime2SysTime;
            sgRtcUpdateTime2SysTime = systime;
            sgRtcTimeSet = result;
#endif
        }
        else
        {
            if(false == fnRtcGetTime(&result))
            {
                //failed to get the rtc dev time
#if (64 > CN_CPU_BITS)
                atom = Int_LowAtomStart();
                result = sgRtcTimeSet + systime - sgRtcUpdateTime2SysTime;
                sgRtcUpdateTime2SysTime = systime;
                sgRtcTimeSet = result;
                Int_LowAtomEnd(atom);
#else
                result = sgRtcTimeSet + systime - sgRtcUpdateTime2SysTime;
                sgRtcUpdateTime2SysTime = systime;
                sgRtcTimeSet = result;
#endif
            }
            else
            {
#if (64 > CN_CPU_BITS)
                atom = Int_LowAtomStart();
                sgRtcUpdateTime2SysTime = systime;
                sgRtcTimeSet = result;
                Int_LowAtomEnd(atom);
#else
                sgRtcUpdateTime2SysTime = systime;
                sgRtcTimeSet = result;
#endif
            }
        }
    }

    result = result/CN_RTC_UNIT_SECOND;
    if(NULL != rtctime)
    {
        *rtctime = result;
    }

    return result;
}
// =============================================================================
// 函数功能：Rtc_TimeUs
//        获取日历时间，单位微秒
// 输入参数：
// 输出参数：rtctime,存储获取的RTC时间，秒数
// 返回值  ：获取的RTC时间，秒数
// 说明：距离1970年的时间;如果有RTC设备，则直接读取RTC设备的US，如果没有RTC时间，则自己用系统运行时间计算
// =============================================================================
s64 __Rtc_TimeUs(s64 *rtctime)
{
    s64 result;
    s64 systime;
    atom_low_t atom;

    systime = DJY_GetSysTime();
    //we'd better to get the RTC time now
    if(NULL == fnRtcGetTime)
    {
        //no rtc dev yet,we could cal the rtc time use the sysrunning time
#if (64 > CN_CPU_BITS)
        atom = Int_LowAtomStart();
        result = sgRtcTimeSet + systime - sgRtcUpdateTime2SysTime;
        sgRtcUpdateTime2SysTime = systime;
        sgRtcTimeSet = result;
        Int_LowAtomEnd(atom);
#else
        result = sgRtcTimeSet + systime - sgRtcUpdateTime2SysTime;
        sgRtcUpdateTime2SysTime = systime;
        sgRtcTimeSet = result;
#endif
    }
    else
    {
        if(false == fnRtcGetTime(&result))
        {
            //failed to get the rtc dev time
#if (64 > CN_CPU_BITS)
            atom = Int_LowAtomStart();
            result = sgRtcTimeSet + systime - sgRtcUpdateTime2SysTime;
            sgRtcUpdateTime2SysTime = systime;
            sgRtcTimeSet = result;
            Int_LowAtomEnd(atom);
#else
            result = sgRtcTimeSet + systime - sgRtcUpdateTime2SysTime;
            sgRtcUpdateTime2SysTime = systime;
            sgRtcTimeSet = result;
#endif
        }
        else
        {
#if (64 > CN_CPU_BITS)
            atom = Int_LowAtomStart();
            sgRtcUpdateTime2SysTime = systime;
            sgRtcTimeSet = result;
            Int_LowAtomEnd(atom);
#else
            sgRtcUpdateTime2SysTime = systime;
            sgRtcTimeSet = result;
#endif
        }
    }
//    result = result;
    if(NULL != rtctime)
    {
        *rtctime = result;
    }

    return result;
}
// =============================================================================
// 函数功能：Rtc_SetTime
//        设置日历时间，单位微秒
// 输入参数：rtctime,设置RTC时间，微秒数
// 输出参数：
// 返回值  ：获取的RTC时间，秒数
// 说明：距离1970年的时间;先设置RTC模块的日历时间；如果有RTC设备，同样记得设置RTC设备时间
// =============================================================================
bool_t __Rtc_SetTime(s64 rtctime)
{
    bool_t result = true;
    atom_low_t atom;

    s64 systime;

    systime = DJY_GetSysTime();

    if(NULL == fnRtcSetTime)
    {
#if (64 > CN_CPU_BITS)
        atom = Int_LowAtomStart();
        sgRtcTimeSet = rtctime;
        sgRtcUpdateTime2SysTime = systime;
        Int_LowAtomEnd(atom);
#else
        sgRtcTimeSet = rtctime;
        sgRtcUpdateTime2SysTime = systime;
#endif
    }
    else
    {
        if(fnRtcSetTime(rtctime))
        {
#if (64 > CN_CPU_BITS)
            atom = Int_LowAtomStart();
            sgRtcTimeSet = rtctime;
            sgRtcUpdateTime2SysTime = systime;
            Int_LowAtomEnd(atom);
#else
            sgRtcTimeSet = rtctime;
            sgRtcUpdateTime2SysTime = systime;
#endif
        }
        else
        {
            //rtc set failed, so don't update the rtc module time,otherwise will
            //make the rtc dev and rtc module time not sync
            result = false;
        }
    }
    return result;
}


