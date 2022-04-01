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
//所属模块: djysh模块
//作者：wjz
//版本：V0.1.0
//文件描述: djyos的shell主模块，提供shell的实现过程以及内置命令
//修订历史:
// 1. 日期:2010-09-10之前
//   作者:lst
//   新版本号：djysh0.0.1
//     修改说明：shell实现的设计。
// 2. 日期:2011-06-15
//   作者:wjz
//   新版本号：djysh0.0.2
//   修改说明: 添加文件系统命令以及注册文件系统shell模块。
// 2. 日期:2011-09-24
//   作者:wjz
//   新版本号：djysh1.0.0
//   修改说明: 添加事件、事件类型、中断、内存、帮助等命令，
//               djysh1.0.0，可以作为一个简单完整的shell了。
// 3. 日期:2017-03-16
//   作者:
//   新版本号：
//   修改说明: 修改为查找符号表实现shell，
//------------------------------------------------------

#include <stdint.h>
#include <ctype.h>
#include <dbug.h>
#include <djyos.h>
#include <lock.h>
#include <shell.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <systime.h>
#include <unistd.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。
//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut);
//    Stdio_KnlInOutInit(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME);
//
//    extern s32 ModuleInstall_Shell(ptu32_t para);
//    ModuleInstall_Shell(0);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"shell"        //shell
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"stdio"            //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束
//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_SHELL == false )
//#warning  " shell  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_SHELL    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,0x10000,
#define CFG_SHELL_STACK            0x1000      //"执行shell命令的栈尺寸",
//%$#@enum,true,false,
#define CFG_ADD_ROUTINE_SHELL      true        //"是否添加常规shell命令",
#define CFG_ADD_EXPAND_SHELL       true        //"是否添加拓展shell命令",
#define CFG_ADD_GLOBAL_FUN         false       //"添加全局函数到shell",
#define CFG_SHOW_ADD_SHEELL        true        //"显示在编译窗口添加的shell命令",
//%$#@string,1,10,
//%$#@SYMBOL        ***不经配置界面，直接定义符号
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

struct
{
    u8 add_routine_shell;
    u8 add_expand_shell;
    u8 add_global_fun ;
    u8 show_add_sheell;
}const pt_shell_config __attribute__ ((section(".shellconfig"))) =
    {
     .add_routine_shell = CFG_ADD_ROUTINE_SHELL,
     .add_expand_shell= CFG_ADD_EXPAND_SHELL,
     .add_global_fun = CFG_ADD_GLOBAL_FUN,
     .show_add_sheell= CFG_SHOW_ADD_SHEELL,
    };
//=============通用函数调用参数与返回值由shell根据输入字符串解析汇编实现=========//
typedef void (*Ex_shell_func)(void);
//=============内部函数参数为字符串由函数本身解析参数========================//
typedef bool_t (*in_shell_func)(char *param);

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

enum commandtype
{
    Sh_Cmdtype_Fun,
    Sh_CmdtypeFun,
    Sh_Cmdtype_Date
};


struct sh_cmd_Tab
{
    char * const cmdname;
    struct shell_cmd *cmdaddr;
    #define    SH_CMDTYPE_ROUTINE_FUN         0   //常规sh函数
    #define    SH_CMDTYPE_ROUTINE_DATA        1   //常规sh数据
    #define    SH_CMDTYPE_EXPAND_FUN          2   //拓展shell函数
    #define    SH_CMDTYPE_EXPAND_DATA         3   //拓展shell数据
    #define    SH_CMDTYPE_GLOBAL_FUN          4   //全局函数
    #define    SH_CMDTYPE_GLOBAL_DATE         5   //全局数据
    int   cmdtype;
};


#define CN_SHELL_CMD_LIMIT 255 // shell 命令串长度限制
extern struct shellinfo p_shell_info; // p_shell_info 来自iboot.lds文件
static struct MutexLCB *__shell_mutex;//shell执行的互斥
static struct shell_list shells_list;

static union param ParameterTab[PARAMETER_MAX];
static enum param_typr ParameterFlagTab[PARAMETER_MAX];

// ============================================================================
// 功能：添加新的shell函数表到shell中, 加载app的时候像函数表中增加新的；
// 参数：
// 返回：
// 备注：
// ============================================================================
bool_t shell_add(struct shell_list *pLisTtab)
{
    Lock_MutexPend(__shell_mutex,CN_TIMEOUT_FOREVER);
    dListInsertBefore(&shells_list.list,&pLisTtab->list);
    Lock_MutexPost(__shell_mutex);
    return (TRUE);
}

// ============================================================================
// 功能：从shell函数链表中删除一个表
// 参数：
// 返回：
// 备注：
// ============================================================================
bool_t shell_del(struct shell_list *pLisTtab)
{
    Lock_MutexPend(__shell_mutex,CN_TIMEOUT_FOREVER);
    dListRemove(&pLisTtab->list);
    Lock_MutexPost(__shell_mutex);
    return TRUE;
}

// ============================================================================
// 功能：从shell的输入参数（字符串）中，提取出一个参数；
// 参数：input -- shell输入参数；
//      next -- 后续参数；
// 返回：剔除的输入参数；
// 备注：参数之间依靠空格作为分隔符；
//      提取出的参数，已将后面的分隔符换成串结束符'\0；
//      input的原始数据已发生变化；
// ============================================================================
char *shell_inputs(char *input, char **next)
{
    u32 i = 0;

    *next = NULL;
    if(!input)
        return (NULL);

    while(1)
    {
        if((input[i] == ' ') || (input[i] == 0))
        {
            if(input[i] == 0)
            {
                return (input);
            }
            else
            {
                input[i] = '\0';
                break;
            }
        }
        i++;
    }
    i++;
    while(input[i] != 0)
    {
        if(input[i]!=' ')
        {
            if((input[i] == '\n') || (input[i] == '\r'))
                *next = NULL;
            else
                *next = &input[i];

            break;
        }

        i++;
    }

    return input;
}

// ============================================================================
// 功能：二分法查找字符找不到返回-1找到返回在表格中的偏移
// 参数：
// 返回：
// 备注：
// ============================================================================
static  struct sh_cmd_Tab * __find(struct shell_list *p_Sh_List, const char *name)
{
    struct sh_cmd_Tab *pcmdtab = (struct sh_cmd_Tab *)p_Sh_List->info.sh_Tab_start;
    u32 left = 0;
    u32 right = (p_Sh_List->info.sh_Tab_end-p_Sh_List->info.sh_Tab_start)/sizeof(struct sh_cmd_Tab)-1;
    u32  mid = left + (right - left) / 2;;

    while( left < right)
    {
        int strflag = strcmp(pcmdtab[mid].cmdname, name);
        if( strflag < 0 )
        {
            left = mid + 1;
        }
        else if(strflag > 0 )
        {
            right = mid;
        }
        else
        {
            break;
        }
        mid =  (right + left)/2;
    }
    if(strcmp(pcmdtab[mid].cmdname, name)==0)
        return &pcmdtab[mid];
    return NULL;
}

// ============================================================================
// 功能：搜索命令, 根据cmdclass的名字补全其他信息.
// 参数：
// 返回：
// 备注：
// ============================================================================
static struct sh_cmd_Tab * __search_cmd(const char *cmdname)
{
    bool_t flag = false;
    struct shell_list *p_Sh_List;
    list_t *loc;
    struct sh_cmd_Tab * sh_cmd;


    p_Sh_List = &shells_list;
    do{
        sh_cmd = __find(p_Sh_List,cmdname);
        if(sh_cmd != NULL)
        {
           flag =true;
           break;
        }
        loc = dListGetAfter(&(p_Sh_List->list));
    }while(loc != &p_Sh_List->list);
    if(flag)
        return sh_cmd;
    return NULL;
}

// ============================================================================
// 功能：给变量赋值,根据dataclass 内容填充dataaddr
// 参数：data_class -- 字符串数据结构
//      DataAddr -- 被填充
// 返回：
// 备注：
// ============================================================================
static bool_t __variable_assignment(struct dataclass *data_class,union param *DataAddr)
{
    switch (data_class->datatype)
    {
         case flag_c   :
             DataAddr->data_c =(char)toascii(*(int *)data_class->datastring);
              break;
         case flag_u8  :
             DataAddr->data_u8 = (u8)strtoul(data_class->datastring,(char **)NULL,0);
              break;
         case flag_u16 :
             DataAddr->data_u16 = (u16)strtoul(data_class->datastring,(char **)NULL,0);
              break;
         case flag_u32 :
             DataAddr->data_u32= strtoul(data_class->datastring,(char **)NULL,0);
             break;
         case flag_u64 :
             DataAddr->data_u64 = strtoull(data_class->datastring,(char **)NULL,0);
              break;
         case flag_s8  :
             DataAddr->data_s8  = (s8)strtol(data_class->datastring,(char **)NULL,0);
             break;
         case flag_s16 :
             DataAddr->data_u16 = (s16)strtol(data_class->datastring,(char **)NULL,0);
             break;
         case flag_s32 :
             DataAddr->data_s32 = strtol(data_class->datastring,(char **)NULL,0);
             break;
         case flag_s64 :
             DataAddr->data_s64 = atoll(data_class->datastring);
             break;
         case flag_b   :
             if(strcmp(data_class->datastring,"true") || strcmp(data_class->datastring,"TRUE"))
                DataAddr->data_b = true;
             else if(strcmp(data_class->datastring,"false") || strcmp(data_class->datastring,"FALSE"))
                 DataAddr->data_b = false;
             break;
         case flag_f   :
             DataAddr->data_f = atof(data_class->datastring);
             break;
         case flag_d   :
             DataAddr->data_d = atoff(data_class->datastring);
             break;
         case flag_str  :
             //TODO 字符串拷贝过多容易出现越界 '\'未做转义字符检查。
             strcpy(DataAddr->data_pc,data_class->datastring);
             break;                    //不支持终端修改字符串
         default:     return false;
    }
    return true;
}

// ============================================================================
// 功能：打印输出变量
// 参数：
// 返回：
// 备注：
// ============================================================================
static bool_t __show_data(struct dataclass *data_class,union param *DataAddr)
{
    switch (data_class->datatype)
    {
         case flag_c   :
             printf("%d \n\r",DataAddr->data_c);
             break;
         case flag_u8  :
             printf("%d\n\r",*(u8*)DataAddr);
             break;
         case flag_u16 :
             printf("%d \n\r",*(u16*)DataAddr);
             break;
         case flag_u32 :
             printf("%d \n\r",*(u32*)DataAddr);
             break;
         case flag_u64 :
              printf("%llu \n\r",DataAddr->data_u64);
             break;
         case flag_s8  :
             printf("%d \n\r",*(s8*)DataAddr);
             break;
         case flag_s16 :
             printf("%d \n\r",*(s16*)DataAddr);
             break;
         case flag_s32 :
             printf("%d \n\r",*(s32*)DataAddr);
             break;
         case flag_s64 :
//             printf("%d \n\r",*(s64*)DataAddr);
             printf("%lld   \n\r",DataAddr->data_s64);
             break;
         case flag_b   :
             printf("%d \n\r",*(bool_t*)DataAddr);
             break;
         case flag_f   :
             printf("%f \n\r",*(float*)DataAddr);
             break;
         case flag_d   :
//             printf("%f \n\r",*(double*)DataAddr);
             printf("%f \n\r",DataAddr->data_d);

             break;
         case flag_str :
             printf("%s \r\n",DataAddr->data_pc);   //不支持打印：char test1[]="hello";   支持打印：char*p="hello";
             break;
         default:     return false;
    }
    return true;
}

// ============================================================================
// 功能：数据类型判断：根据字符串转换成类型标志
// 参数：
// 返回：
// 备注：
// ============================================================================
static enum param_typr __str2type(char *word)
{
    enum param_typr datatype;

    if(strcmp(word,"u8") == 0)
        datatype = flag_u8;
    else if(strcmp(word,"u16") == 0)
        datatype = flag_u16;
    else if(strcmp(word,"u32") == 0)
        datatype = flag_u32;
    else if(strcmp(word,"u64") == 0)
        datatype = flag_u64;
    else if(strcmp(word,"s8") == 0)
        datatype = flag_s8;
    else if(strcmp(word,"s16") == 0)
        datatype = flag_s16;
    else if(strcmp(word,"s32") == 0)
        datatype = flag_s32;
    else if(strcmp(word,"s64") == 0)
        datatype = flag_s64;
    else if(strcmp(word,"b") == 0)
        datatype = flag_b;
    else if(strcmp(word,"f") == 0)
        datatype = flag_f;
    else if(strcmp(word,"d") == 0)
        datatype = flag_d;
    else if((word[0] == '\'') && (word[strlen(word)-1] =='\'' ) && (strlen(word)==2))
        datatype = flag_c;
    else if((word[0] == '\"') && (word[strlen(word)-1] =='\"' ) )//""" == 0x22;
        datatype = flag_str;
    else
        datatype = flag_error;
    return datatype;
}

// ============================================================================
// 功能：根据参数的个数及参数类型调用汇编传递参数并调用函数
// 参数：num -- 参数的个数
//      fun -- 被调用函数的地址
// 返回：成功（TRUE）;失败（FALSE）；
// 备注：
// ============================================================================
static bool_t __asm_execute(u8 num,Ex_shell_func fun)
{
    extern void __asm_ExecuteCmd(union param *ptab,enum param_typr *pflag,Ex_shell_func fun,u32 num);
    __asm_ExecuteCmd(ParameterTab,ParameterFlagTab,fun,num);
    return true;
}


static bool_t shell_help(char *param)
{
    bool_t flag = true;
    struct shell_list *p_Sh_List;
    p_Sh_List = &shells_list;
    u32 idx;

    do{
        struct sh_cmd_Tab *pcmdtab = (struct sh_cmd_Tab *)p_Sh_List->info.sh_Tab_start;
        u32 cmdnum = (p_Sh_List->info.sh_Tab_end-p_Sh_List->info.sh_Tab_start)/sizeof(struct sh_cmd_Tab);

        printf("常规命令：\n\r");
        printf("%-20s  %-12s   %-20s\n\r","命令名","类型名","帮助");
        for(idx=0;idx<cmdnum;idx++)
        {
            if(pcmdtab[idx].cmdtype == SH_CMDTYPE_ROUTINE_FUN)
            {
                printf("%-20s ROUTINE SHELL   %-20s\n\r",pcmdtab[idx].cmdname,pcmdtab[idx].cmdaddr->shell_help_addr);
            }
            else if(pcmdtab[idx].cmdtype == SH_CMDTYPE_ROUTINE_DATA)
            {
                printf("%-20s  ROUTINE DATE \n\r",pcmdtab[idx].cmdname);
            }
        }
        printf("\n\r拓展命令：\n\r");
        for(idx=0;idx<cmdnum;idx++)
        {
            if(pcmdtab[idx].cmdtype == SH_CMDTYPE_EXPAND_FUN)
            {
                printf("%-20s EXPAND SHWLL   %-20s\n\r",pcmdtab[idx].cmdname,pcmdtab[idx].cmdaddr->shell_help_addr);
            }
            else if(pcmdtab[idx].cmdtype == SH_CMDTYPE_EXPAND_DATA)
            {
                printf("%-20s  EXPAND DATE \n\r",pcmdtab[idx].cmdname);
            }
        }

        p_Sh_List = (struct shell_list*)dListGetAfter(&(p_Sh_List->list));
    }while(&shells_list.list != &p_Sh_List->list);

    if(param)//消除告警
        return flag;
    return flag;
}
// ============================================================================
// 功能：分析并执行控制台命令
// 参数：
// 返回：
// 备注：
// ============================================================================
bool_t shell_exec_command(char *buf)
{
    bool_t result = false;
    char *cmdname,*next_param,*word,*wordbak;
    u8 i;
    struct dataclass data_class;
    in_shell_func inshfun;
    Ex_shell_func exshfun;
    bool_t help_flag = false;

    //串口限制读取255字符，在这里提示超长就行。
    if(strnlen(buf, CN_SHELL_CMD_LIMIT+1) > CN_SHELL_CMD_LIMIT)
    {
        printf("输入字符串太长\r\n");
        return false;
    }
    cmdname = shell_inputs(buf, &next_param);//提取函数或者变量名
    if(strcmp("help",cmdname)==0)//如果是帮助信息则拼接成新的名字
    {
        wordbak = shell_inputs(next_param,&next_param);
        if(wordbak != NULL)
        {
            cmdname = wordbak;
            help_flag = true;
        }
    }

    struct sh_cmd_Tab * sh_sym= __search_cmd(cmdname);
    if(sh_sym == NULL)
    {
        printf("命令 %s 不存在  !! \n\r",cmdname);
        return false;
    }

    switch (sh_sym->cmdtype)
    {
        case SH_CMDTYPE_ROUTINE_FUN :       //常规sh函数
            if(help_flag)
            {
                printf("%s", sh_sym->cmdaddr->shell_help_addr);
            }
            else
            {
                inshfun = (in_shell_func)sh_sym->cmdaddr->shell_fun_addr;
                result = inshfun(next_param);
                if(result == false)
                    printf("shell 内部函数执行错误 ！！\n\r");
            }
            break;
        case SH_CMDTYPE_ROUTINE_DATA:     //常规sh数据
        case SH_CMDTYPE_EXPAND_DATA :     //拓展shell数据
        case SH_CMDTYPE_GLOBAL_DATE :     //全局数据
            word = shell_inputs(next_param,&next_param);
            if(word == NULL)
            {
               printf("正确格式为：1、 DataName = 类型  num  2、 DataName  类型\r\n");
               printf("参数类型有：u8/u16/u32/u64/s8/s16/s32/s64/b/f/d/""/''\r\n");
                break;
             }

            if(strcmp(word,"=") == 0)//变量赋值
            {
                word = shell_inputs(next_param,&next_param);
                if(word != NULL)
                    data_class.datatype = __str2type(word);
                else
                    data_class.datatype = flag_error;

                word = shell_inputs(next_param,&next_param);
                wordbak = word;
                word = shell_inputs(next_param,&next_param);
                if((NULL != word) || (wordbak ==NULL)|| \
                        (data_class.datatype == flag_error))
                {
                    printf("正确格式为 DataName = 类型  num \n\r"\
                        "类型为 u8 u16 u32 s8 s16 s32 b f d "" ''\n\r");
                    return false;
                }
                data_class.datastring = wordbak;
                result = __variable_assignment(&data_class,(union param *)sh_sym->cmdaddr);
            }else
            {
                data_class.datatype = __str2type(word);
                if( data_class.datatype == flag_error)
                {
                    printf("shell 变量参数类型错误 ！！\n\r");
                    printf("参数类型有：u8/u16/u32/u64/s8/s16/s32/s64/b/f/d/""/''\r\n");
                    return false;
                }
                result = __show_data(&data_class,(union param *)sh_sym->cmdaddr);
            }
            break;
        case SH_CMDTYPE_EXPAND_FUN  :      //拓展shell函数
        case SH_CMDTYPE_GLOBAL_FUN  :     //全局函数
            if(sh_sym->cmdtype==SH_CMDTYPE_EXPAND_FUN)
            {
                if(help_flag)
                {
                    printf("%s",sh_sym->cmdaddr->shell_help_addr);
                    return true;
                }
                else
                {
                    exshfun = (Ex_shell_func)sh_sym->cmdaddr->shell_fun_addr;
                }
            }
            else
            {
                exshfun = (Ex_shell_func)sh_sym->cmdaddr;
            }

            for(i=1;i<PARAMETER_MAX;i++)
            {
                word = shell_inputs(next_param,&next_param);
                if(word != NULL)
                    ParameterFlagTab[i-1] = __str2type(word);
                else
                {
                    printf("shell 函数缺少参数类型 ！！\n\r");
                    printf("参数类型有：u8/u16/u32/u64/s8/s16/s32/s64/b/f/d\n\r");
                    break;
                }
                if(ParameterFlagTab[i-1] == flag_error)
                {
                    printf("shell 函数参数类型错误 ！！\n\r");
                    printf("参数类型有：u8/u16/u32/u64/s8/s16/s32/s64/b/f/d\n\r");
                    return false;
                }

                word = shell_inputs(next_param,&next_param);
                if(word != NULL)
                {
                    data_class.datastring = word;
                    data_class.datatype = ParameterFlagTab[i-1];
                    __variable_assignment(&data_class,&ParameterTab[i-1]);
                }
                else
                {
                    printf("shell 函数格式错误 ！！\n\r");
                    printf("shell函数格式为：命令名  类型1 参数1 类型2 参数2 类型3 参数3...\r\n");
                    return false;
                }

                if(next_param==NULL)
                       break;
                else if((next_param!=NULL)&&(i==(PARAMETER_MAX-1)))
                {
                     i=i+2; //要执行下面的return false;
                     break;
                }
            }
            if((i == PARAMETER_MAX) )
            {
                 printf("形参数量最多10个，不能超过10个\r\n");
                 return false;
            }
            result = __asm_execute(i,(Ex_shell_func)exshfun);
            break;
        default : break;

    }

    return result;
}

//because the stdin device could do no any more display work,so we must do all the
//display jobs here
//1,the telnet do the local display default,so it must do the nvt and not do the local echo
//2,the uart terminal should do no any display works here
//3,the keyboard should put all the information on the display
//the nvt rules:
//1,1B 5B 41 move the current cursor up
//2,1B 5B 42 move the current cursor down
//3,1B 5B 44 move the current cursor left
//4,1B 5B 43 move the current cursor right
//5,'\B' move the current cursor left
//6,1B delete all the input and make the following code transfer

//defines for the virtual key:0x1B + UKEY +VKEY
#define CN_VK_NULL         (0x000000)    //NULL   CODE
#define CN_VK_ARROWU       (0X41001B)    //UP     ARROW
#define CN_VK_ARROWD       (0X42001B)    //DOWN   ARROW
#define CN_VK_ARROWR       (0X43001B)    //RIGHT  ARROW
#define CN_VK_ARROWL       (0X44001B)    //LEFT   ARROW
#define CN_VK_LF           (0X00000D)    //ENTER
#define CN_VK_CR           (0X00000A)    //NEW LINE
#define CN_VK_TAB          (0X000009)    //TABLE
#define CN_VK_BS           (0X000008)    //BACK SPACE
#define CN_VK_ES           (0X00001B)    //ESPACE

//i think each editor has its own meaning for the control character
//the shell decode the control character as the following:
//the executed command is the ring table,you could use the up and down key for the auto complete
//up key:show the last command before the last command
//down key:show the command after the last command

//should read each character from the stdin,if '\n',then end the input here,do the command execute
//if we get the control character, then decode it
//because too much hook function will be executed here, and we don't know how deep the hook function is,so we just make the
//memory resource file global
#define CN_CMDLEN_MAX  64  //i don't think you could type more world than this,if not,maybe make user too tired
#define CN_CMD_CACHE   8   //you could use the up and down key to execute the history command
typedef struct
{
    char  curcmd[CN_CMDLEN_MAX];               //ended with '\0'
    u8    curoffset;                           //point to the position to be edited,the current position to be written
    char  tab[CN_CMD_CACHE][CN_CMDLEN_MAX];    //cache the history command
    u8    taboffset;                           //point to which cache has been used
}tagShellBuffer;
static void putsbackspace(int times)
{
    char ch;
    int i =0;
    for(i =0;i<times;i++)
    {
        ch ='\b';  //move the cursor left
        putc(ch,stdout);
        ch =' ';   //space key
        putc(ch,stdout);
        ch ='\b';  //move the cursor left
        putc(ch,stdout);
    }
}
static void putsnxtspace(int times)
{
    char ch;
    int i =0;
    ch =' ';  //just put the space
    for(i =0;i<times;i++)
    {
        putc(ch,stdout);
    }
}
//absolutely,just put the right cursor,maybe different terminal has different right virtual key
static void  movescursorright(int times,u32 vk)
{
    char ch;
    int i =0;
    for(i =0;i<times;i++)
    {
        ch = (char)(vk&0xff);
        putc(ch,stdout);
        ch = (char)((vk>>8)&0xff);
        putc(ch,stdout);
        ch = (char)((vk>>16)&0xff);
        putc(ch,stdout);
    }
}
//absolutely,just put the left cursor,maybe different terminal has different right virtual key
static void  movescursoleft(int times)
{
    char ch ='\b';  //move the cursor left
    int i =0;
    for(i =0;i<times;i++)
    {
        putc(ch,stdout);
    }
}

//-----------------------------------------------------------------------------
//功能: 返回console输入的字符，带console输入回车符时，执行命令。一次命令不得超过
//      255字符。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
static ptu32_t Sh_Service(void)
{
     u8 ch;
     int len;
     u8 offset;
     const char *cmdindex;
     char *cwd;
     u32 vk = CN_VK_NULL;    //used when we push back the right key
     u32 vkmask = CN_VK_NULL;
     tagShellBuffer cmdbuf;
     s32 res;

     memset(&cmdbuf,0,sizeof(cmdbuf));
     cwd = getcwd(NULL,0);
     printf("\n\r%s>", cwd);   //push the index character to the terminal
     free(cwd);

     while(1)
     {
         res = getc(stdin);
         if(EOF == res)
         {
             DJY_EventDelay(1000); // 获取数据错误或者end of file，延时1ms再继续（防止出现死循环现象，导致其他线程卡死）。
             continue;
         }

         ch = (u8)res;
         if((ch == CN_VK_NULL)||(ch == 0xFF))
         {
             continue;   //NO CODE GET HERE
         }

         if((vk&0xFF) != 0)  //get the transfer code before
         {
             if(((vk>>8)&0xff) == CN_VK_NULL)  //this maybe the base code
             {
                 vk|=(ch<<8); //get the base code
                 continue;    //continue to get the vk:three bytes to decode for the escape key
             }
             else  //this is the vk code
             {
                 vk |= (ch <<16);
                 vkmask |= (ch <<16);
             }
         }
         else
         {
             vkmask  = ch;
         }
         switch (vkmask)
         {
             case CN_VK_ARROWL:        //left,make the cursor moved left
                 if(cmdbuf.curoffset > 0)//if not,do nothing here
                 {
                     cmdbuf.curoffset--;
                     //push back the left cursor and make the terminal display know what has happened
                     movescursoleft(1);
                 }
                 //flush the vk
                 vk = CN_VK_NULL;
                 vkmask = CN_VK_NULL;
                 break;
             case CN_VK_ARROWR:        //right,make the cursor moved right,if the buffer has the data
                 len = strlen(cmdbuf.curcmd);
                 if(cmdbuf.curoffset < len)
                 {
                     cmdbuf.curoffset++;
                     //push back the right cursor and make the terminal display know what has happened
                     movescursorright(1,vk);
                 }
                 //flush the vk
                 vk = CN_VK_NULL;
                 vkmask = CN_VK_NULL;
                 break;
             case CN_VK_ARROWU:        //moves to the previous command
                 //first,we should clear what we has get,do the reset current command
                 //first moves to the left,then putsnxt to the next to the right,then back
                 len = strlen(cmdbuf.curcmd);
                 if(cmdbuf.curoffset < len)
                 {
                     putsnxtspace(len - cmdbuf.curoffset);
                     cmdbuf.curoffset = len;
                 }
                 putsbackspace(len);
                 memset(cmdbuf.curcmd,0,CN_CMDLEN_MAX);
                 cmdbuf.curoffset = 0;
                 //then copy the previous command to current and echo all the info
                 offset = (cmdbuf.taboffset +CN_CMD_CACHE -1)%CN_CMD_CACHE;
                 strncpy(cmdbuf.curcmd,cmdbuf.tab[offset],CN_CMDLEN_MAX);
                 cmdbuf.taboffset = offset;
                 cmdbuf.curoffset = strlen(cmdbuf.curcmd);
                 //OK,now puts all the current character to the terminal
                 puts(cmdbuf.curcmd);
                 //flush the vk
                 vk = CN_VK_NULL;
                 vkmask = CN_VK_NULL;
                 break;
             case CN_VK_ARROWD:        //moves to the next command
                 //first,we should clear what we has get,do the reset current command
                 //first moves to the right,then backspace all the input
                 len = strlen(cmdbuf.curcmd);
                 if(cmdbuf.curoffset < len)
                 {
                     putsnxtspace(len - cmdbuf.curoffset);
                     cmdbuf.curoffset = len;
                 }
                 putsbackspace(len);
                 memset(cmdbuf.curcmd,0,CN_CMDLEN_MAX);
                 cmdbuf.curoffset = 0;
                 //then copy the next command to current
                 offset = (cmdbuf.taboffset +CN_CMD_CACHE +1)%CN_CMD_CACHE;
                 strncpy(cmdbuf.curcmd,cmdbuf.tab[offset],CN_CMDLEN_MAX);
                 cmdbuf.taboffset = offset;
                 cmdbuf.curoffset = strlen(cmdbuf.curcmd);
                 //OK,now puts all the current character to the terminal
                 puts(cmdbuf.curcmd);
                 //flush the vk
                 vk = CN_VK_NULL;
                 vkmask = CN_VK_NULL;
                 break;
             case CN_VK_TAB: // tab,should auto complete the command TODO:未实现；
                 //should search the command we has installed if some index has get
                 len = strlen(cmdbuf.curcmd);
                 if(len > 0)  //some index has get
                 {

                     {
                         cmdindex = NULL; // 功能待实现；
                     }

                     if(NULL != cmdindex) //get the corresponding command here
                     {
                         //first,we should clear what we has get,do the reset current command
                         //first moves to the right,then backspace all the input
                         len = strlen(cmdbuf.curcmd);
                         if(cmdbuf.curoffset < len)
                         {
                             putsnxtspace(len - cmdbuf.curoffset);
                             cmdbuf.curoffset = len;
                         }
                         putsbackspace(len);
                         memset(cmdbuf.curcmd,0,CN_CMDLEN_MAX);
                         cmdbuf.curoffset = 0;
                         //then copy the command and puts the string here
                         strncpy(cmdbuf.curcmd,cmdindex,CN_CMDLEN_MAX);
                         cmdbuf.curoffset = strlen(cmdbuf.curcmd);
                         //OK,now puts all the current character to the terminal
                         puts(cmdbuf.curcmd);
                     }
                 }
                 //flush the vk
                 vk = CN_VK_NULL;
                 vkmask = CN_VK_NULL;
                 break;
             case CN_VK_LF:      //execute the command here, and push the command to the history cache
                 if(strlen(cmdbuf.curcmd) != 0)
                 {
                     printf("\n\r");
                     //copy the current to the history cache if the current command is not none
                     len = strlen(cmdbuf.curcmd);
                     if(len > 0)
                     {
                         offset=cmdbuf.taboffset;
                         memset(cmdbuf.tab[offset],0,CN_CMDLEN_MAX);
                         strncpy(cmdbuf.tab[offset],cmdbuf.curcmd,CN_CMDLEN_MAX);
                         offset = (offset +1)%CN_CMD_CACHE;
                         cmdbuf.taboffset = offset;
                     }

                     //execute the command
                     shell_exec_command(cmdbuf.curcmd);

                     memset(cmdbuf.curcmd,0,CN_CMDLEN_MAX);
                     cmdbuf.curoffset = 0;
                 }
                 cwd = getcwd(NULL,0);
                 printf("\n\r%s>", cwd);   //push the index character to the terminal
                 free(cwd);
                 //flush the vk
                 vk = CN_VK_NULL;
                 vkmask = CN_VK_NULL;
                 break;
             case CN_VK_CR:      //execute the command here, and push the command to the history cache
                 if(strlen(cmdbuf.curcmd) != 0)
                 {
                     printf("\n\r");
                     //copy the current to the history cache if the current command is not none
                     len = strlen(cmdbuf.curcmd);
                     if(len > 0)
                     {
                         offset=cmdbuf.taboffset;
                         memset(cmdbuf.tab[offset],0,CN_CMDLEN_MAX);
                         strncpy(cmdbuf.tab[offset],cmdbuf.curcmd,CN_CMDLEN_MAX);
                         offset = (offset +1)%CN_CMD_CACHE;
                         cmdbuf.taboffset = offset;
                     }

                     //execute the command
                     shell_exec_command(cmdbuf.curcmd);

                     memset(cmdbuf.curcmd,0,CN_CMDLEN_MAX);
                     cmdbuf.curoffset = 0;
                 }
                 cwd = getcwd(NULL,0);
                 printf("\n\r%s>", cwd);   //push the index character to the terminal
                 free(cwd);
                 //flush the vk
                 vk = CN_VK_NULL;
                 vkmask = CN_VK_NULL;
                 break;
             case CN_VK_BS:      //should delete the current character,move all the following character 1 position to before
                 if(cmdbuf.curoffset >0)
                 {
                     cmdbuf.curcmd[cmdbuf.curoffset] = 0;
                     cmdbuf.curoffset--;
                     putsbackspace(1);
                 }
                 //flush the vk
                 vk = CN_VK_NULL;
                 vkmask = CN_VK_NULL;
                 break;
             case CN_VK_ES:      //esc key,delete all the input here
                 len = strlen(cmdbuf.curcmd);
                 if(cmdbuf.curoffset < len)
                 {
                     putsnxtspace(len - cmdbuf.curoffset);
                     cmdbuf.curoffset = len;
                 }
                 putsbackspace(len);
                 memset(cmdbuf.curcmd,0,CN_CMDLEN_MAX);
                 cmdbuf.curoffset = 0;
                 //this is also the transfer code
                 vk= CN_VK_ES;
                 vkmask = CN_VK_ES;
                 break;
             default: //other control character will be ignored
                 //push the character to the buffer until its full and the '\n' comes
                 if((cmdbuf.curoffset <(CN_CMDLEN_MAX-1))&&(isprint((int)ch)))//the last will be'\0'
                 {
                     cmdbuf.curcmd[cmdbuf.curoffset] = ch;
                     cmdbuf.curoffset++;
                     putc(ch,stdout);   //should do the echo
                 }
                 //flush the vk
                 vk = CN_VK_NULL;
                 vkmask = CN_VK_NULL;
                 break;
         }
     }
    return 0;
}

// ============================================================================
// 功能：shell模块初始化
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 ModuleInstall_Shell(ptu32_t para)
{
    u16 shell_evtt;

    dListInit(&shells_list.list);
    shells_list.info =  p_shell_info;
    if((u32)p_shell_info.sh_Tab_start < (u32)&p_shell_info)
    {
        printf("\n\r error :  未添加shell信息 shell 加载失败 !!\n\r");
        return false;
    }
    __shell_mutex = Lock_MutexCreate("shell");
    if(NULL == __shell_mutex)
        return (-1);

    shell_evtt = DJY_EvttRegist(EN_CORRELATIVE, // 关联型事件
                                2, // 默认优先级
                                0, // 线程保留数，关联型无效
                                0, // 线程上限，关联型无效
                                Sh_Service, // 入口函数
                                NULL, // 由系统分配栈
                                CFG_SHELL_STACK,
                                "shell" // 事件类型名
                                );
    if(shell_evtt==CN_EVTT_ID_INVALID)
    {
        Lock_MutexDelete(__shell_mutex);
        error_printf("module", "shell install failed.");
        return (-1);
    }

    if(DJY_EventPop(shell_evtt, NULL, 0, 0, 0, 0) == CN_EVENT_ID_INVALID)
    {
        DJY_EvttUnregist(shell_evtt);
        return (-1);
    }

    return (para);
}
ADD_TO_ROUTINE_SHELL(help,shell_help,"帮助信息格式 :help [cmd]");
