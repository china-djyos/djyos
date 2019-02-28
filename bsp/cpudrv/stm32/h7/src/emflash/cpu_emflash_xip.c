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
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <misc.h>
#include <systime.h>
#include <device/flash/flash.h> // will be obsolete
#include <device/include/unit_media.h>
#include <xip.h>

extern s32 __embed_read(s64 unit, void *data, struct uopt opt);
extern s32 __embed_req(enum ucmd cmd, ptu32_t args, ...);
extern s32 __embed_write(s64 unit, void *data, struct uopt opt);
extern s32 __embed_erase(s64 unit, struct uesz sz);

s32 xip_emflash_write(struct __icore *core, u8 *data, u32 bytes, u32 pos);
s32 xip_emflash_read(struct __icore *core, u8 *data, u32 bytes, u32 pos);
s32 xip_emflash_erase(struct __icore *core, u32 bytes, u32 pos);


struct __xip_drv XIP_EMFLASH_DRV =
{
    .xip_erase_media = xip_emflash_erase,
    .xip_read_media = xip_emflash_read,
    .xip_write_media = xip_emflash_write
};
// ============================================================================
// ���ܣ�д����
// ������core -- xip�ļ�ϵͳ������Ϣ
//      data -- ���ݻ���
//      bytes -- д�ֽ��������ᳬ��һ�������С��
//      pos -- ���ݵ�ַ
// ���أ��ɹ���0����ʧ�ܣ�-1������Ҫû�п�д�ռ䣨-2����
// ��ע����д�����һ��unitʱ���᳢�Բ���
// ============================================================================
s32 xip_emflash_write(struct __icore *core, u8 *data, u32 bytes, u32 pos)
{
    struct umedia *um = (struct umedia *)core->vol;
    struct uesz esz = {0};
    u32 j, offset, once, more;
    s32 left;
    s64 unit;
    u32 block = 0;

    left = bytes;
    unit = (pos >> um->usz) + core->MStart;
    offset = pos & ((1 << um->usz)-1); // unit��ƫ��
    __embed_req(lock, CN_TIMEOUT_FOREVER);
    while(left>0)
    {
#if 0
        // �����ǰд��ҳ��һ�����е����һҳ����Ԥ��ɾ��������sector
        // (page+1)���ڷ�ֹ��ʽ���˲�����xip�Ŀռ�
        __embed_req(remain, (ptu32_t)&more, &unit);
        if(!more)
        {
            // +1�Ǳ�ʾ��ǰunit�ĺ���һ��
            if(((unit-um->ustart+1)<<um->usz)<um->asz)
            {
                struct uesz sz = {0};
                sz.unit = 1;
                __embed_erase(unit+1, sz); // ������û�в����ɹ�����Ϊ�������д��Ļ������лض�У��
            }
        }
#endif
        if(__embed_read(unit, um->ubuf, um->opt))
        {
            __embed_req(unlock, 0); //
            return (-1);
        }

        once = 1<<um->usz;
        if(left<(s32)once)
            once = left;

        for(j=0; j<(once); j++)
        {
            if(um->ubuf[j+offset]!=data[j])
            {
                if(0xFF!=um->ubuf[j+offset])        //�жϵ�ǰҳ�Ƿ�ΪFF
                {
                    __embed_req(unlock, 0);
                    return (-1);
                }
            }
        }

        memcpy((um->ubuf + offset), data, once);
        if(__embed_write(unit, um->ubuf, um->opt))
        {
            __embed_req(unlock, 0);
            return (-1);
        }

        left -= once;
        offset = 0;
        data += once;
        if(left)
            unit++;
    }

    // �����ǰд��ҳ��һ�����е����һҳ����Ԥ��ɾ��������sector
    // (page+1)���ڷ�ֹ��ʽ���˲�����xip�Ŀռ�
    __embed_req(remain, (ptu32_t)&more, &unit);
    if(!more)
    {
        // +1�Ǳ�ʾ��ǰunit�ĺ���һ��
        if(((unit-core->MStart+1)<<um->usz) >= core->ASize)
        {
            return (-2);
        }
        esz.block = 1;
//        esz->unit = 0;
        __embed_req(whichblock, (ptu32_t)&block, &unit);
        //block�ǵ�ǰҳ���ڵĿ�ţ�block+1��Ϊ�˲�����һ���飨block+1��Ҫ�����Ŀ飬����block+1+1��Ͳ����ˣ�
        __embed_req(format, block+1, block+1+1, &esz);
    }

    __embed_req(unlock, 0); //
    return (0);
}

// ============================================================================
// ���ܣ�������
// ������core -- xip�ļ�ϵͳ������Ϣ
//      data -- ���ݻ���
//      bytes -- ���ֽ���
//      pos -- ���ݵ�ַ
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע������Ҳ��ֻ���һ��unit��Ŀǰ
// ============================================================================
s32 xip_emflash_read(struct __icore *core, u8 *data, u32 bytes, u32 pos)
{
    struct umedia *um = (struct umedia *)core->vol;
    s64 unit;
    u32 offset;
    s32 left = bytes, once;

    unit = (pos >> um->usz) + core->MStart;
    offset = pos & ((1 << um->usz) - 1); // unit��ƫ��
    __embed_req(lock, CN_TIMEOUT_FOREVER);
    while(left>0)
    {
        once = MIN(((1 << um->usz) - offset), left);
        if(__embed_read(unit, um->ubuf, um->opt))
        {
            __embed_req(unlock, 0); //
            return (-1);
        }

        memcpy(data, (um->ubuf + offset), once);
        left -= once;
        offset = 0;
        unit++;
        data += once;
    }

    __embed_req(unlock, 0); //
    return (0);
}

// ============================================================================
// ���ܣ���������
// ������core -- xip�ļ�ϵͳ������Ϣ
//       dwBytes -- �ֽ���
//       dwAddr -- ���ݵ�ַ
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 xip_emflash_erase(struct __icore *core, u32 bytes, u32 pos)
{
    struct umedia *um = (struct umedia *)core->vol;
    struct uesz esz = {0};
    s64 unit;
    u32 erases, offset;
    s32 left = bytes;

    esz.unit = 1;
    unit = (pos >> um->usz) + core->MStart;
    offset = pos & ((1 << um->usz)-1); // unit��ƫ��
    __embed_req(lock, CN_TIMEOUT_FOREVER);
    while(left>0)
    {
        if(__embed_req(remain, (ptu32_t)&erases, (ptu32_t)&unit))
        {
            printf("\r\n: erro : xipfs  : erase unit %lld failed, cannot get remain.", unit);
            return (-1);
        }

        if(__embed_erase(unit, esz))
        {
            printf("\r\n: erro : xipfs  : erase unit %lld failed, with erases %d.", unit, erases);
            return (-1);
        }

        erases++; // ������һ����ʾ������ǰ��unit
        left -= ((erases << um->usz) - offset);
        unit += erases;
        offset = 0;
    }

    return (0);
}


// ============================================================================
// ���ܣ���ʽ��xip�ļ�ϵͳ
// ������core -- xip�ļ�ϵͳ������Ϣ
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 xip_fs_format(struct __icore *core)
{
    u32 remainunit = 0,block = 0,endunit = 0;
    s64 startunit = core->MStart;
    struct uesz esz = {0};
    esz.block = 1;
    endunit = (core->ASize/core->bufsz) + startunit;
    __embed_req(whichblock, (ptu32_t)&block, (ptu32_t)&startunit);
    if(__embed_erase(block, esz))
    {
        printf("\r\n: erro : xipfs  : erase block %d failed.", block);
        return (-1);
    }
    while(1)
    {
        __embed_req(remain, (ptu32_t)&remainunit, (ptu32_t)&startunit);
        startunit = startunit + remainunit + 1;    //��1��������һ��ĵ�һҳ
        if(startunit >= endunit)
        {
            break;
        }
        __embed_req(whichblock, (ptu32_t)&block, &startunit);
        if(__embed_erase(block, esz))
        {
            printf("\r\n: erro : xipfs  : erase block %d failed.", block);
            return (-1);
        }
    }
    return (0);
}
