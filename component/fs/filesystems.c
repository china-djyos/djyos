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
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <list.h>
#include <objfile.h>
#include "filesystems.h"
#include "dbug.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"filesystem"   //文件系统组件
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:核心组件            //选填“第三方组件、核心组件、bsp组件、用户组件”，本属性用于在IDE中分组
//select:可选                   //选填“必选、可选、不可选”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//grade:none                    //初始化时机，可选值：none，init，main。none表示无须初始化，
                                //init表示在调用main之前，main表示在main函数中初始化
//dependence:"none",             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
//%$#@num,0,100
//%$#@enum,true,false
//%$#@string
//%$#@select
//%$#@free
//%$#@end configue  ****参数配置结束
//@#$%component end configure


#define DJYFS_PATH_BUFFER_SIZE          266

static char __DJYFS_PATH_BUFFER[DJYFS_PATH_BUFFER_SIZE];
char *DJYFS_PATH_BUFFER = __DJYFS_PATH_BUFFER; // 用于移植的文件系统的路径拼接

struct filesystem{
    struct dListNode list;
    tagFST *pType;
} *pFileSystemTypes;


// ============================================================================
// 功能：查找文件系统类型。
// 参数：pType -- 文件系统类型名。
// 返回：文件系统类型；未找到（NULL）；
// 备注：
// ============================================================================
tagFST *FS_Find(const char *pType)
{
    list_t *cur;
    struct filesystem *fs;

    if(!pFileSystemTypes)
        return (NULL);

    fs = pFileSystemTypes;
    if(!strcmp(fs->pType->pType, pType))
        return (fs->pType);

    dListForEach(cur, &pFileSystemTypes->list)
    {
        fs = dListEntry(cur, struct filesystem, list);

        if(!strcmp(fs->pType->pType, pType))
        {
            return (fs->pType);
        }
    }

    return (NULL);
}

// ============================================================================
// 功能：新的文件系统类型注册。
// 参数：pType -- 新的文件系统类型。
// 返回：成功（0）；失败（-1）。
// 备注：
// ============================================================================
s32 FS_Register(tagFST *pType)
{
    struct filesystem *new;

    if(FS_Find(pType->pType))
        return (-1);

    new = malloc(sizeof(*new));
    new->pType = pType;

    if(!pFileSystemTypes)
    {
        dListInit(&(new->list));
        pFileSystemTypes = new;
    }
    else
    {
        dListInsertAfter(&(pFileSystemTypes->list), &(new->list));
    }

    return (0);
}

// ============================================================================
// 功能：新的文件系统安装
// 参数：pSource -- 将要挂上的文件系统，通常是一个设备名；
//      pTarget -- 文件系统所要挂载的目标对象（目录）；
//      pType -- 文件系统类型；
//      dwFlags -- 文件系统读写访问标志；
//      pData -- 文件系统特有参数；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 FS_Mount(const char *pSource, const char *pTarget, const char *pType, u32 dwFlags, void *data)
{
    tagFST *type;
    tagFSC *super;
    struct Object *source, *target, *temp;
    s32 res;
    char *notfind;

    type = FS_Find(pType);
    if(!type)
    {
        debug_printf("FS","mount failed. cannot find \"%s\"<type>.", pType);
        return (-1);
    }

    source = OBJ_MatchPath(pSource, &notfind, 0);
    if(notfind)
    {
        // 未找到设备
        debug_printf("FS","mount failed. cannot find \"%s\"<device>.", pSource);
        return (-1);
    }

    target = OBJ_MatchPath(pTarget, &notfind, 0);
    if(notfind)
    {
#if 0
        // 未找到安装点
        target = OBJ_BufferPath(target, notfind); // 建立安装点
#else
        return (-1); // 安装点必须准备好。
#endif
    }
    else
    {
        // 目标节点已存在，判断其是否是目录
        res = __OBJ_Type(target);
        if((DIR_POINT == res) || (GROUP_POINT == res))
        {
            temp = target;
            target = OBJ_Replace(temp); // 原来的节点被新的替代。
            if(!target)
            {
                debug_printf("FS","mount failed. cannot mount on the \"%s\"<target replace>.", pTarget);
                return (-1);
            }

            __OBJ_SetMountPoint(temp); // 将原节点设置为安装点，防止其关闭时被释放（针对第三方的文件系统），因为恢复时仍需要。
        }
        else
        {
            debug_printf("FS","mount failed. cannot mount on the \"%s\"<target type>.", pTarget);
            return (-1);
        }

    }

    super = malloc(sizeof(*super));
    super->pTarget = target;
    super->pDev = source;
    __OBJ_SetRepresent(target, (ptu32_t)super);
    __OBJ_SetOps(super->pTarget, type->fileOps);
    res = type->install(super, dwFlags, data);
    if(res)
    {
         // 安装失败
        temp = OBJ_Restore(super->pTarget);
        __OBJ_ClearMountPoint(temp); //
        free(super);
        return (-1);
    }

    // if(!DJYFS_PATH_BUFFER)



    return (0);

}

// ============================================================================
// 功能：
// 参数：
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
void *FS_Core(struct Object *pObj)
{
    tagFSC *super;

    if(!pObj)
        return (NULL);

    super = (tagFSC *)__OBJ_Represent(pObj);
    if(!super)
        return (NULL);

    return (super->pCore);
}

