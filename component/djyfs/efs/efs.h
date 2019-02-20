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
#ifndef __EFS_H__
#define __EFS_H__

#ifdef __cplusplus
extern "C" {
#endif
//
//
//
#include "stddef.h"

#define ESIG_LEN                20
#define ESIGN                   "easy file system v0" // ������������20���ֽ�
#define SYS_LOOPS               6
#define SYS_TMP                 ((u64)-1)
#define BUFLEN                  (252) // �ļ������С,ÿд��ô�����ݻ��һ��4�ֽڵ�ECCֵ
#define BUFBITS                 (8)
#define KEYLEN                  (6) // �ļ�keyֵ��С��
//
// efs�ļ�ϵͳ��flash�Ĳ���������
//
struct __efs_drv{
    s32 (*efs_write_media) (struct __ecore *core, s64 units, void *buf);
    s32 (*efs_read_media) (struct __ecore *core, s64 units, void *buf);
    s32 (*efs_erase_media) (struct __ecore *core, s64 units, s64 size);
};
//
// �ļ�ϵͳ���ƽṹ��
//
struct __ecore{
    u32 ssz; // ϵͳ��Ϣ��С,�������ļ�ϵͳ��Ϣ��idx����ļ���Ϣ������룬��unitΪ��λ����
    u8 serial; // ��ǰʹ�õ�ϵͳ��Ϣ��������ϵͳ���ļ���;
    u8 finfo; // �ļ���Ϣ���ڵ�ƫ�ã�����unitΪ��λ����
    u8 ecc[256]; // ����ECCУ�飻
    u8 eccu; // һ��unit�ڵ��ļ����ݱ�ECCռ�õĿռ䣬�����ֽ�Ϊ��λ��
    u8 *ebuf; // ���壬media��unit��С
    u16 nsz; // �ļ�������
    u32 fmax; // �������ļ������ֵ�����ʱ��һ���ļ�����һ���ļ�����
    u32 fsz; // �ļ�������;������룬��unitΪ��λ����
    s64 MStart;             // ��ý���е���ʼunit,unitΪ��λ��
    s64 ASize;               // ����������ܴ�С��ByteΪ��λ��
    struct umedia *media;
    struct MutexLCB *mutex;
    struct __efs_drv *drv;
};

//
// ����
//
struct __eidx{
    u8 key[KEYLEN];
    u8 bakup[KEYLEN];
    u32 order;
    u32 ecc;
};

//
// �ļ�ϵͳ��Ϣ�������У�
//
struct __ecstruct{
    char signature[ESIG_LEN];
    s64 range; // �ļ�ϵͳ��������
    u32 files; // �ļ�����
    u64 age; //
    u32 ecc; // ǰ���CRCУ��ֵ��
    u32 status; //
#define SYS_UPDATING        0xFFFFFFFF
#define SYS_UPDATED         0x55555555
#define SYS_NOECCSIZE       8
};

//
// �ļ���Ϣ
//
struct __esize{
    s64 s;
    u32 e;
};

struct __ename{
    char *n;
    u32 *e;
};

struct __efstruct{
    struct __ename name; //
    struct __esize *size; //
};

//
// �ļ�����λ��
//
struct __loc{
    list_t list;
    u32 loc;
    u32 order; // ��һ��
};

//
// �����ļ�ϵͳ�ṹ
//
struct __efile{
//    struct __portBasic basic; // �����ļ�ϵͳ��
    u32 flags; // �ļ����Ʊ�־λ��
    s64 size;
    struct __loc *loc; // �ļ�����λ�ã���idx��ƫ�ã�
};

struct __econtext{
    u8 wbuf[BUFLEN];
    u8 wpos; // wbufʹ������
    u8 dirty; // ���������Ƿ��ѱ���д��
    s64 pos; // ��ǰλ��Ϊwpos+pos;
};

//
// ɨ���߼�
//
struct __scanlogic{
    enum escan{
        nosys,
        badsys,
        brokensys,
        goodsys,
    }stat;
    u32 age;
};

enum locmove{
    position,
    direct,
};

struct __dentry{
    char *name;
    struct __dentry *nxt;
};

struct __dentrys{
    u32 items;
    u32 scans;
    struct __dentry *item;
};

#ifdef __cplusplus
}
#endif

#endif
