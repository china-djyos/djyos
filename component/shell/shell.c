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
//------------------------------------------------------
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "object.h"
#include "djyos.h"
#include "hmi-input.h"
#include "shell.h"
#include "version.h"
#include "time.h"
#include "systime.h"
#include <ctype.h>

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_Sh(void);
//    ModuleInstall_Sh();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"shell"        //shell
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:核心组件             //选填“第三方组件、核心组件、bsp组件、用户组件”，本属性用于在IDE中分组
//select:可选                   //选填“必选、可选、不可选”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//grade:init                    //初始化时机，可选值：none，init，main。none表示无须初始化，
                                //init表示在调用main之前，main表示在main函数中初始化
//dependence:"stdio"            //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
//%$#@end configue  ****参数配置结束
//@#$%component end configure


bool_t Sh_ListRscSon(char *param);
bool_t Sh_ListRscTree(char *param);
void Sh_ListRscAll(void);
void __Sh_ShowSon(struct Object *branche);
void __Sh_ShowBranche(struct Object *branche);
bool_t Sh_ShowMemory(char *param);
bool_t Sh_FillMemory(char *param);
bool_t Sh_CmdParser(const char *buf,const char *keyword,char **param);
void Sh_CmdHelp(void);
bool_t Sh_ListCmdHelp(char *param);
bool_t Sh_Ver(char *param);
bool_t Sh_Date(char *param);
bool_t Sh_Time(char *param);
bool_t Sh_ExecCommand(char *cmd);
bool_t Sh_UninstallCmdByName(char *param);
bool_t Sh_SetDate(char *param);
bool_t Sh_SetTime(char *param);

ptu32_t Sh_Service(void);

void (*fng_pPrintWorkPath)(void) = NULL;
bool_t (*fng_pCD_PTT)(const char *PTT_Name) = NULL;

extern bool_t Sh_PrintWorkPath(void);
extern bool_t ShowCpuInfo(char *param);
//内置命令表,包含系统自带的命令
struct ShellCmdTab const shell_cmd_table[] =
{
    {
        "rsc-tree",
        Sh_ListRscTree,
        "列出系统中的资源名字和层次关系",
        "格式: rsc_tree TreeName,省略TreeName则列出全部资源"
    },
    {
        "rsc-son",
        Sh_ListRscSon,
        "列出子资源",
        "格式: rsc_tree RscName,省略RscName则列出根资源"
    },
    {
        "d",
        Sh_ShowMemory,
        "读取内存里的数据",
        "命令格式:d 地址 单元数 每单元字节数"
    },
    {
        "f",
        Sh_FillMemory,
        "写数据到内存",
        "命令格式：f 起始地址  单元数 每单元字节数 填充内容"
    },
    {
        "help",
        Sh_ListCmdHelp,
        "DJYOS的命令帮助信息",
        NULL
    },
    {
        "ver",
        Sh_Ver,
        "显示djyos的版本",
        NULL
    },
    {
        "date",
        Sh_Date,
        "显示或者设置日期",
        NULL
    },
    {
        "time",
        Sh_Time,
        "显示当前时间或者设置输入新时间",
        NULL
    },

    {
        "settime",
        Sh_SetTime,
        "Diaplay or set current time",
        "命令格式:settime year/month/day, hour:min:sec+Enter"
    },

    {
        "uninstall-cmd",
        Sh_UninstallCmdByName,
        "删除shell命令--用名字",
        NULL
    },
    {
        "cpuinfo",
        ShowCpuInfo,
        "usage:cpuinfo:show the cpu",
        NULL
    },
};

static struct ShellCmdRsc tg_shell_cmd_rsc
                        [sizeof(shell_cmd_table)/sizeof(struct ShellCmdTab)];
static struct Object *s_ptShellCmdDir;

//----当前工作路函数指针赋值-----------------------------------------------------
//功能: 函数指针赋值，该指针指向获取当前工作路径的函数
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void Sh_PrintWorkPathSet(void)
{
    fng_pPrintWorkPath = (void*)Sh_PrintWorkPath;
}


extern ptu32_t Sh_ServiceNew(void);

//----调试模块初始化函数-------------------------------------------------------
//功能: 初始化调试模块
//参数: 无
//返回: true = 成功，false = 失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_Sh(void)
{
    u16 shell_evtt;
    u32 loop;
    shell_evtt = Djy_EvttRegist(EN_CORRELATIVE,  //关联型事件
                                CN_PRIO_REAL,       //默认优先级
                                0,                  //线程保留数，关联型无效
                                0,                  //线程上限，关联型无效
//                                1,                  //参数队列长度上限
                                Sh_ServiceNew,         //入口函数
                                NULL,               //由系统分配栈
                                0x1000,             //栈尺寸
                                "shell"             //事件类型名
                                );
    if(shell_evtt == CN_EVTT_ID_INVALID)
        return false;
    s_ptShellCmdDir = OBJ_AddChild(OBJ_Root(), NULL, 0, "shell cmd list");
    if(s_ptShellCmdDir == NULL)
    {
        Djy_EvttUnregist(shell_evtt);
    }
    if(Djy_EventPop(shell_evtt,NULL,0,0,0,0) == CN_EVENT_ID_INVALID)
    {
        Djy_EvttUnregist(shell_evtt);
        OBJ_Del(s_ptShellCmdDir);
        return false;
    }
    for(loop = 0;loop<sizeof(shell_cmd_table)/sizeof(struct ShellCmdTab);loop++)
    {
        tg_shell_cmd_rsc[loop].shell_cmd_func
                                    = shell_cmd_table[loop].shell_cmd_func;
        tg_shell_cmd_rsc[loop].help_hint
                                    = shell_cmd_table[loop].help_hint;
        tg_shell_cmd_rsc[loop].help_detailed
                                    = shell_cmd_table[loop].help_detailed;
        OBJ_AddChild(s_ptShellCmdDir, NULL, (ptu32_t)&tg_shell_cmd_rsc[loop],
                                (const char*)(shell_cmd_table[loop].cmd));
    }
    return true;
}

//----添加shell命令------------------------------------------------------------
//功能: 添加用户的shell命令，由于si版本是一个二进制的可执行影像，不含符号表，不
//      能通过shell直接调用函数，必须把新命令添加到命令表中。在dlsp版本中，可以
//      通过直接调用函数的方式实现，但用命令表的方式，仍然有速度快的优势。
//参数: cmd_tab，命令表指针，可参照shell_cmd_table的方式定义
//      cmd_rsc，资源节点指针，可参照tg_shell_cmd_rsc的方式定义
//      cmd_num, 新添加的表中包含的命令数
//返回: true = 成功。
//      false= 失败，可能是参数错误，也可能是命令表中包含重名或与现有命令重名的
//          命令，如果是重名，则不重名的命令仍然正常加入命令表
//-----------------------------------------------------------------------------
bool_t Sh_InstallCmd(struct ShellCmdTab const *cmd_tab,
                      struct ShellCmdRsc *cmd_rsc,u32 cmd_num)
{
    u32 loop;
    bool_t result = true;
    if( (cmd_tab == NULL) || (cmd_rsc == NULL) || (cmd_num == 0))
        return result;

    for(loop = 0; loop < cmd_num;loop++)
    {
        if(OBJ_SearchChild(s_ptShellCmdDir,(const char*)(cmd_tab[loop].cmd)) == NULL)
        {
            cmd_rsc[loop].shell_cmd_func = cmd_tab[loop].shell_cmd_func;
            cmd_rsc[loop].help_hint = cmd_tab[loop].help_hint;
            cmd_rsc[loop].help_detailed = cmd_tab[loop].help_detailed;
            OBJ_AddChild(s_ptShellCmdDir, NULL, (ptu32_t)&cmd_rsc[loop],(const char*)(cmd_tab[loop].cmd));
        }
        else
        {
            result = false;
        }
    }
    return result;
}
//----卸载shell命令------------------------------------------------------------
//功能: 卸载用户shell命令
//参数: cmd_tab，命令表指针，可参照shell_cmd_table的方式定义,cmd_num，表的大小
//返回: 成功卸载的个数
//-----------------------------------------------------------------------------
u32 Sh_UninstallCmdByTab(struct ShellCmdTab const *cmd_tab,u32 cmd_num)
{
    u32 loop;
    u32 result = 0;
    struct Object *tagShellCmdRsc;
    if( (cmd_tab == NULL) || (cmd_num == 0))
        return result;

    for(loop = 0; loop < cmd_num;loop++)
    {
        tagShellCmdRsc = OBJ_SearchChild(s_ptShellCmdDir,(const char*)(cmd_tab[loop].cmd));
        if(NULL != tagShellCmdRsc)
        {
            OBJ_Del(tagShellCmdRsc);
            result ++;
        }

    }
    return result;
}
bool_t Sh_UninstallCmdByName(char *param)
{
    bool_t result = 0;
    struct Object *tagShellCmdRsc;
    if(NULL == param)
    {
        return false;
    }

    tagShellCmdRsc = OBJ_SearchChild(s_ptShellCmdDir,(const char *)param);
    if(NULL != tagShellCmdRsc)
    {
        OBJ_Del(tagShellCmdRsc);
        result = true;
    }
    else
    {
        result = false;
    }
    return result;
}

//----提取单词---------------------------------------------------------------
//功能: 从buf中提取一个由空格或行结束符隔开的单词，next用于返回下一个单词首地址，
//      如果没有下一个单词，则next=NULL。
//参数: buf，待分析的字符串
//      next，返回下一个单词指针
//返回: 提取的单词指针，已将单词后面的分隔符换成串结束符'\0'
//-----------------------------------------------------------------------------
char *Sh_GetWord(char *buf,char **next)
{
    uint32_t i=0;
    *next = NULL;
    if(buf == NULL)
        return NULL;
    while(1)
    {
        if((buf[i] == ' ') || (buf[i] == 0))
        {
            if(buf[i] == 0)
                return buf;
            else
            {
                buf[i] = '\0';
                break;
            }
        }
        i++;
    }
    i++;
    while(buf[i] != 0)
    {
        if(buf[i]!=' ')
        {
            if((buf[i] == '\n') || (buf[i] == '\r'))
                *next = NULL;
            else
                *next = &buf[i];
            break;
        }
        i++;
    }
    return buf;
}

//----显示子资源----------------------------------------------------------
//功能: 按参数要求显示资源队列中资源的名字，若该资源名字为空，则显示"无名资源"
//参数: 参数字符串，待列出的父资源名，省却则列出根结点下的资源
//返回: true
//-----------------------------------------------------------------------------
bool_t Sh_ListRscSon(char *param)
{
    struct Object *rsc_tree;
    if(param == NULL)
    {
        rsc_tree = OBJ_Root();
    }
    else
    {
        rsc_tree = OBJ_Search(OBJ_Root(),(const char*)param);
        if(rsc_tree == NULL)
        {
            printf("没找到 %s 资源\r\n",param);
        }
    }
    __Sh_ShowSon(rsc_tree);
    return true;
}


//----显示资源树----------------------------------------------------------
//功能: 按参数要求显示资源队列中资源的名字，若该资源名字为空，则显示"无名资源"
//参数: 参数字符串，待列出的树名，省却则列出全部资源
//返回: true
//-----------------------------------------------------------------------------
bool_t Sh_ListRscTree(char *param)
{
    struct Object *rsc_tree;
    if(param == NULL)
        rsc_tree = OBJ_Root();
    else
    {
        rsc_tree = OBJ_Search(OBJ_Root(),(const char*)param);
        if(rsc_tree == NULL)
        {
            printf("没找到 %s 资源树\r\n",param);
        }
    }
    __Sh_ShowBranche(rsc_tree);
    return true;
}

//----显示子资源---------------------------------------------------------------
//功能: 显示某资源结点起始的一个资源分支，不包含该资源自身
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __Sh_ShowSon(struct Object *branche)
{
    struct Object *current_node = branche;
    char *Name;
    while(1)
    {
        current_node = OBJ_TraveChild(branche,current_node);
        if(current_node == NULL)
        {
            printf("\r\n");
            break;
        }

        Name = (char*)OBJ_Name(current_node);
        if(Name)
        {
            printf("  %s\r\n", Name);
        }
        else
        {
            printf("  无名资源\r\n");
        }
    }
}

//----显示资源树---------------------------------------------------------------
//功能: 显示某资源结点起始的一个资源分支，不包含该资源自身
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __Sh_ShowBranche(struct Object *branche)
{
    struct Object *current_node = branche;
    ucpu_t len;
    char neg[20];
    char *Name;
    for(len = 0; len<20; len++)
        neg[len] = '-';
    while(1)
    {
        current_node = OBJ_TraveScion(branche,current_node);
        if(current_node == NULL)
        {
            printf("\r\n");
            break;
        }
        len = OBJ_Level(current_node);
        neg[len] = '\0';
        printf("%s", neg);
        neg[len] = '-';
        Name = (char*)OBJ_Name(current_node);
        if(Name != NULL)
        {
            printf("%s\r\n", Name);
        }else
        {
            printf("无名资源\r\n");
        }
    }
}

//----显示内存-----------------------------------------------------------------
//功能: 显示某地址开始的一段内存，每行显示16个单元，只显示，不能修改
//参数: param，参数串，本命令需要三个参数，用空格隔开
//          参数1:起始地址，10进制或16进制(0x开头)
//          参数2:显示的单元数
//          参数3:每单元字节数，合法值是1、2、4、8，其他数值将返回错误
//返回: true=正常显示，false=错误
//-----------------------------------------------------------------------------
bool_t Sh_ShowMemory(char *param)
{
    ptu32_t addr;
    uint32_t unit_num,unit_bytes,len;
    char *word_addr,*word_un,*word_ub,*word_trail,*next_param;

    //提取3个参数
    word_addr = Sh_GetWord(param,&next_param);
    word_un = Sh_GetWord(next_param,&next_param);
    word_ub = Sh_GetWord(next_param,&next_param);
    word_trail = Sh_GetWord(next_param,&next_param);
    if((word_addr == NULL)||(word_un == NULL)
            ||(word_ub == NULL)||(word_trail != NULL))
    {
        printf("\r\n格式错误，正确格式是：\r\n>d 地址 单元数 每单元字节数\r\n");
        return false;
    }
    addr = strtoul(word_addr, (char **)NULL, 0);
    unit_num = strtol(word_un, (char **)NULL, 0);
    unit_bytes = strtol(word_ub, (char **)NULL, 0);
#if (CN_BYTE_BITS == 8)  //字节位宽=8，最常见的情况
    switch(unit_bytes)
    {
        case 1:
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%02x ", *(uint8_t*)addr);
                addr ++;
                if(addr %8 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
            break;
        case 2:
            addr &= ~(ptu32_t)1;//只能从偶数开始
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%04x ", *(u16*)addr);
                addr +=2;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
            break;
        case 4:
            addr &= ~(ptu32_t)3;//只能从偶数开始
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr +=4;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
            break;
        case 8:
            addr &= ~(ptu32_t)7;//只能从偶数开始
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x", *(uint32_t*)addr);
                addr +=4;
                printf("%08x ", *(uint32_t*)addr);
                addr +=4;
                if(addr %32 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
            break;
        default :
            printf("\r\n参数错误\r\n");
            return false;
            break;
    }
#elif (CN_BYTE_BITS == 16)  //字节位宽=16
    switch(unit_bytes)
    {
        case 1:
        {
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%04x ", *(u16*)addr);
                addr ++;
                if(addr %8 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        case 2:
        {
            addr &= ~(ptu32_t)1;
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr +=2;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        case 4:
        {
            addr &= ~(ptu32_t)3;
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr +=2;
                printf("%08x ", *(uint32_t*)addr);
                putchar(' ');
                addr +=2;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        default :
        {
            printf("\r\n参数错误\r\n");
            return false;
        } break;
    }
#elif (CN_BYTE_BITS == 32)  //字节位宽=32
    switch(unit_bytes)
    {
        case 1:
        {
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr ++;
                if(addr %8 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        case 2:
        {
            addr &= ~(ptu32_t)1;
            printf("0x%08x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                printf("%08x ", *(uint32_t*)addr);
                addr +=2;
                printf("%08x ", *(uint32_t*)addr);
                putchar(' ');
                addr +=2;
                if(addr %16 == 0)
                {
                    printf("\r\n");
                    if(len != unit_num)
                    {
                        printf("0x%08x:", addr);
                    }
                }
            }
        } break;
        default :
        {
            printf("\r\n参数错误\r\n");
            return false;
        } break;
    }
#endif
    printf("\r\n");
    return true;
}

//----写入内存-----------------------------------------------------------------
//功能: 写入某地址开始的一段内存
//参数: param，参数串，本命令需要三个参数，用空格隔开
//          参数1:起始地址，10进制或16进制(0x开头)
//          参数2:显示的单元数
//          参数3:每单元字节数，合法值是1、2、4，其他数值将返回错误
//          参数4:待填充的内容
//返回: true=正常显示，false=错误
//-----------------------------------------------------------------------------
bool_t Sh_FillMemory(char *param)
{
    ptu32_t addr;
    uint32_t unit_num,unit_bytes,len,data;
    char *word,*next_param;

    word = Sh_GetWord(param,&next_param);
    addr = strtoul(word, (char **)NULL, 0);
    word = Sh_GetWord(next_param,&next_param);
    unit_num = strtol(word, (char **)NULL, 0);
    word = Sh_GetWord(next_param,&next_param);
    unit_bytes = strtol(word, (char **)NULL, 0);
    word = Sh_GetWord(next_param,&next_param);
    data = strtoul(word, (char **)NULL, 0);
#if (CN_BYTE_BITS == 8)  //字节位宽=8，最常见的情况
    switch(unit_bytes)
    {
        case 1:
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(uint8_t*)addr = (u8)data;
                addr ++;
            }
            break;
        case 2:
            addr &= ~(ptu32_t)1;
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u16*)addr = (u16)data;
                addr +=2;
            }
            break;
        case 4:
            addr &= ~(ptu32_t)3;
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u32*)addr = (u32)data;
                addr +=4;
            }
            break;
        default :
            printf("\r\n参数错误\r\n");
            return false;
            break;
    }
#elif (CN_BYTE_BITS == 16)  //字节位宽=16
    switch(unit_bytes)
    {
        case 1:
        {
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u16*)addr = (u16)data;
                addr ++;
            }
        } break;
        case 2:
        {
            addr &= ~(ptu32_t)1;
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u32*)addr = (u32)data;
                addr +=2;
            }
        } break;
        default :
        {
            printf("\r\n参数错误\r\n");
            return false;
        } break;
    }
#elif (CN_BYTE_BITS == 32)  //字节位宽=32
    switch(unit_bytes)
    {
        case 1:
        {
            printf("0x%8x:", addr);
            for(len = 0; len < unit_num; len++)
            {
                *(u32*)addr = (u32)data;
                addr ++;
            }
        } break;
        default :
        {
            printf("\r\n参数错误\r\n");
            return false;
        } break;
    }
#endif
    return true;
}

//----显示帮助-----------------------------------------------------------------
//功能: 显示帮助信息
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void Sh_CmdHelp(void)
{
    struct ShellCmdRsc *current_cmd;
    struct Object *CmdObj;
    printf("\r\n有关具体命令的详细信息，请输入help [命令名]\r\n");
    printf("\r\n");
    CmdObj = (struct Object *)s_ptShellCmdDir;
    char *Name;
    while(1)
    {
        CmdObj = OBJ_TraveScion(s_ptShellCmdDir,CmdObj);
        if(CmdObj == NULL)
        {
            break;
        }
        else
        {
            current_cmd = (struct ShellCmdRsc *)OBJ_Represent(CmdObj);
            Name = (char*)OBJ_Name(CmdObj);
            if(current_cmd->help_hint != NULL)
            {
                //todo: 前福查下，第一个%s应该是%-32s才对，下6行同
                printf("%-24s%s", Name, current_cmd->help_hint);

            }
            else
            {
                printf("%-24s没有提供简要帮助信息", Name);
            }
        }
        printf("\r\n");
    }
}


//----显示文件系统命令帮助-----------------------------------------------------
//功能:
//参数: 参数字符串，含义:
//返回: true
//-----------------------------------------------------------------------------
bool_t Sh_ListCmdHelp(char *param)
{
    char *cmd,*next_param;
    struct Object *CmdObj;
    bool_t result;
    struct ShellCmdRsc *help_cmd;

    if(param == NULL)
    {
        Sh_CmdHelp();
        result = true;
    }
    else
    {
        cmd = Sh_GetWord(param,&next_param);
        CmdObj = OBJ_SearchChild(s_ptShellCmdDir,(const char*)cmd);
        if(CmdObj != NULL)
        {
            help_cmd = (struct ShellCmdRsc *)OBJ_Represent(CmdObj);
            if(help_cmd->help_detailed != NULL)
                printf("%s",help_cmd->help_detailed);
            else
                printf("没有提供详细帮助信息");
            printf("\r\n");
            result = true;
        }else
        {
            printf("无效命令名\r\n");
            result = false;
        }
    }
    return result;
}
bool_t Sh_Ver(char *param)
{
    printf("\r\n%s\r\n", djyos_kernel_version);

    return true;
}

bool_t Sh_Date(char *param)
{
    s64 nowtime;
    struct tm dtm;
    char command[20];
    char buf[12];
    int res;

    nowtime = Tm_Time(NULL);
    Tm_LocalTime_r(&nowtime,&dtm);
    Tm_AscTime(&dtm,command);

    printf("\r\n当前时间：%10.10s %s",command, g_cTmWdays[dtm.tm_wday]);
    printf("\r\n输入新日期：");

    fgets(buf,11,stdin);
    if(strlen(buf) != 0)
    {
        memcpy(command,buf,10);
        res = Tm_SetDateTimeStr(command);
        switch (res)
        {
        case EN_CLOCK_YEAR_ERROR:
            printf("年份错误。");
            break;
        case EN_CLOCK_MON_ERROR:
            printf("月份错误。");
            break;
        case EN_CLOCK_DAY_ERROR:
            printf("日期错误。");
            break;
        case EN_CLOCK_FMT_ERROR:
            printf("格式错误。");
            break;
        default:
            break;
        }
    }

    printf("\r\n");
    return true;
}

bool_t Sh_Time(char *param)
{
    s64 nowtime;
    struct tm dtm;
    char command[20];
    int res;

    nowtime = Tm_Time(NULL);
    Tm_LocalTime_r(&nowtime,&dtm);
    Tm_AscTime(&dtm,command);

    printf("\r\n当前时间：%s",command+11);
    printf("\r\n输入新时间：");

    fgets(command+11,9,stdin);
    if(strlen(command+11) != 0)
    {
        res = Tm_SetDateTimeStr(command);
        switch (res)
        {
        case EN_CLOCK_HOUR_ERROR:
            printf("小时错误。");
            break;
        case EN_CLOCK_MIN_ERROR:
            printf("分钟错误。");
            break;
        case EN_CLOCK_SEC_ERROR:
            printf("秒钟错误。");
            break;
        case EN_CLOCK_FMT_ERROR:
            printf("格式错误。");
            break;
        default:
            break;
        }
    }

    printf("\r\n");
    return true;
}


/*
 * 以下两个函数临时添加，用于通过网线在telnet终端修改日期和时间 by zhb20170313
 */
bool_t Sh_SetTime(char *param)
{
    s64 nowtime;
    struct tm dtm;
    char command[20];
    int res;

    nowtime = Tm_Time(NULL);
    Tm_LocalTime_r(&nowtime,&dtm);
    Tm_AscTime(&dtm,command);

   if(param!=NULL)
   {
        memcpy(command,param,20);
        res = Tm_SetDateTimeStr(command);
        switch (res)
        {
        case EN_CLOCK_HOUR_ERROR:
            printf("小时错误。");
            break;
        case EN_CLOCK_MIN_ERROR:
            printf("分钟错误。");
            break;
        case EN_CLOCK_SEC_ERROR:
            printf("秒钟错误。");
            break;
        case EN_CLOCK_FMT_ERROR:
            printf("格式错误。");
            break;
        default:
            break;
        }

        printf("\r\n当前时间：%s %s",command, g_cTmWdays[dtm.tm_wday]);

   }
   else
   {
       printf("\r\n当前时间：%s %s",command, g_cTmWdays[dtm.tm_wday]);
       printf("\r\n输入新时间：");
   }

   printf("\r\n");
   return true;
}



//----判断':'后面是否有数据-----------------------------------------------------
//功能: 从一个可能包含多级路径名和文件名的字符串中判断第一次遇到':'之后是否还有
//      数据
//参数: path，被测路径，格式如"C:\aaaa\bbbb\cccc"
//返回: true=成功, 说明':'之后有数据,或者没有':'
//      false=失败，说明':'之后没有数据
//备注: path应该是经过合法性检查的字符串，不会出现word超长的问题
//-----------------------------------------------------------------------------
bool_t __judge_colon_is_data(char *path)
{
    uint32_t index;
    uint32_t  name_len;

    name_len = strlen(path);
    for(index = 0; index< name_len; index++)
    {
        if(path[index] == ':')    //遇到字符'\'
        {
            if(path[index+1] != '\0')
                return true;
            else
                return false;
        }
    }
    //字符串里没有冒号
    if(index == name_len)
        return true;
    return false;
}

//----执行控制台命令---------------------------------------------------
//功能: 分析并执行控制台命令
//参数: 无
//返回: 无
//备注: 到时候用哈希表等，进行更加快速的比较。如果这样的比较，命令多了。
//      那还不无奈死了。
//---------------------------------------------------------------------
bool_t Sh_ExecCommand(char *buf)
{
    bool_t result = false;
    char *cmd,*next_param;
    struct Object *CmdObj;
    struct ShellCmdRsc *exec_cmd;
    //串口限制读取255字符，在这里提示超长就行。
    if(strnlen(buf, CN_SHELL_CMD_LIMIT+1) > CN_SHELL_CMD_LIMIT)
    {
        printf("输入字符串太长\r\n");
    }
    //转换buf中的大写字母为小写
//    capital_convert_lowercase(buf);

    if(!__judge_colon_is_data(buf))
    {
        if (fng_pCD_PTT != NULL)
        {
            if( ! fng_pCD_PTT(buf))
            {
                printf("分区 %s 不存在",buf);
            }
        }
        else
        {
            printf("not file system");
        }
        return true;
    }
    cmd = Sh_GetWord(buf,&next_param);
    strlwr(cmd);
    CmdObj = OBJ_SearchChild(s_ptShellCmdDir,(const char*)cmd);
    if(CmdObj != NULL)
    {
        exec_cmd = (struct ShellCmdRsc *)OBJ_Represent(CmdObj);
        result = exec_cmd->shell_cmd_func(next_param);
        if(result == false)
        {
            if(exec_cmd->help_detailed)
            {
                printf("命令格式错误，正确格式是：");
                printf("%s",exec_cmd->help_detailed);
            }
            else
                printf("命令格式错误，未提供详细帮助信息");
        }
    }else
    {
        printf("\r\n无此命令\r\n");
        result = true;
    }
    return result;
}

struct MultiplexSetsCB * sg_ptShellStdin;

//----控制台服务函数-----------------------------------------------------------
//功能: 返回console输入的字符，带console输入回车符时，执行命令。一次命令不得超过
//      255字符。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
ptu32_t Sh_Service(void)
{
    char command[CN_SHELL_CMD_LIMIT+1];
//    Multiplex_Create(1);
    printf("\n\r");
    if ((fng_pPrintWorkPath != NULL))
        fng_pPrintWorkPath();
    printf(">");
    while(1)
    {
        if(stdin == NULL)
            Djy_EventDelay(1000*mS);
        else
        {
            //不能用不安全的gets
            fgets(command,CN_SHELL_CMD_LIMIT+1,stdin);
            if(strlen(command) != 0)
                Sh_ExecCommand(command);  //执行命令
            printf("\n\r");
            if ((fng_pPrintWorkPath != NULL))
                fng_pPrintWorkPath();
            printf(">");
        }
    }
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

//we just do the buffer manager and semantic analysis
//use the index to find the corresponding command here
static const char *__matchcmd(const char *index)
{
    const char *ret =NULL;
    struct Object *start;
    struct Object *tmp;

    start = OBJ_Child(s_ptShellCmdDir);
    tmp = start;
    do{
        ret = strstr(OBJ_Name(tmp),index);
        if(ret != NULL)
        {
            ret = OBJ_Name(tmp);
            break;
        }
        tmp = OBJ_Next(tmp);
    }while(tmp != start);
    return ret;
}
//for the windows.the shell get the left rigt up down
ptu32_t Sh_ServiceNew(void)
{
    u8    ch;
    int   len;
    u8    offset;
    const char *cmdindex;
    char *cwd;
    u32   vk = CN_VK_NULL;    //used when we push back the right key
    u32   vkmask = CN_VK_NULL;
    tagShellBuffer cmdbuf;
    s32 res;
    memset(&cmdbuf,0,sizeof(cmdbuf));
    {
        char *cwd = getcwd(NULL,0);
        printf("\n\r%s>", cwd);   //push the index character to the terminal
        free(cwd);
    }
    while(1)
    {
        res = getc(stdin);
        if(EOF == res)
        {
            Djy_EventDelay(1000); // 获取数据错误或者end of file，延时1ms再继续（防止出现死循环现象，导致其他线程卡死）。
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
            case CN_VK_TAB:      //tab,should auto complete the command
                //should search the command we has installed if some index has get
                len = strlen(cmdbuf.curcmd);
                if(len > 0)  //some index has get
                {
                    cmdindex = __matchcmd((const char*)cmdbuf.curcmd);
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
                    Sh_ExecCommand(cmdbuf.curcmd);  //execute the command
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
                    Sh_ExecCommand(cmdbuf.curcmd);  //execute the command
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
}
