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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
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
#include <dirent.h>
#include <djyos.h>
#include <list.h>
#include <stdarg.h>
#include <iap.h>
#include <device/include/unit_media.h>
#include <stdio.h>
#include "../filesystems.h"

//
// 底层接口函数
//
extern s32 __ll_write(void *ll, u8 *data, u32 bytes, u32 addr);
extern s32 __ll_read(void *ll, u8 *data, u32 bytes, u32 addr);
extern s32 __ll_erase(void *LowLevel, u32 Bytes, u32 Addr);
//extern void *LowLevelInit(void *pDev, u32 *pBase, u32 *pBufSize);
//extern void LowLevelDeInit(void *LowLevel);
//extern u32 LowLevelCRC32(void *LowLevel, u32 Offset, u32 Len);

static s32 __iap_fs_install(tagFSC *super, u32 opt, void *config);
static ptu32_t __iap_ops(enum objops ops, ptu32_t o_hdl, ptu32_t args,  ...);
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
#define IAP_UPDATED                     ((u32)0x3)

//
// IAP文件系统类型
//
tagFST typeIAP = {
        __iap_ops,
        __iap_fs_install,
        NULL,
        NULL,
        "IAP"
};

//
// IAP文件信息
//
struct __ifile{
//  char *name; // 这个信息放置于object
    u32 cxbase; // 文件实际内容的偏置（文件头部信息存放于开始，存在一个偏置）。
    u32 sz; // 文件大小
    u32 status; // 文件状态；
    struct MutexLCB *lock; // 文件锁；
};

//
// IAP文件上下文，缓存未考虑预读逻辑，即只针对写进行了缓存，读未考虑缓存；
//
struct __icontext{
    u32 pos; // 文件的当前位置；
    s16 bufed; // 存在于缓存中的数据；
    u8 *buf; // 是物理的一个缓存，逻辑上是对齐的；
};

//
// IAP文件系统管理信息
//
struct __icore{
    void *vol; // 文件系统底层抽象，volume；
    s16 bufsz; // 当大于零时，表示存在缓冲。需要原因，对于小数据量的多次写入会造成内部自带ECC的设备的ECC错误
    u32 inhead; // 文件的一个区域内容是头部+部分内容，大小为bufsz；inhead这部分为部分内容的大小；
    struct obj *root; // IAP文件系统接入的文件系统的根；
    struct MutexLCB *lock; // 系统锁；
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
        printf("\r\n: info : iapfs  : invalid file, empty(head is all 0xFF).");
        return (NULL); // 全FF数据，无效，表示没有文件
    }

    if((S_APP_UPDATE_DONE != head->signature) &&
       (S_APP_DEBUG != head->signature)) // debug模式的程序标签
    {
        printf("\r\n: erro : iapfs  : invalid file, signature(%xH) is bad.", head->signature);
        return (NULL); // 格式错误
    }

    len = strlen(head->name) + 1;
    if(FILE_NAME_MAX_LEN < len)
    {
        printf("\r\n: erro : iapfs  : too long file name(%d), limitation(%d).", len, FILE_NAME_MAX_LEN);
        return (NULL);
    }

    file->sz = head->size;
    file->status = IAP_UPDATED;
    return (file);
}

// ============================================================================
// 功能：计算文件的CRC值
// 参数：core -- IAP文件系统信息；
//      file -- IAP文件；
// 返回：文件的CRC32值；
// 备注：
// ============================================================================
static inline u32 __crc(struct __icore *core, struct __ifile *file)
{
    return (__ll_crc32(core->vol, file->cxbase, file->sz));
}

// ============================================================================
// 功能：文件上锁
// 参数：
// 返回：
// 备注：
// ============================================================================
static inline void __lock(struct __icore *core)
{
    Lock_MutexPend(core->lock, CN_TIMEOUT_FOREVER);
}

// ============================================================================
// 功能：文件解锁
// 参数：
// 返回：
// 备注：
// ============================================================================
static inline void __unlock(struct __icore *core)
{
    Lock_MutexPost(core->lock);
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
//    file->lock = Lock_MutexCreate(NULL);
//    if(!file->lock)
//    {
//        free(file);
//        return (NULL);
//    }

    file->cxbase = FILE_HEAD_SIZE;
    //handle_initbasic(&file->basic, O_RDWR);
    file->status = IAP_TEMPORARY;
    return (file);
}

// ============================================================================
// 功能： 文件头部位置
// 参数：file -- IAP文件
// 返回：文件头部位置偏置
// 备注：
// ============================================================================
static inline u32 __locatefilehead(struct __ifile *file)
{
    return (file->cxbase - FILE_HEAD_SIZE);
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
    u32 base = 0;

    if(file)
        base = __locatefilehead(file);

   return (__ll_erase(core->vol, FILE_HEAD_SIZE, base));
}

// ============================================================================
// 功能：建立文件格式（头部）
// 参数：core -- IAP文件系统信息；
//      file -- IAP文件；
//      name -- IAP文件名；
// 返回：失败（-1）；成功（0）。
// 备注：
// ============================================================================
s32 __makefilehead(struct __icore *core, struct __ifile *file, const char *name)
{
    struct headFormat head;

    head.size = file->sz;
    strcpy(head.name, name);
    head.signature = S_APP_UPDATE_DONE;
    head.reserved = -1;
    head.crc = __crc(core, file);

    if(-1==__ll_write(core->vol, (u8*)&head, FILE_HEAD_SIZE, __locatefilehead(file)))
        return (-1);

    file->status = IAP_UPDATED;
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

    res = __ll_read(core->vol, (u8*)&structFileHead, FILE_HEAD_SIZE, 0);
    if(res)
        return (-1);

    // 当前只有一个文件
    file = __newfile(core);
    if(NULL == __decodefilehead(&structFileHead, file))
    {
#if 0
        printf("IAP : info : format the disk, please wait    ");
        Res = LowLevelFormat(s_ptIAP_Core->vol); // 不存在有效文件，为保险起见，格式化整个vol
        if(Res)
        {
            printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
            printf("                 ");
            printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b erro.\r\n");
            return (-1);
        }

        printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
        printf("                 ");
        printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b successfully.\r\n");
#else
        // 当前逻辑不在上电检索文件的时候格式整个空间,而只格式一个头部。
        // 必须要这个逻辑，因为在升级过程的中断，往往是头部不存在，而后续有内容。而第一次写入时，并不想照顾这个逻辑。
        res = __formatfilehead(core, file);
#endif
        return (res); // 当前系统已无文件，后续逻辑不执行
    }

    // 将内容接入文件系统
//    res = of_virtualize(core->root, &file->basic, structFileHead.name);
//    if(res)
    if(!obj_newchild(core->root, NULL, O_RDWR, (ptu32_t)file, structFileHead.name))
    {
        free(file);
        return (-1);
    }

    printf("\r\n: info : iapfs  : valid file found, name(%s), size(%dKB).", structFileHead.name, (file->sz>>10));
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
    if(file->status == IAP_UPDATING)
        return;

//    of_unlinkbasic(&file->basic);
//    Lock_MutexDelete(file->lock);
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
    if(__ll_erase(core->vol, FILE_HEAD_SIZE, __locatefilehead(file)))
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
    struct __icontext *cx;

    cx = malloc(sizeof(*cx) + core->bufsz);
    if(!cx)
        return (NULL);

    cx->pos = 0;
//    cx->bufs = (u8*)cx + sizeof(*cx);
//    cx->bufc = cx->bufs;
//    cx->bufe = cx->bufs + core->bufsz;
    cx->buf = (u8*)cx + sizeof(*cx);
    memset(cx->buf, 0xFF, core->bufsz);
    cx->bufed = 0;
    return (cx);
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
// 参数：ob -- 文件对象；
//      flags -- 文件操作标志位；
//      uncached -- 需新建的文件名称；
// 返回：成功（IAP文件上下文）；失败（NULL）；
// 备注：
// ============================================================================
static struct objhandle *__iap_open(struct obj *ob, u32 flags, char *uncached)
{
    s32 size;
    struct objhandle *hdl;
    struct obj *tmp;
    struct __ifile *file = NULL;
    struct __icontext *cx = NULL;
    struct __icore *core = (struct __icore*)corefs(ob);

    __lock(core);
    if((!uncached)&&(obj_isset(ob))) // 根目录
    {
        if(!test_directory(flags))
        {
            __unlock(core);
            return (NULL);
        }
    }

    if(test_directory(flags)) // 目录逻辑
    {
        if(uncached)// 不支持新建目录
        {
            printf("\r\n: info : iapfs  : do not support create directory.");
            __unlock(core);
            return (NULL);
        }
    }
    else // 文件的逻辑
    {
        if(uncached) // 文件不存在，需要新建。（文件都是已缓存的）
        {
            if(!test_creat(flags))
            {
                __unlock(core);
                return (NULL); // 打开操作中无新建要求，则返回不存在；
            }

            // 判断iap文件系统是不是已经有文件了，如果存在是否正在使用；
            tmp = obj_child(core->root);
            if(tmp)
            {
                if(obj_isquoted(tmp))
                {
                    printf("\r\n: info : iapfs  : cannot create new for old are using.");
                    __unlock(core);
                    return (NULL);
                }

                if(obj_del(tmp))
                {
                    printf("\r\n: info : iapfs  : cannot create new for old cannot delete.");
                    __unlock(core);
                    return (NULL);
                }
            }

            file = __newfile(core);
            if(!file)
            {
                __unlock(core);
                return (NULL);
            }

            __formatfilehead(core, file);
#if 0
            if(of_virtualize(ob, &file->basic, uncached)) // IAP文件，链入文件系统
                return (NULL);
#else
            if(!obj_newchild(core->root, NULL, O_RDWR, (ptu32_t)file, uncached))
            {
                __unlock(core);
                return (NULL);
            }
#endif
        }
        else // 文件已存在
        {
            if(test_onlycreat(flags))
            {
                __unlock(core);
                return (NULL); // 必须新建逻辑，但文件已存在
            }

            file = (struct __ifile*)obj_GetPrivate(ob);
            if(test_trunc(flags))
            {
                if(-1 == __formatfilehead(core, file))
                {
                    __unlock(core);
                    return (NULL);
                }

                file->sz = 0;
                file->status = IAP_UPDATING;
            }
        }

        cx = __newcontext(core);
        if(!cx)
        {
            if(uncached)
                __freefile(file); // 释放掉上面创建的

            __unlock(core);
            return (NULL);
        }

        if(test_append(flags))
        {
            cx->pos = file->sz;
        }

        // 预缓存逻辑
        if(cx->pos<core->inhead)
        {
            size = core->inhead;
        }
        else
        {
            size = core->bufsz;
        }

        if(__ll_read(core->vol, cx->buf, size, (cx->pos+file->cxbase)))
        {
            if(uncached)
                __freefile(file); // 释放掉上面创建的

            __unlock(core);
            return (NULL);
        }
    }

    hdl = handle_new();
    if(!hdl)
    {
        __freecontext(cx);
        if(uncached)
            __freefile(file); // 释放掉上面创建的
    }

    handle_init(hdl, NULL, flags, (ptu32_t)cx);
    __unlock(core);
    return (hdl);
}

// ============================================================================
// 功能：关闭文件
// 参数：hdl -- IAP文件对象句柄；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __iap_close(struct objhandle *hdl)
{
    s32 size;
    struct __icore *core;
    struct __ifile *file;
    struct __icontext *cx = (struct __icontext*)handle_context(hdl);

    if(cx) // NULL时表示目录
    {
        core = (struct __icore*)handle2sys(hdl);
        file = (struct __ifile*)handle_val(hdl);
        __lock(core);
        if(IAP_UPDATED != file->status) // 数据存在写入操作或者文件是新建的
        {
            if(cx->bufed)
            {
                if(cx->pos<=core->inhead)
                    size = core->inhead;
                else
                    size = core->bufsz;

                if(-1==__ll_write(core->vol, cx->buf, size, (cx->pos-cx->bufed+file->cxbase)))
                {
                    __unlock(core);
                    return (-1);
                }
            }

            if(!iscontender(hdl)) // 最后一个文件使用者关闭文件时，才会设置文件头
            {
                if(__makefilehead(core, file, handle_name(hdl)))
                {
                    __unlock(core);
                    return (-1);
                }
            }
        }

        __unlock(core);
    }

    __freecontext(cx);
    handle_free(hdl);
    return (0);
}

// ============================================================================
// 功能：写文件
// 参数：hdl -- IAP文件对象句柄；
//      data -- 需写入数据的空间；
//      size -- 需写入数据的大小；
// 返回：实际写入的字节数；
// 备注：
// ============================================================================
static s32 __iap_write(struct objhandle *hdl, u8 *data, u32 size)
{
    s32 pos, once, free, res, left = size;
    struct __icontext *cx = (struct __icontext *)handle_context(hdl);
    struct __icore *core = (struct __icore*)handle2sys(hdl);
    struct __ifile *file = (struct __ifile*)handle_val(hdl);

    __lock(core);
    while(left)
    {
        once = left;
        if(cx->pos<core->inhead)
            free = core->inhead - cx->bufed; // 在开始的区域中，文件头部占据了固定空间；
        else
            free = core->bufsz - cx->bufed;

        if(once>free)
            once = free;
#if 0
        // 数据校验，已写入的数据不可改写；
        for(i=0; i<once; i++)
        {
            if((0xFF!=cx->buf[cx->bufed+i])&&(cx->buf[cx->bufed+i]!=data[i]))
            {
                printf("\r\n: erro : iapfs  : write the written area is no supported.");
                return (size-left);
            }
        }
#endif
        memcpy(cx->buf+cx->bufed, data, once);
        cx->bufed += once;
        if((cx->bufed==core->bufsz) || // 缓冲已满，刷入
           ((cx->bufed==core->inhead)&&((cx->pos<core->inhead)))) // 文件头部占据了固定空间，此情况也满（对齐）
        {
            pos = cx->pos - (cx->bufed - once) + file->cxbase; // 当前位置写入时的位置，缓存对齐了的；
            res = __ll_write(core->vol, cx->buf, cx->bufed, pos);// 此时bufed的大小为bufsz或者inhead；
            if(-1==res) // 写错误；
            {
                break;
            }
            else if(-2==res) // 将要没有可写空间，删除一些；
            {
                if(__ll_erase(core->vol, core->bufsz, (pos+cx->bufed+1)))// '+1'表示下一个要写的空间地址；
                    break;
            }

            // 缓存重置，将后续的缓存预取进来（第一个head区域是不会在这里需要缓存的）
            if(__ll_read(core->vol, cx->buf, core->bufsz, pos+cx->bufed))
                break;

            cx->bufed = 0;
        }

        left -= once;
        data += once;
        cx->pos += once;
    }

    if(file->sz<cx->pos)
        file->sz = cx->pos;

    if((IAP_UPDATING!=file->status)&&(left!=size))
        file->status = IAP_UPDATING; // 文件数据发生变改变

    __unlock(core);
    return (size-left);
}

// ============================================================================
// 功能：读文件
// 参数：hdl -- IAP文件对象句柄；
//      data -- 需读出数据的存放空间；
//      size -- 需读出数据的大小；
// 返回：实际读出的单元数；
// 备注：
// ============================================================================
static s32 __iap_read(struct objhandle *hdl, u8 *data, u32 size)
{
    s32 pos, once, left = size;
    struct __icontext *cx = (struct __icontext*)handle_context(hdl);
    struct __icore *core = (struct __icore*)handle2sys(hdl);
    struct __ifile *file = (struct __ifile*)handle_val(hdl);

    __lock(core);
    if((cx->pos+size)>file->sz)
        size = file->sz - cx->pos; // 不能读越界

    if(cx->pos<core->inhead)
        once = core->inhead - cx->bufed; // 在开始的区域中，文件头部占据了固定空间；
    else
        once = core->bufsz - cx->bufed;

    if(once>size)
    {
        // 这里会存在一个隐患，就是读的数据可能和介质中的不一样，
        // 不执行这个逻辑，效率就差，而且在ecc上可能会有麻烦；
        memcpy(data, cx->buf+cx->bufed, size);
        cx->pos += size;
        cx->bufed += size;
        return (size);
    }

    // 缓冲的剩余空间容纳不了回读的数据，则先将数据刷下去，再回读。
    pos = cx->pos - cx->bufed + file->cxbase; // 缓存对齐
    if(-1==__ll_write(core->vol, cx->buf, (cx->bufed+once), pos)) // 缓存整体写
    {
        __unlock(core);
        return (0); // 缓存数据刷入失败；
    }

    memcpy(data, cx->buf+cx->bufed, once); // 将缓存上数据线提取出来；
    cx->pos += once; // 此后pos逻辑上是缓存对齐的；
    left -= once;
    data += once;
    cx->bufed += once;
    while(left)
    {
        cx->bufed = 0;
        if(__ll_read(core->vol, cx->buf, core->bufsz, cx->pos+file->cxbase))
        {
            __unlock(core);
            return (size - left); // 缓存数据刷入失败；
        }

        if(left>core->bufsz)
            once = core->bufsz;
        else
            once = left;

        memcpy(data, cx->buf, once);
        data += once;
        cx->bufed += once;
        left -= once;
        cx->pos += once;
    }

    __unlock(core);
    return (size);
}

// ============================================================================
// 功能：设置文件当前位置；
// 参数：hdl -- IAP文件对象句柄；
//      offset -- 移动位置的量；
//      whence -- 移动位置的起点；
// 返回：成功（0，*pOffset指向新的当前位置）；失败（-1）；
// 备注：
// ============================================================================
static s32 __iap_seek(struct objhandle *hdl, s64 *offset, s32 whence)
{
    struct __icontext *cx;
    struct __ifile *file;
    struct __icore *core;
    s32  npos, movs, pos;

#if 0 // 应该由上级逻辑判断
    if(isdirectory(hdl))
    {
        printf("\r\n: dbug : iapfs  : cannot seek directory.");
        return (-1);
    }
#endif

    core = handle2sys(hdl);
    cx = (struct __icontext*)handle_context(hdl);
    //file = dListEntry(of_basic(hdl), struct __ifile, basic);
    file = (struct __ifile*)handle_val(hdl);
    __lock(core);
    switch(whence)
    {
        case SEEK_END: // 转为从头SEEK逻辑
        {
            *offset = file->sz + *offset;
            if(*offset<0)
                *offset = 0; // 新位置越界了
        }

        case SEEK_SET: // 转为当前位置的SEEK的逻辑
        {
            *offset = *offset - cx->pos;
            if((*offset+cx->pos)<0)
                *offset = cx->pos; // 新位置越界了
        }

        case SEEK_CUR:
        {
            npos = *offset + cx->pos;
            if(npos<0)
                npos = 0;

            *offset = npos;
            // 如果是在缓存范围内的移动，则直接返回；
            movs = npos - cx->pos; // 移动的方向；
            if(movs<0)
            {
                if((movs+cx->bufed)>=0)
                {
                    cx->bufed += movs;
                    cx->pos = npos;
                    return (0);
                }
            }
            else if(movs>0)
            {
                if((movs+cx->bufed)<core->bufsz)
                {
                    cx->bufed += movs;
                    cx->pos = npos;
                    return (0);
                }
            }
            else // 不需要移动；
            {
                return (0);
            }

            // 如果超出在缓存范围内的移动，将缓存刷入介质
            pos = cx->pos - cx->bufed + file->cxbase;
            if(cx->pos<core->inhead)
            {
                if(-1==__ll_write(core->vol, cx->buf, core->inhead, pos))
                    return (-1);
            }
            else
            {
                if(-1==__ll_write(core->vol, cx->buf, core->bufsz, pos))
                    return (-1);
            }

            cx->bufed = 0;
            // 缓存新的位置
            if(npos<core->inhead)
            {
                if(__ll_read(core->vol, cx->buf, core->inhead, file->cxbase))
                    return (-1);

                cx->bufed = npos;
            }
            else
            {
                pos = npos - (npos + file->cxbase) % core->bufsz + file->cxbase;
                if(__ll_read(core->vol, cx->buf, core->bufsz, pos))
                    return (-1);

                cx->bufed = (npos - core->inhead) % core->bufsz;
            }

            cx->pos = npos;
            return (0);
        }

        default:
        {
            break;
        }
    }

    return (-1);
}

// ============================================================================
// 功能：删除文件；
// 参数：ob -- IAP文件对象；
// 返回：成功（0）；失败（-1）；
// 备注：未考虑互斥；当pName为NULL时，表示文件正在被使用；
// ============================================================================
static s32 __iap_remove(struct obj *ob)
{
    struct __ifile *file;
    struct __icore *core;

    core = (struct __icore *)corefs(ob);
    file = (struct __ifile*)obj_GetPrivate(ob);
    return (__delfile(core, file));
}

// ============================================================================
// 功能：文件查询
// 参数：ob -- IAP文件对象；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __iap_stat(struct obj *ob, struct stat *data)
{
    struct __ifile *file;

    //if(GROUP_POINT != __OBJ_Type(ob))
    if(obj_isset(ob))
    {
        data->st_size = 0;
        data->st_mode = S_IFDIR;
    }
    else
    {
        file = (struct __ifile*)obj_GetPrivate(ob);
        data->st_size = file->sz;
        data->st_mode = S_IFREG;
    }

    return (0);
}

// ============================================================================
// 功能：读IAP文件系统目录项；
// 参数：hdl -- IAP文件对象句柄；
//      dentry -- 目录项；
// 返回：全部读完（1）；失败（-1）；读了一项（0）；
// 备注：
// ============================================================================
static s32 __iap_readdentry(struct objhandle *hdl, struct dirent *dentry)
{
    struct obj *ob = handle2obj(hdl);

    ob = obj_child(ob);
    if((ob)&&(dentry->d_ino!=(long)ob))
    {
        dentry->d_ino = (long)ob;
        strcpy(dentry->d_name, obj_name(ob));
        dentry->d_type = DIRENT_IS_REG;
    }

    return (1);
}

// ============================================================================
// 功能：安装IAP文件系统
// 参数：super -- 文件系统管理信息；
//      opt -- 安装逻辑标志位；
//      config -- 自定义数据；
// 返回：成功(0)； 失败(-1)。
// 备注：
// ============================================================================
static s32 __iap_fs_install(tagFSC *super, u32 opt, void *config)
{
    s32 res;
    struct __icore *core;
    struct umedia *um;
    extern u32 gc_pAppRange; // 来自于LDS定义
    extern u32 gc_pAppOffset; // 来自于LDS定义

    core = malloc(sizeof(*core));
    if(!core)
    {
        printf("\r\n: erro : iapfs  : install failed(memory out).");
        return (-1);
    }

    memset(core, 0x0, sizeof(*core));
    um = (struct umedia*)devo2drv(super->pDev);
    if(!um)
    {
        free(core);
        return (-1);
    }

    core->vol = (void*)um;
    core->bufsz = 1 << um->usz; // iap文件系统文件的缓存大小依据unit的尺寸；
    if(core->bufsz<FILE_HEAD_SIZE)
    {
        free(core);
        return (-1);
    }

    core->inhead = core->bufsz - FILE_HEAD_SIZE;
    core->root = super->pTarget;
    res = __scanfiles(core); // 扫描已存在文件
    if(res)
    {
        free(core);
        return (-1);
    }

    core->lock = Lock_MutexCreate("iap fs");
    if(!core->lock)
    {
        free(core);
        return (-1);
    }

    super->pCore = (void*)core;
    return (0);
}

// ============================================================================
// 功能：IAP文件操作接口
// 参数：标准逻辑，查看接口说明；
// 返回：标准逻辑，查看接口说明；
// 备注:
// ============================================================================
static ptu32_t __iap_ops(enum objops ops, ptu32_t o_hdl, ptu32_t args,  ...)
{
    va_list list;

    switch(ops)
    {
        case OBJOPEN:
        {
            char *new;
            struct obj *ob = (struct obj*)o_hdl;
            u32 flags = (u32)args;

            va_start(list, args);
            new = (char*)va_arg(list, u32);
            va_end(list);

            return ((ptu32_t)__iap_open(ob, flags, new));
        }

        case OBJCLOSE:
        {
            struct objhandle *hdl = (struct objhandle*)o_hdl;
            return ((ptu32_t)__iap_close(hdl));
        }

        case OBJCHILDS:
        {
            struct objhandle *hdl = (struct objhandle*)o_hdl;
            struct dirent *ret = (struct dirent *)args;

            return((ptu32_t)__iap_readdentry(hdl, ret));
        }

        case OBJREAD:
        {
            u32 len;
            struct objhandle *hdl = (struct objhandle*)o_hdl;
            u8 *data = (u8*)args;

            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__iap_read(hdl, data, len));
        }

        case OBJWRITE:
        {
            u32 len;
            struct objhandle *hdl = (struct objhandle*)o_hdl;
            u8 *data = (u8*)args;

            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__iap_write(hdl, data, len));
        }

        case OBJDEL:
        {

            struct obj *ob = (struct obj*)o_hdl;
            char *notExist = (char*)args;

            if(notExist)
                return ((ptu32_t)-1);

            return ((ptu32_t)__iap_remove(ob));
        }

        case OBJSEEK:
        {
            s32 whence;
            struct objhandle *hdl = (struct objhandle *)o_hdl;
            s64 *offset = (s64*)args;

            va_start(list, args);
            whence = (s32)va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__iap_seek(hdl, offset, whence));
        }

        case OBJSTAT:
        {
            struct stat *data = (struct stat*)args;
            struct obj *ob = (struct obj*)o_hdl;
            char *path;

            va_start(list, args);
            path = (char*)args;
            va_end(list);

            if(path&&('\0'!=*path))
                return (-1); // 查询的文件不存在；

            return ((ptu32_t)__iap_stat(ob, data));
        }

        default:
        {
            printf("\r\n: dbug : iapfs  : do not support this operation now.");
            break;
        }
    }

    return (-1);
}

// ============================================================================
// 功能：安装IAP文件系统
// 参数：target -- 安装目录；pDev -- 安装设备；
// 返回：失败(-1)； 成功(0)。
// 备注:
// ============================================================================
s32 ModuleInstall_IAP_FS(const char *target, const char *source, u32 opt)
{
    s32 res;
    const char *dpath = "/iboot"; // 默认安装目录

    if(target)
        target = dpath;

    if(!source)
    {
        printf("\r\n: dbug : module : cannot register \"IAP\"(no source).");
        return (-1);
    }

    res = regfs(&typeIAP);
    if(-1==res)
    {
        printf("\r\n: dbug : module : cannot register \"IAP\"(file system type).");
        return (-1); // 失败;
    }

    res = mkdir(target, S_IRWXU); // 创建安装目录 // 创建目录
    if(-1==res)
    {
        printf("\r\n: dbug : module : mount \"IAP\" failed, cannot create \"%s\"(target).", target);
        return (-1);
    }

    res = mountfs(source, target, "IAP", 0, 0);
    if(res<0)
    {
        printf("\r\n: dbug : module : mount \"IAP\" failed, cannot install.");
        remove(target);
        return (-1);
    }

    printf("\r\n: info : module : file system \"IAP\" installed on \"%s\".", source);
    return (0);
}

