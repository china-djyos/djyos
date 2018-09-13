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
// 3. 日期:2017-03-16
//   作者:
//   新版本号：
//   修改说明: 修改为查找符号表实现shell，
//------------------------------------------------------
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <djyos.h>
#include <shell.h>
#include <version.h>
#include <time.h>
#include <systime.h>
#include <list.h>
#include <dbug.h>
#include <lock.h>
#include "cpu-optional.h"

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
//attribute:核心组件             //选填“第三方组件、核心组件、bsp组件、用户组件”，本属性用于在IDE中分组
//select:必选                   //选填“必选、可选、不可选”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//grade:init                    //初始化时机，可选值：none，init，main。none表示无须初始化，
                                //init表示在调用main之前，main表示在main函数中初始化
//dependence:"stdio","cpu_peri_uart"    //该组件的依赖组件名（可以是none，表示无依赖组件），
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
#define CN_SHELL_CMD_LIMIT 255 // shell 命令串长度限制

extern struct shellinfo p_shell_info; // p_shell_info 来自iboot.lds文件
static struct shellinfo *r_shell_info = &p_shell_info;
static struct MutexLCB *__shell_mutex;
struct shell_list shells_list;

enum __shell_mode
{
    dbug = 1,
    normal
};

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
static bool_t __find(struct shell_list *p_Sh_List, struct commandclass *cmdclass)
{
    //二分法搜索
    u32 i=0;
    s32 left = 0;//定义left有符号类型
    s32 right;//定义right 有符号类型
    u32 mid;//定义right
    int strflag;

    struct inshell_func *pt_funTab;
    struct exshell_func  *ptfuntab;
    struct data_struct *ptDatetab;
//--------------查内部函数表-----------------------------------
    pt_funTab = (struct inshell_func *)p_Sh_List->info.In_funTab_start;
    left = 0;
    right = (p_Sh_List->info.In_funTab_end - p_Sh_List->info.In_funTab_start)/sizeof(struct inshell_func);
    /*
    for(i=0;i<right;i++)
    printf("_fun_name %d =  %s _fun_addr= %x   help_addr= %x   *help_addr=%x \r\n  ",i,pt_funTab[i].fun_name,pt_funTab[i].fun_addr,pt_funTab[i].help,*pt_funTab[i].help);
    printf("\r\n");
    */
    right=right-1;
    while(left<=right) //在while循环中直到有一个条件结束搜索
    {
        mid = (left+right)/2;
        strflag = strcmp(pt_funTab[mid].fun_name , cmdclass->cmdname);
        if(strflag<0)
        {
            left = mid+1;
        }
        else if(strflag>0)
        {
            right = mid-1;
            if(right==-1)
            break;
        }
        else if(strflag == 0)
        {
            cmdclass->cmdaddr = pt_funTab[mid].fun_addr;
            cmdclass->cmdtype = Sh_Cmdtype_Fun;
            if(pt_funTab[mid].help==0)
                cmdclass->cmdhelp=NULL;
            else
                cmdclass->cmdhelp=*pt_funTab[mid].help;
            break;
        }

    }
    if(strflag == 0)
        return true;


//--------------查外部函数表-----------------------------------
    ptfuntab = (struct exshell_func *)p_Sh_List->info.Ex_funTab_start;
    left = 0;
    right = (p_Sh_List->info.Ex_funTab_end - p_Sh_List->info.Ex_funTab_start)/sizeof(struct exshell_func);
/*
    for(i=0;i<right;i++)
printf("fun_name %d =  %s   addr= %x\r\n  ",i,ptfuntab[i].fun_name,ptfuntab[i].fun_addr);
    printf("\r\n");
*/
    right=right-1;
    while(left<=right) //在while循环中直到有一个条件结束搜索
    {
        mid = (left+right)/2;
        strflag = strcmp(ptfuntab[mid].fun_name , cmdclass->cmdname);
        if(strflag<0)
        {
            left = mid+1;
        }else if(strflag>0)
        {
            right = mid-1;
            if(right==-1)
            break;
        }else if(strflag == 0)
        {
            cmdclass->cmdaddr = (void *)ptfuntab[mid].fun_addr;
            cmdclass->cmdtype = Sh_CmdtypeFun;
            cmdclass->cmdhelp=NULL;
            break;
        }
    }
    if(strflag == 0)
        return true;

//--------------查变量表-----------------------------------
    ptDatetab = (struct data_struct *)p_Sh_List->info.dataTab_start;
    left = 0;
    right = (p_Sh_List->info.dataTab_end - p_Sh_List->info.dataTab_start)/sizeof(struct data_struct);
    right=right-1;
    while(left<=right) //在while循环中直到有一个条件结束搜索
    {
        mid = (left+right)/2;
        strflag = strcmp(ptDatetab[mid].data_name, cmdclass->cmdname);
        if(strflag<0)
        {
            left = mid+1;
        }
        else if(strflag>0)
        {
            right = mid-1;
            if(right==-1)
            break;
        }
        else if(strflag == 0)
        {
            cmdclass->cmdaddr = ptDatetab[mid].data;
            cmdclass->cmdtype = Sh_Cmdtype_Date;
            cmdclass->cmdhelp=NULL;
            break;
        }
    }
    if(strflag == 0)
        return true;

    return false;
}

// ============================================================================
// 功能：搜索命令, 根据cmdclass的名字补全其他信息.
// 参数：
// 返回：
// 备注：
// ============================================================================
bool_t __search_cmd(struct commandclass *cmdclass)
{
    bool_t flag = false;
    struct shell_list *p_Sh_List;
    list_t *loc;
    u8 i;

    p_Sh_List = &shells_list;
    do{

        if(true == __find(p_Sh_List,cmdclass))
        {
           flag =true;
           break;
        }

        loc = dListGetAfter(&(p_Sh_List->list));
//        p_Sh_List = Container(loc, struct shell_list, list);//todo hemin modify for compiler

    }while(&shells_list != p_Sh_List);
    return flag;
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
    union param Data;

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
             strcpy(DataAddr->data_pc,data_class->datastring);//TODO 字符串拷贝过多容易出现越界 '\'未做转义字符检查。
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
    union param Data;

    switch (data_class->datatype)
    {
         case flag_c   :
//           printf("%d \n\r",*(char*)DataAddr);
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
 //           printf("%d \n\r",*(u64*)DataAddr);
              printf("%d \n\r",DataAddr->data_u64);
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
             printf("%d   \n\r",DataAddr->data_s64);
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
//   printf("(char*)DataAddr:%x   (char*)DataAddr str： %s\r\n\r\n",(char*)DataAddr,(char*)DataAddr);
//   printf(" DataAddr->data_pc: %x   DataAddr->data_pc str: %s\r\n\r\n",DataAddr->data_pc,DataAddr->data_pc);
//           printf("%s \r\n",(char*)DataAddr);     // 支持打印：char test1[]="hello";   不支持打印：char*p="hello";
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
    else if((word[0] == "'") && (word[strlen(word)-1] =="'" ) && (strlen(word)==2))
        datatype = flag_c;
    else if((word[0] == '"') && (word[strlen(word)-1] =='"' ) )//""" == 0x22;
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
//  u8 i=0;
//    ParameterTab[PARAMETER_MAX];
//    ParameterFlagTab[PARAMETER_MAX];
/*
    printf("num= %d \r\n",num);
    for(i=0;i<num;i++)
    {
     printf(" ParameterFlagTab = %d  ParameterTab= %d \r\n",ParameterFlagTab[i],  ParameterTab[i]);
    }
*/
//#if (CN_CPU_OPTIONAL_FLOAT_HARD == 1)
    extern void __asm_ExecuteCmd(union param *ptab,enum param_typr *pflag,Ex_shell_func fun,u32 num);
    __asm_ExecuteCmd(ParameterTab,ParameterFlagTab,fun,num);
//#endif
    return true;
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
    char *cmd,*next_param,*word,*wordbak;
    u8 i;
    struct commandclass cmd_class;
    struct dataclass data_class;
    in_shell_func cmd_fun;

    //串口限制读取255字符，在这里提示超长就行。
    if(strnlen(buf, CN_SHELL_CMD_LIMIT+1) > CN_SHELL_CMD_LIMIT)
    {
        printf("输入字符串太长\r\n");
    }

    cmd = shell_inputs(buf, &next_param);//提取函数或者变量名
    cmd_class.cmdname=cmd;

    if(__search_cmd(&cmd_class) == false)
    {
        printf("命令 %s 不存在  !! \n\r",cmd);
        return false;
    }
/*
   printf("----------1-----------\r\n");
   printf("cmd_class.cmdname =  %s\r\n",cmd_class.cmdname);
   printf("cmd_class.cmdaddr =  %x\r\n",cmd_class.cmdaddr);
   printf("cmd_class.cmdtype =  %d\r\n",cmd_class.cmdtype);
   printf("cmdclass->cmdhelp =  %s\r\n",cmd_class.cmdhelp);
   printf("----------2-----------\r\n");

*/
    switch (cmd_class.cmdtype)
    {
        case Sh_Cmdtype_Fun:  //内部函数直接将字符串传递给函数由函数自己解析
            cmd_fun = (in_shell_func) cmd_class.cmdaddr;
            result = cmd_fun(next_param);

            if(result == false)
            {
                    printf("shell 内部函数执行错误 ！！\n\r");
            }
            break;

        case Sh_CmdtypeFun:  //普通shell函数  TODO 补全参数为字符串的解析
            for(i=0;i<PARAMETER_MAX;i++)
            {
                word = shell_inputs(next_param,&next_param);
                if(word != NULL)
                    ParameterFlagTab[i] = __str2type(word);
                else
                {
                    printf("shell 函数缺少参数类型 ！！\n\r");
                    printf("参数类型有：u8/u16/u32/u64/s8/s16/s32/s64/b/f/d\n\r");
                    break;
                }
                if(ParameterFlagTab[i] == flag_error)
                {
                    printf("shell 函数参数类型错误 ！！\n\r");
                    printf("参数类型有：u8/u16/u32/u64/s8/s16/s32/s64/b/f/d\n\r");
                    return false;
                }

                word = shell_inputs(next_param,&next_param);
                if(word != NULL)
                {
                    data_class.datastring = word;
                    data_class.datatype = ParameterFlagTab[i];
                    __variable_assignment(&data_class,&ParameterTab[i]);
                }
                else
                {
                    printf("shell 函数格式错误 ！！\n\r");
                    printf("shell函数格式为：命令名  类型1 参数1 类型2 参数2 类型3 参数3...\r\n");
                    return false;
                }
                if(next_param==NULL)
                {
                    if(i==0)

                    {
                       i=1; //为了当只有一个形参时，不要执行下面的return false;
                       break;
                    }
                    else
                    {
                        i=i+1;
                        break;
                    }
                }
                else if((next_param!=NULL)&&(i==(PARAMETER_MAX-1)))
                {
                     i=i+2; //要执行下面的return false;
                     break;
                }
            }

            if((i > PARAMETER_MAX) || (i == 0))
                {
                    if(i > PARAMETER_MAX)
                         printf("形参数量最多10个，不能超过10个\r\n");
                    return false;
                }

            result = __asm_execute(i,(Ex_shell_func)cmd_class.cmdaddr);
            break;

        case Sh_Cmdtype_Date:   //变量判断变量类型并赋值或打印 todo 补全参数为字符串的解析
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
                result = __variable_assignment(&data_class,(union param *)cmd_class.cmdaddr);
            }else
            {
                data_class.datatype = __str2type(word);
                if( data_class.datatype == flag_error)
                {
                    printf("shell 变量参数类型错误 ！！\n\r");
                    printf("参数类型有：u8/u16/u32/u64/s8/s16/s32/s64/b/f/d/""/''\r\n");
                    return false;
                }



                result = __show_data(&data_class,(union param *)cmd_class.cmdaddr);
            }
            break;

        default : break;
    }

    return result;
}

#if 0
//----控制台服务函数-----------------------------------------------------------
//功能: 返回console输入的字符，带console输入回车符时，执行命令。一次命令不得超过
//      255字符。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
static ptu32_t Sh_Service(void)
{
    char command[CN_SHELL_CMD_LIMIT+1];
    printf("\n\r");
    if ((fng_pPrintWorkPath != NULL))
        fng_pPrintWorkPath( );
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
            {
                if(Lock_MutexPend(__shell_mutex,CN_TIMEOUT_FOREVER))
                {
                    shell_exec_command(command);  //执行命令
                    Lock_MutexPost(__shell_mutex);
                }
            }
            if ((fng_pPrintWorkPath != NULL))
                fng_pPrintWorkPath( );
            printf(">");
        }
    }
}
#endif

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

//for the windows.the shell get the left rigt up down
static ptu32_t __shell_service(void)
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
    enum __shell_mode mode;
    ptu32_t param;
    ptu32_t param_mode;
    extern s32 shell_default(void);

    Djy_GetEventPara((ptu32_t*)(&param_mode), NULL); // 获取运行模式
    mode = param_mode;
    shell_default(); // 基本的shell命令；
    memset(&cmdbuf,0,sizeof(cmdbuf));
    cwd = getcwd(NULL,0);
    printf("\n\r%s>", cwd);   //push the index character to the terminal
    free(cwd);

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
            case CN_VK_TAB: // tab,should auto complete the command TODO:未实现；
                //should search the command we has installed if some index has get
                len = strlen(cmdbuf.curcmd);
                if(len > 0)  //some index has get
                {
                    if(dbug==mode)
                    {
                        //we just do the buffer manager and semantic analysis
                        //use the index to find the corresponding command here
                        extern const char *__match_debug_cmd(const char *index);

                        cmdindex = __match_debug_cmd((const char*)cmdbuf.curcmd);
                    }
                    else
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
                    if(dbug==mode)
                        shell_debug_execute(cmdbuf.curcmd);
                    else
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
                    if(dbug==mode)
                        shell_debug_execute(cmdbuf.curcmd);
                    else
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
    para = para;
    static enum __shell_mode mode = normal;

    dListInit(&shells_list.list);
    shells_list.info = *r_shell_info;
    __shell_mutex = Lock_MutexCreate("shell");
    if(NULL == __shell_mutex)
        return (-1);

    shell_evtt = Djy_EvttRegist(EN_CORRELATIVE, // 关联型事件
                                CN_PRIO_REAL, // 默认优先级
                                0, // 线程保留数，关联型无效
                                0, // 线程上限，关联型无效
                                __shell_service, // 入口函数
                                NULL, // 由系统分配栈
                                0x1000, // 栈尺寸
                                "shell" // 事件类型名
                                );

    if(shell_evtt==CN_EVTT_ID_INVALID)
    {
        Lock_MutexDelete(__shell_mutex);
        error_printf("module", "shell install failed.");
        return (-1);
    }

#ifdef DEBUG
    {
        extern s32 shell_debug_init(void);
        mode = dbug;
        shell_debug_init();
    }
#endif
    if(Djy_EventPop(shell_evtt, NULL, 0, mode, 0, 0) == CN_EVENT_ID_INVALID)
    {
        Djy_EvttUnregist(shell_evtt);
        return (-1);
    }

    return (0);
}
#if 0
// ============================================================================
// 功能：显示os的版本
// 参数：
// 返回：
// 备注：
// ============================================================================
ADD_TO_SHELL_HELP(ver,"显示os的版本");
ADD_TO_IN_SHELL bool_t ver(char *param)
{
    param = param;
    printf("\r\n%s\r\n", djyos_kernel_version);
    return true;
}

// ============================================================================
// 功能：显示或者设置日期
// 参数：
// 返回：
// 备注：
// ============================================================================
ADD_TO_SHELL_HELP(date,"显示或者设置日期");
ADD_TO_IN_SHELL bool_t date(char *param)
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

// ============================================================================
// 功能：显示当前时间或者设置输入新时间
// 参数：
// 返回：
// 备注：
// ============================================================================
ADD_TO_SHELL_HELP(Time,"显示当前时间或者设置输入新时间");
ADD_TO_IN_SHELL bool_t Time(char *param)
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

// ============================================================================
// 功能：显示内存，显示某地址开始的一段内存，每行显示16个单元，只显示，不能修改
// 参数：param -- 参数串，本命令需要三个参数，用空格隔开
//               参数1:起始地址，10进制或16进制(0x开头)
//               参数2:显示的单元数
//               参数3:每单元字节数，合法值是1、2、4，其他数值将返回错误
//               参数4:待填充的内容
// 返回：true=正常显示，false=错误
// 备注：
// ============================================================================
ADD_TO_SHELL_HELP(d,"读取内存里的数据,命令格式:d 地址 单元数 每单元字节数");
ADD_TO_IN_SHELL bool_t d(char *param)
{
    ptu32_t addr;
    uint32_t unit_num,unit_bytes,len;
    char *word_addr,*word_un,*word_ub,*word_trail,*next_param;

    //提取3个参数
    word_addr = shell_inputs(param,&next_param);
    word_un = shell_inputs(next_param,&next_param);
    word_ub = shell_inputs(next_param,&next_param);
    word_trail = shell_inputs(next_param,&next_param);
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

// ============================================================================
// 功能：写入某地址开始的一段内存
// 参数：param -- 参数串，本命令需要三个参数，用空格隔开
//               参数1:起始地址，10进制或16进制(0x开头)
//               参数2:显示的单元数
//               参数3:每单元字节数，合法值是1、2、4，其他数值将返回错误
//               参数4:待填充的内容
// 返回：true=正常显示，false=错误
// 备注：
// ============================================================================
ADD_TO_SHELL_HELP(f,"写数据到内存,命令格式：f 起始地址  单元数 每单元字节数 填充内容");
ADD_TO_IN_SHELL bool_t f(char *param)
{
    ptu32_t addr;
    uint32_t unit_num,unit_bytes,len,data;
    char *word,*next_param;

    word = shell_inputs(param,&next_param);
    addr = strtoul(word, (char **)NULL, 0);
    word = shell_inputs(next_param,&next_param);
    unit_num = strtol(word, (char **)NULL, 0);
    word = shell_inputs(next_param,&next_param);
    unit_bytes = strtol(word, (char **)NULL, 0);
    word = shell_inputs(next_param,&next_param);
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

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
ADD_TO_SHELL_HELP(dis_exshell,"显示外部shell函数");
ADD_TO_IN_SHELL bool_t dis_exshell(void)
{
    bool_t i,Ex_shell_num;
    struct exshell_func *ptfuntab;
    struct shell_list *p_Sh_List;
    p_Sh_List = &shells_list;

    ptfuntab = (struct exshell_func *)p_Sh_List->info.Ex_funTab_start;
    Ex_shell_num = (p_Sh_List->info.Ex_funTab_end - p_Sh_List->info.Ex_funTab_start)/sizeof(struct exshell_func);

    printf("\r\n以下是所有的外部shell函数命令,共有 %d个\r\n",Ex_shell_num);
    printf("外部shell函数格式为：命令名  类型1 参数1 类型2 参数2 类型3 参数3...\r\n");
    printf("\r\n");

    for(i=0;i<Ex_shell_num;i++)
    {
       printf("%s", ptfuntab[i].fun_name);
        printf("\r\n");
    }
    printf("\r\n");

    return true;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
ADD_TO_SHELL_HELP(dis_globlvar,"显示全局变量");
ADD_TO_IN_SHELL bool_t dis_globlvar(void)
{
    bool_t i,Global_Var_num;
    struct data_struct *ptDatetab;
    struct shell_list *p_Sh_List;
    p_Sh_List = &shells_list;

    ptDatetab = (struct data_struct *)p_Sh_List->info.dataTab_start;
    Global_Var_num = (p_Sh_List->info.dataTab_end - p_Sh_List->info.dataTab_start)/sizeof(struct data_struct);

    printf("\r\n以下是所有的全局变量,共有%d个\r\n",Global_Var_num);
    printf("显示全局变量值的格式：  DataName 类型\r\n");
    printf("更改全局变量值的格式：  DataName = 类型 num  \r\n");
    printf("参数类型有：u8/u16/u32/u64/s8/s16/s32/s64/b/f/d/""/'' \r\n");
    printf("\r\n");


    for(i=0;i<Global_Var_num;i++)
    {

//       printf("%-24s%x", ptDatetab[i].data_name,ptDatetab[i].data);
       printf("%s", ptDatetab[i].data_name);
       printf("\r\n");
    }
    printf("\r\n");
    return true;
}

// ============================================================================
// 功能：显示所有命令的帮组信息
// 参数：
// 返回：
// 备注：
// ============================================================================
static bool_t __help_all(void)
{
    bool_t i,in_shell_num;
    struct inshell_func *pt_funTab;
    struct shell_list *p_Sh_List;
    p_Sh_List = &shells_list;

    printf("\r\n有关具体命令的详细信息，请输入help [命令名]\r\n");
    printf("\r\n");

    pt_funTab = (struct inshell_func *)p_Sh_List->info.In_funTab_start;
    in_shell_num = (p_Sh_List->info.In_funTab_end - p_Sh_List->info.In_funTab_start)/sizeof(struct inshell_func);

    for(i=0;i<in_shell_num;i++)
    {

        if(pt_funTab[i].help != NULL)
               printf("%-24s%s", pt_funTab[i].fun_name, *pt_funTab[i].help);
           else
             printf("%-24s没有提供简要帮助信息", pt_funTab[i].fun_name);

        printf("\r\n");
    }
    printf("\r\n\r\n");
    return true;
}

// ============================================================================
// 功能：显示文件系统命令帮助
// 参数：param -- 参数字符串，含义:
// 返回：
// 备注：
// ============================================================================
ADD_TO_SHELL_HELP(help,"显示帮助信息");
ADD_TO_IN_SHELL bool_t help(char *param)
{

    char *cmd,*next_param;
    struct commandclass cmd_class;
    bool_t result;

    if(param == NULL)
    {
        __help_all();
        result = true;
    }
    else
    {
       cmd = shell_inputs(param,&next_param);
       cmd_class.cmdname = cmd;
       if(__search_cmd(&cmd_class) == false)
       {
           printf("命令 %s不存在 ,不存在帮助信息 !! \n\r",cmd);
           return true;
       }

       if(cmd_class.cmdhelp != NULL)
           printf("\r\n%s\r\n",cmd_class.cmdhelp);
       else
           printf("没有提供详细帮助信息");
       printf("\r\n");
       result = true;
    }

    return result;
}
#endif
