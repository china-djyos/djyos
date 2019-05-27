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
//所属模块: Iboot
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 在应用编程的核心文件
//其他说明:
//修订历史:
//1. 日期: 2016-06-17
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <device.h>
#include <stat.h>
#include <dirent.h>
#include <xip.h>
#include "shell.h"
#include <djyos.h>
#include <stdlib.h>
#include <string.h>
#include "version.h"
#include "cpu_peri.h"
#include "cpu-optional.h"
#include "component_config_iboot.h"
#if(CN_CPU_OPTIONAL_CACHE==1)
#include "set-cache.h"
#endif
#include <osboot.h>
#include <device/flash/flash.h>
#include "dbug.h"
#include "project_config.h"
#if !defined (CFG_RUNMODE_BAREAPP)

static struct SemaphoreLCB *ptUpdateIbootSemp;
// ============================================================================
// 功能：设置运行iboot
// 参数：
// 返回：
// 备注：
// ============================================================================
bool_t  runiboot(char *param)
{
    Set_RunIbootFlag();
#if(CN_CPU_OPTIONAL_CACHE==1)
     Cache_CleanData();
     Cache_InvalidInst();
#endif
     reset();
    return false;
}

// ============================================================================
// 功能：设置运行app
// 参数：
// 返回：
// 备注：
// ============================================================================
bool_t  runapp(char *param)
{
    Set_RunAppFlag();
#if(CN_CPU_OPTIONAL_CACHE==1)
     Cache_CleanData();
     Cache_InvalidInst();
#endif
     reset();
    return false;
}

// ============================================================================
// 功能：设置运行iboot并更新iboot
// 参数：
// 返回：
// 备注：
// ============================================================================
bool_t runibootui(char *param)
{
    Set_RunIbootUpdateIboot();
#if(CN_CPU_OPTIONAL_CACHE==1)
     Cache_CleanData();
     Cache_InvalidInst();
#endif
     reset();
    return false;
}

// ============================================================================
// 功能：设置运行iboot并更新iboot
// 参数：
// 返回：
// 备注：
// ============================================================================
 bool_t  updateruniboot(char *param)
{
    Set_RunIbootUpdateIboot();
#if(CN_CPU_OPTIONAL_CACHE==1)
     Cache_CleanData();
     Cache_InvalidInst();
#endif
     reset();
    return false;
}

// ============================================================================
// 功能：升级APP
// 参数：无
// 返回：
// 备注：
// ============================================================================
//bool_t Sh_UpdateApp(char *param)
bool_t updateapp(char *param)
{
    runiboot(0);
    return (TRUE);
}

// ============================================================================
// 功能：升级Iboot
// 参数：无
// 返回：
// 备注：
// ============================================================================
char iapibootname[512];
#define IAPBUF_SIZE   512
bool (*Update_and_run_mode)(char *param);

bool updateiboot(char *param)
{
    if(param == NULL)
        strcpy(iapibootname,CFG_IBOOT_UPDATE_NAME);
    else
        strcpy(iapibootname,param);
   Update_and_run_mode = NULL;
   Lock_SempPost(ptUpdateIbootSemp);
   return true;
}

// ============================================================================
// 功能：升级Iboot事件，
// 参数：无。
// 返回：1（无意义）。
// 备注
// ============================================================================
static ptu32_t __UpdateIboot(void)
{
    FILE *srciboot;
    FILE *xipiboot;
    u8 buf[IAPBUF_SIZE];
    u32 readsize,res;
    struct stat test_stat;
    s64 srcsize;

    while(1)
    {
       Lock_SempPend(ptUpdateIbootSemp,CN_TIMEOUT_FOREVER);
       stat(iapibootname,&test_stat);
       srcsize = test_stat.st_size;
       srciboot = fopen(iapibootname, "r+");
       if(srciboot==NULL)
       {
           error_printf("IAP","file \"%s\" is not found.\r\n", CFG_IBOOT_UPDATE_NAME);
           break;
       }
       xipiboot = fopen("/xip-iboot/iboot.bin", "r+");
       if(srciboot==NULL)
       {
           error_printf("IAP","cannot open source file xip-iboot .\r\n");
           break;
       }
       while(1)
       {
           readsize = fread(buf, 1, IAPBUF_SIZE, srciboot);
           if((readsize != IAPBUF_SIZE) && srcsize >= IAPBUF_SIZE)
           {
               printf("Iap read file %s error \n\r",iapibootname);
               break;
           }
           if(readsize != IAPBUF_SIZE)
           {
               printf("read file %s End \r\n ",iapibootname);
           }

           res = fwrite(buf, 1, readsize, xipiboot);
           if(res != readsize)
           {
               printf("write file xip-iboot error  \r\n ");
           }
           srcsize -= readsize;
           if(srcsize == 0)
           {
               info_printf("IAP","Iboot update success.\r\n");
               break;
           }
       }
       fclose(srciboot);
       fclose(xipiboot);
       if(srcsize !=0)
           error_printf("IAP","Iboot update error .\r\n");
       else if(Update_and_run_mode != NULL)
           Update_and_run_mode(NULL);

    }
    return 0;
}
// ============================================================================
// 功能：安装IAP组件。
// 参数：无。
// 返回：0（无意义）。
// 备注
// ============================================================================
ptu32_t ModuleInstall_IAP(void)
{
     uint16_t evtt_id,event_id;
    ptUpdateIbootSemp=Lock_SempCreate(1,0,CN_BLOCK_FIFO,"UpdateIbootSemp");
    if(ptUpdateIbootSemp==NULL)
    {
        debug_printf("IAP","IAP ModuleInstall failed.\r\n");
    }
    evtt_id = Djy_EvttRegist(EN_CORRELATIVE,5,0,0,__UpdateIboot,
              NULL,8192,"Update Iboot");
    if(evtt_id!=CN_EVTT_ID_INVALID)
    {
       event_id=Djy_EventPop(evtt_id,NULL,0,0,0,0);
    }
    else
    {
        Djy_EvttUnregist(evtt_id);
        debug_printf("IAP","Update Iboot evtt pop failed.\r\n");
    }

    return 0;
}

ADD_TO_ROUTINE_SHELL(runiboot,runiboot,NULL);
ADD_TO_ROUTINE_SHELL(runapp,runapp,"直接运行APP(仅在采取内存标示确定加载项目时有效)");
//ADD_TO_ROUTINE_SHELL(runibootui,runibootui,"设置运行iboot并更新iboot并升级iboot");
//ADD_TO_ROUTINE_SHELL(updateruniboot,updateruniboot,"设置运行iboot并升级iboot");
//ADD_TO_ROUTINE_SHELL(updateapp,updateapp,"update app lication");
ADD_TO_ROUTINE_SHELL(updateiboot,updateiboot,"Update Iboot.");

#endif
















