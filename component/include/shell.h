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

#ifndef __SHELL_H__
#define __SHELL_H__

#include <stdint.h>
#include <errno.h>
#include <types.h>
#include <stddef.h>
#include <list.h>

#ifdef __cplusplus
extern "C" {
#endif

struct shellinfo
{
   const char *sh_Tab_start; //
   const char *sh_Tab_end; //
};

struct shell_list
{
    list_t list;
    struct shellinfo info;
};
#define PARAMETER_MAX     (10)   //最大参数个数限制

/* 常规shell函数与拓展shell函数区别在于，参数的解析发生在函数内还是函数外
 * 常规shell函数帮助信息与常规shell函数变量主要是与裁减有关，在工具中指定添加哪些类型的shell
 * */
/*shell 和拓展shell*/

struct shell_cmd
{
    void *shell_fun_addr;
    char *shell_help_addr;
};
#define DJYSH_HELP_NAME "djysh_"

#if(CFG_MODULE_ENABLE_SHELL == true)
#define ADD_TO_ROUTINE_SHELL(cmdname,fun,help) \
                __attribute__((section(".ro_shell_cmd")))\
                const struct shell_cmd djysh_##cmdname = \
                        {.shell_fun_addr  = fun,.shell_help_addr = help,}

#define ADD_TO_EXPAND_SHELL(cmdname,fun,help) \
                __attribute__((section(".ex_shell_cmd")))\
                const struct shell_cmd djysh_##cmdname = \
                        {.shell_fun_addr  = fun,.shell_help_addr = help,}

//用法：“ADD_TO_IN_SHELL_DATA u32 sh;”      //把 sh 定义为可在shell中访问的变量
#define ADD_TO_IN_SHELL_DATA  __attribute__((section(".ro_shell_data")))//添加常规shell数据
#define ADD_TO_EX_SHELL_DATA    __attribute__((section(".ex_shell_data")))//拓展shell数据

#else   //for #if(CFG_MODULE_ENABLE_SHELL == true)

//当没有勾选shell组件，这些宏全部置空
#define ADD_TO_ROUTINE_SHELL(cmdname,fun,help)
#define ADD_TO_EXPAND_SHELL(cmdname,fun,help)
#define ADD_TO_IN_SHELL_DATA
#define ADD_TO_EX_SHELL_DATA

#endif   //for #if(CFG_MODULE_ENABLE_SHELL == true)

// ============================================================================
// 功能：从shell的输入参数（字符串）中，提取出一个参数；
// 参数：input -- shell输入参数；
//      next -- 后续参数；
// 返回：剔除的输入参数；
// 备注：参数之间依靠空格作为分隔符；
//      提取出的参数，已将后面的分隔符换成串结束符'\0；
//      input的原始数据已发生变化；
// ============================================================================
char *shell_inputs(char *input, char **next);

bool_t shell_add(struct shell_list *pLisTtab);
bool_t shell_del(struct shell_list *pLisTtab);

s32 ModuleInstall_Shell(ptu32_t para);

#ifdef __cplusplus
}
#endif

#endif   //for #ifndef __SHELL_H__

