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
//------------------------------------------------------
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "object.h"
#include "djyos.h"
#include "hmi-input.h"
#include "shell.h"
#include "version.h"
#include "time.h"
#include "systime.h"
#include <ctype.h>

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_Sh(void);
//    ModuleInstall_Sh();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"shell"        //shell
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:�������             //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                   //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:init                    //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
//dependence:"stdio"            //������������������������none����ʾ�������������
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


bool_t Sh_ListRscSon(char *param);
bool_t Sh_ListRscTree(char *param);
void Sh_ListRscAll(void);
void __Sh_ShowSon(struct Object *branche);
void __Sh_ShowBranche(struct Object *branche);
bool_t Sh_ShowMemory(char *param);
bool_t Sh_FillMemory(char *param);
bool_t Sh_CmdParser(const char *buf,const char *keyword,char **param);
void Sh_CmdHelp(void);
bool_t Sh_ListCmdHelp(char *param);
bool_t Sh_Ver(char *param);
bool_t Sh_Date(char *param);
bool_t Sh_Time(char *param);
bool_t Sh_ExecCommand(char *cmd);
bool_t Sh_UninstallCmdByName(char *param);
bool_t Sh_SetDate(char *param);
bool_t Sh_SetTime(char *param);

ptu32_t Sh_Service(void);

void (*fng_pPrintWorkPath)(void) = NULL;
bool_t (*fng_pCD_PTT)(const char *PTT_Name) = NULL;

extern bool_t Sh_PrintWorkPath(void);
extern bool_t ShowCpuInfo(char *param);
//���������,����ϵͳ�Դ�������
struct ShellCmdTab const shell_cmd_table[] =
{
    {
        "rsc-tree",
        Sh_ListRscTree,
        "�г�ϵͳ�е���Դ���ֺͲ�ι�ϵ",
        "��ʽ: rsc_tree TreeName,ʡ��TreeName���г�ȫ����Դ"
    },
    {
        "rsc-son",
        Sh_ListRscSon,
        "�г�����Դ",
        "��ʽ: rsc_tree RscName,ʡ��RscName���г�����Դ"
    },
    {
        "d",
        Sh_ShowMemory,
        "��ȡ�ڴ��������",
        "�����ʽ:d ��ַ ��Ԫ�� ÿ��Ԫ�ֽ���"
    },
    {
        "f",
        Sh_FillMemory,
        "д���ݵ��ڴ�",
        "�����ʽ��f ��ʼ��ַ  ��Ԫ�� ÿ��Ԫ�ֽ��� �������"
    },
    {
        "help",
        Sh_ListCmdHelp,
        "DJYOS�����������Ϣ",
        NULL
    },
    {
        "ver",
        Sh_Ver,
        "��ʾdjyos�İ汾",
        NULL
    },
    {
        "date",
        Sh_Date,
        "��ʾ������������",
        NULL
    },
    {
        "time",
        Sh_Time,
        "��ʾ��ǰʱ���������������ʱ��",
        NULL
    },

    {
        "settime",
        Sh_SetTime,
        "Diaplay or set current time",
        "�����ʽ:settime year/month/day, hour:min:sec+Enter"
    },

    {
        "uninstall-cmd",
        Sh_UninstallCmdByName,
        "ɾ��shell����--������",
        NULL
    },
    {
        "cpuinfo",
        ShowCpuInfo,
        "usage:cpuinfo:show the cpu",
        NULL
    },
};

static struct ShellCmdRsc tg_shell_cmd_rsc
                        [sizeof(shell_cmd_table)/sizeof(struct ShellCmdTab)];
static struct Object *s_ptShellCmdDir;

//----��ǰ����·����ָ�븳ֵ-----------------------------------------------------
//����: ����ָ�븳ֵ����ָ��ָ���ȡ��ǰ����·���ĺ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Sh_PrintWorkPathSet(void)
{
    fng_pPrintWorkPath = (void*)Sh_PrintWorkPath;
}


extern ptu32_t Sh_ServiceNew(void);

//----����ģ���ʼ������-------------------------------------------------------
//����: ��ʼ������ģ��
//����: ��
//����: true = �ɹ���false = ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_Sh(void)
{
    u16 shell_evtt;
    u32 loop;
    shell_evtt = Djy_EvttRegist(EN_CORRELATIVE,  //�������¼�
                                CN_PRIO_REAL,       //Ĭ�����ȼ�
                                0,                  //�̱߳���������������Ч
                                0,                  //�߳����ޣ���������Ч
//                                1,                  //�������г�������
                                Sh_ServiceNew,         //��ں���
                                NULL,               //��ϵͳ����ջ
                                0x1000,             //ջ�ߴ�
                                "shell"             //�¼�������
                                );
    if(shell_evtt == CN_EVTT_ID_INVALID)
        return false;
    s_ptShellCmdDir = OBJ_AddChild(OBJ_Root(), NULL, 0, "shell cmd list");
    if(s_ptShellCmdDir == NULL)
    {
        Djy_EvttUnregist(shell_evtt);
    }
    if(Djy_EventPop(shell_evtt,NULL,0,0,0,0) == CN_EVENT_ID_INVALID)
    {
        Djy_EvttUnregist(shell_evtt);
        OBJ_Del(s_ptShellCmdDir);
        return false;
    }
    for(loop = 0;loop<sizeof(shell_cmd_table)/sizeof(struct ShellCmdTab);loop++)
    {
        tg_shell_cmd_rsc[loop].shell_cmd_func
                                    = shell_cmd_table[loop].shell_cmd_func;
        tg_shell_cmd_rsc[loop].help_hint
                                    = shell_cmd_table[loop].help_hint;
        tg_shell_cmd_rsc[loop].help_detailed
                                    = shell_cmd_table[loop].help_detailed;
        OBJ_AddChild(s_ptShellCmdDir, NULL, (ptu32_t)&tg_shell_cmd_rsc[loop],
                                (const char*)(shell_cmd_table[loop].cmd));
    }
    return true;
}

//----���shell����------------------------------------------------------------
//����: ����û���shell�������si�汾��һ�������ƵĿ�ִ��Ӱ�񣬲������ű���
//      ��ͨ��shellֱ�ӵ��ú������������������ӵ�������С���dlsp�汾�У�����
//      ͨ��ֱ�ӵ��ú����ķ�ʽʵ�֣����������ķ�ʽ����Ȼ���ٶȿ�����ơ�
//����: cmd_tab�������ָ�룬�ɲ���shell_cmd_table�ķ�ʽ����
//      cmd_rsc����Դ�ڵ�ָ�룬�ɲ���tg_shell_cmd_rsc�ķ�ʽ����
//      cmd_num, ����ӵı��а�����������
//����: true = �ɹ���
//      false= ʧ�ܣ������ǲ�������Ҳ������������а�������������������������
//          ����������������������������Ȼ�������������
//-----------------------------------------------------------------------------
bool_t Sh_InstallCmd(struct ShellCmdTab const *cmd_tab,
                      struct ShellCmdRsc *cmd_rsc,u32 cmd_num)
{
    u32 loop;
    bool_t result = true;
    if( (cmd_tab == NULL) || (cmd_rsc == NULL) || (cmd_num == 0))
        return result;

    for(loop = 0; loop < cmd_num;loop++)
    {
        if(OBJ_SearchChild(s_ptShellCmdDir,(const char*)(cmd_tab[loop].cmd)) == NULL)
        {
            cmd_rsc[loop].shell_cmd_func = cmd_tab[loop].shell_cmd_func;
            cmd_rsc[loop].help_hint = cmd_tab[loop].help_hint;
            cmd_rsc[loop].help_detailed = cmd_tab[loop].help_detailed;
            OBJ_AddChild(s_ptShellCmdDir, NULL, (ptu32_t)&cmd_rsc[loop],(const char*)(cmd_tab[loop].cmd));
        }
        else
        {
            result = false;
        }
    }
    return result;
}
//----ж��shell����------------------------------------------------------------
//����: ж���û�shell����
//����: cmd_tab�������ָ�룬�ɲ���shell_cmd_table�ķ�ʽ����,cmd_num����Ĵ�С
//����: �ɹ�ж�صĸ���
//-----------------------------------------------------------------------------
u32 Sh_UninstallCmdByTab(struct ShellCmdTab const *cmd_tab,u32 cmd_num)
{
    u32 loop;
    u32 result = 0;
    struct Object *tagShellCmdRsc;
    if( (cmd_tab == NULL) || (cmd_num == 0))
        return result;

    for(loop = 0; loop < cmd_num;loop++)
    {
        tagShellCmdRsc = OBJ_SearchChild(s_ptShellCmdDir,(const char*)(cmd_tab[loop].cmd));
        if(NULL != tagShellCmdRsc)
        {
            OBJ_Del(tagShellCmdRsc);
            result ++;
        }

    }
    return result;
}
bool_t Sh_UninstallCmdByName(char *param)
{
    bool_t result = 0;
    struct Object *tagShellCmdRsc;
    if(NULL == param)
    {
        return false;
    }

    tagShellCmdRsc = OBJ_SearchChild(s_ptShellCmdDir,(const char *)param);
    if(NULL != tagShellCmdRsc)
    {
        OBJ_Del(tagShellCmdRsc);
        result = true;
    }
    else
    {
        result = false;
    }
    return result;
}

//----��ȡ����---------------------------------------------------------------
//����: ��buf����ȡһ���ɿո���н����������ĵ��ʣ�next���ڷ�����һ�������׵�ַ��
//      ���û����һ�����ʣ���next=NULL��
//����: buf�����������ַ���
//      next��������һ������ָ��
//����: ��ȡ�ĵ���ָ�룬�ѽ����ʺ���ķָ������ɴ�������'\0'
//-----------------------------------------------------------------------------
char *Sh_GetWord(char *buf,char **next)
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

//----��ʾ����Դ----------------------------------------------------------
//����: ������Ҫ����ʾ��Դ��������Դ�����֣�������Դ����Ϊ�գ�����ʾ"������Դ"
//����: �����ַ��������г��ĸ���Դ����ʡȴ���г�������µ���Դ
//����: true
//-----------------------------------------------------------------------------
bool_t Sh_ListRscSon(char *param)
{
    struct Object *rsc_tree;
    if(param == NULL)
    {
        rsc_tree = OBJ_Root();
    }
    else
    {
        rsc_tree = OBJ_Search(OBJ_Root(),(const char*)param);
        if(rsc_tree == NULL)
        {
            printf("û�ҵ� %s ��Դ\r\n",param);
        }
    }
    __Sh_ShowSon(rsc_tree);
    return true;
}


//----��ʾ��Դ��----------------------------------------------------------
//����: ������Ҫ����ʾ��Դ��������Դ�����֣�������Դ����Ϊ�գ�����ʾ"������Դ"
//����: �����ַ��������г���������ʡȴ���г�ȫ����Դ
//����: true
//-----------------------------------------------------------------------------
bool_t Sh_ListRscTree(char *param)
{
    struct Object *rsc_tree;
    if(param == NULL)
        rsc_tree = OBJ_Root();
    else
    {
        rsc_tree = OBJ_Search(OBJ_Root(),(const char*)param);
        if(rsc_tree == NULL)
        {
            printf("û�ҵ� %s ��Դ��\r\n",param);
        }
    }
    __Sh_ShowBranche(rsc_tree);
    return true;
}

//----��ʾ����Դ---------------------------------------------------------------
//����: ��ʾĳ��Դ�����ʼ��һ����Դ��֧������������Դ����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void __Sh_ShowSon(struct Object *branche)
{
    struct Object *current_node = branche;
    char *Name;
    while(1)
    {
        current_node = OBJ_TraveChild(branche,current_node);
        if(current_node == NULL)
        {
            printf("\r\n");
            break;
        }

        Name = (char*)OBJ_Name(current_node);
        if(Name)
        {
            printf("  %s\r\n", Name);
        }
        else
        {
            printf("  ������Դ\r\n");
        }
    }
}

//----��ʾ��Դ��---------------------------------------------------------------
//����: ��ʾĳ��Դ�����ʼ��һ����Դ��֧������������Դ����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void __Sh_ShowBranche(struct Object *branche)
{
    struct Object *current_node = branche;
    ucpu_t len;
    char neg[20];
    char *Name;
    for(len = 0; len<20; len++)
        neg[len] = '-';
    while(1)
    {
        current_node = OBJ_TraveScion(branche,current_node);
        if(current_node == NULL)
        {
            printf("\r\n");
            break;
        }
        len = OBJ_Level(current_node);
        neg[len] = '\0';
        printf("%s", neg);
        neg[len] = '-';
        Name = (char*)OBJ_Name(current_node);
        if(Name != NULL)
        {
            printf("%s\r\n", Name);
        }else
        {
            printf("������Դ\r\n");
        }
    }
}

//----��ʾ�ڴ�-----------------------------------------------------------------
//����: ��ʾĳ��ַ��ʼ��һ���ڴ棬ÿ����ʾ16����Ԫ��ֻ��ʾ�������޸�
//����: param������������������Ҫ�����������ÿո����
//          ����1:��ʼ��ַ��10���ƻ�16����(0x��ͷ)
//          ����2:��ʾ�ĵ�Ԫ��
//          ����3:ÿ��Ԫ�ֽ������Ϸ�ֵ��1��2��4��8��������ֵ�����ش���
//����: true=������ʾ��false=����
//-----------------------------------------------------------------------------
bool_t Sh_ShowMemory(char *param)
{
    ptu32_t addr;
    uint32_t unit_num,unit_bytes,len;
    char *word_addr,*word_un,*word_ub,*word_trail,*next_param;

    //��ȡ3������
    word_addr = Sh_GetWord(param,&next_param);
    word_un = Sh_GetWord(next_param,&next_param);
    word_ub = Sh_GetWord(next_param,&next_param);
    word_trail = Sh_GetWord(next_param,&next_param);
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
bool_t Sh_FillMemory(char *param)
{
    ptu32_t addr;
    uint32_t unit_num,unit_bytes,len,data;
    char *word,*next_param;

    word = Sh_GetWord(param,&next_param);
    addr = strtoul(word, (char **)NULL, 0);
    word = Sh_GetWord(next_param,&next_param);
    unit_num = strtol(word, (char **)NULL, 0);
    word = Sh_GetWord(next_param,&next_param);
    unit_bytes = strtol(word, (char **)NULL, 0);
    word = Sh_GetWord(next_param,&next_param);
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

//----��ʾ����-----------------------------------------------------------------
//����: ��ʾ������Ϣ
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Sh_CmdHelp(void)
{
    struct ShellCmdRsc *current_cmd;
    struct Object *CmdObj;
    printf("\r\n�йؾ����������ϸ��Ϣ��������help [������]\r\n");
    printf("\r\n");
    CmdObj = (struct Object *)s_ptShellCmdDir;
    char *Name;
    while(1)
    {
        CmdObj = OBJ_TraveScion(s_ptShellCmdDir,CmdObj);
        if(CmdObj == NULL)
        {
            break;
        }
        else
        {
            current_cmd = (struct ShellCmdRsc *)OBJ_Represent(CmdObj);
            Name = (char*)OBJ_Name(CmdObj);
            if(current_cmd->help_hint != NULL)
            {
                //todo: ǰ�����£���һ��%sӦ����%-32s�Ŷԣ���6��ͬ
                printf("%-24s%s", Name, current_cmd->help_hint);

            }
            else
            {
                printf("%-24sû���ṩ��Ҫ������Ϣ", Name);
            }
        }
        printf("\r\n");
    }
}


//----��ʾ�ļ�ϵͳ�������-----------------------------------------------------
//����:
//����: �����ַ���������:
//����: true
//-----------------------------------------------------------------------------
bool_t Sh_ListCmdHelp(char *param)
{
    char *cmd,*next_param;
    struct Object *CmdObj;
    bool_t result;
    struct ShellCmdRsc *help_cmd;

    if(param == NULL)
    {
        Sh_CmdHelp();
        result = true;
    }
    else
    {
        cmd = Sh_GetWord(param,&next_param);
        CmdObj = OBJ_SearchChild(s_ptShellCmdDir,(const char*)cmd);
        if(CmdObj != NULL)
        {
            help_cmd = (struct ShellCmdRsc *)OBJ_Represent(CmdObj);
            if(help_cmd->help_detailed != NULL)
                printf("%s",help_cmd->help_detailed);
            else
                printf("û���ṩ��ϸ������Ϣ");
            printf("\r\n");
            result = true;
        }else
        {
            printf("��Ч������\r\n");
            result = false;
        }
    }
    return result;
}
bool_t Sh_Ver(char *param)
{
    printf("\r\n%s\r\n", djyos_kernel_version);

    return true;
}

bool_t Sh_Date(char *param)
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

bool_t Sh_Time(char *param)
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


/*
 * ��������������ʱ��ӣ�����ͨ��������telnet�ն��޸����ں�ʱ�� by zhb20170313
 */
bool_t Sh_SetTime(char *param)
{
    s64 nowtime;
    struct tm dtm;
    char command[20];
    int res;

    nowtime = Tm_Time(NULL);
    Tm_LocalTime_r(&nowtime,&dtm);
    Tm_AscTime(&dtm,command);

   if(param!=NULL)
   {
        memcpy(command,param,20);
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

        printf("\r\n��ǰʱ�䣺%s %s",command, g_cTmWdays[dtm.tm_wday]);

   }
   else
   {
       printf("\r\n��ǰʱ�䣺%s %s",command, g_cTmWdays[dtm.tm_wday]);
       printf("\r\n������ʱ�䣺");
   }

   printf("\r\n");
   return true;
}



//----�ж�':'�����Ƿ�������-----------------------------------------------------
//����: ��һ�����ܰ����༶·�������ļ������ַ������жϵ�һ������':'֮���Ƿ���
//      ����
//����: path������·������ʽ��"C:\aaaa\bbbb\cccc"
//����: true=�ɹ�, ˵��':'֮��������,����û��':'
//      false=ʧ�ܣ�˵��':'֮��û������
//��ע: pathӦ���Ǿ����Ϸ��Լ����ַ������������word����������
//-----------------------------------------------------------------------------
bool_t __judge_colon_is_data(char *path)
{
    uint32_t index;
    uint32_t  name_len;

    name_len = strlen(path);
    for(index = 0; index< name_len; index++)
    {
        if(path[index] == ':')    //�����ַ�'\'
        {
            if(path[index+1] != '\0')
                return true;
            else
                return false;
        }
    }
    //�ַ�����û��ð��
    if(index == name_len)
        return true;
    return false;
}

//----ִ�п���̨����---------------------------------------------------
//����: ������ִ�п���̨����
//����: ��
//����: ��
//��ע: ��ʱ���ù�ϣ��ȣ����и��ӿ��ٵıȽϡ���������ıȽϣ�������ˡ�
//      �ǻ����������ˡ�
//---------------------------------------------------------------------
bool_t Sh_ExecCommand(char *buf)
{
    bool_t result = false;
    char *cmd,*next_param;
    struct Object *CmdObj;
    struct ShellCmdRsc *exec_cmd;
    //�������ƶ�ȡ255�ַ�����������ʾ�������С�
    if(strnlen(buf, CN_SHELL_CMD_LIMIT+1) > CN_SHELL_CMD_LIMIT)
    {
        printf("�����ַ���̫��\r\n");
    }
    //ת��buf�еĴ�д��ĸΪСд
//    capital_convert_lowercase(buf);

    if(!__judge_colon_is_data(buf))
    {
        if (fng_pCD_PTT != NULL)
        {
            if( ! fng_pCD_PTT(buf))
            {
                printf("���� %s ������",buf);
            }
        }
        else
        {
            printf("not file system");
        }
        return true;
    }
    cmd = Sh_GetWord(buf,&next_param);
    strlwr(cmd);
    CmdObj = OBJ_SearchChild(s_ptShellCmdDir,(const char*)cmd);
    if(CmdObj != NULL)
    {
        exec_cmd = (struct ShellCmdRsc *)OBJ_Represent(CmdObj);
        result = exec_cmd->shell_cmd_func(next_param);
        if(result == false)
        {
            if(exec_cmd->help_detailed)
            {
                printf("�����ʽ������ȷ��ʽ�ǣ�");
                printf("%s",exec_cmd->help_detailed);
            }
            else
                printf("�����ʽ����δ�ṩ��ϸ������Ϣ");
        }
    }else
    {
        printf("\r\n�޴�����\r\n");
        result = true;
    }
    return result;
}

struct MultiplexSetsCB * sg_ptShellStdin;

//----����̨������-----------------------------------------------------------
//����: ����console������ַ�����console����س���ʱ��ִ�����һ������ó���
//      255�ַ���
//����: ��
//����: ��
//-----------------------------------------------------------------------------
ptu32_t Sh_Service(void)
{
    char command[CN_SHELL_CMD_LIMIT+1];
//    Multiplex_Create(1);
    printf("\n\r");
    if ((fng_pPrintWorkPath != NULL))
        fng_pPrintWorkPath();
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
                Sh_ExecCommand(command);  //ִ������
            printf("\n\r");
            if ((fng_pPrintWorkPath != NULL))
                fng_pPrintWorkPath();
            printf(">");
        }
    }
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

//we just do the buffer manager and semantic analysis
//use the index to find the corresponding command here
static const char *__matchcmd(const char *index)
{
    const char *ret =NULL;
    struct Object *start;
    struct Object *tmp;

    start = OBJ_Child(s_ptShellCmdDir);
    tmp = start;
    do{
        ret = strstr(OBJ_Name(tmp),index);
        if(ret != NULL)
        {
            ret = OBJ_Name(tmp);
            break;
        }
        tmp = OBJ_Next(tmp);
    }while(tmp != start);
    return ret;
}
//for the windows.the shell get the left rigt up down
ptu32_t Sh_ServiceNew(void)
{
    u8    ch;
    int   len;
    u8    offset;
    const char *cmdindex;
    char *cwd;
    u32   vk = CN_VK_NULL;    //used when we push back the right key
    u32   vkmask = CN_VK_NULL;
    tagShellBuffer cmdbuf;
    s32 res;
    memset(&cmdbuf,0,sizeof(cmdbuf));
    {
        char *cwd = getcwd(NULL,0);
        printf("\n\r%s>", cwd);   //push the index character to the terminal
        free(cwd);
    }
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
            case CN_VK_TAB:      //tab,should auto complete the command
                //should search the command we has installed if some index has get
                len = strlen(cmdbuf.curcmd);
                if(len > 0)  //some index has get
                {
                    cmdindex = __matchcmd((const char*)cmdbuf.curcmd);
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
                    Sh_ExecCommand(cmdbuf.curcmd);  //execute the command
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
                    Sh_ExecCommand(cmdbuf.curcmd);  //execute the command
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
