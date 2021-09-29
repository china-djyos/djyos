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
#include "stdint.h"
#include "stdio.h"
#include "xip.h"
#include <stdlib.h>
#include "string.h"
#include "cpu-optional.h"
#include "component_config_iboot.h"
#if(CN_CPU_OPTIONAL_CACHE==1)
#include "set-cache.h"
#endif
#include "dbug.h"
#include "iboot_info.h"
#include <xip.h>

struct CopyRecord{
    u32 load_start_address;
    u32 run_start_address;
    u32 size;
    u32 type;    //0:zero init;    1:copy
};

struct copy_table{
    u32 record_size;
    u32 record_cnt;
    struct CopyRecord record[1];
};

//-----------------------------------------------------------------
//功能：由硬件决定是否强制进入Iboot，若此函数返回TRUE，则强制运行Iboot。通常会使
//      用一个gpio，通过跳线决定。
//      正常情况下，如果正在运行APP，是可以用runiboot命令切换到Iboot状态的，设置
//      此硬件的目的有二：
//     1、在严重异常错误，不能用shell切换时，提供一个补救措施。
//     2、出于安全考虑，APP中没有包含切换代码，或者由于资源的关系，裁掉了shell。
//参数：无
//返回：无。
//说明：本函数所涉及到的硬件，须在本文件中初始化，特别需要注意的是，不允许调用未
//      加载的函数，特别是库函数。
//      本函数必须提供，如果没有设置相应硬件，可以简单返回false。
//-----------------------------------------------------------------
__attribute__((weak))  bool_t Iboot_IAP_IsForceIboot()
{
    return false;
}
//----选择加载项目代码-----------------------------------------------------------
//功能：选择加载存储存储器中哪个项目代码,Iboot或APP。
//参数: 无。
//返回: 无。
//----------------------------------------------------------------------------
void Iboot_IAP_SelectLoadProgam(void)
{
#if (CFG_RUNMODE_BAREAPP == 1)
        Iboot_LoadPreload();   //直接加载运行APP
#else
    Iboot_SiIbootAppInfoInit();

    if(Iboot_IAP_IsForceIboot())//硬件设置运行iboot
    {
        Run_Iboot(HARD_SET_RUN_IBOOT);//填充硬件设置运行iboot信息
    }
    if(XIP_IsRamIbootFlag())//ram中标记运行iboot
    {
        Run_Iboot(RAM_SET_RUN_IBOOT);
    }

#if (CFG_APP_RUNMODE == CN_RUN_FORM_FILE)
    if(false == Run_App(RUN_APP_FROM_FILE))//
#elif (CFG_APP_RUNMODE == CN_DIRECT_RUN)
    if(false == Run_App(RUN_APP_FROM_DIRECT))
#else
    #error "error ： 没有定义APP加载方式！！";
#endif
        Run_Iboot(CHACK_ERROR);
#endif      //for (CFG_RUNMODE_BAREAPP == 1)

}

void Iboot_PreStart(void);

extern struct copy_table preload_copy_table;

//----预加载程序---------------------------------------------------------------
//功能：加载主加载器、中断管理模块，紧急代码
//参数: 无。
//返回: 无。
//----------------------------------------------------------------------------
//备注: 本函数移植敏感，与开发系统有关，也与目标硬件配置有关
void Iboot_LoadPreload(void)
{
//    void (*volatile pl_1st)(void) = Iboot_PreStart;

    volatile u32 *src,*des;
    volatile u32 i, j;
    for(i=0; i<preload_copy_table.record_cnt; i++) {
        src = (u32*) preload_copy_table.record[i].load_start_address;
        des = (u32*) preload_copy_table.record[i].run_start_address;
        if(preload_copy_table.record[i].type == 1) {    //copy
            if(src != des) {
                for(j=0; j<preload_copy_table.record[i].size; src++,des++) {
                    *des=*src;
                    j+=4;
                }
            }
        } else if(preload_copy_table.record[i].type == 0) {    //zero init
            for(j=0; j<preload_copy_table.record[i].size; des++) {
                *des=0;
                j+=4;
            }
        }
    }

#if CN_CPU_OPTIONAL_CACHE == 1
    Cache_CleanData();
    Cache_InvalidInst();
    Cache_config();
#endif

    Iboot_PreStart();
}

extern struct copy_table sysload_copy_table;

//----操作系统内核加载程序-----------------------------------------------------
//功能：加载所有操作系统内核代码，以及在si模式下全部应用程序代码。
//参数: 无。
//返回: 无。
//----------------------------------------------------------------------------
//备注: 本函数移植敏感，与开发系统有关，也与目标硬件配置有关
void Iboot_Loader(void)
{
   volatile  u32 *src,*des;
   volatile  u32 i, j;

    for(i=0; i<sysload_copy_table.record_cnt; i++) {
        src = (u32*) sysload_copy_table.record[i].load_start_address;
        des = (u32*) sysload_copy_table.record[i].run_start_address;
        if(sysload_copy_table.record[i].type == 1) {    //copy
            if(src != des) {
                for(j=0; j<sysload_copy_table.record[i].size; src++,des++) {
                    *des=*src;
                    j+=4;
                }
            }
        } else if(sysload_copy_table.record[i].type == 0) {    //zero init
            for(j=0; j<sysload_copy_table.record[i].size; des++) {
                *des=0;
                j+=4;
            }
        }
    }

#if CN_CPU_OPTIONAL_CACHE == 1
    Cache_CleanData();
    Cache_InvalidInst();
#endif
}

void Heap_StaticModuleInit(void);
void Int_Init(void);
void critical(void);
void SYS_Start(void);
void __libc_init_array(void);
void __libc_fini_array(void);
//----操作系统内核加载程序-----------------------------------------------------
//功能：加载所有操作系统内核代码，以及在si模式下全部应用程序代码。
//参数: 无。
//返回: 无。
//备注: 本函数移植敏感，与开发系统有关，也与目标硬件配置有关
//----------------------------------------------------------------------------
void Iboot_PreStart(void)
{
    extern void HardExp_Init(void);
    //自此可以调用malloc族函数，用准静态堆分配算法，若要释放内存，要求严格配对
    //调用malloc-free函数，或者不调用
    Heap_StaticModuleInit();
    HardExp_Init();
    Int_Init();
    critical();
    Iboot_Loader();
    //__libc_init_array和__libc_fini_array均由ld产生静态数组的方式调用全局构造和析构
    //函数，进适用于SI模式，动态加载需要重新设计。
    //只管到loader，没有涉及preloader，故紧急代码等不允许用c++写。
    __libc_init_array();
    SYS_Start();        //开始启动系统
    __libc_fini_array();
}

