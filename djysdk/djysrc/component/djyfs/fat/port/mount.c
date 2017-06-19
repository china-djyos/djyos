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

#include <stdio.h>
#include <string.h>
#include <driver.h>
#include <djyos.h>
#include "fat_port.h"
#include "./drivers/fat_drivers.h"
//
// FAT���ļ�ϵͳ�����ӿ�
//
struct FileOperations g_tFATFileOps =
{
    .FileOpen       = FATOpen,
    .FileClose      = FATClose,
    .FileDelete     = FATDelete,
    .FileSync       = FATSync,
    .FileSeek       = FATSeek,
    .FileWrite      = FATWrite,
    .FileRead       = FATRead,
    .FileStat       = FATStat,
    .FileTruncate   = FATTruncate,
	.DirRead        = FATDirRead
};
//
// FAT���ļ�ϵͳ����
//
struct FileSysType FAT =
{
    .FileOps = &g_tFATFileOps,
    .VOps = NULL,
    .Install = FATInstall,
    .Uninstall = FATUninstall,
    .Property = MOUNT_FILE,
    .MountList = NULL,
    .Name = "FAT",
};

//-----------------------------------------------------------------------------
//����:
//����:
//����: 0 -- �ɹ�; -1 -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
#define INSTALL_MAKE_NEW            (0x1)
#define INSTALL_DELAY				(0x2) // ʹ��ʱ�ŷ�����װ����
s32 FATInstall(struct MountInfo *pInfo, void *pOptions)
{
    char *volume, *temp;
    FATFS *structFAT;
    FRESULT res;
    u32 length;
    s32 volumeNum;
    struct DjyDevice *device;
    u32 options = (*(u32*)pOptions) & INSTALL_DELAY;

    device = Container(pInfo->Dev, struct DjyDevice, Node);
    length = strlen(pInfo->Dev->Name);
    volume = malloc(length+2); // �ļ�ϵͳ������FAT�¾��Ƿ���������Ҫ���':'
    if(!volume)
        return (-1);

    strcpy(volume, pInfo->Dev->Name);
    volume[length] = ':';
    volume[length+1] = '\0';
    temp = volume; // ִ��get_ldnumber()�����volume

    volumeNum = get_ldnumber((const char**)&temp); // ��Ҫffconf.h�ж���"_VOLUME_STRS"
    if (volumeNum < 0)
    {
        printf("\r\nFAT: mount failed, can not find the predefined device(volume) \"%s\"\r\n", volume);
        free(volume);
    	return (-1); //
    }

    if(FatDrvInitialize(LD2PD(volumeNum), (struct FatDrvFuns*)(device->PrivateTag)))
    {
        free(volume);
    	return (-1); // ��װ����ʧ��
    }

    structFAT = malloc(sizeof(FATFS));
	if(!structFAT)
	{
		free(volume);
		return (-1);
	}

    res = f_mount(structFAT, volume, options); // ����
    if ((FR_NO_FILESYSTEM == res) && (*((u32*)pOptions) & INSTALL_MAKE_NEW))
        res = f_mkfs(volume, 1, 0); // �豸�ϲ������ļ�ϵͳ�����½�FAT

    if(FR_OK != res) // ʧ��
    {
        free(structFAT);
        free(volume);
        return (-1);
    }

    // �ɹ�
    pInfo->Mount.Name = volume;
    pInfo->Private = (void*)structFAT;
    return (0);
}

//-----------------------------------------------------------------------------
//����:
//����:
//����: 0 -- �ɹ�;
//��ע:
//-----------------------------------------------------------------------------
s32 FATUninstall(struct MountInfo *pInfo)
{

    free(pInfo->Mount.Name);
    free(pInfo->Private);
    return (0);
}
