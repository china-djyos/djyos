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
#if(CN_CPU_OPTIONAL_CACHE==1)
#include "set-cache.h"
#endif
#include <osboot.h>
#include <device/flash/flash.h>
#include "dbug.h"

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
bool updateiboot(char *param)
{
   Lock_SempPost(ptUpdateIbootSemp);
   return true;
}

bool XIP_UpdateIboot()
{
    return (updateiboot(0));
}

// ============================================================================
// ���ܣ�����Iboot�¼���
// �������ޡ�
// ���أ�1�������壩��
// ��ע
// ============================================================================
static ptu32_t __UpdateIboot(void)
{
#if 0
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
       dir=opendir(CFG_IBOOT_UPDATE_PATH);
       if(dir)
       {
           while((structDirent = readdir(dir)) != NULL)
           {
              if(0 == strcmp(structDirent->d_name, CFG_IBOOT_UPDATE_NAME))
               {
                  info_printf("IAP","file \"%s\" will be programmed.\r\n",
                            structDirent->d_name);
                    found = 1;
                    break;
               }
           }
           if(!found)
           {
               error_printf("IAP","file \"%s\" is not found.\r\n", CFG_IBOOT_UPDATE_NAME);
               goto END;
           }
           path = buffer;
           strcpy(path, CFG_IBOOT_UPDATE_PATH);
           strcpy(path+RootLen, "/");
           strcpy(path+RootLen+1, structDirent->d_name);
           source = open(path, O_RDONLY);
           if(-1 == source)
           {
               error_printf("IAP","cannot open source file \"%s\".\r\n", path);
               goto END;
           }
           res = fstat(source, &sourceInfo);
           if(res)
           {
               error_printf("IAP","cannot statistics source file \"%s\".\r\n", path);
               goto END;
           }

           Dev = open("dev/embedded flash", O_RDWR);
           if(Dev==-1)
           {
               error_printf("IAP","cannot open embedded flash.\r\n");
              goto END;
           }

           //Chip=(struct FlashChip *)Dev_GetDevTag(Dev);
           //if(Chip==NULL)
           if(-1 == fcntl(Dev, F_GETDRVTAG, &Chip))
           {
               goto END;
           }
           dwPageBytes=Chip->dwPageBytes;
           buf=malloc(dwPageBytes);
           if(buf==NULL)
           {
               error_printf("IAP","memroy out.\r\n");
               goto END;
           }
           dwPageNo=gc_ptIbootSize/dwPageBytes;
           res=Chip->Ops.PageToBlk(dwPageNo,&Remains,&SectorEnd);
           if(res==-1)
           {
               error_printf("IAP","Calculate the flash sector failed.\r\n");
               goto END;
           }
           dwPageNo=0;
           for(i=0;i<SectorEnd;i++)
           {
               res=Chip->Ops.ErsBlk(i);
               if(res==-1)
               {
                   error_printf("IAP","Erase the sector failed.\r\n");
                   goto END;
               }
           }
           while(1)
           {
               res = read(source, buf, dwPageBytes);
               if(!res)
               {
                   error_printf("IAP","read source file error.\r\n");
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
                   error_printf("IAP","write destination file error.\r\n");
                    goto END;
               }
               dwPageNo++;
               if(!sourceInfo.st_size)
               {
                   error_printf("IAP","Iboot update success.\r\n");
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
        result=XIP_IsRamIbootFlag();
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

           addr=XIP_GetAPPStartAddr() - 0x100;
           Buf=(char *)addr;

           fread(Buf,0x100,1,fp);           //read the 256 byte file info

           fseek(fp,0,0);

           if(XIP_APPIsDebug())             //dbg not need crc
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
               len=XIP_GetAPPSize();
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
#endif
    return 0;
}




// ============================================================================
// ���ܣ�ͨ��ģʽһ����Ӧ�ó���
// ������pPath -- �ļ�����·��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
void UpdateApplication(void)
{
/*    bool_t doUpdate;
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
    */
}
// ============================================================================
// ���ܣ���װIAP�����
// �������ޡ�
// ���أ�0�������壩��
// ��ע
// ============================================================================
ptu32_t ModuleInstall_IAP(void)
{
 /*   uint16_t evtt_id,event_id;
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
    */
    return 0;
}

ADD_TO_ROUTINE_SHELL(runiboot,runiboot,NULL);
ADD_TO_ROUTINE_SHELL(runapp,runapp,"ֱ������APP(���ڲ�ȡ�ڴ��ʾȷ��������Ŀʱ��Ч)");
ADD_TO_ROUTINE_SHELL(runibootui,runibootui,"��������iboot������iboot������iboot");
ADD_TO_ROUTINE_SHELL(updateruniboot,updateruniboot,"��������iboot������iboot");
ADD_TO_ROUTINE_SHELL(updateapp,updateapp,"update app lication");
ADD_TO_ROUTINE_SHELL(updateiboot,updateiboot,"Update Iboot.");


















