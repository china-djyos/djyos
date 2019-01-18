//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

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
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

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
struct FlashChip *pNOR;
extern struct NorDescr *nordescription;
extern u32 AT45_OP_TIMEOUT;
extern struct MutexLCB *pAT45_Lock;   //оƬ������ʱ���
//extern struct SPI_Device *s_ptAT45_Dev;
extern char *At45Name;
extern struct obj *s_ptDeviceRoot;
#if 1

#endif
// ============================================================================
// ���ܣ�SPI FLASHУ��оƬID
// ��������
// ���أ�У������(0)��У�鲻�ɹ�(-1)��
// ��ע��TODO: ����ȡID��У�����
// ============================================================================
s32 _IDCheck(void)
{
    struct SPI_DataFrame frame;
    u8 command = JEDEC_ID_READ;
    s32 res;
    u8 id[5];

    frame.RecvBuf = id;
    frame.RecvLen = 5;
    frame.RecvOff = 1;
    frame.SendBuf = &command;
    frame.SendLen = 1;

    if(FALSE ==  Lock_MutexPend(pAT45_Lock, AT45_OP_TIMEOUT))
        return (-1);

    if(FALSE == SPI_CsActive(nordescription->Port, AT45_OP_TIMEOUT))
    {
        Lock_MutexPost(pAT45_Lock);
        return (-1);
    }

    res = SPI_Transfer(nordescription->Port, &frame, TRUE, AT45_OP_TIMEOUT);
    SPI_CsInactive(nordescription->Port);

    if(res)
    {
        printf("\r\nNOR : error : cannot get the ID from SPI bus\r\n");
        Lock_MutexPost(pAT45_Lock);
        return (-1);
    }

    //
    if( (id[0] != 0x1F) ||            //Manufacturer_ID
        (id[1] != 0x27) ||            //Device_ID1
        (id[2] != 0x01) ||            //Device_ID2
        (id[3] != 0x01) ||
        (id[4] != 0x00))              //Extended_Info
    {
        Lock_MutexPost(pAT45_Lock);
        return (-1);
    }

    Lock_MutexPost(pAT45_Lock);
    return (0);    //Match SPI Flash ID successful
}

// ============================================================================
// ���ܣ���ѯ�����Ƿ����
// ������
// ���أ��ɹ���0����ʧ�ܣ�����ֵ����
// ��ע��
// ============================================================================
static s32 __AT45_Done(void)
{
    struct SPI_DataFrame frame;
    u8 command = JEDEC_STATUS_READ;
    u8 status[2];
    s32 res;

    frame.RecvBuf = status;
    frame.RecvLen = 2;
    frame.RecvOff = 1;
    frame.SendBuf = &command;
    frame.SendLen = 1;

    if(FALSE == SPI_CsActive(nordescription->Port, AT45_OP_TIMEOUT))
        return (-1);

    res = SPI_Transfer(nordescription->Port, &frame, TRUE, AT45_OP_TIMEOUT);
    SPI_CsInactive(nordescription->Port);

    if(CN_SPI_EXIT_NOERR == res)
    {
        if(!(status[0] & 0x80))
        {
            //printf("\r\nNOR : debug : still busy with an internal operation.\r\n");
            return (-1);
        }
        else
        {
            return (0);
        }
    }

    printf("\r\nNOR : error : should not be here.\r\n");
    return (-2);
}

// ============================================================================
// ���ܣ�дҳ
// ������dwPage -- ҳ�ţ�pBuf -- ���ݻ��壻dwDummy -- δʹ�ã�
// ���أ�ʵ��д�������ֽ�����
// ��ע��
// ============================================================================
static s32 __AT45_PageWrite(u32 dwPage, u8 *pBuf, u32 dwDummy)
{
    struct SPI_DataFrame frame;
    s32 ret = 0;
    s32 res;
    u8 command[] = {0, 0 ,0 ,0};

    command[0] = JEDEC_PAGE_WRITE_DIRECT_BY_BUF1;
    command[1] = (dwPage >> 0x7) & 0x3F;
    command[2] = (dwPage << 0x1) & 0xFE;
    command[3] = 0x00;

    frame.RecvBuf = NULL;
    frame.RecvLen = 0;
    frame.RecvOff = 0;
    frame.SendBuf = command;
    frame.SendLen = 4;

    if(FALSE == Lock_MutexPend(pAT45_Lock, AT45_OP_TIMEOUT))
        return (0);

    {
        u32 wait = 1000;
        while(1)
        {
            if(!__AT45_Done())
                break;

            Djy_EventDelay(1000);
            wait--;
            if(!wait)
            {
                printf("\r\nNOR : debug : device is busy before page write.");
                return (-2);
            }
        }
    }

    if(FALSE == SPI_CsActive(nordescription->Port, AT45_OP_TIMEOUT))
    {
        Lock_MutexPost(pAT45_Lock);
        return (0);
    }

    res = SPI_Transfer(nordescription->Port, &frame, TRUE, AT45_OP_TIMEOUT);
    if(CN_SPI_EXIT_NOERR == res)
    {
        frame.RecvBuf = NULL;
        frame.RecvLen = 0;
        frame.RecvOff = 0;
        frame.SendBuf = pBuf;
        frame.SendLen = nordescription->BytesPerPage;

        res = SPI_Transfer(nordescription->Port, &frame, TRUE, AT45_OP_TIMEOUT);
        if(CN_SPI_EXIT_NOERR == res)
        {
            ret = nordescription->BytesPerPage;
        }
    }

    SPI_CsInactive(nordescription->Port);
    Djy_EventDelay(4000);// ��ʱ�г�. 4ms

    {
        u32 wait = 1000;
        while(1)
        {
            if(!__AT45_Done())
                break;

            Djy_EventDelay(1000);
            wait--;
            if(!wait)
            {
                printf("\r\nNOR : debug : device is busy after page write.");
                ret = 0;
                break;
            }
        }
    }

    Lock_MutexPost(pAT45_Lock);
    return (ret);
}

// ============================================================================
// ���ܣ���ҳ
// ������dwPage -- ҳ�ţ�pBuf -- ���ݻ��壻dwDummy -- δʹ�ã�
// ���أ�ʵ�ʶ��������ֽ�����
// ��ע��
// ============================================================================
static s32 __AT45_PageRead(u32 dwPage, u8 *pBuf, u32 dwDummy)
{
    struct SPI_DataFrame frame;
    u8 command[] = {0, 0 , 0, 0, 0};
    s32 ret = 0;

    command[0] = JEDEC_ARRAY_READ_HIGH_FREQ;
    command[1] = (dwPage >> 7) & 0x3F;
    command[2] = (dwPage << 1) & 0xFE;
    command[3] = 0x0;
    command[4] = 0x0;

    frame.RecvBuf = pBuf;
    frame.RecvLen = nordescription->BytesPerPage;
    frame.RecvOff = 5;
    frame.SendBuf = command;
    frame.SendLen = 5;

    if(FALSE == Lock_MutexPend(pAT45_Lock, AT45_OP_TIMEOUT))
        return (0);

    {
        u32 wait = 1000;
        while(1)
        {
            if(!__AT45_Done())
                break;

            Djy_EventDelay(1000);
            wait--;
            if(!wait)
            {
                printf("\r\nNOR : debug : device is busy before page read.");
                return (0);
            }
        }
    }

    if(FALSE == SPI_CsActive(nordescription->Port, AT45_OP_TIMEOUT))
    {
        Lock_MutexPost(pAT45_Lock);
        return (0);
    }

    ret = SPI_Transfer(nordescription->Port, &frame, TRUE, AT45_OP_TIMEOUT);
    SPI_CsInactive(nordescription->Port);
    if(CN_SPI_EXIT_NOERR == ret)
    {
        ret = nordescription->BytesPerPage;
    }

    {
        u32 wait = 1000;
        while(1)
        {
            if(!__AT45_Done())
                break;

            Djy_EventDelay(1000);
            wait--;
            if(!wait)
            {
                printf("\r\nNOR : debug : device is busy after page read.");
                ret = 0;
                break;
            }
        }
    }

    Lock_MutexPost(pAT45_Lock);
    return (ret);
}

// ============================================================================
// ���ܣ�����һ��
// ������dwBlock -- ��ţ�
// ���أ��ɹ���0����ʧ�ܣ�����ֵ��
// ��ע��
// ============================================================================
static s32 __AT45_BlockErase(u32 dwBlock)
{
    struct SPI_DataFrame frame;
    u8 command[] = {0, 0 ,0 ,0};
    s32 res;
    u8 timeout = 0;

    command[0] = JEDEC_BLOCK_ERASE;
    command[1] = ((dwBlock >> 4) & 0x3F);
    command[2] = ((dwBlock << 4) & 0xF0);
    command[3] = 0x0;

    frame.RecvBuf = NULL;
    frame.RecvLen = 0;
    frame.RecvOff = 0;
    frame.SendBuf = command;
    frame.SendLen = 4;

    if(FALSE == Lock_MutexPend(pAT45_Lock, AT45_OP_TIMEOUT))
        return (-1);

    {
        u32 wait = 1000;
        while(1)
        {
            if(!__AT45_Done())
                break;

            Djy_EventDelay(1000);
            wait--;
            if(!wait)
            {
                printf("\r\nNOR : debug : device is busy before block erase.");
                return (-1);
            }
        }
    }

    if(FALSE == SPI_CsActive(nordescription->Port, AT45_OP_TIMEOUT))
    {
        Lock_MutexPost(pAT45_Lock);
        return (-1);
    }

    res = SPI_Transfer(nordescription->Port, &frame, TRUE, AT45_OP_TIMEOUT);
    SPI_CsInactive(nordescription->Port);

    if(!res)
    {
        // �����ʱ100ms,�ڼ��ѯ50��
        do
        {
            Djy_EventDelay(2000);// ��ʱ�г�.2ms
            res = __AT45_Done();
            if(!res)
            {
                break;
            }
        }
        while(++timeout<50);
    }

    Lock_MutexPost(pAT45_Lock);
    return (res);
}
#if 0
// ============================================================================
// ���ܣ���ѯҳ���ڵĿ�ţ��������ڿ��ʣ��ҳ��
// ������dwPage -- ҳ�ţ�������ƣ�pRemain -- ʣ��ҳ����pBlock -- ��ţ�������ƣ�
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static s32 __AT45_PageToBlock(u32 dwPage, u32 *pRemain, u32 *pBlock)
{
    if(!pRemain || !pBlock || ((u16)dwPage >= (nordescription->SectorsPerBlk * nordescription->Blks)))
    {
        return (-1);
    }

    *pBlock = (dwPage >> 3); // ��dwPage����8��תΪblock
    *pRemain = 7 - (dwPage & 7); // ��dwPage��8ȡ�࣬�ٱ�7������ΪdwPage�Ǵ���ƣ�

    return (0);
}
#endif

#if 0
// ============================================================================
// ���ܣ���ʼ��SPI FLASHģ�飬У��оƬID�Ƿ���ȷ
// ������pName -- �豸����
//      dwStart -- ��ʼ�飻
//      dwSize -- ������
//      dwSpecial -- ����EFS��2�����������������ļ�ϵͳ��1����
//      pPrivate -- ͨ����
// ���أ��ɹ���ʼ����0������ʼ��ʧ�ܣ�-1����
// ��ע���߼����Ż�
// ============================================================================
s32 __AT45_MakePartition(char *pName, u32 dwStart, u32 dwSize, u32 dwSpecial, void *pPrivate)
{
    static u8 number = 0; // �����ţ���1���߼���
    struct FlashChip *flash, *partition;
    u8 *buf;
    char *name;

    if(0 == dwSize)
    {
        return (0); // ��������
    }

    name = malloc(strlen(pName) + 16);
    if(!name)
    {
        printf("\r\n: erro : device : memory out.\r\n");
        return (-1);
    }

    sprintf(name, "%s%s", pName, " part ");
    itoa(number++, (name+strlen(name)), 10);
    if(!pNOR)
    {
        buf = malloc(sizeof(*flash)+ 512);
        if(!buf)
        {
            printf("\r\n: erro : device : memory out.\r\n");
            return (-1);
        }

        memset(buf, 0x0, (sizeof(*flash)+ 512));
        flash = (struct FlashChip*)buf;
        flash->Buf = buf + sizeof(*flash);
        flash->Lock = (void*)Lock_MutexCreate("Flash Lock");
        if(!flash->Lock)
        {
            free(buf);
            printf("\r\n: erro : device : cannot create lock.\r\n");
            return (-1);
        }

        // AT45��sector��block�󣬶���sector�Ĵ�С��һ�¡������߼��Ͼͽ�sector����page��
        // ��Ȼsector,block���
        flash->Descr.Nor.PortType = NOR_SPI;
        flash->Descr.Nor.Port = pPrivate;
        flash->Descr.Nor.BytesPerPage = 512;
        flash->Descr.Nor.PagesPerSector = 1;
        flash->Descr.Nor.SectorsPerBlk = 8;
        flash->Descr.Nor.Blks = 1024; // ȫ������������
        flash->Descr.Nor.ReservedBlks = 0;
        flash->dwPageBytes = 512;
        flash->dwTotalPages = 1024 << 3; //
        flash->dwPagesReserved = 0; //
        flash->Type = F_NOR;
        flash->Ops.RdPage = __AT45_PageRead;
        flash->Ops.WrPage = __AT45_PageWrite;
        flash->Ops.ErsBlk = __AT45_BlockErase;
        flash->Ops.PageToBlk = __AT45_PageToBlock;
        pNOR = flash;
        nordescription = &(flash->Descr.Nor);
    }

    //
    if(1 == dwSpecial)
    {
        buf = malloc(sizeof(*partition)+ 512);
        if(!buf)
        {
            printf("\r\n: erro : device : memory out.\r\n");
            return (-1);
        }

        memset(buf, 0x0, (sizeof(*partition)+ 512));
        partition = (struct FlashChip*)buf;
        partition->Buf = buf + sizeof(*partition);
        partition->Lock = pNOR->Lock;

        // AT45��sector��block�󣬶���sector�Ĵ�С��һ�¡������߼��Ͼͽ�sector����page��
        // ��Ȼsector,block���
        partition->Descr.Nor.PortType = NOR_SPI;
        partition->Descr.Nor.Port = pPrivate;
        partition->Descr.Nor.BytesPerPage = 512;
        partition->Descr.Nor.PagesPerSector = 1;
        partition->Descr.Nor.SectorsPerBlk = 8;
        partition->Descr.Nor.Blks = 0; // ��ʱδ��
        partition->Descr.Nor.ReservedBlks = 0;
        partition->dwPageBytes = 512;
        partition->dwTotalPages = (dwSize + dwStart) << 3;
        partition->dwPagesReserved = dwStart << 3;
        partition->Type = F_NOR;
        partition->Ops.RdPage = __AT45_PageRead;
        partition->Ops.WrPage = __AT45_PageWrite;
        partition->Ops.ErsBlk = __AT45_BlockErase;
        partition->Ops.PageToBlk = __AT45_PageToBlock;

        if(!dev_Create(name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)partition))
        {
            free(buf);
            printf("\r\n: erro : device : register AT45db321 failed.");
            return (-1);
        }
    }

#if 0
    // ��ǰ�߼�����EFS
    if((!pDevEFS) && (2 == dwSpecial))
    {
        buf = malloc(sizeof(*partition) + 512 + sizeof(*pDevEFS));
        if(!buf)
        {
            printf("\r\nNOR : error : memory out.\r\n");
            return (-1);
        }

        memset(buf, 0x0, (sizeof(*partition) + 512 + sizeof(*pDevEFS)));
        partition = (struct FlashChip*)buf;
        partition->Buf = buf + sizeof(*partition);
        pDevEFS = (tagdevEFS *)(buf + sizeof(*partition) + 512);
        partition->Lock = pNOR->Lock;

        // AT45��sector��block�󣬶���sector�Ĵ�С��һ�¡������߼��Ͼͽ�sector����page��
        // ��Ȼsector,block���
        partition->Descr.Nor.PortType = NOR_SPI;
        partition->Descr.Nor.Port = pPrivate;
        partition->Descr.Nor.BytesPerPage = 512;
        partition->Descr.Nor.PagesPerSector = 1;
        partition->Descr.Nor.SectorsPerBlk = 8;
        partition->Descr.Nor.Blks = dwSize << 3;
        partition->Descr.Nor.ReservedBlks = 0;
        partition->dwPageBytes = 512;
        partition->dwTotalPages = (dwSize + dwStart) << 3;
        partition->dwPagesReserved = dwStart << 3;
        partition->Type = F_NOR;
        partition->Ops.RdPage = __AT45_PageRead;
        partition->Ops.WrPage = __AT45_PageWrite;
        partition->Ops.ErsBlk = __AT45_BlockErase;
        partition->Ops.PageToBlk = __AT45_PageToBlock;

        if(!DevAdd(name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)partition))
        {
            free(buf);
            printf("\r\nNOR : error : register device failed.\r\n");
            return (-1);
        }

        pDevEFS->EFS_IF_CheckBlockReady = __EFS_IF_CheckBlockReady;
        pDevEFS->EFS_IF_Erase = __EFS_IF_Erase;
        pDevEFS->EFS_IF_ReadData = __EFS_IF_ReadData;
        pDevEFS->EFS_IF_WriteData = __EFS_IF_WriteData;
        pDevEFS->dwBlockBytes = 512 << 3;
        pDevEFS->dwStart = dwStart;
        pDevEFS->dwTotal = dwSize;
    }
#else
    if(2 == dwSpecial)
    {
        struct umedia *um;
        struct uopt opt;

        um = malloc(sizeof(struct umedia)+512);
        if(!um)
            return (-1);

        opt.hecc = 0;
        opt.main = 1;
        opt.necc = 1;
        opt.secc = 0;
        opt.spare = 0;
        um->asz = dwSize << 12;
        um->esz = 12; // 4KB
        um->usz = 9; // 512B;
        um->merase = __at45_erase;
        um->mread = __at45_read;
        um->mreq = __at45_req;
        um->mwrite = __at45_write;
        um->opt = opt; // ���������߼�
        um->type = nor;
        um->ubuf = (u8*)um + sizeof(struct umedia);
        um->ustart = dwStart * 8; // ��ʼunit��

        return (um_add((const char*)name, um));
    }
#endif

    return (0);
}
#endif



#if 1 // �½ӿ�
#include <device/include/unit_media.h>
// ============================================================================
// ���ܣ���AT45
// ������unit -- ҳ�ţ�data --��������ݵĻ��棻
// ���أ��ɹ� -- ��0����ʧ�� -- ��-1��
// ��ע��
// ============================================================================
s32 __at45_read(s64 unit, void *data, struct uopt opt)
{
    s32 res;
    res = __AT45_PageRead((u32)unit, (u8*)data, 0);
    if(res != nordescription->BytesPerPage)
    {
        return -1;
    }
    return 0;
}

// ============================================================================
// ���ܣ�дAT45
// ������unit -- ҳ�ţ�data -- Ҫд�����ݣ�
// ���أ��ɹ� -- ��0����ʧ�� -- ��-1��
// ��ע��
// ============================================================================
s32 __at45_write(s64 unit, void *data, struct uopt opt)
{
    s32 res;
    res = __AT45_PageWrite((u32)unit, (u8*)data, 0);
    if(res != nordescription->BytesPerPage)
    {
        return -1;
    }
    return 0;
}

// ============================================================================
// ���ܣ���AT45��ҳ���
// ������unit -- ҳ���߿飻
//      sz -- ҳ���߿飻
// ���أ��ɹ� -- ��0����ʧ�� -- ��-1��
// ��ע��
// ============================================================================
s32 __at45_erase(s64 unit, struct uesz sz)
{
    u32 block;

    if(sz.unit)
        block = unit / 8; // һ��������8��page
    else if (sz.block)
        block = (u32)unit;

    return (__AT45_BlockErase(block));
}

// ============================================================================
// ���ܣ�at45����
// ������cmd -- ���
//      args -- �ɱ�Σ����������
// ���أ�
// ��ע��
// ============================================================================
s32 __at45_req(enum ucmd cmd, ptu32_t args, ...)
{
    s32 res = 0;

    switch(cmd)
    {
        case whichblock:
        {
            va_list list;
            u32 *block;
            s64 *unit;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);
            *block = *unit / nordescription->SectorsPerBlk;
            break;
        }

        case totalblocks:
        {
            // args = &blocks
            *((u32*)args) =  nordescription->Blks;
            break;
        }

        case blockunits:
        {
            // args = &units
            *((u32*)args) = nordescription->SectorsPerBlk;
            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = nordescription->BytesPerPage;
            break;
        }

        case format:
        {
            va_list list;
            s32 start, end;
            struct uesz *sz;

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            sz = (struct uesz*)va_arg(list, u32);
            va_end(list);

            if(!sz->block)
                return (-1);

            if(-1==end)
                end = nordescription->Blks;

            do
            {
                if(__at45_erase((s64)--end, *sz))
                {
                    res = -1;
                    break;
                }
            }
            while(end!=start);

            break;
        }

        case remain:
        {
            va_list list;
            u32 *left;
            s64 *unit;

            left = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);
            *left = (nordescription->SectorsPerBlk - 1)- (*unit % nordescription->SectorsPerBlk);

            break;
        }

        case checkbad: break; // ��黵��
        default: res = -1; break;
    }

    return (res);
}
#endif

// ============================================================================
// ���ܣ���ʼ��SPI FLASHģ�飬�����ļ�ϵͳ
// ������   fs -- ��ý����Ҫ��װ�ļ�ϵͳmount�����֣�
//      MountPart -- �ļ�ϵͳ��װ�ڵڼ�������
//      dwStart -- ��ʼ�飻
//      dwSize -- ������
//      dwSpecial -- ����������1����������������0����
// ���أ��ɹ���ʼ����0������ʼ��ʧ�ܣ�-1����
// ��ע�������߼������ļ�ϵͳ��ֱ�ӷ����߼��������÷�����
// ============================================================================
s32 __AT45_FsInstallInit(const char *fs, u32 dwStart, u32 dwEnd, u32 dwSpecial)
{
    char *FullPath,*notfind;
    struct obj *targetobj;
    struct FsCore *super;
    s32 res;
    u32 BlockNum;

    targetobj = obj_matchpath(fs, &notfind);
    if(notfind)
    {
        error_printf("nand"," not found need to install file system.");
        return -1;
    }
    super = (struct FsCore *)obj_GetPrivate(targetobj);

    if(dwSpecial)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        __at45_req(format, dwStart , dwEnd, &sz);
    }

    if((s32)dwEnd == -1)
    {
        dwEnd = nordescription->Blks;
        BlockNum = dwEnd - dwStart;
    }
    else
    {
        BlockNum = dwEnd - dwStart;
    }
    super->AreaSize = BlockNum * (nordescription->BytesPerPage * nordescription->SectorsPerBlk);
    super->MediaStart = dwStart * 8; // ��ʼunit��

    res = strlen(At45Name)+strlen(s_ptDeviceRoot->name);
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,At45Name);		//��ȡ�豸��ȫ·��
    FsBeMedia(FullPath,fs);		//�����豸�����ļ�ϵͳ
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, dwStart, dwEnd);
    return (0);
}

//// =============================================================================
//// ���ܣ���ʼ��AT45��һ�����ڰ�װ�ļ�ϵͳ
//// ������bstart -- ��ʼ�飻
////     bcount -- �÷���һ���ж��ٿ�
////     dwSpecial -- ����������1����������������0����
//// ���أ��ɹ���0����ʧ�ܣ�-1����
//// ��ע�������߼������ļ�ϵͳ��ֱ�ӷ����߼��������÷�����
//s32 __at45_part_init(const char *fs,s32 MountPart,u32 bstart, u32 bcount, u32 doformat)
//{
//    static char *name = "AT45DB321E";    //Ҫ��ModuleInstall_at45db321�е�name����һ��
//    if(s_ptAT45_Dev == NULL)
//    {
//        printf("\r\n: error : device : AT45 not add the SPI.");
//        return (-1);
//    }
//    if(__AT45_PartitionInit(name,bstart,bcount,doformat,s_ptAT45_Dev) == 0)
//    {
//        return (0);
//    }
//    return (-1);
//}

#if 0
/******************************************************************************
                            �����ļ�ϵͳ�����ӿ�
******************************************************************************/

// ============================================================================
// ���ܣ� дһҳ������
// ������
// ���أ� ʧ�ܣ�0�����ɹ���д�����ݴ�С��
// ��ע��
// ============================================================================
static s32 __WritePage(u32 dwPage, u8 *pData, u32 dwDummy)
{
    return (__AT45_PageWrite(dwPage, pData, 0));
}

// ============================================================================
// ���ܣ� дһҳ������
// ������
// ���أ� ʧ�ܣ�0�����ɹ���д�����ݴ�С��
// ��ע��
// ============================================================================
static s32 __ReadPage(u32 dwPage, u8 *pData, u32 dwDummy)
{
    return (__AT45_PageRead(dwPage, pData, 0));
}

// ============================================================================
// ���ܣ� дС��һҳ������
// ������
// ���أ� ʧ�ܣ�-2�����ɹ���д�����ݴ�С��
// ��ע������528�ֽڲ���
// ============================================================================
static s32 __WriteFragment(u32 dwPage, u32 dwOffset, const u8 *pBuf, u32 dwSize)
{
    struct SPI_DataFrame frame;
    s32 ret = -2;
    s32 res;
    u8 command[] = {0, 0, 0, 0};
    u32 address = ((dwPage << 9) & 0x003FFE00) | ((dwOffset) & 0x1FF);

    command[0] = JEDEC_PAGE_WRITE_DIRECT_BY_BUF1;
    command[1] = (u8)(address >> 16);
    command[2] = (u8)(address >> 8);
    command[3] = (u8)(address);

    frame.RecvBuf = NULL;
    frame.RecvLen = 0;
    frame.RecvOff = 0;
    frame.SendBuf = command;
    frame.SendLen = 4;

    if(FALSE == Lock_MutexPend(pNOR->Lock, AT45_OP_TIMEOUT))
        return (-2);

    {
        u32 wait = 1000;
        while(1)
        {
            if(!__AT45_Done())
                break;

            Djy_EventDelay(1000);
            wait--;
            if(!wait)
            {
                printf("\r\nNOR : debug : device is busy before fragment write.\r\n");
                return (-2);
            }
        }
    }

    if(FALSE == SPI_CsActive(pNOR->Descr.Nor.Port, AT45_OP_TIMEOUT))
    {
        Lock_MutexPost(pNOR->Lock);
        return (-2);
    }

    res = SPI_Transfer(pNOR->Descr.Nor.Port, &frame, TRUE, AT45_OP_TIMEOUT);
    if(CN_SPI_EXIT_NOERR == res)
    {
        frame.RecvBuf = NULL;
        frame.RecvLen = 0;
        frame.RecvOff = 0;
        frame.SendBuf = (u8*)pBuf;
        frame.SendLen = dwSize;

        res = SPI_Transfer(pNOR->Descr.Nor.Port, &frame, TRUE, AT45_OP_TIMEOUT);
        if(CN_SPI_EXIT_NOERR == res)
        {
            ret = dwSize;
        }
    }

    SPI_CsInactive(pNOR->Descr.Nor.Port);
    Djy_EventDelay(4000);// ��ʱ�г�. 4ms

    if(CN_SPI_EXIT_NOERR == res)
    {
        u32 wait = 1000;
        while(1)
        {
            if(!__AT45_Done())
                break;

            Djy_EventDelay(1000);
            wait--;
            if(!wait)
            {
                printf("\r\nNOR : debug : device is busy after fragment write.\r\n");
                ret = -2; // ʧ��
            }
        }
    }

    Lock_MutexPost(pNOR->Lock);
    return (ret);
}

// ============================================================================
// ���ܣ� ��С��һҳ������
// ������
// ���أ� ʧ�ܣ�-2�����ɹ������������ֽ�������
// ��ע��
// ============================================================================
static s32 __ReadFragment(u32 dwPage, u32 dwOffset, u8 *pBuf, u32 dwSize)
{
    struct SPI_DataFrame frame;
    u8 command[] = {0, 0 , 0, 0, 0};
    s32 ret = -2;
    u32 address = ((dwPage << 9) & 0x003FFE00) | ((dwOffset) & 0x1FF);

    command[0] = JEDEC_ARRAY_READ_HIGH_FREQ;
    command[1] = (u8)(address >> 16);
    command[2] = (u8)(address >> 8);
    command[3] = (u8)(address);
    command[4] = 0x0;

    frame.RecvBuf = pBuf;
    frame.RecvLen = dwSize;
    frame.RecvOff = 5;
    frame.SendBuf = command;
    frame.SendLen = 5;

    if(FALSE == Lock_MutexPend(pNOR->Lock, AT45_OP_TIMEOUT))
        return (-2);

    {
        u32 wait = 1000;
        while(1)
        {
            if(!__AT45_Done())
                break;

            Djy_EventDelay(1000);
            wait--;
            if(!wait)
            {
                printf("\r\nNOR : debug : device is busy before fragment read.\r\n");
                return (-2);
            }
        }
    }

    if(FALSE == SPI_CsActive(pNOR->Descr.Nor.Port, AT45_OP_TIMEOUT))
    {
        Lock_MutexPost(pNOR->Lock);
        return (-2);
    }

    ret = SPI_Transfer(pNOR->Descr.Nor.Port, &frame, TRUE, AT45_OP_TIMEOUT);
    SPI_CsInactive(pNOR->Descr.Nor.Port);
    if(CN_SPI_EXIT_NOERR == ret)
    {
        ret = dwSize;
    }

    {
        u32 wait = 1000;
        while(1)
        {
            if(!__AT45_Done())
                break;

            Djy_EventDelay(1000);
            wait--;
            if(!wait)
            {
                printf("\r\nNOR : debug : device is busy after fragment read.");
                ret = -2;
                break;
            }
        }
    }

    Lock_MutexPost(pNOR->Lock);
    return (ret);
}

// ============================================================================
// ���ܣ� д����
// ������ dwBlock -- ���;
//       dwOffset -- ����ƫ��;
//       pBuf -- д����;
//       dwSize -- д���ݴ�С
//       bFlags -- �޹���(0x0); ECC���ܿ��������ݴ�СΪ����256��0x1����
// ���أ�����0�����ɹ���д�����ݴ�С����
// ��ע��
// ============================================================================
u32 __EFS_IF_WriteData(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags)
{
    u32 page = (dwBlock << 3) + (dwOffset >> 9); // ҳ��
    u32 offset = dwOffset & 0x1FF; // ҳ�ڵ�ƫ��
    u32 wrLen = dwSize;
    s32 res;
    u8 *data = pBuf;

    if(!data)
        return (0);

#if 0 // NOR�豸����ECC��ͬʱΪ�˼��ݾɵ�ʹ���߼������߼���512��ҳ�߼����еĶ�д��
    // ECC������ԴΪ256�ֽڣ��������ݺ��,3���ֽ�
    if(1 == bFlags)
    {
        hamming_compute_256x(data, 256, data+256);
        wrLen = dwSize + 3;
        offset += 3 * (offset >> 8); // ������߼���ÿ256������һ��ECC
    }
#endif

    for(;;)
    {
        if((!offset) && (wrLen >= pNOR->dwPageBytes))
            res = __WritePage(page, data, NO_ECC);
        else if((wrLen >= pNOR->dwPageBytes) || // д���ݴ������һҳ,�����ݿ�ҳ
                (((wrLen+offset) > pNOR->dwPageBytes) && (!bFlags))) // д����С��һҳ,�����ݿ�ҳ(ֻ�п��Ƿ�ECC�����)
            res = __WriteFragment(page, offset, (const u8*)data, (pNOR->dwPageBytes-offset));
        else
            res = __WriteFragment(page, offset, (const u8*)data, wrLen);

        if(res <= 0)
            break; // ���������д��

        wrLen -= res;
        if(wrLen <= 0)
            break; // д��

        data += res;
        page++;
        if(offset)
            offset = 0;
    }

    if(res >= 0)
        return (dwSize);
    else
        return (0);
}

// ============================================================================
// ���ܣ� ������
// ������ dwBlock -- ���;
//       Offset -- ����ƫ��;
//       pBuf -- ������;
//       dwSize -- �����ݴ�С
//       bFlags -- 0x0 -- �޹���; 0x1 - ECC���ܿ���(���ݴ�СΪ����256)
// ���أ�ʧ�ܣ�0�����ɹ���������ֽ�������
// ��ע��
// ============================================================================
u32 __EFS_IF_ReadData(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags)
{
    s32 res;
    u32 page = (dwBlock << 3) + (dwOffset >> 9); // ҳ��
    u32 offset = dwOffset & 0x1FF; // ҳ�ڵ�ƫ��
    u32 rdLen = dwSize;
    u8 *data = pBuf;

    if(!data)
        return (0);
#if 0 // NOR�豸����ECC��ͬʱΪ�˼��ݾɵ�ʹ���߼������߼���512��ҳ�߼����еĶ�д��
    if(1 == bFlags)
    {
        rdLen += 3;
        offset += 3 * (offset >> 8); // ������߼���ÿ256������һ��ECC
    }
#endif

    for(;;)
    {
        if((!offset) && (rdLen >= pNOR->dwPageBytes))
            res = __ReadPage(page, data, NO_ECC);
        else if((rdLen >= pNOR->dwPageBytes) || // д���ݴ������һҳ,�����ݿ�ҳ
                (((rdLen+offset) > pNOR->dwPageBytes) && (!bFlags))) // ������С��һҳ,�����ݿ�ҳ(ֻ�з�ECC�������Ҫ����)
            res = __ReadFragment(page, offset, data, (pNOR->dwPageBytes - offset));
        else
            res = __ReadFragment(page, offset, data, rdLen);

        if(res <= 0)
            break; // ���������д��

        rdLen -= res;
        if(rdLen <= 0)
            break;

        data += res;
        page++;
        if(offset)
            offset = 0;
    }


    if(res <= 0)
        return (0);

#if 0 // NOR�豸����ECC��ͬʱΪ�˼��ݾɵ�ʹ���߼������߼���512��ҳ�߼����еĶ�д��
    if((1 == bFlags) && (256 == dwSize))
    {
        u8 *ecc;
        u32 eccRes;

        ecc = pBuf + 256;
        eccRes = hamming_verify_256x(pBuf, 256, ecc);
        if (eccRes && (eccRes != HAMMING_ERROR_SINGLE_BIT))
        {
            printf("EFS : debug : cannot be fixed data by ECC\r\n");
            return (0);
        }
    }
#endif

    return (dwSize);
}

// ============================================================================
// ���ܣ� ����
// ������ BlkNo -- ���;
// ���أ��ɹ���TRUE����ʧ�ܣ�FALSE����
// ��ע��
// ============================================================================
bool_t __EFS_IF_Erase(u32 dwBlock)
{
    if(!__AT45_BlockErase(dwBlock))
        return (TRUE);

    return (FALSE);
}
#if 0
// ============================================================================
// ���ܣ� ����ļ��Ƿ��д
// ������dwBlock -- ��ţ�dwOffset -- ƫ�ã�
// ���أ���д��0��������д��-1����
// ��ע��
// ============================================================================
s32 __EFS_IF_IsFragmentWritten(u32 dwBlock, u32 dwOffset, u8 *pData)
{
    u8 *buf = pData;
    u16 i;
    u32 offset = dwOffset & 0x1FF; // ҳ��ƫ��
#if 0
    u8 ecc = 3 * (offset >> 8); // ECCƫ�ã�������߼���ÿ256������һ��ECC
#else
    u8 ecc = 0, len = 256;
#endif

    if(0 == __EFS_IF_ReadData(dwBlock, (dwOffset+ecc), buf, len, 0))
        return (-1);

    for(i = 0; i < len; i++)
    {
        if(0xFF != buf[i])
            return (-1);
    }

    return (0);
}
#endif
// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t __EFS_IF_CheckBlockReady(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize)
{
    static u8 data[259];
    u8 *buf = pBuf;
    u32 i, rLen, block = dwBlock, offset = dwOffset, left = dwSize;
    u32 blockBytes = pNOR->Descr.Nor.SectorsPerBlk * pNOR->Descr.Nor.BytesPerPage *
                     pNOR->Descr.Nor.PagesPerSector;

    block += offset / blockBytes;
    offset = offset % blockBytes;

    do{
        rLen = left;
        if(rLen > 256)
            rLen = 256;

        if(0 == __EFS_IF_ReadData(block, (offset), data, rLen, 0))
            return (FALSE);

        for(i=0; i < rLen; i++)
        {
            if((0xFF != data[i]) && (data[i] != buf[i]))
                return (FALSE);
        }

        left -= rLen;
        offset += rLen;
    }while(left);

    return (TRUE);
}

// ============================================================================
// ���ܣ����ز����߼�
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 TEST_Module_AT45(void)
{
    u16 *buf, *check;
    s32 res;
    u32 i, j;
    u32 block, page;

    buf = (u16*)malloc(pNOR->dwPageBytes);
    if(!buf)
    {
        printf("\r\nTEST : error : memory out.\r\n");
        while(1);
    }

    check = (u16*)malloc(pNOR->dwPageBytes);
    if(!check)
    {
        printf("\r\nTEST : error : memory out.\r\n");
        while(1);
    }

    memset((u8*)check, 0x0, pNOR->dwPageBytes);

    for(i = 0; i < (pNOR->dwPageBytes >> 1); i++)
    {
        buf[i] = i;
    }

    _IDCheck();

    block = 512;
    __AT45_BlockErase(block);

    page = block * 8;
    for(i = 0; i < 8; i++)
    {
        res = __AT45_PageRead(page+i, (u8*)check, 0);
        if(res != pNOR->dwPageBytes)
        {
            printf("\r\nTEST : error : read failed.\r\n");
        }

        for(j = 0; j < (pNOR->dwPageBytes >> 1); j++)
        {
            if(0xFFFF != check[j])
            {
                printf("\r\nTEST : error : data was not erased, <%d> <%d> <%04x>.\r\n", i, j, check[j]);
            }
        }
    }

    for(i = 0; i < 8; i++)
    {
        res = __AT45_PageWrite(page+i, (u8*)buf, 0);
        if(res != pNOR->dwPageBytes)
        {
            printf("\r\nTEST : error : write failed.\r\n");
        }

        res = __AT45_PageRead(page+i, (u8*)check, 0);
        if(res != pNOR->dwPageBytes)
        {
            printf("\r\nTEST : error : read failed.\r\n");
        }

        for(j = 0; j < (pNOR->dwPageBytes >> 1); j++)
        {
            if(check[j] != buf[j])
            {
                printf("\r\nTEST : error : data was not erased, <%d> <%d> <%04x> <%04x>.\r\n", i, j, check[j], buf[j]);
            }
        }

    }

    {
        u8 *blockBuf;
        u32 size;

        blockBuf = malloc(pDevEFS->dwBlockBytes);
        if(!blockBuf)
            while(1);

        memset(blockBuf, 0x00, pDevEFS->dwBlockBytes);
        __EFS_IF_Erase(pDevEFS->dwStart);

        size = pDevEFS->dwBlockBytes;

        if(!__EFS_IF_ReadData(pDevEFS->dwStart, 0, blockBuf, size, 0))
            while(1);

        for(i=0; i<size; i++)
        {
            if(0xFF != blockBuf[i])
                while(1);
        }

        memset(blockBuf, 0x00, pDevEFS->dwBlockBytes);
        size = 100;

        if(!__EFS_IF_ReadData(pDevEFS->dwStart, 0, blockBuf, size, 0))
            while(1);

        for(i=0; i<size; i++)
        {
            if(0xFF != blockBuf[i])
                while(1);
        }

        memset(blockBuf, 0x00, pDevEFS->dwBlockBytes);
        size = 900;

        if(!__EFS_IF_ReadData(pDevEFS->dwStart, 0, blockBuf, size, 0))
            while(1);

        for(i=0; i<size; i++)
        {
            if(0xFF != blockBuf[i])
                while(1);
        }

        memset(blockBuf, 0x00, pDevEFS->dwBlockBytes);
        size = 900;

        if(!__EFS_IF_ReadData(pDevEFS->dwStart, 200, blockBuf, size, 0))
            while(1);

        for(i=0; i<size; i++)
        {
            if(0xFF != blockBuf[i])
                while(1);
        }

        memset(blockBuf, 0x00, pDevEFS->dwBlockBytes);
        size = pDevEFS->dwBlockBytes;

        for(i=0; i<size; i++)
        {
            blockBuf[i] = i;
        }

        if(!__EFS_IF_WriteData(pDevEFS->dwStart, 0, blockBuf, size, 0))
            while(1);

        if(!__EFS_IF_ReadData(pDevEFS->dwStart, 0, blockBuf, size, 0))
            while(1);

        for(i=0; i<size; i++)
        {
            if(((u8)i) != blockBuf[i])
                while(1);
        }

        __EFS_IF_Erase(pDevEFS->dwStart);
        memset(blockBuf, 0x00, pDevEFS->dwBlockBytes);
        size = 100;

        for(i=0; i<size; i++)
        {
            blockBuf[i] = i;
        }

        if(!__EFS_IF_WriteData(pDevEFS->dwStart, 0, blockBuf, size, 0))
            while(1);

        if(!__EFS_IF_ReadData(pDevEFS->dwStart, 0, blockBuf, size, 0))
            while(1);

        for(i=0; i<size; i++)
        {
            if(((u8)i) != blockBuf[i])
                while(1);
        }

        {
            pDevEFS->dwStart = 0x10A;
            __EFS_IF_Erase(pDevEFS->dwStart);
            memset(blockBuf, 0x00, pDevEFS->dwBlockBytes);
            size = 130;

            for(i=0; i<size; i++)
            {
                blockBuf[i] = i;
            }

            if(!__EFS_IF_WriteData(pDevEFS->dwStart, 130, blockBuf, size, 0))
                while(1);

            memset(blockBuf, 0, 266);
            if(!__EFS_IF_ReadData(pDevEFS->dwStart, 130, blockBuf, size, 0))
                while(1);

            memset(blockBuf, 0, 266);
            if(!__EFS_IF_ReadData(pDevEFS->dwStart, 0, blockBuf, size+130, 0))
                while(1);



            for(i=0; i<size; i++)
            {
                if(((u8)i) != blockBuf[i])
                    while(1);
            }
        }

        __EFS_IF_Erase(pDevEFS->dwStart);
        memset(blockBuf, 0x00, pDevEFS->dwBlockBytes);
        size = 900;

        for(i=0; i<size; i++)
        {
            blockBuf[i] = i;
        }

        if(!__EFS_IF_WriteData(pDevEFS->dwStart, 0, blockBuf, size, 0))
            while(1);

        if(!__EFS_IF_ReadData(pDevEFS->dwStart, 0, blockBuf, size, 0))
            while(1);

        for(i=0; i<size; i++)
        {
            if(((u8)i) != blockBuf[i])
                while(1);
        }


        __EFS_IF_Erase(pDevEFS->dwStart);
        memset(blockBuf, 0x00, pDevEFS->dwBlockBytes);
        size = 900;

        for(i=0; i<size; i++)
        {
            blockBuf[i] = i;
        }

        if(!__EFS_IF_WriteData(pDevEFS->dwStart, 300, blockBuf, size, 0))
            while(1);

        if(!__EFS_IF_ReadData(pDevEFS->dwStart, 300, blockBuf, size, 0))
            while(1);

        for(i=0; i<size; i++)
        {
            if(((u8)i) != blockBuf[i])
                while(1);
        }

        __EFS_IF_Erase(pDevEFS->dwStart);
        memset(blockBuf, 0x00, pDevEFS->dwBlockBytes);
        size = 256;

        for(i=0; i<size; i++)
        {
            blockBuf[i] = i;
        }

        if(!__EFS_IF_WriteData(pDevEFS->dwStart, 0, blockBuf, size, 1))
            while(1);

        if(!__EFS_IF_ReadData(pDevEFS->dwStart, 0, blockBuf, size, 1))
            while(1);

        for(i=0; i<size; i++)
        {
            if(((u8)i) != blockBuf[i])
                while(1);
        }

        if(!__EFS_IF_WriteData(pDevEFS->dwStart, 256, blockBuf, size, 1))
            while(1);

        if(!__EFS_IF_ReadData(pDevEFS->dwStart, 256, blockBuf, size, 1))
            while(1);

        for(i=0; i<size; i++)
        {
            u8 temp= blockBuf[i];
            if(((u8)i) != temp)
                while(1);
        }
    }


    //while(1);
    return (0);
}
#endif


