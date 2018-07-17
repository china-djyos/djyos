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

#include <stddef.h>
#include <endian.h>
#include "flash.h"
#include "dbug.h"

// ============================================================================
// ���ܣ�����NAND FLASH��ONFI����ҳ��
// ������pRaw -- NandParamterPage
//       pONFI -- Nand�����ṹ��
//       bLittle -- NandParamterPage�ĸ�ʽ,"1"��ʾ32λС�ˡ�
// ���أ��ɹ�(0); ʧ��(-1);
// ��ע������ҳ�ľ������ݽṹ,Ҫ��ע����NAND FLASH�Ĳ����ֲ�
// ============================================================================
s32 DecodeONFI(const char *pRaw, struct NandDescr *pONFI, u8 bLittle)
{
    u8 i;
    u32 offset;
    u8 temp[20] = {0};
    char signature[4] = {'O', 'N', 'F', 'I'}; // ����ʶ���Ƿ���ONFI����
 
    // ONFI���
    offset = 0;
    if(bLittle)
    {
        *(u32 *)signature = swapl(*(u32 *)signature);
    }
    for (i = 0; i < 4; i++)
    {
        if (signature[i] != (char)pRaw[offset+i])
            return (-1); // ������ݲ���Ҫ��
    }
 
    // ONFI�汾��
    offset = 4;
    for (i = 0; i < 4; i++)
    {
        temp[i] = (u8)pRaw[offset+i];
    }
    
    if(bLittle)
        pONFI->Revision = (u16)swapl(*(u32 *)temp);
    else
        pONFI->Revision = (u16)(*(u32 *)temp);
 
    // Device Manufacturer
    offset = 32;
    for (i = 0; i < 12; i++)
    {
        temp[i] = (u8)pRaw[offset+i];
    }
    if(bLittle)
    {
        for (i = 0; i < 3; i++)
        {
            ((u32 *)temp)[i] = swapl(((u32 *)temp)[i]);
        }
    }
    for (i = 0; i < 12; i++)
    {
        pONFI->Manufacturer[i] = (char)temp[i];
    }
    pONFI->Manufacturer[i] = '\0';
 
    // Device Model
    offset = 44;
    for (i = 0; i < 20; i++)
    {
        temp[i] = (u8)pRaw[offset+i];
    }
    if(bLittle)
    {
        for (i = 0; i < 5; i++)
        {
            ((u32 *)temp)[i] = swapl(((u32 *)temp)[i]);
        }
    }
    for (i = 0; i < 20; i++)
    {
        pONFI->DeviceModel[i] = (char)temp[i];
    }
    pONFI->DeviceModel[i] = '\0';
 
    // Number of data bytes per page
    offset = 80;
    for (i = 0; i < 4; i++)
    {
        temp[i] = (u8)pRaw[offset+i];
    }
    if(bLittle)
    {
        pONFI->BytesPerPage = swapl(*(u32 *)temp);
    }
    else
    {
        pONFI->BytesPerPage = *(u32 *)temp;
    }
 
    // Number of spare bytes per page
    offset = 84;
    for (i = 0; i < 4; i++)
    {
        temp[i] = (u8)pRaw[offset+i];
    }
    if(bLittle)
    {
        pONFI->OOB_Size = swapl(*(u32 *)temp);
    }
    else
    {
        pONFI->OOB_Size = *(u32 *)temp;
    }
 
    // Number of pages per block
    offset = 92;
    for (i = 0; i < 4; i++)
    {
        temp[i] = (u8)pRaw[offset+i];
    }
    if(bLittle)
    {
        pONFI->PagesPerBlk = swapl(*(u32 *)temp);
    }
    else
    {
        pONFI->PagesPerBlk = *(u32 *)temp;
    }
 
    // Number of blocks per LUN
    offset = 96;
    for (i = 0; i < 4; i++)
    {
        temp[i] = (u8)pRaw[offset+i];
    }
    if(bLittle)
    {
        pONFI->BlksPerLUN = swapl(*(u32 *)temp);
    }
    else
    {
        pONFI->BlksPerLUN = *(u32 *)temp;
    }
 
    // Number of LUNs
    offset = 100;
    for (i = 0; i < 4; i++)
    {
        temp[i] = (u8)pRaw[offset+i];
    }
    if(bLittle)
    {
        pONFI->LUNs = (u8)swapl(*(u32 *)temp);
    }
    else
    {
        pONFI->LUNs = (u8)(*(u32 *)temp);
    }
 
 #if 0
    // Guaranteed valid block at the beginning of target
    for (i = 0; i < 4; i++)
    {
        temp[i] = (u8)pRaw[104+i];
    }
    *(u32 *)temp = swapl(*(u32 *)temp);
    pONFI->SafeBlkNo = temp[3];
 #endif
 
    debug_printf("flash","\r\n");
    debug_printf("flash","FLASH's ONFI :\r\n");
    debug_printf("flash","    device's manufacturer is \"%s\"\r\n", pONFI->Manufacturer);
    debug_printf("flash","    device's model is %s\r\n", pONFI->DeviceModel);
    debug_printf("flash","    device's details, %d LUNs, %d blocks/LUN, %d pages/block, %d bytes/page, %d bytes/OOB.",
           pONFI->LUNs, pONFI->BlksPerLUN, pONFI->PagesPerBlk, pONFI->BytesPerPage, pONFI->OOB_Size);
    debug_printf("flash","\r\n\r\n");

 #if 0
//    TraceDrv(FLASH_TRACE_DEBUG, "Guaranteed block:0x%x\r\n", pONFI->SafeBlkNo );
    debug_printf("flash","Guaranteed block:0x%x\r\n", pONFI->SafeBlkNo);
 #endif
 
    return (0);
}

// ============================================================================
// ���ܣ� ��������оƬ(���������)
// ������
// ���أ� >=0 -- ���ڵĻ�������-1 -- ʧ��;
// ��ע��
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
#if 0
// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
u32 __FLASH_Write(tagOFile *devFile, u8 *pBuf, u32 dwLen, u32 dwTimeout)
{
    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
u32 __FLASH_Read(tagOFile *devFile, u8 *pBuf, u32 dwLen, u32 dwTimeout)
{
    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 FLASH_Register(char *name)
{

}
#endif
