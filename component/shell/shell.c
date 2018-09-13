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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��: djyshģ��
//���ߣ�wjz
//�汾��V0.1.0
//�ļ�����: djyos��shell��ģ�飬�ṩshell��ʵ�ֹ����Լ���������
//�޶���ʷ:
// 1. ����:2010-09-10֮ǰ
//   ����:lst
//   �°汾�ţ�djysh0.0.1
//     �޸�˵����shellʵ�ֵ���ơ�
// 2. ����:2011-06-15
//   ����:wjz
//   �°汾�ţ�djysh0.0.2
//   �޸�˵��: ����ļ�ϵͳ�����Լ�ע���ļ�ϵͳshellģ�顣
// 2. ����:2011-09-24
//   ����:wjz
//   �°汾�ţ�djysh1.0.0
//   �޸�˵��: ����¼����¼����͡��жϡ��ڴ桢���������
//               djysh1.0.0��������Ϊһ����������shell�ˡ�
// 3. ����:2017-03-16
//   ����:
//   �°汾�ţ�
//   �޸�˵��: �޸�Ϊ���ҷ��ű�ʵ��shell��
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

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut);
//    Stdio_KnlInOutInit(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME);
//
//    extern s32 ModuleInstall_Shell(ptu32_t para);
//    ModuleInstall_Shell(0);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"shell"        //shell
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:�������             //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                   //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:init                    //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
//dependence:"stdio","cpu_peri_uart"    //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
//%$#@end configue  ****�������ý���
//@#$%component end configure
#define CN_SHELL_CMD_LIMIT 255 // shell �����������

extern struct shellinfo p_shell_info; // p_shell_info ����iboot.lds�ļ�
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
// ���ܣ�����µ�shell������shell��, ����app��ʱ���������������µģ�
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t shell_add(struct shell_list *pLisTtab)
{
    Lock_MutexPend(__shell_mutex,CN_TIMEOUT_FOREVER);
    dListInsertBefore(&shells_list.list,&pLisTtab->list);
    Lock_MutexPost(__shell_mutex);
    return (TRUE);
}

// ============================================================================
// ���ܣ���shell����������ɾ��һ����
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t shell_del(struct shell_list *pLisTtab)
{
    Lock_MutexPend(__shell_mutex,CN_TIMEOUT_FOREVER);
    dListRemove(&pLisTtab->list);
    Lock_MutexPost(__shell_mutex);
    return TRUE;
}

// ============================================================================
// ���ܣ���shell������������ַ������У���ȡ��һ��������
// ������input -- shell���������
//      next -- ����������
// ���أ��޳������������
// ��ע������֮�������ո���Ϊ�ָ�����
//      ��ȡ���Ĳ������ѽ�����ķָ������ɴ�������'\0��
//      input��ԭʼ�����ѷ����仯��
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
// ���ܣ����ַ������ַ��Ҳ�������-1�ҵ������ڱ���е�ƫ��
// ������
// ���أ�
// ��ע��
// ============================================================================
static bool_t __find(struct shell_list *p_Sh_List, struct commandclass *cmdclass)
{
    //���ַ�����
    u32 i=0;
    s32 left = 0;//����left�з�������
    s32 right;//����right �з�������
    u32 mid;//����right
    int strflag;

    struct inshell_func *pt_funTab;
    struct exshell_func  *ptfuntab;
    struct data_struct *ptDatetab;
//--------------���ڲ�������-----------------------------------
    pt_funTab = (struct inshell_func *)p_Sh_List->info.In_funTab_start;
    left = 0;
    right = (p_Sh_List->info.In_funTab_end - p_Sh_List->info.In_funTab_start)/sizeof(struct inshell_func);
    /*
    for(i=0;i<right;i++)
    printf("_fun_name %d =  %s _fun_addr= %x   help_addr= %x   *help_addr=%x \r\n  ",i,pt_funTab[i].fun_name,pt_funTab[i].fun_addr,pt_funTab[i].help,*pt_funTab[i].help);
    printf("\r\n");
    */
    right=right-1;
    while(left<=right) //��whileѭ����ֱ����һ��������������
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


//--------------���ⲿ������-----------------------------------
    ptfuntab = (struct exshell_func *)p_Sh_List->info.Ex_funTab_start;
    left = 0;
    right = (p_Sh_List->info.Ex_funTab_end - p_Sh_List->info.Ex_funTab_start)/sizeof(struct exshell_func);
/*
    for(i=0;i<right;i++)
printf("fun_name %d =  %s   addr= %x\r\n  ",i,ptfuntab[i].fun_name,ptfuntab[i].fun_addr);
    printf("\r\n");
*/
    right=right-1;
    while(left<=right) //��whileѭ����ֱ����һ��������������
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

//--------------�������-----------------------------------
    ptDatetab = (struct data_struct *)p_Sh_List->info.dataTab_start;
    left = 0;
    right = (p_Sh_List->info.dataTab_end - p_Sh_List->info.dataTab_start)/sizeof(struct data_struct);
    right=right-1;
    while(left<=right) //��whileѭ����ֱ����һ��������������
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
// ���ܣ���������, ����cmdclass�����ֲ�ȫ������Ϣ.
// ������
// ���أ�
// ��ע��
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
// ���ܣ���������ֵ,����dataclass �������dataaddr
// ������data_class -- �ַ������ݽṹ
//      DataAddr -- �����
// ���أ�
// ��ע��
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
             strcpy(DataAddr->data_pc,data_class->datastring);//TODO �ַ��������������׳���Խ�� '\'δ��ת���ַ���顣
             break;                    //��֧���ն��޸��ַ���
         default:     return false;
    }
    return true;
}

// ============================================================================
// ���ܣ���ӡ�������
// ������
// ���أ�
// ��ע��
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
//   printf("(char*)DataAddr:%x   (char*)DataAddr str�� %s\r\n\r\n",(char*)DataAddr,(char*)DataAddr);
//   printf(" DataAddr->data_pc: %x   DataAddr->data_pc str: %s\r\n\r\n",DataAddr->data_pc,DataAddr->data_pc);
//           printf("%s \r\n",(char*)DataAddr);     // ֧�ִ�ӡ��char test1[]="hello";   ��֧�ִ�ӡ��char*p="hello";
             printf("%s \r\n",DataAddr->data_pc);   //��֧�ִ�ӡ��char test1[]="hello";   ֧�ִ�ӡ��char*p="hello";
             break;
         default:     return false;
    }
    return true;
}

// ============================================================================
// ���ܣ����������жϣ������ַ���ת�������ͱ�־
// ������
// ���أ�
// ��ע��
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
// ���ܣ����ݲ����ĸ������������͵��û�ഫ�ݲ��������ú���
// ������num -- �����ĸ���
//      fun -- �����ú����ĵ�ַ
// ���أ��ɹ���TRUE��;ʧ�ܣ�FALSE����
// ��ע��
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
// ���ܣ�������ִ�п���̨����
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t shell_exec_command(char *buf)
{
    bool_t result = false;
    char *cmd,*next_param,*word,*wordbak;
    u8 i;
    struct commandclass cmd_class;
    struct dataclass data_class;
    in_shell_func cmd_fun;

    //�������ƶ�ȡ255�ַ�����������ʾ�������С�
    if(strnlen(buf, CN_SHELL_CMD_LIMIT+1) > CN_SHELL_CMD_LIMIT)
    {
        printf("�����ַ���̫��\r\n");
    }

    cmd = shell_inputs(buf, &next_param);//��ȡ�������߱�����
    cmd_class.cmdname=cmd;

    if(__search_cmd(&cmd_class) == false)
    {
        printf("���� %s ������  !! \n\r",cmd);
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
        case Sh_Cmdtype_Fun:  //�ڲ�����ֱ�ӽ��ַ������ݸ������ɺ����Լ�����
            cmd_fun = (in_shell_func) cmd_class.cmdaddr;
            result = cmd_fun(next_param);

            if(result == false)
            {
                    printf("shell �ڲ�����ִ�д��� ����\n\r");
            }
            break;

        case Sh_CmdtypeFun:  //��ͨshell����  TODO ��ȫ����Ϊ�ַ����Ľ���
            for(i=0;i<PARAMETER_MAX;i++)
            {
                word = shell_inputs(next_param,&next_param);
                if(word != NULL)
                    ParameterFlagTab[i] = __str2type(word);
                else
                {
                    printf("shell ����ȱ�ٲ������� ����\n\r");
                    printf("���������У�u8/u16/u32/u64/s8/s16/s32/s64/b/f/d\n\r");
                    break;
                }
                if(ParameterFlagTab[i] == flag_error)
                {
                    printf("shell �����������ʹ��� ����\n\r");
                    printf("���������У�u8/u16/u32/u64/s8/s16/s32/s64/b/f/d\n\r");
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
                    printf("shell ������ʽ���� ����\n\r");
                    printf("shell������ʽΪ��������  ����1 ����1 ����2 ����2 ����3 ����3...\r\n");
                    return false;
                }
                if(next_param==NULL)
                {
                    if(i==0)

                    {
                       i=1; //Ϊ�˵�ֻ��һ���β�ʱ����Ҫִ�������return false;
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
                     i=i+2; //Ҫִ�������return false;
                     break;
                }
            }

            if((i > PARAMETER_MAX) || (i == 0))
                {
                    if(i > PARAMETER_MAX)
                         printf("�β��������10�������ܳ���10��\r\n");
                    return false;
                }

            result = __asm_execute(i,(Ex_shell_func)cmd_class.cmdaddr);
            break;

        case Sh_Cmdtype_Date:   //�����жϱ������Ͳ���ֵ���ӡ todo ��ȫ����Ϊ�ַ����Ľ���
            word = shell_inputs(next_param,&next_param);
            if(word == NULL)
            {
               printf("��ȷ��ʽΪ��1�� DataName = ����  num  2�� DataName  ����\r\n");
               printf("���������У�u8/u16/u32/u64/s8/s16/s32/s64/b/f/d/""/''\r\n");
                break;
             }

            if(strcmp(word,"=") == 0)//������ֵ
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
                    printf("��ȷ��ʽΪ DataName = ����  num \n\r"\
                        "����Ϊ u8 u16 u32 s8 s16 s32 b f d "" ''\n\r");
                    return false;
                }
                data_class.datastring = wordbak;
                result = __variable_assignment(&data_class,(union param *)cmd_class.cmdaddr);
            }else
            {
                data_class.datatype = __str2type(word);
                if( data_class.datatype == flag_error)
                {
                    printf("shell �����������ʹ��� ����\n\r");
                    printf("���������У�u8/u16/u32/u64/s8/s16/s32/s64/b/f/d/""/''\r\n");
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
//----����̨������-----------------------------------------------------------
//����: ����console������ַ�����console����س���ʱ��ִ�����һ������ó���
//      255�ַ���
//����: ��
//����: ��
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
            //�����ò���ȫ��gets
            fgets(command,CN_SHELL_CMD_LIMIT+1,stdin);
            if(strlen(command) != 0)
            {
                if(Lock_MutexPend(__shell_mutex,CN_TIMEOUT_FOREVER))
                {
                    shell_exec_command(command);  //ִ������
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

    Djy_GetEventPara((ptu32_t*)(&param_mode), NULL); // ��ȡ����ģʽ
    mode = param_mode;
    shell_default(); // ������shell���
    memset(&cmdbuf,0,sizeof(cmdbuf));
    cwd = getcwd(NULL,0);
    printf("\n\r%s>", cwd);   //push the index character to the terminal
    free(cwd);

    while(1)
    {
        res = getc(stdin);
        if(EOF == res)
        {
            Djy_EventDelay(1000); // ��ȡ���ݴ������end of file����ʱ1ms�ټ�������ֹ������ѭ�����󣬵��������߳̿�������
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
            case CN_VK_TAB: // tab,should auto complete the command TODO:δʵ�֣�
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
                        cmdindex = NULL; // ���ܴ�ʵ�֣�
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
// ���ܣ�shellģ���ʼ��
// ������
// ���أ�
// ��ע��
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

    shell_evtt = Djy_EvttRegist(EN_CORRELATIVE, // �������¼�
                                CN_PRIO_REAL, // Ĭ�����ȼ�
                                0, // �̱߳���������������Ч
                                0, // �߳����ޣ���������Ч
                                __shell_service, // ��ں���
                                NULL, // ��ϵͳ����ջ
                                0x1000, // ջ�ߴ�
                                "shell" // �¼�������
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
// ���ܣ���ʾos�İ汾
// ������
// ���أ�
// ��ע��
// ============================================================================
ADD_TO_SHELL_HELP(ver,"��ʾos�İ汾");
ADD_TO_IN_SHELL bool_t ver(char *param)
{
    param = param;
    printf("\r\n%s\r\n", djyos_kernel_version);
    return true;
}

// ============================================================================
// ���ܣ���ʾ������������
// ������
// ���أ�
// ��ע��
// ============================================================================
ADD_TO_SHELL_HELP(date,"��ʾ������������");
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

    printf("\r\n��ǰʱ�䣺%10.10s %s",command, g_cTmWdays[dtm.tm_wday]);
    printf("\r\n���������ڣ�");

    fgets(buf,11,stdin);
    if(strlen(buf) != 0)
    {
        memcpy(command,buf,10);
        res = Tm_SetDateTimeStr(command);
        switch (res)
        {
        case EN_CLOCK_YEAR_ERROR:
            printf("��ݴ���");
            break;
        case EN_CLOCK_MON_ERROR:
            printf("�·ݴ���");
            break;
        case EN_CLOCK_DAY_ERROR:
            printf("���ڴ���");
            break;
        case EN_CLOCK_FMT_ERROR:
            printf("��ʽ����");
            break;
        default:
            break;
        }
    }

    printf("\r\n");
    return true;
}

// ============================================================================
// ���ܣ���ʾ��ǰʱ���������������ʱ��
// ������
// ���أ�
// ��ע��
// ============================================================================
ADD_TO_SHELL_HELP(Time,"��ʾ��ǰʱ���������������ʱ��");
ADD_TO_IN_SHELL bool_t Time(char *param)
{
    s64 nowtime;
    struct tm dtm;
    char command[20];
    int res;

    nowtime = Tm_Time(NULL);
    Tm_LocalTime_r(&nowtime,&dtm);
    Tm_AscTime(&dtm,command);

    printf("\r\n��ǰʱ�䣺%s",command+11);
    printf("\r\n������ʱ�䣺");

    fgets(command+11,9,stdin);
    if(strlen(command+11) != 0)
    {
        res = Tm_SetDateTimeStr(command);
        switch (res)
        {
        case EN_CLOCK_HOUR_ERROR:
            printf("Сʱ����");
            break;
        case EN_CLOCK_MIN_ERROR:
            printf("���Ӵ���");
            break;
        case EN_CLOCK_SEC_ERROR:
            printf("���Ӵ���");
            break;
        case EN_CLOCK_FMT_ERROR:
            printf("��ʽ����");
            break;
        default:
            break;
        }
    }

    printf("\r\n");
    return true;
}

// ============================================================================
// ���ܣ���ʾ�ڴ棬��ʾĳ��ַ��ʼ��һ���ڴ棬ÿ����ʾ16����Ԫ��ֻ��ʾ�������޸�
// ������param -- ����������������Ҫ�����������ÿո����
//               ����1:��ʼ��ַ��10���ƻ�16����(0x��ͷ)
//               ����2:��ʾ�ĵ�Ԫ��
//               ����3:ÿ��Ԫ�ֽ������Ϸ�ֵ��1��2��4��������ֵ�����ش���
//               ����4:����������
// ���أ�true=������ʾ��false=����
// ��ע��
// ============================================================================
ADD_TO_SHELL_HELP(d,"��ȡ�ڴ��������,�����ʽ:d ��ַ ��Ԫ�� ÿ��Ԫ�ֽ���");
ADD_TO_IN_SHELL bool_t d(char *param)
{
    ptu32_t addr;
    uint32_t unit_num,unit_bytes,len;
    char *word_addr,*word_un,*word_ub,*word_trail,*next_param;

    //��ȡ3������
    word_addr = shell_inputs(param,&next_param);
    word_un = shell_inputs(next_param,&next_param);
    word_ub = shell_inputs(next_param,&next_param);
    word_trail = shell_inputs(next_param,&next_param);
    if((word_addr == NULL)||(word_un == NULL)
            ||(word_ub == NULL)||(word_trail != NULL))
    {
        printf("\r\n��ʽ������ȷ��ʽ�ǣ�\r\n>d ��ַ ��Ԫ�� ÿ��Ԫ�ֽ���\r\n");
        return false;
    }
    addr = strtoul(word_addr, (char **)NULL, 0);
    unit_num = strtol(word_un, (char **)NULL, 0);
    unit_bytes = strtol(word_ub, (char **)NULL, 0);
#if (CN_BYTE_BITS == 8)  //�ֽ�λ��=8����������
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
            addr &= ~(ptu32_t)1;//ֻ�ܴ�ż����ʼ
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
            addr &= ~(ptu32_t)3;//ֻ�ܴ�ż����ʼ
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
            addr &= ~(ptu32_t)7;//ֻ�ܴ�ż����ʼ
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
            printf("\r\n��������\r\n");
            return false;
            break;
    }
#elif (CN_BYTE_BITS == 16)  //�ֽ�λ��=16
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
            printf("\r\n��������\r\n");
            return false;
        } break;
    }
#elif (CN_BYTE_BITS == 32)  //�ֽ�λ��=32
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
            printf("\r\n��������\r\n");
            return false;
        } break;
    }
#endif
    printf("\r\n");
    return true;
}

// ============================================================================
// ���ܣ�д��ĳ��ַ��ʼ��һ���ڴ�
// ������param -- ����������������Ҫ�����������ÿո����
//               ����1:��ʼ��ַ��10���ƻ�16����(0x��ͷ)
//               ����2:��ʾ�ĵ�Ԫ��
//               ����3:ÿ��Ԫ�ֽ������Ϸ�ֵ��1��2��4��������ֵ�����ش���
//               ����4:����������
// ���أ�true=������ʾ��false=����
// ��ע��
// ============================================================================
ADD_TO_SHELL_HELP(f,"д���ݵ��ڴ�,�����ʽ��f ��ʼ��ַ  ��Ԫ�� ÿ��Ԫ�ֽ��� �������");
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

#if (CN_BYTE_BITS == 8)  //�ֽ�λ��=8����������
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
            printf("\r\n��������\r\n");
            return false;
            break;
    }
#elif (CN_BYTE_BITS == 16)  //�ֽ�λ��=16
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
            printf("\r\n��������\r\n");
            return false;
        } break;
    }
#elif (CN_BYTE_BITS == 32)  //�ֽ�λ��=32
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
            printf("\r\n��������\r\n");
            return false;
        } break;
    }
#endif
    return true;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
ADD_TO_SHELL_HELP(dis_exshell,"��ʾ�ⲿshell����");
ADD_TO_IN_SHELL bool_t dis_exshell(void)
{
    bool_t i,Ex_shell_num;
    struct exshell_func *ptfuntab;
    struct shell_list *p_Sh_List;
    p_Sh_List = &shells_list;

    ptfuntab = (struct exshell_func *)p_Sh_List->info.Ex_funTab_start;
    Ex_shell_num = (p_Sh_List->info.Ex_funTab_end - p_Sh_List->info.Ex_funTab_start)/sizeof(struct exshell_func);

    printf("\r\n���������е��ⲿshell��������,���� %d��\r\n",Ex_shell_num);
    printf("�ⲿshell������ʽΪ��������  ����1 ����1 ����2 ����2 ����3 ����3...\r\n");
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
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
ADD_TO_SHELL_HELP(dis_globlvar,"��ʾȫ�ֱ���");
ADD_TO_IN_SHELL bool_t dis_globlvar(void)
{
    bool_t i,Global_Var_num;
    struct data_struct *ptDatetab;
    struct shell_list *p_Sh_List;
    p_Sh_List = &shells_list;

    ptDatetab = (struct data_struct *)p_Sh_List->info.dataTab_start;
    Global_Var_num = (p_Sh_List->info.dataTab_end - p_Sh_List->info.dataTab_start)/sizeof(struct data_struct);

    printf("\r\n���������е�ȫ�ֱ���,����%d��\r\n",Global_Var_num);
    printf("��ʾȫ�ֱ���ֵ�ĸ�ʽ��  DataName ����\r\n");
    printf("����ȫ�ֱ���ֵ�ĸ�ʽ��  DataName = ���� num  \r\n");
    printf("���������У�u8/u16/u32/u64/s8/s16/s32/s64/b/f/d/""/'' \r\n");
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
// ���ܣ���ʾ��������İ�����Ϣ
// ������
// ���أ�
// ��ע��
// ============================================================================
static bool_t __help_all(void)
{
    bool_t i,in_shell_num;
    struct inshell_func *pt_funTab;
    struct shell_list *p_Sh_List;
    p_Sh_List = &shells_list;

    printf("\r\n�йؾ����������ϸ��Ϣ��������help [������]\r\n");
    printf("\r\n");

    pt_funTab = (struct inshell_func *)p_Sh_List->info.In_funTab_start;
    in_shell_num = (p_Sh_List->info.In_funTab_end - p_Sh_List->info.In_funTab_start)/sizeof(struct inshell_func);

    for(i=0;i<in_shell_num;i++)
    {

        if(pt_funTab[i].help != NULL)
               printf("%-24s%s", pt_funTab[i].fun_name, *pt_funTab[i].help);
           else
             printf("%-24sû���ṩ��Ҫ������Ϣ", pt_funTab[i].fun_name);

        printf("\r\n");
    }
    printf("\r\n\r\n");
    return true;
}

// ============================================================================
// ���ܣ���ʾ�ļ�ϵͳ�������
// ������param -- �����ַ���������:
// ���أ�
// ��ע��
// ============================================================================
ADD_TO_SHELL_HELP(help,"��ʾ������Ϣ");
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
           printf("���� %s������ ,�����ڰ�����Ϣ !! \n\r",cmd);
           return true;
       }

       if(cmd_class.cmdhelp != NULL)
           printf("\r\n%s\r\n",cmd_class.cmdhelp);
       else
           printf("û���ṩ��ϸ������Ϣ");
       printf("\r\n");
       result = true;
    }

    return result;
}
#endif
