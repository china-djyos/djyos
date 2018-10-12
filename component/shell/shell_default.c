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
// Copyright (c) 2018 ����Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
#include <stdio.h>
#include <stddef.h>
#include <version.h>
#include <time.h>
#include <systime.h>
#include "../include/shell.h"

extern struct shell_list shells_list;

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
ADD_TO_SHELL_HELP(time,"��ʾ��ǰʱ���������������ʱ��");
ADD_TO_IN_SHELL bool_t __time(char *param)
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
    extern bool_t __search_cmd(struct commandclass *cmdclass);

    if(param == NULL)
    {
        __help_all();
    }
    else
    {
       cmd = shell_inputs(param,&next_param);
       cmd_class.cmdname = cmd;
       if(__search_cmd(&cmd_class) == FALSE)
       {
           printf("���� %s������ ,�����ڰ�����Ϣ !! \n\r",cmd);
           return (TRUE);
       }

       if(cmd_class.cmdhelp != NULL)
           printf("\r\n%s\r\n",cmd_class.cmdhelp);
       else
           printf("û���ṩ��ϸ������Ϣ");

       printf("\r\n");
    }

    return (TRUE);
}

//----��ʾ����-----------------------------------------------------------------
//���ܣ���ʾ������Ϣ
//��������
//���أ���
//-----------------------------------------------------------------------------
extern struct obj *shell_debug_set;
static void __debug_help_all(void)
{
    struct __shell_debug *current;
    struct obj *shell_ob;


    printf("\r\n�йؾ����������ϸ��Ϣ��������help [������]\r\n");
    printf("\r\n");
    shell_ob = (struct obj *)shell_debug_set;
    char *Name;
    while(1)
    {
        shell_ob = obj_foreach_scion(shell_debug_set,shell_ob);
        if(shell_ob == NULL)
        {
            break;
        }
        else
        {
            //current = (struct ShellCmdRsc *)obj_val(shell_ob);
            current = (struct __shell_debug*)obj_GetPrivate(shell_ob);
            Name = (char*)obj_name(shell_ob);
            if(current->help_hint != NULL)
            {
                //todo��ǰ�����£���һ��%sӦ����%-32s�Ŷԣ���6��ͬ
                printf("%-24s%s", Name, current->help_hint);

            }
            else
            {
                printf("%-24sû���ṩ��Ҫ������Ϣ", Name);
            }
        }
        printf("\r\n");
    }
}

// ============================================================================
// ���ܣ���ʾSHELL�������(debugģʽ��)
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t debug_help(char *param)
{
    char *cmd,*next_param;
    struct obj *shell_ob;
    bool_t result;
    struct  __shell_debug *shell;

    if(param == NULL)
    {
        __debug_help_all();
        result = true;
    }
    else
    {
        cmd = shell_inputs(param,&next_param);
        shell_ob = obj_search_child(shell_debug_set,(const char*)cmd);
        if(shell_ob != NULL)
        {
            //shell = (struct ShellCmdRsc *)obj_val(shell_ob);
            shell = (struct __shell_debug*)obj_GetPrivate(shell_ob);
            if(shell->help_detailed != NULL)
                printf("%s",shell->help_detailed);
            else
                printf("û���ṩ��ϸ������Ϣ");
            printf("\r\n");
            result = true;
        }else
        {
            printf("��Ч������\r\n");
            result = FALSE;
        }
    }
    return result;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
//���������, ����ϵͳ�Դ�������
struct shell_debug const shell_default_debug[] =
{

    {
        "d",
        d,
        "��ȡ�ڴ��������",
        "�����ʽ:d ��ַ ��Ԫ�� ÿ��Ԫ�ֽ���"
    },
    {
        "f",
        f,
        "д���ݵ��ڴ�",
        "�����ʽ��f ��ʼ��ַ  ��Ԫ�� ÿ��Ԫ�ֽ��� �������"
    },
    {
        "help",
        debug_help,
        "DJYOS�����������Ϣ",
        NULL
    },
    {
        "ver",
        ver,
        "��ʾdjyos�İ汾",
        NULL
    },
    {
        "date",
        date,
        "��ʾ������������",
        NULL
    },
    {
        "time",
        __time,
        "��ʾ��ǰʱ���������������ʱ��",
        NULL
    },

//    {
//        "settime",
//        Sh_SetTime,
//        "Diaplay or set current time",
//        "�����ʽ:settime year/month/day, hour:min:sec+Enter"
//    },

//    {
//        "uninstall-cmd",
//        Sh_UninstallCmdByName,
//        "ɾ��shell����--������",
//        NULL
//    },

//    {
//        "cpuinfo",
//        ShowCpuInfo,
//        "usage:cpuinfo:show the cpu",
//        NULL
//    },
};

// ============================================================================
// ���ܣ�
// ������
// ���أ�ʵ������˵�����������
// ��ע��
// ============================================================================

s32 shell_default(void)
{
    s32 commands;
    commands = sizeof(shell_default_debug) / sizeof(struct shell_debug);

    if(commands!=shell_debug_add(shell_default_debug, commands))
        return (-1);

    if(kernel_command())
        return (-1);

    return (0);
}
