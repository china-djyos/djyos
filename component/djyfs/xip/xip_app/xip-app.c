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
#include <xip.h>
#include <device/unit_media.h>
#include <stdio.h>
#include <djyfs/filesystems.h>
#include <Iboot_info.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern s32 ModuleInstall_XIP_APP_FS(u32 opt, void *data);
//    ModuleInstall_XIP_APP_FS(0,NULL);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"xip app file system"//用于app的在线升级
//parent:"file system"//填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
//dependence:"file system"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_XIP_APP_FILE_SYSTEM == false )
//#warning  " xip_app_file_system  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_XIP_APP_FILE_SYSTEM    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure

// 底层接口函数
//

static s32 xip_app_fs_install(struct FsCore *super, u32 opt, void *config);
s32 xip_app_ops(void *opsTarget, u32 cmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3);

//
//
//
#define __FILE_NAME_MAX_LEN               (240) // 支持的最大文件名长度(包括结束符)

//
// xip文件用途标签
//
#define __TAG_RELEASE                      ((u32)0x61707000) // 发布模式的文件；
#define __TAG_DEBUG                        ((u32)0x00676264) // 调试模式的文件；

// xip文件状态
#define __STATUS_TEMP                   ((u32)0x1)
#define __STATUS_UPDATING                    ((u32)0x2)
#define __STATUS_UPDATED                     ((u32)0x3)


//extern s32 __ll_write(struct __icore *core, u8 *data, u32 bytes, u32 pos);
//extern s32 __ll_read(struct __icore *core, u8 *data, u32 bytes, u32 pos);
//extern s32 __ll_erase(struct __icore *core, u32 bytes, u32 pos);
//extern u32 __ll_crc32(struct __icore *core, u32 pos, u32 len);
// ============================================================================
// 功能：解析文件格式(头部)；
// 参数：head -- xip文件头部格式；
//      file -- xip文件；
// 返回：xip文件；
// 备注：
// ============================================================================
struct __ifile *xip_app_decodefilehead(void *head, struct __ifile *file)
{
    if(!file)
        return (NULL);

    if(false == XIP_AppFileCheckEasy(head))
    {
        //printf("\r\n: info : xipfs  : Error checking local app file header,检查本地的app时出错，不影响待升级的app。 \r\n");
        printf("\r\nInfo: the fireware was erased, so ready to write new firmware, ok!\r\n");
        return (NULL);
    }
    file->sz = XIP_GetAppSize(head);
    file->status = __STATUS_UPDATED;
    return (file);
}

//==========================================================================
// 功能：文件上锁
// 参数：
// 返回：
// 备注：
// ============================================================================
static inline void xip_app_lock(struct __icore *core)
{
    Lock_MutexPend(core->lock, CN_TIMEOUT_FOREVER);
}

// ============================================================================
// 功能：文件解锁
// 参数：
// 返回：
// 备注：
// ============================================================================
static inline void xip_app_unlock(struct __icore *core)
{
    Lock_MutexPost(core->lock);
}

// ============================================================================
// 功能：新建xip文件数据结构
// 参数：core -- xip文件系统信息；
// 返回：成功（xip文件）； 失败(NULL);
// 备注：
// ============================================================================
static struct __ifile *xip_app_newfile(struct __icore *core)
{
    struct __ifile *file;
    u32 size = sizeof(*file);

    core = core;
    file = malloc(size);
    if(!file)
        return (NULL);

    memset(file, 0x0, size);
    file->cxbase = Iboot_GetAppHeadSize();
    file->status = __STATUS_TEMP;
    return (file);
}

// ============================================================================
// 功能： 文件头部位置
// 参数：file -- xip文件
// 返回：文件头部位置偏置
// 备注：
// ============================================================================
static inline u32 xip_app_locatefilehead(struct __ifile *file)
{
    return (file->cxbase - Iboot_GetAppHeadSize());
}

// ============================================================================
// 功能： 格式化xip文件（头部）
// 参数：core -- xip文件系统信息；
//      file -- xip文件；
// 返回：失败（-1）；成功（0）。
// 备注：
// ============================================================================
s32 xip_app_formatfilehead(struct __icore *core, struct __ifile *file)
{
    u32 base = 0;

    if(file)
        base = xip_app_locatefilehead(file);

   return (core->drv->xip_erase_media(core, Iboot_GetAppHeadSize(), base));
}

// ============================================================================
// 功能：建立文件格式（头部）
// 参数：core -- xip文件系统信息；
//      file -- xip文件；
//      name -- xip文件名；
// 返回：失败（-1）；成功（0）。
// 备注：
// ============================================================================
s32 xip_app_makefilehead(struct __icontext *cx,struct __icore *core,
                                    struct __ifile *file, const char *name)
{
    if(file->cxbase != cx->Wappsize)
        return -1;
    file->sz += file->cxbase;
    Iboot_RewriteAppHeadFileInfo(cx->apphead,name,file->sz);
    if(-1 == core->drv->xip_write_media(core, cx->apphead, file->cxbase, xip_app_locatefilehead(file)))
        return (-1);

    file->status = __STATUS_UPDATED;
    return (0);
}

// ============================================================================
// 功能：扫描xip文件
// 参数：core -- xip文件系统信息；
// 返回：失败，未找到文件（-1）；成功找到文件（0）。
// 备注：
// ============================================================================
static s32 xip_app_scanfiles(struct __icore *core)
{
    s32 res;
    u32 size = Iboot_GetAppHeadSize();
    u8 * structFileHead = malloc(size);
    char *name;
    struct __ifile *file=NULL;
    memset(structFileHead, 0xff, size);
    res = core->drv->xip_read_media(core, structFileHead,size, 0);
    if(res)
        goto Error;

    // 当前只有一个文件
    file = xip_app_newfile(core);
    if(NULL == xip_app_decodefilehead(structFileHead, file))
    {   // 当前逻辑不在上电检索文件的时候格式整个空间,而只格式一个头部。
        // 必须要这个逻辑，因为在升级过程的中断，往往是头部不存在，而后续有内容。
        // 而第一次写入时，并不想照顾这个逻辑。
        res = xip_app_formatfilehead(core, file);
        goto Error; // 当前系统已无文件，后续逻辑不执行
    }
    name = Iboot_GetAppName(structFileHead);
    // 将内容接入文件系统
    if(!OBJ_NewChild(core->root, xip_app_ops, (ptu32_t)file, name))
    {
        goto Error;
    }
//  obj_Close(core->root->child);   //  把子节点引用次数减一，因为xip的文件并没有打开，只是事先加到obj里
    printf("\r\n: info : xipfs  : valid file found, name(%s), size(%dKB).", name, (file->sz>>10));
    free(file);
    free(structFileHead);
    return (0);
Error:
    free(file);
    free(structFileHead);
    return (-1);
}

// ============================================================================
// 功能：释放xip文件（内存中的空间）
// 参数：file -- xip文件；
// 返回：
// 备注：INLINE
// ============================================================================
static void xip_app_freefile(struct __ifile *file)
{
    if(file->status == __STATUS_UPDATING)
        return;

//    of_unlinkbasic(&file->basic);
//    Lock_MutexDelete(file->lock);
    free(file);
}

// ============================================================================
// 功能：删除xip文件，包括存储介质上的数据
// 参数：core -- xip文件系统信息；
//      file -- xip文件；
// 返回：失败（-1）；成功（0）。
// 备注：INLINE
// ============================================================================
static s32 xip_app_delfile(struct __icore *core, struct __ifile *file)
{
    if(core->drv->xip_erase_media(core, Iboot_GetAppHeadSize(), xip_app_locatefilehead(file)))
        return (-1);

    xip_app_freefile(file);
    return (0);
}

// ============================================================================
// 功能：新建xip文件上下文；
// 参数：core -- xip文件系统信息；
// 返回：成功（xip文件上下文）；失败（NULL）；
// 备注：
// ============================================================================
static struct __icontext *xip_app_newcontext(struct __icore *core)
{
    struct __icontext *cx;
    u32 appheadsize = Iboot_GetAppHeadSize();

    cx = malloc(sizeof(*cx) + core->bufsz+appheadsize);
    if(!cx)
        return (NULL);

    memset(cx, 0, sizeof(*cx) + core->bufsz+appheadsize);
    cx->pos = 0;
    cx->buf = (u8*)cx + sizeof(*cx);
    cx->apphead = cx->buf+core->bufsz;
    cx->Wappsize = 0;
    cx->bufed = 0;
    return (cx);
}

// ============================================================================
// 功能：释放xip文件上下文
// 参数：pContext -- xip文件上下文；
// 返回：
// 备注：INLINE
// ============================================================================
void xip_app_freecontext(struct __icontext *pContext)
{
    return (free(pContext));
}

// ============================================================================
// 功能：打开文件
// 参数：ob -- 文件对象；
//      flags -- 文件操作标志位；
//      uncached -- 需新建的文件名称；
// 返回：成功（xip文件上下文）；失败（NULL）；
// 备注：
// ============================================================================
static struct objhandle *xip_app_open(struct Object *ob, u32 flags, char *uncached)
{
    s32 size;
    struct objhandle *hdl;
    struct Object *tmp;
    struct __ifile *file = NULL;
    struct __icontext *cx = NULL;
    struct __icore *core = (struct __icore*)File_Core(ob);
//    mode_t mode;
    xip_app_lock(core);
    if((!uncached)&&(File_ObjIsMount(ob))) // 根目录
    {
        if(!Handle_FlagIsDirectory(flags))
        {
            xip_app_unlock(core);
            return (NULL);
        }
    }
    if(uncached)
    {
    //关闭文件后文件的obj会从mount点的obj中删除，所以如果mount点的obj中没有文件的obj则先从flash中获取该文件信息
        xip_app_scanfiles(core);//扫描文件
        do
        {
            if(strcmp(uncached,OBJ_GetName(ob)) == 0)
            {
                uncached = NULL;
                break;
            }
            tmp = OBJ_GetChild(ob);
            if(tmp == NULL)
            {
                break;
            }
            ob = tmp;
        }while(1);
    }
    if(Handle_FlagIsDirectory(flags)) // 目录逻辑
    {
        xip_app_scanfiles(core);//扫描文件
        if(uncached)// 不支持新建目录
        {
            printf("\r\n: info : xipfs  : do not support create directory.");
            xip_app_unlock(core);
            return (NULL);
        }
    }
    else // 文件的逻辑
    {
        if(uncached) // 文件不存在，需要新建。（文件都是已缓存的）
        {
            if(!Handle_FlagIsCreate(flags))
            {
                xip_app_unlock(core);
                return (NULL); // 打开操作中无新建要求，则返回不存在；
            }

            // 判断xip文件系统是不是已经有文件了，如果存在是否正在使用；
            tmp = OBJ_GetChild(core->root);
            if(tmp)
            {
                if(Handle_FlagIsWriteable(flags))
                {
                    if(OBJ_IsOnDuty(tmp))
                    {
                        printf("\r\n: info : xipfs  : cannot create new for old are using.");
                        xip_app_unlock(core);
                        return (NULL);
                    }
                    ob = OBJ_GetParent(tmp);
                    if(OBJ_Delete(tmp))        //xip文件系统，只要存在写一个不存在文件的操作，则直接覆盖里面原来的文件
                    {
                        printf("\r\n: info : xipfs  : cannot create new for old cannot delete.");
                        xip_app_unlock(core);
                        return (NULL);
                    }
                }
                else
                {
                    printf("\r\n: info : xipfs  : \"%s\"  file nonentity.", uncached);
                    xip_app_unlock(core);
                    return (NULL);
                }
            }
            file = xip_app_newfile(core);
            if(!file)
            {
                xip_app_unlock(core);
                return (NULL);
            }

            xip_app_formatfilehead(core, file);
#if 0
            if(of_virtualize(ob, &file->basic, uncached)) // xip文件，链入文件系统
                return (NULL);
//#else
//            if(!OBJ_NewChild(core->root, xip_app_ops, (ptu32_t)file, uncached))
//            {
//                xip_app_unlock(core);
//                return (NULL);
//            }
#endif
        }
        else // 文件已存在
        {
            if(Handle_FlagIsOnlyCreate(flags))
            {
                xip_app_unlock(core);
                return (NULL); // 必须新建逻辑，但文件已存在
            }

            file = (struct __ifile*)OBJ_GetPrivate(ob);
            if(Handle_FlagIsTrunc(flags))
            {
                if(-1 == xip_app_formatfilehead(core, file))
                {
                    xip_app_unlock(core);
                    return (NULL);
                }

                file->sz = 0;
                file->status = __STATUS_UPDATING;
            }
        }

        cx = xip_app_newcontext(core);
        if(!cx)
        {
            if(uncached)
                xip_app_freefile(file); // 释放掉上面创建的

            xip_app_unlock(core);
            return (NULL);
        }

        // 预缓存逻辑
        if(file->sz<core->inhead)
        {
            size = file->sz;
        }
        else
        {
            size = core->inhead;
        }

        if(Handle_FlagIsAppend(flags))
        {
            cx->pos = file->sz;
            if(file->sz<core->inhead)
                size = file->sz;
            else if(size == (s32)file->sz)
                size = 0;
            else
                size = (file->sz - core->inhead) % core->bufsz;

            cx->bufed = size;
        }
        else
        {
            size = core->inhead;
            cx->bufed = 0;
        }

        if(core->drv->xip_read_media(core, cx->buf, size, (cx->pos+file->cxbase-cx->bufed)))
        {
            if(uncached)
                xip_app_freefile(file); // 释放掉上面创建的

            xip_app_unlock(core);
            return (NULL);
        }
    }

    hdl = Handle_New();
    if(!hdl)
    {
        xip_app_freecontext(cx);
        if(uncached)
            xip_app_freefile(file); // 释放掉上面创建的
    }

    Handle_Init(hdl, NULL, flags, (ptu32_t)cx);

    if(hdl)
    {
        //TODO：从yaffs2中读取权限等，暂时赋予全部权限。
//        mode = S_IALLUGO | S_IFREG;     //建立的路径，属性是目录。
        //继承操作方法，对象的私有成员保存访问模式（即 stat 的 st_mode ）
        ob = OBJ_BuildTempPath(ob, xip_app_ops, (ptu32_t)file,uncached);
        OBJ_LinkHandle(hdl, ob);
    }
    xip_app_unlock(core);
    return (hdl);
}

// ============================================================================
// 功能：关闭文件
// 参数：hdl -- xip文件对象句柄；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 xip_app_close(struct objhandle *hdl)
{
    s32 size;
    struct __icore *core;
    struct __ifile *file;
    struct __icontext *cx = (struct __icontext*)Handle_GetContext(hdl);

    if(cx) // NULL时表示目录
    {
        core = (struct __icore*)File_Core(Handle_GetHostObj(hdl));
        file = (struct __ifile*)handle_GetHostObjectPrivate(hdl);
        xip_app_lock(core);
        if(__STATUS_UPDATED != file->status) // 数据存在写入操作或者文件是新建的
        {
            if(cx->bufed)
            {
                if(cx->pos<=core->inhead)
                    size = core->inhead;
                else
                    size = core->bufsz;

                if(-1 == core->drv->xip_write_media(core, cx->buf, size, (cx->pos-cx->bufed+file->cxbase)))
                {
                    xip_app_unlock(core);
                    return (-1);
                }
            }

            if(!iscontender(hdl)) // 最后一个文件使用者关闭文件时，才会设置文件头
            {
                if(xip_app_makefilehead(cx,core, file, Handle_GetName(hdl)))
                {
                    xip_app_unlock(core);
                    return (-1);
                }
            }
        }

        xip_app_unlock(core);
    }

    xip_app_freecontext(cx);
//  Handle_Delete(hdl);
    return (0);
}

// ============================================================================
// 功能：写文件
// 参数：hdl -- xip文件对象句柄；
//      data -- 需写入数据的空间；
//      size -- 需写入数据的大小；
// 返回：实际写入的字节数；
// 备注：
// ============================================================================
static s32 xip_app_write(struct objhandle *hdl, u8 *data, u32 size)
{
    s32 pos, once, free, res;
    u32 left;
    struct __icontext *cx = (struct __icontext *)Handle_GetContext(hdl);
    struct __icore *core = (struct __icore*)File_Core(Handle_GetHostObj(hdl));
    struct __ifile *file = (struct __ifile*)handle_GetHostObjectPrivate(hdl);
    u32 wsize = 0;

    if(cx->Wappsize < file->cxbase)
    {
        if(size > (file->cxbase - cx->Wappsize))
        {
            wsize = (file->cxbase - cx->Wappsize);
            memcpy(&cx->apphead[cx->Wappsize],data,wsize);
            size-= wsize;
            data+=wsize;
            cx->Wappsize+=wsize;
        }
        else
        {
            memcpy(&cx->apphead[cx->Wappsize],data,size);
            cx->Wappsize+=size;
            return size;
        }
    }

    left = size;               //第一次应该减去文件头信息.故需要写的字节数应放这里
    xip_app_lock(core);
    if(cx->pos<=core->inhead) // 缓存中剩余可写空间；（连续写和不连续写会有这么处理，256时）
        free = core->inhead - cx->pos; // 在开始的区域中，文件头部占据了固定空间；
    else
        free = core->bufsz - ((cx->pos - core->inhead) % core->bufsz);

    memset(cx->buf, 0xff, core->bufsz);
    while(left)
    {
        once = left;
        if(once>free) // 单次先写入到缓存中；
            once = free;
        memcpy(cx->buf+cx->bufed, data, once);
        cx->bufed += once;
        if((cx->bufed==core->bufsz) || // 缓冲已满，刷入
           ((cx->bufed==(s16)core->inhead) // 文件头部占据了固定空间，此情况也满（对齐）
             &&(((cx->pos+once)<=core->inhead)))) // (等于零的情况用于存在于seek逻辑)
        {
            pos = cx->pos - (cx->bufed - once) + file->cxbase; // 当前位置写入时的位置，缓存对齐了的；
            res = core->drv->xip_write_media(core, cx->buf, cx->bufed, pos);// 此时bufed的大小为bufsz或者inhead；
            if(-1==res) // 写错误；
            {
                break;
            }
            else if(-2==res) // 将要没有可写空间，删除一些；
            {
                if(core->drv->xip_erase_media(core, core->bufsz, (pos+cx->bufed+1)))// '+1'表示下一个要写的空间地址；
                    break;
            }

            // 缓存重置，将后续的缓存预取进来（第一个head区域是不会在这里需要缓存的）
            // 此后seek就直接移动bufed;
            if(left < (u32)core->bufsz)
            {
                if(core->drv->xip_read_media(core, cx->buf, core->bufsz, pos+cx->bufed))
                    break;
            }
            cx->bufed = 0;
            memset(cx->buf, 0xff, core->bufsz);
        }

        left -= once;
        data += once;
        cx->pos += once;
        free = core->bufsz;
    }

    if(file->sz<cx->pos)
        file->sz = cx->pos;

    if((__STATUS_UPDATING != file->status)&&(left != size))
        file->status = __STATUS_UPDATING; // 文件数据发生变改变

    xip_app_unlock(core);
    return (wsize+size-left);
}
static s32 xip_app_sync(struct objhandle *hdl)
{
    s32 size;
    struct __icontext *cx = (struct __icontext *)Handle_GetContext(hdl);
    struct __icore *core = (struct __icore*)File_Core(Handle_GetHostObj(hdl));
    struct __ifile *file = (struct __ifile*)handle_GetHostObjectPrivate(hdl);
    xip_app_lock(core);
    if(__STATUS_UPDATED != file->status) // 数据存在写入操作或者文件是新建的
    {
        if(cx->bufed)
        {
            if(cx->pos<=core->inhead)
                size = core->inhead;
            else
                size = core->bufsz;

            if(0 != core->drv->xip_write_media(core, cx->buf, size, (cx->pos-cx->bufed+file->cxbase)))
            {
                xip_app_unlock(core);
                return (-1);
            }
        }
    }

    xip_app_unlock(core);
    return (0);
}

// ============================================================================
// 功能：读文件
// 参数：hdl -- xip文件对象句柄；
//      data -- 需读出数据的存放空间；
//      size -- 需读出数据的大小；
// 返回：实际读出的单元数；
// 备注：
// ============================================================================
static s32 xip_app_read(struct objhandle *hdl, u8 *data, u32 size)
{
    s32 once, left = size;
    struct __icontext *cx = (struct __icontext*)Handle_GetContext(hdl);
    struct __icore *core = (struct __icore*)File_Core(Handle_GetHostObj(hdl));
    struct __ifile *file = (struct __ifile*)handle_GetHostObjectPrivate(hdl);

    xip_app_lock(core);
    if((cx->pos+size)>file->sz)
        size = file->sz - cx->pos; // 不能读越界

    // 获取预缓存空间；一方面文件是预先缓存在buf中，且是对齐；另一方面size是不会大于文件大小；
    if(cx->pos<=core->inhead)
        once = core->inhead - cx->pos; // 在开始的区域中，文件头部占据了固定空间；
    else
        once = (core->bufsz - (cx->pos - core->inhead)) % core->bufsz;

    if(once>=(s32)size)
    {
        // 这里会存在一个隐患，就是读的数据可能和介质中的不一样，
        // 不执行这个逻辑，效率就差，而且在ecc上可能会有麻烦；
        memcpy(data, cx->buf+cx->bufed, size);
        cx->pos += size;
        cx->bufed += size;
        return (size);
    }
#if 0
    // 缓冲的剩余空间容纳不了回读的数据，则先将数据刷下去，再回读。
    pos = cx->pos - cx->bufed + file->cxbase; // 缓存对齐
    if(-1 == core->drv->xip_write_media(core, cx->buf, (cx->bufed+once), pos)) // 缓存整体写
    {
        xip_app_unlock(core);
        return (0); // 缓存数据刷入失败；
    }
#endif
    memcpy(data, cx->buf+cx->bufed, once); // 将缓存上数据线提取出来；
    cx->pos += once; // 此后pos逻辑上是缓存对齐的；
    left -= once;
    data += once;
    cx->bufed += once;
    while(left)
    {
        cx->bufed = 0;
        if(core->drv->xip_read_media(core, cx->buf, core->bufsz, cx->pos+file->cxbase))
        {
            xip_app_unlock(core);
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

    xip_app_unlock(core);
    return (size);
}

// ============================================================================
// 功能：设置文件当前位置；
// 参数：hdl -- xip文件对象句柄；
//      offset -- 移动位置的量；
//      whence -- 移动位置的起点；
// 返回：成功（0，*pOffset指向新的当前位置）；失败（-1）；
// 备注：
// ============================================================================
static off_t xip_app_seek(struct objhandle *hdl, off_t *offset, s32 whence)
{
    struct __icontext *cx;
    struct __ifile *file;
    struct __icore *core;
    s32  npos, movs, pos;
    off_t position = *offset;
#if 0 // 应该由上级逻辑判断
    if(File_IsDirectory(hdl))
    {
        printf("\r\n: dbug : xipfs  : cannot seek directory.");
        return (-1);
    }
#endif

    core = File_Core(Handle_GetHostObj(hdl));
    cx = (struct __icontext*)Handle_GetContext(hdl);
    //file = dListEntry(of_basic(hdl), struct __ifile, basic);
    file = (struct __ifile*)handle_GetHostObjectPrivate(hdl);
    xip_app_lock(core);
    switch(whence)
    {
        case SEEK_END: // 转为从头SEEK逻辑
        {
            position = file->sz + position;
            if(position<0)
                position = 0; // 新位置越界了
        }
        __attribute__((fallthrough));

        case SEEK_SET: // 转为当前位置的SEEK的逻辑
        {
            position = position - cx->pos;
            if((position+cx->pos)<0)
                position = cx->pos; // 新位置越界了
        }

        case SEEK_CUR:
        {
            npos = position + cx->pos;
            if(npos<0)
                npos = 0;

            // 如果是在缓存范围内的移动，则直接返回；
            movs = npos - cx->pos; // 移动的方向；
            if(movs<0)
            {
                if((movs+cx->bufed)>=0)
                {
                    cx->bufed -= movs;
                    position = cx->pos = npos;
                    return (position);
                }
            }
            else if(movs>0)
            {
                if(cx->pos<=core->inhead)
                {
                    if((u32)(movs+cx->bufed)<core->inhead)
                    {
                        cx->bufed += movs;
                        position = cx->pos = npos;
                        return (position);
                    }
                }
                else
                {
                    if((movs+cx->bufed)<core->bufsz)
                    {
                        cx->bufed += movs;
                        position = cx->pos = npos;
                        return (position);
                    }
                }
            }
            else // 不需要移动；
            {
                position = cx->pos;
                return (position);
            }

            // 如果超出在缓存范围内的移动，将缓存刷入介质
            pos = cx->pos - cx->bufed + file->cxbase;
            if(cx->pos<core->inhead)
            {
                if(-1 == core->drv->xip_write_media(core, cx->buf, core->inhead, pos))
                    return (-1);
            }
            else
            {
                if(-1 == core->drv->xip_write_media(core, cx->buf, core->bufsz, pos))
                    return (-1);
            }

            if(npos<=(s32)core->inhead) // 缓存新的位置
            {
                if(core->drv->xip_read_media(core, cx->buf, core->inhead, file->cxbase))
                {
                    memset(cx->buf, 0xFF, core->bufsz);
                    cx->bufed = 0;
                    return (-1);
                }

                cx->bufed = npos;
            }
            else
            {
                pos = npos - (npos + file->cxbase) % core->bufsz + file->cxbase;
                if(core->drv->xip_read_media(core, cx->buf, core->bufsz, pos))
                {
                    memset(cx->buf, 0xFF, core->bufsz);
                    cx->bufed = 0;
                    return (-1);
                }

                cx->bufed = (npos - core->inhead) % core->bufsz;
            }

            position = cx->pos = npos;
            return (position);
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
// 参数：ob -- xip文件对象；
// 返回：成功（0）；失败（-1）；
// 备注：未考虑互斥；当pName为NULL时，表示文件正在被使用；
// ============================================================================
static s32 xip_app_remove(struct Object *ob)
{
    struct __ifile *file;
    struct __icore *core;

    core = (struct __icore *)File_Core(ob);
    file = (struct __ifile*)OBJ_GetPrivate(ob);
    return (xip_app_delfile(core, file));
}

// ============================================================================
// 功能：文件查询
// 参数：ob -- xip文件对象；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 xip_app_stat(struct Object *ob, struct stat *data)
{
    struct __ifile *file;

    if(File_ObjIsMount(ob))
    {
        data->st_size = 0;
        data->st_mode = S_IFDIR;
    }
    else
    {
        file = (struct __ifile*)OBJ_GetPrivate(ob);
        data->st_size = file->sz;
        data->st_mode = S_IFREG|S_IRWXUGO;
    }

    return (0);
}

// ============================================================================
// 功能：读xip文件系统目录项；
// 参数：hdl -- xip文件对象句柄；
//      dentry -- 目录项；
// 返回：全部读完（1）；失败（-1）；读了一项（0）；
// 备注：
// ============================================================================
static s32 xip_app_readdentry(struct objhandle *hdl, struct dirent *dentry)
{
    struct Object *ob = Handle_GetHostObj(hdl);

    ob = OBJ_GetChild(ob);
    if((ob)&&(dentry->d_ino!=(long)ob))
    {
        dentry->d_ino = (long)ob;
        strcpy(dentry->d_name, OBJ_GetName(ob));
        dentry->d_type = DIRENT_IS_REG;
        return (0);
    }

    return (1);
}
// ============================================================================
// 功能：初始化xip对media的驱动
// 参数：core -- efs文件系统管理；  drv -- media的操作函数集
// 返回：0 -- 成功；  -1 --失败
// 备注：
// ============================================================================
int xip_app_install_drv(struct __icore *core, struct __xip_drv *drv)
{
    if (!core || !drv)
         return (-1);

    core->drv = malloc(sizeof(*drv));
    core->drv->xip_erase_media = drv->xip_erase_media;
    core->drv->xip_read_media = drv->xip_read_media;
    core->drv->xip_write_media = drv->xip_write_media;
    return (0);
}
// ============================================================================
// 功能：安装xip文件系统
// 参数：super -- 文件系统管理信息；
//      opt -- 安装逻辑标志位；
//      config -- 自定义数据；
// 返回：成功(0)； 失败(-1)。
// 备注：
// ============================================================================
static s32 xip_app_fs_install(struct FsCore *super, u32 opt, void *config)
{

    struct __icore *core;
    struct umedia *um;

    config = config;
    opt = opt;
    core = malloc(sizeof(*core));
    if(!core)
    {
        printf("\r\n: erro : xipfs  : install failed(memory out).");
        return (-1);
    }

    memset(core, 0x0, sizeof(*core));
    um = (struct umedia*)super->MediaInfo;
    if(!um)
    {
        free(core);
        return (-1);
    }
//    um->mreq(unitbytes,(ptu32_t)&flash_page_size);
    xip_app_install_drv(core,super->MediaDrv);
    core->ASize = super->AreaSize;
    core->MStart = super->MediaStart;
    core->vol = (void*)um;
    core->bufsz = (s16)512; // xip文件系统文件的缓存大小依据unit的尺寸；todo：512哪里来的？
    if(core->bufsz<(s16)Iboot_GetAppHeadSize())
    {
        free(core);
        return (-1);
    }

    core->inhead = core->bufsz - Iboot_GetAppHeadSize();
    core->root = super->pTarget;
    core->lock = Lock_MutexCreate("xip-app fs");
    if(!core->lock)
    {
        free(core);
        return (-1);
    }
    xip_app_scanfiles(core); // 扫描已存在文件

    super->pCore = (void*)core;
    return (0);
}

// ============================================================================
// 功能：xip文件操作接口
// 参数：标准逻辑，查看接口说明；
// 返回：标准逻辑，查看接口说明；
// 备注:
// ============================================================================
s32 xip_app_ops(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
//    va_list list;
    s32 result = CN_OBJ_CMD_EXECUTED;
    switch(objcmd)
    {
        case CN_OBJ_CMD_OPEN:
        {
            struct objhandle *hdl;
            hdl = xip_app_open((struct Object *)opsTarget, (u32)(*(u64*)OpsArgs2), (char*)OpsArgs3);
            *(struct objhandle **)OpsArgs1 = hdl;
            break;
        }

      case CN_OBJ_CMD_READDIR:
      {
          struct objhandle *hdl = (struct objhandle*)OpsArgs3;
          struct dirent *ret = (struct dirent *)OpsArgs1;

          if((ptu32_t)xip_app_readdentry(hdl, ret) == 0)
              result = CN_OBJ_CMD_TRUE;
          else
              result = CN_OBJ_CMD_FALSE;
          break;
      }

        case CN_OBJ_CMD_READ:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = xip_app_read(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_WRITE:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = xip_app_write(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_CLOSE:
        {
            if(xip_app_close((struct objhandle*)opsTarget) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_SEEK:
        {
            *(off_t*)OpsArgs1 = xip_app_seek((struct objhandle *)opsTarget,
                                        (off_t*)OpsArgs2, (s32)OpsArgs3);
            break;
        }

        case CN_OBJ_CMD_DELETE:
        {
            if(xip_app_remove((struct Object*)opsTarget) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_STAT:
        {
            char * path = (char*)OpsArgs2;
            if(path&&('\0'!=*path))
                return (-1); // 查询的文件不存在；
            if(xip_app_stat((struct Object*)opsTarget, (struct stat *)OpsArgs1) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_SYNC:
        {
            if((ptu32_t)xip_app_sync((struct objhandle*)opsTarget) == 0)
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

__attribute__((weak)) s32 xip_fs_format(void *core)
{
    return 0;
}
#pragma GCC diagnostic pop

// ============================================================================
// 功能：安装xip文件系统
// 参数：target -- 安装目录；
//      opt -- 文件系统配置选项；如MS_INSTALLCREAT
//      data -- 传递给xip安装逻辑的数据；
// 返回：失败(-1)； 成功(0)。
// 备注:
// ============================================================================
s32 ModuleInstall_XIP_APP_FS(u32 opt, void *data)
{
    struct Object * mountobj;
    static struct filesystem *typeXIPAPP = NULL;
    s32 res;

    if(typeXIPAPP == NULL)
    {
        typeXIPAPP = malloc(sizeof(*typeXIPAPP));

        typeXIPAPP->fileOps = xip_app_ops;
        typeXIPAPP->install = xip_app_fs_install;
        typeXIPAPP->pType = "XIP-APP";
        typeXIPAPP->format = xip_fs_format;
        typeXIPAPP->uninstall = NULL;
    }
    res = File_RegisterFs(typeXIPAPP);
    if(-1==res)
    {
        printf("\r\n: dbug : module : cannot register \"XIP-APP\"(file system type).");
        return (-1); // 失败;
    }

    mountobj = OBJ_NewChild(OBJ_GetRoot(), __File_MountOps, 0, CN_XIP_APP_MOUNT);
    if(NULL == mountobj)
    {
        printf("\r\n: dbug : module : mount \"xip\" failed, cannot create \"%s\"(target).", CN_XIP_APP_MOUNT);
        return (-1);
    }
//    OBJ_DutyUp(mountobj);
    opt |= MS_DIRECTMOUNT;      //直接挂载不用备份
    res = File_Mount(NULL, CN_XIP_APP_MOUNT, "XIP-APP", opt, data);
    if(res == -1)
    {
        printf("\r\n: dbug : module : mount \"XIP-APP\" failed, cannot install.");
        OBJ_Delete(mountobj);
        return (-1);
    }

    return (0);
}


