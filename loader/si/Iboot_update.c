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
//����ģ��: Iboot
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ��Ӧ�ñ�̵ĺ����ļ�
//����˵��:
//�޶���ʷ:
//1. ����: 2016-06-17
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <device.h>
#include <stat.h>
#include <dirent.h>
#include <xip.h>
#include "shell.h"
#include <djyos.h>
#include <stdlib.h>
#include <string.h>
#include "version.h"
#include "cpu_peri.h"
#include "cpu-optional.h"
#include "component_config_iboot.h"
#if(CN_CPU_OPTIONAL_CACHE==1)
#include "set-cache.h"
#endif
#include <osboot.h>
#include <device/flash/flash.h>
#include "dbug.h"
#include "project_config.h"
#if !defined (CFG_RUNMODE_BAREAPP)

static struct SemaphoreLCB *ptUpdateIbootSemp;
// ============================================================================
// ���ܣ���������iboot
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t  runiboot(char *param)
{
    Set_RunIbootFlag();
#if(CN_CPU_OPTIONAL_CACHE==1)
     Cache_CleanData();
     Cache_InvalidInst();
#endif
     reset();
    return false;
}

// ============================================================================
// ���ܣ���������app
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t  runapp(char *param)
{
    Set_RunAppFlag();
#if(CN_CPU_OPTIONAL_CACHE==1)
     Cache_CleanData();
     Cache_InvalidInst();
#endif
     reset();
    return false;
}

// ============================================================================
// ���ܣ���������iboot������iboot
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t runibootui(char *param)
{
    Set_RunIbootUpdateIboot();
#if(CN_CPU_OPTIONAL_CACHE==1)
     Cache_CleanData();
     Cache_InvalidInst();
#endif
     reset();
    return false;
}

// ============================================================================
// ���ܣ���������iboot������iboot
// ������
// ���أ�
// ��ע��
// ============================================================================
 bool_t  updateruniboot(char *param)
{
    Set_RunIbootUpdateIboot();
#if(CN_CPU_OPTIONAL_CACHE==1)
     Cache_CleanData();
     Cache_InvalidInst();
#endif
     reset();
    return false;
}

// ============================================================================
// ���ܣ�����APP
// ��������
// ���أ�
// ��ע��
// ============================================================================
//bool_t Sh_UpdateApp(char *param)
bool_t updateapp(char *param)
{
    runiboot(0);
    return (TRUE);
}

// ============================================================================
// ���ܣ�����Iboot
// ��������
// ���أ�
// ��ע��
// ============================================================================
char iapibootname[512];
#define IAPBUF_SIZE   512
bool (*Update_and_run_mode)(char *param);

bool updateiboot(char *param)
{
    if(param == NULL)
        strcpy(iapibootname,CFG_IBOOT_UPDATE_NAME);
    else
        strcpy(iapibootname,param);
   Update_and_run_mode = NULL;
   Lock_SempPost(ptUpdateIbootSemp);
   return true;
}

// ============================================================================
// ���ܣ�����Iboot�¼���
// �������ޡ�
// ���أ�1�������壩��
// ��ע
// ============================================================================
static ptu32_t __UpdateIboot(void)
{
    FILE *srciboot;
    FILE *xipiboot;
    u8 buf[IAPBUF_SIZE];
    u32 readsize,res;
    struct stat test_stat;
    s64 srcsize;

    while(1)
    {
       Lock_SempPend(ptUpdateIbootSemp,CN_TIMEOUT_FOREVER);
       stat(iapibootname,&test_stat);
       srcsize = test_stat.st_size;
       srciboot = fopen(iapibootname, "r+");
       if(srciboot==NULL)
       {
           error_printf("IAP","file \"%s\" is not found.\r\n", CFG_IBOOT_UPDATE_NAME);
           break;
       }
       xipiboot = fopen("/xip-iboot/iboot.bin", "r+");
       if(srciboot==NULL)
       {
           error_printf("IAP","cannot open source file xip-iboot .\r\n");
           break;
       }
       while(1)
       {
           readsize = fread(buf, 1, IAPBUF_SIZE, srciboot);
           if((readsize != IAPBUF_SIZE) && srcsize >= IAPBUF_SIZE)
           {
               printf("Iap read file %s error \n\r",iapibootname);
               break;
           }
           if(readsize != IAPBUF_SIZE)
           {
               printf("read file %s End \r\n ",iapibootname);
           }

           res = fwrite(buf, 1, readsize, xipiboot);
           if(res != readsize)
           {
               printf("write file xip-iboot error  \r\n ");
           }
           srcsize -= readsize;
           if(srcsize == 0)
           {
               info_printf("IAP","Iboot update success.\r\n");
               break;
           }
       }
       fclose(srciboot);
       fclose(xipiboot);
       if(srcsize !=0)
           error_printf("IAP","Iboot update error .\r\n");
       else if(Update_and_run_mode != NULL)
           Update_and_run_mode(NULL);

    }
    return 0;
}
// ============================================================================
// ���ܣ���װIAP�����
// �������ޡ�
// ���أ�0�������壩��
// ��ע
// ============================================================================
ptu32_t ModuleInstall_IAP(void)
{
     uint16_t evtt_id,event_id;
    ptUpdateIbootSemp=Lock_SempCreate(1,0,CN_BLOCK_FIFO,"UpdateIbootSemp");
    if(ptUpdateIbootSemp==NULL)
    {
        debug_printf("IAP","IAP ModuleInstall failed.\r\n");
    }
    evtt_id = Djy_EvttRegist(EN_CORRELATIVE,5,0,0,__UpdateIboot,
              NULL,8192,"Update Iboot");
    if(evtt_id!=CN_EVTT_ID_INVALID)
    {
       event_id=Djy_EventPop(evtt_id,NULL,0,0,0,0);
    }
    else
    {
        Djy_EvttUnregist(evtt_id);
        debug_printf("IAP","Update Iboot evtt pop failed.\r\n");
    }

    return 0;
}

ADD_TO_ROUTINE_SHELL(runiboot,runiboot,NULL);
ADD_TO_ROUTINE_SHELL(runapp,runapp,"ֱ������APP(���ڲ�ȡ�ڴ��ʾȷ��������Ŀʱ��Ч)");
//ADD_TO_ROUTINE_SHELL(runibootui,runibootui,"��������iboot������iboot������iboot");
//ADD_TO_ROUTINE_SHELL(updateruniboot,updateruniboot,"��������iboot������iboot");
//ADD_TO_ROUTINE_SHELL(updateapp,updateapp,"update app lication");
ADD_TO_ROUTINE_SHELL(updateiboot,updateiboot,"Update Iboot.");

#endif
















