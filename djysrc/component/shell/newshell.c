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


//----����µ�shell������shell��-------------------------------------------
//����: ����app��ʱ���������������µ�
//����: pLisTtab
//����:true/false
//��ע:
//---------------------------------------------------------------------
bool_t Sh_InstallTab(struct ShellListTab *pLisTtab)
{
    Lock_MutexPend(shell_lcb,CN_TIMEOUT_FOREVER);
    dListInsertBefore(&Sh_List_head.Node,&pLisTtab->Node);
    Lock_MutexPost(shell_lcb);
    return true;
}

//-----��shell����������ɾ��һ����---------------------------------------------
//����:
//����:
//����:
//��ע:
//---------------------------------------------------------------------
bool_t Sh_uninstallTab(struct ShellListTab *pLisTtab)
{
    Lock_MutexPend(shell_lcb,CN_TIMEOUT_FOREVER);
    dListRemove(&pLisTtab->Node);
    Lock_MutexPost(shell_lcb);
    return true;
}
//----��ȡ����---------------------------------------------------------------
//����: ��buf����ȡһ���ɿո���н����������ĵ��ʣ�next���ڷ�����һ�������׵�ַ��
//      ���û����һ�����ʣ���next=NULL��
//����: buf�����������ַ���
//      next��������һ������ָ��
//����: ��ȡ�ĵ���ָ�룬�ѽ����ʺ���ķָ������ɴ�������'\0'
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


//===========���ַ������ַ��Ҳ�������-1�ҵ������ڱ���е�ƫ��================================
//
//
//
//
//===========================================================================
static bool_t Sh_find(struct ShellListTab *p_Sh_List,struct CmdClass *cmdclass)
{
    //���ַ�����
	u32 i=0;
    s32 left = 0;//����left�з�������
    s32 right;//����right �з�������
    u32 mid;//����right
    int strflag;

    struct In_FunTab *pt_funTab;
    struct Ex_FunTab  *ptfuntab;
    struct DateTab *ptDatetab;
//--------------���ڲ�������-----------------------------------
    pt_funTab = (struct In_FunTab *)p_Sh_List->info.In_funTab_start;
    left = 0;
    right = (p_Sh_List->info.In_funTab_end - p_Sh_List->info.In_funTab_start)/sizeof(struct In_FunTab);
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


//--------------����ͨ������-----------------------------------
    ptfuntab = (struct Ex_FunTab *)p_Sh_List->info.Ex_funTab_start;
    left = 0;
    right = (p_Sh_List->info.Ex_funTab_end - p_Sh_List->info.Ex_funTab_start)/sizeof(struct Ex_FunTab);
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
            cmdclass->cmdaddr = ptfuntab[mid].fun_addr;
            cmdclass->cmdtype = Sh_CmdtypeFun;
            cmdclass->cmd_help=NULL;
            break;
        }
    }
    if(strflag == 0)
        return true;

//--------------�������-----------------------------------
    ptDatetab = (struct DateTab *)p_Sh_List->info.dataTab_start;
    left = 0;
    right = (p_Sh_List->info.dataTab_end - p_Sh_List->info.dataTab_start)/sizeof(struct DateTab);
    right=right-1;
    while(left<=right) //��whileѭ����ֱ����һ��������������
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
//===========��������===============================
//����cmdclass�����ֲ�ȫ������Ϣ
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

//----��������ֵ-------------------------------------------------------
//����: ����dataclass �������dataaddr
//����: data_class �ַ������ݽṹ
//     DataAddr �����
//����: true/false
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
             strcpy(DataAddr->data_pc,data_class->datastring);//TODO �ַ��������������׳���Խ�� '\'δ��ת���ַ���顣
             break;                    //��֧���ն��޸��ַ���
         default:     return false;
    }
    return true;
}


//==============��ӡ�������============================
//������data_class ���ݽṹ
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
//   printf("(char*)DataAddr:%x   (char*)DataAddr str�� %s\r\n\r\n",(char*)DataAddr,(char*)DataAddr);
//   printf(" DataAddr->data_pc: %x   DataAddr->data_pc str: %s\r\n\r\n",DataAddr->data_pc,DataAddr->data_pc);
//       	 printf("%s \r\n",(char*)DataAddr);     // ֧�ִ�ӡ��char test1[]="hello";   ��֧�ִ�ӡ��char*p="hello";
        	 printf("%s \r\n",DataAddr->data_pc);   //��֧�ִ�ӡ��char test1[]="hello";   ֧�ִ�ӡ��char*p="hello";
             break;
         default:     return false;
    }
    return true;
}

//=============���������жϣ������ַ���ת���ɱ�־=============================
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

//================��ഫ�β�������Ӧ�ĺ���==================================
//���ܣ����ݲ����ĸ������������͵��û�ഫ�ݲ��������ú���
//������num�����ĸ���
//    fun �����ú����ĵ�ַ
//���أ�true/false
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
//----��ǰ����·����ָ�븳ֵ-----------------------------------------------------
//����: ����ָ�븳ֵ����ָ��ָ���ȡ��ǰ����·���ĺ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
//void Sh_PrintWorkPathSet(void)
//{
//    fng_pPrintWorkPath = (void*)Sh_PrintWorkPath;
//}

//----ִ�п���̨����---------------------------------------------------
//����: ������ִ�п���̨����
//����: ��
//����: ��
//��ע:
//---------------------------------------------------------------------
static bool_t Sh_ExecCommand(char *buf)
{
    bool_t result = false;
    char *cmd,*next_param,*word,*wordbak;
    u8 i;
    struct CmdClass cmd_class;
    struct DataClass data_class;
    in_shell_func cmd_fun;

    //�������ƶ�ȡ255�ַ�����������ʾ�������С�
    if(strnlen(buf, CN_SHELL_CMD_LIMIT+1) > CN_SHELL_CMD_LIMIT)
    {
        printf("�����ַ���̫��\r\n");
    }
    cmd = NewSh_GetWord(buf,&next_param);//��ȡ�������߱�����
    cmd_class.cmdname=cmd;

   if(Sh_SearchCmd(&cmd_class) == false)
   {
       printf("���� %s ������  !! \n\r",cmd);
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
                word = NewSh_GetWord(next_param,&next_param);
                if(word != NULL)
                    ParameterFlagTab[i] = Type_str2flag(word);
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

                word = NewSh_GetWord(next_param,&next_param);
                if(word != NULL)
                {
                    data_class.datastring = word;
                    data_class.datatype = ParameterFlagTab[i];
                    variable_assignment(&data_class,&ParameterTab[i]);
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

            result = _AsmExecuteCmd(i,(Ex_shell_func)cmd_class.cmdaddr);
            break;

        case Sh_Cmdtype_Date:   //�����жϱ������Ͳ���ֵ���ӡ todo ��ȫ����Ϊ�ַ����Ľ���
            word = NewSh_GetWord(next_param,&next_param);
            if(word == NULL)
            {
               printf("��ȷ��ʽΪ��1�� DataName = ����  num  2�� DataName  ����\r\n");
               printf("���������У�u8/u16/u32/u64/s8/s16/s32/s64/b/f/d/""/''\r\n");
                break;
             }

            if(strcmp(word,"=") == 0)//������ֵ
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
                    printf("��ȷ��ʽΪ DataName = ����  num \n\r"\
                        "����Ϊ u8 u16 u32 s8 s16 s32 b f d "" ''\n\r");
                    return false;
                }
                data_class.datastring = wordbak;
                result = variable_assignment(&data_class,(union Parameter *)cmd_class.cmdaddr);
            }else
            {
                data_class.datatype = Type_str2flag(word);
                if( data_class.datatype == flag_error)
                {
                    printf("shell �����������ʹ��� ����\n\r");
                    printf("���������У�u8/u16/u32/u64/s8/s16/s32/s64/b/f/d/""/''\r\n");
                    return false;
                }



                result = ShowData(&data_class,(union Parameter *)cmd_class.cmdaddr);
            }
            break;

        default :         break;
    }


    return result;
}
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
                if(Lock_MutexPend(shell_lcb,CN_TIMEOUT_FOREVER))
                {
                    Sh_ExecCommand(command);  //ִ������
                    Lock_MutexPost(shell_lcb);
                }
            }
            if ((fng_pPrintWorkPath != NULL))
                fng_pPrintWorkPath( );
            printf(">");
        }
    }
}



//----����ģ���ʼ������-------------------------------------------------------
//����: ��ʼ������ģ��
//����: ��
//����: 1
//-----------------------------------------------------------------------------

ptu32_t ModuleInstall_NewSh(ptu32_t para)
{
    u16 shell_evtt;

    dListInit(&Sh_List_head.Node);
    Sh_List_head.info = *r_shell_info;

    shell_lcb = Lock_MutexCreate("shell");
    if(NULL == shell_lcb)
        return false;
    shell_evtt = Djy_EvttRegist(EN_CORRELATIVE,  //�������¼�
                                CN_PRIO_REAL,       //Ĭ�����ȼ�
                                0,                  //�̱߳���������������Ч
                                0,                  //�߳����ޣ���������Ч
                                Sh_Service,         //��ں���
                                NULL,               //��ϵͳ����ջ
                                0x1000,             //ջ�ߴ�
                                "shell"             //�¼�������
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



//��ʾdjyos�İ汾

ADD_TO_SHELL_HELP(ver,"��ʾdjyos�İ汾");
ADD_TO_IN_SHELL bool_t ver(char *param)
{
    printf("\r\n%s\r\n", djyos_kernel_version);
    return true;
}



 //��ʾ������������

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


//��ʾ��ǰʱ���������������ʱ��

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


//----��ʾ�ڴ�-----------------------------------------------------------------
//����: ��ʾĳ��ַ��ʼ��һ���ڴ棬ÿ����ʾ16����Ԫ��ֻ��ʾ�������޸�
//����: param������������������Ҫ�����������ÿո����
//          ����1:��ʼ��ַ��10���ƻ�16����(0x��ͷ)
//          ����2:��ʾ�ĵ�Ԫ��
//          ����3:ÿ��Ԫ�ֽ������Ϸ�ֵ��1��2��4��8��������ֵ�����ش���
//����: true=������ʾ��false=����
//-----------------------------------------------------------------------------
ADD_TO_SHELL_HELP(d,"��ȡ�ڴ��������,�����ʽ:d ��ַ ��Ԫ�� ÿ��Ԫ�ֽ���");
ADD_TO_IN_SHELL bool_t d(char *param)
{
    ptu32_t addr;
    uint32_t unit_num,unit_bytes,len;
    char *word_addr,*word_un,*word_ub,*word_trail,*next_param;

    //��ȡ3������
    word_addr = NewSh_GetWord(param,&next_param);
    word_un = NewSh_GetWord(next_param,&next_param);
    word_ub = NewSh_GetWord(next_param,&next_param);
    word_trail = NewSh_GetWord(next_param,&next_param);
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


//----д���ڴ�-----------------------------------------------------------------
//����: д��ĳ��ַ��ʼ��һ���ڴ�
//����: param������������������Ҫ�����������ÿո����
//          ����1:��ʼ��ַ��10���ƻ�16����(0x��ͷ)
//          ����2:��ʾ�ĵ�Ԫ��
//          ����3:ÿ��Ԫ�ֽ������Ϸ�ֵ��1��2��4��������ֵ�����ش���
//          ����4:����������
//����: true=������ʾ��false=����
//-----------------------------------------------------------------------------

ADD_TO_SHELL_HELP(f,"д���ݵ��ڴ�,�����ʽ��f ��ʼ��ַ  ��Ԫ�� ÿ��Ԫ�ֽ��� �������");
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


ADD_TO_SHELL_HELP(dis_exshell,"��ʾ�ⲿshell����");
ADD_TO_IN_SHELL bool_t dis_exshell(void)
{
	bool_t i,Ex_shell_num;
	struct Ex_FunTab *ptfuntab;
    struct ShellListTab *p_Sh_List;
    p_Sh_List = &Sh_List_head;

    ptfuntab = (struct Ex_FunTab *)p_Sh_List->info.Ex_funTab_start;
    Ex_shell_num = (p_Sh_List->info.Ex_funTab_end - p_Sh_List->info.Ex_funTab_start)/sizeof(struct Ex_FunTab);

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

ADD_TO_SHELL_HELP(dis_globlvar,"��ʾȫ�ֱ���");
ADD_TO_IN_SHELL bool_t dis_globlvar(void)
{
	bool_t i,Global_Var_num;
	struct DateTab *ptDatetab;
    struct ShellListTab *p_Sh_List;
    p_Sh_List = &Sh_List_head;

    ptDatetab = (struct DateTab *)p_Sh_List->info.dataTab_start;
    Global_Var_num = (p_Sh_List->info.dataTab_end - p_Sh_List->info.dataTab_start)/sizeof(struct DateTab);

    printf("\r\n���������е�ȫ�ֱ���,����%d��\r\n",Global_Var_num);
    printf("��ʾȫ�ֱ���ֵ�ĸ�ʽ��  DataName ����\r\n");
    printf("����ȫ�ֱ���ֵ�ĸ�ʽ��  DataName = ���� num  \r\n");
    printf("���������У�u8/u16/u32/u64/s8/s16/s32/s64/b/f/d/""/'' \r\n");
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

    printf("\r\n�йؾ����������ϸ��Ϣ��������help [������]\r\n");
    printf("\r\n");

    pt_funTab = (struct In_FunTab *)p_Sh_List->info.In_funTab_start;
    in_shell_num = (p_Sh_List->info.In_funTab_end - p_Sh_List->info.In_funTab_start)/sizeof(struct In_FunTab);

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
//----��ʾ�ļ�ϵͳ�������-----------------------------------------------------
//����:
//����: �����ַ���������:
//����: true
//-----------------------------------------------------------------------------

ADD_TO_SHELL_HELP(help,"��ʾ������Ϣ");
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
	       printf("���� %s������ ,�����ڰ�����Ϣ !! \n\r",cmd);
	       return true;
	   }

	   if(cmd_class.cmd_help != NULL)
           printf("\r\n%s\r\n",cmd_class.cmd_help);
       else
           printf("û���ṩ��ϸ������Ϣ");
       printf("\r\n");
       result = true;

   }
    return result;
}












