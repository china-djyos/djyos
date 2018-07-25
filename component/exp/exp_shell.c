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
// ģ������: SysExp_Shell.c
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 9:20:36 AM/Mar 27, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע:���ļ���Ҫ��Ϊ��ʵ��SHELL��һЩ�ӿڹ���


#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "exp.h"
#include "exp_record.h"
#include "shell.h"
#include "systime.h"
#include "dbug.h"
#include "newshell.h"
bool_t  Exp_InfoDecoder(struct ExpRecordPara *recordpara);

extern char *Sh_GetWord(char *buf,char **next);
// =============================================================================
// �������ܣ�Exp_ShellBrowseAssignedRecord
//           shell�鿴ָ����Ŀ���쳣��Ϣ
// ���������param,ָ����Ŀ�ַ���
// �����������
// ����ֵ  ��true�ɹ�  falseʧ��
// ˵��    ��
// =============================================================================
//bool_t Exp_ShellBrowseAssignedRecord(char *param)
ADD_TO_SHELL_HELP(expi,"browse the specified exception item");
ADD_TO_IN_SHELL bool_t expi(char *param)
{
    bool_t   result;
    char *word,*next_param;
    u32  item_num;
    u32  item_len;
    u8   *infomem;
    struct ExpRecordPara recordpara;

    if(NULL == param)
    {
        result = false;
        goto browse_end;
    }

    word = Sh_GetWord(param,&next_param);
    item_num = strtol(word, (char **)NULL, 0);

    result = Exp_RecordCheckLen(item_num, &item_len);
    if(false == result)
    {
        debug_printf("shell","��ָ����ȷ����Ŀ��\n\r");
        return true;
    }

    infomem = M_MallocLc(item_len, CN_TIMEOUT_FOREVER);
    if(NULL == infomem)
    {
        debug_printf("shell","��ָ����ȷ����Ŀ��\n\r");
        return true;
    }

    result = Exp_RecordGet(item_num,item_len,infomem, &recordpara);
    if(false == result)
        debug_printf("shell","��ȡ�쳣��Ϣʧ��\n\r");
    else
        Exp_InfoDecoder(&recordpara);

    free(infomem);
    return true;

browse_end:
    return result;

}
// =============================================================================
// �������ܣ�Exp_ShellBrowseRecordNum
//           shell�鿴�洢���쳣��Ŀ��
// ���������param,��
// �����������
// ����ֵ  ��true�ɹ�  falseʧ��
// ˵��    ��
// =============================================================================
//bool_t Exp_ShellBrowseRecordNum(char *param)
ADD_TO_SHELL_HELP(expn,"browse the exception number");
ADD_TO_IN_SHELL bool_t expn(char *param)
{
    bool_t  result;

    u32 expnum;

    result = Exp_RecordCheckNum(&expnum);
    if(true == result)
    {
        debug_printf("shell","ShellSysExp:There has been 0x%08x Exp record!\n\r",expnum);
        if(expnum > 0)
        {
            debug_printf("shell","ShellSysExp:VALID NO.(1-%d)\n\r",expnum);
        }
        else
        {
            debug_printf("shell","There has been no exception record yet!\n\r");
        }
    }
    else
        debug_printf("shell","Get Exception Number Failed\n\r");
    return result;
}

// =============================================================================
// �������ܣ�Exp_ShellRecordClear
//           shell������е��쳣��Ϣ
// ���������param,��
// �����������
// ����ֵ  ��true�ɹ�  falseʧ��
// ˵��    ��
// =============================================================================
//bool_t Exp_ShellRecordClear(char *param)

ADD_TO_SHELL_HELP(expc,"clear all the exception item");
ADD_TO_IN_SHELL bool_t expc(char *param)
{
    bool_t  result;

    result = Exp_RecordClear();

    if(true == result)
    {
        debug_printf("shell","ShellSysExp:Clear SysExp info success!");
    }
    else
    {
        debug_printf("shell","ShellSysExp:Clear SysExp info failed!");
    }

    return result;
}
//struct ShellCmdTab  gtExpShellCmdTab[] =
//{
//    {
//        "expi",
//        Exp_ShellBrowseAssignedRecord,
//        "browse the specified exception item",
//        "expi item\n\r"
//    },
//    {
//        "expn",
//        Exp_ShellBrowseRecordNum,
//        "browse the exception number",
//        "expn"
//    },
//    {
//        "expc",
//        Exp_ShellRecordClear,
//        "clear all the exception item",
//        "expc"
//    }
//};
//#define CN_EXPSHELL_NUM  ((sizeof(gtExpShellCmdTab))/(sizeof(struct ShellCmdTab)))
//static struct ShellCmdRsc sgExpShellRsc[CN_EXPSHELL_NUM];
// =============================================================================
// �������ܣ�Exp_ShellInit
//          expshellģ���ʼ��
// ���������param,��
// �����������
// ����ֵ  ��true�ɹ�  falseʧ��
// ˵��    ��
// =============================================================================
//bool_t Exp_ShellInit()
//{
//    bool_t result;
//    if(Sh_InstallCmd(gtExpShellCmdTab,sgExpShellRsc,CN_EXPSHELL_NUM))
//    {
//        info_printf("module","exception installed.");
//        result = true;
//    }
//    else
//    {
//        error_printf("shell","exception installed failed<shell>.");
//        result = false;
//    }
//    return result;
//}

