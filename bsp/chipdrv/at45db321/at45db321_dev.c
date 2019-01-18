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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
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
extern struct MutexLCB *pAT45_Lock;   //芯片互斥访问保护
//extern struct SPI_Device *s_ptAT45_Dev;
extern char *At45Name;
extern struct obj *s_ptDeviceRoot;
#if 1

#endif
// ============================================================================
// 功能：SPI FLASH校验芯片ID
// 参数：无
// 返回：校验正常(0)；校验不成功(-1)；
// 备注：TODO: 将获取ID与校验分离
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
// 功能：查询操作是否完成
// 参数：
// 返回：成功（0）；失败（其他值）；
// 备注：
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
// 功能：写页
// 参数：dwPage -- 页号；pBuf -- 数据缓冲；dwDummy -- 未使用；
// 返回：实际写出数据字节数；
// 备注：
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
    Djy_EventDelay(4000);// 延时切出. 4ms

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
// 功能：读页
// 参数：dwPage -- 页号；pBuf -- 数据缓冲；dwDummy -- 未使用；
// 返回：实际读入数据字节数；
// 备注：
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
// 功能：擦除一块
// 参数：dwBlock -- 块号；
// 返回：成功（0）；失败（其他值）
// 备注：
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
        // 最大延时100ms,期间查询50次
        do
        {
            Djy_EventDelay(2000);// 延时切出.2ms
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
// 功能：查询页所在的块号，及其所在块的剩余页。
// 参数：dwPage -- 页号；（从零计）pRemain -- 剩余页数；pBlock -- 块号；（从零计）
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __AT45_PageToBlock(u32 dwPage, u32 *pRemain, u32 *pBlock)
{
    if(!pRemain || !pBlock || ((u16)dwPage >= (nordescription->SectorsPerBlk * nordescription->Blks)))
    {
        return (-1);
    }

    *pBlock = (dwPage >> 3); // 即dwPage除以8，转为block
    *pRemain = 7 - (dwPage & 7); // 即dwPage对8取余，再被7减（因为dwPage是从零计）

    return (0);
}
#endif

#if 0
// ============================================================================
// 功能：初始化SPI FLASH模块，校验芯片ID是否正确
// 参数：pName -- 设备名；
//      dwStart -- 起始块；
//      dwSize -- 块数；
//      dwSpecial -- 用于EFS（2）；用于其他其他文件系统（1）；
//      pPrivate -- 通信线
// 返回：成功初始化（0）；初始化失败（-1）；
// 备注：逻辑待优化
// ============================================================================
s32 __AT45_MakePartition(char *pName, u32 dwStart, u32 dwSize, u32 dwSpecial, void *pPrivate)
{
    static u8 number = 0; // 分区号，从1算逻辑。
    struct FlashChip *flash, *partition;
    u8 *buf;
    char *name;

    if(0 == dwSize)
    {
        return (0); // 不做处理
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

        // AT45的sector比block大，而且sector的大小不一致。这里逻辑上就将sector等于page，
        // 忽然sector,block最大。
        flash->Descr.Nor.PortType = NOR_SPI;
        flash->Descr.Nor.Port = pPrivate;
        flash->Descr.Nor.BytesPerPage = 512;
        flash->Descr.Nor.PagesPerSector = 1;
        flash->Descr.Nor.SectorsPerBlk = 8;
        flash->Descr.Nor.Blks = 1024; // 全部器件的容量
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

        // AT45的sector比block大，而且sector的大小不一致。这里逻辑上就将sector等于page，
        // 忽然sector,block最大。
        partition->Descr.Nor.PortType = NOR_SPI;
        partition->Descr.Nor.Port = pPrivate;
        partition->Descr.Nor.BytesPerPage = 512;
        partition->Descr.Nor.PagesPerSector = 1;
        partition->Descr.Nor.SectorsPerBlk = 8;
        partition->Descr.Nor.Blks = 0; // 暂时未用
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
    // 当前逻辑用于EFS
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

        // AT45的sector比block大，而且sector的大小不一致。这里逻辑上就将sector等于page，
        // 忽然sector,block最大。
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
        um->opt = opt; // 驱动操作逻辑
        um->type = nor;
        um->ubuf = (u8*)um + sizeof(struct umedia);
        um->ustart = dwStart * 8; // 起始unit号

        return (um_add((const char*)name, um));
    }
#endif

    return (0);
}
#endif



#if 1 // 新接口
#include <device/include/unit_media.h>
// ============================================================================
// 功能：读AT45
// 参数：unit -- 页号；data --存读到数据的缓存；
// 返回：成功 -- （0）；失败 -- （-1）
// 备注：
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
// 功能：写AT45
// 参数：unit -- 页号；data -- 要写的数据；
// 返回：成功 -- （0）；失败 -- （-1）
// 备注：
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
// 功能：差AT45的页或块
// 参数：unit -- 页或者块；
//      sz -- 页或者块；
// 返回：成功 -- （0）；失败 -- （-1）
// 备注：
// ============================================================================
s32 __at45_erase(s64 unit, struct uesz sz)
{
    u32 block;

    if(sz.unit)
        block = unit / 8; // 一个块内有8个page
    else if (sz.block)
        block = (u32)unit;

    return (__AT45_BlockErase(block));
}

// ============================================================================
// 功能：at45命令
// 参数：cmd -- 命令；
//      args -- 可变参，命令参数；
// 返回：
// 备注：
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

        case checkbad: break; // 检查坏块
        default: res = -1; break;
    }

    return (res);
}
#endif

// ============================================================================
// 功能：初始化SPI FLASH模块，用做文件系统
// 参数：   fs -- 该媒体所要安装文件系统mount点名字；
//      MountPart -- 文件系统安装在第几个分区
//      dwStart -- 起始块；
//      dwSize -- 块数；
//      dwSpecial -- 擦除该区域（1）；不擦除该区域（0）；
// 返回：成功初始化（0）；初始化失败（-1）；
// 备注：分区逻辑用于文件系统，直接访问逻辑不用设置分区。
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
    super->MediaStart = dwStart * 8; // 起始unit号

    res = strlen(At45Name)+strlen(s_ptDeviceRoot->name);
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,At45Name);		//获取设备的全路径
    FsBeMedia(FullPath,fs);		//往该设备挂载文件系统
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, dwStart, dwEnd);
    return (0);
}

//// =============================================================================
//// 功能：初始化AT45，一般用于安装文件系统
//// 参数：bstart -- 起始块；
////     bcount -- 该分区一共有多少块
////     dwSpecial -- 擦除该区域（1）；不擦除该区域（0）；
//// 返回：成功（0）；失败（-1）；
//// 备注：分区逻辑用于文件系统，直接访问逻辑不用设置分区。
//s32 __at45_part_init(const char *fs,s32 MountPart,u32 bstart, u32 bcount, u32 doformat)
//{
//    static char *name = "AT45DB321E";    //要和ModuleInstall_at45db321中的name保持一致
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
                            简易文件系统函数接口
******************************************************************************/

// ============================================================================
// 功能： 写一页的数据
// 参数：
// 返回： 失败（0）；成功（写出数据大小）
// 备注：
// ============================================================================
static s32 __WritePage(u32 dwPage, u8 *pData, u32 dwDummy)
{
    return (__AT45_PageWrite(dwPage, pData, 0));
}

// ============================================================================
// 功能： 写一页的数据
// 参数：
// 返回： 失败（0）；成功（写出数据大小）
// 备注：
// ============================================================================
static s32 __ReadPage(u32 dwPage, u8 *pData, u32 dwDummy)
{
    return (__AT45_PageRead(dwPage, pData, 0));
}

// ============================================================================
// 功能： 写小于一页的数据
// 参数：
// 返回： 失败（-2）；成功（写出数据大小）
// 备注：按照528字节操作
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
    Djy_EventDelay(4000);// 延时切出. 4ms

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
                ret = -2; // 失败
            }
        }
    }

    Lock_MutexPost(pNOR->Lock);
    return (ret);
}

// ============================================================================
// 功能： 读小于一页的数据
// 参数：
// 返回： 失败（-2）；成功（读入数据字节数）；
// 备注：
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
// 功能： 写数据
// 参数： dwBlock -- 块号;
//       dwOffset -- 块内偏置;
//       pBuf -- 写缓冲;
//       dwSize -- 写数据大小
//       bFlags -- 无功能(0x0); ECC功能开启，数据大小为必须256（0x1）。
// 返回：错误（0）；成功（写出数据大小）；
// 备注：
// ============================================================================
u32 __EFS_IF_WriteData(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags)
{
    u32 page = (dwBlock << 3) + (dwOffset >> 9); // 页号
    u32 offset = dwOffset & 0x1FF; // 页内的偏置
    u32 wrLen = dwSize;
    s32 res;
    u8 *data = pBuf;

    if(!data)
        return (0);

#if 0 // NOR设备不做ECC，同时为了兼容旧的使用逻辑（旧逻辑按512的页逻辑进行的读写）
    // ECC的数据源为256字节，放入数据后端,3个字节
    if(1 == bFlags)
    {
        hamming_compute_256x(data, 256, data+256);
        wrLen = dwSize + 3;
        offset += 3 * (offset >> 8); // 这里的逻辑是每256都会有一个ECC
    }
#endif

    for(;;)
    {
        if((!offset) && (wrLen >= pNOR->dwPageBytes))
            res = __WritePage(page, data, NO_ECC);
        else if((wrLen >= pNOR->dwPageBytes) || // 写数据大与等于一页,但内容跨页
                (((wrLen+offset) > pNOR->dwPageBytes) && (!bFlags))) // 写数据小于一页,但内容跨页(只有考虑非ECC的情况)
            res = __WriteFragment(page, offset, (const u8*)data, (pNOR->dwPageBytes-offset));
        else
            res = __WriteFragment(page, offset, (const u8*)data, wrLen);

        if(res <= 0)
            break; // 错误或者已写完

        wrLen -= res;
        if(wrLen <= 0)
            break; // 写完

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
// 功能： 读数据
// 参数： dwBlock -- 块号;
//       Offset -- 块内偏置;
//       pBuf -- 读缓冲;
//       dwSize -- 读数据大小
//       bFlags -- 0x0 -- 无功能; 0x1 - ECC功能开启(数据大小为必须256)
// 返回：失败（0）；成功（读入的字节数）；
// 备注：
// ============================================================================
u32 __EFS_IF_ReadData(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags)
{
    s32 res;
    u32 page = (dwBlock << 3) + (dwOffset >> 9); // 页号
    u32 offset = dwOffset & 0x1FF; // 页内的偏置
    u32 rdLen = dwSize;
    u8 *data = pBuf;

    if(!data)
        return (0);
#if 0 // NOR设备不做ECC，同时为了兼容旧的使用逻辑（旧逻辑按512的页逻辑进行的读写）
    if(1 == bFlags)
    {
        rdLen += 3;
        offset += 3 * (offset >> 8); // 这里的逻辑是每256都会有一个ECC
    }
#endif

    for(;;)
    {
        if((!offset) && (rdLen >= pNOR->dwPageBytes))
            res = __ReadPage(page, data, NO_ECC);
        else if((rdLen >= pNOR->dwPageBytes) || // 写数据大与等于一页,但内容跨页
                (((rdLen+offset) > pNOR->dwPageBytes) && (!bFlags))) // 读数据小于一页,但内容跨页(只有非ECC的情况需要考虑)
            res = __ReadFragment(page, offset, data, (pNOR->dwPageBytes - offset));
        else
            res = __ReadFragment(page, offset, data, rdLen);

        if(res <= 0)
            break; // 错误或者已写完

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

#if 0 // NOR设备不做ECC，同时为了兼容旧的使用逻辑（旧逻辑按512的页逻辑进行的读写）
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
// 功能： 擦除
// 参数： BlkNo -- 块号;
// 返回：成功（TRUE）；失败（FALSE）；
// 备注：
// ============================================================================
bool_t __EFS_IF_Erase(u32 dwBlock)
{
    if(!__AT45_BlockErase(dwBlock))
        return (TRUE);

    return (FALSE);
}
#if 0
// ============================================================================
// 功能： 检查文件是否可写
// 参数：dwBlock -- 块号；dwOffset -- 偏置；
// 返回：可写（0）；不可写（-1）；
// 备注：
// ============================================================================
s32 __EFS_IF_IsFragmentWritten(u32 dwBlock, u32 dwOffset, u8 *pData)
{
    u8 *buf = pData;
    u16 i;
    u32 offset = dwOffset & 0x1FF; // 页内偏置
#if 0
    u8 ecc = 3 * (offset >> 8); // ECC偏置，这里的逻辑是每256都会有一个ECC
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
// 功能：
// 参数：
// 返回：
// 备注：
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
// 功能：本地测试逻辑
// 参数：
// 返回：
// 备注：
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


