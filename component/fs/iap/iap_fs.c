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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//TODO: 支持多个文件，那么就可以支持multiboot
#include <systime.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <device.h>
#include <djyos.h>
#include <list.h>
#include <stdarg.h>
#include <iap.h>
#include "iap_fs.h"
#include "dbug.h"
#include "../filesystems.h"
#include "component_config_iap.h"

//
// 底层接口函数
//
extern s32 LowLevelWrite(void *LowLevel, u8 *Buf, u32 Bytes, u32 Addr);
extern s32 LowLevelRead(void *LowLevel, u8 *Buf, u32 Bytes, u32 Addr);
extern s32 LowLevelErase(void *LowLevel, u32 Bytes, u32 Addr);
extern void *LowLevelInit(void *pDev, u32 *pBase, u32 *pBufSize);
extern void LowLevelDeInit(void *LowLevel);
extern u32 LowLevelCRC32(void *LowLevel, u32 Offset, u32 Len);

static s32 __iap_fs_install(tagFSC *pSuper, u32 flags, void *pData);
static ptu32_t __iap_file_operations(u32 cmd, ptu32_t oof, ptu32_t args,  ...);
//
//
//
#define FILE_HEAD_SIZE                  ((u32)256)
#define FILE_HEAD_OFFSET                (FILE_HEAD_SIZE)
#define MAX_FILES                       (1) // 支持的最大文件数
#define FILE_NAME_MAX_LEN               (240) // 支持的最大文件名长度(包括结束符)

//
// IAP文件用途标签
//
#define S_APP_UPDATE_DONE               ((u32)0x61707000)
#define S_APP_UPDATE_ING                ((u32)0xFFFFFFFF)
#define S_APP_DEBUG                     ((u32)0x00676264)

// IAP文件状态
#define IAP_TEMPORARY                   ((u32)0x1)
#define IAP_UPDATING                    ((u32)0x2)
#define IAP_UPDATED                 ((u32)0x3)

//
// IAP文件系统类型
//
tagFST typeIAP = {
        __iap_file_operations,
        __iap_fs_install,
        NULL,
        NULL,
        "IAP"
};

//
// IAP文件信息
//
struct __ifile{
    struct __portBasic basic; // 接入文件系统用
//  char *pName; // 这个信息放置于object
    u32 dwBase; // 缓存时，文件内容的偏置。
    u32 dwSize; // 文件大小
    u32 dwStatus; // 文件状态；
    struct MutexLCB *pLock; // 文件锁；
};

//
// IAP文件上下文
//
struct __icontext{
    u32 dwCur; // 文件的当前位置是dwCur+buffer已缓存部分；
    u8 *pBufSt; // 缓存边界，起始；
    u8 *pBufMov; // 已缓存；
    u8 *pBufEd; // 缓冲边界，结束；
};

//
// IAP文件系统管理信息
//
struct __icore {
    void *pVol; // 文件系统底层抽象，volume；
    u32 dwBufferSize; // 当大于零时，表示存在缓冲。原因，对于小数据量的多次写入会造成内部自带ECC的设备的ECC错误
    u32 dwBase; // 文件系统在volume中的偏置
    struct Object *pRoot; // IAP文件系统接入DJYFS的文件对象；
};

// ============================================================================
// 功能：解析文件格式(头部)；
// 参数：head -- IAP文件头部格式；
//      file -- IAP文件；
// 返回：IAP文件；
// 备注：
// ============================================================================
struct __ifile *__decodefilehead(struct headFormat *head, struct __ifile *file)
{
    u16 i, len;
    u8 *temp = (u8*)head;

    if(!file)
        return (NULL);

    for(i = 0; i < FILE_HEAD_SIZE; i++)
    {
        if(0xFF != temp[i])
            break;
    }

    if(i == FILE_HEAD_SIZE)
    {
        info_printf("iapfs","invalid file, empty(head is all 0xFF).");
        return (NULL); // 全FF数据，无效，表示没有文件
    }

    if((S_APP_UPDATE_DONE != head->signature) &&
       (S_APP_DEBUG != head->signature)) // debug模式的程序标签
    {
        error_printf("iapfs","invalid file, signature<%xH> is bad.", head->signature);
        return (NULL); // 格式错误
    }

    len = strlen(head->name) + 1;
    if(FILE_NAME_MAX_LEN < len)
    {
        error_printf("iapfs","too long file name<%d>, limitation<%d>.", len, FILE_NAME_MAX_LEN);
        return (NULL);
    }

    file->dwSize = head->size;
    file->dwStatus = IAP_UPDATED;

    return (file);
}

// ============================================================================
// 功能：计算文件的CRC值
// 参数：core -- IAP文件系统信息；
//      file -- IAP文件；
// 返回：文件的CRC32值；
// 备注：
// ============================================================================
static u32 __crc(struct __icore *core, struct __ifile *file)
{
    return (LowLevelCRC32(core->pVol, file->dwBase, file->dwSize));
}

// ============================================================================
// 功能：新建IAP文件
// 参数：core -- IAP文件系统信息；
// 返回：成功（IAP文件）； 失败(NULL);
// 备注：
// ============================================================================
static struct __ifile *__newfile(struct __icore *core)
{
    struct __ifile *file;
    u32 size = sizeof(*file);

    file = malloc(size);
    if(!file)
        return (NULL);

    memset(file, 0x0, size);
    file->pLock = Lock_MutexCreate(NULL);
    if(!file->pLock)
    {
        free(file);
        return (NULL);
    }

    file->dwBase = core->dwBase + FILE_HEAD_SIZE;
    of_initbasic(&file->basic, O_RDWR);
    file->dwStatus = IAP_TEMPORARY;

    return (file);
}

// ============================================================================
// 功能： 文件头部位置
// 参数：file -- IAP文件
// 返回：文件头部位置偏置
// 备注：
// ============================================================================
u32 __locatefilehead(struct __ifile *file)
{
    return (file->dwBase - FILE_HEAD_SIZE);
}

// ============================================================================
// 功能： 格式化IAP文件（头部）
// 参数：core -- IAP文件系统信息；
//      file -- IAP文件；
// 返回：失败（-1）；成功（0）。
// 备注：
// ============================================================================
s32 __formatfilehead(struct __icore *core, struct __ifile *file)
{
    s32 res;
    u32 base;

    if(file)
        base = __locatefilehead(file);
    else
        base = core->dwBase;

    res = LowLevelErase(core->pVol, FILE_HEAD_SIZE, base);
    if(FILE_HEAD_SIZE != res)
        return (-1);

    return (0);
}

// ============================================================================
// 功能：建立文件格式（头部）
// 参数：core -- IAP文件系统信息；
//      file -- IAP文件；
//      pName -- IAP文件名；
// 返回：失败（-1）；成功（0）。
// 备注：
// ============================================================================
s32 __makefilehead(struct __icore *core, struct __ifile *file, const char *pName)
{
    s32 res;
    struct headFormat head;

    head.size = file->dwSize;
    strcpy(head.name, pName);
    head.signature = S_APP_UPDATE_DONE;
    head.reserved = -1;
    head.crc = __crc(core, file);

    res = LowLevelWrite(core->pVol, (u8*)&head, FILE_HEAD_SIZE,
            __locatefilehead(file));
    if(FILE_HEAD_SIZE != res)
        return (-1);

    file->dwStatus = IAP_UPDATED;

    return (0);
}


// ============================================================================
// 功能：扫描IAP文件
// 参数：core -- IAP文件系统信息；
// 返回：失败，未找到文件（-1）；成功找到文件（0）。
// 备注：
// ============================================================================
static s32 __scanfiles(struct __icore *core)
{
    s32 res;
    struct headFormat structFileHead;
    struct __ifile *file;

    res = LowLevelRead(core->pVol, (u8*)&structFileHead, FILE_HEAD_SIZE, core->dwBase);
    if(res != FILE_HEAD_SIZE)
        return (-1);

    // 当前只有一个文件
    file = __newfile(core);
    if(NULL == __decodefilehead(&structFileHead, file))
    {
#if 0
        info_printf("IAP","format the disk, please wait    ");
        Res = LowLevelFormat(s_ptIAP_Core->vol); // 不存在有效文件，为保险起见，格式化整个vol
        if(Res)
        {
            info_printf("iapfs","\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
            info_printf("iapfs","                 ");
            info_printf("iapfs","\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b erro.\r\n");
            return (-1);
        }

        info_printf("iapfs","\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
        info_printf("iapfs","                 ");
        info_printf("iapfs","\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b successfully.\r\n");
#else
        // 当前逻辑不在上电检索文件的时候格式整个空间,而只格式一个头部。
        // 必须要这个逻辑，因为在升级过程的中断，往往是头部不存在，而后续有内容。而第一次写入时，并不想照顾这个逻辑。
        res = __formatfilehead(core, file);
#endif
        return (res); // 当前系统已无文件，后续逻辑不执行
    }


    // 将内容接入文件系统
    res = of_virtualize(core->pRoot, &file->basic, structFileHead.name);
    if(res)
    {
        free(file);
        return (-1);
    }

    info_printf("iapfs","valid file, name<%s>, size<%dKB>.", structFileHead.name, (file->dwSize>>10));

    return (0);
}

// ============================================================================
// 功能：释放IAP文件（内存中的空间）
// 参数：file -- IAP文件；
// 返回：
// 备注：INLINE
// ============================================================================
static void __freefile(struct __ifile *file)
{
    if(file == IAP_UPDATING)
        return;

    of_unlinkbasic(&file->basic);
    Lock_MutexDelete(file->pLock);
    free(file);
}

// ============================================================================
// 功能：删除IAP文件，包括存储介质上的数据
// 参数：core -- IAP文件系统信息；
//      file -- IAP文件；
// 返回：失败（-1）；成功（0）。
// 备注：INLINE
// ============================================================================
static s32 __delfile(struct __icore *core, struct __ifile *file)
{
    s32 res;

    res = LowLevelErase(core->pVol, FILE_HEAD_SIZE, __locatefilehead(file));
    if(FILE_HEAD_SIZE != res)
        return (-1);

    __freefile(file);
    return (0);
}

// ============================================================================
// 功能：新建IAP文件上下文；
// 参数：core -- IAP文件系统信息；
// 返回：成功（IAP文件上下文）；失败（NULL）；
// 备注：
// ============================================================================
static struct __icontext *__newcontext(struct __icore *core)
{
    struct __icontext *context;

    context = malloc(sizeof(*context) + core->dwBufferSize);
    if(!context)
        return (NULL);

    context->dwCur = 0;
    context->pBufSt = (u8*)context + sizeof(*context);
    context->pBufMov = context->pBufSt;
    context->pBufEd = context->pBufSt + core->dwBufferSize;

    return (context);
}

// ============================================================================
// 功能：释放IAP文件上下文
// 参数：pContext -- IAP文件上下文；
// 返回：
// 备注：INLINE
// ============================================================================
void __freecontext(struct __icontext *pContext)
{
    return (free(pContext));
}

// ============================================================================
// 功能：打开文件
// 参数：pObj -- 文件对象；
//      dwFlags -- 文件操作标志位；
//      pNew -- 需新建的文件名称；
// 返回：成功（IAP文件上下文）；失败（NULL）；
// 备注：
// ============================================================================
static tagOFile *__iap_open(struct Object *pObj, u32 dwFlags, char *pNew)
{
    tagOFile *of;
    struct __ifile *file = NULL;
    struct __icontext *context = NULL;
    struct __icore *core = (struct __icore*)FS_Core(pObj);

    if(GROUP_POINT == __OBJ_Type(pObj))
    {
        if(!testdirectory(dwFlags))
            return (NULL);
    }

    if(testdirectory(dwFlags)) // 目录逻辑
    {
        if(pNew)// 不支持新建目录
        {
            info_printf("iapfs","do not support create directory.");
            return (NULL);
        }
    }
    else // 文件的逻辑
    {
        if(pNew) // 文件不存在，需要新建
        {
            if(!testcreat(dwFlags))
            {
                return (NULL); // 打开操作中无新建要求，则返回不存在；
            }

            file = __newfile(core);
            if(!file)
            {
                return (NULL);
            }

            __formatfilehead(core, file);
            if(of_virtualize(pObj, &file->basic, pNew)) // IAP文件，链入文件系统
                return (NULL);
        }
        else // 文件已存在
        {
            if(testonlycreat(dwFlags))
            {
                return (NULL); // 必须新建逻辑，但文件已存在
            }

            file = dListEntry(of_basiclinko(pObj), struct __ifile, basic);
            if(testtrunc(dwFlags))
            {
                if(-1 == __formatfilehead(core, file))
                {
                    return (NULL);
                }

                file->dwSize = 0;
                file->dwStatus = IAP_UPDATING;
            }
        }

        context = __newcontext(core);
        if(!context)
        {
            if(pNew)
                __freefile(file); // 释放掉上面创建的

            return (NULL);
        }

        if(testappend(dwFlags))
        {
            context->dwCur = file->dwSize;
        }
    }

    of = of_new();
    if(!of)
    {
        __freecontext(context);
        if(pNew)
            __freefile(file); // 释放掉上面创建的
    }

    of_init(of, NULL, dwFlags, (ptu32_t)context);

    return (of);
}

// ============================================================================
// 功能：关闭文件
// 参数：of -- 由open打开的IAP文件上下文；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __iap_close(tagOFile *of)
{
    s32 res, offset, todos;
    struct __icore *core;
    struct __ifile *file;
    struct __icontext *context = (struct __icontext*)of_context(of);

    if(context) // NULL时表示目录
    {
        core = (struct __icore*)of_core(of);
        file = dListEntry(of_basic(of), struct __ifile, basic);
        Lock_MutexPend(file->pLock, CN_TIMEOUT_FOREVER);

        if(IAP_UPDATED != file->dwStatus) // 数据存在写入操作或者文件是新建的
        {
            if(context->pBufMov != context->pBufSt)
            {
                todos = context->pBufMov - context->pBufSt;
                offset = context->dwCur + file->dwBase;
                res = LowLevelWrite(core->pVol, context->pBufSt, todos, offset);
                if(res != todos)
                {
                    Lock_MutexPost(file->pLock);
                    return (-1);
                }
            }

            if(!iscontender(of)) // 最后一个文件使用者关闭文件时，才会设置文件头
            {
                res = __makefilehead(core, file, of_name(of)); // 文件关闭时，建立文件头
                if(res)
                {
                    Lock_MutexPost(file->pLock);
                    return (-1);
                }
            }
        }

        Lock_MutexPost(file->pLock);
    }

    __freecontext(context);
    of_free(of);
    return (0);
}

// ============================================================================
// 功能：写文件
// 参数：of -- 由open打开的IAP文件上下文；
//      buf -- 需写入数据的空间；
//      size -- 需写入数据的大小；
// 返回：实际写入的字节数；
// 备注：
// ============================================================================
static s32 __iap_write(tagOFile *of, u8 *buf, u32 size)
{
    s32 offset, todos, buffers, wren = 0, frees, res = 0;
    struct __icontext *context = (struct __icontext *)of_context(of);
    struct __icore *core = (struct __icore*)of_core(of);
    struct __ifile *file = dListEntry(of_basic(of), struct __ifile, basic);

    Lock_MutexPend(file->pLock, CN_TIMEOUT_FOREVER);
    buffers = context->pBufMov - context->pBufSt;
    if(((context->dwCur + buffers) < file->dwSize) || (IAP_UPDATED == file->dwStatus))
    {
        info_printf("iapfs","writing the written area is unpredictable.");
    }

    while(1)
    {
        if(context->dwCur < (core->dwBufferSize - file->dwBase))
        {
            todos = context->dwCur % (core->dwBufferSize - file->dwBase); // 考虑了缓存对齐
            todos = frees = (context->pBufEd - context->pBufMov - file->dwBase) - todos; // 缓冲中的剩余空间,去除第一页的文件头
        }
        else
        {
            todos = (context->dwCur - file->dwBase) % core->dwBufferSize; // 考虑了缓存对齐
            todos = frees = context->pBufEd - context->pBufMov - todos; // 缓冲中的剩余空间
        }

        if(size < frees)
            todos = size;

        memcpy(context->pBufMov , buf, todos);
        context->pBufMov += todos;
        if(size < frees)
            break;

        // 缓冲已满，刷入
        offset = context->dwCur + file->dwBase;
        buffers = context->pBufMov - context->pBufSt;
        res = LowLevelWrite(core->pVol, context->pBufSt, buffers, offset);
        if(res != buffers)
        {
            todos = 0; // 当全部写入失败处理；
            break;
        }

        // 缓冲重置
        context->pBufMov = context->pBufSt;
        memset(context->pBufSt, 0xFF, core->dwBufferSize);
        wren += todos;
        size -= todos;
        buf += todos;
        context->dwCur += buffers;
    }

    wren += todos;
    buffers = context->pBufMov - context->pBufSt;
    if(file->dwSize < (context->dwCur + buffers))
        file->dwSize = context->dwCur + buffers;

    if((wren) && (IAP_UPDATING != file->dwStatus))
        file->dwStatus = IAP_UPDATING; // 文件数据发生变改变

    Lock_MutexPost(file->pLock);

    return (wren);
}

// ============================================================================
// 功能：读文件
// 参数：of -- 由open打开的IAP文件上下文；
//      buf -- 需读出数据的存放空间；
//      size -- 需读出数据的大小；
// 返回：实际读出的单元数；
// 备注：
// ============================================================================
static s32 __IAP_Read(tagOFile *of, u8 *buf, u32 size)
{
    s32 buffers, res, offset, unalign, frees;
    struct __ifile *file = dListEntry(of_basic(of), struct __ifile, basic);
    struct __icontext *context = (struct __icontext*)of_context(of);
    struct __icore *core = (struct __icore*)of_core(of);

    Lock_MutexPend(file->pLock, CN_TIMEOUT_FOREVER);
    buffers = context->pBufMov - context->pBufSt;
    if((context->dwCur + buffers + size) > file->dwSize)
    {
        size = file->dwSize - (context->dwCur + buffers); // 读越界了
    }

    if(context->dwCur < (core->dwBufferSize - file->dwBase))
    {
        unalign = context->dwCur % (core->dwBufferSize - file->dwBase); // 考虑了缓存对齐
        frees = context->pBufEd - context->pBufMov - file->dwBase - unalign; // 文件头放置在最开始的地方，需要预留空间
    }
    else
    {
        unalign = (context->dwCur - file->dwBase) % core->dwBufferSize; // 考虑了缓存对齐
        frees = context->pBufEd - context->pBufMov - unalign;
    }

    if(frees < size)
    {
        // 缓冲的剩余空间容纳不了回读的数据，则先将数据刷下去，再回读。回读后，将不对其的数据存入缓冲；
        if(buffers)
        {
            offset = context->dwCur + file->dwBase;
            res = LowLevelWrite(core->pVol, context->pBufSt, buffers, offset);
            if(res != buffers)
            {
                // 缓冲数据刷入失败；
                Lock_MutexPost(file->pLock);
                return (0);
            }
        }

        // 从当前位置把数据读回
        offset = context->dwCur + buffers + file->dwBase;
        res = LowLevelRead(core->pVol, buf, size, offset);
        if(res)
        {
            if(buffers)
            {
                // 不放在上面，为了防止读失败；从而保证了缓存的对齐逻辑；
                context->pBufMov = context->pBufSt; // 缓冲全部刷入，重置到缓冲头。
                memset(context->pBufSt, 0xFF, core->dwBufferSize);
            }

            // 把不对齐的部分放入缓冲，从而保证dwCur是缓冲对齐的
            offset = context->dwCur + buffers + res; // 读后的位置
            buffers = core->dwBufferSize - file->dwBase; // 考虑了含有头部的可缓存大小的对齐逻辑
            if(offset > buffers)
                unalign = (offset - buffers) % core->dwBufferSize;
            else
                unalign = offset % buffers;

            context->dwCur = offset - unalign;
            memcpy(context->pBufMov, (buf + res - unalign), unalign);
            context->pBufMov += unalign;
        }
    }
    else
    {
        // 缓冲的剩余空间足够，回读后，将数据也存入缓冲；
        offset = context->pBufMov - context->pBufSt;
        offset += context->dwCur + file->dwBase;
        res = LowLevelRead(core->pVol, buf, size, offset);
        memcpy(context->pBufMov, buf, res);
        context->pBufMov += res;
    }

    Lock_MutexPost(file->pLock);
    return (res);
}

// ============================================================================
// 功能：设置文件当前位置
// 参数：of -- 由open打开的IAP文件上下文；
//      pOffset -- 移动位置的量；
//      dwWhence -- 移动位置的起点；
// 返回：成功（0，*pOffset指向新的当前位置）；失败（-1）；
// 备注：
// ============================================================================
static s32 __iap_seek(tagOFile *of, s64 *pOffset, s32 dwWhence)
{
    struct __icontext *context;
    struct __ifile *file;
    struct __icore *core;
    s32 todos, res, offset, pos, unalign;
    u32 bufUsed, bufFree;
    s64 moves = *pOffset;

    if(isdirectory(of))
    {
        info_printf("iapfs","cannot seek directory.");
        return (-1);
    }

    core = of_core(of);
    context = (struct __icontext*)of_context(of);
    file = dListEntry(of_basic(of), struct __ifile, basic);

    Lock_MutexPend(file->pLock, CN_TIMEOUT_FOREVER);
    switch(dwWhence)
    {
        case SEEK_END:
        {
            moves = file->dwSize + offset; // 转为从头SEEK逻辑
        }

        case SEEK_SET:
        {
            // 转为当前位置的SEEK的逻辑
            moves = offset - (context->dwCur + (context->pBufMov - context->pBufSt));
        }

        case SEEK_CUR:
        {
            todos = context->pBufMov - context->pBufSt;
            pos = context->dwCur + todos + moves; // 新位置
            if(pos < 0)
                pos = 0; // 越界了

            // 新位置在缓存范围内，不需要将缓冲刷下去，先计算缓存边界（这里考了到缓存的对齐）
            if(context->dwCur <= (core->dwBufferSize - file->dwBase))
            {
                bufUsed = context->pBufMov - context->pBufSt;
                unalign = context->dwCur % (core->dwBufferSize - file->dwBase);
                bufFree = context->pBufEd - context->pBufMov - file->dwBase - unalign;
            }
            else
            {
                bufUsed = context->pBufMov - context->pBufSt;
                unalign = (context->dwCur - file->dwBase) % core->dwBufferSize;
                bufFree = context->pBufEd - context->pBufMov - unalign;
            }

            offset = context->dwCur + file->dwBase;
            if((offset >= 0) && ((bufFree - offset) <= 0)) // 新位置在缓冲范围内，向后seek;
            {
                res = LowLevelRead(core->pVol, context->pBufMov, offset, offset);
                context->pBufMov += res;
                if(res != offset)
                {
                    res = -1;
                    break;
                }
            }
            else if((offset < 0) && ((offset + bufUsed) >= 0)) // 新位置在缓冲范围内， 向前seek
            {
                context->pBufMov += offset;
                memset(context->pBufMov, 0xFF, (-offset));
            }
            else // 新位置不在缓存范围内，缓冲数据需先刷下去,再更新
            {
                todos = context->pBufMov - context->pBufSt;
                if(todos)
                {
                    res = LowLevelWrite(core->pVol, context->pBufSt, todos, offset);
                    if(res != todos)
                    {
                        // 缓冲数据刷入失败；
                        res = -1;
                        break;
                    }

                    context->pBufMov = context->pBufSt; // 缓冲重置
                    memset(context->pBufSt, 0xFF, core->dwBufferSize);
                }

                context->dwCur = pos;
                // 缓冲不对齐的部分
                if(pos <= (core->dwBufferSize - file->dwBase))
                    unalign = pos % (core->dwBufferSize - file->dwBase);
                else
                    unalign = pos % core->dwBufferSize;

                if(unalign)
                {
                    context->dwCur -= unalign;
                    offset = context->dwCur + file->dwBase;
                    res = LowLevelRead(core->pVol, context->pBufSt, unalign, offset);
                    context->pBufMov += res;
                    if(res != todos)
                    {
                        res = -1;
                        break;
                    }
                }
            }

            *pOffset = context->dwCur + (context->pBufMov - context->pBufSt);
            res = 0;
            break;
        }

        default:
        {
            res = -1;
            break;
        }
    }

    return (res);
}

// ============================================================================
// 功能：删除文件
// 参数：pObj -- IAP文件对象；
// 返回：成功（0）；失败（-1）；
// 备注：未考虑互斥；当pName为NULL时，表示文件正在被使用；
// ============================================================================
static s32 __iap_remove(struct Object *pObj)
{
    struct __ifile *file;
    struct __icore *core;

    core = (struct __icore *)FS_Core(pObj);
    file = dListEntry(of_basiclinko(pObj), struct __ifile, basic);

    return (__delfile(core, file));

}

// ============================================================================
// 功能: 文件查询
// 参数:
// 返回: 成功（0）；失败（-1）；
// 备注:
// ============================================================================
static s32 __iap_stat(struct Object *obj, struct stat *data)
{
    struct __ifile *file = dListEntry(of_basiclinko(obj), struct __ifile, basic);

    if(GROUP_POINT != __OBJ_Type(obj))
    {
        data->st_size = file->dwSize;
        data->st_mode = S_IFREG;
    }
    else
    {
        data->st_size = 0;
        data->st_mode = S_IFDIR;
    }
    return (0);
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
static s32 __iap_readdentry(tagOFile *pDirectory, struct dirent *pDirent)
{
   ;
}

// ============================================================================
// 功能：安装IAP文件系统
// 参数：pSuper -- 文件系统管理信息；
//      dwFlags -- 安装逻辑标志位；
//      pData -- 自定义数据；
// 返回：成功(0)； 失败(-1)。
// 备注：
// ============================================================================
static s32 __iap_fs_install(tagFSC *pSuper, u32 dwFlags, void *pData)
{
    s32 res;
    u32 base;
    u32 size;
    struct __icore *core;

    core = malloc(sizeof(*core));
    if(!core)
    {
        error_printf("iapfs","install failed<memory out>.");
        return (-1);
    }

    memset(core, 0x0, sizeof(*core));
    core->pVol = LowLevelInit((void*)dev_dtago(pSuper->pDev), &base, &size);
    if(NULL == core->pVol)
    {
        free(core);
        return (-1);
    }

    core->dwBase = base;
    core->dwBufferSize = size;
    core->pRoot = pSuper->pTarget;
    res = __scanfiles(core); // 扫描已存在文件
    if(res)
    {
        free(core);
        return (-1);
    }

    pSuper->pCore = (void*)core;

    return (0);
}

// ============================================================================
// 功能：IAP文件操作接口
// 参数：标准逻辑，查看接口说明；
// 返回：标准逻辑，查看接口说明；
// 备注:
// ============================================================================
static ptu32_t __iap_file_operations(u32 cmd, ptu32_t oof, ptu32_t args,  ...)
{
    va_list list;

    switch(cmd)
    {
        case CN_OBJ_CMD_OPEN:
        {
            char *new;
            struct Object *obj = (struct Object*)oof;
            u32 flags = (u32)args;

            va_start(list, args);
            new = (char*)va_arg(list, u32);
            va_end(list);

            return ((ptu32_t)__iap_open(obj, flags, new));
        }

        case CN_OBJ_CMD_CLOSE:
        {
            tagOFile *of = (tagOFile*)oof;
            return ((ptu32_t)__iap_close(of));
        }

        case CN_OBJ_CMD_READDIR:
        {
            tagOFile *of = (tagOFile*)oof;
            struct dirent *ret = (struct dirent *)args;

            return((ptu32_t)__iap_readdentry(of, ret));
        }

        case CN_OBJ_CMD_READ:
        {
            u32 len;
            tagOFile *of = (tagOFile*)oof;
            u8 *buf = (u8*)args;

            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__IAP_Read(of, buf, len));
        }

        case CN_OBJ_CMD_WRITE:
        {
            u32 len;
            tagOFile *of = (tagOFile*)oof;
            u8 *buf = (u8*)args;

            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__iap_write(of, buf, len));
        }

        case CN_OBJ_CMD_DELETE:
        {

            struct Object *obj = (struct Object*)oof;
            char *notExist = (char*)args;

            if(notExist)
                return ((ptu32_t)-1);

            return ((ptu32_t)__iap_remove(obj));
        }

        case CN_OBJ_CMD_SEEK:
        {
            s32 whence;
            tagOFile *of = (tagOFile *)oof;
            s64 *offset = (s64*)args;


            va_start(list, args);
            whence = (s32)va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__iap_seek(of, offset, whence));
        }

        case CN_OBJ_CMD_STAT:
        {
            struct stat *data;
            struct Object *obj = (struct Object*)oof;
            u32 notfound = (u32)args;

            if(notfound)
                return (-1); // 查询的文件不存在；

            va_start(list, args);
            data = (struct stat*)args;
            va_end(list);
            return ((ptu32_t)__iap_stat(obj, data));
        }

        default:
        {
            debug_printf("iapfs","do not support this operation now.");
            break;
        }
    }

    return (-1);
}

// ============================================================================
// 功能：安装IAP文件系统
// 参数：pDir -- 安装目录；pDev -- 安装设备；
// 返回：失败(-1)； 成功(0)。
// 备注:
// ============================================================================
s32 ModuleInstall_IAP_FS(const char *pDir, const char *pDevPath)
{
    s32 res, fd;
    char *path = "/iboot"; // 默认安装目录
    char *dev = "/dev/embedded flash";

    if(pDir)
        path = (char*)pDir;

    if(pDevPath)
        dev = (char*)pDevPath;

    res = FS_Register(&typeIAP);
    if(res)
    {
        error_printf("iapfs","cannot register \"IAP\"<file system type>.");
        return (-1); // 失败;
    }

    fd = open(path, O_DIRECTORY | O_CREAT | O_EXCL | O_RDWR, 0); // 创建目录
    if(-1 == fd)
    {
        error_printf("iapfs","mount \"IAP\" failed, cannot create \"%s\"<mount point>.", path);
        return (-1);
    }

    res = FS_Mount(dev, path, "IAP", 0, 0);
    if(res < 0)
    {
        error_printf("iapfs","mount \"IAP\" failed, cannot install.");
        close(fd);
        remove(path);
        return (-1);
    }

    close(fd);
    info_printf("iapfs","file system \"IAP\" installed on \"%s\".", dev);
    return (0);
}

