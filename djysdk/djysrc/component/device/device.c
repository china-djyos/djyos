//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
#include <objfile.h>
#include <pool.h>
#include <systime.h>
#include <multiplex.h>
#include <../../djyos/core/component_config_core.h>
#include <lock.h>
#include "dbug.h"
#include "../include/device.h"
#include "component_config_devfile.h"

static struct MemCellPool *pDevPool; // 设备控制块内存池头指针
static struct MutexLCB s_tDevMutex; // 保护设备安全
struct Object *devRoot; // TODO 返回object


struct __device
{
    fntDevOpen dOpen; //
    fntDevClose dClose; //
    fntDevWrite dWrite; //
    fntDevRead dRead; //
    fntDevCntl dCntl; //
    ptu32_t dTag; // 本设备特征的数据，在创建设备时，参数传入
    ptu32_t uTag; // 用户特征数据，用户使用过程中设置
};


struct device{
    struct __portBasic basic;
    struct __device *dev;   //lst : 与 pObj->represent 重复了吗？
};

// ============================================================================
// 功能：是否Block Complete操作
// 参数：flags -- 文件使用标志；
// 返回：目录（1）；非目录（0）；
// 备注：用于设备的操作模式
// ============================================================================
inline s32 isbc(u32 flags)
{
    if(flags & O_DBC)
        return (1);

    return (0);
}

// ============================================================================
// 功能：是否RAW操作
// 参数：flags -- 文件使用标志；
// 返回：目录（1）；非目录（0）；非法（-1）；
// 备注：用于设备的操作模式
// ============================================================================
inline s32 israw(tagOFile *devof)
{
    if(!devof)
        return (-1);

    if(of_mode(devof) & O_DRAW)
        return (1);

    return (0);
}

// ============================================================================
// 功能：打开设备文件；
// 参数：
// 返回：
// 备注：
// ============================================================================
tagOFile *__devfileopen(struct Object *obj, u32 flags, u32 timeout)
{
    tagOFile *devfile;
    struct __device *dev;
    s32 res;

    devfile = of_new();
    if(!devfile)
        return (NULL);

    of_init(devfile, obj, flags, 0);
    dev = (struct __device*)__OBJ_Represent(obj);
    if(dev)
    {
        if(dev->dOpen)
        {
            res = dev->dOpen(devfile, flags, timeout);
            if(res)
            {
                of_free(devfile);
                debug_printf("device","cannot open device \"%s\".", __OBJ_Name(obj));
                return (NULL);
            }
        }
    }
    else
    {
        if(GROUP_POINT == __OBJ_Type(obj))
        {
            // 操作的是"/dev"
            if(!testdirectory(flags))
            {
                of_free(devfile);
                return (NULL); // 只有安装点是目录，其他都是文件
            }
        }
        else
        {
            debug_printf("device","open device \"%s\" unknown(type).", __OBJ_Name(obj));
        }
    }

    return (devfile);
}

// ============================================================================
// 功能：关闭设备文件；
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __devfileclose(tagOFile *devof)
{
    s32 res = 0;
    struct __device *dev = (struct __device*)of_represent(devof);

    if(dev && dev->dClose)
        res = (s32)dev->dClose(devof);

    if(!res)
        of_free(devof);

    return (res);
}

// ============================================================================
// 功能：写设备文件；
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __devfilewrite(tagOFile *devof, u8 *pBuf, u32 dwLen)
{
    u32 res = 0;
    struct __device *dev = (struct __device*)of_represent(devof);

    if(dev && dev->dWrite)
    {
        res = dev->dWrite(devof, pBuf, dwLen, 0, of_timeout(devof));
    }

    return (res);
}

// ============================================================================
// 功能：读设备文件；
// 参数：
// 返回：
// 备注：TODO
// ============================================================================
static s32 __devfileread(tagOFile *devof, u8 *pBuf, u32 dwLen)
{
    s32 res = 0;
    struct __device *dev = (struct __device*)of_represent(devof);

    if(dev && dev->dRead)
    {
        res = dev->dRead(devof, pBuf, dwLen, 0, of_timeout(devof));
    }

    return (res);
}

// ============================================================================
// 功能：读设备目录项；
// 参数：
// 返回：全部读完（1）；获取取一个目录项（0）
// 备注：
// ============================================================================
static s32 __devfiledentry(tagOFile *devof, struct dirent *pDirent)
{
    if(!pDirent->d_ino)
        pDirent->d_ino = (long)of_o(devof);

    pDirent->d_ino = (long)OBJ_TraveChild(of_o(devof), (struct Object*)(pDirent->d_ino));
    if(pDirent->d_ino)
    {
        strcpy(pDirent->d_name, __OBJ_Name((struct Object*)(pDirent->d_ino)));
        pDirent->d_reclen = strlen(pDirent->d_name);
        pDirent->d_type |= S_IFREG;
        return (0);
    }

    return (1);
}

// ============================================================================
// 功能：设备文件控制；
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __devfilecntl(tagOFile *devof, u32 cmd, ptu32_t arg0, ptu32_t arg1)
{
    struct __device *dev;
    s32 care = 0, res = 0, ret = -1;

    switch(cmd)
    {
        case F_OF_SETTIMEOUT:
        {
            of_settimeout(devof, arg0); // arg0 = timeout;
            break;
        }

        case F_OF_GETTIMEOUT:
        {
            u32 *timeout = (u32*)arg0;
            *timeout = of_timeout(devof);
            break;
        }

        case F_GETDDTAG:
        {
            ptu32_t *dTag = (ptu32_t*)arg0;
            *dTag = devfiledtag(devof);
            break;
        }

        case F_GETDUTAG:
        {
            ptu32_t *uTag = (ptu32_t*)arg0;
            *uTag = devfileutag(devof);
            break;
        }

        default:break;
    }

    dev = (struct __device*)of_represent(devof);
    if(dev && dev->dCntl)
        ret = dev->dCntl(devof, cmd, arg0, arg1);

    if(care) // 是否在意dCntl的返回值
        res = ret;

    return (res);
}

// ============================================================================
// 功能：
// 参数：
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 __devfilestat(struct Object *obj, struct stat *pStat)
{
    struct __device *dev;

    memset(pStat, 0x0, sizeof(*pStat));
    dev = (struct __device*)__OBJ_Represent(obj);
    if(dev)
    {
        // TODO：当完成FLASH等块设备抽象后，利用basic逻辑完善块设备逻辑；
        pStat->st_mode = S_IFCHR; // 设备文件都设计为字符（裸）类型；
        pStat->st_dev = (dev_t)dev;
    }
    else
    {
        if(GROUP_POINT == __OBJ_Type(obj))
            pStat->st_mode = S_IFDIR; // "/dev"节点为目录
        else
            debug_printf("device","stat device \"%s\" unknown(type).", __OBJ_Name(obj));
    }

    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static ptu32_t __devfileoperations(u32 cmd, ptu32_t oof, ptu32_t args, ...)
{
    va_list list;

    switch(cmd)
    {
        case CN_OBJ_CMD_OPEN:
        {
            struct Object *obj = (struct Object*)oof;
            u32 flags = (u32)args;
            u32 notFind;

            va_start(list, args);
            notFind = va_arg(list, u32);
            va_end(list);

            if(notFind) // 设备都是都已经存在了的，不会找不到
                return (0);

            if(testonlycreat(flags)) // dev设备不能新建、或者是目录
                return (0);

            return ((ptu32_t)__devfileopen(obj, flags, 0));
        }

        case CN_OBJ_CMD_CLOSE:
        {
            tagOFile *devfile = (tagOFile*)oof;
            return ((ptu32_t)__devfileclose(devfile));
        }

        case CN_OBJ_CMD_READDIR: // TODO : 这个是不是可以考虑不放在这里
        {
            tagOFile *devfile = (tagOFile*)oof;
            struct dirent *ret = (struct dirent*)args;

            return(__devfiledentry(devfile, ret));
        }

        case CN_OBJ_CMD_READ:
        {
            u32 len;
            tagOFile *devfile = (tagOFile*)oof;
            u8 *buf = (u8*)args;

            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__devfileread(devfile, buf, len));
        }

        case CN_OBJ_CMD_WRITE:
        {
            u32 len;
            tagOFile *devfile = (tagOFile*)oof;
            u8 *buf = (u8*)args;

            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__devfilewrite(devfile, buf, len));
        }

        case CN_OBJ_CMD_STAT:
        {
            struct Object *obj = (struct Object*)oof;
            u32 notFind= (u32)args;
            struct stat *ret;

            va_start(list, args);
            ret = (struct stat*)va_arg(list, u32);
            va_end(list);

            if(notFind)
                return (-1);

            return((ptu32_t)__devfilestat(obj, ret));
        }

        default:
        {
            ptu32_t arg0, arg1;
            tagOFile *devfile = (tagOFile*)oof;
            va_list list = *((va_list*)args); // 这里传进来的本事就是参数的地址

            arg0 = (ptu32_t)va_arg(list, u32);
            arg1 = (ptu32_t)va_arg(list, u32);
            va_end(list);

            if((CN_OBJ_FCNTL_START <= cmd) && (cmd < CN_DEV_CTRL_START))
            {
                cmd -= CN_OBJ_FCNTL_START;
                return ((ptu32_t)__devfilecntl(devfile, cmd, arg0, arg1));
            }

            if(cmd >= CN_DEV_CTRL_START)
            {
                cmd -= CN_DEV_CTRL_START;
                return ((ptu32_t)__devfilecntl(devfile, cmd, arg0, arg1));
            }

            return ((ptu32_t)CN_OBJ_CMD_UNSUPPORT);
        }
    }

    return (-1);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
ptu32_t devfiledtag(tagOFile *of)
{
    struct __device *dev;

    if(of)
    {
        dev = (struct __device *)of_represent(of);
        return (dev->dTag);
    }

    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
ptu32_t devfileutag(tagOFile *of)
{
    struct __device *dev;

    if(of)
    {
        dev = (struct __device *)of_represent(of);
        return (dev->uTag);
    }

    return (0);
}

// ============================================================================
// 功能：添加设备子类
// 参数：name -- 设备子类名；
// 返回：
// 备注：
// ============================================================================
inline struct Object *dev_grpaddo(char *name)
{
    struct Object *obj;
    struct __portBasic *group;

    group = malloc(sizeof(*group));
    if(!group)
        return(NULL);

    of_initbasic(group, O_RDWR);
    obj = of_virtualizeo(devRoot, group, name);
    if(!obj)
    {
        return (NULL);
    }

    return (obj);
}

// ============================================================================
// 功能：添加设备子类
// 参数：name -- 设备子类名；
// 返回：
// 备注：
// ============================================================================
s32 dev_grpadd(char *name)
{
    if(dev_grpaddo(name))
        return (0);

    return (-1);

}

// ============================================================================
// 功能：删除设备子类
// 参数：grp -- 设备子类节点；
// 返回：
// 备注：
// ============================================================================
inline s32 dev_grpdelo(struct Object *grp)
{
    struct Object *dev0, *devx;
    u8 end = 0;
    s32 res = 0;

    if(!grp)
        return (-1);

    dev0 = OBJ_Child(grp);
    do
    {
        devx = OBJ_Next(dev0);
        if(devx == dev0)
            end = 1; // 最后一个成员；

        if(isondutyo(devx))
            res = -1; // 设备正在使用中，无法删除
        else
            __OBJ_Del(devx);


    }
    while(!end);

    if(!res)
    {
        if(isondutyo(grp))
        {
            __OBJ_Del(grp);
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
// 参数：grp -- 设备子类名；
// 返回：
// 备注：
// ============================================================================
s32 dev_grpdel(char *name)
{
    char *left, *tmp;
    u8 index = 0;
    struct Object *obj;

    if(name)
    {
        // 判断group格式，拼接完整的/dev路径
        if('/' == name[0])
        {
            index = 1;
        }

        if(('d' == name[index]) && ('e' == name[index+1]) &&
           ('v' == name[index+2]) && ('/' == name[index+3]))
        {
            tmp = name; // group已经含有了dev/
        }
        else
        {
            tmp = malloc(strlen(name)+4);
            if(!tmp)
            {
                warning_printf("device","memory out(%s).", tmp);
                return (-1);
            }

            sprintf(tmp, "%s/%s", "dev/", name);
        }

        obj = OBJ_MatchPath(tmp, &left, NULL);
        if((!left) || (obj))
        {
            free(tmp);
            return (-1);
        }

        free(tmp);
    }

    return (dev_grpdelo(obj));
}

// ============================================================================
// 功能：添加一个设备到其类目；
// 参数：grp -- 设备子类；为NULL，则表示其是devcie的子类；
//      name -- 设备名；
// 返回：
// 备注：
// ============================================================================
inline struct Object *dev_addo(struct Object *grpo, const char *name,
                               fntDevOpen dopen, fntDevClose dclose,
                               fntDevWrite dwrite, fntDevRead dread,
                               fntDevCntl dcntl, ptu32_t dtag)
{
    struct device *device;
    struct Object *devo;

    if(!grpo)
        grpo = devRoot;

    device = malloc(sizeof(*device));
    if(!device)
    {
        return (NULL);
    }

    device->dev = (struct __device*)Mb_Malloc(pDevPool, 0);
    if(!device->dev)
    {
        free(device);
        return (NULL);
    }

    of_initbasic(&device->basic, O_RDWR);
    devo = of_virtualizeo(grpo, &device->basic, (char*)name);
    if(!devo)
    {
        free(device->dev);
        free(device);
        return (NULL);
    }

    __OBJ_SetRepresent(devo, (ptu32_t)device->dev);
    device->dev->dClose = dclose;
    device->dev->dOpen = dopen;
    device->dev->dCntl = dcntl;
    device->dev->dRead = dread;
    device->dev->dWrite = dwrite;
    device->dev->dTag = dtag;

    return (devo);
}

// ============================================================================
// 功能：
// 参数：group -- 设备添加到的设备类；
//      name -- 设备名；
// 返回：
// 备注：
// ============================================================================
s32 dev_add(const char *grp, const char *name, fntDevOpen dopen, fntDevClose dclose,
                       fntDevWrite dwrite, fntDevRead dread,
                       fntDevCntl dcntl, ptu32_t dtag)
{
    struct Object *obj = devRoot;
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

        obj = OBJ_MatchPath(tmp, &left, NULL);
        if(left||(!obj))
        {
            free(tmp);
            return (-1);
        }

        free(tmp);
    }

    if(dev_addo(obj, name, dopen, dclose, dwrite, dread, dcntl, dtag))
        return (0);

    return (-1);
}

// ============================================================================
// 功能：
// 参数：devo -- 设备节点；
// 返回：
// 备注：与文件系统接口要形成互斥；（要是在删除的过程中出现了使用）
// ============================================================================
inline s32 dev_delo(struct Object *devo)
{
    struct device *device;

    if(!devo)
        return (-1);

    if(isondutyo(devo))
        return (-1); // 设备被使用中

    device = dListEntry(of_basiclinko(devo), struct device, basic);
    free(device->dev);
    free(device);
    __OBJ_Del(devo);

    return (0);
}

// ============================================================================
// 功能：
// 参数：grp -- 设备子类；
//      name -- 设备名；
// 返回：
// 备注：
// ============================================================================
s32 dev_del(const char *grp, const char *name)
{
    char *left, *tmp;
    struct Object *obj = devRoot;
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

        obj = OBJ_MatchPath(tmp, &left, NULL);
        if((!left) || (obj))
        {
            free(tmp);
            return (-1);
        }

        free(tmp);
    }

    obj = OBJ_SearchChild(obj, name); // 查找设备节点；
    if(!obj)
        return (-1);

    return(dev_delo(obj));
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：TODO: INLINE
// ============================================================================
ptu32_t dev_dtago(struct Object *devo)
{
    struct __device *dev;

    if(!devo)
        return (0);

    dev = (struct __device*)__OBJ_Represent(devo);
    if(dev)
        return (dev->dTag);

    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
inline const char *dev_nameo(struct Object *devo)
{
    if(devo)
        return (__OBJ_Name(devo));

    return (0);
}

// ============================================================================
// 功能：设备文件系统安装
// 参数：
// 返回：
// 备注：TODO
// ============================================================================
s32 __InstallDevFS(void)
{
    u8 *mem;

    if(devRoot || pDevPool)
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

    pDevPool = Mb_CreatePool((void*)mem,
    								CFG_DEVFILE_LIMIT,
                                   sizeof(struct __device),
                                   0, 0,
                                   "设备控制块池");
    if(!pDevPool)
    {
        debug_printf("device","cannot install device file system(pool).");
        return (-1);
    }

    Lock_MutexCreate_s(&s_tDevMutex, "dev");
    devRoot = __mounto("dev", "/", (tagObjOps)__devfileoperations, NULL);
    if(!devRoot)
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
// 返回：成功打开设备返回设备句柄,否则返回-1.
// 备注：O_RDONLY、O_WRONLY、O_RDWR 须符合POSIX（2016）的最新定义
// ============================================================================
s32 DevOpen(const char *pName, s32 flags, u32 timeout)
{
    struct Object *dev;
    tagOFile *devfile = NULL;

    if(!Lock_MutexPend(&s_tDevMutex, timeout))
        return (-1);

    dev = OBJ_SearchChild(devRoot, pName); // 在设备目录中搜索设备
    if(dev)
    {
        devfile = __devfileopen(dev, (flags|O_DRAW), timeout);
    }

    Lock_MutexPost(&s_tDevMutex);
    if(!devfile)
        return (-1);

    return ofno(devfile);
}

// ============================================================================
// 功能：关闭设备，由于设备受互斥量保护，故只有设备拥有者(即打开设备的事件)才能关
//      闭设备，这是由互斥量的特点决定的。
// 参数：dwFD -- 设备文件句柄
// 返回：成功（TRUE）；失败（FALSE）；
// 备注：
// ============================================================================
s32 DevClose(s32 dwFD)
{
#if 0
    struct __device * dev;
    tagOFile* devFile;
    struct Object *obj;
    s32 Mode;

    devFile = of( dwFD );
    if(devFile == NULL)
        return false;
    obj = of_o(devFile);
    dev = (struct __device *)OBJ_GetPrivate(obj);
    Mode = (s32)KF_GetFileContext(devFile);

    if(Mode & O_RDONLY)
    {
        Lock_MutexPost(dev->dReadMutex );
    }

    if(Mode & O_WRONLY)
    {
        Lock_MutexPost(dev->dWriteMutex );
    }
    OBJ_DecUseOne(obj);
    return true;
#else

    s32 res;
    tagOFile *devfile;

    devfile = of(dwFD);
    if(!devfile)
        return (-1);

    res = __devfileclose(devfile);
    if(res)
        return (-1);

    // TODO: 未减一
    return (0);
#endif
}

// ============================================================================
// 功能：从设备读取数据.
// 参数：dev,设备指针
//      pBuf,用于接收数据的缓冲区，其容量必须不小于len。buf的容量无法检查。
//      dwLen,buf的容量，接收数据将不大于它，须小于buf的实际容量。
//      offset,读取位置在设备中的偏移，对于流设备(例如串口)来说，通常是0.
//      timeout，超时设置,单位是微秒，CN_TIMEOUT_FOREVER=无限等待，0则立即按
//          超时返回。非0值将被向上调整为CN_CFG_TICK_US的整数倍
//      以上参数，driver模型只是如实传送给具体设备，设备开发者也可以不遵守上述
//      规定。
// 返回：实际读到的数据量；
// 备注：
// ============================================================================
s32 DevRead(s32 handle, void *buf, u32 len, u32 offset, u32 timeout)
{
    struct __device *dev;
    tagOFile *devfile;

    if((!len) || (!buf))
        return (0);

    devfile = of(handle);
    if(!devfile)
        return (0);

    dev = (struct __device*)of_represent(devfile);
    if(dev && dev->dRead)
        return (dev->dRead(devfile, buf, len, offset, timeout));

    return (0);

}

// ============================================================================
// 功能：把数据写入设备.
// 参数：dwFD -- 设备指针
//      pBuf -- 待发送数据的缓冲区。
//      dwLen -- 发送的数据量
//      dwOffset -- 写入位置在设备中的偏移，对于流设备(例如串口)来说，通常是0.
//      timeout -- 超时设置,单位是微秒，CN_TIMEOUT_FOREVER=无限等待，0则立即按
//          超时返回。非0值将被向上调整为CN_CFG_TICK_US的整数倍
// 返回：实际写出的数据量；
// 备注：
// ============================================================================
s32 DevWrite(s32 handle, void *buf, u32 len, u32 offset, u32 timeout)
{
    struct __device * dev;
    tagOFile *devfile;

    if((!len) || (!buf))
        return (0);

    devfile = of(handle);
    if(!devfile)
        return (0);

    dev = (struct __device*)of_represent(devfile);
    if(dev && dev->dWrite)
        return (dev->dWrite(devfile, buf, len, offset, timeout));

    return (0);

}

// ============================================================================
// 功能：控制设备.
// 参数：dwFD,设备指针
//      dwCmd,命令号，driver.h中预定义了一些命令，以及用户命令的使用规则，可参阅
//          常量CN_DEV_CTRL_START定义附近代码。
//      data1，data2,控制命令所需要的参数，由设备开发者定义
// 返回：-1表示参数检查出错，其他值由设备开发者定义
// 备注：
// ============================================================================
s32 DevCntl(s32 dwFD, u32 dwCmd, ptu32_t data1, ptu32_t data2)
{
    struct __device *dev;
    tagOFile *devfile;

    devfile = of(dwFD);
    if(!devfile)
        return (-1);

    dev = (struct __device*)of_represent(devfile);
    if(dev && dev->dCntl)
        return (dev->dCntl(devfile, dwCmd, data1, data2));

    return (-1);
}
