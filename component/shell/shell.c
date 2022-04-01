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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"stdio"            //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������
//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_SHELL == false )
//#warning  " shell  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_SHELL    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,0x10000,
#define CFG_SHELL_STACK            0x1000      //"ִ��shell�����ջ�ߴ�",
//%$#@enum,true,false,
#define CFG_ADD_ROUTINE_SHELL      true        //"�Ƿ���ӳ���shell����",
#define CFG_ADD_EXPAND_SHELL       true        //"�Ƿ������չshell����",
#define CFG_ADD_GLOBAL_FUN         false       //"���ȫ�ֺ�����shell",
#define CFG_SHOW_ADD_SHEELL        true        //"��ʾ�ڱ��봰����ӵ�shell����",
//%$#@string,1,10,
//%$#@SYMBOL        ***�������ý��棬ֱ�Ӷ������
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
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
//=============ͨ�ú������ò����뷵��ֵ��shell���������ַ����������ʵ��=========//
typedef void (*Ex_shell_func)(void);
//=============�ڲ���������Ϊ�ַ����ɺ��������������========================//
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
    char  *datastring; // ������Ӧ���ַ����� "100";
    enum param_typr datatype; // �����Ӧ������
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
    #define    SH_CMDTYPE_ROUTINE_FUN         0   //����sh����
    #define    SH_CMDTYPE_ROUTINE_DATA        1   //����sh����
    #define    SH_CMDTYPE_EXPAND_FUN          2   //��չshell����
    #define    SH_CMDTYPE_EXPAND_DATA         3   //��չshell����
    #define    SH_CMDTYPE_GLOBAL_FUN          4   //ȫ�ֺ���
    #define    SH_CMDTYPE_GLOBAL_DATE         5   //ȫ������
    int   cmdtype;
};


#define CN_SHELL_CMD_LIMIT 255 // shell �����������
extern struct shellinfo p_shell_info; // p_shell_info ����iboot.lds�ļ�
static struct MutexLCB *__shell_mutex;//shellִ�еĻ���
static struct shell_list shells_list;

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
// ���ܣ���������, ����cmdclass�����ֲ�ȫ������Ϣ.
// ������
// ���أ�
// ��ע��
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
// ���ܣ���������ֵ,����dataclass �������dataaddr
// ������data_class -- �ַ������ݽṹ
//      DataAddr -- �����
// ���أ�
// ��ע��
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
             //TODO �ַ��������������׳���Խ�� '\'δ��ת���ַ���顣
             strcpy(DataAddr->data_pc,data_class->datastring);
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
    else if((word[0] == '\'') && (word[strlen(word)-1] =='\'' ) && (strlen(word)==2))
        datatype = flag_c;
    else if((word[0] == '\"') && (word[strlen(word)-1] =='\"' ) )//""" == 0x22;
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

        printf("�������\n\r");
        printf("%-20s  %-12s   %-20s\n\r","������","������","����");
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
        printf("\n\r��չ���\n\r");
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

    if(param)//�����澯
        return flag;
    return flag;
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
    char *cmdname,*next_param,*word,*wordbak;
    u8 i;
    struct dataclass data_class;
    in_shell_func inshfun;
    Ex_shell_func exshfun;
    bool_t help_flag = false;

    //�������ƶ�ȡ255�ַ�����������ʾ�������С�
    if(strnlen(buf, CN_SHELL_CMD_LIMIT+1) > CN_SHELL_CMD_LIMIT)
    {
        printf("�����ַ���̫��\r\n");
        return false;
    }
    cmdname = shell_inputs(buf, &next_param);//��ȡ�������߱�����
    if(strcmp("help",cmdname)==0)//����ǰ�����Ϣ��ƴ�ӳ��µ�����
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
        printf("���� %s ������  !! \n\r",cmdname);
        return false;
    }

    switch (sh_sym->cmdtype)
    {
        case SH_CMDTYPE_ROUTINE_FUN :       //����sh����
            if(help_flag)
            {
                printf("%s", sh_sym->cmdaddr->shell_help_addr);
            }
            else
            {
                inshfun = (in_shell_func)sh_sym->cmdaddr->shell_fun_addr;
                result = inshfun(next_param);
                if(result == false)
                    printf("shell �ڲ�����ִ�д��� ����\n\r");
            }
            break;
        case SH_CMDTYPE_ROUTINE_DATA:     //����sh����
        case SH_CMDTYPE_EXPAND_DATA :     //��չshell����
        case SH_CMDTYPE_GLOBAL_DATE :     //ȫ������
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
                result = __variable_assignment(&data_class,(union param *)sh_sym->cmdaddr);
            }else
            {
                data_class.datatype = __str2type(word);
                if( data_class.datatype == flag_error)
                {
                    printf("shell �����������ʹ��� ����\n\r");
                    printf("���������У�u8/u16/u32/u64/s8/s16/s32/s64/b/f/d/""/''\r\n");
                    return false;
                }
                result = __show_data(&data_class,(union param *)sh_sym->cmdaddr);
            }
            break;
        case SH_CMDTYPE_EXPAND_FUN  :      //��չshell����
        case SH_CMDTYPE_GLOBAL_FUN  :     //ȫ�ֺ���
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
                    printf("shell ����ȱ�ٲ������� ����\n\r");
                    printf("���������У�u8/u16/u32/u64/s8/s16/s32/s64/b/f/d\n\r");
                    break;
                }
                if(ParameterFlagTab[i-1] == flag_error)
                {
                    printf("shell �����������ʹ��� ����\n\r");
                    printf("���������У�u8/u16/u32/u64/s8/s16/s32/s64/b/f/d\n\r");
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
                    printf("shell ������ʽ���� ����\n\r");
                    printf("shell������ʽΪ��������  ����1 ����1 ����2 ����2 ����3 ����3...\r\n");
                    return false;
                }

                if(next_param==NULL)
                       break;
                else if((next_param!=NULL)&&(i==(PARAMETER_MAX-1)))
                {
                     i=i+2; //Ҫִ�������return false;
                     break;
                }
            }
            if((i == PARAMETER_MAX) )
            {
                 printf("�β��������10�������ܳ���10��\r\n");
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
//����: ����console������ַ�����console����س���ʱ��ִ�����һ������ó���
//      255�ַ���
//����: ��
//����: ��
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
             DJY_EventDelay(1000); // ��ȡ���ݴ������end of file����ʱ1ms�ټ�������ֹ������ѭ�����󣬵��������߳̿�������
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
// ���ܣ�shellģ���ʼ��
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 ModuleInstall_Shell(ptu32_t para)
{
    u16 shell_evtt;

    dListInit(&shells_list.list);
    shells_list.info =  p_shell_info;
    if((u32)p_shell_info.sh_Tab_start < (u32)&p_shell_info)
    {
        printf("\n\r error :  δ���shell��Ϣ shell ����ʧ�� !!\n\r");
        return false;
    }
    __shell_mutex = Lock_MutexCreate("shell");
    if(NULL == __shell_mutex)
        return (-1);

    shell_evtt = DJY_EvttRegist(EN_CORRELATIVE, // �������¼�
                                2, // Ĭ�����ȼ�
                                0, // �̱߳���������������Ч
                                0, // �߳����ޣ���������Ч
                                Sh_Service, // ��ں���
                                NULL, // ��ϵͳ����ջ
                                CFG_SHELL_STACK,
                                "shell" // �¼�������
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
ADD_TO_ROUTINE_SHELL(help,shell_help,"������Ϣ��ʽ :help [cmd]");
