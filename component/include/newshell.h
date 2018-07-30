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
//����ģ��:ʱ��ģ��
//����:  ������
//�汾��V1.0.0
//�ļ�����: Ϊϵͳ�ṩʱ��
//����˵��:
//�޶���ʷ:
// 1. ����: 2011-10-28
//   ����:  ������
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
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



//=============ͨ�ú������ò����뷵��ֵ��shell���������ַ����������ʵ��=========//
typedef void (*Ex_shell_func)(void);
//=============�ڲ���������Ϊ�ַ����ɺ��������������========================//
typedef bool_t (*in_shell_func)(char *param);

struct In_FunTab
{
    const char *const fun_name; //�ڲ�shell��������
    const in_shell_func const fun_addr;//�ڲ�������ַ
    const char **const help;//������Ϣ
};
struct Ex_FunTab
{
    const char *const fun_name;//��ͨshell����
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
    char          *cmdname;//�����Ӧ������
    enum Cmd_type cmdtype; //�����Ӧ������
    void  *cmdaddr;        //�����Ӧ�ĵ�ַ
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

#define PARAMETER_MAX     (10)   //��������������

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
    char          *datastring;   //������Ӧ���ַ����� "100";
    enum ParameterTypr datatype; //�����Ӧ������
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

