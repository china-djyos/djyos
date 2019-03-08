//----------------------------------------------------
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
// 该简易文件系统太过复杂，需要重构，暂时屏蔽该文件系统


#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <misc.h>
#include <stdarg.h>
#include <systime.h>
#include <lock.h>
#include <dirent.h>
#include <objhandle.h>
#include <stdio.h>
#include <device/include/unit_media.h>
#include "../filesystems.h"
#include "component_config_efs.h"
#include "efs.h"
//
//
//
s32 e_operations(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                 ptu32_t OpsArgs2, ptu32_t OpsArgs3);


// ============================================================================
// 功能：解锁文件系统；
// 参数：core -- 文件系统控制；
// 返回：
// 备注：
// ============================================================================
inline static void __unlock(struct __ecore *core)
{
    Lock_MutexPost(core->mutex);
}
// ============================================================================
// 功能：锁定文件系统；
// 参数：core -- 文件系统控制；
// 返回：
// 备注：
// ============================================================================
inline static void __lock(struct __ecore *core)
{
    Lock_MutexPend(core->mutex, CN_TIMEOUT_FOREVER);
}

// ============================================================================
// 功能：通过ECC校验和修复数据；
// 参数：core -- 文件系统控制结构；
//      data -- 数据；
//      len -- 数据长度；
//      ecc -- 数据的ECC值；
// 返回：成功（0）；失败（-1）；
// 备注：不能超过256个字节；
// ============================================================================
static s32 __fix(struct __ecore *core, void *data, u32 len, u32 *ecc)
{
    u8 *buf;

    if(len > 256)
        return (-1);

    if(len < 256)
    {
        memset(core->ecc, 0x0, 256);
        memcpy(core->ecc, data, len);
        buf = core->ecc;
    }
    else
    {
        buf = data;
    }

    if(hamming_verify_256x(buf, 256, ((const u8*)ecc)))
    {
        // printf("\r\n: erro : efs    : data ecc error.");
        return (-1);
    }

    return (0);
}

// ============================================================================
// 功能：生成ECC；
// 参数：core -- 文件系统控制结构；
//      data -- 数据；
//      len -- 数据长度；
//      ecc -- 数据的ECC值；
// 返回：成功（0）；失败（-1）；
// 备注：不能超过256个字节；
// ============================================================================
static s32 __gen(struct __ecore *core, void *data, u32 len, u32 *ecc)
{
    u8 *buf;

    if(len > 256)
        return (-1);

    if(len < 256)
    {
        memset(core->ecc, 0x0, 256);
        memcpy(core->ecc, data, len);
        buf = core->ecc;
    }
    else
    {
        buf = (u8*)data;
    }

    *ecc = 0;
    hamming_compute_256x(buf, 256, (u8*)ecc);
    return (0);
}

// ============================================================================
// 功能：为文件大小添加一个新空间
// 参数：list -- loc链表；
//      loc -- 物理位置偏置；
//      order -- 序号标记；
//      sort -- 0（新成员在接list后）；追加；
//              1（链表本身是按从小到大排列的，新成员也按序插入，list参数在函数返回时，指向最小序号成员；按序；
//
// 返回：成功（新加入的loc）；失败（NULL）；
// 备注：输入的list可以指向空，表示没有队列；
// ============================================================================
static struct __loc *__addloc(struct __loc **list, u32 loc, u32 order, u8 sort)
{
    list_t *start, *cur;
    struct __loc *nloc, *cloc;

    nloc = (struct __loc*)malloc(sizeof(struct __loc));
    if(!nloc)
    {
        printf("\r\n: error: efs    : add location failed(memory out).");
        return (NULL);
    }

    dListInit(&nloc->list);
    nloc->order = order;
    nloc->loc = loc;
    if(list) // 没有队列，则直接返回新申请的loc
    {
        if(!*list)
        {
            *list = nloc; // 输入的队列指针为空；
        }
        else if(sort) // 按序插入
        {
            // 链表本身是按从小到大排列的，新成员也按序插入，list参数在函数返回时，指向最小序号成员；
            // 此时队列头是队列的最小序号；
            start = &(*list)->list;
            cloc = dListEntry(start, struct __loc, list); // 第一个成员；
            if(cloc->order>=order) // 第一个成员大于新插入的新成员；则第一个成员变更为新成员；
            {
                // 直接将新成员插入到头部前面，然后变更头部指向；
                dListInsertBefore(start, &nloc->list);
                *list = nloc; // 头部指向最小成员
                return (nloc);
            }

            cloc = dListEntry(dListGetBefore(start), struct __loc, list); // 最后一个成员；
            if(cloc->order<=order) // 最后一个成员小于新插入的新成员；则新成员直接追加其后面；
            {
                dListInsertAfter(&cloc->list, &nloc->list);
                return (nloc);
            }

            dListForEach(cur, start) // 在队列中查找新成员的插入位置；
            {
                cloc = dListEntry(cur, struct __loc, list);
                if(cloc->order>=order)
                {
                    dListInsertBefore(cur, &nloc->list);
                    return (nloc);
                }
            }
        }
        else // 追加插入
        {
            dListInsertAfter(&((*list)->list), &(nloc->list)); // 新成员接在list后
        }
    }

    return (nloc);
}

// ============================================================================
// 功能：查找offset对应的loc区域；
// 参数：core -- 文件系统；
//      offset -- 文件的位置；-1表示获取文件尾部；
// 返回：成功（offset对应的loc区）；失败（NULL）；
// 备注：head必须在调用逻辑保证；
// ============================================================================
static struct __loc *__getloc(enum locmove movon, struct __ecore *core, struct __loc *loc, s64 offset)
{
    list_t *ret;
    s32 movs;
    struct __loc *cloc;
    s64 eccesz;

    if(position==movon)
    {
        if(-1 == offset)
        {
            ret = dListGetBefore(&(loc->list));
        }
        else
        {
            eccesz = (1 << (core->media->usz - BUFBITS)) * 4; // 一个unit内的ECC数据量；
            eccesz = (core->fsz << core->media->usz) - (core->fsz * eccesz); // 除去ECC数据的文件空间大小；
            movs = offset / eccesz;
            ret = &(loc->list);
            while(movs--)
            {
                ret = dListGetAfter(ret);
                if(ret == &(loc->list))
                {
                    if((!movs)&&(!(offset%eccesz))) // 文件位置正好在文件尾部，且此时尺寸正好与文件空间对齐；
                        ret = dListGetBefore(&(loc->list));
                    else
                        return (NULL);
                }
            }
        }
    }
    else if(direct==movon)
    {
        if(offset<0)
        {
            ret = dListGetBefore(&(loc->list));
        }
        else
        {
            ret = dListGetAfter(&(loc->list));
        }
    }

    cloc = dListEntry(ret, struct __loc, list);
    return (cloc);
}
// ============================================================================
// 功能：删除loc；
// 参数：end -- loc链表成员；
//      ward -- 返回成员,向前（-1）；向后（1）；
// 返回：返回操作后的loc链表的相邻一个；全部删除，则返回NULL
// 备注：
// ============================================================================
static struct __loc *__delloc(struct __loc *end, s8 ward)
{
    list_t *loc;
    struct __loc *res;

    if(dListIsEmpty(&(end->list)))
    {
        res = NULL;
    }
    else if(1==ward) //向后
    {
        loc = dListGetAfter(&(end->list));
        res = dListEntry(loc, struct __loc, list);
        dListRemove(&(end->list));
    }
    else if(-1==ward) // 向前
    {

        loc = dListGetBefore(&(end->list));
        res = dListEntry(loc, struct __loc, list);
        dListRemove(&(end->list));
    }

    free(end);
    return (res);
}

// ============================================================================
// 功能：通过buf构建file信息；
// 参数：core -- 文件系统控制结构；
//      file -- 文件信息；
//      buf -- 文件信息所在的空间；
// 返回：
// 备注：
// ============================================================================
static inline void __e_structfile(struct __ecore *core, struct __efstruct *file, u8 *buf)
{
    file->name.n = (char*)buf;
    file->name.e = (u32*)(buf+core->nsz);
    file->size = (struct __esize*)(buf+core->nsz+4);
}

// ============================================================================
// 功能：检查文件loc链表的有效性
// 参数：core -- 文件系统控制结构；
//      loc -- 文件内容的loc链表；
//      name -- 文件名；
// 返回：正常（0）；有问题（-1）；
// 备注：检查loc链表是否按序，是否存在同序号;（减少链表检查需要提高key的唯一性）；
// ============================================================================
static void __e_sanityloc(struct __ecore *core, struct __loc **loc, const char *name)
{
    s64 units;
    u8 delsub = 0;
    u8 found;
    u8 one2one = 0;
    struct __efstruct fstruct;
    struct __loc *nloc, *sub;

    sub = *loc;
    if(1!=sub->order) // 文件序号不以1位开头，表示文件数据丢失了
    {
        printf("\r\n: warn : efs    : file (%s)'s location is not sanity, the 1 to %d has lost", name,
                (sub->order));

        while(sub)
            sub = __delloc(sub, 1);

        *loc = NULL;
        return;
    }

    // 按序号进行分段(sub)解析；
    // 例如：loc （1-1-1-1）--（2-2-2）--（3-3）--（4）
    //           sub         sub        sub     sub
    do
    {   // __SUB_LOC_SCAN:
        nloc = __getloc(direct, core, sub, 1);
        if((nloc->order==sub->order)&&(nloc!=(*loc))) // sub中存在order相同的loc，需要详细甄别sub中每个成员
        {
            if(one2one)
            {
                printf("\r\n: warn : efs    : file (%s)'s location is not sanity(sub order %d should be single).",
                       name, sub->order);
                one2one = 0;
                // TODO
            }

            units = core->serial * core->ssz + core->finfo + sub->loc;
            if(core->drv->efs_read_media(core, units, core->ebuf)) // 文件信息
            {
                printf("\r\n: dbug : efs    : file (%s)'s location is not sanity, info (%d) can not read",
                       name, sub->loc);
                found = 0;
                delsub = 1;
            }
            else
            {
                __e_structfile(core, &fstruct, core->ebuf);
                if(strcmp(fstruct.name.n, name))
                {
                    __fix(core, fstruct.name.n, (strlen(name)+1), fstruct.name.e);
                    if(strcmp(fstruct.name.n, name))
                    {
                        delsub = 1;
                        found = 0;
                    }
                    else
                    {
                        found = 1;
                    }
                }
                else
                {
                    found = 1;
                }
            }

            while((nloc->order==sub->order)&&(nloc!=(*loc)))
            {
                if(found) // 已找到，则删除其他同order的loc
                {
                    nloc = __delloc(nloc, 1); // 删除当前，并返回后一个成员；
                }
                else
                {
                    units = core->serial * core->ssz + core->finfo + nloc->loc;
                    if(core->drv->efs_read_media(core, units, core->ebuf)) // 文件信息
                    {
                        printf("\r\n: dbug : efs    : file (%s)'s location is not sanity, info (%d) can not read",
                               name, nloc->loc);
                        nloc = __delloc(nloc, 1); // 删除当前，并返回后一个成员；
                        continue;
                    }
                    else
                    {
                        __e_structfile(core, &fstruct, core->ebuf);
                        if(strcmp(fstruct.name.n, name))
                        {
                            __fix(core, fstruct.name.n, (strlen(name)+1), fstruct.name.e);
                            if(strcmp(fstruct.name.n, name))
                            {
                                nloc = __delloc(nloc, 1); // 删除当前，并返回后一个成员；
                                continue;
                            }
                        }

                        found = 1;
                        nloc = __getloc(direct, core, nloc, 1);
                    }
                }
            }
        }
        else if(!one2one)
        {
            // sub中有多个后，变为1个时，会检查一次；；
            // 或者开始时，第一个sub只有一个时，也会检查一次；
            units = core->serial * core->ssz + core->finfo + sub->loc;
            if(core->drv->efs_read_media(core, units, core->ebuf)) // 文件信息
            {
                printf("\r\n: dbug : efs    : file (%s)'s location is not sanity, info (%d) can not read",
                       name, sub->loc);
                delsub = 1;
                found = 0;
            }
            else
            {
                __e_structfile(core, &fstruct, core->ebuf);
                if(strcmp(fstruct.name.n, name))
                {
                    __fix(core, fstruct.name.n, (strlen(name)+1), fstruct.name.e);
                    if(strcmp(fstruct.name.n, name))
                    {
                        delsub = 1;
                        found = 0;
                    }
                    else
                    {
                        one2one = 1; // 只要一组sub里面不再后同号order,后续的就不应该再有同号（逻辑上）；
                        // nloc = __getloc(direct, core, nloc, 1);
                        found = 1;
                    }
                }
                else
                {
                    one2one = 1; // 只要一组sub里面不再后同号order,后续的就不应该再有同号（逻辑上）；
                    // nloc = __getloc(direct, core, nloc, 1);
                    found = 1;
                }
            }
        }
        else // one2one = 1 (非第一个)
        {
            found = 1; // 不作检查了（检查太多影响效率）
            // nloc = __getloc(direct, core, nloc, 1);
        }

        if(found) // sub中找到文件需要的loc
        {
            if(delsub) // 删除sub后，sub应该指向的是这组sub中文件对应的loc
            {
                delsub = 0;
                if(nloc==sub)
                    nloc = __getloc(direct, core, nloc, 1);

                if(sub==*loc) // 第一组sub
                {
                    sub = __delloc(sub, 1);
                    *loc = sub;
                }
                else // 后续组sub
                {
                    sub = __delloc(sub, 1);
                }
            }

            if((nloc->order==(sub->order+1)) // 符合sub的order逻辑
               ||(nloc==(*loc))) // 结束
            {
                sub = nloc;
                // goto __SUB_LOC_SCAN; // 执行下一组
            }
            else // 不符合sub的order逻辑；
            {
                printf("\r\n: dbug : efs    : file (%s)'s location is not sanity, %d to %d has lost.",
                        name, sub->order, nloc->order);
                #if 1
                while(nloc!=(*loc)) // 删除后续链表
                    nloc = __delloc(nloc, 1);
                #else // 不删除链表，继续检查（防止断链后的数据删除不了，但如此考虑因素太多，好复杂）
                if(one2one)
                    one2one = 0;

                sub = nloc;
                goto __SUB_LOC_SCAN; // 执行下一组
                #endif
            }
        }
        else // sub组中未找到文件需要的loc，删除链表剩余的loc成员；
        {
            if(sub==(*loc)) // 全部都不符合
            {
                while(sub)
                    sub = __delloc(sub, 1);

                *loc = NULL;
            }
            else
            {
                while(sub!=(*loc))
                    sub = __delloc(sub, 1);
            }
        }
    }
    while(sub&&(sub!=(*loc)));
}

// ============================================================================
// 功能：生成文件key值；
// 参数：name -- 文件名；
//      key -- 文件key值；
// 返回：
// 备注：键值计算原理：字符串的前3个字符，和后3个字符；如果字符长度不够，重复最后一个字符；
//      test0的key为tesst0
// ============================================================================
static void __key(const char *name, u8 *key)
{
    s16 i, j, offset, len = strlen(name);

    for(i = 0, j = 0; i< 3; )
    {
        key[i] = name[j];

        if((++i) < len)
            j++;
    }

    if(len > 3)
    {
        offset = len - 3;
        len = 3; //
    }
    else
    {
        offset = 0;
    }

    for(i = 0, j = 0; i< 3; )
    {
        key[i+3] = name[j+offset];

        if((++i) < len)
            j++;
    }
}

// ============================================================================
// 功能：校验key值；
// 参数：core -- 文件系统控制结构；
//      idx -- 索引；
//      key -- key值；
// 返回：匹配（0）；不匹配（1）；空闲数据（2）；数据被破快且无法修复（-1）；
// 备注：
// ============================================================================
static inline s32 __e_matchkey(struct __ecore *core, struct __eidx *idx, u8 *key)
{
    u8 i;
    struct __eidx recheck;

    for(i = 0; i < sizeof(*idx); i++)
    {
        if(0xFF != ((u8*)idx)[i])
            break;
    }

    if(i == sizeof(*idx))
        return (2); //

    for(i = 0; i < KEYLEN;)
    {
        if((idx->key[i] == idx->bakup[i]))
        {
            if(idx->key[i] != key[i])
                return (1); // 假设是两个同时被改了，修复不了。认为它不匹配的可能性更大

            i++;
            continue;
        }
        else
        {
            // 数据出错了，尝试修复
            if((idx->key[i] == key[i]) ||
               (idx->bakup[i] == key[i]))
            {
                 // 当中只有一个都key匹配，数据肯定是被改了
                for(i = 0; i < KEYLEN; i++)
                    recheck.key[i] = key[i];

                for(i = 0; i < KEYLEN; i++)
                    recheck.bakup[i] = key[i];

                recheck.order = idx->order;
                recheck.ecc = 0;
                __gen(core, &recheck, sizeof(struct __eidx), &recheck.ecc);
                if(recheck.ecc != idx->ecc)
                    return (-1); // 包括ecc在内，也不一样，无法修复了

                __fix(core, (u8*)idx, sizeof(*idx), &(idx->ecc)); // 尝试修复
            }
            else
            {
                // key和bakup都与输入的key不一样，不匹配的可能性大。
                return (1);
            }
        }

        break;
    }

    if(i < KEYLEN)
        return (-1); //

    return (0);
}

// ============================================================================
// 功能：回收整理文件系统；
// 参数：core -- 文件系统控制结构；
// 返回：成功（0）；失败（-1）；
// 备注：触发回收条件：文件信息空间file size使用完、没有空闲空间
// ============================================================================
static s32 __e_recycle(struct __ecore *core)
{
    struct __eidx *idx;
    struct __ecstruct *estruct;
    struct __efstruct fstruct;
    s64 units;
    s32 i, j, k, idxs, sizes, res;
    u8 new, *idxbit;

    // 文件数量用位表示，需要占多少个字节；
    if(core->fmax%8)
        sizes = core->fmax / 8 + 1;
    else
        sizes = core->fmax / 8;

    idxbit = malloc(sizes);
    if(!idxbit)
    {
        printf("\r\n: warn : efs    : recycle failed(memory out).");
        return (-1);
    }

    sizes = ((1 << core->media->usz) - (core->nsz + 4)) / sizeof(struct __esize); // 文件信息空间内可以存size次数;
    idxs = (1 << core->media->usz) / sizeof(*idx); // 一页中能存放多少个index；
    new = core->serial;
    __lock(core);

__RETRY_RECYCLE:
    if(++new == SYS_LOOPS)
        new = 0;

    if(new == core->serial)
    {
        printf("\r\n: erro : efs    : recycle failed(cannot erase all the rest).");
        free(idxbit);
        __unlock(core);
        return (-1); // 新区全部尝试完，文件系统不能再用了；
    }

    units = new * core->ssz;
    if(core->drv->efs_erase_media(core, units, core->ssz)) // 格式化下一个系统区，作为新区域；
    {
        printf("\r\n: warn : efs    : recycle(cannot erase %d).", new);
        goto __RETRY_RECYCLE;
    }

    // 整理目前使用的系统空间，并拷贝到next系统空间区；
    // 将系统信息更新到头部，此时是更新状态；
    units = core->serial * core->ssz;
    res = core->drv->efs_read_media(core, units, core->ebuf);
    if(res)
    {
        printf("\r\n: erro : efs    : recycle cannot read system area.");
        // 读失败没有关系，可以自己构建；
    }

    estruct = (struct __ecstruct*)core->ebuf;
    estruct->status = SYS_UPDATING;
    if(res || __fix(core, estruct, (sizeof(*estruct)-SYS_NOECCSIZE), &(estruct->ecc))) // 系统头部存在问题，直接构建；
    {
        printf("\r\n: dbug : efs    : recycle (bad estruct).");
        memset(estruct, 0xFF, (1<<core->media->usz));
        memcpy(estruct->signature, ESIGN, ESIG_LEN);
        estruct->age = 0xFFFFFFFF; // 当头部被破坏时的专用age；
        estruct->files = core->fmax;
        estruct->range = core->ASize;
        estruct->ecc = 0;
    }
    else
    {
        // 正常的系统头部，只增加头部age；
        if(estruct->age == 0xFFFFFFFE)
            estruct->age = 0; // 0xFFFFFFFF保留给不正常的estruct的修复逻辑；
        else
            estruct->age ++;
    }

    __gen(core, estruct, (sizeof(*estruct)-SYS_NOECCSIZE), &(estruct->ecc));
    units = new * core->ssz;
    res = core->drv->efs_write_media(core, units, estruct);
    if(res)
    {
        printf("\r\n: warn : efs    : recycle %d cannot write.", new);
        goto __RETRY_RECYCLE;
    }

    // 整理idx表；
    for(i=0; (u32)i<core->fmax;)
    {
        if(!(i%idxs)) // 读idx表；
        {
            units = core->serial * core->ssz + 1 + i / idxs;
            res = core->drv->efs_read_media(core, units, core->ebuf);
            if(res)
            {
                printf("\r\n: erro : efs    : recycle(cannot read backup).");
                free(idxbit);
                __unlock(core);
                return (-1);
            }

            idx = (struct __eidx *)core->ebuf;
        }

        for(j=0; (u32)j<sizeof(*idx); j++)
        {
            if((0!=((u8*)idx)[j]) && (0xFF!=((u8*)idx)[j]))
                break; // 有效的idx
        }

        k = i / 8; // bit位所在的字节；
        if(j==sizeof(*idx)) // 已废弃的idx，进一步重置该区域；
        {
            memset((u8*)idx, 0xFF, j);
            idxbit[k] |= (u8)(1 << (i % 8)); // 1表示idx不存在；
        }
        else
        {
            idxbit[k] &= (~(u8)(1 << (i % 8))); // 0表示idx存在；
        }

        if(!((++i)%idxs)) // 一个unit内的idx整理完，将其刷新下去；
        {
            units= core->ssz * new + 1 + ((i - 1) / idxs);
            res = core->drv->efs_write_media(core, units, core->ebuf);
            if(res) // 将idx更新到新区；
            {
                printf("\r\n: erro : efs    : recycle %d cannot write new idx table.", new);
                goto __RETRY_RECYCLE;
            }
        }

        idx ++; // 下一个idx；
    }

    // 整理文件信息；
    for(i=0; (u32)i<core->fmax; i++)
    {
        k = i / 8;
        j = i % 8;
        if(idxbit[k]&((u8)(1 << j)))
            continue; // idx是空闲的表示文件空间是无效，整理下一个；

        units = core->serial * core->ssz + core->finfo + i;
        res = core->drv->efs_read_media(core, units, core->ebuf);
        if(res)
        {
            printf("\r\n: erro : efs    : recycle cannot read old file space %d's info.", i);
            continue;
        }

        __e_structfile(core, &fstruct, core->ebuf);
        if(__fix(core, fstruct.name.n, core->nsz, fstruct.name.e)) // 不知道原文件名长，使用最大范围；
        {
            printf("\r\n: dbug : efs    : recycle old file info %d's name maybe corrupted(%s).", i, fstruct.name.n);
        }

        for(k=0, j=0; j<sizes; j++)
        {
            if((-1 == (s32)fstruct.size[j].s) && (-1 == (s32)fstruct.size[j].e))
            {
                // 未知逻辑，文件第一个size为空闲，不可能不存在；
                // 进一步检查，如果后续size也全部是空闲，则是存在问题；
                if(j==0)
                {
                    k = 0;
                    continue;
                }
                else
                {
                    // 未知逻辑，文件第一个size不可能不存在；
                    // 尝试第二个位置，看是否还是空闲；如果还是空间；
                    if(++k==j)
                        continue;

                    break; // 遇到空闲size位置，则退出；
                }
            }
        }

        if(j!=k)
        {
            j--; // 最后一个size记录；
            if(__fix(core, &(fstruct.size[j].s), sizeof(fstruct.size[j].s), &(fstruct.size[j].e)))
            {
                printf("\r\n: warn : efs    : recycle old file info %d's size of maybe corrupted.", i);
            }

            fstruct.size[0].s = fstruct.size[j].s; // 将size记录到第一个位置；
            fstruct.size[0].e = fstruct.size[j].e;
            memset(&fstruct.size[1], 0xFF, (sizes-1)*sizeof(struct __esize)); // 剩余的size空间重置；
        }
        else
        {
            // size记录位置全为空闲；
            printf("\r\n: warn : efs    : recycle old file info %d's size is not exist.", i);
        }

        units = new * core->ssz + core->finfo + i; // 将文件信息更新到新区；
        if(core->drv->efs_write_media(core, units, core->ebuf))
        {
            printf("\r\n: erro : efs    : recycle cannot write new file info %d.", i);
            goto __RETRY_RECYCLE;
        }
    }

    // 将系统信息更新完的标志位到头部；
    // 此阶段，如果读写失败了，就先不管；
    units = new * core->ssz;
    res = core->drv->efs_read_media(core, units, estruct);
    if(res)
    {
        printf("\r\n: erro : efs    : recycle %d cannot read new system area to finish.", new);
    }

    if(__fix(core, estruct, (sizeof(*estruct)-SYS_NOECCSIZE), &(estruct->ecc))) // 系统头部存在问题,尝试下一个区域；
    {
        printf("\r\n: erro : efs    : recycle %d new system area has corrupted to finish.", new);
    }

    estruct->status = SYS_UPDATED;
    res = core->drv->efs_write_media(core, units, estruct);
    if(res)
    {
        printf("\r\n: erro : efs    : recycle %d cannot write new system area to finish.", new);
    }

    printf("\r\n: info : efs    : recycle successfully, use %d.", new);
    core->serial = new;
    __unlock(core);
    return (0);
}

// ============================================================================
// 功能：标记索引；
// 参数：core -- 文件系统控制结构；
//      loc -- 文件空间位置；
//      name -- 文件名；
//      order -- 文件空间在文件中的序列（从一计）；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __e_markidx(struct __ecore *core, s32 loc, char *name, u32 order)
{
    s64 units;
    struct __eidx idx; // 不使用指针，是防止写失败了，又需要重新计算；
    u32 idxs;

    idxs = (1 << core->media->usz) / sizeof(struct __eidx); // 一个unit内可存放idx的数量；
    units = loc / idxs + 1; // idx所在unit, unit对齐；
    idxs = sizeof(struct __eidx) * (loc % idxs); // idx在unit内的偏置；
    if(name&&order)
    {
        __key(name, idx.key);
        memcpy(idx.bakup, idx.key, KEYLEN);
        idx.order = order;
        idx.ecc = 0;
        __gen(core, &idx, (sizeof(struct __eidx)-4), &idx.ecc);
    }
    else
    {
        memset(&idx, 0x0, sizeof(struct __eidx));
    }

    memset(core->ebuf, 0xFF, (1<<core->media->usz));
    memcpy((core->ebuf + idxs), &idx, sizeof(struct __eidx));
    if(core->drv->efs_write_media(core, (core->serial*core->ssz+units), core->ebuf))
    {
        printf("\r\n: dbug : efs    : mark idx failed, try recycle.");
        if(__e_recycle(core)) // 写失败，尝试回收，换一个系统区；
            return (-1);

        memset(core->ebuf, 0xFF, (1<<core->media->usz));
        memcpy((core->ebuf + idxs), &idx, sizeof(struct __eidx));
        if(core->drv->efs_write_media(core, (core->serial*core->ssz+units), core->ebuf))
            return (-1);
    }

    return (0);
}


// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
#if 0
static s32 __e_recovery(struct __ecore *core)
{
    struct umedia *media;
    u8 *buf;
    s32 res, i, size;
    s64 bakup, recovery;
    struct __ecstruct *estruct;

    bakup = (core->serial + 1) * core->ssz + media->startu;
    media = core->media;
    estruct = (struct __ecstruct*)core->buf;

    res = core->drv->efs_read_media(media, backup, buf);
    if(res)
    {
        printf("\r\n: erro : efs    : recovery<cannot read>.");
        return (-1);
    }

    if((__fix(estruct, (size-4), estruct->ecc)) ||
       (strcmp(estruct->signature, ESIGN)) ||
       (estruct->range != core->tsz) ||
       (estruct->files != core->max))
    {
        return (-1); // 文件头部错误
    }

    if(core->serial > SYS_LOOPS)
    {
        core->serial = 1;
    }

    sys = core->serial * core->ssz + media->startu;
    res = core->drv->efs_read_media(media, sys, media->sizeu, media->buf);
    if(res)
    {
        printf("\r\n: erro : efs    : recovery<cannot read>.");
        return (-1);
    }

    for(i=0; i<media->sizeu; i++)
    {
        if(0xFF != (media->buf)[i])
        {
            res = core->drv->efs_erase_media(media, sys, core->ssz*2);
            if(res)
            {
                printf("\r\n: erro : efs    : recovery<cannot erase>.");
                return (-1);
            }
        }
    }

    res = core->drv->efs_write_media(media, sys, estruct);
    if(res)
    {
        printf("\r\n: erro : efs    : recovery<cannot write>.");
        return (-1);
    }



    return (0);
}

#endif
// ============================================================================
// 功能：检索文件系统
// 参数：
// 返回：错误或文件系统被破坏（-1）；存在文件系统（0）；不存在文件系统（1）；
// 备注：
// ============================================================================
static s32 __e_scan(struct __ecore *core)
{
    u32 size, eldest, i, j;
    u8 serial, found = 0;
    struct __ecstruct *estruct;
    struct __scanlogic scan[SYS_LOOPS];

    estruct = (struct __ecstruct*)core->ebuf;
    eldest = 0;
    size = 1 << core->media->usz;

    // 检查系统信息有效性；
    for(i = 0; i < SYS_LOOPS; i++)
    {
        if(core->drv->efs_read_media(core, (core->ssz*i), estruct))
        {
            printf("\r\n: erro : efs    : scan in loop %d(cannot read).", i);
            scan[i].stat = badsys;
            continue;
        }

        for(j = 0; j < size; j++)
        {
            if(0xFF != ((u8*)estruct)[j])
                break;
        }

        if(j == size)
        {
            scan[i].stat = nosys;
        }
        else
        {
            // 检验数据正确性；
            __fix(core, estruct, (sizeof(*estruct)-SYS_NOECCSIZE), &(estruct->ecc));
            if((estruct->range != (core->ASize)) ||
               (strcmp(estruct->signature, ESIGN)) ||
               (estruct->files != core->fmax) ||
               (estruct->status != SYS_UPDATED))
            {
                // 存在不一致
                if((estruct->range == (core->ASize)) ||
                   (!strcmp(estruct->signature, ESIGN)) ||
                   (estruct->files == core->fmax) ||
                   (estruct->status == SYS_UPDATED))
                {
                    scan[i].stat = badsys; // 只有部分数据正确，说明被被破坏了
                }
                else
                {
                    scan[i].stat = nosys; // 全部数据不正确，可能是不存在文件系统信息
                }

                continue;
            }
            else
            {
                scan[i].stat = goodsys;
                scan[i].age = estruct->age;
            }

            if(goodsys == scan[i].stat)
            {
                // 对于有效的系统信息，查找到最后一次的记录
                found = 1;
                if(!i) // 第一个系统空间
                {
                    eldest = scan[i].age; // 记录最大age；
                    serial = i; //
                }
                else if((eldest < scan[i].age) ||
                   ((eldest) && (0 == scan[i].age))) // age数据溢出时，即从最大值到最小值的变换时
                {
                    eldest = scan[i].age; // 记录最大age；
                    serial = i; //
                }
                else if(eldest == scan[i].age)
                {
                    printf("\r\n: erro : efs    : scan in loop %d(unknow logic, two eldest age).", i);
                    return (-1);
                }
            }
        }
    }

    if(found)// 找到最老的，存在有效系统信息
    {
        core->serial = serial;
        return (0);
    }
    else
    {
        for(i = 0; i < SYS_LOOPS; i++)
        {
            if(nosys != scan[i].stat)
                return (-1); // 未找到有效系统信息, 文件系统被破坏
        }
    }

    return (1); // 不存在文件系统
}

// ============================================================================
// 功能：建立文件系统
// 参数：core -- 文件系统控制结构；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __e_build(struct __ecore *core)
{
    struct __ecstruct *estruct;
    s32 res, erases, esz;
    u8 i, once;
    s64 offset = 0;

    // 只格式化系统区
    printf("\r\n: info : efs    : new system is building...");
    erases = core->ssz * SYS_LOOPS;
    esz = (1 << (core->media->esz - core->media->usz)) << 1; // 一次擦除的unit数量；
    while(erases)
    {
        if(erases > esz)
            once = esz;
        else
            once = erases;

        res = core->drv->efs_erase_media(core, offset, once);
        if(res)
        {
            printf("\r\n: erro : efs    : new system build failed(cannot format).");
            return (-1);
        }

        switch(i)
        {
            case 0 : printf("\b\b\b.  ");i = 1;break;
            case 1 : printf("\b\b\b.. ");i = 2;break;
            case 2 : printf("\b\b\b...");i = 0;break;
            default: break;
        }

        erases -= once;
        offset += once;
    }

    printf("\b\b\b\b\b\b\b\b\b\b\bformated.");
    // 建立系统结构
    memset(core->ebuf, 0xFF, (1<<core->media->usz));
    estruct = (struct __ecstruct*)core->ebuf;
    estruct->age = 0;
    estruct->files = core->fmax;
    estruct->range = core->ASize;
    memcpy(estruct->signature, ESIGN, ESIG_LEN);
    estruct->ecc = 0;
    __gen(core, estruct, (sizeof(struct __ecstruct)-SYS_NOECCSIZE), &(estruct->ecc));
    estruct->status = SYS_UPDATED;

    for(i = 0; i < SYS_LOOPS; i++)
    {
        res = core->drv->efs_write_media(core, (i*core->ssz), estruct);
        if(!res)
        {
            break; // 成功；
        }

        // 失败，尝试下一个，把这个擦除
        printf("\r\n: warn : efs    : new system can not write in sys loop %d.", i);
        res = core->drv->efs_erase_media(core, (i*core->ssz), core->ssz);
        if(res)
        {
            printf("\r\n: erro : efs    : new system build failed in sys loop %d(erase).", i);
            return (-1);
        }
    }

    if(SYS_LOOPS == i)
    {
        printf("\r\n: erro : efs    : new system build failed(no valid sys loop).");
        return (-1);
    }

    return (0);
}

// ============================================================================
// 功能：查找空闲的文件空间；
// 参数：
// 返回：空闲（空闲位置）；出错（-1）；
// 备注：
// ============================================================================
static s32 __e_lookupfree(struct __ecore *core)
{
    s64 units;
    s32 freeloc = -1, size, i, j, idxs;
    u8 *tmp, recycle = 0;

    size = sizeof(struct __eidx);
    idxs = (1 << core->media->usz) / size; // 一个unit中能存放多少个idx
    __lock(core);

__RETRY_LOOKUPFREE:
    for(i = 0; (u32)i < core->fmax; i++)
    {
        if(!(i%idxs))
        {
            units = core->serial * core->ssz + 1 + i / idxs;
            if(core->drv->efs_read_media(core, units, core->ebuf))
            {
                printf("\r\n: erro : efs    : lookup free failed(read).");
                i = idxs + i - 1; // 减一是为是了照顾for逻辑
                continue; // 读idx表失败，尝试下一个表
            }

            tmp = core->ebuf;
        }

        for(j = 0; j < size; j++)
        {
            if(0xFF != tmp[j])
                break;
        }

        if(j == size)
        {
            freeloc = i; // 空间全非FF，则表示为空闲的idx
            break;
        }

        tmp += size;
   }

   if(!recycle &&  (-1 == freeloc)) // 没有空闲了，尝试回收；
   {
       recycle = 1;
       printf("\r\n: dbug : efs    : lookup free failed, no space, try recycle.");
       if(!__e_recycle(core))
           goto __RETRY_LOOKUPFREE; // 成功回收后，再尝试获取空闲idx
   }

   __unlock(core);
   return (freeloc);
}

// ============================================================================
// 功能：缓存文件信息
// 参数：
// 返回：
// 备注：
// ============================================================================
static inline struct __efile *__e_cachefile(struct __ecore *core, struct __loc *loc, char *name, struct obj *ob)
{
    struct __efstruct fstruct;
    s64 units, size;
    s32 res, szmax, i;
    u32 ecc;
    struct __efile *file;
    struct obj *find;
    struct __loc *eloc = loc;

    find = obj_search_child(ob, name); // 先在对象系统中查找
    if(find)
        return ((struct __efile*)obj_GetPrivate(find));

    eloc = __getloc(position, core, loc, -1); // 获取文件最末尾的位置
    units = (core->ssz * core->serial) + (core->finfo + eloc->loc);
    res = core->drv->efs_read_media(core, units, core->ebuf); // 文件信息
    if(res)
    {
        printf("\r\n: erro : efs    : cache file failed(cannot read info %d).", eloc->loc);
        return (NULL);
    }

    __e_structfile(core, &fstruct, core->ebuf);
    if(strcmp(name, fstruct.name.n))
    {
        __gen(core, name, strlen(name+1), &ecc); // 因为知道文件名，就使用知道文件名的ecc值；
        __fix(core, fstruct.name.n, (strlen(name)+1), &ecc);
        if(strcmp(name, fstruct.name.n))
        {
            printf("\r\n: error : efs    : file name is corrupted(\"%s\" != \"%s\").", fstruct.name.n, name);
            return (NULL); // 名字不匹配
        }
    }

    // 获取size记录位置
    szmax = (1 << core->media->usz) - (core->nsz + 4); // 一个unit内的可容纳size记录数量
    for(i = 0; i < szmax; i++)
    {
        if((-1 == (s32)fstruct.size[i].s) && (-1 == (s32)fstruct.size[i].e))
        {
            break; // 直至未被记录的位置
        }
    }

    if(-1 != (--i)) // 确保不是第0个（不合设计逻辑）
    {
        __gen(core, &(fstruct.size[i].s), sizeof(fstruct.size[i].s), &ecc);
        if(ecc != fstruct.size[i].e) // 检测size的ECC值是否一致，如果不一致，说明数据被改了，尝试修复
        {
            // 因为不知道实际的大小，所以使用介质上值；
            size = fstruct.size[i].s;
            if(__fix(core, &(fstruct.size[i].s), sizeof(fstruct.size[i].s), &(fstruct.size[i].e)))
            {
                printf("\r\n: warn : efs    : file \"%s\"'s size maybe corrupted.", name);
            }

            if(fstruct.size[i].s != size) // 获取backup失败，使用与name同一个区；
                fstruct.size[i].s = size; // 使用原始数据，因为不知道哪个是对的；
        }
    }
    else // 无有效size
    {
        printf("\r\n: erro : efs    : file \"%s\"'s size maybe corrupted(no data).", name);
        fstruct.size[0].s = 0;
    }

    file = malloc(sizeof(*file));
    if(!file)
    {
        printf("\r\n: erro : efs    : cache file \"%s\" failed(memory out).", name);
        return (NULL);
    }

    file->loc = loc;
    file->size = fstruct.size[i].s;
    if(!obj_newchild(ob, e_operations, (ptu32_t)file, name))
    {
        printf("\r\n: erro : efs    : cache file \"%s\" failed(virtual).", name);
        free(file);
        return (NULL);
    }

    return (file);
}

// ============================================================================
// 功能：分配文件空间
// 参数：core -- 文件系统控制；
//      loc -- 文件位置；
//      name -- 文件名；
//      order -- 文件内容序列（从一计）；
// 返回：
// 备注：
// ============================================================================
static s32 __e_allocfile(struct __ecore *core, u32 *loc, const char *name, u32 order)
{
    struct __efstruct fstruct;
    s64 units;
    s32 max, res, i;

    max = 1 << core->media->usz;
    while(1)
    {
        units = core->ssz * core->serial + core->finfo + *loc;
        res = core->drv->efs_read_media(core, units, core->ebuf); // 文件信息
        if(!res)
        {
            for(i = 0; i < max; i++)
            {
                if(0xFF != core->ebuf[i])
                {
                    res = -1; // 文件信息区不干净，标记为不可用；并再申请一个空间；
                    break;
                }
            }
        }

        if(i == max)
        {
            units = (core->ssz * SYS_LOOPS) + (*loc * core->fsz);
            res = core->drv->efs_erase_media(core, units, core->fsz); // 准备文件空间
            if(!res)
            {
                // 是可用的区域，先写信息
                memset(core->ebuf, 0xFF, (1<<core->media->usz));
                __e_structfile(core, &fstruct, core->ebuf);
                strcpy(fstruct.name.n, name);
                __gen(core, fstruct.name.n, (strlen(name)+1), fstruct.name.e);
                fstruct.size->s = 0;
                __gen(core, &(fstruct.size->s), 8, &(fstruct.size->e));
                units = core->ssz * core->serial + core->finfo + *loc;
                res = core->drv->efs_write_media(core, units, core->ebuf);
                if(!res)
                {
                    res = __e_markidx(core, *loc, fstruct.name.n, order); // 设置idx
                }
            }
        }

        if(res) // 失败，再申请一个
        {
            __e_markidx(core, *loc, NULL, 0); // 标记不可用区域
            *loc = __e_lookupfree(core); // 获取一个可用区域
            if(-1 == (s32)*loc)
            {
                printf("\r\n: erro : efs    : allocate file \"%s\"(no space).", name);
                return (-1);
            }
        }
        else
        {
            break; // 设置成功
        }

    }

    return (res);
}
// ============================================================================
// 功能：新建文件；
// 参数：core -- 文件系统控制；
//      loc -- 文件位置；
//      name -- 文件名；
//      ob -- 文件缓存节点；
// 返回：成功（文件结构）；失败（NULL）；
// 备注：
// ============================================================================
static struct __efile *__e_newfile(struct __ecore *core, u32 loc, char *name, struct obj *ob)
{
    s32 res;
    struct __efile *file;

    res = __e_allocfile(core, &loc, name, 1);
    if(res)
    {
        printf("\r\n: erro : efs    : new file \"%s\" failed(create).", name);
        return (NULL);
    }

    file = malloc(sizeof(*file));
    if(!file)
    {
        printf("\r\n: erro : efs    : new file \"%s\"(memory out).", name);
        return (NULL);
    }

    file->loc = __addloc(NULL, loc, 1, 0);
    file->size = 0;
    if(!obj_newchild(ob, e_operations, (ptu32_t)file, name))
    {
        printf("\r\n: erro : efs    : new file \"%s\"(virtual).", name);
        free(file);
        return (NULL);
    }

    return (file);
}

// ============================================================================
// 功能：更新文件尺寸
// 参数：core -- 文件系统控制；
//      file -- 文件；
//      size -- 更新的尺寸；
//      name -- 文件名；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __e_updatefilesz(struct __ecore *core, struct __efile *file, s64 size, const char *name)
{
    s64 units, dels;
    struct __efstruct fstruct;
    s32 res, szmax, i;
    u32 loc, eccu, ecce;
    struct __loc *eloc;

    if(size == file->size)
        return (0); // file size只会在本函数内发生变更

    __lock(core);
    eloc = __getloc(position, core, file->loc, -1);
    if(size<file->size) // 尺寸减少
    {
        eccu = (1 << (core->media->usz - BUFBITS)) * 4; // 一个unit内被ECC占用的空间；
        ecce = core->fsz * eccu; // 一个文件空间内被ECC占用掉的空间；
        dels = file->size - size; // 需要删除的大小
        // 需减少的部分，判断是否大于等于文件在最后一个文件空间的已使用量；
        // 如果大于等于，则需要删除至少最后一个文件空间；
        dels -= (file->size % ((core->fsz << core->media->usz) - ecce));
        if(dels>=0) // 大于， 则需要至少删除最后一个区域
        {
            dels = dels / ((core->fsz << core->media->usz) - ecce) + 1; // 总体需要删除的区域数，此处"+1"表示至少要删除一个文件空间；
            while(dels--)
            {
                // 从最后一个区域开始删除文件内容（索引）
                if(__e_markidx(core, eloc->loc, NULL, 0))
                {
                    printf("\r\n: warn : efs    : update file size has problem(shrink).");
                }

                eloc = __delloc(eloc, -1);
            }
        }
    }

    if(!eloc) // 文件被重置为零了，但是需要为其重新申请一个区域；（空文件逻辑上也是需要分配文件空间的，相当于新建一个文件）
    {
        loc = __e_lookupfree(core);
        if(-1 == (s32)loc)
        {
            printf("\r\n: warn : efs    : update file size has problem(no space).");
            file->loc = NULL;
            file->size = 0;
            __unlock(core);
            return (-1);
        }

        res = __e_allocfile(core, &loc, name, 1);
        if(res)
        {
            printf("\r\n: warn : efs    : update file size has problem(cannot allocate).");
            file->loc = NULL;
            file->size = 0;
            __unlock(core);
            return (-1);
        }

        eloc = __addloc(NULL, loc, 1, 0);
        if(!eloc)
        {
            printf("\r\n: warn : efs    : update file size has problem(no memory).");
            file->loc = NULL;
            file->size = 0;
            __unlock(core);
            return (-1);
        }

        file->loc = eloc;
        file->size = 0;
    }
    else // 文件尺寸发生了变，需要更新尺寸数据；
    {
        units = core->ssz * core->serial + core->finfo + eloc->loc;
        res = core->drv->efs_read_media(core, units, core->ebuf); // 文件信息
        if(res)
        {
            printf("\r\n: erro : efs    : cannot update file size(read).");
            __unlock(core);
            return (-1);
        }

        __e_structfile(core, &fstruct, core->ebuf);
        szmax = ((1 << core->media->usz) - (core->nsz + 4)) / sizeof(struct __esize); // 可以存放file size的次数
        for(i = 0; i < szmax; i++ )
        {
            if((-1 == (s32)fstruct.size[i].s) && (-1 == (s32)fstruct.size[i].e))
            {
                break; // 查找到空闲file size
            }
        }

        if(szmax == i) // size空间写满，进行回收
        {
            printf("\r\n: dbug : efs    : file size full, try recycle.");
            res = __e_recycle(core);
            if(res)
            {
                printf("\r\n: erro : efs    : cannot update file size(recycle).");
                __unlock(core);
                return (-1);
            }

            i = 1; // size的新位置；回收时，第一个位置肯定是被用掉了的
        }

        memset(core->ebuf, 0xFF, (1<<core->media->usz));
        file->size = size;
        fstruct.size[i].s = file->size;
        __gen(core, &(fstruct.size[i].s), sizeof(fstruct.size[i].s), &(fstruct.size[i].e));
        units = core->serial * core->ssz + core->finfo + eloc->loc; // 可能由回收带来的serial变化
        res = core->drv->efs_write_media(core, units, core->ebuf);
        if(res)
        {
            __unlock(core);
            return (-1);
        }
    }

    __unlock(core);
    return (0);
}

// ============================================================================
// 功能：删除文件
// 参数：core -- 文件系统控制；
//      loc -- 文件位置；
// 返回：成功（0）；失败（-1）；
// 备注：从文件尾部开始往前删除
// ============================================================================
static s32 __e_destroyfile(struct __ecore *core, struct __loc *loc)
{
    s64 units;
    u32 idxmax, oloc = 1;
    u8 update = 1;
    s32 res;
    struct __eidx *idx;
    struct __loc *eloc;

    idxmax = (1 << core->media->usz) / sizeof(struct __eidx); // 一个unit内可存放idx的数量
    eloc = __getloc(position, core, loc, -1);
    if(!eloc)
    {
        printf("\r\n: erro : efs    : destroy file(no location).");
        return (-1);
    }

    while(eloc)
    {
        if(update)
        {
            units = core->serial * core->ssz + 1;
            units += eloc->loc / idxmax;
            res = core->drv->efs_read_media(core, units, core->ebuf); // 索引表
            if(res)
            {
                printf("\r\n: erro : efs    : destroy file(read idx).");
                return (-1);
            }
        }

        idx = (struct __eidx*)core->ebuf;
        memset((idx+(eloc->loc%idxmax)), 0x0, sizeof(struct __eidx)); // 将idx区域置零
        oloc = eloc->loc; // 已删除的loc
        eloc = __delloc(eloc, -1);
        if(!eloc)
        {
            update = 1; // 全部删除
        }
        else
        {
            // 判断是否需要读idx表，因为idx的loc跨unit引起
            if(eloc->loc>oloc)
            {
                if((eloc->loc-oloc)>(idxmax-(oloc %idxmax)))
                    update = 1;
                else
                    update = 0;
            }
            else
            {
                if((oloc-eloc->loc) > (oloc %idxmax))
                    update = 1;
                else
                    update = 0;
            }
        }

        if(update) // 重新读或者删除完成
        {
            res = core->drv->efs_write_media(core, units, core->ebuf);
            if(res)
            {
                printf("\r\n: erro : efs    : destroy file \"%s\"(write).", (char*)core->ebuf);
                return (-1);
            }
        }
    }

    return (0);
}

// ============================================================================
// 功能：文件同步；
// 参数：hdl -- 内部句柄；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __e_sync(struct objhandle *hdl)
{
    s64 units, sz;
    s32 bufsmax, bufc, bufo, eccu;
    struct __loc *eloc;
    struct __econtext *cx = (struct __econtext*)handle_context(hdl);
    struct __ecore *core = (struct __ecore*)corefs(handle_GetHostObj(hdl));
    struct __efile *file = (struct __efile *)handle_GetHostObjectPrivate(hdl);

    if(!cx->dirty)
        return (0); // 没有需要更新的数据

    if((cx->pos+cx->wpos)>file->size) // 同步的时候，只有文件尺寸扩大才会更新文件尺寸
    {
        if(__e_updatefilesz(core, file, cx->pos+cx->wpos, handle_name(hdl)))
            return (-1);
    }

    if(cx->wpos)
    {
        sz = (cx->pos - (cx->pos % BUFLEN)) + BUFLEN;
        if(sz<file->size) // 文件的当前位置不能小于文件大小(预留了缓冲余量)
        {
            // 目前不支持改写
            printf("\r\n: erro : efs    : \"%s\" write failed(do not support rewrite beyond buffer size).", handle_name(hdl));
            return (-1);
        }

        eccu = (1 << (core->media->usz - BUFBITS)) * 4; // 一个unit内被ECC占用的空间；
        eloc = __getloc(position, core, file->loc, (cx->pos+cx->wpos));
        units = (core->ssz*SYS_LOOPS) + (core->fsz * eloc->loc);
        units += (cx->pos / ((1 << core->media->usz) - eccu)) % core->fsz; // 此时cx->pos逻辑上是保证wbuf对齐的；
        bufsmax = 1 << (core->media->usz - BUFBITS); // 一个unit内有多少个buffer区；
        bufc = (cx->pos / BUFLEN) % bufsmax; // 一个unit内已使用了多少个buffer区；
        bufo = bufc << BUFBITS; // 文件指针在一个unit内的偏置,buffer区对齐；
        memset(core->ebuf, 0xFF, (1<<core->media->usz));
        memcpy(core->ebuf+bufo, cx->wbuf, cx->wpos); // 不做ECC；
        if(core->drv->efs_write_media(core, units, core->ebuf))
            return (-1);

        cx->dirty = 0;
    }

    return (0);
}

// ============================================================================
// 功能：查找文件
// 参数：core -- 文件系统控制；
//      name -- 文件名；
//      loc -- 文件位置；
// 返回：空闲（空闲位置，逻辑增一,使用时减一）；找到（0）；出错（-1）；没有空闲（-2）；
// 备注：
// ============================================================================
static s32 __e_lookupfile(struct __ecore *core, const char *name, struct __loc **loc)
{
    s32 size, i, idxs, res;
    struct __eidx *idx;
    u8 key[KEYLEN];
    u32 freeloc = -2;
    s64 units;
    struct __loc *hloc = NULL;

    size = strlen(name);
    if(size >= core->nsz)
    {
        printf("\r\n: erro : efs    : lookup failed(file name \"%s\" is illegal).", name);
        return (-1);
    }

    __key(name, key);
    idxs = (1 << core->media->usz) / sizeof(*idx); // 一个unit中能存放多少个idx
    __lock(core);
    for(i = 0; (u32)i < core->fmax; i++)
    {
        if(!(i%idxs))// 一个unit内的idx读完，尝试下一页
        {
            units = core->serial * core->ssz + 1 + (i / idxs);
            if(core->drv->efs_read_media(core, units, core->ebuf)) // 索引表
            {
                printf("\r\n: erro : efs    : lookup has problem(can not read %d).", i);
                i += idxs;
                continue; // 查询下一个索引表
            }

            idx = (struct __eidx *)core->ebuf;
        }

        res = __e_matchkey(core, idx, key);
        if(0 == res)
        {
            // 匹配成功
            if(!__addloc(&hloc, i, idx->order, 1))
            {
                printf("\r\n: erro : efs    : lookup failed(memory out).");
                __unlock(core);
                return (-1); //
            }
        }
        else if((-2 == (s32)freeloc) && (2 == res))
        {
            freeloc = i;
        }
        else if(-1 == res)
        {
            printf("\r\n: warn : efs    : lookup idx %d is corrupted.", i);
        }

        idx++;
    }

    __unlock(core);
    if(hloc)
        __e_sanityloc(core, &hloc, name); // 进一步校验数据正确性

    if(hloc)
    {
        *loc = hloc;
        return (0);
    }
    else
    {
        return (freeloc+1); // 未找到，返回空闲
    }
}

// ============================================================================
// 功能：打开文件；
// 参数：ob -- 文件节点；
//      flags -- 文件打开方式；
//      uncached -- 文件未缓存；
// 返回：成功（文件上下文）；失败（NULL）；
// 备注：
// ============================================================================
static struct objhandle *__e_open(struct obj *ob, u32 flags, char *uncached)
{
    s32 res, bufs, eccu;
    struct __loc *loc;
    s64 units;
    struct __efile *file;
    struct __econtext *cx;
    struct objhandle *hdl;
    struct __ecore *core = (struct __ecore*)corefs(ob);
    mode_t mode, property = 0;

    if(test_directory(flags)) // 目录逻辑
    {
        if(uncached) // EFS不支持目录逻辑
        {
            printf("\r\n : dbug  : efs    : do not support directory.");
            return (NULL);
        }
        else
        {
            if(!obj_isMount(ob)) // 确认打开的目录是安装点
                return (NULL);
        }
        property = S_IFDIR;
    }
    else // 文件的逻辑
    {
        __lock(core);
        if(!uncached)
        {
            // 文件已缓存，但要确认不是安装点
            if(obj_isMount(ob))
            {
                __unlock(core);
                return (NULL);
            }
        }
        else
        {
            // 文件未缓存，需要到介质上查找
            res = __e_lookupfile(core, uncached, &loc);
            if(-1 == res)
            {
                printf("\r\n : erro : efs    : open failed(look up).");
                __unlock(core);
                return (NULL);
            }
            else if(!res) // 文件已存在
            {
                if(test_onlycreat(flags)) // 已存在，但只要求新建；
                {
                    printf("\r\n : dbug : efs    : open \"%s\" failed(already exist).", uncached);
                    __unlock(core);
                    return (NULL);
                }

                file = __e_cachefile(core, loc, uncached, ob);
                if(!file)
                {
                    while(loc)
                        loc = __delloc(loc, 1);

                    printf("\r\n : dbug : efs    : open \"%s\" failed(cache in).", uncached);
                    __unlock(core);
                    return (NULL);
                }
            }
            else // 文件不存在
            {
                if(!test_creat(flags)) // 文件不存在，但不要求新建；
                {
                    printf("\r\n : dbug : efs    : open \"%s\" failed(do not exist).", uncached);
                    __unlock(core);
                    return (NULL);
                }
                else
                {
                    if(-2 == res) // 没有空闲
                    {
                        printf("\r\n: dbug : efs    : open failed, no space, try recycle.");
                        res = __e_recycle(core); // 尝试回收；
                        if(!res)
                        {
                            printf("\r\n : dbug : efs    : open \"%s\" failed(recycle).", uncached);
                            __unlock(core);
                            return (NULL);
                        }

                        res = __e_lookupfree(core); // 再尝试获取空闲
                        if(-1 == res)
                        {
                            printf("\r\n : dbug : efs    : open \"%s\" failed(no space).", uncached);
                            __unlock(core);
                            return (NULL);
                        }
                    }

                    file = __e_newfile(core, (res-1), uncached, ob); // 新建文件
                    if(!file)
                    {
                        printf("\r\n : erro : efs    : open \"%s\" failed(cannot create).", uncached);
                        __unlock(core);
                        return (NULL);
                    }
                }
            }
            property = S_IFREG;
        }

        cx = (struct __econtext*)malloc(sizeof(*cx)); // 文件上下文
        if(!cx)
        {
            printf("\r\n : erro : efs    : open \"%s\" failed(memory out).", uncached);
            __unlock(core);
            return (NULL);
        }

        memset(cx, 0xFF, BUFLEN);
        cx->pos = 0;
        cx->wpos = 0;
        cx->dirty = 0;
        if(test_trunc(flags))
        {
            if(!uncached) // 文件已缓存
            {
                if(obj_isonduty(ob))
                    printf("\r\n : warn : efs    : truncate file \"%s\" while others are using.", obj_name(ob));

                file = (struct __efile*)obj_GetPrivate(ob);
            }

            if(file->size)
            {
                if(uncached)
                    res = __e_updatefilesz(core, file, 0, uncached); // 文件内容清空，尺寸变为0
                else
                    res = __e_updatefilesz(core, file, 0, obj_name(ob)); // 文件内容清空，尺寸变为0

                if(res)
                {
                    free(cx);
                    __unlock(core);
                    return (NULL);
                }
            }
        }

        if(test_append(flags))
        {
            if(!uncached)
                //file = dListEntry(of_basiclinko(ob), struct __efile, basic);
                file = (struct __efile*)obj_GetPrivate(ob);

            cx->pos = file->size;
            if(cx->pos % BUFLEN)
            {
                // 最后一部分数据不对齐，将其缓存到buf；
                eccu = (1 << (core->media->usz - BUFBITS)) * 4; // 一个unit内被ECC占用的空间
                loc = __getloc(position, core, file->loc, -1);
                units = (core->ssz * SYS_LOOPS) + (loc->loc * core->fsz) + ((cx->pos / ((1 << core->media->usz) - eccu)) % core->fsz);
                res = core->drv->efs_read_media(core, units, core->ebuf); // 文件内容
                if(res)
                {
                    free(cx);
                    __unlock(core);
                    return (NULL);
                }

                bufs = (1 << core->media->usz) / (BUFLEN+3); // 一个unit内有多少个buffer
                bufs = (cx->pos / BUFLEN) % bufs; // 在一个unit内的需要缓冲的位置
                cx->wpos = cx->pos % BUFLEN; // 需要缓冲的量；
                cx->pos -= cx->wpos;
                memcpy(cx->wbuf, (core->ebuf+(bufs<<BUFBITS)), cx->wpos);
            }
        }
    }

    hdl = handle_new();
    if(!hdl)
    {
        printf("\r\n : erro : efs    : open failed(memory out).");
        __unlock(core);
        return (NULL);
    }

    handle_init(hdl, NULL, flags, (ptu32_t)cx);
    if(hdl)
    {
        //TODO：从yaffs2中读取权限等，暂时赋予全部权限。
        mode = S_IALLUGO | S_IFDIR | property;     //建立的路径，属性是目录。
        //继承操作方法，对象的私有成员保存访问模式（即 stat 的 st_mode ）
        ob = obj_buildpath(ob, e_operations, mode,uncached);
        obj_LinkHandle(hdl, ob);
    }
    __unlock(core);
    return (hdl);
}

// ============================================================================
// 功能：关闭文件；
// 参数：hdl -- 内部句柄；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __e_close(struct objhandle *hdl)
{
    struct __econtext *cx = (struct __econtext*)handle_context(hdl);
    struct __ecore *core = (struct __ecore*)corefs(handle_GetHostObj(hdl));
    struct __efile *file;
    struct obj *nxt, *head;
    struct __loc *eloc;

    __lock(core);
    if(cx) // 文件；
    {
        if(__e_sync(hdl))
        {
            printf("\r\n : erro : efs    : \"%s\" close failed(sync).", handle_name(hdl));
        }

        free(cx); // TODO: 这里并没有释放文件缓存，下次打开时，就不用再次查找文件了；
    }
    else // 目录；
    {
        // 关闭目录时，把目录下未使用却已缓存的文件释放掉；
        while(1)
        {
            head = obj_child(handle_GetHostObj(hdl));
            if(!head)
                break;

            if(!obj_isonduty(head))
            {
                file = (struct __efile*)obj_GetPrivate(head);
                eloc = __getloc(position, core, file->loc, -1);
                while(eloc)
                    eloc = __delloc(eloc, -1);

                free(file);
                obj_Delete(head);
            }
            else
            {
                nxt = obj_next(head);
                while(nxt != head)
                {
                    if(!obj_isonduty(nxt))
                    {
                        file = (struct __efile*)obj_GetPrivate(head);
                        eloc = __getloc(position, core, file->loc, -1);
                        while(eloc)
                            eloc = __delloc(eloc, -1);

                        free(file);
                        obj_Delete(head);
                    }
                }
            }
        }
    }

    __unlock(core);
    handle_Delete(hdl);
    return (0);
}

// ============================================================================
// 功能：写文件；
// 参数：hdl -- 内部句柄；
//      data -- 数据；
//      len -- 数据长度；
// 返回：实际写入的字节数；
// 备注：
// ============================================================================
static s32 __e_write(struct objhandle *hdl, u8 *data, u32 len)
{
    s64 units;
    s32 sz, szc,  updatesz = 0;
    u32 loc, bufsmax, bufc, bufo, bufs = 0, all = len, ecc, ecce, eccu;
    struct __loc *eloc;
    struct __econtext *cx = (struct __econtext *)handle_context(hdl);
    struct __ecore *core = (struct __ecore*)corefs(handle_GetHostObj(hdl));
    struct __efile *file = (struct __efile *)handle_GetHostObjectPrivate(hdl);;

    if(handle_isAppend(hdl)) // 追加模式下的lseek引起文件位置回溯是无效的；
    {
        if((cx->pos+cx->wpos)<file->size)
        {
            cx->pos = file->size;
            cx->wpos = 0;
        }
    }

    // 写时，目前是允许最后一段大小小于BUFLEN时，重复写入的;TODO：数据不允许改写校验；
    sz = (cx->pos - (cx->pos % BUFLEN)) + BUFLEN;
    if(sz<file->size)
    {
        printf("\r\n: erro : efs    : \"%s\" write failed(do not support rewrite beyond buffer).", handle_name(hdl));
        return (0);
    }

    eccu = (1 << (core->media->usz - BUFBITS)) * 4; // 一个unit内被ECC占用的空间；
    ecce = core->fsz * eccu; // 一个文件空间内被ECC占用掉的空间；
    bufsmax = 1 << (core->media->usz - BUFBITS); // 一个unit内有多少个buffer区；
    __lock(core);
    eloc = __getloc(position, core, file->loc, (cx->pos+cx->wpos+1)); // 获取文件位置；(+1的逻辑，是防止当前位置正好在边界，那么此时需要申请空间)
    if(!eloc)
    {
        // 文件位置不存在，是由于seek引起的空洞，即逻辑上是依靠seek是无法拓展文件尺寸的，必须结合写的动作；因此在写时才会申请空间；
        // 另外，文件上次一次写入时，正好是在文件空间的结束位置。在写完成时，不会拓展文件空间，在下一次写时拓展；
        eloc = __getloc(position, core, file->loc, -1); // 获取文件实际末尾；
        sz =  cx->pos + cx->wpos -  (eloc->order * ((core->fsz << core->media->usz) - ecce)); // 相差空间的尺寸；
        if(sz<0)
        {
            printf("\r\n: erro : efs    : \"%s\" write failed(unknown, %ld, %d).", handle_name(hdl), (long)cx->pos, (u32)cx->wpos);
            __unlock(core);
            return (0);
        }

        if(sz) // 当前位置大于文件实际尺寸（等于零的情况就是上述的"另外.."部分）
        {
            // 需要扩展文件容量数量
            if(sz%((core->fsz<<core->media->usz)-ecce))
                sz = sz / ((core->fsz << core->media->usz) - ecce) + 1;
            else
                sz = sz / ((core->fsz << core->media->usz) - ecce);
        }
        else
        {
            sz = 1; // 等于零时，也是需要申请一块文件空间，因为接下来文件要写入，需要空间；
        }

        while(sz--) // 需要扩展的文件空间数量
        {
            loc = __e_lookupfree(core);
            if(-1 == (s32)loc)
            {
                printf("\r\n: erro : efs    : \"%s\" write failed(no space).", handle_name(hdl));
                __unlock(core);
                return (0);
            }

            if(__e_allocfile(core, &loc, handle_name(hdl), eloc->order+1))
            {
                printf("\r\n: erro : efs    : \"%s\" write failed(no space).", handle_name(hdl));
                __unlock(core);
                return (0);
            }

            eloc = __addloc(&eloc, loc, eloc->order+1, 0);
            if(!eloc)
            {
                printf("\r\n: erro : efs    : \"%s\" write failed(memory out).", handle_name(hdl));
                __unlock(core);
                return (0);
            }
        }
    }

    // 先确保cx->pos对齐,将不对齐的回读
    sz = cx->pos % BUFLEN;
    if(sz) // 将不对齐的部分回读
    {
        if((cx->pos<=file->size)||((cx->pos-sz)<file->size)) // 只有有数据才会读（逻辑上存在一个可能，就是将其seek扩展到某个不对齐位置，此时没有必要读）
        {
            // 最后一部分数据不对齐，将其缓存到buf；可能是此前的fsync等操作引起的；
            units = (core->ssz * core->serial) + (eloc->loc * core->fsz) + ((cx->pos / ((1 << core->media->usz) - eccu)) % core->fsz);
            if(core->drv->efs_read_media(core, units, core->ebuf))
            {
                printf("\r\n: erro : efs    : \"%s\" write failed(read back).", handle_name(hdl));
                __unlock(core);
                return (0);
            }

            bufc = (cx->pos / BUFLEN) % bufsmax; // 一个unit内已使用了多少个wbuf区；
            bufo = bufc << BUFBITS; // 文件指针在一个unit内的偏置，wbuf区对齐；
            if((file->size-cx->pos+sz) >= BUFLEN) // 回读的位置存在ECC校验
            {
                if(__fix(core, core->ebuf+bufo, BUFLEN, (u32*)(core->ebuf+ bufo + BUFLEN)))
                {
                    if(0xFFFFFFFF != *((u32*)(core->ebuf+bufo+BUFLEN)))
                        printf("\r\n: warn : efs    : \"%s\"'s data may corrupted(%ld).", handle_name(hdl), (long)cx->pos);
                }
            }

            memcpy(cx->wbuf, (core->ebuf+bufo), sz);
        }

        cx->pos -= sz;
        cx->wpos += sz;
    }

    // 此时cx->pos是wbuf对齐的（逻辑保证）
    while(len) // 按文件空间对齐分批写入
    {
        // 计算单批次写出的大小（文件空间大小）
        sz = ((cx->pos + cx->wpos) % ((core->fsz << core->media->usz) - ecce)); // 已存在的文件空间不对齐部分，去除了ECC空间的
        if((len+sz)>((core->fsz<<core->media->usz)-ecce))
        {
            // 需写出大小，是需要跨文件空间的，按文件空间对齐计算写出大小；
            sz = ((core->fsz << core->media->usz) - ecce) - sz;
        }
        else
        {
            sz = len; // 大小小于等于文件空间
        }

        updatesz = (sz + cx->wpos) - ((sz + cx->wpos) % BUFLEN); // 一个文件空间范围内，需要写出的数据大小（剔除缓存在wbuf的数据，因为要更新file size）；
        while(sz) // 文件空间内，按BUFLEN大小分批写出；
        {
            if(sz>(BUFLEN-cx->wpos)) // 单次循环的数据量；
                szc = BUFLEN - cx->wpos;
            else
                szc = sz; // wbuf可以缓存下；

            memcpy(cx->wbuf+cx->wpos, data, szc);
            data += szc;
            sz -= szc;
            len -= szc;
            if((cx->wpos+szc)<BUFLEN)
            {
                cx->wpos += szc; // 只是写出到wbuf，且未写满，就此退出；
                cx->dirty = 1;
            }
            else
            {
                // 数据缓冲内放置不下，需进入介质；
                if((-1!=updatesz)&&updatesz) // 先更新文件尺寸；防止写过程中掉电，造成逻辑破坏（文件空间逻辑只需写一次）；
                {
                    if((cx->pos+cx->wpos+updatesz)>file->size) // 只有文件尺寸扩大才会更新尺寸；
                    {
                        if(__e_updatefilesz(core, file, (cx->pos+updatesz), handle_name(hdl))) // 此时cx->pos是对齐的；
                        {
                            printf("\r\n: erro : efs    : write failed(update file size).");
                            goto __ERROR_WR;
                        }

                        updatesz = 0; // 更新了文件尺寸；
                    }

                    updatesz = -1; // 防止逻辑重复进入；
                    memset(core->ebuf, 0xFF, (1 << core->media->usz)); // 第一次重置ebuf；
                }

                cx->wpos = 0;
                ecc = 0;
                __gen(core, cx->wbuf, BUFLEN, &ecc);
                bufc = (cx->pos / BUFLEN) % bufsmax; // 一个unit内已使用了多少个wbuf区；
                bufo = bufc << BUFBITS; // 在一个unit内的偏置(wbuf区对齐)；
                memcpy((core->ebuf + bufo), cx->wbuf, BUFLEN);
                memcpy((core->ebuf + bufo + BUFLEN), (u8*)&ecc, 4);
                if(!(((++bufc)<bufsmax)&&(sz>BUFLEN))) // 除了剩余容量仍然可以写出这个unit的情况外，其他都写出到介质；
                {
                    units = (core->ssz * SYS_LOOPS) + (eloc->loc * core->fsz) + ((cx->pos / ((1 << core->media->usz) - eccu)) % core->fsz);
                    if(core->drv->efs_write_media(core, units, core->ebuf))
                    {
                        printf("\r\n: erro : efs    : write failed.");
                        cx->pos -= bufs * BUFLEN; // 失败，退回缓冲在ebuf的数据；
                        if(!updatesz) // 文件尺寸在上面发生变更，但实际写入的尺寸不对；
                        {
                            if(__e_updatefilesz(core, file, cx->pos, handle_name(hdl)))
                                printf("\r\n: erro : efs    : write failed(update file size after write failed).");
                        }

                        if(bufs) // 本次写入失败时，ebuf有多个wbuf缓冲，将第一个数据放在缓冲，其他的抛弃；
                        {
                            bufo = ((bufc - 1) - bufs) << BUFBITS;
                            memcpy(cx->wbuf, (core->ebuf+bufo), BUFLEN);
                        }

                        cx->wpos = BUFLEN;
                        cx->dirty = 1;
                        goto __ERROR_WR;
                    }

                    bufs = 0;
                    if(len>=BUFLEN)
                        memset(core->ebuf, 0xFF, (1 << core->media->usz)); // 缓存已刷入，重置缓存；
                }

                memset(cx->wbuf, 0xFF, BUFLEN);
                bufs ++; // 单次写出的buf数量；
                cx->pos += BUFLEN;
            }
        }

        if(len)
        {
            // 只能从文件末尾写，如果没有写完，则需要拓展空间；
            // 这里没有考虑seek回去的逻辑；TODO
            loc = __e_lookupfree(core);
            if(-1 == (s32)loc)
            {
                printf("\r\n: erro : efs    : write failed(no space).");
                goto __ERROR_WR;
            }

            if(__e_allocfile(core, &loc, handle_name(hdl), eloc->order+1))
            {
                printf("\r\n: erro : efs    : write failed(allocate file space).");
                goto __ERROR_WR;
            }

            eloc = __addloc(&eloc, loc, (eloc->order+1), 0);
            if(!eloc)
            {
                printf("\r\n: erro : efs    : write failed(memory out).");
                goto __ERROR_WR;
            }
        }
    }

__ERROR_WR:
    __unlock(core);

    return (all-len);
}

// ============================================================================
// 功能：读文件
// 参数：hdl -- 内部句柄；
//      data -- 数据；
//      len -- 数据长度；
// 返回：
// 备注：
// ============================================================================
static s32 __e_read(struct objhandle *hdl, u8 *data, u32 len)
{
    s64 units;
    s16 rebuf;
    u32 rdsz = 0, sz, szu, movs;
    u32 bufsmax, bufc, eccu, ecce;
    struct __loc *eloc;
    struct __econtext *cx = (struct __econtext *)handle_context(hdl);
    struct __ecore *core = (struct __ecore*)corefs(handle_GetHostObj(hdl));
    struct __efile *file = (struct __efile *)handle_GetHostObjectPrivate(hdl);

    __lock(core);
    if((cx->pos+cx->wpos)>=file->size)
    {
        __unlock(core);
        return (0); // 当前文件无内容可读；
    }

    if(len>(file->size-cx->pos-cx->wpos)) // 读越界了；
        len = file->size - cx->pos - cx->wpos;

    eccu = (1 << (core->media->usz - BUFBITS)) * 4; // 一个unit内被ECC占用的空间；
    ecce = core->fsz * eccu; // 一个文件空间内被ECC占用掉的空间；
    bufsmax = 1 << (core->media->usz - BUFBITS); // 一个unit内有多少个wbuf区；
    rebuf = (cx->pos + cx->wpos + len) % BUFLEN; // 计算读完后，结尾不对齐的部分，读完后需将从data缓存到wbuf；
    if((cx->wpos)&&((cx->wpos+len)>=BUFLEN)) // 存在缓冲，同时读数据会超出缓冲范围，则先将数据刷下去；
    {
        if(__e_sync(hdl))
        {
            printf("\r\n: warn : efs   : \"%s\"'s read(sync).", handle_name(hdl));
        }

        memset(cx->wbuf, 0xFF, BUFLEN);
    }

    if(BUFLEN==cx->wpos) // wbuf已满，将wpos置空；
    {
        cx->wpos = 0;
        cx->pos += BUFLEN;
    }

    do // 按文件空间尺寸，分批读入data空间
    {
        sz = ((cx->pos + cx->wpos) % ((core->fsz << core->media->usz) - ecce)); // 当前位置在文件空间不对齐部分，去除了ECC空间的；
        if((len+sz)>((core->fsz<<core->media->usz)-ecce))
            sz = ((core->fsz << core->media->usz) -ecce) - sz; // 需读入大小，需要跨文件空间，按文件空间对齐计算单次大小；
        else
            sz = len; // 小于一个文件空间内的数据量

        len -= sz;
        eloc = __getloc(position, core, file->loc, (cx->pos+cx->wpos)); // 文件位置;
        units = (core->ssz * SYS_LOOPS) + (core->fsz * eloc->loc)
                + (((cx->pos / ((1 << core->media->usz)- eccu))) % core->fsz); // 当前位置所在的unit
        do // 按文件空间尺寸对齐，一次循环读一个unit；
        {
            if(core->drv->efs_read_media(core, units++, core->ebuf)) // 文件内容；
            {
                printf("\r\n: dbug : efs    : \"%s\"'s read failed(%ld).", handle_name(hdl), (long)(cx->pos));
                __unlock(core);
                return (rdsz);
            }

            szu = ((cx->pos + cx->wpos) % ((1 << core->media->usz) - eccu)); // 当前位置在unit不对齐部分，去除了ECC空间的；
            if((sz+szu)>((1<<core->media->usz)-eccu))
                szu = ((1 << core->media->usz) -eccu) - szu; // 需读入大小，是需要跨unit的，按unit对齐计算大小；
            else
                szu = sz; // 小于一个unit内的数据量

            sz -= szu;
            bufc = (cx->pos / BUFLEN) % bufsmax; // 一个unit内已使用了的wbuf区；
            do
            {
                movs = szu;
                if((cx->wpos+movs)>BUFLEN)
                    movs = BUFLEN - cx->wpos;

                if((cx->pos+BUFLEN)<=file->size)
                {
                    // 只有尺寸大于BUFLEN才存在ECC；
                    if(__fix(core, (core->ebuf+(bufc<<BUFBITS)), BUFLEN, (u32*)(core->ebuf+(bufc<<BUFBITS)+BUFLEN)))
                    {
                        if(0xFFFFFFFF != *((u32*)(core->ebuf+(bufc<<BUFBITS)+BUFLEN)))
                            printf("\r\n: warn : efs    : \"%s\"'s data may corrupted(%ld).", handle_name(hdl), (long)(cx->pos));
                    }
                }

                memcpy((data+rdsz), (core->ebuf+(bufc<<BUFBITS)+cx->wpos), movs);
                rdsz += movs; // 已读数据量；
                szu -= movs;
                bufc ++;
                if((movs+cx->wpos)==BUFLEN)
                {
                    cx->wpos = 0; // 读超过wbuf范围，将wpos置空；
                    cx->pos += BUFLEN;
                }

                if((szu)&&(bufc==bufsmax))
                {
                    printf("\r\n: erro : efs    : \"%s\"'s read failed(%ld, bad logic).", handle_name(hdl), (long)(cx->pos));
                    return (rdsz);
                }
            }
            while(szu); // unit尺寸内的循环，一次大小为BUFLEN；
        }
        while(sz);
    }
    while(len);

    if(!len) // 将结尾不对齐的部分，缓存在wbuf；
    {
        rebuf -= cx->wpos;
        if(rebuf>0)
            memcpy((cx->wbuf+cx->wpos), (data+rdsz-rebuf), rebuf);

        cx->wpos += rebuf;
    }

    __unlock(core);
    return (rdsz);
}

// ============================================================================
// 功能：文件重定位；
// 参数：hdl -- 内部句柄；
//      offset -- 文件新位置；
//      whence -- 文件新位置的基准；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static off_t __e_seek(struct objhandle *hdl, off_t *offset, s32 whence)
{
    s64 movs = *offset, npos, units;
    s32 bufo, bufsmax, eccu;
    off_t position = -1;
    struct __loc *eloc;
    struct __econtext *cx = (struct __econtext *)handle_context(hdl);
    struct __ecore *core = (struct __ecore*)corefs(handle_GetHostObj(hdl));
    struct __efile *file = (struct __efile *)handle_GetHostObjectPrivate(hdl);

    eccu = (1 << (core->media->usz - BUFBITS)) * 4; // 一个unit内被ECC占用的空间
    switch(whence)
    {
        case SEEK_END:
        {
            movs = file->size + movs; // 转为SET逻辑；
        }

        case SEEK_SET:
        {
            movs = movs - (cx->pos + cx->wpos); // 转为CUR逻辑；
        }

        case SEEK_CUR:
        {
            if(!movs)
                break;

            npos = cx->pos + cx->wpos + movs; // 新位置
            if(npos<0)
            {
                printf("\r\n: erro : efs    : seek failed(out of range).");
                return (-1);
            }

            if(cx->wpos) // 有缓存
            {
                if((((movs>0)&&((movs-(BUFLEN-cx->wpos))>0)) || // 向前移动，且超过wbuf空间；
                   (((movs<0)&&(cx->wpos+movs)<0)))) // 向后移动，且超过wbuf空间；
                {
                    if(__e_sync(hdl)) // 将缓存数据刷入介质
                    {
                        printf("\r\n: erro : efs    : seek failed(sync).");
                        return (-1);
                    }

                    memset(cx->wbuf, 0xFF, BUFLEN); // 重置缓存
                    cx->wpos = 0;
                }
            }

            if(npos<=file->size) // seek后，将不对齐的部分缓存入wbuf（在文件尺寸 范围内）；尤其是wbuf范围内向后移动，将已存在介质的内容缓存进来；
            {
                if((movs<0)&&(cx->wpos+movs)>=0) // 向前移动，且范围在wbuf空间内；则不需要。
                {
                    cx->wpos += movs;
                }
                else if(npos%BUFLEN)
                {
                    eloc = __getloc(position, core, file->loc, npos);
                    units = (core->ssz * SYS_LOOPS) + (core->fsz * eloc->loc) + ((npos / ((1 << core->media->usz) - eccu)) % core->fsz);
                    if(core->drv->efs_read_media(core, units, core->ebuf))
                    {
                        printf("\r\n: erro : efs    : \"%s\"'s data cannot read(%ld).", handle_name(hdl), (long)npos);
                        return (-1);
                    }

                    bufsmax = 1 << (core->media->usz - BUFBITS); // 一个unit内有多少个buffer区；
                    bufo = ((npos / BUFLEN) % bufsmax) << BUFBITS; // wbuf区内偏置(含ECC)
                    if((file->size-(npos-(npos%BUFLEN)))>BUFLEN)
                    {
                        // 数据是存在ECC数据的，则先校验修复
                        if(__fix(core, (core->ebuf+bufo), BUFLEN, (u32*)(core->ebuf+bufo+BUFLEN)))
                        {
                            printf("\r\n: warn : efs    : \"%s\"'s data has corrupted(%ld).", handle_name(hdl), (long)npos);
                        }
                    }

                    movs = npos % BUFLEN - cx->wpos;
                    memcpy((cx->wbuf+cx->wpos), (core->ebuf+bufo+cx->wpos), movs); // 只会更新wbuf没有缓存的部分
                }

                cx->wpos += movs;
                cx->pos = npos - cx->wpos;
            }
            else // 新位置实际上是在扩展文件大小，没有实际文件内容；
            {
                if((movs+cx->wpos)>BUFLEN)
                {
                    cx->pos = npos;
                    cx->wpos = 0; // 不管新位置是否是wbuf对齐的，缓冲等写用时再去刷入，对于读逻辑上是不成立的；
                }
                else
                {
                    // 新的位置移动是小于wbuf，直接移动；不管wbuf内原来是什么，一方面不改变原来的数据，可以确保原来的修改得以保存。
                    cx->wpos += movs;
                }
            }

            break;
        }

        default : return (-1);
    }

    position = cx->pos + cx->wpos;
    return (position);
}

// ============================================================================
// 功能：删除文件
// 参数：ob -- 文件对象；
// 返回：成功（0）；失败（-1）；
// 备注：未考虑互斥；当pName为NULL时，表示文件正在被使用；
// ============================================================================
static s32 __e_remove(struct obj *ob, char *uncached)
{
    struct __loc *loc;
    struct __efile *cached = NULL;
    struct __ecore *core = (struct __ecore*)corefs(ob);

    __lock(core);
    if(uncached)
    {
        if(__e_lookupfile(core, uncached, &loc))
        {
            __unlock(core);
            return (-1);
        }
    }
    else
    {
        // 文件已缓存
        // cached = dListEntry(of_basiclinko(ob), struct __efile, basic);
        struct __efile *cached = (struct __efile *)obj_GetPrivate(ob);
        loc = cached->loc;
    }

    if(__e_destroyfile(core, loc))
    {
        printf("\r\n: dbug : efs    : cannot delete file info.");
        {
            __unlock(core);
            return (-1);
        }
    }

    if(cached)
    {
        //of_unlinkbasic(&file->basic);
        obj_SetPrivate(ob, 0);
        free(cached);
    }

    __unlock(core);
    return (0);
}

// ============================================================================
// 功能：查询文件；
// 参数：ob -- 文件节点；
//      data -- 文件信息；
//      uncached -- 未缓存文件名；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __e_stat(struct obj *ob, struct stat *data, char *uncached)
{
    s32 res;
    struct __loc *loc;
    struct __efile *file;
    struct __ecore *core = (struct __ecore*)corefs(ob);

    if((!uncached)&&(obj_isMount(ob)))
    {
        data->st_size = 0; // 安装点；
        data->st_mode = S_IFDIR;
    }
    else
    {
        if(uncached)
        {
            res = __e_lookupfile(core, uncached, &loc);
            if(res)
            {
                return (-1); // 文件不存在
            }

            file = __e_cachefile(core, loc, uncached, ob);
            if(!file)
            {
                return (-1); // 文件不可读
            }
        }
        else // 文件已打开；
        {
            //file = dListEntry(of_basiclinko(ob), struct __efile, basic);
            file = (struct __efile*)obj_GetPrivate(ob);
        }

        data->st_size = (off_t)file->size;
        data->st_mode = S_IFREG|S_IRUGO|S_IWUGO;
    }

    return (0);
}

// ============================================================================
// 功能：检查目录项是否已缓存，未缓存则缓存目录项；
// 参数：dentrys -- 目录项缓存体；
//      name -- 目录项名；
// 返回：成功（1）；失败（0）；
// 备注：
// ============================================================================
static s32 __checkdentry(struct __dentrys *dentrys, char *name)
{
    struct __dentry *uncached, *last, *cur;

    last = cur = dentrys->item;
    while(cur)
    {
        if(!strcmp(cur->name, name))
            return (0); // name已存在于链表

        last = cur;
        cur = cur->nxt;
    }

    uncached = malloc(strlen(name)+5);
    strcpy(uncached->name, name);
    uncached->nxt = NULL;
    if(last)
        last->nxt = uncached;
    else
        dentrys->item = uncached;

    dentrys->items++;
    return (1);
}

// ============================================================================
// 功能：缓存目录项；
// 参数：core -- efs文件系统管理；
//      dentrys -- 目录项缓存体；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __cachedentry(struct __ecore *core, struct __dentrys *dentrys)
{
    struct __efstruct fstruct;
    s32 idxs, i, j, news;
    struct __eidx *idx;
    s64 units;

    idxs = (1 << core->media->usz) / sizeof(*idx); // 一个unit中能存放多少个idx
    __lock(core);
    do
    {
        if(dentrys->scans >= core->fmax)
        {
            __unlock(core);
            return (-1); // 已全部遍历完；
        }

        units = core->serial * core->ssz + 1 + (dentrys->scans / idxs);
        if((u32)idxs>(core->fmax-dentrys->scans))
            idxs = core->fmax - dentrys->scans; //

        if(core->drv->efs_read_media(core, units, core->ebuf)) // 索引表
        {
            printf("\r\n: erro : efs    : lookup has problem(can not read %d).", i);
            i += idxs;
            continue; // 查询下一个索引表
        }

        idx = (struct __eidx *)core->ebuf;
        for(i=0; i<idxs; i++)
        {
            for(j=0; (u32)j<sizeof(*idx); j++)
            {
                if(0xFF!=((u8*)idx)[i])
                    break;
            }

            if(j!=sizeof(*idx))
            {
                for(j=0; (u32)j<sizeof(*idx); j++)
                {
                    if(0x0!=((u8*)idx)[i])
                        break;
                }
            }

            if(j!=sizeof(*idx))
            {
                units = core->serial * core->ssz + core->finfo + i;
                if(core->drv->efs_read_media(core, units, core->ebuf)) // 文件信息
                {
                    printf("\r\n: erro : efs    : file's info (%d) can not read", i);
                }

                __e_structfile(core, &fstruct, core->ebuf);
                if(__checkdentry(dentrys, fstruct.name.n))
                    news++;
            }

            idx++;
        }

        dentrys->scans += idxs; // 已遍历的
    }while(!news);

    __unlock(core);
    return (0);
}

// ============================================================================
// 功能：获取目录项；
// 参数：core -- 文件系统管理结构体；
//      dentrys -- 目录项缓存体；
//      offset -- 当前读取目录项位置；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static char *__getdentry(struct __ecore *core, struct __dentrys *dentrys, u32 offset)
{
    u32 i;
    struct __dentry *current = dentrys->item;

    if(offset>dentrys->items)
    {
        if(__cachedentry(core, dentrys))
            return (NULL);
    }

    for(i=0; i<offset; i++)
        current = current->nxt;

    return (current->name);
}

// ============================================================================
// 功能：释放缓存目录项；
// 参数：dentrys -- 目录项缓存体；
// 返回：
// 备注：
// ============================================================================
static void __uncachedentry(struct __dentrys *dentrys)
{
    struct __dentry *release, *current;

    if(!dentrys)
        return;

    current = dentrys->item;
    while(current)
    {
        release = current;
        current = current->nxt;
        free(release);
    }

    free(dentrys);
}

// ============================================================================
// 功能：读目录项；
// 参数：
// 返回：全部读完（1）；失败（-1）；读了一项（0）；
// 备注：
// ============================================================================
static s32 __e_readdentry(struct objhandle *directory, struct dirent *dentry)
{
    struct __ecore *core = (struct __ecore*)corefs(handle_GetHostObj(directory));
    struct __dentrys *dentrys = (struct __dentrys *)(dentry->d_ino);
    char *name;

    if(!dentry->d_ino)
    {
        dentrys = (struct __dentrys *)malloc(sizeof(struct __dentrys));
        if(dentrys)
        {
            printf("\r\n: erro : efs    : memory out when read dir \"%s\".", handle_name(directory));
            return (-1);
        }

        dentry->d_ino = (long)dentrys;
    }
    else
    {
        dentrys = (struct __dentrys *)(dentry->d_ino);
    }

    name = __getdentry(core, dentrys, (u32)dentry->d_off);
    if(!name)
    {
        __uncachedentry(dentrys); // 全部读完，释放
        return (1);
    }

    strcpy(dentry->d_name, name);
    dentry->d_type = DIRENT_IS_REG;
    return (0);
}

// ============================================================================
// 功能：文件系统接口；
// 参数：
// 返回：根据各命令要求
// 备注：
// ============================================================================
s32 e_operations(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                 ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
//    va_list list;
    s32 result = CN_OBJ_CMD_EXECUTED;

    switch(objcmd)
    {
        case CN_OBJ_CMD_OPEN:
        {
            struct objhandle *hdl;
            hdl = __e_open((struct obj *)opsTarget, (u32)(*(u64*)OpsArgs2), (char*)OpsArgs3);
            *(struct objhandle **)OpsArgs1 = hdl;
            break;
        }

      case CN_OBJ_CMD_READDIR:
      {
          struct objhandle *hdl = (struct objhandle*)OpsArgs3;
          struct dirent *ret = (struct dirent *)OpsArgs1;

          if((ptu32_t)__e_readdentry(hdl, ret) == 0)
              result = CN_OBJ_CMD_TRUE;
          else
              result = CN_OBJ_CMD_FALSE;
          break;
      }

        case CN_OBJ_CMD_READ:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = __e_read(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_WRITE:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = __e_write(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
            return((ptu32_t)__e_write((struct objhandle *)opsTarget, (u8*)OpsArgs1, (u32)OpsArgs2));
        }

        case CN_OBJ_CMD_CLOSE:
        {
            if(__e_close((struct objhandle*)opsTarget) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_SEEK:
        {
            *(off_t*)OpsArgs1 = __e_seek((struct objhandle *)opsTarget,
                                        (off_t*)OpsArgs2, (s32)OpsArgs3);
            break;
        }

        case CN_OBJ_CMD_DELETE:
        {
            if(__e_remove((struct obj*)opsTarget, (char *)OpsArgs3) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_STAT:
        {
            if(__e_stat((struct obj*)opsTarget, (struct stat *)OpsArgs1,
                                                (char*)OpsArgs3) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_SYNC:
        {
            if(__e_sync((struct objhandle *)opsTarget) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        default:
        {
            result = CN_OBJ_CMD_UNSUPPORT;
            break;
        }
    }

    return result;
}

// ============================================================================
// 功能：初始化efs对media的驱动
// 参数：core -- efs文件系统管理；  drv -- media的操作函数集
// 返回：0 -- 成功；  -1 --失败
// 备注：
// ============================================================================
int efs_install_drv(struct __ecore *core, struct __efs_drv *drv)
{
    if (!core || !drv)
         return (-1);

    core->drv->efs_erase_media = drv->efs_erase_media;
    core->drv->efs_read_media = drv->efs_read_media;
    core->drv->efs_write_media = drv->efs_write_media;
    return (0);
}

// ============================================================================
// 功能：安装文件系统；
// 参数：super -- 文件系统结构；
//      opts -- 安装方式；
//      config -- 文件系统配置；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 e_install(struct FsCore *pSuper, u32 opts, void *config)
{
    struct umedia *media;
    struct __ecore *core;
    s32 res, idxs, tmp, sys, i;
    s32 fileblock = (s32)config; // 文件块尺寸
//    static u8 installs;
//    char *lock = "efs0";
//    installs++;

    media = (struct umedia*)pSuper->MediaInfo;
    core = malloc(sizeof(*core) + (1 << media->usz));
    if(!core)
    {
        printf("\r\n: erro : efs    : install failed(memory out).");
        return (-1);
    }

    memset(core, 0x0, sizeof(*core));
    core->mutex = Lock_MutexCreate(NULL);
    if(!core->mutex)
    {
        free(core);
        printf("\r\n: erro : efs    : install failed(cannot create lock).");
        return (-1);
    }

    core->media = media;
    efs_install_drv(core,pSuper->MediaDrv);
    core->MStart = pSuper->MediaStart;
    core->ASize = pSuper->AreaSize;
    core->ebuf = (u8*)core + sizeof(*core);
    if(media->usz>9)
    {
        core->nsz = 256;
    }
    else
    {
        core->nsz = 128; // units尺寸小于512bytes时，名字长度缩小为128；
    }

    if(fileblock&((1<<media->esz)-1)) // fileblock按可擦除大小对齐
    {
        printf("\r\n: info : efs    : single file space's range expand from %d to ", fileblock);
        fileblock -= fileblock & ((1 << media->esz) - 1); // 剔除不对齐
        fileblock += 1 << media->esz; // 总体增加一个可擦除空间，对齐了
        printf("%d.", fileblock);
    }

    core->fsz = fileblock >> media->usz; // 单文件的容量，unit为单位；
    core->fmax = core->ASize / fileblock; // 预估文件数
    tmp = (1 << media->usz) / sizeof(struct __eidx); // 单个unit能存放的索引数
    if(core->fmax%tmp) // 索引空间按单个unit对齐操作
    {
        idxs = core->fmax / tmp + 1; // 索引空间需占用的unit数，unit为单位；
    }
    else
    {
        idxs = core->fmax / tmp; // 索引空间需占用的unit数，unit为单位；
    }

    core->finfo = 1 + idxs; // 文件信息的偏置，unit为单位;
    sys = 1 + idxs + core->fmax; // 预估系统空间大小（系统信息+索引+文件信息）,unit为单位
    tmp = ((1 << media->esz) >> media->usz); // 可擦除空间转为unit单位；
    if(sys%tmp) // 系统尺寸按可擦除大小对齐,unit为单位
    {
        sys = (sys - sys % tmp) + tmp;
    }

    core->ssz = sys; // 对齐后的系统空间，unit为单位
    sys = core->ssz * 6; // 全部系统部分逻辑所占用的空间（可擦除空间对齐，unit为单位）；
    tmp = (core->ASize - ((core->fmax * core->fsz) << media->usz)); // 预估系统可用空间的剩余空间
    tmp = tmp & (0xFFFFFFFF - ((1 << media->esz) -1)); // 预估系统可用空间的剩余空间, 可擦除对齐
    tmp = tmp >> media->usz; // // 预估系统可用空间的剩余空间，unit为单位
    if(tmp<sys) // 预估的系统空间不够，从文件空间里获取
    {
        i = 1;
        while(1)
        {
            if((tmp+i*core->fsz) >= (u32)sys)
                break;

            i++;
        }
    }

    if(core->fmax<=(u32)i)
    {
        printf("\r\n: erro : efs    : file system cannot create for bad logic.");
        free(core);
        return (-1);
    }

    core->fmax -= i; // 实际文件数量；(减掉系统空间需要的空间)
    res = __e_scan(core);
    if(1==res)
    {
        if(!(opts&MS_INSTALLCREAT))
        {
            printf("\r\n: erro : efs    : file system does not exist, but no create.");
            free(core);
            return (-1);
        }

        res = __e_build(core); // 建立efs文件系统；
        if(res)
        {
            printf("\r\n: erro : efs    : file system does not exist, and cannot create.");
            free(core);
            return (-1);
        }
    }
    else if (-1 == res)
    {
        printf("\r\n: erro : efs    : file system has corrupted.");
        free(core);
        return (-1);
    }

    pSuper->pCore = (void*)core;
    return (0);
}
