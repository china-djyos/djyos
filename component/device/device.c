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
#include <filesystems.h>
#include <../../djyos/core/component_config_core.h>
#include <lock.h>
#include "dbug.h"
#include "../include/device.h"
#include "component_config_devfile.h"

struct __device
{
    fnDevOpen dOpen; //
    fnDevClose dClose; //
    fnDevWrite dWrite; //
    fnDevRead dRead; //
    fnDevCntl dCntl; //
    ptu32_t DrvTag;        // 本设备驱动程序设置的标签
    ptu32_t UserTag;       // 应用程序设置的标签
};

struct Object *s_ptDeviceRoot; // TODO 返回object
static struct MemCellPool *s_ptDevicePool; // 设备控制块内存池头指针
static struct __device s_tDeviceInitPool[CFG_DEVFILE_LIMIT];
static struct MutexLCB s_tDeviceMutex; // 保护设备安全
static struct objhandle *s_ptDeviceDirHandle;

// ============================================================================
// 功能：测试标志是否传输完成才算写完成
// 参数：flags -- 文件使用标志；
// 返回：（1）写操作在传输完成后返回；（0）写操作在写入buffer即返回；
// ============================================================================
s32 handle_IsBlockComplete(u32 flags)
{
    if(flags & O_BLOCK_COMPLETE)
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
static struct objhandle *__Device_open(struct Object *ob, u32 flags, u32 timeout)
{
    struct objhandle *devfile;
    struct __device *dev;
    s32 res;

    devfile = Handle_New();
    if(!devfile)
        return (NULL);

    Handle_Init(devfile, ob, flags, 0);
    dev = (struct __device*)OBJ_GetPrivate(ob);
    if(dev)
    {
        if(dev->dOpen)
        {
            res = dev->dOpen(devfile, flags, timeout);
            if(res)
            {
                Handle_Delete(devfile);
                debug_printf("device","cannot open device \"%s\".", OBJ_GetName(ob));
                return (NULL);
            }
//            OBJ_LinkHandle(devfile, ob);      //将objhandle与obj关联
            return (devfile);
        }
        else
            return NULL;
    }
    else
    {
        if(File_ObjIsMount(ob))
        {
            // 操作的是"/dev"
            if(!Handle_FlagIsDirectory(flags))
            {
                Handle_Delete(devfile);
                return (NULL); // 只有安装点是目录，其他都是文件
            }
        }
        else
        {
            debug_printf("device","open device \"%s\" unknown(type).", OBJ_GetName(ob));
        }
    }

    return NULL;
}

// ============================================================================
// 功能：关闭设备；
// 参数：hdl -- 设备对象句柄；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __Device_close(struct objhandle *hdl)
{
    s32 res = 0;
    struct __device *dev = (struct __device*)handle_GetHostObjectPrivate(hdl);

    if(dev && dev->dClose)
        res = (s32)dev->dClose(hdl);

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
static s32 __Device_write(struct objhandle *hdl, u8 *data, u32 len)
{
    u32 res = 0;
    struct __device *dev = (struct __device*)handle_GetHostObjectPrivate(hdl);

    if(dev&&dev->dWrite)
    {
        res = dev->dWrite(hdl, data, len, 0, handle_gettimeout(hdl));
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
static ssize_t __Device_read(struct objhandle *hdl, u8 *data, ssize_t len)
{
    s32 res = 0;
    struct __device *dev = (struct __device*)handle_GetHostObjectPrivate(hdl);

    if(dev && dev->dRead)
    {
        res = dev->dRead(hdl, data, len, 0, handle_gettimeout(hdl));
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
static s32 __Device_dentry(struct objhandle *hdl, struct dirent *dentry)
{
    if(!dentry->d_ino)
        dentry->d_ino = (long)Handle_GetHostObj(hdl);

    dentry->d_ino = (long)OBJ_ForeachChild(Handle_GetHostObj(hdl), (struct Object*)(dentry->d_ino));
    if(dentry->d_ino)
    {
        strcpy(dentry->d_name, OBJ_GetName((struct Object*)(dentry->d_ino)));
        dentry->d_reclen = strlen(dentry->d_name);
        if(File_ObjIsMount((struct Object*)dentry->d_ino))
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
static s32 __Device_cntl(struct objhandle *hdl, u32 cmd, va_list *arg0)
{
    struct __device *dev;
    s32 ret = 0;

    switch(cmd)
    {
        case F_SETTIMEOUT:
        {
            u32 timeout;
            timeout = va_arg(*arg0, u32);
            Handle_SetTimeOut(hdl, timeout);
            break;
        }

        case F_GETTIMEOUT:
        {
            u32 *timeout;
            timeout = va_arg(*arg0, u32 *);
            *timeout = handle_gettimeout(hdl);
            break;
        }

        case F_GETDRVTAG:
        {
            ptu32_t *drv;
            drv = va_arg(*arg0, ptu32_t *);
            *drv = Device_GetDrvTag(Handle2fd(hdl));
            break;
        }

        case F_GETUSERTAG:
        {
            ptu32_t *usr;
            usr = va_arg(*arg0, ptu32_t *);
            *usr = Device_GetUserTag(Handle2fd(hdl));
            break;
        }

        case F_SETDRVTAG:
        {
            ptu32_t drv;
            drv = va_arg(*arg0, ptu32_t);
            Device_SetDrvTag(Handle2fd(hdl),drv);
            break;
        }

        case F_SETUSERTAG:
        {
            ptu32_t user;
            user = va_arg(*arg0, ptu32_t);
            Device_SetUserTag(Handle2fd(hdl),user);
            break;
        }

        default:
            dev = (struct __device*)handle_GetHostObjectPrivate(hdl);
            if(dev && dev->dCntl)
                ret = dev->dCntl(hdl, cmd, arg0);
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
static s32 __Device_stat(struct Object *ob, struct stat *data)
{
    struct __device *dev;

    memset(data, 0x0, sizeof(*data));
    dev = (struct __device*)OBJ_GetPrivate(ob);
    if(dev)
    {
        data->st_mode = S_IFFLOW|S_IRUGO|S_IWUGO; // 设备文件都是流式数据；
        data->st_dev = (dev_t)dev;
    }
    else
    {
        return -1;
    }

    return (0);
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//----设备目录操作函数---------------------------------------------------------
//功能：实现打开和读目录的功能。
//参数：Target，参考函数指针类型 fnObjOps 的定义
//      cmd，命令码，参见 CN_OBJ_CMD_OPEN 族定义
//      para1，para2：与命令码相关
//返回：与命令码相关
//-----------------------------------------------------------------------------
s32 Device_DirObjOps(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                         ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
    s32 result = CN_OBJ_CMD_EXECUTED;
    switch(objcmd)
    {
        case CN_OBJ_CMD_OPEN:
        {
            OBJ_DutyUp((struct Object*)opsTarget);
            *(struct objhandle **)OpsArgs1 = s_ptDeviceDirHandle;
        }break;
        case CN_OBJ_CMD_READDIR:
        {
            DIR *dir = (DIR*)OpsArgs1;
            struct dirent *direntry;
            if(dir->__fd == NULL)
                dir->__fd = (struct Object*)opsTarget;
            dir->__fd = OBJ_ForeachChild((struct Object*)opsTarget,dir->__fd);
            if(dir->__fd == NULL)
                result = 1;
            else
            {
                direntry = &dir->__ptr;
                strcpy(direntry->d_name, OBJ_GetName((struct Object *)(dir->__fd)));
                direntry->d_reclen = strlen(direntry->d_name);
                direntry->d_type |= S_IFREG;
                result = 0;
            }
        }break;
        default:
            result = CN_OBJ_CMD_UNSUPPORT;
            break;
    }
    return result;
}
#pragma GCC diagnostic pop

// ============================================================================
// 功能：设备操作集
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 Device_DevObjOps(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                         ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
    s32 result = CN_OBJ_CMD_EXECUTED;

    switch(objcmd)
    {
        case CN_OBJ_CMD_OPEN:
        {
            struct objhandle *hdl;
            if((char*)OpsArgs3 != NULL)     // 设备都是都已经存在了的，opsTarget后应该是NULL
            {
                *(struct objhandle **)OpsArgs1 = NULL;
                break;
            }
            if(Handle_FlagIsOnlyCreate((u32)(*(u64*)OpsArgs2)))       // dev设备不能新建、或者是目录
            {
                *(struct objhandle **)OpsArgs1 = NULL;
                break;
            }
            hdl = __Device_open((struct Object *)opsTarget, (u32)(*(u64*)OpsArgs2), CN_TIMEOUT_FOREVER);
            *(struct objhandle **)OpsArgs1 = hdl;
            break;
        }

//      case OBJ_TRAVERSE: // TODO : 这个是不是可以考虑不放在这里
//      {
//          struct objhandle *devfile = (struct objhandle*)opsTarget;
//          struct dirent *ret = (struct dirent*)OpsArgs1;
//
//          return(__Device_dentry(devfile, ret));
//      }

        case CN_OBJ_CMD_READ:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = __Device_read(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_WRITE:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = __Device_write(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_CLOSE:
        {
            __Device_close((struct objhandle*)opsTarget);
            result = CN_OBJ_CMD_TRUE;
            break;
        }

        case CN_OBJ_CMD_STAT:
        {
            char * path = (char*)OpsArgs2;
            if(path&&('\0'!=*path))
                return (-1);        // 查询的文件不存在；
            if(__Device_stat((struct Object*)opsTarget, (struct stat *)OpsArgs1) == 0)
            {
                result = CN_OBJ_CMD_TRUE;
            }
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_FCNTL:
        case CN_OBJ_IOCTL:
        {
//          ptu32_t arg0, arg1;
//          struct objhandle *devfile = (struct objhandle*)opsTarget;
//          u32 cmd = (u32)OpsArgs1;
//
//          va_start(list, OpsArgs1);
//          list = *((va_list*)va_arg(list, u32)); // 这里传进来的本事就是参数的地址
//          arg0 = (ptu32_t)va_arg(list, u32);
//          arg1 = (ptu32_t)va_arg(list, u32);
//          va_end(list);

            *(s32*)OpsArgs1 = __Device_cntl((struct objhandle*)opsTarget, OpsArgs2,
                                                (va_list *)OpsArgs3);
            break;
        }

        default:
        {
            return CN_OBJ_CMD_UNSUPPORT;
            break;
        }
    }

    return result;
}

// ============================================================================
// 功能：设置设备的驱动标签；
// 参数：fd -- 设备文件描述符；
//       DrvTag --设备的驱动标签；
// 返回：
// 备注：
// ============================================================================
void Device_SetDrvTag(s32 fd,ptu32_t DrvTag)
{
    struct __device *dev;
    struct objhandle *hdl;

    if(fd >= 0)
    {
        hdl = fd2Handle(fd);
        dev = (struct __device *)handle_GetHostObjectPrivate(hdl);
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
ptu32_t Device_SetUserTag(s32 fd,ptu32_t UserTag)
{
    struct __device *dev;
    struct objhandle *hdl;

    if(fd >= 0)
    {
        hdl = fd2Handle(fd);
        dev = (struct __device *)handle_GetHostObjectPrivate(hdl);
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
ptu32_t Device_GetDrvTag(s32 fd)
{
    struct __device *dev;
    struct objhandle *hdl;

    if(fd >= 0)
    {
        hdl = fd2Handle(fd);
        dev = (struct __device *)handle_GetHostObjectPrivate(hdl);
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
ptu32_t Device_GetUserTag(s32 fd)
{
    struct __device *dev;
    struct objhandle *hdl;

    if(fd >= 0)
    {
        hdl = fd2Handle(fd);
        dev = (struct __device *)handle_GetHostObjectPrivate(hdl);
        return (dev->UserTag);
    }

    return (0);
}

//// ============================================================================
//// 功能：添加设备子类
//// 参数：name -- 设备子类名；
//// 返回：成功（设备子类）；失败（NULL）；
//// 备注：
//// ============================================================================
//struct Object *dev_group_addo(char *name)
//{
//#if 0
//    struct Object *ob;
//    struct __portBasic *group;
//
//    group = malloc(sizeof(*group));
//    if(!group)
//        return(NULL);
//
//    handle_initbasic(group, O_RDWR);
//    ob = of_virtualizeo(s_ptDeviceRoot, group, name);
//    if(!ob)
//    {
//        return (NULL);
//    }
//
//    return (ob);
//#else
//    struct Object *ob;
//
//    ob = OBJ_NewChild(s_ptDeviceRoot, Device_DevObjOps, 0, name);
//    if(!ob)
//        return (NULL);
//
//    if(obj_allow2set(ob)) // 允许对象上建立集合(类)；
//    {
//        OBJ_Delete(ob);
//        return (NULL);
//    }
//
//    if(obj_change2set(ob)) // 将对象转为集合（类）；
//    {
//        OBJ_Delete(ob);
//        return (NULL);
//    }
//
//    return (ob);
//#endif
//}

//// ============================================================================
//// 功能：删除设备子类
//// 参数：grp -- 设备子类节点；
//// 返回：成功（0）；失败（-1）；
//// 备注：
//// ============================================================================
//s32 dev_group_delo(struct Object *grp)
//{
//    struct Object *dev0, *devx;
//    u8 end = 0;
//    s32 res = 0;
//
//    if((!grp)
//       ||(!__isdev(grp)))
//        return (-1);
//
//    dev0 = OBJ_GetChild(grp);
//    do
//    {
//        devx = OBJ_GetNext(dev0);
//        if(devx == dev0)
//            end = 1; // 最后一个成员；
//
//        if(OBJ_IsOnDuty(devx))
//            res = -1; // 设备正在使用中，无法删除
//        else
//            OBJ_Delete(devx);
//    }
//    while(!end);
//
//    if(!res)
//    {
//        if(OBJ_IsOnDuty(grp))
//        {
//            OBJ_Delete(grp);
//        }
//        else
//        {
//            res = -1; // 类目录正在使用中，无法删除
//        }
//    }
//
//    return (res);
//}

//// ============================================================================
//// 功能：删除设备子类
//// 参数：name -- 设备子类名；
//// 返回：成功（0）；失败（-1）；
//// 备注：
//// ============================================================================
//s32 dev_group_del(char *name)
//{
//    char *left, *tmp;
//    u8 index = 0;
//    struct Object *ob;
//
//    if(name)
//    {
//        // 判断group格式，拼接完整的/dev路径
//        if('/'==name[0])
//        {
//            index = 1;
//        }
//
//        if(('d'==name[index])&&('e'==name[index+1])
//           &&('v'==name[index+2])&&('/'==name[index+3]))
//        {
//            tmp = name; // group已经含有了dev/
//        }
//        else
//        {
//            tmp = malloc(strlen(name)+4);
//            if(!tmp)
//            {
//                printf("\r\n: warn : device : memory out(%s).", tmp);
//                return (-1);
//            }
//
//            sprintf(tmp, "%s/%s", "dev/", name);
//        }
//
//        ob = OBJ_MatchPath(tmp, &left, NULL);
//        if((!left) || (ob))
//        {
//            free(tmp);
//            return (-1);
//        }
//
//        free(tmp);
//    }
//
//    return (dev_group_delo(ob));
//}

struct Object *Device_Create(const char *name,fnDevOpen dopen, fnDevClose dclose,
                               fnDevWrite dwrite, fnDevRead dread,
                               fnDevCntl dcntl, ptu32_t DrvTag)
{

   struct Object *devo;
   struct __device *dev;


   dev = (struct __device*)Mb_Malloc(s_ptDevicePool, 0);
   if(!dev)
       return (NULL);

   devo = OBJ_NewChild(s_ptDeviceRoot, Device_DevObjOps, (ptu32_t)dev, name);
   if(!devo)
   {
       Mb_Free(s_ptDevicePool, dev);
       return (NULL);
   }

   dev->dClose = dclose;
   dev->dOpen = dopen;
   dev->dCntl = dcntl;
   dev->dRead = dread;
   dev->dWrite = dwrite;
   dev->DrvTag = DrvTag;
   return (devo);
}

//// ============================================================================
//// 功能：添加一个设备到其设备类；
//// 参数：grp -- 设备子类；为NULL，则表示其是dev的子类；
////      name -- 设备名；
//// 返回：成功（设备对象）；失败（NULL）；
//// 备注：
//// ============================================================================
//struct Object *dev_addo(struct Object *grpo, const char *name,
//                               fnDevOpen dopen, fnDevClose dclose,
//                               fnDevWrite dwrite, fnDevRead dread,
//                               fnDevCntl dcntl, ptu32_t DrvTag)
//{
//
//    struct Object *devo;
//    struct __device *dev;
//
//    if(!grpo)
//    {
//        grpo = s_ptDeviceRoot;
//    }
//    else
//    {
//        if((grpo!=s_ptDeviceRoot)
//           &&(OBJ_GetParent(grpo)!=s_ptDeviceRoot)) // 逻辑上设备类对象的父必须是dev（控制对象树深度）
//            return (NULL);
//    }
//
//    dev = (struct __device*)Mb_Malloc(s_ptDevicePool, 0);
//    if(!dev)
//        return (NULL);
//
//    devo = OBJ_NewChild(grpo, NULL, (ptu32_t)dev, name);
//    if(!devo)
//    {
//        Mb_Free(s_ptDevicePool, dev);
//        return (NULL);
//    }
//
//    dev->dClose = dclose;
//    dev->dOpen = dopen;
//    dev->dCntl = dcntl;
//    dev->dRead = dread;
//    dev->dWrite = dwrite;
//    dev->DrvTag = DrvTag;
//    return (devo);
//}

//// ============================================================================
//// 功能：添加一个设备到其设备类；
//// 参数：grp -- 设备类；
////       name -- 设备名；
//// 返回：成功（0）；失败（-1）；
//// 备注：
//// ============================================================================
//s32 dev_add(const char *grp, const char *name, fnDevOpen dopen, fnDevClose dclose,
//            fnDevWrite dwrite, fnDevRead dread, fnDevCntl dcntl, ptu32_t dtag)
//{
//    struct Object *ob = s_ptDeviceRoot;
//    char *left, *tmp;
//    u8 index = 0;
//
//    if(grp)
//    {
//        // 判断group格式，拼接完整的/dev路径
//        if('/' == grp[0])
//        {
//            index = 1;
//        }
//
//        if(('d' == grp[index]) && ('e' == grp[index+1]) &&
//           ('v' == grp[index+2]) && ('/' == grp[index+3]))
//        {
//            tmp = (char*)grp; // group已经含有了dev/
//        }
//        else
//        {
//            tmp = malloc(strlen(grp)+6);
//            if(!tmp)
//            {
//                warning_printf("device","memory out(%s).", tmp);
//                return (-1);
//            }
//
//            sprintf(tmp, "%s/%s", "/dev", grp);
//        }
//
//        ob = OBJ_MatchPath(tmp, &left, NULL);
//        if(left||(!ob))
//        {
//            free(tmp);
//            return (-1);
//        }
//
//        free(tmp);
//    }
//
//    if(dev_addo(ob, name, dopen, dclose, dwrite, dread, dcntl, dtag))
//        return (0);
//
//    return (-1);
//}

// ============================================================================
// 功能：删除设备对象；
// 参数：devo -- 设备节点；
// 返回：成功（0）；失败（-1）；
// 备注：与文件系统接口要形成互斥；（要是在删除的过程中出现了使用）
// ============================================================================
s32 Device_DeleteAtObject(struct Object *dev)
{
    // struct device *device;

    if((!dev) ||(OBJ_IsOnDuty(dev))) // 设备被使用中
        return (-1);

    Mb_Free(s_ptDevicePool, (void*)OBJ_GetPrivate(dev));
    OBJ_Delete(dev);
    return (0);
}

// ============================================================================
// 功能：删除设备；
// 参数：grp -- 设备子类；
//      name -- 设备名；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 Device_DeleteByName(const char *name)
{
    struct Object *ob;

    ob = OBJ_SearchChild(s_ptDeviceRoot, name); // 查找设备节点；
    if(!ob)
        return (-1);

    return(Device_DeleteAtObject(ob));
}

//// ============================================================================
//// 功能：取对象取设备的驱动标签；
//// 参数：devo -- 设备对象；
//// 返回：struct __device的 DrvTag 成员。出错返回-1，但用户以返回-1为出错条件要谨慎，因为
////      DrvTag 的真实值也可能是-1.
//// 备注：如果对非 Dev 的 object 调用此函数，后果不可预料。
//// ============================================================================
//ptu32_t dev_GetDrvTagFromObj(struct obj *devboj)
//{
//    struct __device *dev;
//
//    if(!devboj)
//        return (0);
//
//    dev = (struct __device*)OBJ_GetPrivate(devboj);
//    if(dev)
//        return (dev->DrvTag);
//    else
//        return (0);
//}

// ============================================================================
// 功能：获取设备对象的名字
// 参数：devo -- 设备对象指针
// 返回：设备对象的名字
// 备注：
// ============================================================================
const char *Device_GetName(struct Object *devo)
{
    if(devo)
        return (OBJ_GetName(devo));

    return (NULL);
}

// ============================================================================
// 功能：初始化设备文件系统
// 参数：
// 返回：
// 备注：TODO
// ============================================================================
s32 ModuleInstall_dev(void)
{
    s_ptDevicePool = Mb_CreatePool((void*)s_tDeviceInitPool,
                                   CFG_DEVFILE_LIMIT,
                                   sizeof(struct __device),
                                   0, 0,
                                   "设备控制块池");
    if(!s_ptDevicePool)
    {
        debug_printf("device","cannot install device file system(pool).");
        return (-1);
    }

    Lock_MutexCreate_s(&s_tDeviceMutex, "dev");
    s_ptDeviceRoot = OBJ_NewChild(OBJ_GetRoot(), Device_DirObjOps, 0, "dev");
    s_ptDeviceDirHandle = Handle_New();
    if((NULL == s_ptDeviceRoot) ||(NULL == s_ptDeviceDirHandle))
    {
        OBJ_Delete(s_ptDeviceRoot);
        Handle_Delete(s_ptDeviceDirHandle);
        debug_printf("device","cannot install device file system(mount).");
        return (-1);
    }
    else
    {
        Handle_Init(s_ptDeviceDirHandle,s_ptDeviceRoot,O_DIRECTORY,0);
        OBJ_LinkHandle(s_ptDeviceDirHandle,s_ptDeviceRoot);
        return (0);
    }
}

// ============================================================================
// 功能：根据设备名打开设备，搜索整个设备资源树，找到名称与name匹配的资源结点，
//      返回该句柄指针。多线程互斥方式如下:
//      A线程以D_RDWR模式打开,------其他线程不能再打开
//      A线程以D_RDONLY模式打开,----任意线程(包含A)还可以用D_WRONLY模式打开
//      D_WRONLY同上
// 参数：name,设备名字符串,包含路径名，但不必包含'dev\'这4个字符
//      flags,设备打开模式,O_RDONLY,O_WRONLY,O_RDWR 中的一个
//      timeout，超时设置,单位是微秒，CN_TIMEOUT_FOREVER=无限等待，0则立即按
//          超时返回。非0值将被向上调整为CN_CFG_TICK_US的整数倍
// 返回：成功打开设备返回描述符,否则返回-1.
// 备注：O_RDONLY、O_WRONLY、O_RDWR 须符合POSIX（2016）的最新定义
// ============================================================================
s32 Device_Open(const char *name, s32 flags, u32 timeout)
{
    struct Object *dev;
    struct objhandle *devfile = NULL;

    if(!Lock_MutexPend(&s_tDeviceMutex, timeout))
        return (-1);

    dev = OBJ_SearchPath(s_ptDeviceRoot, name); // 在设备目录中搜索设备
    if(dev)
    {
        devfile = __Device_open(dev, flags, timeout);
    }

    Lock_MutexPost(&s_tDeviceMutex);
    if(!devfile)
        return (-1);

    return Handle2fd(devfile);
}

// ============================================================================
// 功能：关闭设备，由于设备受互斥量保护，故只有设备拥有者(即打开设备的事件)才能关
//      闭设备，这是由互斥量的特点决定的。
// 参数：fd -- 设备文件描述符
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 Device_Close(s32 fd)
{
#if 0
    struct __device * dev;
    struct objhandle* devFile;
    struct Object *ob;
    s32 Mode;

    devFile = fd2Handle( dwFD );
    if(devFile == NULL)
        return false;
    ob = Handle_GetHostObj(devFile);
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

    res = __Device_close(devfile);

    //注意：和socket一样，dev设备文件有自己的关闭函数，没有经过对象系统的close调用，故需要
    //自己删除句柄，像fat、yaffs2、efs等就不用自己删除。
    if(res == 0)
        Handle_Delete(devfile);

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
s32 Device_Read(s32 fd, void *buf, u32 len, u32 offset, u32 timeout)
{
    struct __device *dev;
    struct objhandle *devfile;

    if((!len) || (!buf))
        return (0);

    devfile = fd2Handle(fd);
    if(!devfile)
        return (0);

    dev = (struct __device*)handle_GetHostObjectPrivate(devfile);
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
s32 Device_Write(s32 fd, void *buf, u32 len, u32 offset, u32 timeout)
{
    struct __device * dev;
    struct objhandle *devfile;

    if((!len) || (!buf))
        return (0);

    devfile = fd2Handle(fd);
    if(!devfile)
        return (0);

    dev = (struct __device*)handle_GetHostObjectPrivate(devfile);
    if(dev&&dev->dWrite)
        return (dev->dWrite(devfile, buf, len, offset, timeout));

    return (0);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// ============================================================================
// 功能：控制设备.
// 参数：fd -- 设备指针
//       dwCmd -- 命令号，driver.h中预定义了一些命令，以及用户命令的使用规则，可参阅
//          常量 CN_DEV_CTRL_START 定义附近代码。
//       data1，data2 -- 控制命令所需要的参数，由设备开发者定义
// 返回：-1表示参数检查出错，其他值由设备开发者定义
// 备注：
// ============================================================================
s32 Device_Ctrl(s32 fd, u32 dwCmd, ptu32_t data1, ptu32_t data2)
{
//    struct __device *dev;
//    struct objhandle *devfile;
//
//    devfile = fd2Handle(fd);
//    if(!devfile)
//        return (-1);
//
//    dev = (struct __device*)handle_GetHostObjectPrivate(devfile);
//    if(dev && dev->dCntl)
//        return (dev->dCntl(devfile, dwCmd, data1, data2));
//
    return (-1);
}
#pragma GCC diagnostic pop
