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

#include "stdint.h"
#include "stdio.h"
#include "driver.h"
#include "IAP.h"
#include "shell.h"
#include "djyos.h"
#include <cfg/Iboot_config.h>
#include "verify.h"
#include <stdlib.h>
#include "string.h"
#include "version.h"
#include "cpu_peri.h"
#include "cpu-optional.h"
#include "IAP_Inner.h"
#if(CN_CPU_OPTIONAL_CACHE==1)
#include "set-cache.h"
#endif
#include <osboot.h>

extern const char *g_pcIbootDir;
extern tagIapVar pg_IapVar;
extern struct AppInfo gc_ptAppInfo;
extern u32 g_pAppCodeStartRamAddr;
extern u32 g_pAppCodeEndRamAddr;
extern struct IbootCtrl gc_ptIbootCtrl;

extern void reset(u32 key);
extern bool_t IAP_IsRamIbootFlag(void);
extern bool_t HAL_SetUpdateFlag(void);
extern s32 IAP_PrintIbootVersion(void);
extern bool_t IAP_IsForceIboot(void);
extern void __AppStart(void); // ������iboot.lds�ж�λ�ĺ�������APP��lds�ļ��У�
                              // ȷ������ָ�� AppStart ����

bool_t Sh_RunIBoot(char *param);
bool_t Sh_RunAPP(char *param);
bool_t Sh_GetAPPInfor(char *param);
bool_t Sh_GetStatus(char *param);
bool_t Sh_GetRunMode(char *param);

#define CN_VERSION_CHAR_LEN   100

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
u32 IAP_GetAPPStartCodeRamAddr(void)
{
    return ((u32)&g_pAppCodeStartRamAddr);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
u32 IAP_GetAPPCodeRamSize(void)
{
   return ((u32)&g_pAppCodeEndRamAddr - (u32)&g_pAppCodeStartRamAddr);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
u32 IAP_GetAPPStartAddr(void)
{
    return (u32)(&gc_ptAppInfo.RomStartAddr);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t IAP_APPIsDebug(void)
{
    u8 *dbgflg;

    dbgflg = (u8*)&gc_ptIbootCtrl.flag;
    if( (dbgflg[0] == 'd') && (dbgflg[1] == 'b') && (dbgflg[2] == 'g') )
    {
        return true;
    }
    return false;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
u32 IAP_GetAPPSize(void)
{
   return (gc_ptAppInfo.AppSize);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
u32 IAP_GetAPPCRC(void)
{
    return (gc_ptIbootCtrl.Iap_crc);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t  Sh_RunIBoot(char *param)
{
     pg_IapVar.IbootFlag[0]=0x52;   //R
     pg_IapVar.IbootFlag[1]=0x75;   //u
     pg_IapVar.IbootFlag[2]=0x6E;   //n
     pg_IapVar.IbootFlag[3]=0x49;   //i
     pg_IapVar.IbootFlag[4]=0x62;   //b
     pg_IapVar.IbootFlag[5]=0x6F;   //o
     pg_IapVar.IbootFlag[6]=0x6F;   //o
     pg_IapVar.IbootFlag[7]=0x74;   //t
     pg_IapVar.IbootFlag[8]=0x0;    // \0
#if(CN_CPU_OPTIONAL_CACHE==1)
     Cache_CleanData();
     Cache_InvalidInst();
#endif
     reset(CN_BOOT_LEGALKEY);
     return true;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t  Sh_RunAPP(char *param)
{
    pg_IapVar.IbootFlag[0]=0x52;   //R
    pg_IapVar.IbootFlag[1]=0x75;   //u
    pg_IapVar.IbootFlag[2]=0x6E;   //n
    pg_IapVar.IbootFlag[3]=0x41;   //a
    pg_IapVar.IbootFlag[4]=0x50;   //p
    pg_IapVar.IbootFlag[5]=0x50;   //p
    pg_IapVar.IbootFlag[6]=0x00;   // \0
    pg_IapVar.IbootFlag[7]=0x00;
#if(CN_CPU_OPTIONAL_CACHE==1)
     Cache_CleanData();
     Cache_InvalidInst();
#endif
     reset(CN_BOOT_LEGALKEY);
    return true;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
void RunIboot(void)
{
    Sh_RunIBoot(NULL);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
void RunApp(void)
{
    Sh_RunAPP(NULL);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t Sh_GetAPPInfor(char *param)
{
    u32 size,crc32;
    char *filename;
    size=gc_ptIbootCtrl.AppSize;
    crc32=gc_ptIbootCtrl.Iap_crc;
    filename=gc_ptIbootCtrl.filename;
    printf("size:0x%08x,crc32:0x%08x,filename:%s.\r\n",size,crc32,filename);
    return true;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t Sh_GetStatus(char *param)
{
    switch (pg_IapVar.IbootStatus)
    {
        case EN_NO_ERR:
            printf("IAP status:No Err.\r\n");
            break;
        case EN_FORCE_IBOOT:
            printf("IAP status:Force Run Iboot.\r\n");
            break;
        case EN_RAM_IBOOT_FLAG:
            printf("IAP status:RAM Iboot flag.\r\n");
            break;
        case EN_LOAD_FROM_DATA_MODE:
            printf("IAP status:Load from data mode.\r\n");
            break;
        case EN_CRC_ERR:
            printf("IAP status:CRC Err.\r\n");
            break;
        case EN_APP_FLAG_ERR:
            printf("IAP status:APP Flag Err.\r\n");
            break;
        case EN_FILE_NO_EXSIT_ERR:
            printf("IAP Err:APP File Not Exist Err.\r\n");
            break;
        case EN_FILE_SIZE_INVALID_ERR:
            printf("IAP Err:APP File Size Invalid.\r\n");
            break;
        case EN_BIN_INCOMPLETED_ERR:
            printf("IAP Err:APP File Not Completed.\r\n");
            break;
        case EN_lDS_MISMATCH:
            printf("IAP Err:Iboot/App Ids MisMatch����gc_AppAddr ������RomStartAddr\r\n");
            printf("���ܵ�ԭ����iboot��APP��memory.lds��IbootSize���岻һ��!\r\n");
            break;
        default:
            break;
    }
    return true;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
#if 0 // ���������࣬�Ѿ�����ver����
bool_t Sh_GetOSVersion(char *param)
{
   char *str;
   str=malloc(CN_VERSION_CHAR_LEN);
   if(str!=NULL)
   {
       str=djyos_kernel_version;
       printf("Version:%s.\r\n",str);
   }
   return true;
}
#endif

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t Sh_GetRunMode(char *param)
{
    if(pg_IapVar.RunMode==CN_IAP_MODE_IBOOT)
    {
           printf("Run Mode:Iboot.\r\n");
    }
    else
    {
#if (DEBUG == 1)
        printf("Run Mode:APP(debug).\r\n");
#else
        printf("Run Mode:APP(release).\r\n");
#endif
    }
    return true;
}

// ============================================================================
// ���ܣ�����APP
// ��������
// ���أ�
// ��ע��
// ============================================================================
bool_t Sh_UpdateApp(char *param)
{
    HAL_SetUpdateFlag();
    RunIboot();
    return (TRUE);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t Sh_GetIbootVersion(char *pDummy)
{
    pDummy = pDummy;
    IAP_PrintIbootVersion();
    return (TRUE);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
struct ShellCmdTab const shell_cmd_iap_table[]=
    {
        {
        "runiboot",
        Sh_RunIBoot,
        "�л���Iboot(���ڲ�ȡ�ڴ��ʾȷ��������Ŀ��APP�м���DJYOS shellģ��ʱ��Ч)",
        "COMMAND:runiboot+enter"
        },

        {
        "runapp",
        Sh_RunAPP,
        "ֱ������APP(���ڲ�ȡ�ڴ��ʾȷ��������Ŀʱ��Ч)",
        "COMMAND:runapp+enter"
        },

        {
        "appinfo",
        Sh_GetAPPInfor,
        "��ȡ����bin�ļ���Ϣ",
        "COMMAND:appinfo+enter"
        },

        {
        "iapstatus",
        Sh_GetStatus,
        "��ȡIboot״̬��Ϣ",
        "COMMAND:iapstatus+enter"
        },
#if 0
        {
        "iapver",
        Sh_GetIAPVersion,
        "��ȡIAP�汾��Ϣ",
        "COMMAND:iapver+enter"
        },
#endif
        {
        "iapmode",
        Sh_GetRunMode,
        "��ȡ��ǰ����ģʽ(Iboot or APP)",
        "COMMAND:iapmode+enter"
        },

        {
        "updateapp",
        Sh_UpdateApp,
        "Update app.",
        "COMMAND:updateapp+enter"
        },

        {
        "ibootver",
        Sh_GetIbootVersion,
        "Get Iboot version.",
        "COMMAND:ibootver+enter"
        }
};

// ============================================================================
// ���ܣ���װSHELL����
// �������ޡ�
// ���أ�1�������壩��
// ��ע
// ============================================================================
#define CN_IAP_SHELL_NUM  sizeof(shell_cmd_iap_table)/sizeof(struct ShellCmdTab)
static struct ShellCmdRsc tg_iap_shell_cmd_rsc[CN_IAP_SHELL_NUM];
ptu32_t IAP_Shell_Module_Install(void)
{
    Sh_InstallCmd(shell_cmd_iap_table,tg_iap_shell_cmd_rsc,CN_IAP_SHELL_NUM);
    return 1;
}

// ============================================================================
// ���ܣ���װIAP�����
// �������ޡ�
// ���أ�0�������壩��
// ��ע
// ============================================================================
ptu32_t ModuleInstall_IAP(void)
{
    IAP_Shell_Module_Install();
    printf("IAP ModuleInstall success.\r\n");
    return 0;
}

// ============================================================================
// ���ܣ����ļ��м���APP�����С�(�ж��Ƿ���Ҫ���ļ�ϵͳ�м���APP��ִ��)
// �������ޡ�
// ���أ��ޡ�
// ��ע����IAP����ģʽΪLoadFromDataʱ��ֻ��������������������1.����APP��2.IAP����ģ
//      ʽΪLoadFromData��3.APP�ļ�����.�Ż���ļ��м��ص�RAM�����С�
// ============================================================================
bool_t IAP_LoadAPPFromFile(void)
{
    bool_t result = true;
    char *Buf;
    u32 crc,len,addr;
    FILE *fp;
    struct stat FpInfo;

    //�����ж��Ƿ���Ҫ����APP״̬
    result = IAP_IsForceIboot();
    if(result)
    {
        pg_IapVar.IbootStatus=EN_FORCE_IBOOT;
        result = false;  //����Iboot
    }
    else
    {
        result=IAP_IsRamIbootFlag();
        if(result)
        {
            pg_IapVar.IbootStatus=EN_RAM_IBOOT_FLAG;
            result = false;   //����Iboot
        }
        else
        {
            if(g_IbootType==EN_LOAD_FORM_DATA)
            {
               fp=fopen(g_pcIbootDir,"r");
               if(fp==NULL)
               {
                   result = false;
                   pg_IapVar.IbootStatus = EN_FILE_NO_EXSIT_ERR;
                   goto LOAD_FILE_EXIT;
               }

               addr=IAP_GetAPPStartAddr() - 0x100;
               Buf=(char *)addr;

               fread(Buf,0x100,1,fp);           //read the 256 byte file info

               fseek(fp,0,0);

               if(IAP_APPIsDebug())             //dbg not need crc
               {
                    if(0 == stat(g_pcIbootDir,&FpInfo))
                    {
                        len = (u32)FpInfo.st_size;
                    }
                    else
                    {
                        fclose(fp);
                        result = false;
                        pg_IapVar.IbootStatus = EN_BIN_INCOMPLETED_ERR;
                        goto LOAD_FILE_EXIT;
                    }
                    fread(Buf,len,1,fp);
               }
               else
               {
                   len=IAP_GetAPPSize();
                   fread(Buf,len,1,fp);
                   crc=crc32_buf(Buf,len);
                   if(crc!=gc_ptIbootCtrl.Iap_crc)
                   {
                       fclose(fp);
                       result = false;
                       pg_IapVar.IbootStatus = EN_CRC_ERR;
                       goto LOAD_FILE_EXIT;
                   }
               }
               pg_IapVar.IbootStatus = EN_LOAD_FROM_DATA_MODE;
               pg_IapVar.RunMode = CN_IAP_MODE_APP;
#if(CN_CPU_OPTIONAL_CACHE==1)
     Cache_CleanData();
     Cache_InvalidInst();
#endif
                __AppStart();
                return true;        //clear compile warning
            }
        }

    }
LOAD_FILE_EXIT:
    pg_IapVar.RunMode = CN_IAP_MODE_IBOOT;
    return result;
}








