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
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <cpu_peri.h>
#include <device/flash/flash.h>
#include <device/include/unit_media.h>
#include <device.h>
#include <spibus.h>
#include <systime.h>
#include <math.h>
#include "at45db321.h"
#include <dbug.h>
#include <filesystems.h>
#include <efs.h>


extern s32 __at45_write(s64 unit, void *data, struct uopt opt);
extern s32 __at45_read(s64 unit, void *data, struct uopt opt);
extern s32 __at45_erase(s64 unit, struct uesz sz);

s32 efs_at45_erase(struct __ecore *core, s64 units, s64 size);
s32 efs_at45_write(struct __ecore *core, s64 units, void *buf);
s32 efs_at45_read(struct __ecore *core, s64 units, void *buf);

struct __efs_drv EFS_AT45_DRV =
{
    .efs_erase_media = efs_at45_erase,
    .efs_read_media = efs_at45_read,
    .efs_write_media = efs_at45_write
};

// ============================================================================
// ���ܣ�����
// ������core -- �ļ�ϵͳ���ƣ�
//      units -- ��ʼunit�ţ�
//      buf -- ���ݣ�
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע����unit�ռ��С����
// ============================================================================
s32 efs_at45_read(struct __ecore *core, s64 units, void *buf)
{
    struct uopt opt = {0};

    opt.necc = 1;
    opt.main = 1;
    units = core->MStart + units;
    return (__at45_read(units, (u8*)buf, opt));
}

// ============================================================================
// ���ܣ�д��
// ������core -- �ļ�ϵͳ���ƣ�
//      units -- ��ʼunit�ţ�
//      buf -- ���ݣ�
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע����unit�ռ��Сд��
// ============================================================================
s32 efs_at45_write(struct __ecore *core, s64 units, void *buf)
{
    struct uopt opt = {0};

    opt.necc = 1;
    opt.main = 1;
    units = core->MStart + units;
    return (__at45_write(units, (u8*)buf, opt));

}

// ============================================================================
// ���ܣ�������
// ������core -- �ļ�ϵͳ���ƣ�
//      units -- ��ʼunit�Ų������谴�ɲ��ߴ���룻
//      size -- �������unit�������谴�ɲ��ߴ���룻
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 efs_at45_erase(struct __ecore *core, s64 units, s64 size)
{
    s32 res = -1;
    s64 erases;
    struct uesz sz;

    sz.unit = 1;
    sz.block = 0;
    erases = 1 << (core->blacksize - core->pagesize); // �ɲ����ռ������е�unit����
    if((size % erases) || (units % erases))
    {
        printf("\r\n: warn : efs    : not aligned area will be erased(%d, %d)", (u32)units, (u32)size); // ���������ݲ�û�ж��룬���ܴ�������
    }

    while(size > 0)
    {
        res = __at45_erase(units, sz);
        if(-1 == res)
            return (-1);

        units += erases;
        size -= erases;
    }

    return (0);
}
