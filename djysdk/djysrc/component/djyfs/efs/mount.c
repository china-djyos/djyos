//-----------------------------------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
#include <string.h>
#include <stdlib.h>
#include <driver/flash/flash.h>
#include <djyfs/filesystems_configs.h>
#include "efs_port.h"

//
// EFS���ļ�ϵͳ�����ӿ�
//
struct FileOperations EfsFileOps =
{
    .FileOpen       = EfsOpen,
    .FileClose      = EfsClose,
    .FileDelete     = EfsDelete,
    .FileSync       = EfsSync,
    .FileSeek       = EfsSeek,
    .FileWrite      = EfsWrite,
    .FileRead       = EfsRead,
    .FileStat       = EfsStat,
    .DirRead        = EfsDirRead,
};
//
// EFS���ļ�ϵͳ����
//
struct FileSysType EFS =
{
    .FileOps = &EfsFileOps,
    .VOps = NULL,
    .Install = EfsInstall,
    .Uninstall = EfsUninstall,
    .Format = EfsFormat,
    .Property = MOUNT_FILE,
    .MountList = NULL,
    .Name = "EFS",
};
//-----------------------------------------------------------------------------
//����:1.���̷���EFS����ȫ��ɨ�裻2.��ʼ��EFS�ļ�
//����: 
//����: 0 -- �ɹ�; -1 -- ʧ��;
//��ע: 
//-----------------------------------------------------------------------------
#define INSTALL_MAKE_NEW             (0x1)
s32 EfsInstall(struct MountInfo *Info, void *Private)
{
    char *DiskName;
    tagEFS *NewEfs;
    FSRESULT Ret;
    u32 Length;

    Length = strlen(Info->Dev->Name);
    DiskName = malloc(Length + 2);// �ļ�ϵͳ����,FAT�¾��Ƿ�����
    if(NULL == DiskName)
        return (-1);

    strcpy(DiskName, Info->Dev->Name);
    DiskName[Length] = ':';
    DiskName[Length+1] = '\0';

    NewEfs = malloc(sizeof(tagEFS));
    if(NULL == NewEfs)
        return (-1);
    
    memset(NewEfs,0x00,sizeof(tagEFS));
    Ret = Efs_Mount(NewEfs, DiskName, 1);// ����
    if ((FSR_NO_FILESYSTEM == Ret) && ((*(u32*)Private) & INSTALL_MAKE_NEW))
        Ret = Efs_Mkfs(NewEfs,DiskName, 1, 0);//�豸�ϲ������ļ�ϵͳ�����½�

    if(FSR_OK != Ret)// ʧ��
    {
        free(NewEfs);
        return (-1);
    }

    // �ɹ�
    Info->Mount.Name = DiskName;
    Info->Private = (void*)NewEfs;
    return (0);
}

//-----------------------------------------------------------------------------
//����:
//����: 
//����: 0 -- �ɹ�;
//��ע: 
//-----------------------------------------------------------------------------
s32 EfsUninstall(struct MountInfo *Info)
{
    
    free(Info->Mount.Name);
    free(Info->Private);
    return (0);
}

//-----------------------------------------------------------------------------
//����:
//����: 
//����: 0 -- �ɹ�; -1 -- ��������; -3 -- ��ʽ��ʧ��;
//��ע: 
//-----------------------------------------------------------------------------
s32 EfsFormat(void *Private)
{
    FSRESULT Ret;
    tagEFS *NewEfs = (tagEFS *)Private;

    if(!NewEfs)
        return (-1);

    Ret = Efs_Mkfs(NewEfs, NewEfs->name, 1, 0);
    if(FSR_OK != Ret)// ʧ��
        return (-3);

    return (0);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����: 0 -- �ɹ�;
//��ע:
//-----------------------------------------------------------------------------
#if 1

tagdevEFS *pDevEFS;

bool_t ModuleInit_EfsInfo(tagEFS* pEFS, char *pName)
{
    u8 *buf;

    if(!pDevEFS)
    {
        printf("EFS : debug : EFS has no low level device.\r\n");
        return (FALSE);
    }

    buf = (u8*)malloc(EFS_CFG_LIMIT);
    if(buf == NULL)
        return (FALSE);

    pEFS->block_buf_mutex = Lock_MutexCreate("EFS_BLOCK_BUF_SEMP");
    if(NULL == pEFS->block_buf_mutex)
    {
        free(buf);
        return (FALSE);
    }

    strcpy(pEFS->name, pName);
    pEFS->block_size = pDevEFS->dwBlockBytes; // ���С
    pEFS->start_block = pDevEFS->dwStart; // ���ļ�ϵͳ��ʼsector
    pEFS->block_sum = pDevEFS->dwTotal; // sector���ڴ�Ÿ��ļ�ϵͳ
    pEFS->block_buf = buf;
    pEFS->files_sum = 0;
    pEFS->erase_block = pDevEFS->EFS_IF_Erase;
    pEFS->query_block_ready = pDevEFS->EFS_IF_CheckBlockReady;
    pEFS->read_data = pDevEFS->EFS_IF_ReadData;
    pEFS->write_data = pDevEFS->EFS_IF_WriteData;

    __GetConfiguration();

    return TRUE;
}


#else

#ifdef EFS_ON
extern struct NandDescr *s_ptNandInfo;
extern bool_t EFS_IF_Erase(u32 BlkNo);
extern u32 EFS_IF_ReadData(u32 BlkNo, u32 Offset, u8 *Buf, u32 Size, u8 Flags);
extern u32 EFS_IF_WriteData(u32 BlkNo, u32 Offset, u8 *Buf, u32 Size, u8 Flags);
extern bool_t EFS_IF_CheckBlockReady(u32 block,u32 offset, u8 *buf,u32 size);
extern s32 EFS_IF_IsFragmentWritten(u32 BlkNo, u32 Offset);

bool_t ModuleInit_EfsInfo(tagEFS* efs,char *para)
{
    //оƬ��ʼ��
    u8 *chip_sector_buf;

    chip_sector_buf = (u8*)malloc(EFS_CFG_LIMIT);
    if( (chip_sector_buf == NULL) || (s_ptNandInfo == NULL))
        return FALSE;


    efs->block_buf_mutex = Lock_MutexCreate("EFS_BLOCK_BUF_SEMP");
    if(NULL == efs->block_buf_mutex)
    {
        free(chip_sector_buf);
        return FALSE;
    }

    strcpy(efs->name,para);
    efs->block_size          = s_ptNandInfo->BytesPerPage * s_ptNandInfo->PagesPerBlk;//���С
    efs->start_block         = 0;//���ļ�ϵͳ��ʼsector
    efs->block_sum           = s_ptNandInfo->BlksPerLUN;//sector���ڴ�Ÿ��ļ�ϵͳ
    efs->block_buf           = chip_sector_buf;
    efs->files_sum           = 0;
    efs->erase_block         = EFS_IF_Erase;
    efs->query_block_ready   = EFS_IF_CheckBlockReady;
    efs->read_data           = EFS_IF_ReadData;
    efs->write_data          = EFS_IF_WriteData;
    return true;

}
#else
bool_t ModuleInit_EfsInfo(tagEFS* efs,char *para)
{
    return FALSE;
}
#endif

#endif
//-----------------------------------------------------------------------------
//����:
//����:
//����: 0 -- �ɹ�;
//��ע:
//-----------------------------------------------------------------------------
bool_t Efs_Initlization(tagEFS* efs,char *para)
{
    return ModuleInit_EfsInfo(efs,para);
}


