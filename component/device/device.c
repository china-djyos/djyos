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

//所属模块:设备管理模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述:提供设备管理功能
//其他说明:
//修订历史:
//3. 日起:2014-06-14
//   作者:  罗侍田.
//   新版本号：2.0.0
//   修改说明: 对设备驱动模块做了全面修改，不再提供左右手两套接口。
//2. 日期:2009-03-03
//   作者:  罗侍田.
//   新版本号：1.0.1
//   修改说明: 修正了dev_close_left函数和dev_close_right函数的错误，该bug由
//             网友sniper提交
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <object.h>
#include <objhandle.h>
#include <pool.h>
#include <systime.h>
#include <multiplex.h>
#include <../../djyos/core/component_config_core.h>
#include <lock.h>
#include "dbug.h"
#include "../include/device.h"
#include "component_config_devfile.h"

static struct MemCellPool *__dev_struct_pool; // 设备控制块内存池头指针
static struct MutexLCB __dev_sys_lock; // 保护设备安全
static struct obj *__dev_sys_root; // TODO 返回object

struct __device
{
    fntDevOpen dOpen; //
    fntDevClose dClose; //
    fntDevWrite dWrite; //
    fntDevRead dRead; //
    fntDevCntl dCntl; //
    ptu32_t DrvTag;        // 本设备驱动程序设置的标签
    ptu32_t UserTag;       // 应用程序设置的标签
};

// ============================================================================
// 功能：是否为写完成返回操作
// 参数：flags -- 文件使用标志；
// 返回：（1）写完成返回；（0）非写完成返回；
// 备注：用于判断设备的操作模式
// ============================================================================
inline s32 isbc(u32 flags)
{
    if(flags & O_DBC)
        return (1);

    return (0);
}

// ============================================================================
// 功能：是否裸操作，不考虑文件属性
// 参数：had -- 被判断设备对象的句柄；
// 返回：-1 参数错误 1具有RAW属性 0不具有RAW属性
// 备注：用于判断设备的操作模式
// ============================================================================
inline s32 israw(struct objhandle *hdl)
{
    if(!hdl)
        return (-1);

    if(handle_mode(hdl) & O_DRAW)
        return (1);

    return (0);
}

// ============================================================================
// 功能：打开设备；
// 参数：ob -- 设备对象；
//       flags -- 打开模式；
//       timeout -- 超时时间；
// 返回：成功（设备对象句柄）；失败（NULL）；
// 备注：
// ============================================================================
static struct objhandle *__devopen(struct obj *ob, u32 flags, u32 timeout)
{
    struct objhandle *devfile;
    struct __device *dev;
    s32 res;

    devfile = handle_new();
    if(!devfile)
        return (NULL);

    handle_init(devfile, ob, flags, 0);
    dev = (struct __device*)obj_GetPrivate(ob);
    if(dev)
    {
        if(dev->dOpen)
        {
            res = dev->dOpen(devfile, flags, timeout);
            if(res)
            {
                handle_free(devfile);
                debug_printf("device","cannot open device \"%s\".", obj_name(ob));
                return (NULL);
            }
        }
    }
    else
    {
        if(obj_isset(ob))
        {
            // 操作的是"/dev"
            if(!test_directory(flags))
            {
                handle_free(devfile);
                return (NULL); // 只有安装点是目录，其他都是文件
            }
        }
        else
        {
            debug_printf("device","open device \"%s\" unknown(type).", obj_name(ob));
        }
    }

    return (devfile);
}

// ============================================================================
// 功能：关闭设备；
// 参数：hdl -- 设备对象句柄；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __devclose(struct objhandle *hdl)
{
    s32 res = 0;
    struct __device *dev = (struct __device*)handle_val(hdl);

    if(dev && dev->dClose)
        res = (s32)dev->dClose(hdl);

    if(!res)
        handle_free(hdl);

    return (res);
}

// ============================================================================
// 功能：写设备；
// 参数：hdl -- 设备对象句柄；
//      data -- 数据；
//      len -- 数据大小；
// 返回：
// 备注：
// ============================================================================
static s32 __devwrite(struct objhandle *hdl, u8 *data, u32 len)
{
    u32 res = 0;
    struct __device *dev = (struct __device*)handle_val(hdl);

    if(dev&&dev->dWrite)
    {
        res = dev->dWrite(hdl, data, len, 0, handle_timeout(hdl));
    }

    return (res);
}

// ============================================================================
// 功能：读设备；
// 参数：hdl -- 设备对象句柄；
//      data -- 数据；
//      len -- 数据大小；
// 备注：
// ============================================================================
static s32 __devread(struct objhandle *hdl, u8 *data, u32 len)
{
    s32 res = 0;
    struct __device *dev = (struct __device*)handle_val(hdl);

    if(dev && dev->dRead)
    {
        res = dev->dRead(hdl, data, len, 0, handle_timeout(hdl));
    }

    return (res);
}

// ============================================================================
// 功能：读设备子类中的设备；
// 参数：hdl -- 设备类对象句柄；
//      dentry -- 目录项；
// 返回：全部读完（1）；获取取一个目录项（0）
// 备注：
// ============================================================================
static s32 __devdentry(struct objhandle *hdl, struct dirent *dentry)
{
    if(!dentry->d_ino)
        dentry->d_ino = (long)handle2obj(hdl);

    dentry->d_ino = (long)obj_foreach_child(handle2obj(hdl), (struct obj*)(dentry->d_ino));
    if(dentry->d_ino)
    {
        strcpy(dentry->d_name, obj_name((struct obj*)(dentry->d_ino)));
        dentry->d_reclen = strlen(dentry->d_name);
        if(obj_isset((struct obj*)dentry->d_ino))
            dentry->d_type |= S_IFDIR;
        else
            dentry->d_type |= S_IFREG;

        return (0);
    }

    return (1);
}

// ============================================================================
// 功能：设备控制；
// 参数：hdl -- 设备对象句柄；
//      cmd -- 控制命令；
//      arg0， arg0 -- 命令参数；
// 返回：由具体命令决定
// 备注：
// ============================================================================
static s32 __devcntl(struct objhandle *hdl, u32 cmd, ptu32_t arg0, ptu32_t arg1)
{
    struct __device *dev;
    s32 ret = 0;

    switch(cmd)
    {
        case F_SETTIMEOUT:
        {
            handle_settimeout(hdl, arg0); // arg0 = timeout;
            break;
        }

        case F_GETTIMEOUT:
        {
            u32 *timeout = (u32*)arg0;
            *timeout = handle_timeout(hdl);
            break;
        }

        case F_GETDRVTAG:
        {
            ptu32_t *drv = (ptu32_t*)arg0;
            *drv = dev_GetDrvTag(Handle2fd(hdl));
            break;
        }

        case F_GETUSERTAG:
        {
            ptu32_t *usr = (ptu32_t*)arg0;
            *usr = dev_GetUserTag(Handle2fd(hdl));
            break;
        }

        case F_SETDRVTAG:
        {
            dev_SetDrvTag(Handle2fd(hdl),arg0);
            break;
        }

        case F_SETUSERTAG:
        {
            dev_SetUserTag(Handle2fd(hdl),arg0);
            break;
        }

        default:
            dev = (struct __device*)handle_val(hdl);
            if(dev && dev->dCntl)
                ret = dev->dCntl(hdl, cmd, arg0, arg1);
    }

    return (ret);
}

// ============================================================================
// 功能：设备查询；
// 参数：ob -- 设备对象；
//      data -- 设备信息；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __devstat(struct obj *ob, struct stat *data)
{
    struct __device *dev;

    memset(data, 0x0, sizeof(*data));
    dev = (struct __device*)obj_GetPrivate(ob);
    if(dev)
    {
        // TODO：当完成FLASH等块设备抽象后，利用basic逻辑完善块设备逻辑；
        data->st_mode = S_IFCHR; // 设备文件都设计为字符（裸）类型；
        data->st_dev = (dev_t)dev;
    }
    else
    {
        if(obj_isset(ob))
            data->st_mode = S_IFDIR; // "/dev"节点为目录
        else
            printf("\r\n: dbug : device : stat device \"%s\" unknown(type).", obj_name(ob));
    }

    return (0);
}

// ============================================================================
// 功能：设备操作集
// 参数：
// 返回：
// 备注：
// ============================================================================
static ptu32_t Dev_DevObjOps(enum objops ops, ptu32_t o_hdl, ptu32_t args, ...)
{
    va_list list;

    switch(ops)
    {
        case OBJOPEN:
        {
            struct obj *ob = (struct obj*)o_hdl;
            u32 flags = (u32)args;
            u32 notFind;

            va_start(list, args);
            notFind = va_arg(list, u32);
            va_end(list);

            if(notFind) // 设备都是都已经存在了的，不会找不到
                return (0);

            if(test_onlycreat(flags)) // dev设备不能新建、或者是目录
                return (0);

            return ((ptu32_t)__devopen(ob, flags, 0));
        }

        case OBJCLOSE:
        {
            struct objhandle *devfile = (struct objhandle*)o_hdl;
            return ((ptu32_t)__devclose(devfile));
        }

        case OBJCHILDS: // TODO : 这个是不是可以考虑不放在这里
        {
            struct objhandle *devfile = (struct objhandle*)o_hdl;
            struct dirent *ret = (struct dirent*)args;

            return(__devdentry(devfile, ret));
        }

        case OBJREAD:
        {
            u32 len;
            struct objhandle *devfile = (struct objhandle*)o_hdl;
            u8 *buf = (u8*)args;

            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__devread(devfile, buf, len));
        }

        case OBJWRITE:
        {
            u32 len;
            struct objhandle *devfile = (struct objhandle*)o_hdl;
            u8 *buf = (u8*)args;

            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__devwrite(devfile, buf, len));
        }

        case OBJSTAT:
        {
            char *path;
            struct obj *ob = (struct obj*)o_hdl;
            struct stat *ret = (struct stat*)args;

            va_start(list, args);
            path = (char *)va_arg(list, u32);
            va_end(list);

            if(path&&('\0'!=*path))
                return (-1); // 设备的对象不存在（都是已缓存的）；

            return((ptu32_t)__devstat(ob, ret));
        }

        case OBJIOCTL:
        {
            ptu32_t arg0, arg1;
            struct objhandle *devfile = (struct objhandle*)o_hdl;
            u32 cmd = (u32)args;

            va_start(list, args);
            arg0 = (ptu32_t)va_arg(list, u32);
            list = *((va_list*)va_arg(list, u32)); // 这里传进来的本事就是参数的地址
            //list = (va_list)va_arg(list, va_list);
            //list = *((va_list*)va_arg(list, u32)); // 这里传进来的本事就是参数的地址
            arg1 = (ptu32_t)va_arg(list, u32);
            va_end(list);

            return ((ptu32_t)__devcntl(devfile, cmd, arg0, arg1));
        }

        case OBJCTL:
        {
            ptu32_t arg0, arg1;
            struct objhandle *devfile = (struct objhandle*)o_hdl;
            u32 cmd = (u32)args;

            va_start(list, args);
            list = (va_list)va_arg(list, va_list);
            //list = *((va_list*)va_arg(list, u32)); // 这里传进来的本事就是参数的地址
            arg0 = (ptu32_t)va_arg(list, u32);
            arg1 = (ptu32_t)va_arg(list, u32);
            va_end(list);

            return ((ptu32_t)__devcntl(devfile, cmd, arg0, arg1));
        }

        default:
        {
            break;
        }
    }

    return (-1);
}

// ============================================================================
// 功能：对象是否是设备（类）；
// 参数：
// 返回：是（1）；否（0）；
// 备注：
// ============================================================================
static s32 __isdev(struct obj *dev)
{
    dev = dev;
    return (1);
}

// ============================================================================
// 功能：设置设备的驱动标签；
// 参数：fd -- 设备文件描述符；
//       DrvTag --设备的驱动标签；
// 返回：
// 备注：
// ============================================================================
void dev_SetDrvTag(s32 fd,ptu32_t DrvTag)
{
    struct __device *dev;
    struct objhandle *hdl;

    if(fd >= 0)
    {
        hdl = fd2Handle(fd);
        dev = (struct __device *)handle_val(hdl);
        dev->DrvTag = DrvTag;
    }

    return ;
}

// ============================================================================
// 功能：设置设备的用户标签
// 参数：：fd -- 设备文件描述符；
//         DrvTag --设备的用户标签；
// 返回：用户标签
// 备注：
// ============================================================================
ptu32_t dev_SetUserTag(s32 fd,ptu32_t UserTag)
{
    struct __device *dev;
    struct objhandle *hdl;

    if(fd >= 0)
    {
        hdl = fd2Handle(fd);
        dev = (struct __device *)handle_val(hdl);
        dev->UserTag = UserTag;
    }

    return (0);
}

// ============================================================================
// 功能：获取设备的驱动标签；
// 参数：fd -- 设备文件描述符；
// 返回：驱动标签；
// 备注：
// ============================================================================
ptu32_t dev_GetDrvTag(s32 fd)
{
    struct __device *dev;
    struct objhandle *hdl;

    if(fd >= 0)
    {
        hdl = fd2Handle(fd);
        dev = (struct __device *)handle_val(hdl);
        return (dev->DrvTag);
    }

    return (0);
}

// ============================================================================
// 功能：获取设备的用户标签
// 参数：fd -- 设备文件描述符；
// 返回：用户标签
// 备注：
// ============================================================================
ptu32_t dev_GetUserTag(s32 fd)
{
    struct __device *dev;
    struct objhandle *hdl;

    if(fd >= 0)
    {
        hdl = fd2Handle(fd);
        dev = (struct __device *)handle_val(hdl);
        return (dev->UserTag);
    }

    return (0);
}

// ============================================================================
// 功能：添加设备子类
// 参数：name -- 设备子类名；
// 返回：成功（设备子类）；失败（NULL）；
// 备注：
// ============================================================================
inline struct obj *dev_group_addo(char *name)
{
#if 0
    struct obj *ob;
    struct __portBasic *group;

    group = malloc(sizeof(*group));
    if(!group)
        return(NULL);

    handle_initbasic(group, O_RDWR);
    ob = of_virtualizeo(__dev_sys_root, group, name);
    if(!ob)
    {
        return (NULL);
    }

    return (ob);
#else
    struct obj *ob;

    ob = obj_newchild(__dev_sys_root, Dev_DevObjOps, O_RDWR, 0, name);
    if(!ob)
        return (NULL);

    if(obj_allow2set(ob)) // 允许对象上建立集合(类)；
    {
        obj_del(ob);
        return (NULL);
    }

    if(obj_change2set(ob)) // 将对象转为集合（类）；
    {
        obj_del(ob);
        return (NULL);
    }

    return (ob);
#endif
}

// ============================================================================
// 功能：添加设备子类
// 参数：name -- 设备子类名；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 dev_group_add(char *name)
{
    if(dev_group_addo(name))
        return (0);

    return (-1);
}

// ============================================================================
// 功能：删除设备子类
// 参数：grp -- 设备子类节点；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
inline s32 dev_group_delo(struct obj *grp)
{
    struct obj *dev0, *devx;
    u8 end = 0;
    s32 res = 0;

    if((!grp)
       ||(!__isdev(grp)))
        return (-1);

    dev0 = obj_child(grp);
    do
    {
        devx = obj_next(dev0);
        if(devx == dev0)
            end = 1; // 最后一个成员；

        if(obj_isonduty(devx))
            res = -1; // 设备正在使用中，无法删除
        else
            obj_del(devx);
    }
    while(!end);

    if(!res)
    {
        if(obj_isonduty(grp))
        {
            obj_del(grp);
        }
        else
        {
            res = -1; // 类目录正在使用中，无法删除
        }
    }

    return (res);
}

// ============================================================================
// 功能：删除设备子类
// 参数：name -- 设备子类名；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 dev_group_del(char *name)
{
    char *left, *tmp;
    u8 index = 0;
    struct obj *ob;

    if(name)
    {
        // 判断group格式，拼接完整的/dev路径
        if('/'==name[0])
        {
            index = 1;
        }

        if(('d'==name[index])&&('e'==name[index+1])
           &&('v'==name[index+2])&&('/'==name[index+3]))
        {
            tmp = name; // group已经含有了dev/
        }
        else
        {
            tmp = malloc(strlen(name)+4);
            if(!tmp)
            {
                printf("\r\n: warn : device : memory out(%s).", tmp);
                return (-1);
            }

            sprintf(tmp, "%s/%s", "dev/", name);
        }

        ob = obj_matchpath(tmp, &left, NULL);
        if((!left) || (ob))
        {
            free(tmp);
            return (-1);
        }

        free(tmp);
    }

    return (dev_group_delo(ob));
}

// ============================================================================
// 功能：添加一个设备到其设备类；
// 参数：grp -- 设备子类；为NULL，则表示其是dev的子类；
//      name -- 设备名；
// 返回：成功（设备对象）；失败（NULL）；
// 备注：
// ============================================================================
inline struct obj *dev_addo(struct obj *grpo, const char *name,
                               fntDevOpen dopen, fntDevClose dclose,
                               fntDevWrite dwrite, fntDevRead dread,
                               fntDevCntl dcntl, ptu32_t DrvTag)
{
#if 0
    struct device *device;
    struct obj *devo;

    if(!grpo)
        grpo = __dev_sys_root;

    device = malloc(sizeof(*device));
    if(!device)
    {
        return (NULL);
    }

    device->dev = (struct __device*)Mb_Malloc(__dev_struct_pool, 0);
    if(!device->dev)
    {
        free(device);
        return (NULL);
    }

    handle_initbasic(&device->basic, O_RDWR);
    devo = of_virtualizeo(grpo, &device->basic, (char*)name);
    if(!devo)
    {
        free(device->dev);
        free(device);
        return (NULL);
    }

    obj_setval(devo, (ptu32_t)device->dev);
    device->dev->dClose = dclose;
    device->dev->dOpen = dopen;
    device->dev->dCntl = dcntl;
    device->dev->dRead = dread;
    device->dev->dWrite = dwrite;
    device->dev->drv = dtag;

    return (devo);
#else
    struct obj *devo;
    struct __device *dev;

    if(!grpo)
    {
        grpo = __dev_sys_root;
    }
    else
    {
        if((grpo!=__dev_sys_root)
           &&(obj_parent(grpo)!=__dev_sys_root)) // 逻辑上设备类对象的父必须是dev（控制对象树深度）
            return (NULL);
    }

    dev = (struct __device*)Mb_Malloc(__dev_struct_pool, 0);
    if(!dev)
        return (NULL);

    devo = obj_newchild(grpo, NULL, O_RDWR, (ptu32_t)dev, name);
    if(!devo)
    {
        Mb_Free(__dev_struct_pool, dev);
        return (NULL);
    }

    dev->dClose = dclose;
    dev->dOpen = dopen;
    dev->dCntl = dcntl;
    dev->dRead = dread;
    dev->dWrite = dwrite;
    dev->DrvTag = DrvTag;
    return (devo);
#endif
}

// ============================================================================
// 功能：添加一个设备到其设备类；
// 参数：grp -- 设备类；
//       name -- 设备名；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 dev_add(const char *grp, const char *name, fntDevOpen dopen, fntDevClose dclose,
            fntDevWrite dwrite, fntDevRead dread, fntDevCntl dcntl, ptu32_t dtag)
{
    struct obj *ob = __dev_sys_root;
    char *left, *tmp;
    u8 index = 0;

    if(grp)
    {
        // 判断group格式，拼接完整的/dev路径
        if('/' == grp[0])
        {
            index = 1;
        }

        if(('d' == grp[index]) && ('e' == grp[index+1]) &&
           ('v' == grp[index+2]) && ('/' == grp[index+3]))
        {
            tmp = (char*)grp; // group已经含有了dev/
        }
        else
        {
            tmp = malloc(strlen(grp)+6);
            if(!tmp)
            {
                warning_printf("device","memory out(%s).", tmp);
                return (-1);
            }

            sprintf(tmp, "%s/%s", "/dev", grp);
        }

        ob = obj_matchpath(tmp, &left, NULL);
        if(left||(!ob))
        {
            free(tmp);
            return (-1);
        }

        free(tmp);
    }

    if(dev_addo(ob, name, dopen, dclose, dwrite, dread, dcntl, dtag))
        return (0);

    return (-1);
}

// ============================================================================
// 功能：删除设备对象；
// 参数：devo -- 设备节点；
// 返回：成功（0）；失败（-1）；
// 备注：与文件系统接口要形成互斥；（要是在删除的过程中出现了使用）
// ============================================================================
inline s32 dev_delo(struct obj *devo)
{
    // struct device *device;

    if((!devo)
       ||(obj_isonduty(devo)) // 设备被使用中
       ||(!__isdev(devo))) // 不是设备对象
        return (-1);

    // TODO：增加删除设备控制；
    Mb_Free(__dev_struct_pool, (void*)obj_GetPrivate(devo));
    obj_del(devo);
    return (0);
}

// ============================================================================
// 功能：删除设备；
// 参数：grp -- 设备子类；
//      name -- 设备名；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 dev_del(const char *grp, const char *name)
{
    char *left, *tmp;
    struct obj *ob = __dev_sys_root;
    u8 index = 0;

    if(grp)
    {
        // 判断group格式，拼接完整的/dev路径
        if('/' == grp[0])
        {
            index = 1;
        }

        if(('d' == grp[index]) && ('e' == grp[index+1]) &&
           ('v' == grp[index+2]) && ('/' == grp[index+3]))
        {
            tmp = (char*)grp; // group已经含有了dev/
        }
        else
        {
            tmp = malloc(strlen(grp)+4);
            if(!tmp)
            {
                warning_printf("device","memory out(%s).", tmp);
                return (-1);
            }

            sprintf(tmp, "%s/%s", "dev/", grp);
        }

        ob = obj_matchpath(tmp, &left, NULL);
        if((!left) || (ob))
        {
            free(tmp);
            return (-1);
        }

        free(tmp);
    }

    ob = obj_search_child(ob, name); // 查找设备节点；
    if(!ob)
        return (-1);

    return(dev_delo(ob));
}

// ============================================================================
// 功能：设备对象的设备标签；
// 参数：devo -- 设备对象；
// 返回：
// 备注：TODO: INLINE
// ============================================================================
ptu32_t devo2drv(struct obj *devo)
{
    struct __device *dev;

    if((!devo)||(!__isdev(devo)))
        return (0);

    dev = (struct __device*)obj_GetPrivate(devo);
    if(dev)
        return (dev->DrvTag);

    return (0);
}

// ============================================================================
// 功能：获取设备对象的名字
// 参数：devo -- 设备对象指针
// 返回：设备对象的名字
// 备注：
// ============================================================================
inline const char *devo2name(struct obj *devo)
{
    if((devo)&&(__isdev(devo)))
        return (obj_name(devo));

    return (NULL);
}

// ============================================================================
// 功能：设备文件系统安装
// 参数：
// 返回：
// 备注：TODO
// ============================================================================
s32 mount_device_system(void)
{
    u8 *mem;

    if(__dev_sys_root || __dev_struct_pool)
    {
        debug_printf("device","device file system has been installed.");
        return (-1);
    }

    mem = M_Malloc(CFG_DEVFILE_LIMIT * sizeof(struct __device), 0x1);
    if(!mem)
    {
        debug_printf("device","cannot install device file system(memory out).");
        return (-1);
    }

    __dev_struct_pool = Mb_CreatePool((void*)mem,
                                   CFG_DEVFILE_LIMIT,
                                   sizeof(struct __device),
                                   0, 0,
                                   "设备控制块池");
    if(!__dev_struct_pool)
    {
        debug_printf("device","cannot install device file system(pool).");
        return (-1);
    }

    Lock_MutexCreate_s(&__dev_sys_lock, "dev");
    __dev_sys_root = obj_newchild_set(objsys_root(), "dev", Dev_DevObjOps, 0, O_RDWR);
    if(!__dev_sys_root)
    {
        debug_printf("device","cannot install device file system(mount).");
        return (-1);
    }

    return (0);
}

// ============================================================================
// 功能：根据设备名打开设备，搜索整个设备资源树，找到名称与name匹配的资源结点，
//      返回该句柄指针。多线程互斥方式如下:
//      A线程以D_RDWR模式打开,------其他线程不能再打开
//      A线程以D_RDONLY模式打开,----任意线程(包含A)还可以用D_WRONLY模式打开
//      D_WRONLY同上
// 参数：name,设备名字符串,包含路径名，但不必包含'dev\'这4个字符
//      flags,设备打开模式,D_RDONLY,D_WRONLY,D_RDWR中的一个
//      timeout，超时设置,单位是微秒，CN_TIMEOUT_FOREVER=无限等待，0则立即按
//          超时返回。非0值将被向上调整为CN_CFG_TICK_US的整数倍
// 返回：成功打开设备返回描述符,否则返回-1.
// 备注：O_RDONLY、O_WRONLY、O_RDWR 须符合POSIX（2016）的最新定义
// ============================================================================
s32 DevOpen(const char *name, s32 flags, u32 timeout)
{
    struct obj *dev;
    struct objhandle *devfile = NULL;

    if(!Lock_MutexPend(&__dev_sys_lock, timeout))
        return (-1);

    dev = obj_search_path(__dev_sys_root, name); // 在设备目录中搜索设备
    if(dev)
    {
        devfile = __devopen(dev, (flags|O_DRAW), timeout);
    }

    Lock_MutexPost(&__dev_sys_lock);
    if(!devfile)
        return (-1);

    return Handle2fd(devfile);
}

// ============================================================================
// 功能：关闭设备，由于设备受互斥量保护，故只有设备拥有者(即打开设备的事件)才能关
//      闭设备，这是由互斥量的特点决定的。
// 参数：fd -- 设备文件描述符
// 返回：成功（TRUE）；失败（FALSE）；
// 备注：
// ============================================================================
s32 DevClose(s32 fd)
{
#if 0
    struct __device * dev;
    struct objhandle* devFile;
    struct obj *ob;
    s32 Mode;

    devFile = fd2Handle( dwFD );
    if(devFile == NULL)
        return false;
    ob = handle2obj(devFile);
    dev = (struct __device *)OBJ_GetPrivate(ob);
    Mode = (s32)KF_GetFileContext(devFile);

    if(Mode & O_RDONLY)
    {
        Lock_MutexPost(dev->dReadMutex );
    }

    if(Mode & O_WRONLY)
    {
        Lock_MutexPost(dev->dWriteMutex );
    }
    OBJ_DecUseOne(ob);
    return true;
#else

    s32 res;
    struct objhandle *devfile;

    devfile = fd2Handle(fd);
    if(!devfile)
        return (-1);

    res = __devclose(devfile);
    if(res)
        return (-1);

    // TODO: 未减一
    return (0);
#endif
}

// ============================================================================
// 功能：从设备读取数据.
// 参数：fd -- 设备描述符
//       buf -- 用于接收数据的缓冲区，其容量必须不小于len。buf的容量无法检查。
//       offset -- 读取位置在设备中的偏移，对于流设备(例如串口)来说，通常是0.
//       timeout -- 超时设置,单位是微秒，CN_TIMEOUT_FOREVER=无限等待，0则立即按
//          超时返回。非0值将被向上调整为CN_CFG_TICK_US的整数倍
//      以上参数，driver模型只是如实传送给具体设备，设备开发者也可以不遵守上述
//      规定。
// 返回：实际读到的数据量；
// 备注：
// ============================================================================
s32 DevRead(s32 fd, void *buf, u32 len, u32 offset, u32 timeout)
{
    struct __device *dev;
    struct objhandle *devfile;

    if((!len) || (!buf))
        return (0);

    devfile = fd2Handle(fd);
    if(!devfile)
        return (0);

    dev = (struct __device*)handle_val(devfile);
    if(dev && dev->dRead)
        return (dev->dRead(devfile, buf, len, offset, timeout));

    return (0);

}

// ============================================================================
// 功能：把数据写入设备.
// 参数：fd -- 设备描述符
//       buf -- 待发送数据的缓冲区。
//       len -- 发送的数据量
//       offset -- 写入位置在设备中的偏移，对于流设备(例如串口)来说，通常是0.
//       timeout -- 超时设置,单位是微秒，CN_TIMEOUT_FOREVER=无限等待，0则立即按
//          超时返回。非0值将被向上调整为CN_CFG_TICK_US的整数倍
// 返回：实际写出的数据量；
// 备注：
// ============================================================================
s32 DevWrite(s32 fd, void *buf, u32 len, u32 offset, u32 timeout)
{
    struct __device * dev;
    struct objhandle *devfile;

    if((!len) || (!buf))
        return (0);

    devfile = fd2Handle(fd);
    if(!devfile)
        return (0);

    dev = (struct __device*)handle_val(devfile);
    if(dev&&dev->dWrite)
        return (dev->dWrite(devfile, buf, len, offset, timeout));

    return (0);
}

// ============================================================================
// 功能：控制设备.
// 参数：fd -- 设备指针
//       dwCmd -- 命令号，driver.h中预定义了一些命令，以及用户命令的使用规则，可参阅
//          常量CN_DEV_CTRL_START定义附近代码。
//       data1，data2 -- 控制命令所需要的参数，由设备开发者定义
// 返回：-1表示参数检查出错，其他值由设备开发者定义
// 备注：
// ============================================================================
s32 DevCntl(s32 fd, u32 dwCmd, ptu32_t data1, ptu32_t data2)
{
    struct __device *dev;
    struct objhandle *devfile;

    devfile = fd2Handle(fd);
    if(!devfile)
        return (-1);

    dev = (struct __device*)handle_val(devfile);
    if(dev && dev->dCntl)
        return (dev->dCntl(devfile, dwCmd, data1, data2));

    return (-1);
}
