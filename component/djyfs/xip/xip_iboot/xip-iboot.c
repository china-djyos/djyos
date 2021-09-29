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
#include "Iboot_info.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern s32 ModuleInstall_XIP_IBOOT_FS(u32 opt, void *data);
//    ModuleInstall_XIP_IBOOT_FS(0,NULL);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述组件描述开始
//component name:"xip iboot file system"//用于iboot的在线升级
//parent:"file system"//填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"file system"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_XIP_IBOOT_FILE_SYSTEM == false )
//#warning  " xip_iboot_file_system  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_XIP_IBOOT_FILE_SYSTEM    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
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


u64 FileNowPos = 0;
//static const char *xip_iboot_target = "xip-iboot";
// 底层接口函数
//
//extern s32 xip_fs_format(void *core);
static s32 xip_iboot_fs_install(struct FsCore *super, u32 opt, void *config);
s32 xip_iboot_ops(void *opsTarget, u32 cmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

__attribute__((weak)) s32 xip_fs_format(void *core)
{
    return 0;
}

//==========================================================================
// 功能：文件上锁
// 参数：
// 返回：
// 备注：
// ============================================================================
static inline void xip_iboot_lock(struct __icore *core)
{
    Lock_MutexPend(core->lock, CN_TIMEOUT_FOREVER);
}

// ============================================================================
// 功能：文件解锁
// 参数：
// 返回：
// 备注：
// ============================================================================
static inline void xip_iboot_unlock(struct __icore *core)
{
    Lock_MutexPost(core->lock);
}

// ============================================================================
// 功能：打开文件
// 参数：ob -- 文件对象；
//      flags -- 文件操作标志位；
//      uncached -- 需新建的文件名称；
// 返回：成功（xip-iboot文件句柄）；失败（NULL）；
// 备注：
// ============================================================================
static struct objhandle *xip_iboot_open(struct Object *ob, u32 flags, char *uncached)
{
    struct objhandle *hdl = NULL;
    struct __icore *core = (struct __icore*)File_Core(ob);
//    mode_t mode;

    if((Handle_FlagIsWriteable(flags) == 0) || (Handle_FlagIsDirectory(flags) == 1))
    {
        printf("\r\n: error :cannot open the xip-iboot directory and the files in it only support write mode.");
        return (NULL);    //xip-iboot只支持写模式
    }

    xip_iboot_lock(core);

//    if(strcmp(OBJ_GetName(ob),EN_XIP_IBOOT_TARGET) == 0)      //判断访问的路径是不是xip-iboot，如果不是则直接返回NULL
//    {
        xip_fs_format(core);        //擦除iboot所在的flash区域

//        if(!OBJ_NewChild(core->root, xip_iboot_ops, (ptu32_t)0, uncached))
//        {
//            xip_iboot_unlock(core);
//            return (NULL);
//        }

        hdl = Handle_New();
        if(!hdl)
        {
            return (NULL);
        }

        Handle_Init(hdl, NULL, flags, 0);

        if(hdl)
        {
            //TODO：从yaffs2中读取权限等，暂时赋予全部权限。
//            mode = S_IALLUGO | S_IFREG;     //建立的路径，属性是目录。
            //继承操作方法，对象的私有成员保存访问模式（即 stat 的 st_mode ）
            ob = OBJ_BuildTempPath(ob, xip_iboot_ops, (ptu32_t)0,uncached);
            OBJ_LinkHandle(hdl, ob);
        }
//    }
    xip_iboot_unlock(core);
    return (hdl);
}

// ============================================================================
// 功能：关闭文件
// 参数：hdl -- xip-iboot文件对象句柄；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 xip_iboot_close(struct objhandle *hdl)
{
    FileNowPos = 0;
//  Handle_Delete(hdl);
    return (0);
}

// ============================================================================
// 功能：写文件
// 参数：hdl -- xip-iboot文件对象句柄；
//      data -- 需写入数据的空间；
//      size -- 需写入数据的大小；
// 返回：实际写入的字节数；
// 备注：
// ============================================================================
static s32 xip_iboot_write(struct objhandle *hdl, u8 *data, u32 size)
{
    s32 res;
    struct __icore *core = (struct __icore*)File_Core(Handle_GetHostObj(hdl));
    xip_iboot_lock(core);
    res = core->drv->xip_write_media(core,data,size,FileNowPos);
    if(-1 == res)
    {
        xip_iboot_unlock(core);
        return (-1);
    }
    if(-2 == res)
    {
        xip_iboot_unlock(core);
        printf("\r\n: dbug : xip_iboot : storage space full.");
        return (-1);
    }
    FileNowPos += size;
    xip_iboot_unlock(core);
    return (size);
}

// ============================================================================
// 功能：查询xip-iboot状态；
// 参数：ob -- stdio对象；
//      data -- stdio状态；
// 返回：
// 备注：
// ============================================================================
static s32 xip_iboot_stat(struct Object *ob, struct stat *data)
{
    memset(data, 0x0, sizeof(struct stat));

    data->st_mode = S_IFREG|S_IRUGO|S_IWUGO;

    return (0);
}
#pragma GCC diagnostic pop

// ============================================================================
// 功能：初始化xip对media的驱动
// 参数：core -- efs文件系统管理；  drv -- media的操作函数集
// 返回：0 -- 成功；  -1 --失败
// 备注：
// ============================================================================
int xip_iboot_install_drv(struct __icore *core, struct __xip_drv *drv)
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
static s32 xip_iboot_fs_install(struct FsCore *super, u32 opt, void *config)
{

    struct __icore *core;
    struct umedia *um;
    u32 flash_page_size;
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
    um->mreq(unitbytes,(ptu32_t)&flash_page_size);
    xip_iboot_install_drv(core,super->MediaDrv);
    core->ASize = super->AreaSize;
    core->MStart = super->MediaStart;
    core->vol = (void*)um;
    core->bufsz = (s16)flash_page_size; // xip文件系统文件的缓存大小依据unit的尺寸；
    core->inhead = 0;  //inhead这部分为部分内容的大小
    core->root = super->pTarget;
    core->lock = Lock_MutexCreate("xip-iboot、 fs");
    if(!core->lock)
    {
        free(core);
        return (-1);
    }

    super->pCore = (void*)core;
    return (0);
}

// ============================================================================
// 功能：xip文件操作接口
// 参数：标准逻辑，查看接口说明；
// 返回：标准逻辑，查看接口说明；
// 备注:
// ============================================================================
s32 xip_iboot_ops(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
    s32 result = CN_OBJ_CMD_EXECUTED;
    switch(objcmd)
    {
        case CN_OBJ_CMD_OPEN:
        {
            struct objhandle *hdl;
            hdl = xip_iboot_open((struct Object *)opsTarget, (u32)(*(u64*)OpsArgs2), (char*)OpsArgs3);
            *(struct objhandle **)OpsArgs1 = hdl;
            break;
        }

        case CN_OBJ_CMD_WRITE:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = xip_iboot_write(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_CLOSE:
        {
            if(xip_iboot_close((struct objhandle*)opsTarget) == 0)
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
            if(xip_iboot_stat((struct Object*)opsTarget, (struct stat *)OpsArgs1) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
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
// 功能：安装xip文件系统
// 参数： opt -- 文件系统配置选项；如MS_INSTALLCREAT
//      data -- 传递给xip-iboot安装逻辑的数据；
// 返回：失败(-1)； 成功(0)。
// 备注:
// ============================================================================
s32 ModuleInstall_XIP_FS(u32 opt, void *data,char * xip_target)
{
    struct Object * mountobj;
    static struct filesystem *typeXIPIBOOT = NULL;
    s32 res;
    if(typeXIPIBOOT == NULL)
    {
        typeXIPIBOOT = malloc(sizeof(*typeXIPIBOOT));

        typeXIPIBOOT->fileOps = xip_iboot_ops;
        typeXIPIBOOT->install = xip_iboot_fs_install;
        typeXIPIBOOT->pType = "XIP-IBOOT";
        typeXIPIBOOT->format = xip_fs_format;
        typeXIPIBOOT->uninstall = NULL;
    }
    res = File_RegisterFs(typeXIPIBOOT);
    if(-1==res)
    {
        printf("\r\n: dbug : module : cannot register \"xip-iboot\"(file system type).");
        return (-1); // 失败;
    }

    mountobj = OBJ_NewChild(OBJ_GetRoot(), __File_MountOps, 0, xip_target);
    if(NULL == mountobj)
    {
        printf("\r\n: dbug : module : mount \"xip\" failed, cannot create \"%s\"(target).", xip_target);
        return (-1);
    }
//    OBJ_DutyUp(mountobj);
    opt |= MS_DIRECTMOUNT;      //直接挂载不用备份
    res = File_Mount(NULL, xip_target, "XIP-IBOOT", opt, data);
    if(res == -1)
    {
        printf("\r\n: dbug : module : mount \"XIP-IBOOT\" failed, cannot install.");
        OBJ_Delete(mountobj);
        return (-1);
    }
    return (0);
}

// ============================================================================
// 功能：安装xip文件系统
// 参数： opt -- 文件系统配置选项；如MS_INSTALLCREAT
//      data -- 传递给xip-iboot安装逻辑的数据；
// 返回：失败(-1)； 成功(0)。
// 备注:
// ============================================================================
s32 ModuleInstall_XIP_IBOOT_FS(u32 opt, void *data)
{
    ModuleInstall_XIP_FS(opt,data,CN_XIP_IBOOT_MOUNT);
    return 0;
}

