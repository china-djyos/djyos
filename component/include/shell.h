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

#ifndef __SHELL_H__
#define __SHELL_H__

#include <stdint.h>
#include <errno.h>
#include <types.h>
#include <stddef.h>
#include <list.h>

#ifdef __cplusplus
extern "C" {
#endif

struct shellinfo
{
   const char *sh_Tab_start; //
   const char *sh_Tab_end; //
};

struct shell_list
{
    list_t list;
    struct shellinfo info;
};
#define PARAMETER_MAX     (10)   //��������������

/* ����shell��������չshell�����������ڣ������Ľ��������ں����ڻ��Ǻ�����
 * ����shell����������Ϣ�볣��shell����������Ҫ����ü��йأ��ڹ�����ָ�������Щ���͵�shell
 * */
/*shell ����չshell*/

struct shell_cmd
{
    void *shell_fun_addr;
    char *shell_help_addr;
};
#define DJYSH_HELP_NAME "djysh_"

#if(CFG_MODULE_ENABLE_SHELL == true)
#define ADD_TO_ROUTINE_SHELL(cmdname,fun,help) \
                __attribute__((section(".ro_shell_cmd")))\
                const struct shell_cmd djysh_##cmdname = \
                        {.shell_fun_addr  = fun,.shell_help_addr = help,}

#define ADD_TO_EXPAND_SHELL(cmdname,fun,help) \
                __attribute__((section(".ex_shell_cmd")))\
                const struct shell_cmd djysh_##cmdname = \
                        {.shell_fun_addr  = fun,.shell_help_addr = help,}

//�÷�����ADD_TO_IN_SHELL_DATA u32 sh;��      //�� sh ����Ϊ����shell�з��ʵı���
#define ADD_TO_IN_SHELL_DATA  __attribute__((section(".ro_shell_data")))//��ӳ���shell����
#define ADD_TO_EX_SHELL_DATA    __attribute__((section(".ex_shell_data")))//��չshell����

#else   //for #if(CFG_MODULE_ENABLE_SHELL == true)

//��û�й�ѡshell�������Щ��ȫ���ÿ�
#define ADD_TO_ROUTINE_SHELL(cmdname,fun,help)
#define ADD_TO_EXPAND_SHELL(cmdname,fun,help)
#define ADD_TO_IN_SHELL_DATA
#define ADD_TO_EX_SHELL_DATA

#endif   //for #if(CFG_MODULE_ENABLE_SHELL == true)

// ============================================================================
// ���ܣ���shell������������ַ������У���ȡ��һ��������
// ������input -- shell���������
//      next -- ����������
// ���أ��޳������������
// ��ע������֮�������ո���Ϊ�ָ�����
//      ��ȡ���Ĳ������ѽ�����ķָ������ɴ�������'\0��
//      input��ԭʼ�����ѷ����仯��
// ============================================================================
char *shell_inputs(char *input, char **next);

bool_t shell_add(struct shell_list *pLisTtab);
bool_t shell_del(struct shell_list *pLisTtab);

s32 ModuleInstall_Shell(ptu32_t para);

#ifdef __cplusplus
}
#endif

#endif   //for #ifndef __SHELL_H__

