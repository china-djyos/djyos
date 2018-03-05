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
#ifndef __EFS_PORT_H__
#define __EFS_PORT_H__

#if 0
#include <stdlib.h>

#include "../djyfs/mount.h"
#include "../djyfs/vfile.h"
#include "../djyfs/iofile.h"
#endif

#include "efs.h"
#include <djyfs/mount.h>
#include <djyfs/vfile.h>

typedef struct __devEFS
{
    u32 dwStart; // ��ʼ��
    u32 dwTotal; // ������
    u32 dwBlockBytes; // ���С
    bool_t (*EFS_IF_Erase)(u32 dwBlock);
    u32 (*EFS_IF_ReadData)(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 Size, u8 dwFlags);
    u32 (*EFS_IF_WriteData)(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 Size, u8 dwFlags);
    bool_t (*EFS_IF_CheckBlockReady)(u32 dwBlock,u32 dwOffset, u8 *pBuf,u32 dwSize);
}tagdevEFS;

// 
// ��������
//
 struct FileContext *EfsOpen(const char *Path, u32 Mode, const char *Root);
 s32 EfsClose(struct FileContext *FileCt);
 s32 EfsSync(struct FileContext *FileCt);
 s32 EfsSeek(struct FileContext *FileCt, s64 Offset, int Whence);
 s32 EfsDelete(const char *Path, const char *Root);
 s32 EfsWrite(const void *Buf, u32 Size, u32 Nmemb, struct FileContext *FileCt);
 s32 EfsRead(void *Buf, u32 Size, u32 Nmemb, struct FileContext *FileCt);
 s32 EfsStat(struct FileContext *FileCt, const char *Path, struct Stat *Buf, const char *Root);
 s32 EfsDirRead(struct FileContext *FileCt, struct Dirent *Content);
 s32 EfsInstall(struct MountInfo *Info, void *Private);
 s32 EfsUninstall(struct MountInfo *Info);
 s32 EfsFormat(void *Private);

 s32 __GetConfiguration(void);
#endif/* __FAT_PORT_H__ */
