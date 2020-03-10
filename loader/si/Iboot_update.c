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
#include <blackbox.h>
#include <device/flash/flash.h>
#include "dbug.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�
#include "Iboot_info.h"
#if (CFG_RUNMODE_BAREAPP == 0)

#define IAPBUF_SIZE   512
extern void reboot();
extern void reset();
extern void restart_system(u32 key);
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
    return true;
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
    return true;
}



// ============================================================================
// ���ܣ���������iboot������iboot
// ������
// ���أ�
// ��ע��
// ============================================================================
// bool_t  updateruniboot(char *param)
//{
//    Set_RunIbootUpdateIboot();
//#if(CN_CPU_OPTIONAL_CACHE==1)
//     Cache_CleanData();
//     Cache_InvalidInst();
//#endif
//     reset();
//    return false;
//}

 // ============================================================================
 // ���ܣ������ַ������������ɹ���������app����iboot
 // ������
 // ���أ�true -- ���óɹ���false -- û������
 // ��ע��
 // ============================================================================
 bool_t runapp_or_runiboot(char *mode)
 {
      if(strcmp(mode, "runapp") == 0)
          Set_UpdateRunModet(1);
      else if(strcmp(mode, "runiboot") == 0)
              Set_UpdateRunModet(0);
      else
          return false;

      return true;
 }
// ============================================================================
// ���ܣ���������APP��־���ڽ�����Ϣ����������app·��
// ������param -- ������app·��
// ���أ�
// ��ע��
// ============================================================================
//bool_t Sh_UpdateApp(char *param)
bool_t updateapp(char *param)
{
    bool_t res = true;
    char *word_param, *next_param;
    Set_RunIbootUpdateApp();

    word_param = shell_inputs(param,&next_param);
    if(word_param == NULL)
    {
        res = Fill_MutualUpdatePath(CFG_APP_UPDATE_NAME, sizeof(CFG_APP_UPDATE_NAME));
        Set_UpdateRunModet(1);      //����������app
    }
    else
    {
        if(runapp_or_runiboot(word_param) == false)
        {
            res = Fill_MutualUpdatePath(word_param, strlen(word_param));
            word_param = shell_inputs(next_param,&next_param);
            if(word_param != NULL)
            {
                if(runapp_or_runiboot(word_param) == false)
                    Set_UpdateRunModet(1);      //δָ�������ɹ�������app����iboot��������iboot
            }
            else
                Set_UpdateRunModet(1);      //����������app
        }
        else
        {
            word_param = shell_inputs(next_param,&next_param);
            if(word_param != NULL)
                res = Fill_MutualUpdatePath(word_param, strlen(word_param));
            else
                res = Fill_MutualUpdatePath(CFG_APP_UPDATE_NAME, sizeof(CFG_APP_UPDATE_NAME));
        }
    }

    if(res == false)
        error_printf("iap"," file name is longer than 40.");
    else
        runiboot(0);

    Djy_EventDelay(5000*1000);      //��ʱһ�£������������е���Ϣ�ܴ�ӡ����
    return (TRUE);
}

// ============================================================================
// ���ܣ�����App�¼���
// �������ޡ�
// ���أ�0�������壩��
// ��ע
// ============================================================================
bool_t Iboot_UpdateApp(void)
{
    char apppath[MutualPathLen];
    char xipapppath[MutualPathLen];
    struct stat test_stat;
    FILE *srcapp = NULL;
    FILE *xipapp;
    s64 srcsize;
    u8 buf[IAPBUF_SIZE];
    u32 readsize,res;
    char *file;
    char percentage_last = 0, percentage = 0;

    if((Get_UpdateSource() == 0) && (Get_UpdateApp() == true))
    {
        if(!stat(CFG_FORCED_UPDATE_PATH,&test_stat))
            srcapp = fopen(CFG_FORCED_UPDATE_PATH, "r+");

        info_printf("IAP","app update start.\r\n");
        if(srcapp == NULL)
        {
            strcpy(apppath, Get_MutualUpdatePath());
            srcapp = fopen(apppath, "r+");
        }
        else
        {
            strcpy(apppath, CFG_FORCED_UPDATE_PATH);
            Set_UpdateRunModet(1);
        }
        if(srcapp != NULL)
        {
            file = strrchr(apppath, '/');
            if(file)
            {
                sprintf(xipapppath, "%s%s", "/xip-app", file);
                xipapp = fopen(xipapppath, "w+");
                if(xipapp != NULL)
                {
                    stat(apppath,&test_stat);
                    srcsize = test_stat.st_size;
                    printf("\r\nloading       ");
                    while(1)
                    {
                        percentage = 100 - ((char)((srcsize * 100)/ test_stat.st_size));
                        if(percentage != percentage_last)
                        {
                            printf("\b\b\b%2d%%",percentage);
                            percentage_last = percentage;
                        }
                        readsize = fread(buf, 1, IAPBUF_SIZE, srcapp);
                        if((readsize != IAPBUF_SIZE) && srcsize >= IAPBUF_SIZE)
                        {
                            printf("Iap read file %s error \n\r",apppath);
                            break;
                        }
                        if(readsize != IAPBUF_SIZE)
                        {
                            printf("read file %s End \r\n ",apppath);
                        }

                        res = fwrite(buf, 1, readsize, xipapp);
                        if(res != readsize)
                        {
                            printf("write file xip-app error  \r\n ");
                        }
                        srcsize -= readsize;
                        if(srcsize == 0)
                        {
                            info_printf("IAP","App update success.  waiting to restart.\r\n");
                            Clear_RunIbootUpdateApp();
                            break;
                        }
                    }
                    fclose(srcapp);
                    fclose(xipapp);
                    if(srcsize !=0)
                    {
                        error_printf("IAP","app update error .\r\n");
                        Djy_EventDelay(1000*1000);      //��ʱһ�£������������е���Ϣ�ܴ�ӡ����
                    }
                    else
                        Update_ToRun();
                }
                else
                {
                    error_printf("IAP","cannot open source file xip-app .\r\n");
                    Djy_EventDelay(1000*1000);      //��ʱһ�£������������е���Ϣ�ܴ�ӡ����
                }
            }
            else
            {
                error_printf("IAP","App file error .\r\n");
                Djy_EventDelay(1000*1000);      //��ʱһ�£������������е���Ϣ�ܴ�ӡ����
            }
        }
        else
        {
            error_printf("IAP","file \"%s\" is not found.\r\n", apppath);
            Djy_EventDelay(1000*1000);      //��ʱһ�£������������е���Ϣ�ܴ�ӡ����
        }
    }

    return TRUE;
}

// ============================================================================
// ���ܣ���������iboot��־���ڽ�����Ϣ����������iboot·��
// ������param -- ������iboot·��
// ���أ�
// ��ע��
// ============================================================================
bool_t updateiboot(char *param)
{
    bool_t res = true;
    char *word_param, *next_param;
    Set_RunAppUpdateIboot();

    word_param = shell_inputs(param,&next_param);
    if(word_param == NULL)
    {
        res = Fill_MutualUpdatePath(CFG_IBOOT_UPDATE_NAME, sizeof(CFG_IBOOT_UPDATE_NAME));
        Set_UpdateRunModet(0);      //����������iboot
    }
    else
    {
        if(runapp_or_runiboot(word_param) == false)
        {
            res = Fill_MutualUpdatePath(word_param, strlen(word_param));
            word_param = shell_inputs(next_param,&next_param);
            if(word_param != NULL)
            {
                if(runapp_or_runiboot(word_param) == false)
                    Set_UpdateRunModet(0);      //δָ�������ɹ�������app����iboot��������iboot
            }
            else
                Set_UpdateRunModet(0);      //����������iboot
        }
        else
        {
            word_param = shell_inputs(next_param,&next_param);
            if(word_param != NULL)
                res = Fill_MutualUpdatePath(word_param, strlen(word_param));
            else
                res = Fill_MutualUpdatePath(CFG_IBOOT_UPDATE_NAME, sizeof(CFG_IBOOT_UPDATE_NAME));
        }
    }

    if(res == false)
        error_printf("iap"," file name is longer than 40.");
    else
        runapp(0);

    Djy_EventDelay(5000*1000);      //��ʱһ�£������������е���Ϣ�ܴ�ӡ����
    return (TRUE);
}

// ============================================================================
// ���ܣ�����Iboot�¼���
// �������ޡ�
// ���أ�1�������壩��
// ��ע
// ============================================================================
bool (*Update_and_run_mode)(char *param);
bool_t App_UpdateIboot(char *param)
{
    FILE *srciboot;
    FILE *xipiboot;
    u8 buf[IAPBUF_SIZE];
    u32 readsize,res;
    struct stat test_stat;
    s64 srcsize;
    char iapibootname[MutualPathLen];
    char percentage_last = 0, percentage = 0;

    info_printf("IAP","iboot update start.\r\n");
    strcpy(iapibootname, Get_MutualUpdatePath());
    Update_and_run_mode = NULL;

    srciboot = fopen(iapibootname, "r+");
    if(srciboot != NULL)
    {
       xipiboot = fopen("/xip-iboot/iboot.bin", "w+");
       if(xipiboot != NULL)
       {
           stat(iapibootname,&test_stat);
           srcsize = test_stat.st_size;
           printf("\r\nloading       ");
           while(1)
           {
               percentage = 100 - ((char)((srcsize * 100)/ test_stat.st_size));
               if(percentage != percentage_last)
               {
                   printf("\b\b\b%2d%%",percentage);
                   percentage_last = percentage;
               }
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
                   Clear_RunAppUpdateIboot();
                   break;
               }
           }
           fclose(srciboot);
           fclose(xipiboot);
           if(srcsize !=0)
           {
               error_printf("IAP","Iboot update error .\r\n");
               Djy_EventDelay(1000*1000);      //��ʱһ�£������������е���Ϣ�ܴ�ӡ����
           }
           else if(Update_and_run_mode != NULL)
               Update_and_run_mode(NULL);
           if(srcsize ==0)
               Update_ToRun();
       }
       else
       {
           error_printf("IAP","cannot open source file xip-iboot .\r\n");
           Djy_EventDelay(1000*1000);      //��ʱһ�£������������е���Ϣ�ܴ�ӡ����
       }
    }
    else
    {
       error_printf("IAP","file \"%s\" is not found.\r\n", iapibootname);
       Djy_EventDelay(1000*1000);      //��ʱһ�£������������е���Ϣ�ܴ�ӡ����
    }
    return TRUE;
}

//�û�����д�ú�����ָ����ô����iboot
__attribute__((weak)) bool_t user_update_iboot(char *param)
{
    return false;
}

bool_t ModuleInstall_XIP(void)
{
    uint16_t evtt_Update = CN_EVTT_ID_INVALID;
    char run_mode = Get_RunMode();
    if(user_update_iboot(0) == false)
    {
        if(Get_UpdateSource() == 0)
        {
            if(run_mode == 1)
            {
                if(Get_Updateiboot() == true)
                {
                    evtt_Update = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0,
                                                App_UpdateIboot, NULL, CFG_MAINSTACK_LIMIT, "update iboot");
                }
            }
            else
                return false;

            if(evtt_Update != CN_EVTT_ID_INVALID)
            {
                if(Djy_EventPop(evtt_Update, NULL, 0, NULL, 0, 0) != CN_EVENT_ID_INVALID)
                {
                    if(run_mode == 1)
                        info_printf("XIP","add iboot update function.\r\n");

                    return true;
                }
            }
            return false;
        }
        else
            return false;
    }
    return true;
}


ADD_TO_ROUTINE_SHELL(runiboot,runiboot,NULL);
ADD_TO_ROUTINE_SHELL(runapp,runapp,"ֱ������APP(���ڲ�ȡ�ڴ��ʾȷ��������Ŀʱ��Ч)");
//ADD_TO_ROUTINE_SHELL(runibootui,runibootui,"��������iboot������iboot������iboot");
//ADD_TO_ROUTINE_SHELL(updateruniboot,updateruniboot,"��������iboot������iboot");
ADD_TO_ROUTINE_SHELL(updateapp,updateapp,"update app lication");
ADD_TO_ROUTINE_SHELL(updateiboot,updateiboot,"Update Iboot.");

#endif


//add some boot commands
//static bool_t rebootshell(char *param)
bool_t rebootshell(char *param)
{

    reboot( );
    return true;
}
//static bool_t resetshell(char *param)
bool_t resetshell(char *param)
{
    u32 key = 0;
    if(NULL != param)
    {
        key = strtoul(param,NULL,0);
    }
    reset(key);
    return true;
}
//static bool_t reloadshell(char *param)
bool_t restart(char *param)
{
    u32 key = 0;
    if(NULL != param)
    {
        key = strtoul(param,NULL,0);
    }
    restart_system(key);
    return true;
}

//static bool_t bootaddressshell(char *param)
bool_t bootaddress(char *param)
{
    u32 addr;
    u32 InitCpu_Addr;
    addr = strtoul(param,NULL,0);

    debug_printf("OsStart","%s:addr:0x%08x\n\r",__FUNCTION__,addr);
    InitCpu_Addr = *(u32*)addr;
    ((void (*)(void))(InitCpu_Addr))();

    return true;
}
ADD_TO_ROUTINE_SHELL(restart,restart,"usage:restart [key](if key is 0XAA55AA55 then will not record)");
ADD_TO_ROUTINE_SHELL(bootaddress,bootaddress,"usage:bootaddress [address]");
ADD_TO_ROUTINE_SHELL(rebootshell,rebootshell,"usage:rebootshell [key](if key is 0XAA55AA55 then will not record)");
ADD_TO_ROUTINE_SHELL(resetshell,resetshell,"usage:resetshell [key](if key is 0XAA55AA55 then will not record)");








