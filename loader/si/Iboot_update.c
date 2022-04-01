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
//#include <device/djy_flash.h>
#include "dbug.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。
#include "include/Iboot_info.h"
#if (CFG_RUNMODE_BAREAPP == 0)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// ============================================================================
// 功能：重启并运行iboot，先设置好内存标志，然后重启系统。Reset要求硬件提供一块内存，
//      在系统复位后能保留复位前的数据，一般SRAM才可以，甚至有些特殊的单片机如博通的
//      BK7251，复位后片内RAM都被初始化为0xAA。
// 参数：param，“1”代表使用 reset ，空或其他代表使用 reboot
// 返回：
// 备注：
// ============================================================================
bool_t  runiboot(char *param)
{
    s32 temp;
    temp = atoi(param);
    Iboot_SetRunIbootFlag();
#if(CN_CPU_OPTIONAL_CACHE==1)
    Cache_CleanData();
    Cache_InvalidInst();
#endif
    if(temp == 1)
        CPU_Reset();
    else
        CPU_Reboot();
    return true;
}

// ============================================================================
// 功能：重启并运行app，先设置好内存标志，然后重启系统。Reset要求硬件提供一块内存，
//      在系统复位后能保留复位前的数据，一般SRAM才可以，甚至有些特殊的单片机如博通的
//      BK7251，复位后片内RAM都被初始化为0xAA。
// 参数：param，“1”代表使用 reset ，空或其他代表使用 reboot
// 返回：
// 备注：
// ============================================================================
bool_t  runapp(char *param)
{
    s32 temp;
    temp = atoi(param);
    Iboot_SetRunAppFlag();
#if(CN_CPU_OPTIONAL_CACHE==1)
    Cache_CleanData();
    Cache_InvalidInst();
#endif
    if(temp == 1)
        CPU_Reset();
    else
        CPU_Reboot();
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
//     CPU_Reset();
//    return false;
//}

//// ============================================================================
//// 功能：设置升级APP标志和在交互信息中填充待升级app路径，然后重启进入iboot，将由
////      iboot执行具体的升级工作。（即使当前运行模式是iboot，也重启）
//// 参数：param -- 待升级app路径
//// 返回：
//// 备注：
//// ============================================================================
////bool_t Sh_UpdateApp(char *param)
//bool_t updateapp(char *param)
//{
//    char *word_param, *next_param;
//
//    word_param = shell_inputs(param,&next_param);
//    Iboot_SetRunIbootAndUpdateApp();
//    if(word_param == NULL)
//    {
//        set_upgrade_info(CFG_APP_UPDATE_NAME, sizeof(CFG_APP_UPDATE_NAME));
//    }
//    else
//    {
//        if(strlen(word_param) < 31)    //31是update_info.file.file_path的尺寸，暂且写着，后改。
//        {
//            set_upgrade_info(word_param, sizeof(word_param));
//        }
//        else
//        {
//            set_upgrade_info(CFG_APP_UPDATE_NAME, sizeof(CFG_APP_UPDATE_NAME));
//        }
//    }
//    Iboot_SetAfterUpdateRunMode(1);      //启动后运行app
//
//    runiboot(0);
//
//    return (TRUE);
//}
//
//// ============================================================================
//// 功能：设置升级iboot标志和在交互信息中填充待升级iboot路径，然后重启进入app，将由
////      app执行具体的升级工作。（即使当前运行模式是app，也重启），特别注意，APP中应
////      该有升级相关代码，这些代码可能跟bsp有关，因为许多移植版本的中断和异常向量在
////      iboot区间，运行中重写这些区域可能导致运行错误。
//// 参数：param -- 待升级iboot路径
//// 返回：
//// 备注：
//// ============================================================================
//bool_t updateiboot(char *param)
//{
//    char *word_param, *next_param;
//
//    word_param = shell_inputs(param,&next_param);
//    Iboot_SetRunAppAndUpdateIboot();
//    if(word_param == NULL)
//    {
//        set_upgrade_info(CFG_IBOOT_UPDATE_NAME, sizeof(CFG_IBOOT_UPDATE_NAME));
//    }
//    else
//    {
//        if(strlen(word_param) < 31)    //31是update_info.file.file_path的尺寸，暂且写着，后改。
//        {
//            set_upgrade_info(word_param, sizeof(word_param));
//        }
//        else
//        {
//            set_upgrade_info(CFG_IBOOT_UPDATE_NAME, sizeof(CFG_IBOOT_UPDATE_NAME));
//        }
//    }
//    Iboot_SetAfterUpdateRunMode(0);      //启动后运行app
//
//    runapp(0);
//
//    return (TRUE);
//}

//----------------------------------------------------------------------------
//功能: 把app从可寻址内存写入到xip文件系统中，由iboot调用，app头部需要在写入时修改的
//      跟指纹相关的部分，已经修改好
//参数: addr：内存起始地址，
//      file_size：文件大小，
//      file_name：app编译时的bin文件名，存在文件内部，不会因文件改名而被修改
//返回: true: 成功； false ： 失败.
//-----------------------------------------------------------------------------
bool_t WriteAppFromRam(s8 *addr,u32 file_size, s8 *file_name)
{
    bool_t ret = false;
    FILE *xipapp = 0;
    u32 file_name_len,res, residue_size = (s32)file_size;
    char *xipapppath;
    char percentage_last = 0, percentage = 0;
    file_name_len = strlen((const char *)file_name)+ strlen((const char *)CN_XIP_APP_PATH) + 2;
    xipapppath = (char *)malloc(file_name_len);
    if(xipapppath == NULL)
        return ret;
    sprintf(xipapppath, "%s/%s", CN_XIP_APP_PATH, file_name);
    xipapp = fopen(xipapppath, "w+");
    if(xipapp)
    {
        printf("open file success   %s ,\r\n", xipapppath);
        printf("\r\nloading       ");
        while(1)
        {
            percentage = 100 - ((char)((residue_size * 100)/ file_size));
            if(percentage != percentage_last)
            {
                printf("\b\b\b%2d%%",percentage);
                percentage_last = percentage;
            }
            if(residue_size > 1024)
            {
                res = fwrite(addr, 1, 1024, xipapp);
                if(res != 1024)
                    printf("write file xip-app error  \r\n ");
                addr += res;
                residue_size -= res;
            }
            else
            {
                res = fwrite(addr, 1, residue_size, xipapp);
                if(res != residue_size)
                    printf("write file xip-app error  \r\n ");
                addr += res;
                residue_size -= res;
            }
            if(residue_size == 0)
            {
                info_printf("IAP","app update success.\r\n");
                Iboot_ClearRunIbootAndUpdateApp();
                ret = true;
                break;
            }
            if((s32)residue_size < 0)
            {
                info_printf("IAP","app update fail.\r\n");
                break;
            }
        }
        fclose(xipapp);
        free(xipapppath);
    }
    else
        printf("open file fail,\r\n");

    return ret;
}

#define DjyosHeadSize    512
#define IAPBUF_SIZE      DjyosHeadSize
static u8 readbuffer[DjyosHeadSize];

// ============================================================================
// 功能：从文件中升级App，文件路径在iboot和app交互的96字节RAM中
// 参数：production_time，生产时间缓冲区，年月星期，ASCII码，4字节
//      production_num，生产序号缓冲区，一周内不重复，34进制，5字节
// 返回：false = 失败；true=成功
// 备注
// ============================================================================
//原名：Iboot_UpdateApp
bool_t WriteAppFromFile(u8 *production_time,u8* production_num)
{
    char apppath[CN_UPDATE_PATH_LIMIT];
    char xipapppath[CN_UPDATE_PATH_LIMIT];
    struct stat file_state;
    struct AppHead *p_apphead;
    struct ProductInfo *p_productinfo;
    FILE *srcapp = NULL;
    FILE *xipapp;
    s64 srcsize;
    u32 readsize,res;
    bool_t ret = false;
//  char *file;
    char percentage_last = 0, percentage = 0;

    Iboot_GetOtaFilename(apppath, CN_UPDATE_PATH_LIMIT);
    if(apppath[0] != '/')       //路径须完整的全路径
    {
        printk(" file path error \r\n");
        return false;
    }
    if(!XIP_CheckAppInFile(apppath))
    {
        printk("verify file error \r\n");
        return false;
    }
    memset(&file_state, 0, sizeof(struct stat));
    if(stat(apppath,&file_state))
    {
        printk("get ota file fail\r\n");
        return false;
    }
    srcapp = fopen(apppath, "r");
    if(srcapp == NULL)
    {
        error_printf("IAP","file \"%s\" is not found.\r\n", apppath);
        DJY_EventDelay(1000*1000);      //延时一下，让升级过程中的信息能打印出来
    }
    p_apphead = (struct AppHead *)readbuffer;
    p_productinfo = (struct ProductInfo *)(p_apphead+1);
    res = fread(readbuffer, 1, DjyosHeadSize,srcapp);
    if(res == DjyosHeadSize)
    {
        srcsize = file_state.st_size;
        if(srcsize  == p_apphead->app_bin_size)
        {
            info_printf("IAP","app update start...\r\n");
            memcpy(p_productinfo->ProductionNumber, production_num, sizeof(p_productinfo->ProductionNumber));
            memcpy(p_productinfo->ProductionTime, production_time, sizeof(p_productinfo->ProductionTime));
//          file = strrchr(p_apphead->app_name, '/');
            //把新APP的文件名连到 "/xip-app" 后面，形成xip文件路径
            sprintf(xipapppath, "%s/%s", CN_XIP_APP_PATH, p_apphead->app_name);
            if(!File_Format((const char *)CN_XIP_APP_PATH))
            {
                xipapp = fopen(xipapppath, "w+");
                if(xipapp != NULL)
                {
                    res = fwrite(readbuffer, 1, DjyosHeadSize, xipapp);
                    srcsize -= res;
                    while(1)
                    {
                        percentage = 100 - ((char)((srcsize * 100)/ file_state.st_size));
                        if(percentage != percentage_last)
                        {
                            printf("\b\b\b%2d%%",percentage);
                            percentage_last = percentage;
                        }
                        readsize = fread(readbuffer, 1, IAPBUF_SIZE, srcapp);
                        if((readsize != IAPBUF_SIZE) && srcsize >= IAPBUF_SIZE)
                        {
                            printf("Iap read file %s error \n\r",apppath);
                            break;
                        }
                        if(readsize != IAPBUF_SIZE)
                        {
                            printf("read file %s End \r\n ",apppath);
                        }

                        res = fwrite(readbuffer, 1, readsize, xipapp);
                        if(res != readsize)
                        {
                            printf("write file xip-app error  \r\n ");
                        }
                        srcsize -= readsize;
                        if(srcsize == 0)
                        {
                            info_printf("IAP","App update success.  waiting to restart.\r\n");
//                            Iboot_ClearRunIbootAndUpdateApp();
                            ret = true;
                            break;
                        }
                    }
                    fclose(srcapp);
                    fclose(xipapp);
                    if(srcsize !=0)
                    {
                        error_printf("IAP","app update error .\r\n");
                        DJY_EventDelay(1000*1000);      //延时一下，让升级过程中的信息能打印出来
                    }
//                    else
//                        Iboot_UpdateToRun();
                }
                else
                {
                    error_printf("IAP","cannot open source file xip-app .\r\n");
                    DJY_EventDelay(1000*1000);      //延时一下，让升级过程中的信息能打印出来
                }
            }
            else
            {
                printk("erase flash fail\r\n");
                ret = false;
            }
        }
        else
        {
            printk("file size is overlong\r\n");
        }
    }
    else
    {
        printk("open ota file fail\r\n");
        ret = false;
    }


    return ret;
}

// ============================================================================
// 功能：升级Iboot事件，
// 参数：无。
// 返回：1（无意义）。
// 备注
// ============================================================================
bool (*Update_and_run_mode)(char *param);
ptu32_t Iboot_AppUpdateIboot(void)
{
    FILE *srciboot;
    FILE *xipiboot;
    u8 buf[IAPBUF_SIZE];
    u32 readsize,res;
    struct stat test_stat;
    s64 srcsize;
    char iapibootname[CN_UPDATE_PATH_LIMIT];
    char percentage_last = 0, percentage = 0;

    info_printf("IAP","iboot update start.\r\n");
    Iboot_GetOtaFilename(iapibootname, CN_UPDATE_PATH_LIMIT);
    if(iapibootname[0] != '/')       //路径须完整的全路径
    {
        printk(" file path error \r\n");
        DJY_EventDelay(100*1000);      //延时一下，让升级过程中的信息能打印出来
        return false;
    }
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
                   Iboot_ClearRunAppAndUpdateIboot();
                   break;
               }
           }
           fclose(srciboot);
           fclose(xipiboot);
           if(srcsize !=0)
           {
               error_printf("IAP","Iboot update error .\r\n");
               DJY_EventDelay(1000*1000);      //延时一下，让升级过程中的信息能打印出来
           }
           else if(Update_and_run_mode != NULL)
               Update_and_run_mode(NULL);
           if(srcsize ==0)
               Iboot_UpdateToRun();
       }
       else
       {
           error_printf("IAP","cannot open source file xip-iboot .\r\n");
           DJY_EventDelay(1000*1000);      //延时一下，让升级过程中的信息能打印出来
       }
    }
    else
    {
       error_printf("IAP","file \"%s\" is not found.\r\n", iapibootname);
       DJY_EventDelay(1000*1000);      //延时一下，让升级过程中的信息能打印出来
    }
    return TRUE;
}

//------------------------------------------------------------------------------
//功能：一个弱函数，用户可以自己在APP中重新实现，用来升级iboot。
//参数：无效
//返回：恒定返回false，用户重新实现则须返回true
//------------------------------------------------------------------------------
__attribute__((weak)) bool_t Iboot_UserUpdateIboot(char *param)
{
    return false;
}

bool_t ModuleInstall_UpdateIboot(void)
{
    uint16_t evtt_Update = CN_EVTT_ID_INVALID;
    char run_mode = Iboot_GetRunMode();
    if(Iboot_UserUpdateIboot(0) == false)
    {
        if(Iboot_GetUpdateSource() == CN_STORE_IN_FILE)
        {
            if(run_mode == 1)
            {
                if(Iboot_GetUpdateIboot() == true)
                {
                    evtt_Update = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0,
                                                Iboot_AppUpdateIboot, NULL, CFG_MAINSTACK_LIMIT, "update iboot");
                }
            }
            else
                return false;

            if(evtt_Update != CN_EVTT_ID_INVALID)
            {
                if(DJY_EventPop(evtt_Update, NULL, 0, 0, 0, 0) != CN_EVENT_ID_INVALID)
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
//ADD_TO_ROUTINE_SHELL(updateapp,updateapp,"update app lication");
//ADD_TO_ROUTINE_SHELL(updateiboot,updateiboot,"Update Iboot.");

#endif


//add some boot commands
//static bool_t rebootshell(char *param)
bool_t rebootshell(char *param)
{

    CPU_Reboot( );
    return true;
}
#pragma GCC diagnostic pop

//static bool_t resetshell(char *param)
bool_t resetshell(char *param)
{
    CPU_Reset();
    return true;
}
//static bool_t reloadshell(char *param)
bool_t restart(char *param)
{
    CPU_RestartSystem();
    return true;
}

ADD_TO_ROUTINE_SHELL(restart,restart,"usage:restart [key](if key is 0XAA55AA55 then will not record)");
ADD_TO_ROUTINE_SHELL(reboot,rebootshell,"usage:rebootshell [key](if key is 0XAA55AA55 then will not record)");
ADD_TO_ROUTINE_SHELL(reset,resetshell,"usage:resetshell [key](if key is 0XAA55AA55 then will not record)");








