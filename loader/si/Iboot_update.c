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
//#include <device/djy_flash.h>
#include "dbug.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�
#include "include/Iboot_info.h"
#if (CFG_RUNMODE_BAREAPP == 0)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// ============================================================================
// ���ܣ�����������iboot�������ú��ڴ��־��Ȼ������ϵͳ��ResetҪ��Ӳ���ṩһ���ڴ棬
//      ��ϵͳ��λ���ܱ�����λǰ�����ݣ�һ��SRAM�ſ��ԣ�������Щ����ĵ�Ƭ���粩ͨ��
//      BK7251����λ��Ƭ��RAM������ʼ��Ϊ0xAA��
// ������param����1������ʹ�� reset ���ջ���������ʹ�� reboot
// ���أ�
// ��ע��
// ============================================================================
bool_t  runiboot(char *param)
{
    s32 temp;
    temp = atoi(param);
    Iboot_SetRunIbootFlag();
#if(CN_CPU_OPTIONAL_CACHE==1)
    Cache_CleanData();
    Cache_InvalidInst();
#endif
    if(temp == 1)
        CPU_Reset();
    else
        CPU_Reboot();
    return true;
}

// ============================================================================
// ���ܣ�����������app�������ú��ڴ��־��Ȼ������ϵͳ��ResetҪ��Ӳ���ṩһ���ڴ棬
//      ��ϵͳ��λ���ܱ�����λǰ�����ݣ�һ��SRAM�ſ��ԣ�������Щ����ĵ�Ƭ���粩ͨ��
//      BK7251����λ��Ƭ��RAM������ʼ��Ϊ0xAA��
// ������param����1������ʹ�� reset ���ջ���������ʹ�� reboot
// ���أ�
// ��ע��
// ============================================================================
bool_t  runapp(char *param)
{
    s32 temp;
    temp = atoi(param);
    Iboot_SetRunAppFlag();
#if(CN_CPU_OPTIONAL_CACHE==1)
    Cache_CleanData();
    Cache_InvalidInst();
#endif
    if(temp == 1)
        CPU_Reset();
    else
        CPU_Reboot();
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
//     CPU_Reset();
//    return false;
//}

//// ============================================================================
//// ���ܣ���������APP��־���ڽ�����Ϣ����������app·����Ȼ����������iboot������
////      ibootִ�о������������������ʹ��ǰ����ģʽ��iboot��Ҳ������
//// ������param -- ������app·��
//// ���أ�
//// ��ע��
//// ============================================================================
////bool_t Sh_UpdateApp(char *param)
//bool_t updateapp(char *param)
//{
//    char *word_param, *next_param;
//
//    word_param = shell_inputs(param,&next_param);
//    Iboot_SetRunIbootAndUpdateApp();
//    if(word_param == NULL)
//    {
//        set_upgrade_info(CFG_APP_UPDATE_NAME, sizeof(CFG_APP_UPDATE_NAME));
//    }
//    else
//    {
//        if(strlen(word_param) < 31)    //31��update_info.file.file_path�ĳߴ磬����д�ţ���ġ�
//        {
//            set_upgrade_info(word_param, sizeof(word_param));
//        }
//        else
//        {
//            set_upgrade_info(CFG_APP_UPDATE_NAME, sizeof(CFG_APP_UPDATE_NAME));
//        }
//    }
//    Iboot_SetAfterUpdateRunMode(1);      //����������app
//
//    runiboot(0);
//
//    return (TRUE);
//}
//
//// ============================================================================
//// ���ܣ���������iboot��־���ڽ�����Ϣ����������iboot·����Ȼ����������app������
////      appִ�о������������������ʹ��ǰ����ģʽ��app��Ҳ���������ر�ע�⣬APP��Ӧ
////      ����������ش��룬��Щ������ܸ�bsp�йأ���Ϊ�����ֲ�汾���жϺ��쳣������
////      iboot���䣬��������д��Щ������ܵ������д���
//// ������param -- ������iboot·��
//// ���أ�
//// ��ע��
//// ============================================================================
//bool_t updateiboot(char *param)
//{
//    char *word_param, *next_param;
//
//    word_param = shell_inputs(param,&next_param);
//    Iboot_SetRunAppAndUpdateIboot();
//    if(word_param == NULL)
//    {
//        set_upgrade_info(CFG_IBOOT_UPDATE_NAME, sizeof(CFG_IBOOT_UPDATE_NAME));
//    }
//    else
//    {
//        if(strlen(word_param) < 31)    //31��update_info.file.file_path�ĳߴ磬����д�ţ���ġ�
//        {
//            set_upgrade_info(word_param, sizeof(word_param));
//        }
//        else
//        {
//            set_upgrade_info(CFG_IBOOT_UPDATE_NAME, sizeof(CFG_IBOOT_UPDATE_NAME));
//        }
//    }
//    Iboot_SetAfterUpdateRunMode(0);      //����������app
//
//    runapp(0);
//
//    return (TRUE);
//}

//----------------------------------------------------------------------------
//����: ��app�ӿ�Ѱַ�ڴ�д�뵽xip�ļ�ϵͳ�У���iboot���ã�appͷ����Ҫ��д��ʱ�޸ĵ�
//      ��ָ����صĲ��֣��Ѿ��޸ĺ�
//����: addr���ڴ���ʼ��ַ��
//      file_size���ļ���С��
//      file_name��app����ʱ��bin�ļ����������ļ��ڲ����������ļ����������޸�
//����: true: �ɹ��� false �� ʧ��.
//-----------------------------------------------------------------------------
bool_t WriteAppFromRam(s8 *addr,u32 file_size, s8 *file_name)
{
    bool_t ret = false;
    FILE *xipapp = 0;
    u32 file_name_len,res, residue_size = (s32)file_size;
    char *xipapppath;
    char percentage_last = 0, percentage = 0;
    file_name_len = strlen((const char *)file_name)+ strlen((const char *)CN_XIP_APP_PATH) + 2;
    xipapppath = (char *)malloc(file_name_len);
    if(xipapppath == NULL)
        return ret;
    sprintf(xipapppath, "%s/%s", CN_XIP_APP_PATH, file_name);
    xipapp = fopen(xipapppath, "w+");
    if(xipapp)
    {
        printf("open file success   %s ,\r\n", xipapppath);
        printf("\r\nloading       ");
        while(1)
        {
            percentage = 100 - ((char)((residue_size * 100)/ file_size));
            if(percentage != percentage_last)
            {
                printf("\b\b\b%2d%%",percentage);
                percentage_last = percentage;
            }
            if(residue_size > 1024)
            {
                res = fwrite(addr, 1, 1024, xipapp);
                if(res != 1024)
                    printf("write file xip-app error  \r\n ");
                addr += res;
                residue_size -= res;
            }
            else
            {
                res = fwrite(addr, 1, residue_size, xipapp);
                if(res != residue_size)
                    printf("write file xip-app error  \r\n ");
                addr += res;
                residue_size -= res;
            }
            if(residue_size == 0)
            {
                info_printf("IAP","app update success.\r\n");
                Iboot_ClearRunIbootAndUpdateApp();
                ret = true;
                break;
            }
            if((s32)residue_size < 0)
            {
                info_printf("IAP","app update fail.\r\n");
                break;
            }
        }
        fclose(xipapp);
        free(xipapppath);
    }
    else
        printf("open file fail,\r\n");

    return ret;
}

#define DjyosHeadSize    512
#define IAPBUF_SIZE      DjyosHeadSize
static u8 readbuffer[DjyosHeadSize];

// ============================================================================
// ���ܣ����ļ�������App���ļ�·����iboot��app������96�ֽ�RAM��
// ������production_time������ʱ�仺�������������ڣ�ASCII�룬4�ֽ�
//      production_num��������Ż�������һ���ڲ��ظ���34���ƣ�5�ֽ�
// ���أ�false = ʧ�ܣ�true=�ɹ�
// ��ע
// ============================================================================
//ԭ����Iboot_UpdateApp
bool_t WriteAppFromFile(u8 *production_time,u8* production_num)
{
    char apppath[CN_UPDATE_PATH_LIMIT];
    char xipapppath[CN_UPDATE_PATH_LIMIT];
    struct stat file_state;
    struct AppHead *p_apphead;
    struct ProductInfo *p_productinfo;
    FILE *srcapp = NULL;
    FILE *xipapp;
    s64 srcsize;
    u32 readsize,res;
    bool_t ret = false;
//  char *file;
    char percentage_last = 0, percentage = 0;

    Iboot_GetOtaFilename(apppath, CN_UPDATE_PATH_LIMIT);
    if(apppath[0] != '/')       //·����������ȫ·��
    {
        printk(" file path error \r\n");
        return false;
    }
    if(!XIP_CheckAppInFile(apppath))
    {
        printk("verify file error \r\n");
        return false;
    }
    memset(&file_state, 0, sizeof(struct stat));
    if(stat(apppath,&file_state))
    {
        printk("get ota file fail\r\n");
        return false;
    }
    srcapp = fopen(apppath, "r");
    if(srcapp == NULL)
    {
        error_printf("IAP","file \"%s\" is not found.\r\n", apppath);
        DJY_EventDelay(1000*1000);      //��ʱһ�£������������е���Ϣ�ܴ�ӡ����
    }
    p_apphead = (struct AppHead *)readbuffer;
    p_productinfo = (struct ProductInfo *)(p_apphead+1);
    res = fread(readbuffer, 1, DjyosHeadSize,srcapp);
    if(res == DjyosHeadSize)
    {
        srcsize = file_state.st_size;
        if(srcsize  == p_apphead->app_bin_size)
        {
            info_printf("IAP","app update start...\r\n");
            memcpy(p_productinfo->ProductionNumber, production_num, sizeof(p_productinfo->ProductionNumber));
            memcpy(p_productinfo->ProductionTime, production_time, sizeof(p_productinfo->ProductionTime));
//          file = strrchr(p_apphead->app_name, '/');
            //����APP���ļ������� "/xip-app" ���棬�γ�xip�ļ�·��
            sprintf(xipapppath, "%s/%s", CN_XIP_APP_PATH, p_apphead->app_name);
            if(!File_Format((const char *)CN_XIP_APP_PATH))
            {
                xipapp = fopen(xipapppath, "w+");
                if(xipapp != NULL)
                {
                    res = fwrite(readbuffer, 1, DjyosHeadSize, xipapp);
                    srcsize -= res;
                    while(1)
                    {
                        percentage = 100 - ((char)((srcsize * 100)/ file_state.st_size));
                        if(percentage != percentage_last)
                        {
                            printf("\b\b\b%2d%%",percentage);
                            percentage_last = percentage;
                        }
                        readsize = fread(readbuffer, 1, IAPBUF_SIZE, srcapp);
                        if((readsize != IAPBUF_SIZE) && srcsize >= IAPBUF_SIZE)
                        {
                            printf("Iap read file %s error \n\r",apppath);
                            break;
                        }
                        if(readsize != IAPBUF_SIZE)
                        {
                            printf("read file %s End \r\n ",apppath);
                        }

                        res = fwrite(readbuffer, 1, readsize, xipapp);
                        if(res != readsize)
                        {
                            printf("write file xip-app error  \r\n ");
                        }
                        srcsize -= readsize;
                        if(srcsize == 0)
                        {
                            info_printf("IAP","App update success.  waiting to restart.\r\n");
//                            Iboot_ClearRunIbootAndUpdateApp();
                            ret = true;
                            break;
                        }
                    }
                    fclose(srcapp);
                    fclose(xipapp);
                    if(srcsize !=0)
                    {
                        error_printf("IAP","app update error .\r\n");
                        DJY_EventDelay(1000*1000);      //��ʱһ�£������������е���Ϣ�ܴ�ӡ����
                    }
//                    else
//                        Iboot_UpdateToRun();
                }
                else
                {
                    error_printf("IAP","cannot open source file xip-app .\r\n");
                    DJY_EventDelay(1000*1000);      //��ʱһ�£������������е���Ϣ�ܴ�ӡ����
                }
            }
            else
            {
                printk("erase flash fail\r\n");
                ret = false;
            }
        }
        else
        {
            printk("file size is overlong\r\n");
        }
    }
    else
    {
        printk("open ota file fail\r\n");
        ret = false;
    }


    return ret;
}

// ============================================================================
// ���ܣ�����Iboot�¼���
// �������ޡ�
// ���أ�1�������壩��
// ��ע
// ============================================================================
bool (*Update_and_run_mode)(char *param);
ptu32_t Iboot_AppUpdateIboot(void)
{
    FILE *srciboot;
    FILE *xipiboot;
    u8 buf[IAPBUF_SIZE];
    u32 readsize,res;
    struct stat test_stat;
    s64 srcsize;
    char iapibootname[CN_UPDATE_PATH_LIMIT];
    char percentage_last = 0, percentage = 0;

    info_printf("IAP","iboot update start.\r\n");
    Iboot_GetOtaFilename(iapibootname, CN_UPDATE_PATH_LIMIT);
    if(iapibootname[0] != '/')       //·����������ȫ·��
    {
        printk(" file path error \r\n");
        DJY_EventDelay(100*1000);      //��ʱһ�£������������е���Ϣ�ܴ�ӡ����
        return false;
    }
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
                   Iboot_ClearRunAppAndUpdateIboot();
                   break;
               }
           }
           fclose(srciboot);
           fclose(xipiboot);
           if(srcsize !=0)
           {
               error_printf("IAP","Iboot update error .\r\n");
               DJY_EventDelay(1000*1000);      //��ʱһ�£������������е���Ϣ�ܴ�ӡ����
           }
           else if(Update_and_run_mode != NULL)
               Update_and_run_mode(NULL);
           if(srcsize ==0)
               Iboot_UpdateToRun();
       }
       else
       {
           error_printf("IAP","cannot open source file xip-iboot .\r\n");
           DJY_EventDelay(1000*1000);      //��ʱһ�£������������е���Ϣ�ܴ�ӡ����
       }
    }
    else
    {
       error_printf("IAP","file \"%s\" is not found.\r\n", iapibootname);
       DJY_EventDelay(1000*1000);      //��ʱһ�£������������е���Ϣ�ܴ�ӡ����
    }
    return TRUE;
}

//------------------------------------------------------------------------------
//���ܣ�һ�����������û������Լ���APP������ʵ�֣���������iboot��
//��������Ч
//���أ��㶨����false���û�����ʵ�����뷵��true
//------------------------------------------------------------------------------
__attribute__((weak)) bool_t Iboot_UserUpdateIboot(char *param)
{
    return false;
}

bool_t ModuleInstall_UpdateIboot(void)
{
    uint16_t evtt_Update = CN_EVTT_ID_INVALID;
    char run_mode = Iboot_GetRunMode();
    if(Iboot_UserUpdateIboot(0) == false)
    {
        if(Iboot_GetUpdateSource() == CN_STORE_IN_FILE)
        {
            if(run_mode == 1)
            {
                if(Iboot_GetUpdateIboot() == true)
                {
                    evtt_Update = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0,
                                                Iboot_AppUpdateIboot, NULL, CFG_MAINSTACK_LIMIT, "update iboot");
                }
            }
            else
                return false;

            if(evtt_Update != CN_EVTT_ID_INVALID)
            {
                if(DJY_EventPop(evtt_Update, NULL, 0, 0, 0, 0) != CN_EVENT_ID_INVALID)
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
//ADD_TO_ROUTINE_SHELL(updateapp,updateapp,"update app lication");
//ADD_TO_ROUTINE_SHELL(updateiboot,updateiboot,"Update Iboot.");

#endif


//add some boot commands
//static bool_t rebootshell(char *param)
bool_t rebootshell(char *param)
{

    CPU_Reboot( );
    return true;
}
#pragma GCC diagnostic pop

//static bool_t resetshell(char *param)
bool_t resetshell(char *param)
{
    CPU_Reset();
    return true;
}
//static bool_t reloadshell(char *param)
bool_t restart(char *param)
{
    CPU_RestartSystem();
    return true;
}

ADD_TO_ROUTINE_SHELL(restart,restart,"usage:restart [key](if key is 0XAA55AA55 then will not record)");
ADD_TO_ROUTINE_SHELL(reboot,rebootshell,"usage:rebootshell [key](if key is 0XAA55AA55 then will not record)");
ADD_TO_ROUTINE_SHELL(reset,resetshell,"usage:resetshell [key](if key is 0XAA55AA55 then will not record)");








