//----------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 著作权由都江堰操作系统开源团队所有。著作权人保留一切权利。
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
#include "shell_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

//=============通用函数调用参数与返回值由shell根据输入字符串解析汇编实现=========//
typedef void (*Ex_shell_func)(void);
//=============内部函数参数为字符串由函数本身解析参数========================//
typedef bool_t (*in_shell_func)(char *param);

struct shellinfo
{
   const char *In_funTab_start; //
   const char *In_funTab_end; //
   const char *Ex_funTab_start;
   const char *Ex_funTab_end;
   const char *dataTab_start;
   const char *dataTab_end;
};

struct shell_list
{
    list_t list;
    struct shellinfo info;
};

enum param_typr
{
    flag_u8,
    flag_u16,
    flag_u32,
    flag_u64,
    flag_s8,
    flag_s16,
    flag_s32,
    flag_s64,
    flag_b,
    flag_f,
    flag_d,
    flag_c,
    flag_str,
    flag_error
};

union param{
    u8 data_u8;
    u16 data_u16;
    u32 data_u32;
    u64 data_u64;
    s8  data_s8;
    s16 data_s16;
    s32 data_s32;
    s64 data_s64;
    bool_t data_b;
    float data_f;
    double data_d;
    char data_c;
    char *data_pc;
};

struct dataclass
{
    char  *datastring; // 参数对应的字符串如 "100";
    enum param_typr datatype; // 命令对应的类型
};

struct inshell_func
{
    const char *const fun_name; //内部shell函数名字
    const in_shell_func const fun_addr;//内部函数地址
    const char **const help;//帮助信息
};

struct exshell_func
{
    const char *const fun_name; // 普通shell名字
    const void * const fun_addr; //
};

struct data_struct
{
    const char *const data_name;
    void *const data;
};

enum commandtype
{
    Sh_Cmdtype_Fun,
    Sh_CmdtypeFun,
    Sh_Cmdtype_Date
};

struct commandclass
{
    char *cmdname; // 命令对应的名字
    enum commandtype cmdtype; // 命令对应的类型
    void *cmdaddr; // 命令对应的地址
    char *cmdhelp;
};

#define PARAMETER_MAX     (10)   //最大参数个数限制
#define ADD_TO_IN_SHELL  __attribute__((section(".in_shell_cmd")))
#define ADD_TO_EX_SHELL   __attribute__((section(".ex_shell_cmd")))
#define ADD_TO_SHELL_HELP(cmdname,help)  __attribute__((section(".in_shell_cmd_help"))) \
                                         const char *cmdname##_help = help

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

ptu32_t ModuleInstall_NewSh(ptu32_t para);
#ifdef __cplusplus
}
#endif

#endif //__SHELL_H__

