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
#include "dbug.h"
#include "component_config_core.h"
#include "newshell.h"
extern struct EventECB g_tECB_Table[];
extern struct EventType g_tEvttTable[];
extern struct EventECB  *s_ptEventFree; //��������ͷ,������

//void __M_ShowHeap(void);
//void __M_ShowHeapSpy(void);

//bool_t Sh_ShowEvent(char *param);
//bool_t Sh_ShowEvtt(char *param);
//bool_t Sh_ShowStack(char *param);
//----��ʾ�¼���---------------------------------------------------------------
//����: ��ʾ�¼��б�
//����: ��
//����: true
//-----------------------------------------------------------------------------
//bool_t Sh_ShowEvent(char *param)
ADD_TO_SHELL_HELP(event,"��ʾ�¼���");
ADD_TO_IN_SHELL bool_t event(char *param)
{
    u16 pl_ecb;
    u32 time1,MemSize;
    char *name;

    MemSize = 0;
    debug_printf("knlshell","�¼���  ���ͺ�  ���ȼ� CPU  ջ�ߴ�   ������\n\r");
    for(pl_ecb = 0; pl_ecb < CFG_EVENT_LIMIT; pl_ecb++)
    {
        if(g_tECB_Table[pl_ecb].previous !=
                        (struct EventECB*)&s_ptEventFree)
        {
            debug_printf("knlshell","%05d %05d     ",pl_ecb,g_tECB_Table[pl_ecb].evtt_id &(~CN_EVTT_ID_MASK));
            debug_printf("knlshell","%03d    ",g_tECB_Table[pl_ecb].prio);
           // debug_printf("knlshell","%02d%%  %8x",time1,g_tECB_Table[pl_ecb].vm->stack_size);
#if CFG_OS_TINY == false
            time1 = g_tECB_Table[pl_ecb].consumed_cnt_second/10000;
            name = g_tEvttTable[g_tECB_Table[pl_ecb].evtt_id&(~CN_EVTT_ID_MASK)].evtt_name;
#else
            time1 = 0;
            name = "unkown";
#endif  //CFG_OS_TINY == false
            if(NULL == g_tECB_Table[pl_ecb].vm)
            {
                debug_printf("knlshell","%02d%%  %08x %s",0,0,name);
            }
            else
            {
                debug_printf("knlshell","%02d%%  %08x %s",time1,g_tECB_Table[pl_ecb].vm->stack_size,name);
                MemSize += g_tECB_Table[pl_ecb].vm->stack_size;
            }
            debug_printf("knlshell","\n\r");
        }
        else
        {
//          debug_printf("knlshell","%5d   ����",pl_ecb);
        }
    }
    debug_printf("knlshell","�����¼�ջ�ߴ��ܼ�:           %08x\n\r",MemSize);
    debug_printf("knlshell","������¼�����:              %d\n\r",CFG_EVENT_LIMIT);

    return true;
}

//----��ʾ�¼����ͱ�-----------------------------------------------------------
//����: ��ʾ�¼������б�
//����: ��
//����: true
//-----------------------------------------------------------------------------
//bool_t Sh_ShowEvtt(char *param)
ADD_TO_SHELL_HELP(evtt,"��ʾ�¼����ͱ�");
ADD_TO_IN_SHELL bool_t evtt(char *param)
{
    u16 pl_ecb;
    u32 MemSize;
    char *name;
    MemSize = 0;
    debug_printf("knlshell","���ͺ�  ���ȼ� ������  ջ����   ����\n\r");
    for(pl_ecb = 0; pl_ecb < CFG_EVENT_TYPE_LIMIT; pl_ecb++)
    {
        if(g_tEvttTable[pl_ecb].property.registered ==1)
        {
            MemSize += g_tEvttTable[pl_ecb].stack_size;
            debug_printf("knlshell","%05d   ",pl_ecb);
            debug_printf("knlshell","%03d    ",g_tEvttTable[pl_ecb].default_prio);
            debug_printf("knlshell","%08x  %08x ",
                        (ptu32_t)g_tEvttTable[pl_ecb].thread_routine,
                        g_tEvttTable[pl_ecb].stack_size);
#if CFG_OS_TINY == false
            name = g_tEvttTable[pl_ecb].evtt_name;
#else
            name = "unkown";
#endif  //CFG_OS_TINY == false
            debug_printf("knlshell","%s",name);
            debug_printf("knlshell","\n\r");
        }
        else
        {
//          printf("knlshell","%05d   ����",pl_ecb);
        }
    }
    debug_printf("knlshell","��������ջ�����ܼ�:      %08x\n\r",MemSize);
    debug_printf("knlshell","������¼���������:      %d\n\r",CFG_EVENT_TYPE_LIMIT);
    return true;
}

//----��ʾջʹ�����-----------------------------------------------------------
//����: ��ʾϵͳ�������Ѿ������̵߳��¼���ջ��Ϣ
//����: ��
//����: true
//-----------------------------------------------------------------------------
//bool_t Sh_ShowStack(char *param)

ADD_TO_SHELL_HELP(stack,"��ʾϵͳ�������Ѿ������̵߳��¼���ջ��Ϣ");
ADD_TO_IN_SHELL bool_t stack(char *param)
{
    u16 pl_ecb;
    u32 loop,StackSize,pads;
    u32 *stack;
    char *name;

    debug_printf("knlshell","�¼��� �߳�   ջ��     ջָ��   ջ�ߴ�   ʣ����   ������\n\r");
    for(pl_ecb = 0; pl_ecb < CFG_EVENT_LIMIT; pl_ecb++)
    {

        if(g_tECB_Table[pl_ecb].previous !=
                        (struct EventECB*)&s_ptEventFree)
        {
            debug_printf("knlshell","%05d  ",pl_ecb);
            if(g_tECB_Table[pl_ecb].vm)
                debug_printf("knlshell","�ѷ��� ");
            else
            {
                debug_printf("knlshell","δ���� ");
                continue ;
            }
            StackSize = g_tECB_Table[pl_ecb].vm->stack_size;
            pads = 0x64646464;
            stack = (u32*)(&(g_tECB_Table[pl_ecb].vm[1]));
            for(loop = 0; loop < (StackSize>>2)-1; loop++)
            {
                if(stack[loop] != pads)
                {
                    break;
                }
            }
#if CFG_OS_TINY == false
            name = g_tEvttTable[g_tECB_Table[pl_ecb].evtt_id&(~CN_EVTT_ID_MASK)].evtt_name;
#else
            name = "unkown";
#endif  //CFG_OS_TINY == false
            debug_printf("knlshell","%08x %08x %08x %08x %s",
                        (ptu32_t)(&g_tECB_Table[pl_ecb].vm[1]),
                        (ptu32_t)(g_tECB_Table[pl_ecb].vm->stack),
                        StackSize,(loop<<2),
                        name);
            debug_printf("knlshell","\n\r");
        }
        else
        {
//            debug_printf("knlshell","%05d  �����¼����ƿ�",pl_ecb);
        }
    }
    debug_printf("knlshell","ջָ�������һ���������л�ʱ�����ֵ\n\r");

    return true;
}

//struct ShellCmdTab const tg_ShellKernelCmdTbl[] =
//{
//   {
//        "event",
//        Sh_ShowEvent,
//        "��ʾ�¼���",
//        NULL
//    },
//    {
//        "evtt",
//        Sh_ShowEvtt,
//        "��ʾ�¼����ͱ�",
//        NULL
//    },
//    {
//        "heap",
//        (bool_t (*)(char*))__M_ShowHeap,
//        "��ʾ��ʹ�����",
//        NULL
//    },
//    {
//        "heap-spy",
//        (bool_t (*)(char*))__M_ShowHeapSpy,
//        "��ʾ��̬�ڴ���ϸ�������",
//        NULL
//    },
//    {
//        "stack",
//        (bool_t (*)(char*))Sh_ShowStack,
//        "��ʾϵͳ�������Ѿ������̵߳��¼���ջ��Ϣ",
//        NULL
//    },
//};
//
//static struct ShellCmdRsc tg_ShellKernelCmd
//                        [sizeof(tg_ShellKernelCmdTbl)/sizeof(struct ShellCmdTab)];

#if CFG_OS_TINY == false
ptu32_t Debug_Scan(void)
{
    u32 pl_ecb;

    while(1)
    {

        for(pl_ecb = 0; pl_ecb < CFG_EVENT_LIMIT; pl_ecb++)
        {
            g_tECB_Table[pl_ecb].consumed_cnt_second =
                              (u32)g_tECB_Table[pl_ecb].consumed_cnt
                            - g_tECB_Table[pl_ecb].consumed_cnt_record;
            g_tECB_Table[pl_ecb].consumed_cnt_record =
                            (u32)g_tECB_Table[pl_ecb].consumed_cnt;
        }
        Djy_EventDelay(1000*mS);
    }
}
#endif  //CFG_OS_TINY == false

//----��ʼ��������Ϣģ��-------------------------------------------------------
//����: ����������Ϣ�¼����Ͳ�����֮
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void ModuleInstall_DebugInfo(ptu32_t para)
{
    u16 evtt_debug;
    para = para;        //�����������澯
#if CFG_OS_TINY == false
    evtt_debug = Djy_EvttRegist(EN_CORRELATIVE,1,0,0,
                                 Debug_Scan,NULL,1024,"debug_info");
    if(evtt_debug == CN_EVTT_ID_INVALID)
        return;
    Djy_EventPop(evtt_debug,NULL,0,0,0,0);
#endif  //CFG_OS_TINY == false
//    Sh_InstallCmd(tg_ShellKernelCmdTbl,tg_ShellKernelCmd,
//            sizeof(tg_ShellKernelCmdTbl)/sizeof(struct ShellCmdTab));
}


