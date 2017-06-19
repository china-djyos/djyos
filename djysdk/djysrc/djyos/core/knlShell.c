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
//����ģ��:����ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:�ں˵�shell���
//����˵��:
//�޶���ʷ:
//1. ����: 2016-04-26
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include "stdint.h"
#include "board-config.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "object.h"
#include "lock.h"
#include "int.h"
#include "systime.h"
#include "djyos.h"
#include "shell.h"
#include "exp.h"
#include "core_config.h"


extern struct EventECB *g_ptECB_Table;
extern struct EventType *g_ptEvttTable;
extern struct EventECB  *s_ptEventFree; //��������ͷ,������
extern s64 g_s64RunningStartTime;        //��ǰ�������¼��Ŀ�ʼִ��ʱ��.

void __M_ShowHeap(void);
void __M_ShowHeapSpy(void);
void __Lock_ShowLock(void);

bool_t Sh_ShowEvent(char *param);
bool_t Sh_ShowEvtt(char *param);
bool_t Sh_ShowStack(char *param);
//----��ʾ�¼���---------------------------------------------------------------
//����: ��ʾ�¼��б�
//����: ��
//����: true
//-----------------------------------------------------------------------------
bool_t Sh_ShowEvent(char *param)
{
    u16 pl_ecb;
    u32 time1,MemSize;

    MemSize = 0;
    printf("�¼���  ��������  ���ȼ� CPU  ջ�ߴ�\n\r");
    for(pl_ecb = 0; pl_ecb < gc_u32CfgEventLimit; pl_ecb++)
    {
        if(g_ptECB_Table[pl_ecb].previous !=
                        (struct EventECB*)&s_ptEventFree)
        {
            printf("%05d   %05d     ",pl_ecb,g_ptECB_Table[pl_ecb].evtt_id &(~CN_EVTT_ID_MASK));
            printf("%03d    ",g_ptECB_Table[pl_ecb].prio);
            time1 = g_ptECB_Table[pl_ecb].consumed_time_second/10000;
           // printf("%02d%%  %8x",time1,g_ptECB_Table[pl_ecb].vm->stack_size);
            if(NULL == g_ptECB_Table[pl_ecb].vm)
            {
               printf("Not run yet!");
            }
            else
            {
                printf("%02d%%  %08x",time1,g_ptECB_Table[pl_ecb].vm->stack_size);
                MemSize += g_ptECB_Table[pl_ecb].vm->stack_size;
            }
            printf("\n\r");
        }
        else
        {
//          printf("%5d   ����",pl_ecb);
        }
    }
    printf("�����¼�ջ�ߴ��ܼ�:           %08x\n\r",MemSize);
    printf("������¼�����:              %d\n\r",gc_u32CfgEventLimit);

    return true;
}

//----��ʾ�¼����ͱ�-----------------------------------------------------------
//����: ��ʾ�¼������б�
//����: ��
//����: true
//-----------------------------------------------------------------------------
bool_t Sh_ShowEvtt(char *param)
{
    u16 pl_ecb;
    u32 MemSize;
    MemSize = 0;
    printf("���ͺ�  ���ȼ� ������  ջ����   ����\n\r");
    for(pl_ecb = 0; pl_ecb < gc_u32CfgEvttLimit; pl_ecb++)
    {
        if(g_ptEvttTable[pl_ecb].property.registered ==1)
        {
            MemSize += g_ptEvttTable[pl_ecb].stack_size;
            printf("%05d   ",pl_ecb);
            printf("%03d    ",g_ptEvttTable[pl_ecb].default_prio);
            printf("%08x  %08x ",
                        (ptu32_t)g_ptEvttTable[pl_ecb].thread_routine,
                        g_ptEvttTable[pl_ecb].stack_size);
            if(g_ptEvttTable[pl_ecb].evtt_name[0] != '\0')
                printf("%s",g_ptEvttTable[pl_ecb].evtt_name);
            else
                printf("����");
            printf("\n\r");
        }
        else
        {
//          printf("%05d   ����",pl_ecb);
        }
    }
    printf("��������ջ�����ܼ�:      %08x\n\r",MemSize);
    printf("������¼���������:      %d\n\r",gc_u32CfgEvttLimit);
    return true;
}

//----��ʾջʹ�����-----------------------------------------------------------
//����: ��ʾϵͳ�������Ѿ������̵߳��¼���ջ��Ϣ
//����: ��
//����: true
//-----------------------------------------------------------------------------
bool_t Sh_ShowStack(char *param)
{
    u16 pl_ecb;
    u32 loop,StackSize,pads;
    u32 *stack;

    printf("�¼��� �߳�   ջ��     ջָ��   ջ�ߴ�   ������   ������\n\r");
    for(pl_ecb = 0; pl_ecb < gc_u32CfgEventLimit; pl_ecb++)
    {

        if(g_ptECB_Table[pl_ecb].previous !=
                        (struct EventECB*)&s_ptEventFree)
        {
            printf("%05d  ",pl_ecb);
            if(g_ptECB_Table[pl_ecb].vm)
                printf("�ѷ��� ");
            else
                printf("δ���� ");
            StackSize = g_ptECB_Table[pl_ecb].vm->stack_size;
            pads = 0x64646464;
            stack = (u32*)(&(g_ptECB_Table[pl_ecb].vm[1]));
            for(loop = 0; loop < (StackSize>>2)-1; loop++)
            {
                if(stack[loop] != pads)
                {
                    break;
                }
            }
            printf("%08x %08x %08x %08x %s",
                        (ptu32_t)(&g_ptECB_Table[pl_ecb].vm[1]),
                        (ptu32_t)(g_ptECB_Table[pl_ecb].vm->stack),
                        StackSize,StackSize-(loop<<2),
                        g_ptEvttTable[g_ptECB_Table[pl_ecb].evtt_id&(~CN_EVTT_ID_MASK)].evtt_name);
            printf("\n\r");
        }
        else
        {
//            printf("%05d  �����¼����ƿ�",pl_ecb);
        }
    }
    printf("ջָ�������һ���������л�ʱ�����ֵ\n\r");

    return true;
}

struct ShellCmdTab const tg_ShellKernelCmdTbl[] =
{
   {
        "event",
        Sh_ShowEvent,
        "��ʾ�¼���",
        NULL
    },
    {
        "evtt",
        Sh_ShowEvtt,
        "��ʾ�¼����ͱ�",
        NULL
    },
    {
        "heap",
        (bool_t (*)(char*))__M_ShowHeap,
        "��ʾ��ʹ�����",
        NULL
    },
    {
        "heap-spy",
        (bool_t (*)(char*))__M_ShowHeapSpy,
        "��ʾ��̬�ڴ���ϸ�������",
        NULL
    },
    {
        "stack",
        (bool_t (*)(char*))Sh_ShowStack,
        "��ʾϵͳ�������Ѿ������̵߳��¼���ջ��Ϣ",
        NULL
    },
    {
        "lock",
        (bool_t (*)(char*))__Lock_ShowLock,
        "��ʾϵͳ�������ź����ͻ���������Ϣ",
        NULL
    },
};

static struct ShellCmdRsc tg_ShellKernelCmd
                        [sizeof(tg_ShellKernelCmdTbl)/sizeof(struct ShellCmdTab)];

ptu32_t Debug_Scan(void)
{
    u32 pl_ecb;

    while(1)
    {

        for(pl_ecb = 0; pl_ecb < gc_u32CfgEventLimit; pl_ecb++)
        {
            g_ptECB_Table[pl_ecb].consumed_time_second =
                              (u32)g_ptECB_Table[pl_ecb].consumed_time
                            - g_ptECB_Table[pl_ecb].consumed_time_record;
            g_ptECB_Table[pl_ecb].consumed_time_record =
                            (u32)g_ptECB_Table[pl_ecb].consumed_time;
        }
        Djy_EventDelay(1000*mS);
    }
}

//----��ʼ��������Ϣģ��-------------------------------------------------------
//����: ����������Ϣ�¼����Ͳ�����֮
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void ModuleInstall_DebugInfo(ptu32_t para)
{
    u16 evtt_debug;
    para = para;        //�����������澯
    evtt_debug = Djy_EvttRegist(EN_CORRELATIVE,1,0,0,
                                 Debug_Scan,NULL,500,"debug_info");
    if(evtt_debug == CN_EVTT_ID_INVALID)
        return;
    Sh_InstallCmd(tg_ShellKernelCmdTbl,tg_ShellKernelCmd,
            sizeof(tg_ShellKernelCmdTbl)/sizeof(struct ShellCmdTab));
    Djy_EventPop(evtt_debug,NULL,0,0,0,0);
}


