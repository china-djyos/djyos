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
// 文件名     ：
// 模块描述:
// 模块版本:
// 创建人员: czz
// 创建时间:
// =============================================================================

#ifndef __SI_INFO_H__
#define __SI_INFO_H__
#include "stdio.h"

#define MutualPathLen   40   //在交互信息中存待升级文件路径的最大长度
enum runibootmode{
    HEARD_SET_RUN_IBOOT,
    RAM_SET_RUN_IBOOT,
    UPDATE_APP_RUN_IBOOT,
    CHACK_ERROR,
};

enum runappmode{
    RUN_APP_FROM_FILE,
    RUN_APP_FROM_DIRECT,
    RUN_APP_FROM_UPDATE,
};

enum hardflag
{
    POWER_ON_FLAG,
    HEAD_RESET_FLAG,
    HEAD_WDT_RESET,
    LOW_POWER_WAKEUP,
    POWER_ON_RESET,

};
//#define IBOOT_APP_INFO_VER                 (1)
struct IbootAppInfo
{
    #define PREVIOURESET_IBOOT   (0x12345678)//复位前运行iboot
    #define PREVIOURESET_APP     (0x87654321)//复位前运行APP
    u32 PreviouReset;//复位前运行模式
    struct{
        u32 heard_set_run_iboot   :1;//硬件设置运行iboot
        u32 restart_run_iboot     :1;//指示启动后运行Iboot
        u32 restart_run_app       :1;//指示启动后运行APP
        u32 runmode_iboot         :1;//当前运行模式是iboot
        u32 runmode_app           :1;//当前运行模式为app
        u32 Before_run_iboot      :1;//之前运行模式为iboot
        u32 Before_run_app        :1;//之前运行模式为app
        u32 run_app_form_file     :1;//从文件中加载app
        u32 run_iboot_update_app  :1;//启动（Iboot）后，自动升级APP
        u32 run_app_update_iboot  :1;//启app后升级iboot自身
        u32 update_from           :2;//升级文件来源0文件 1——3待定义
        u32 update_runmode        :1;//升级完成后运行0.iboot --  1.app
        u32 error_app_check       :1;//校验出错
        u32 error_app_no_file     :1;//没有这个文件或文件格式错误
        u32 error_app_size        :1;//app文件大小错误
        //上电复位硬件标志0=无此硬件 1=有此硬件，但无标志；2=有标志，阅后即焚；3=有，非阅后即焚；
        u32 power_on_flag         :2;
        u32 head_wdt_reset        :1;//看门狗复位标志
        u32 soft_reset_flag       :1;//软件引起的内部复位
        u32 reboot_flag           :1;//reboot 标志
        u32 restart_app_flag      :1;//restart_app标志
        u32 head_reset_flag       :1;//外部硬件复位标志
        u32 low_power_wakeup      :1;//低功耗深度休眠中断唤醒标志
        u32 call_fun_resent       :1;//1=内部复位/重启是主动调用相关函数引发的；0=异常重启
        u32 power_on_resent_flag  :1;//上电复位标志，结合b18~19以及“上电标志”字判定
    }runflag; //运行标志
    u64  reserved;//保留
    u16  iboot_buildyear;    /* years since 1900 */
    u8   iboot_buildmon;     /* months since January [0-11] */
    u8   iboot_buildmday;    /* day of the month [1-31] */
    u8   iboot_buildhour;    /* hours since midnight [0-23] */
    u8   iboot_buildmin;     /* minutes after the hour [0-59] */
    u8   iboot_buildsec;     /* seconds after the minute [0-60] */
    u16  app_buildyear;    /* years since 1900 */
    u8   app_buildmon;     /* months since January [0-11] */
    u8   app_buildmday;    /* day of the month [1-31] */
    u8   app_buildhour;    /* hours since midnight [0-23] */
    u8   app_buildmin;     /* minutes after the hour [0-59] */
    u8   app_buildsec;     /* seconds after the minute [0-60] */
    u8   ibootVer_small;         //iboot 版本 xx.xx.__
    u8   ibootVer_medium;        //iboot 版本 xx.__.xx
    u8   ibootVer_large;         //iboot 版本 __.xx.xx
    u8   appVer_small;           //app 版本 xx.xx.__
    u8   appVer_medium;          //app 版本 xx.__.xx
    u8   appVer_large;           //app 版本 __.xx.xx
    char boardname[20];    //组件名
    char update_path[40];    //待升级文件路径
};

bool_t Run_Iboot(enum runibootmode mode);
bool_t Run_App(enum runappmode mode);

bool_t Set_UpdateRunModet(u8 mode);
bool_t Set_RunIbootUpdateIboot();
bool_t Set_RunIbootUpdateApp();
bool_t Set_RunAppFlag();
bool_t Set_RebootFlag();
bool_t Set_RunIbootFlag();
bool_t Set_SoftResetFlag();
bool_t Set_PreviouResetFlag();
bool_t Clear_RunIbootUpdateApp();
char Get_RunMode(void);
char * Get_MutualAppPath(void);
char Get_UpdateApp(void);
bool_t clear_resetflag();
bool_t Update_ToRun();
bool_t Si_IbootAppInfoInit();
bool_t XIP_IsRamIbootFlag();
bool_t Fill_MutualUpdatePath(char* Path);

u32  XIP_GetAPPSize(void * apphead);
u32  Get_AppHeadSize(void);
void * XIP_GetAPPStartAddr(void * apphead);
char*  Get_AppName(void * apphead);
bool_t XIP_APPIsDebug(void );
bool_t XIP_AppFileChack(void * apphead);
bool_t Rewrite_AppHead(void * apphead,const char*name,u32 filesize);

#endif /* __IICBUS_H__ */
