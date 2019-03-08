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
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <list.h>
#include <object.h>
#include <objhandle.h>
#include <stdio.h>
#include "filesystems.h"
#include "dbug.h"
#include "component_config_fs.h"
#include <device.h>


static char __DJYFS_PATH_BUFFER[DJYFS_PATH_BUFFER_SIZE];
char *DJYFS_PATH_BUFFER = __DJYFS_PATH_BUFFER; // 用于移植的文件系统的路径拼接


// ============================================================================
// 功能：查找文件系统类型。
// 参数：pType -- 文件系统类型名。
// 返回：文件系统类型；未找到（NULL）；
// 备注：
// ============================================================================
static struct filesystem *__findtype(const char *pType)
{
    list_t *cur;
    struct filesystem *fs;

    if(!pFileSystemTypes)
        return (NULL);

    fs = pFileSystemTypes;
    if(!strcmp(fs->pType, pType))
        return (fs);

    dListForEach(cur, &pFileSystemTypes->list)
    {
        fs = dListEntry(cur, struct filesystem, list);
        if(!strcmp(fs->pType, pType))
        {
            return (fs);
        }
    }

    return (NULL);
}

// ============================================================================
// 功能：新的文件系统类型注册。
// 参数：pType -- 新的文件系统类型。
// 返回：成功（0）；失败（-1）。已注册（1）；
// 备注：
// ============================================================================
s32 regfs(struct filesystem *type)
{
    if(__findtype(type->pType))
        return (1);

    if(!pFileSystemTypes)
    {
        dListInit(&(type->list));
        pFileSystemTypes = type;
    }
    else
    {
        dListInsertAfter(&(pFileSystemTypes->list), &(type->list));
    }

    return (0);
}

// ============================================================================
// 功能：mount点的文件操作接口，opsTarget 肯定是 mountpoint，但实际操作的，可能是下级
//      路径上的文件或目录，只是该文件或目录还在磁盘里面，还没有挂到object树中。此时，应
//      该转调用具体文件系统的ops。所有 opsTarget 是 struct obj *的命令，OpsArgs3 均
//      表示被操作路径中 opsTarget 后的部分，因此，只要 opsTarget 是 struct obj *，且
//      opsTarget != NULL 的，就要调用具体文件系统的 ops 函数。
// 参数：标准逻辑，查看接口说明；
// 返回：标准逻辑，查看接口说明；
// 备注:
// ============================================================================
s32 __mount_ops(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                       ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
    struct FsCore *super;
    struct obj *me;
    fnObjOps MyOps;
    s32 result = CN_OBJ_CMD_EXECUTED;

    if((objcmd & CN_TARGET_IS_OBJ) && ((((u32)(*(u64*)OpsArgs2) & O_DIRECTORY) || (char*)OpsArgs3 != NULL)))
    {
        me = (struct obj *)opsTarget;
        super = (struct FsCore *)obj_GetPrivate(me);
        MyOps = super->pFsType->fileOps;
        result = MyOps(opsTarget, objcmd, OpsArgs1, OpsArgs2, OpsArgs3);
    }
    else
    {
        switch(objcmd)
        {
            case CN_OBJ_CMD_STAT:
            {
                ((struct stat *)OpsArgs1)->st_mode = S_IFMOUNT;
                result = CN_OBJ_CMD_TRUE;
                break;
            }

            default:
            {
                result = CN_OBJ_CMD_UNSUPPORT;
                break;
            }
        }
    }
    return (result);
}

//----获取完整路径---------------------------------------------------------------
//功能：获取某文件系统内、自挂节点开始的完整路径，例如，yaffs2被mount到"/abc/d/"目录，
//      参数object对应"/abc/d/123/"，path = "456/my.txt"，则结果是"123/456/my.txt"
//参数： BaseObject，起点对象指针，必须包含mount点
//      PathTail，相对于 BaseObject 的路径
//      EntirePath，返回完整路径的buffer
//返回：false = 失败，true = 成功
//-----------------------------------------------------------------------------
bool_t GetEntirePath(struct obj *BaseObject, char * PathTail, char * EntirePath,
                     u32 BufSize)
{
    struct obj *current;
    char *Name;
    char *Entire = EntirePath;
    u32 objnum = 0,len;
    s32 offset = BufSize;
    //这个循环，沿 BaseObject->parent 一直查到挂载点，并把各级 object->name指针保存
    //在 EntirePath 中，从 EntirePath 的后面往前存。
    for(current = BaseObject; current != obj_root(); current = obj_parent(current))
    {
        offset -= sizeof(void*);
        if(offset < 0)
            return false;
        objnum++;
        memcpy(EntirePath + offset, &current->name, sizeof(void*));
        if(obj_GetOps(current) == __mount_ops)
            break;
    }
    //从 EntirePath 取出指针，把名字依序copy到 EntirePath 中。
    for(; objnum != 0; objnum--)
    {
        memcpy(&Name, EntirePath + offset, sizeof(void*));
        offset += sizeof(void*);
        len = strlen(Name);
        memcpy(Entire,Name,len);
        Entire[len] = '/';
        Entire += len+1;
    }
    //把PathTail中的字符串copy到EntirePath中
    Name = PathTail;
    while('/' == *Name)
        Name++; // 过滤多余的'/'
    if((Name != 0) && (*Name != 0))
    {
        len = strlen(Name);
        strcpy(Entire,Name);
    }
    else
    {
        if(*(Entire - 1) == '/')
        {
            *(Entire - 1) = '\0';
        }
    }
    return true;
}

// ============================================================================
// 功能：对象句柄关联的文件是否是目录；
// 参数：hdl -- 对象句柄；
// 返回：目录（1）；非目录（0）；
// ============================================================================
bool_t isDirectory(struct objhandle *hdl)
{
    mode_t mymode;
    mymode = (mode_t)handle_GetHostObjectPrivate(hdl);
    return S_ISDIR(mymode);
}


// ============================================================================
// 功能：判断某对象是否文件系统 mount 点
// 参数：obj，对象；
// 返回：true = 是 mount 点，false = 不是
// ============================================================================
bool_t obj_isMount(struct obj *obj)
{
    if(obj_GetOps(obj) == __mount_ops)
        return true;
    else
        return false;
}
// ============================================================================
// 功能：新的文件系统安装
// 参数：source -- 将要挂上的文件系统，通常是一个设备名；
//      target -- 文件系统所要挂载的目标对象（目录）；
//      type -- 文件系统类型；
//      flags -- 文件系统读写访问标志；
//      data -- 文件系统特有参数；
// 返回：成功（0）；失败（-1）；还未挂载到具体的媒体上去（-2）；
// 备注：
// ============================================================================
s32 mountfs(const char *source, const char *target, const char *type, u32 opt, void *data)
{
    struct filesystem *fstype;
    struct FsCore *super;
    struct obj *targetobj, *tmpobj;
//    s32 res;
    char *notfind;

    fstype = __findtype(type);
    if(!fstype)
    {
        debug_printf("fs","mount failed(cannot find type \"%s\")", type);
        return (-1);
    }

    targetobj = obj_matchpath(target, &notfind);
    if(notfind)
    {
#if 0
        // 未找到安装点
        targetobj = obj_buildpath(targetobj, notfind); // 建立安装点
#else
        return (-1); // 安装点必须准备好。
#endif
    }
    else
    {
        super = malloc(sizeof(*super));
        memset(super, 0, sizeof(*super));
        super->pFsType = fstype;
        if(opt & MS_DIRECTMOUNT)
        {
            obj_SetPrivate(targetobj, (ptu32_t)super);
            super->pTarget = targetobj;
            super->MountBak = NULL;
        }
        else
        {
            tmpobj = obj_newprev(targetobj, __mount_ops, (ptu32_t)super, obj_name(targetobj));
            obj_detach(targetobj);
            super->pTarget = tmpobj;
            super->MountBak = targetobj;
        }
        super->InstallWay = opt;
        super->Config = data;
    }

    return (0);
}

// ============================================================================
// 功能：获取文件系统对象（集合点）的管理体
// 参数：ob -- 文件系统对象集合；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
void *corefs(struct obj *ob)
{
    struct FsCore *super;
    struct obj *parent;

    if(!ob)
        return (NULL);

//  if(!obj_isMount(ob)) // 对象不是集合点；
//  {
//      ob = obj_set(ob); // 获取对象集合；
//      if((!ob)&&(!obj_isMount(ob)))
//          return (NULL); // 不是集合或者不存在；
//  }
    parent = ob;
    while( (!obj_isMount(parent)) && (parent != obj_root()))
    {
        parent = obj_parent(parent);
    }
    super = (struct FsCore *)obj_GetPrivate(parent);
    if(!super)
        return (NULL);

    return (super->pCore);
}

// ============================================================================
// 功能：判断文件目标文件系统是否有指定media，如果没有指定，则把文件系统安装到source上去
// 参数：source -- 将要挂上文件系统的媒体，通常是一个设备名；
//      target -- 文件系统所要挂载的目标对象；
// 返回：
// 备注：
// ============================================================================
void FsBeMedia(const char *source, const char *target)
{
    struct obj *srcobj,*targetobj;
    char *notfind;
    struct FsCore *super;
    s32 res;

    if((!target) || (!source))
    {
        return ;
    }
    srcobj = obj_matchpath(source, &notfind);
    if(notfind)
    {
        return ;
    }
    targetobj = obj_matchpath(target, &notfind);
    if(notfind)
    {
        return ;
    }
    super = (struct FsCore *)obj_GetPrivate(targetobj);
    if(!super->MediaDrv)
    {
        return ;        //如果没有找到媒体驱动则不挂载文件系统
    }
    res = super->pFsType->install(super,super->InstallWay,super->Config);  //挂载
    if(res == 0)
    {
        debug_printf("fs","file system \"%s\" installed on \"%s\".",targetobj->name,srcobj->name);
    }
    else
    {
        error_printf("fs","file system \"%s\" installed fail on \"%s\".",targetobj->name,srcobj->name);
    }

}
