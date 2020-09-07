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

#ifndef __UNIT_MEDIA_H__
#define __UNIT_MEDIA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "jedec.h"
#include "nor.h"
#include "nand.h"
#include "embedded_flash.h"


enum utype{
    nand = 1,
    nor,
    usb,
    emmc,
    mmc,
    sd,
    embed,
};

enum ucmd{
    remain = 1,     // ĳunit������block��������unit����req(remain, *left, *unit)
    whichblock,     // ĳunit������block�ĺ�, req(whichblock, *block, *unit)
    totalblocks,    // �ܿ���, req(totalblocks, *blocks)
    totalSectors,   // ��������, req(totalblocks, *blocks)
    unitbytes,      // ĳunit��Bytes, req(unitbytes, *bytes)
    sparebytes,     // ĳunit��spare�����bytes��req(sparebytes,*bytes);
    blockunits,     // һ�����е�unit������req(blockunits, *units, block)
    format,         // ��ʽ��ĳһ������;req(format, start, counts, *esz)
    lock,
    unlock,
    markbad,        // ���ĳ���ǻ��飻req(markbad, block)
    checkbad,       // ĳ���Ƿ��ǻ��飻req(checkbad, block)
    savebads,       // ���滵���req(savebads, *table)
    getbads,        // ��ȡ�����req(getbads, **table)
    mapaddr,        // ��ȡ�洢��ӳ���ַ��req(mapaddr, *addr)
};

struct uopt{
    u32 hecc:1;     // Ӳ��ʵ��ECC
    u32 secc:1;     // ���ʵ��ECC
    u32 necc:1;     // ��ECC
    u32 main:1;     // 0 = ��д����
    u32 spare:1;    // OOB�ռ�
};

struct uesz{
    u32 unit:1;     // 1 = ����ҳ
    u32 block:1;    // 1 = ������
};

struct ustatistics{     //û�������õ�
    u32 erases;
    u32 brases;
    u32 reads;
    u32 breads;
    u32 writes;
    u32 bwrites;
};

struct umedia{
    enum utype type;
    u8 *ubuf;
    s32 (*mreq)(enum ucmd cmd, ptu32_t args, ...); // �ɹ���0����ʧ�ܣ�-1����
};

typedef s32 (*__um_req)(enum ucmd cmd, ptu32_t args, ...);

s32 um_add(const char *name, struct umedia *media);
s32 nandscanbads(__um_req req, u32 **table);
s32 nandvalidbads(u32 *badtable);
u32 *nandbuildbads(s32 (*um_req)(enum ucmd cmd, ptu32_t args, ...));
void nandbadfreeblock(u32 *badtable, u32 *block, s32 (*um_req)(enum ucmd cmd, ptu32_t args, ...));
void nandbadfreeunit(u32 *badtable, s64 *unit, __um_req req);

//s32 ModuleInstall_UnitMedia(s32(*dev_init)(u32 bstart, u32 bcount, u32 doformat), u8 parts, ...);

#ifdef __cplusplus
}
#endif

#endif // #ifndef __UNIT_MEDIA_H__
