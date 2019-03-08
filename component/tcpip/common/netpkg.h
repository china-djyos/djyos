//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ:
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ��������:������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

#ifndef __NETPKG_H__
#define __NETPKG_H__
struct NetPkg;
void PkgInit(struct NetPkg *pkg, u8 flag, u16 offset, u16 datalen, u8* buf);
bool_t Pkg_SetAlignOffset(u16 alignsize);
u16 PkgAlignSize(u16 size);
struct NetPkg *PkgMalloc(u16 bufsize, u8 flags);
bool_t PkgTryFreePart(struct NetPkg *pkg);
struct NetPkg *PkgGetNextUnit(struct NetPkg *NextUnit);
void PkgSetNextUnit(struct NetPkg *pkg,struct NetPkg *NextUnit);
u8* PkgGetCurrentBuffer(struct NetPkg *pkg);
u8* PkgGetBuffer(struct NetPkg *pkg);
void PkgSetBuffer(struct NetPkg *pkg, u8 *buf);
u16 PkgGetDataLen(struct NetPkg *pkg);
void PkgSetDataLen(struct NetPkg *pkg,u16 len);
u16 PkgGetOffset(struct NetPkg *pkg);
void PkgSetOffset(struct NetPkg *pkg,u16 offset);
ptu32_t PkgGetPrivate(struct NetPkg *pkg);
void PkgSetPrivate(struct NetPkg *pkg,ptu32_t PkgPrivate);
bool_t PkgIsBufferEnd(struct NetPkg *pkg);
void PkgMoveOffsetUp(struct NetPkg *pkg, u16 len);
void PkgMoveOffsetDown(struct NetPkg *pkg, u16 len);
u16 PkgListDatastatistics(struct NetPkg *pkg);
u16 PkgFrameDatastatistics(struct NetPkg *pkg);
u16 PkgListDataCopy(struct NetPkg *pkg,u8 *dst);
u16 PkgFrameDataCopy(struct NetPkg *pkg,u8 *dst);
void PkgCopyListToPkg(struct NetPkg *pkg,struct NetPkg *dst);
void PkgCopyFrameToPkg(struct NetPkg *pkg,struct NetPkg *dst);
bool_t PkgTryFreeLst(struct NetPkg  *pkglst);
bool_t PkgTryFreeQ(struct NetPkg  *pkglst);
bool_t PkgCachedPart(struct NetPkg  *pkg);
bool_t PkgCachedLst(struct NetPkg   *pkglst);
bool_t PkgModuleInit(void);
#endif // for __NETPKG_H__
