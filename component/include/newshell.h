//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
//所属模块:时钟模块
//作者:  王建忠
//版本：V1.0.0
//文件描述: 为系统提供时钟
//其他说明:
//修订历史:
// 1. 日期: 2011-10-28
//   作者:  王建忠
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#ifndef __NEWSHWLL_H__
#define __NEWSHELL_H__

#include "stdint.h"
#include "errno.h"
#include <types.h>
#include <stddef.h>
#include "list.h"
#ifdef __cplusplus
extern "C" {
#endif



//=============通用函数调用参数与返回值由shell根据输入字符串解析汇编实现=========//
typedef void (*Ex_shell_func)(void);
//=============内部函数参数为字符串由函数本身解析参数========================//
typedef bool_t (*in_shell_func)(char *param);

struct In_FunTab
{
    const char *const fun_name; //内部shell函数名字
    const in_shell_func const fun_addr;//内部函数地址
    const char **const help;//帮助信息
};
struct Ex_FunTab
{
    const char *const fun_name;//普通shell名字
    const void * const fun_addr;//
};
struct DateTab
{
    const char *const data_name;
    void *const pData;
};

enum Cmd_type
{
 Sh_Cmdtype_Fun,
 Sh_CmdtypeFun,
 Sh_Cmdtype_Date
};
static struct CmdClass
{
    char          *cmdname;//命令对应的名字
    enum Cmd_type cmdtype; //命令对应的类型
    void  *cmdaddr;        //命令对应的地址
    char * cmd_help;
};

struct shellinfo
{
   const char *In_funTab_start;
   const char *In_funTab_end;
   const char *Ex_funTab_start;
   const char *Ex_funTab_end;
   const char *dataTab_start;
   const char *dataTab_end;

};
struct ShellListTab
{
    list_t Node;
    struct shellinfo info;
};

#define PARAMETER_MAX     (10)   //最大参数个数限制

static union Parameter{
 u8     data_u8;
 u16    data_u16;
 u32    data_u32;
 u64    data_u64;
 s8     data_s8;
 s16    data_s16;
 s32    data_s32;
 s64    data_s64;
 bool_t data_b;
 float  data_f;
 double data_d;
 char   data_c;
 char  *data_pc;
}ParameterTab[PARAMETER_MAX];

static enum ParameterTypr
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
}ParameterFlagTab[PARAMETER_MAX];

static struct DataClass
{
    char          *datastring;   //参数对应的字符串如 "100";
    enum ParameterTypr datatype; //命令对应的类型
};


#define ADD_TO_IN_SHELL  __attribute__((section(".in_shell_cmd")))
#define ADD_TO_EX_SHELL   __attribute__((section(".ex_shell_cmd")))
#define ADD_TO_SHELL_HELP(cmdname,help)  __attribute__((section(".in_shell_cmd_help"))) \
		 const char *cmdname##_help = help



bool_t Sh_InstallTab(struct ShellListTab *pLisTtab);
bool_t Sh_uninstallTab(struct ShellListTab *pLisTtab);
char *NewSh_GetWord(char *buf,char **next);
static bool_t Sh_find(struct ShellListTab *p_Sh_List,struct CmdClass *cmdclass);
static bool_t Sh_SearchCmd(struct CmdClass *cmdclass);

ptu32_t ModuleInstall_NewSh(ptu32_t para);
//void Sh_PrintWorkPathSet(void);

extern void (*fng_pPrintWorkPath)(void);





#ifdef __cplusplus
}
#endif

#endif

