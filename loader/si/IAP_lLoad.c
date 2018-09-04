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

#include <stdint.h>
#include <stdio.h>
#include <device.h>
#include <stat.h>
#include <dirent.h>
#include <iap.h>
#include "shell.h"
#include <djyos.h>
#include <stdlib.h>
#include <string.h>
#include "version.h"
#include "cpu_peri.h"
#include "cpu-optional.h"
#include "IAP_Inner.h"
#if(CN_CPU_OPTIONAL_CACHE==1)
#include "set-cache.h"
#endif
#include <osboot.h>
#include <device/flash/flash.h>
#include "dbug.h"
#include "component_config_loader.h"

extern tagIapVar pg_IapVar;
extern struct AppInfo gc_ptAppInfo;
extern u32 g_pAppCodeStartRamAddr;
extern u32 g_pAppCodeEndRamAddr;
extern struct IbootCtrl gc_ptIbootCtrl;
extern u32 gc_ptIbootSize;
static struct SemaphoreLCB *ptUpdateIbootSemp;


extern void reset(u32 key);
extern bool_t IAP_IsRamIbootFlag(void);
extern bool_t HAL_SetUpdateFlag(void);
extern s32 IAP_PrintIbootVersion(void);
extern bool_t IAP_IsForceIboot(void);
extern void __AppStart(void); // ������iboot.lds�ж�λ�ĺ�������APP��lds�ļ��У�
                              // ȷ������ָ�� AppStart ����

bool_t Sh_RunIBoot(char *param);
bool_t Sh_RunAPP(char *param);
//bool_t Sh_GetAPPInfor(char *param);
//bool_t Sh_GetStatus(char *param);
//bool_t Sh_GetRunMode(char *param);
bool Sh_UpdateIboot(char *param);
#define CN_VERSION_CHAR_LEN         100
//#define RW_SIZE                       512
ptu32_t __UpdateIboot(void);
bool UpdateIbootFlag=false;
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
ADD_TO_SHELL_HELP(runiboot,"�л���Iboot(���ڲ�ȡ�ڴ��ʾȷ��������Ŀ��APP�м���DJYOS shellģ��ʱ��Ч)");
ADD_TO_IN_SHELL bool_t  runiboot(char *param)
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

ADD_TO_SHELL_HELP(runapp,"ֱ������APP(���ڲ�ȡ�ڴ��ʾȷ��������Ŀʱ��Ч)");
ADD_TO_IN_SHELL bool_t  runapp(char *param)
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
void Amend_IBootFlag_RunIBoot(void)
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
}


// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
void Amend_IBootFlag_RunAPP(void)
{
    pg_IapVar.IbootFlag[0]=0x52;   //R
    pg_IapVar.IbootFlag[1]=0x75;   //u
    pg_IapVar.IbootFlag[2]=0x6E;   //n
    pg_IapVar.IbootFlag[3]=0x41;   //a
    pg_IapVar.IbootFlag[4]=0x50;   //p
    pg_IapVar.IbootFlag[5]=0x50;   //p
    pg_IapVar.IbootFlag[6]=0x00;   // \0
    pg_IapVar.IbootFlag[7]=0x00;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
void RunIboot(void)
{
    runiboot(NULL);
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
//bool_t Sh_GetAPPInfor(char *param)
ADD_TO_SHELL_HELP(appinfo,"��ȡ����bin�ļ���Ϣ");
ADD_TO_IN_SHELL bool_t appinfo(char *param)
{
    u32 size,crc32;
    char *filename;
    size=gc_ptIbootCtrl.AppSize;
    crc32=gc_ptIbootCtrl.Iap_crc;
    filename=gc_ptIbootCtrl.filename;
    debug_printf("IAP","size:0x%08x,crc32:0x%08x,filename:%s.\r\n",size,crc32,filename);
    return true;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
//bool_t Sh_GetStatus(char *param)
ADD_TO_SHELL_HELP(iapstatus,"��ȡIboot״̬��Ϣ");
ADD_TO_IN_SHELL bool_t iapstatus(char *param)
{
    switch (pg_IapVar.IbootStatus)
    {
        case EN_NO_ERR:
            debug_printf("IAP","IAP status:No Err.\r\n");
            break;
        case EN_FORCE_IBOOT:
            debug_printf("IAP","IAP status:Force Run Iboot.\r\n");
            break;
        case EN_RAM_IBOOT_FLAG:
            debug_printf("IAP","IAP status:RAM Iboot flag.\r\n");
            break;
        case EN_LOAD_FROM_DATA_MODE:
            debug_printf("IAP","IAP status:Load from data mode.\r\n");
            break;
        case EN_CRC_ERR:
            debug_printf("IAP","IAP status:CRC Err.\r\n");
            break;
        case EN_APP_FLAG_ERR:
            debug_printf("IAP","IAP status:APP Flag Err.\r\n");
            break;
        case EN_FILE_NO_EXSIT_ERR:
            debug_printf("IAP","IAP Err:APP File Not Exist Err.\r\n");
            break;
        case EN_FILE_SIZE_INVALID_ERR:
            debug_printf("IAP","IAP Err:APP File Size Invalid.\r\n");
            break;
        case EN_BIN_INCOMPLETED_ERR:
            debug_printf("IAP","IAP Err:APP File Not Completed.\r\n");
            break;
        case EN_lDS_MISMATCH:
            debug_printf("IAP","IAP Err:Iboot/App Ids MisMatch����gc_AppAddr ������RomStartAddr\r\n");
            debug_printf("IAP","���ܵ�ԭ����iboot��APP��memory.lds��IbootSize���岻һ��!\r\n");
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
       debug_printf("IAP","Version:%s.\r\n",str);
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
bool_t GetRunMode(void)
{
    if(pg_IapVar.RunMode==CN_IAP_MODE_IBOOT)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//bool_t Sh_GetRunMode(char *param)
ADD_TO_SHELL_HELP(iapmode,"��ȡ��ǰ����ģʽ(iboot or app)");
ADD_TO_IN_SHELL bool_t iapmode(char *param)
{
    if(GetRunMode())
    {
        debug_printf("IAP","Run Mode:Iboot.\r\n");
    }
    else
    {
#if (DEBUG == 1)
        debug_printf("IAP","Run Mode:APP(debug).\r\n");
#else
        debug_printf("IAP","Run Mode:APP(release).\r\n");
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
//bool_t Sh_UpdateApp(char *param)
ADD_TO_SHELL_HELP(updateapp,"update application");
ADD_TO_IN_SHELL bool_t updateapp(char *param)
{
    HAL_SetUpdateFlag();
    RunIboot();
    return (TRUE);
}

// ============================================================================
// ���ܣ�����Iboot
// ��������
// ���أ�
// ��ע��
// ============================================================================
bool Sh_UpdateIboot(char *param)
{
   Lock_SempPost(ptUpdateIbootSemp);
   return true;
}
ADD_TO_SHELL_HELP(updateiboot,"Update Iboot.");
ADD_TO_IN_SHELL bool updateiboot(char *param)
{
   Lock_SempPost(ptUpdateIbootSemp);
   return true;
}

bool IAP_UpdateIboot()
{
    return (Sh_UpdateIboot(0));
}

bool IAP_GetUpdateIbootStatus()
{
  return UpdateIbootFlag;
}
// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
//bool_t Sh_GetIbootVersion(char *pDummy)
ADD_TO_SHELL_HELP(ibootver,"get iboot version.");
ADD_TO_IN_SHELL bool_t ibootver(char *dummy)
{
    dummy = dummy;
    IAP_PrintIbootVersion();
    return (TRUE);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
struct shell_debug const shell_cmd_iap_table[]=
    {
        {
        "runiboot",
        runiboot,
        "�л���Iboot(���ڲ�ȡ�ڴ��ʾȷ��������Ŀ��APP�м���DJYOS shellģ��ʱ��Ч)",
        "COMMAND:runiboot+enter"
        },

        {
        "runapp",
        runapp,
        "ֱ������APP(���ڲ�ȡ�ڴ��ʾȷ��������Ŀʱ��Ч)",
        "COMMAND:runapp+enter"
        },

        {
        "appinfo",
        appinfo,
        "��ȡ����bin�ļ���Ϣ",
        "COMMAND:appinfo+enter"
        },

        {
        "iapstatus",
        iapstatus,
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
        iapmode,
        "��ȡ��ǰ����ģʽ(Iboot or APP)",
        "COMMAND:iapmode+enter"
        },

        {
        "updateapp",
        updateapp,
        "Update app.",
        "COMMAND:updateapp+enter"
        },

        {
            "ibootver",
            ibootver,
            "Get Iboot version.",
            "COMMAND:ibootver+enter"
        },
        {
            "updateiboot",
            updateiboot,
            "Update Iboot.",
            "COMMAND:updateiboot+enter",
        }
};

// ============================================================================
// ���ܣ���װSHELL����
// �������ޡ�
// ���أ�1�������壩��
// ��ע
// ============================================================================
#define CN_IAP_SHELL_NUM  sizeof(shell_cmd_iap_table)/sizeof(struct shell_debug)
//static struct ShellCmdRsc tg_iap_shell_cmd_rsc[CN_IAP_SHELL_NUM];
ptu32_t IAP_ShellInstall(void)
{
    shell_debug_add(shell_cmd_iap_table, CN_IAP_SHELL_NUM);
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

//    IAP_ShellInstall();
        info_printf("module","IAP installed.");
    return 0;
}

// ============================================================================
// ���ܣ�����Iboot�¼���
// �������ޡ�
// ���أ�1�������壩��
// ��ע
// ============================================================================
ptu32_t __UpdateIboot(void)
{
    s32 Dev;
    DIR *dir;
    char *path;
    struct FlashChip *Chip;
    struct dirent *structDirent;
    char *buf;
    char buffer[256];
    u8 found = 0;
    struct stat sourceInfo;
    s32 source = -1;
    s32 res;
    u32 Res;
    u8 RootLen = strlen(CFG_IBOOT_UPDATE_PATH);
    uint32_t dwPageBytes=0;
    uint32_t dwPageNo=0;
    u8 i=0;
    u32 SectorEnd=0,Remains=0;
    u32 k=0;

    while(1)
    {
       Lock_SempPend(ptUpdateIbootSemp,CN_TIMEOUT_FOREVER);
       UpdateIbootFlag=false;
       dir=opendir(CFG_IBOOT_UPDATE_PATH);
       if(dir)
       {
           while((structDirent = readdir(dir)) != NULL)
           {
              if(0 == strcmp(structDirent->d_name, CFG_IBOOT_UPDATE_NAME))
               {
                  debug_printf("IAP","file \"%s\" will be programmed.\r\n",
                            structDirent->d_name);
                    found = 1;
                    break;
               }
           }
           if(!found)
           {
               debug_printf("IAP","file \"%s\" is not found.\r\n", CFG_IBOOT_UPDATE_NAME);
               goto END;
           }
           path = buffer;
           strcpy(path, CFG_IBOOT_UPDATE_PATH);
           strcpy(path+RootLen, "/");
           strcpy(path+RootLen+1, structDirent->d_name);
           source = open(path, O_RDONLY);
           if(-1 == source)
           {
               debug_printf("IAP","cannot open source file \"%s\".\r\n", path);
               goto END;
           }
           res = fstat(source, &sourceInfo);
           if(res)
           {
               debug_printf("IAP","cannot statistics source file \"%s\".\r\n", path);
               goto END;
           }

           // Dev = DevOpen("embedded flash", O_RDWR, CN_TIMEOUT_FOREVER);
           // if(Dev==NULL)
           Dev = open("dev/embedded flash", O_RDWR);
           if(Dev==-1)
           {
               debug_printf("IAP","cannot open embedded flash.\r\n");
              goto END;
           }

           //Chip=(struct FlashChip *)Dev_GetDevTag(Dev);
           //if(Chip==NULL)
           if(-1 == fcntl(Dev, F_GETDDRV, &Chip))
           {
               goto END;
           }
           dwPageBytes=Chip->dwPageBytes;
           buf=malloc(dwPageBytes);
           if(buf==NULL)
           {
               debug_printf("IAP","memroy out.\r\n");
               goto END;
           }
           dwPageNo=gc_ptIbootSize/dwPageBytes;
           res=Chip->Ops.PageToBlk(dwPageNo,&Remains,&SectorEnd);
           if(res==-1)
           {
               debug_printf("IAP","Calculate the flash sector failed.\r\n");
               goto END;
           }
           dwPageNo=0;
           for(i=0;i<SectorEnd;i++)
           {
               res=Chip->Ops.ErsBlk(i);
               if(res==-1)
               {
                   debug_printf("IAP","Erase the sector failed.\r\n");
                   goto END;
               }
           }
           while(1)
           {
               res = read(source, buf, dwPageBytes);
               if(!res)
               {
                   debug_printf("IAP","read source file error.\r\n");
                    break; // û�����ݿɶ�
               }
               sourceInfo.st_size -= res; // ʣ��
               if(res<dwPageBytes)
               {
                   for(k=res;k<dwPageBytes;k++)
                   {
                       buf[k]=0xFF;
                   }
               }
               Res=Chip->Ops.WrPage(dwPageNo,buf,NO_ECC);
               if((res !=Res)&&(Res!=dwPageBytes))
               {
                   debug_printf("IAP","write destination file error.\r\n");
                    goto END;
               }
               dwPageNo++;
               if(!sourceInfo.st_size)
               {
                   debug_printf("IAP","Iboot update success.\r\n");
                   UpdateIbootFlag=true;
                   goto END; // ȫ������
               }

           }

END:
            closedir(dir);
            if(-1 != source)
            {
                res = close(source);
                if(res)
                {
                    debug_printf("IAP","close source file failed.\r\n");
                }
            }
       }
    }
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
#if (CFG_APP_RUNMODE == EN_FORM_FILE)
           fp=fopen(CFG_APP_FILENAME,"r");
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
                if(0 == stat(CFG_APP_FILENAME,&FpInfo))
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
#endif
        }

    }
LOAD_FILE_EXIT:
    pg_IapVar.RunMode = CN_IAP_MODE_IBOOT;
    return result;
}




// ============================================================================
// ���ܣ�ͨ��ģʽһ����Ӧ�ó���
// ������pPath -- �ļ�����·��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
void UpdateApplication(void)
{
    bool_t doUpdate;
    s32 res;
    u8 doPrint = 1;
    doUpdate = HAL_CheckUpdateFlag();
    if(doUpdate)
    {
        if(doPrint)
        {
            debug_printf("IAP","APP need update(flag).");
            doPrint = 0;
        }

        res = IAP_Update(2, IAP_GetMethod(), IAP_GetPath());
        if(res != -1)
        {
            HAL_ClrUpdateFlag();
            debug_printf("IAP","APP update succeed. Now going to running APP in 5 seconds.\r\n");
            Djy_EventDelay(5000*mS);
            RunApp();
        }
        else
        {
            debug_printf("IAP","APP update failed. will retry.");
        }
    }
    else if(pg_IapVar.IbootStatus==EN_CRC_ERR)
    {
        if(doPrint)
        {
            debug_printf("IAP","APP need update(status).");
            doPrint = 0;
        }
        res = IAP_Update(2, IAP_GetMethod(), IAP_GetPath());
        if(res != -1)
        {
            debug_printf("IAP","APP update succeed. Now going to running APP in 5 seconds.\r\n");
            Djy_EventDelay(5000*mS);
            RunApp();
        }
        else
        {
            debug_printf("IAP","APP update failed. will retry.");
        }
    }
    else
    {

    }
}





