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
#include <blackbox.h>
#include <device/flash/flash.h>
#include "dbug.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。
#include "Iboot_info.h"
#if (CFG_RUNMODE_BAREAPP == 0)

#define IAPBUF_SIZE   512
extern void reboot();
extern void reset();
extern void restart_system(u32 key);
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
    return true;
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
    return true;
}



// ============================================================================
// 功能：设置运行iboot并更新iboot
// 参数：
// 返回：
// 备注：
// ============================================================================
// bool_t  updateruniboot(char *param)
//{
//    Set_RunIbootUpdateIboot();
//#if(CN_CPU_OPTIONAL_CACHE==1)
//     Cache_CleanData();
//     Cache_InvalidInst();
//#endif
//     reset();
//    return false;
//}

 // ============================================================================
 // 功能：根据字符串设置升级成功后是运行app还是iboot
 // 参数：
 // 返回：true -- 设置成功；false -- 没有设置
 // 备注：
 // ============================================================================
 bool_t runapp_or_runiboot(char *mode)
 {
      if(strcmp(mode, "runapp") == 0)
          Set_UpdateRunModet(1);
      else if(strcmp(mode, "runiboot") == 0)
              Set_UpdateRunModet(0);
      else
          return false;

      return true;
 }
// ============================================================================
// 功能：设置升级APP标志和在交互信息中填充待升级app路径
// 参数：param -- 待升级app路径
// 返回：
// 备注：
// ============================================================================
//bool_t Sh_UpdateApp(char *param)
bool_t updateapp(char *param)
{
    bool_t res = true;
    char *word_param, *next_param;
    Set_RunIbootUpdateApp();

    word_param = shell_inputs(param,&next_param);
    if(word_param == NULL)
    {
        res = Fill_MutualUpdatePath(CFG_APP_UPDATE_NAME, sizeof(CFG_APP_UPDATE_NAME));
        Set_UpdateRunModet(1);      //启动后运行app
    }
    else
    {
        if(runapp_or_runiboot(word_param) == false)
        {
            res = Fill_MutualUpdatePath(word_param, strlen(word_param));
            word_param = shell_inputs(next_param,&next_param);
            if(word_param != NULL)
            {
                if(runapp_or_runiboot(word_param) == false)
                    Set_UpdateRunModet(1);      //未指定升级成功后运行app还是iboot，则运行iboot
            }
            else
                Set_UpdateRunModet(1);      //启动后运行app
        }
        else
        {
            word_param = shell_inputs(next_param,&next_param);
            if(word_param != NULL)
                res = Fill_MutualUpdatePath(word_param, strlen(word_param));
            else
                res = Fill_MutualUpdatePath(CFG_APP_UPDATE_NAME, sizeof(CFG_APP_UPDATE_NAME));
        }
    }

    if(res == false)
        error_printf("iap"," file name is longer than 40.");
    else
        runiboot(0);

    Djy_EventDelay(5000*1000);      //延时一下，让升级过程中的信息能打印出来
    return (TRUE);
}

// ============================================================================
// 功能：升级App事件，
// 参数：无。
// 返回：0（无意义）。
// 备注
// ============================================================================
bool_t Iboot_UpdateApp(void)
{
    char apppath[MutualPathLen];
    char xipapppath[MutualPathLen];
    struct stat test_stat;
    FILE *srcapp = NULL;
    FILE *xipapp;
    s64 srcsize;
    u8 buf[IAPBUF_SIZE];
    u32 readsize,res;
    char *file;
    char percentage_last = 0, percentage = 0;

    if((Get_UpdateSource() == 0) && (Get_UpdateApp() == true))
    {
        if(!stat(CFG_FORCED_UPDATE_PATH,&test_stat))
            srcapp = fopen(CFG_FORCED_UPDATE_PATH, "r+");

        info_printf("IAP","app update start.\r\n");
        if(srcapp == NULL)
        {
            strcpy(apppath, Get_MutualUpdatePath());
            srcapp = fopen(apppath, "r+");
        }
        else
        {
            strcpy(apppath, CFG_FORCED_UPDATE_PATH);
            Set_UpdateRunModet(1);
        }
        if(srcapp != NULL)
        {
            file = strrchr(apppath, '/');
            if(file)
            {
                sprintf(xipapppath, "%s%s", "/xip-app", file);
                xipapp = fopen(xipapppath, "w+");
                if(xipapp != NULL)
                {
                    stat(apppath,&test_stat);
                    srcsize = test_stat.st_size;
                    printf("\r\nloading       ");
                    while(1)
                    {
                        percentage = 100 - ((char)((srcsize * 100)/ test_stat.st_size));
                        if(percentage != percentage_last)
                        {
                            printf("\b\b\b%2d%%",percentage);
                            percentage_last = percentage;
                        }
                        readsize = fread(buf, 1, IAPBUF_SIZE, srcapp);
                        if((readsize != IAPBUF_SIZE) && srcsize >= IAPBUF_SIZE)
                        {
                            printf("Iap read file %s error \n\r",apppath);
                            break;
                        }
                        if(readsize != IAPBUF_SIZE)
                        {
                            printf("read file %s End \r\n ",apppath);
                        }

                        res = fwrite(buf, 1, readsize, xipapp);
                        if(res != readsize)
                        {
                            printf("write file xip-app error  \r\n ");
                        }
                        srcsize -= readsize;
                        if(srcsize == 0)
                        {
                            info_printf("IAP","App update success.  waiting to restart.\r\n");
                            Clear_RunIbootUpdateApp();
                            break;
                        }
                    }
                    fclose(srcapp);
                    fclose(xipapp);
                    if(srcsize !=0)
                    {
                        error_printf("IAP","app update error .\r\n");
                        Djy_EventDelay(1000*1000);      //延时一下，让升级过程中的信息能打印出来
                    }
                    else
                        Update_ToRun();
                }
                else
                {
                    error_printf("IAP","cannot open source file xip-app .\r\n");
                    Djy_EventDelay(1000*1000);      //延时一下，让升级过程中的信息能打印出来
                }
            }
            else
            {
                error_printf("IAP","App file error .\r\n");
                Djy_EventDelay(1000*1000);      //延时一下，让升级过程中的信息能打印出来
            }
        }
        else
        {
            error_printf("IAP","file \"%s\" is not found.\r\n", apppath);
            Djy_EventDelay(1000*1000);      //延时一下，让升级过程中的信息能打印出来
        }
    }

    return TRUE;
}

// ============================================================================
// 功能：设置升级iboot标志和在交互信息中填充待升级iboot路径
// 参数：param -- 待升级iboot路径
// 返回：
// 备注：
// ============================================================================
bool_t updateiboot(char *param)
{
    bool_t res = true;
    char *word_param, *next_param;
    Set_RunAppUpdateIboot();

    word_param = shell_inputs(param,&next_param);
    if(word_param == NULL)
    {
        res = Fill_MutualUpdatePath(CFG_IBOOT_UPDATE_NAME, sizeof(CFG_IBOOT_UPDATE_NAME));
        Set_UpdateRunModet(0);      //启动后运行iboot
    }
    else
    {
        if(runapp_or_runiboot(word_param) == false)
        {
            res = Fill_MutualUpdatePath(word_param, strlen(word_param));
            word_param = shell_inputs(next_param,&next_param);
            if(word_param != NULL)
            {
                if(runapp_or_runiboot(word_param) == false)
                    Set_UpdateRunModet(0);      //未指定升级成功后运行app还是iboot，则运行iboot
            }
            else
                Set_UpdateRunModet(0);      //启动后运行iboot
        }
        else
        {
            word_param = shell_inputs(next_param,&next_param);
            if(word_param != NULL)
                res = Fill_MutualUpdatePath(word_param, strlen(word_param));
            else
                res = Fill_MutualUpdatePath(CFG_IBOOT_UPDATE_NAME, sizeof(CFG_IBOOT_UPDATE_NAME));
        }
    }

    if(res == false)
        error_printf("iap"," file name is longer than 40.");
    else
        runapp(0);

    Djy_EventDelay(5000*1000);      //延时一下，让升级过程中的信息能打印出来
    return (TRUE);
}

// ============================================================================
// 功能：升级Iboot事件，
// 参数：无。
// 返回：1（无意义）。
// 备注
// ============================================================================
bool (*Update_and_run_mode)(char *param);
bool_t App_UpdateIboot(char *param)
{
    FILE *srciboot;
    FILE *xipiboot;
    u8 buf[IAPBUF_SIZE];
    u32 readsize,res;
    struct stat test_stat;
    s64 srcsize;
    char iapibootname[MutualPathLen];
    char percentage_last = 0, percentage = 0;

    info_printf("IAP","iboot update start.\r\n");
    strcpy(iapibootname, Get_MutualUpdatePath());
    Update_and_run_mode = NULL;

    srciboot = fopen(iapibootname, "r+");
    if(srciboot != NULL)
    {
       xipiboot = fopen("/xip-iboot/iboot.bin", "w+");
       if(xipiboot != NULL)
       {
           stat(iapibootname,&test_stat);
           srcsize = test_stat.st_size;
           printf("\r\nloading       ");
           while(1)
           {
               percentage = 100 - ((char)((srcsize * 100)/ test_stat.st_size));
               if(percentage != percentage_last)
               {
                   printf("\b\b\b%2d%%",percentage);
                   percentage_last = percentage;
               }
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
                   Clear_RunAppUpdateIboot();
                   break;
               }
           }
           fclose(srciboot);
           fclose(xipiboot);
           if(srcsize !=0)
           {
               error_printf("IAP","Iboot update error .\r\n");
               Djy_EventDelay(1000*1000);      //延时一下，让升级过程中的信息能打印出来
           }
           else if(Update_and_run_mode != NULL)
               Update_and_run_mode(NULL);
           if(srcsize ==0)
               Update_ToRun();
       }
       else
       {
           error_printf("IAP","cannot open source file xip-iboot .\r\n");
           Djy_EventDelay(1000*1000);      //延时一下，让升级过程中的信息能打印出来
       }
    }
    else
    {
       error_printf("IAP","file \"%s\" is not found.\r\n", iapibootname);
       Djy_EventDelay(1000*1000);      //延时一下，让升级过程中的信息能打印出来
    }
    return TRUE;
}

//用户可以写该函数来指定怎么升级iboot
__attribute__((weak)) bool_t user_update_iboot(char *param)
{
    return false;
}

bool_t ModuleInstall_XIP(void)
{
    uint16_t evtt_Update = CN_EVTT_ID_INVALID;
    char run_mode = Get_RunMode();
    if(user_update_iboot(0) == false)
    {
        if(Get_UpdateSource() == 0)
        {
            if(run_mode == 1)
            {
                if(Get_Updateiboot() == true)
                {
                    evtt_Update = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0,
                                                App_UpdateIboot, NULL, CFG_MAINSTACK_LIMIT, "update iboot");
                }
            }
            else
                return false;

            if(evtt_Update != CN_EVTT_ID_INVALID)
            {
                if(Djy_EventPop(evtt_Update, NULL, 0, NULL, 0, 0) != CN_EVENT_ID_INVALID)
                {
                    if(run_mode == 1)
                        info_printf("XIP","add iboot update function.\r\n");

                    return true;
                }
            }
            return false;
        }
        else
            return false;
    }
    return true;
}


ADD_TO_ROUTINE_SHELL(runiboot,runiboot,NULL);
ADD_TO_ROUTINE_SHELL(runapp,runapp,"直接运行APP(仅在采取内存标示确定加载项目时有效)");
//ADD_TO_ROUTINE_SHELL(runibootui,runibootui,"设置运行iboot并更新iboot并升级iboot");
//ADD_TO_ROUTINE_SHELL(updateruniboot,updateruniboot,"设置运行iboot并升级iboot");
ADD_TO_ROUTINE_SHELL(updateapp,updateapp,"update app lication");
ADD_TO_ROUTINE_SHELL(updateiboot,updateiboot,"Update Iboot.");

#endif


//add some boot commands
//static bool_t rebootshell(char *param)
bool_t rebootshell(char *param)
{

    reboot( );
    return true;
}
//static bool_t resetshell(char *param)
bool_t resetshell(char *param)
{
    u32 key = 0;
    if(NULL != param)
    {
        key = strtoul(param,NULL,0);
    }
    reset(key);
    return true;
}
//static bool_t reloadshell(char *param)
bool_t restart(char *param)
{
    u32 key = 0;
    if(NULL != param)
    {
        key = strtoul(param,NULL,0);
    }
    restart_system(key);
    return true;
}

//static bool_t bootaddressshell(char *param)
bool_t bootaddress(char *param)
{
    u32 addr;
    u32 InitCpu_Addr;
    addr = strtoul(param,NULL,0);

    debug_printf("OsStart","%s:addr:0x%08x\n\r",__FUNCTION__,addr);
    InitCpu_Addr = *(u32*)addr;
    ((void (*)(void))(InitCpu_Addr))();

    return true;
}
ADD_TO_ROUTINE_SHELL(restart,restart,"usage:restart [key](if key is 0XAA55AA55 then will not record)");
ADD_TO_ROUTINE_SHELL(bootaddress,bootaddress,"usage:bootaddress [address]");
ADD_TO_ROUTINE_SHELL(rebootshell,rebootshell,"usage:rebootshell [key](if key is 0XAA55AA55 then will not record)");
ADD_TO_ROUTINE_SHELL(resetshell,resetshell,"usage:resetshell [key](if key is 0XAA55AA55 then will not record)");








