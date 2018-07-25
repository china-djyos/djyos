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
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "object.h"
#include "djyos.h"
#include "hmi-input.h"
#include "newshell.h"
#include "version.h"
#include "time.h"
#include "systime.h"
#include "list.h"
#include "cpu-optional.h"


//----添加新的shell函数表到shell中-------------------------------------------
//功能: 加载app的时候像函数表中增加新的
//参数: pLisTtab
//返回:true/false
//备注:
//---------------------------------------------------------------------
bool_t Sh_InstallTab(struct ShellListTab *pLisTtab)
{
    Lock_MutexPend(shell_lcb,CN_TIMEOUT_FOREVER);
    dListInsertBefore(&Sh_List_head.Node,&pLisTtab->Node);
    Lock_MutexPost(shell_lcb);
    return true;
}

//-----从shell函数链表中删除一个表---------------------------------------------
//功能:
//参数:
//返回:
//备注:
//---------------------------------------------------------------------
bool_t Sh_uninstallTab(struct ShellListTab *pLisTtab)
{
    Lock_MutexPend(shell_lcb,CN_TIMEOUT_FOREVER);
    dListRemove(&pLisTtab->Node);
    Lock_MutexPost(shell_lcb);
    return true;
}
//----提取单词---------------------------------------------------------------
//功能: 从buf中提取一个由空格或行结束符隔开的单词，next用于返回下一个单词首地址，
//      如果没有下一个单词，则next=NULL。
//参数: buf，待分析的字符串
//      next，返回下一个单词指针
//返回: 提取的单词指针，已将单词后面的分隔符换成串结束符'\0'
//-----------------------------------------------------------------------------
char *NewSh_GetWord(char *buf,char **next)
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


//===========二分法查找字符找不到返回-1找到返回在表格中的偏移================================
//
//
//
//
//===========================================================================
static bool_t Sh_find(struct ShellListTab *p_Sh_List,struct CmdClass *cmdclass)
{
    //二分法搜索
	u32 i=0;
    s32 left = 0;//定义left有符号类型
    s32 right;//定义right 有符号类型
    u32 mid;//定义right
    int strflag;

    struct In_FunTab *pt_funTab;
    struct Ex_FunTab  *ptfuntab;
    struct DateTab *ptDatetab;
//--------------查内部函数表-----------------------------------
    pt_funTab = (struct In_FunTab *)p_Sh_List->info.In_funTab_start;
    left = 0;
    right = (p_Sh_List->info.In_funTab_end - p_Sh_List->info.In_funTab_start)/sizeof(struct In_FunTab);
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
        }else if(strflag>0)
        {
            right = mid-1;
            if(right==-1)
            break;
        }else if(strflag == 0)
        {
            cmdclass->cmdaddr = pt_funTab[mid].fun_addr;
            cmdclass->cmdtype = Sh_Cmdtype_Fun;
            if(pt_funTab[mid].help==0)
            	cmdclass->cmd_help=NULL;
            else
                cmdclass->cmd_help=*pt_funTab[mid].help;
            break;
        }

    }
    if(strflag == 0)
        return true;


//--------------查普通函数表-----------------------------------
    ptfuntab = (struct Ex_FunTab *)p_Sh_List->info.Ex_funTab_start;
    left = 0;
    right = (p_Sh_List->info.Ex_funTab_end - p_Sh_List->info.Ex_funTab_start)/sizeof(struct Ex_FunTab);
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
            cmdclass->cmdaddr = ptfuntab[mid].fun_addr;
            cmdclass->cmdtype = Sh_CmdtypeFun;
            cmdclass->cmd_help=NULL;
            break;
        }
    }
    if(strflag == 0)
        return true;

//--------------查变量表-----------------------------------
    ptDatetab = (struct DateTab *)p_Sh_List->info.dataTab_start;
    left = 0;
    right = (p_Sh_List->info.dataTab_end - p_Sh_List->info.dataTab_start)/sizeof(struct DateTab);
    right=right-1;
    while(left<=right) //在while循环中直到有一个条件结束搜索
    {
        mid = (left+right)/2;
        strflag = strcmp(ptDatetab[mid].data_name, cmdclass->cmdname);
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
            cmdclass->cmdaddr = ptDatetab[mid].pData;
            cmdclass->cmdtype = Sh_Cmdtype_Date;
            cmdclass->cmd_help=NULL;
            break;
        }
    }
    if(strflag == 0)
        return true;

    return false;
}
//===========搜索命令===============================
//根据cmdclass的名字补全其他信息
//
//
//
//===============================================
static bool_t Sh_SearchCmd(struct CmdClass *cmdclass)
{
    bool_t flag = false;
    struct ShellListTab *p_Sh_List;
    list_t *loc;
    u8 i;

    p_Sh_List = &Sh_List_head;
    do{

        if(true == Sh_find(p_Sh_List,cmdclass))
        {
           flag =true;
           break;
        }

        loc = dListGetAfter(&(p_Sh_List->Node));
//        p_Sh_List = Container(loc, struct ShellListTab, Node);//todo hemin modify for compiler

    }while(&Sh_List_head != p_Sh_List);
    return flag;
}

//----给变量赋值-------------------------------------------------------
//功能: 根据dataclass 内容填充dataaddr
//参数: data_class 字符串数据结构
//     DataAddr 被填充
//返回: true/false
//-----------------------------------------------------------------------------
static bool_t variable_assignment(struct DataClass *data_class,union Parameter *DataAddr)
{
    union Parameter Data;

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


//==============打印输出变量============================
//参数：data_class 数据结构
//
//
//
//
//============================================================
static bool_t ShowData(struct DataClass *data_class,union Parameter *DataAddr)
{
    union Parameter Data;

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
//       	 printf("%s \r\n",(char*)DataAddr);     // 支持打印：char test1[]="hello";   不支持打印：char*p="hello";
        	 printf("%s \r\n",DataAddr->data_pc);   //不支持打印：char test1[]="hello";   支持打印：char*p="hello";
             break;
         default:     return false;
    }
    return true;
}

//=============数据类型判断：根据字符串转换成标志=============================
//
//
//
//=====================================================
static enum ParameterTypr Type_str2flag(char *word)
{
    enum ParameterTypr datatype;

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

//================汇编传参并调用相应的函数==================================
//功能：根据参数的个数及参数类型调用汇编传递参数并调用函数
//参数：num参数的个数
//    fun 被调用函数的地址
//返回：true/false
//==================================================================
static bool_t _AsmExecuteCmd(u8 num,Ex_shell_func fun)
{
	u8 i=0;
    ParameterTab[PARAMETER_MAX];
    ParameterFlagTab[PARAMETER_MAX];
/*
    printf("num= %d \r\n",num);
    for(i=0;i<num;i++)
    {
     printf(" ParameterFlagTab = %d  ParameterTab= %d \r\n",ParameterFlagTab[i],  ParameterTab[i]);
    }
*/
#if (CN_CPU_OPTIONAL_FLOAT_HARD == 1)
    extern void __asm_ExecuteCmd(union Parameter *ptab,enum ParameterTypr *pflag,Ex_shell_func fun,u32 num);
    __asm_ExecuteCmd(ParameterTab,ParameterFlagTab,fun,num);
#endif
    return true;
}

//extern bool_t Sh_PrintWorkPath(void);
//----当前工作路函数指针赋值-----------------------------------------------------
//功能: 函数指针赋值，该指针指向获取当前工作路径的函数
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
//void Sh_PrintWorkPathSet(void)
//{
//    fng_pPrintWorkPath = (void*)Sh_PrintWorkPath;
//}

//----执行控制台命令---------------------------------------------------
//功能: 分析并执行控制台命令
//参数: 无
//返回: 无
//备注:
//---------------------------------------------------------------------
static bool_t Sh_ExecCommand(char *buf)
{
    bool_t result = false;
    char *cmd,*next_param,*word,*wordbak;
    u8 i;
    struct CmdClass cmd_class;
    struct DataClass data_class;
    in_shell_func cmd_fun;

    //串口限制读取255字符，在这里提示超长就行。
    if(strnlen(buf, CN_SHELL_CMD_LIMIT+1) > CN_SHELL_CMD_LIMIT)
    {
        printf("输入字符串太长\r\n");
    }
    cmd = NewSh_GetWord(buf,&next_param);//提取函数或者变量名
    cmd_class.cmdname=cmd;

   if(Sh_SearchCmd(&cmd_class) == false)
   {
       printf("命令 %s 不存在  !! \n\r",cmd);
       return false;
   }
/*
   printf("----------1-----------\r\n");
   printf("cmd_class.cmdname =  %s\r\n",cmd_class.cmdname);
   printf("cmd_class.cmdaddr =  %x\r\n",cmd_class.cmdaddr);
   printf("cmd_class.cmdtype =  %d\r\n",cmd_class.cmdtype);
   printf("cmdclass->cmd_help =  %s\r\n",cmd_class.cmd_help);
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
                word = NewSh_GetWord(next_param,&next_param);
                if(word != NULL)
                    ParameterFlagTab[i] = Type_str2flag(word);
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

                word = NewSh_GetWord(next_param,&next_param);
                if(word != NULL)
                {
                    data_class.datastring = word;
                    data_class.datatype = ParameterFlagTab[i];
                    variable_assignment(&data_class,&ParameterTab[i]);
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

            result = _AsmExecuteCmd(i,(Ex_shell_func)cmd_class.cmdaddr);
            break;

        case Sh_Cmdtype_Date:   //变量判断变量类型并赋值或打印 todo 补全参数为字符串的解析
            word = NewSh_GetWord(next_param,&next_param);
            if(word == NULL)
            {
               printf("正确格式为：1、 DataName = 类型  num  2、 DataName  类型\r\n");
               printf("参数类型有：u8/u16/u32/u64/s8/s16/s32/s64/b/f/d/""/''\r\n");
                break;
             }

            if(strcmp(word,"=") == 0)//变量赋值
            {
                word = NewSh_GetWord(next_param,&next_param);
                if(word != NULL)
                    data_class.datatype = Type_str2flag(word);
                else
                    data_class.datatype = flag_error;

                word = NewSh_GetWord(next_param,&next_param);
                wordbak = word;
                word = NewSh_GetWord(next_param,&next_param);
                if((NULL != word) || (wordbak ==NULL)|| \
                        (data_class.datatype == flag_error))
                {
                    printf("正确格式为 DataName = 类型  num \n\r"\
                        "类型为 u8 u16 u32 s8 s16 s32 b f d "" ''\n\r");
                    return false;
                }
                data_class.datastring = wordbak;
                result = variable_assignment(&data_class,(union Parameter *)cmd_class.cmdaddr);
            }else
            {
                data_class.datatype = Type_str2flag(word);
                if( data_class.datatype == flag_error)
                {
                    printf("shell 变量参数类型错误 ！！\n\r");
                    printf("参数类型有：u8/u16/u32/u64/s8/s16/s32/s64/b/f/d/""/''\r\n");
                    return false;
                }



                result = ShowData(&data_class,(union Parameter *)cmd_class.cmdaddr);
            }
            break;

        default :         break;
    }


    return result;
}
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
                if(Lock_MutexPend(shell_lcb,CN_TIMEOUT_FOREVER))
                {
                    Sh_ExecCommand(command);  //执行命令
                    Lock_MutexPost(shell_lcb);
                }
            }
            if ((fng_pPrintWorkPath != NULL))
                fng_pPrintWorkPath( );
            printf(">");
        }
    }
}



//----调试模块初始化函数-------------------------------------------------------
//功能: 初始化调试模块
//参数: 无
//返回: 1
//-----------------------------------------------------------------------------

ptu32_t ModuleInstall_NewSh(ptu32_t para)
{
    u16 shell_evtt;

    dListInit(&Sh_List_head.Node);
    Sh_List_head.info = *r_shell_info;

    shell_lcb = Lock_MutexCreate("shell");
    if(NULL == shell_lcb)
        return false;
    shell_evtt = Djy_EvttRegist(EN_CORRELATIVE,  //关联型事件
                                CN_PRIO_REAL,       //默认优先级
                                0,                  //线程保留数，关联型无效
                                0,                  //线程上限，关联型无效
                                Sh_Service,         //入口函数
                                NULL,               //由系统分配栈
                                0x1000,             //栈尺寸
                                "shell"             //事件类型名
                                );
    if(shell_evtt == CN_EVTT_ID_INVALID)
    {
        free(shell_lcb);
        return false;
    }
    if(Djy_EventPop(shell_evtt,NULL,0,0,0,0) == CN_EVENT_ID_INVALID)
    {
        Djy_EvttUnregist(shell_evtt);
        return false;
    }

    return 1;
}



//显示djyos的版本

ADD_TO_SHELL_HELP(ver,"显示djyos的版本");
ADD_TO_IN_SHELL bool_t ver(char *param)
{
    printf("\r\n%s\r\n", djyos_kernel_version);
    return true;
}



 //显示或者设置日期

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


//显示当前时间或者设置输入新时间

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


//----显示内存-----------------------------------------------------------------
//功能: 显示某地址开始的一段内存，每行显示16个单元，只显示，不能修改
//参数: param，参数串，本命令需要三个参数，用空格隔开
//          参数1:起始地址，10进制或16进制(0x开头)
//          参数2:显示的单元数
//          参数3:每单元字节数，合法值是1、2、4、8，其他数值将返回错误
//返回: true=正常显示，false=错误
//-----------------------------------------------------------------------------
ADD_TO_SHELL_HELP(d,"读取内存里的数据,命令格式:d 地址 单元数 每单元字节数");
ADD_TO_IN_SHELL bool_t d(char *param)
{
    ptu32_t addr;
    uint32_t unit_num,unit_bytes,len;
    char *word_addr,*word_un,*word_ub,*word_trail,*next_param;

    //提取3个参数
    word_addr = NewSh_GetWord(param,&next_param);
    word_un = NewSh_GetWord(next_param,&next_param);
    word_ub = NewSh_GetWord(next_param,&next_param);
    word_trail = NewSh_GetWord(next_param,&next_param);
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

ADD_TO_SHELL_HELP(f,"写数据到内存,命令格式：f 起始地址  单元数 每单元字节数 填充内容");
ADD_TO_IN_SHELL bool_t f(char *param)
{
    ptu32_t addr;
    uint32_t unit_num,unit_bytes,len,data;
    char *word,*next_param;

    word = NewSh_GetWord(param,&next_param);
    addr = strtoul(word, (char **)NULL, 0);
    word = NewSh_GetWord(next_param,&next_param);
    unit_num = strtol(word, (char **)NULL, 0);
    word = NewSh_GetWord(next_param,&next_param);
    unit_bytes = strtol(word, (char **)NULL, 0);
    word = NewSh_GetWord(next_param,&next_param);
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


ADD_TO_SHELL_HELP(dis_exshell,"显示外部shell函数");
ADD_TO_IN_SHELL bool_t dis_exshell(void)
{
	bool_t i,Ex_shell_num;
	struct Ex_FunTab *ptfuntab;
    struct ShellListTab *p_Sh_List;
    p_Sh_List = &Sh_List_head;

    ptfuntab = (struct Ex_FunTab *)p_Sh_List->info.Ex_funTab_start;
    Ex_shell_num = (p_Sh_List->info.Ex_funTab_end - p_Sh_List->info.Ex_funTab_start)/sizeof(struct Ex_FunTab);

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

ADD_TO_SHELL_HELP(dis_globlvar,"显示全局变量");
ADD_TO_IN_SHELL bool_t dis_globlvar(void)
{
	bool_t i,Global_Var_num;
	struct DateTab *ptDatetab;
    struct ShellListTab *p_Sh_List;
    p_Sh_List = &Sh_List_head;

    ptDatetab = (struct DateTab *)p_Sh_List->info.dataTab_start;
    Global_Var_num = (p_Sh_List->info.dataTab_end - p_Sh_List->info.dataTab_start)/sizeof(struct DateTab);

    printf("\r\n以下是所有的全局变量,共有%d个\r\n",Global_Var_num);
    printf("显示全局变量值的格式：  DataName 类型\r\n");
    printf("更改全局变量值的格式：  DataName = 类型 num  \r\n");
    printf("参数类型有：u8/u16/u32/u64/s8/s16/s32/s64/b/f/d/""/'' \r\n");
    printf("\r\n");


    for(i=0;i<Global_Var_num;i++)
    {

//       printf("%-24s%x", ptDatetab[i].data_name,ptDatetab[i].pData);
       printf("%s", ptDatetab[i].data_name);
       printf("\r\n");
    }
    printf("\r\n");
    return true;
}

bool_t NewSh_CmdHelp(void)
{
	bool_t i,in_shell_num;
	struct In_FunTab *pt_funTab;
    struct ShellListTab *p_Sh_List;
    p_Sh_List = &Sh_List_head;

    printf("\r\n有关具体命令的详细信息，请输入help [命令名]\r\n");
    printf("\r\n");

    pt_funTab = (struct In_FunTab *)p_Sh_List->info.In_funTab_start;
    in_shell_num = (p_Sh_List->info.In_funTab_end - p_Sh_List->info.In_funTab_start)/sizeof(struct In_FunTab);

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
//----显示文件系统命令帮助-----------------------------------------------------
//功能:
//参数: 参数字符串，含义:
//返回: true
//-----------------------------------------------------------------------------

ADD_TO_SHELL_HELP(help,"显示帮助信息");
ADD_TO_IN_SHELL bool_t help(char *param)
{

    char *cmd,*next_param;
    struct CmdClass cmd_class;
    bool_t result;


    if(param == NULL)
    {
    	NewSh_CmdHelp();
        result = true;
    }
    else
   {
	   cmd = NewSh_GetWord(param,&next_param);
       cmd_class.cmdname = cmd;
	   if(Sh_SearchCmd(&cmd_class) == false)
	   {
	       printf("命令 %s不存在 ,不存在帮助信息 !! \n\r",cmd);
	       return true;
	   }

	   if(cmd_class.cmd_help != NULL)
           printf("\r\n%s\r\n",cmd_class.cmd_help);
       else
           printf("没有提供详细帮助信息");
       printf("\r\n");
       result = true;

   }
    return result;
}












