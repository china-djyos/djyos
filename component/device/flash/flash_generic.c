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

#include <stddef.h>
#include <endian.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "device/unit_media.h"
#include "device/djy_flash.h"      // will be obsolete
#include "dbug.h"

// ============================================================================
// ���ܣ�����NAND FLASH��ONFI����ҳ��
// ������data -- NandParamterPage
//      onfi -- Nand�����ṹ��
//      little -- NandParamterPage�ĸ�ʽ,"1"��ʾ32λС�ˡ�
// ���أ��ɹ�(0); ʧ��(-1);
// ��ע������ҳ�ľ������ݽṹ,Ҫ��ע����NAND FLASH�Ĳ����ֲ�
// ============================================================================
s32 deonfi(const struct NandONFI *data, struct NandDescr *onfi, u8 little)
{
    u8 i;

    char signature[4] = {'O', 'N', 'F', 'I'}; // ����ʶ���Ƿ���ONFI����

    // ONFI���
    if(little)
    {
        *(u32 *)signature = swapl(*(u32 *)signature);
    }
    for (i = 0; i < 4; i++)
    {
        if (signature[i] != data->signature[i])
        {
            printf("\r\n: info : device : flash's ONFI is unavailable\r\n");
            return (-1); // ������ݲ���Ҫ��
        }
    }

    // ONFI�汾��

    if(little)
        onfi->Revision = (u16)swapl(data->revision_num);
    else
        onfi->Revision = (u16)(data->revision_num);

    // Device Manufacturer
    for (i = 0; i < 12; i++)
    {
        onfi->Manufacturer[i] = data->dev_manufacturer[i];
    }
    if(little)
    {
        for (i = 0; i < 3; i++)
        {
            ((u32 *)onfi->Manufacturer)[i] = swapl(((u32 *)onfi->Manufacturer)[i]);
        }
    }
    onfi->Manufacturer[sizeof(onfi->Manufacturer)-1] = '\0';

    // Device Model
    for (i = 0; i < 20; i++)
    {
        onfi->DeviceModel[i] = data->dev_model[i];
    }
    if(little)
    {
        for (i = 0; i < 5; i++)
        {
            ((u32 *)onfi->DeviceModel)[i] = swapl(((u32 *)onfi->DeviceModel)[i]);
        }
    }
    onfi->DeviceModel[sizeof(onfi->DeviceModel) - 1] = '\0';

    // Number of data bytes per page
    if(little)
    {
        onfi->BytesPerPage = swapl((u32)(data->bytes_per_page));
    }
    else
    {
        onfi->BytesPerPage = (u32)data->bytes_per_page;
    }

    // Number of spare bytes per page
    if(little)
    {
        onfi->OOB_Size = swapl((u16)data->spare_bytes_per_page);
    }
    else
    {
        onfi->OOB_Size = (u16)data->spare_bytes_per_page;
    }

    // Number of pages per block
    if(little)
    {
        onfi->PagesPerBlk = swapl((u32)data->pages_per_block);
    }
    else
    {
        onfi->PagesPerBlk = (u32)data->pages_per_block;
    }

    // Number of blocks per LUN
    if(little)
    {
        onfi->BlksPerLUN = swapl((u32)data->blocks_per_LUN);
    }
    else
    {
        onfi->BlksPerLUN = (u32)data->blocks_per_LUN;
    }

    // Number of LUNs
    if(little)
    {
        onfi->LUNs = swapl((u8)data->LUNs);
    }
    else
    {
        onfi->LUNs = (u8)data->LUNs;
    }

 #if 0
    // Guaranteed valid block at the beginning of target
    for (i = 0; i < 4; i++)
    {
        temp[i] = (u8)data[104+i];
    }
    *(u32 *)temp = swapl(*(u32 *)temp);
    onfi->SafeBlkNo = temp[3];
 #endif

    printf("\r\n");
    printf(": info : device : flash's ONFI >>>>\r\n");
    printf("                  manufacturer is %s\r\n", onfi->Manufacturer);
    printf("                  model is %s\r\n", onfi->DeviceModel);
    printf("                  details (%d LUNs; %d blocks/LUN; %d pages/block; %d bytes/page; %d bytes/OOB",
           onfi->LUNs, onfi->BlksPerLUN, onfi->PagesPerBlk, onfi->BytesPerPage, onfi->OOB_Size);

 #if 0
   printf(";guaranteed %d blocks", onfi->SafeBlkNo );
 #endif
   printf(").");
    return (0);
}

#if 0
// ============================================================================
// ���ܣ� ��������оƬ(���������)
// ������
// ���أ� >=0 -- ���ڵĻ�������-1 -- ʧ��;
// ��ע��will be obsolete��
// ============================================================================
s32 EarseWholeChip(struct FlashChip *Chip)
{
    u32 i;
    u32 Blks;
    s32 BadBlks = 0;

    if(NULL == Chip)
        return (-1);

    if(F_NAND == Chip->Type)
    {
        Blks = Chip->Descr.Nand.BlksPerLUN * Chip->Descr.Nand.LUNs;

        for(i = Chip->Descr.Nand.ReservedBlks; i < Blks; i++)
        {
            if(0 != Chip->Ops.ErsBlk(i))
            {
                // ����ʧ��
                BadBlks++;
//                TraceDrv(FLASH_TRACE_ERROR, "block %d cannot be erased!\r\n", i);
                error_printf("flash","block %d cannot be erased!\r\n", i);
            }
        }
    }
    else if(F_NOR == Chip->Type)
    {
        if(0 == Chip->Descr.Nor.ReservedBlks)
        {
            if(0 != Chip->Ops.Special.Nor.ErsChip())
            {
                // ����ʧ��
                BadBlks = -1;
 //               TraceDrv(FLASH_TRACE_ERROR, "chip %d cannot be erased!\r\n", i);
                error_printf("flash","chip %d cannot be erased!\r\n", i);
            }
        }
        else
        {
            for(i = Chip->Descr.Nand.ReservedBlks; i < Chip->Descr.Nor.Blks; i++)
            {
                if(0 != Chip->Ops.ErsBlk(i))
                {
                    // ����ʧ��
                    BadBlks++;
 //                   TraceDrv(FLASH_TRACE_ERROR, "block %d cannot be erased!\r\n", i);
                    error_printf("flash","block %d cannot be erased!\r\n", i);
                }
            }
        }

    }
    else
    {
        BadBlks = -1;
    }
    return (BadBlks);
}
#endif

// ============================================================================
// ������Ϣ�ṹ
// ============================================================================
// ��ǩ          "bad blocks table" // 12�ֽ�
// ������                                                  // 4�ֽ�
// ��һ�������                                      // 4�ֽ�
// �ڶ��������                                      // 4�ֽ�
// �ȵ�
// ���128������
//

// ============================================================================
// ���ܣ���ӡ������Ϣ
// ������req -- unit media����
//      table -- �����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static void __print_bads(u32 *table)
{
    u32 i;

    printf("\r\n: info : device : bad blocks information:");
    printf("\r\n                  bad block nums %d", *table);

    for(i=0; i<*table; i++)
    {
        if(!(i%8))
            printf("\r\n                  ");

        printf("[%03d]", table[i+1]);
    }

    printf("\r\n");
}

// ============================================================================
// ���ܣ��������飬�����������
// ������req -- unit media����
//      table -- �����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
#define BAD_BLOCK_TAG           "bad blocks table"
s32 nandscanbads( __um_req req, u32 **table)
{
    u32 i, blocks;
    u32 tmp[128]; // Ԥ����128�����飻
    u8 bads = 0;

    if(req(totalblocks, (ptu32_t)&blocks))
        return (-1);

    for(i = 0; i < blocks; i++)
    {
        if(req(checkbad, i) != 0) // �ǻ���
        {
            tmp[bads] = i;
            bads++;
            if(bads>128)
            {
                info_printf("device", "too much bad blocks for nand.\r\n");
                return (-1);
            }
        }
    }

    *table = malloc((bads << 2) + 4);
    if(!(*table))
    {
        error_printf("device", "no memory for bad blocks table.\r\n");
        return (-1);
    }

    **table = bads; // ��ʼ���ڼ�¼bad����
    memcpy((u8*)(*table+1), tmp, bads<<2);
    __print_bads(*table);
    return (0);
}

// ============================================================================
// ���ܣ��Ƿ�����Ч�����
// ������badtable -- �����
// ���أ��ǣ�1������0����
// ��ע��
// ============================================================================
s32 nandvalidbads(u32 *badtable)
{
    if(strcmp((char*)badtable, BAD_BLOCK_TAG))
        return (0);

    return (1);
}
// ============================================================================
// ���ܣ����������
// ������req -- ����
// ���أ��ɹ����������ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
u32 *nandbuildbads(__um_req req)
{
    u8 *tmp, *badcount;
    s32 res;
    u32 *table, bytes;

    if(!req)
        return (NULL);

    if(req(unitbytes, (ptu32_t)&bytes))
        return (NULL);

    tmp = malloc(bytes);
    if(!tmp)
        return (NULL);

    res = req(getbads, (ptu32_t)&tmp); // ��ȡ�����
    if(-1==res)
    {
        // ��ȡʧ�ܣ�
        free(tmp);
        return (NULL);
    }

    // �鿴������Ƿ��������б�ǩ���������Ļ����
    if(strcmp(BAD_BLOCK_TAG, (char*)tmp)) // �����������������
    {

        if(nandscanbads(req, &table))
        {
            table = NULL;
        }
        else
        {
            // ����������ӱ�ǩ����д��nand
            strcpy((char*)tmp, BAD_BLOCK_TAG);
            memcpy((tmp+(strlen(BAD_BLOCK_TAG)+4)), table, ((table[0]+1)<<2)); //
            if(req(savebads, (ptu32_t)tmp)) // ���滵���
                debug_printf("device", "bad block table save failed for nand.\r\n");
        }
    }
    else // ������ ��ȡ���������ֹ���ڴ棻
    {
        badcount = (tmp + strlen(BAD_BLOCK_TAG) + 4);
        bytes = (*((u32*)badcount) + 1) << 2;
        table = malloc(bytes);
        if(table)
            memcpy(table, badcount, bytes);
    }

    free(tmp);
    return (table);
}

// ============================================================================
// ���ܣ���ȡ���λ����Ŀ�ţ�
// ������badtable -- �����
//      block -- ��ţ�
//      req -- ����
// ���أ��ޣ�
// ��ע��
// ============================================================================
void nandbadfreeblock(u32 *badtable, u32 *block, __um_req req)
{
    u32 i, count = badtable[0];

    if(!req || !badtable || !block)
        return;

    for(i = 1; i <= count; i++)
    {
        if(*block >= badtable[i])
        {
            *block += 1;
        }
        else
        {
            break;
        }
    }
}

// ============================================================================
// ���ܣ���ȡ���λ�������µ�unit�ţ�
// ������badtable -- �����
//      unit -- unit�ţ�
//      req -- ����
// ���أ��ޣ�
// ��ע��
// ============================================================================
void nandbadfreeunit(u32 *badtable, s64 *unit, __um_req req)
{
    u32 i, block = 0, count = badtable[0], movs = 0, units = 0;

    if(!req || !badtable || !unit)
        return;

    req(whichblock, (ptu32_t)&block, *unit);
    for(i = 1; i <= count; i++)
    {
        if(block >= badtable[i])
        {
            block += 1;
            req(blockunits, (ptu32_t)&units, block);
            movs += units;
        }
        else
        {
            break;
        }
    }

    *unit += movs;
}

#if 0
// ============================================================================
// ���ܣ�NAND������ʼ����
// ������bstart -- ��ʼ�飻
//      bcount -- ������
//      doformat -- ��ʽ����
// ���أ�
// ��ע��
// ============================================================================
__attribute__((weak)) s32 __nand_part_init(u32 bstart, u32 bcount, u32 doformat)
{
    bstart = bstart;
    bcount = bcount;
    doformat = doformat;
    return (-1);
}


// ============================================================================
// ���ܣ�embaed������ʼ����
// ������bstart -- ��ʼ�飻
//      bcount -- ������
//      doformat -- ��ʽ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
__attribute__((weak)) s32 __embed_part_init(u32 bstart, u32 bcount, u32 doformat)
{
    bstart = bstart;
    bcount = bcount;
    doformat = doformat;
    return (-1);
}

// ============================================================================
// ���ܣ���NAND FLASH�ϰ�װ������
// ������ parts -- ��������
//      �������� -- ��ʼ�飬�����������Ƿ��ʽ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 ModuleInstall_PART_ON_NAND(u8 parts, ...)
{
    u8 part;
    u32 startblock, blocks, doformat;
    va_list list;
    s32 res = 0;

    va_start(list, parts);
    for(part=0; part<parts; part++)
    {
        startblock = (u32)va_arg(list, u32);
        blocks = (u32)va_arg(list, u32);
        doformat = (u32)va_arg(list, u32);
        if(__nand_part_init(startblock, blocks, doformat))
        {

            res = -1;
            break;
        }
    }

    va_end(list);
    return (res);
}


// ============================================================================
// ���ܣ���EMBEDED FLASH�ϰ�װ������
// ������parts -- ��������
//      �������� -- ��ʼ�飬�����������Ƿ��ʽ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 ModuleInstall_PART_ON_EMBEDED(u8 parts, ...)
{
    u8 part;
    u32 startblock, blocks, doformat;
    va_list list;
    s32 res = 0;

    va_start(list, parts);
    for(part=0; part<parts; part++)
    {
        startblock = (u32)va_arg(list, u32);
        blocks = (u32)va_arg(list, u32);
        doformat = (u32)va_arg(list, u32);
        if(__embed_part_init(startblock, blocks, doformat))
        {
            res = -1;
            break;
        }
    }

    va_end(list);
    return (res);
}
#endif

