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
//所属模块: 资源管理
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 创建、添加、移动、删除、搜索资源结点的服务
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __OBJECT_H__
#define __OBJECT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <list.h>
#include <stdio.h>

#define CN_OBJ_NAME_LIMIT   255

//Target根据cmd不同，有两种可能，struct Object*，或struct ObjectPort*，在实现
//时使用强制类型转换。
//struct Object*：CN_OBJ_CMD_OPEN、CN_OBJ_CMD_SHOW、CN_OBJ_CMD_READDIR、
//                CN_OBJ_CMD_DELETE、CN_OBJ_CMD_STAT
//struct ObjectPort*：其他命令

//对于4个参数不够用的情况，para2有两种应对方案。
//1、使用va_list *；2、用结构指针
typedef ptu32_t (*fnObjOps)(u32 dwCMD, ptu32_t context, ptu32_t args, ...);
//不同命令参数含义：
//-----------------CN_OBJ_CMD_OPEN---------------------------------
//功能：打开文件（目录）
// target：struct Object *，待打开的path中已经在Object队列中的最后一个Object
// para1：path中target后的部分，可能是NULL。
// para2：u64*，低32位是open调用的oflag参数，高32位仅当 cmd == O_CREAT 时有用，
//        表示文件访问权限， S_ISUID 系列常数，stat.h中定义
//-----------------CN_OBJ_CMD_READ---------------------------------
//功能：读文件
// target：struct ObjectPort * 类型，被读取的文件指针
// para1：接收数据的 buf 指针。
// para2：buf 的尺寸。
//----------------CN_OBJ_CMD_WRITE---------------------------------
//功能：写文件
// target：struct ObjectPort * 类型，被写入的文件指针
// para1：写入数据的 buf 指针。
// para2：写入数据的长度。
//----------------CN_OBJ_CMD_CLOSE---------------------------------
//功能：关闭文件（目录）
// target：struct ObjectPort * 类型，被关闭的文件指针
// para1：无用。
// para2：无用。
//----------------CN_OBJ_CMD_SHOW----------------------------------
//功能：显示文件（目录）内容，注意，目录的内容，不是子目录列表，可能是目录的说明，
//      也可能是其他内容，由CN_OBJ_CMD_SHOW目录的实现者决定
// target：struct Object * 类型，被显示的object指针
// para1：显示参数。
// para2：无用。
//----------------CN_OBJ_CMD_SEEK----------------------------------
//功能：移动文件指针
// target：struct ObjectPort * 类型，被操作的文件指针
// para1：off_t *，目标偏移量。
// para2：偏移参考，SEEK_SET、SEEK_CUR、SEEK_END 之一。

//对象操作命令定义，新增不能超过CN_OBJ_CMD_USER，否则整个系统需要重新编译。
//16bit，fnObjOps长度参数cmd是32位数，其中高16位表示命令码所需的参数长度。
//参数长度用于向内核态 copy 参数时使用，用户定义命令码时无须关心。
#define CN_OBJ_CMD_OPEN             0   // 打开文件。(返回：正常：struct ObjectPort 指针；出错：NULL；)
#define CN_OBJ_CMD_READ             1   // 读文件。(返回：读出的数据量；出错：-1。)
#define CN_OBJ_CMD_WRITE            2   // 写文件。返回：写入的数据量，出错：-1
#define CN_OBJ_CMD_CLOSE            3   // 关闭文件。返回：正常：0；文件类型错误：-2，其他错误：-1
#define CN_OBJ_CMD_SHOW             4   // 返回：正常：0，出错：-1
#define CN_OBJ_CMD_CRTL             5   // 控制文件；返回：由控制命令具体定义
#define CN_OBJ_CMD_SEEK             6   // 重定位文件。(返回：正常：0，出错：-1)
#define CN_OBJ_CMD_TELL             7   // 。(返回：正常：0；出错：-1)
#define CN_OBJ_CMD_DELETE           8   // 删除文件。(返回：正常：0；出错：-1)
#define CN_OBJ_CMD_STAT             9   // 返回：正常：0，出错：-1
#define CN_OBJ_CMD_TRUNCATE         10  // 返回：正常：0，出错：-1
#define CN_OBJ_CMD_READDIR          11  // 返回：正常：0，出错：-1，完成：1
#define CN_OBJ_CMD_SYNC             12  // 返回：正常：0，出错：-1，完成：1
#define CN_OBJ_CMD_MULTIPLEX_ADD    13  // 返回：正常：0，出错：-1，完成：1
#define CN_OBJ_CMD_FILE_SPECIAL     14  // 返回：参看 CN_FILE_STATICDATA 等
#define CN_OBJ_CMD_FSTAT            15  // 返回：正常：0，出错：-1

#define CN_OBJ_FCNTL_START          0x1000 // fcntl 命令组起始命令
#define CN_OBJ_IOCTL_START          0x2000 // ioctl 命令组起始命令
#define CN_OBJ_CMD_USER             0xF000 // 用户自定义命令起始值，由用户写的驱动程序解释，不同模块可以定义重复的值

//建议（非强制）：遇到不支持的命令时 fnObjOps 函数返回值。
#define CN_OBJ_CMD_UNSUPPORT        (-1)


#define GROUP_POINT                 (1)
#define TEMPORARY_POINT             (2)
#define DIR_POINT                   (3)
#define REG_POINT                   (4)

typedef fnObjOps    tagObjOps;

struct Object *OBJ_NewSanityChild(struct Object*, fnObjOps, ptu32_t, const char*);
s32            OBJ_Del(struct Object*);
s32            OBJ_SetOps(struct Object*, fnObjOps);
s32            OBJ_SetRepresent(struct Object*, ptu32_t);
void           OBJ_SetCurrent(struct Object*);
struct Object *OBJ_AddToPrevious(struct Object*, fnObjOps, ptu32_t, const char*);
struct Object *OBJ_AddToNext(struct Object*, fnObjOps, ptu32_t, const char*);
struct Object *OBJ_AddChild(struct Object*, fnObjOps, ptu32_t, const char*);
struct Object *OBJ_AddChildHead(struct Object*,fnObjOps, ptu32_t,const char*);
struct Object *OBJ_InsertChild(struct Object*, struct Object*);
struct Object *OBJ_TakeOutBranch(struct Object*);
s32            OBJ_MoveToBranch(struct Object*,struct Object*);
s32            OBJ_MoveToLast(struct Object*);
s32            OBJ_MoveToHead(struct Object*);
s32            OBJ_MoveToNext(struct Object*, struct Object*);
s32            OBJ_MoveToPrevious(struct Object*, struct Object*);
s32            OBJ_RoundPrevious(struct Object*);
s32            OBJ_RoundNext(struct Object*);
s32            OBJ_Rename(struct Object*, const char*);
fnObjOps       OBJ_Ops(struct Object*);
ptu32_t        OBJ_Represent(struct Object*);
list_t        *OBJ_PortList(struct Object*);
const char    *OBJ_Name(struct Object*);
struct Object *OBJ_Parent(struct Object*);
struct Object *OBJ_Child(struct Object*);
struct Object *OBJ_Previous(struct Object*);
struct Object *OBJ_Next(struct Object*);
struct Object *OBJ_Head(struct Object*);
struct Object *OBJ_Twig(struct Object*);
u32            OBJ_Level(struct Object*);
bool_t         OBJ_IsParentChild(struct Object*, struct Object*);
bool_t         OBJ_IsLast(struct Object*);
bool_t         OBJ_IsHead(struct Object*);
struct Object *OBJ_TraveChild(struct Object*, struct Object*);
struct Object *OBJ_TraveScion(struct Object*, struct Object*);
struct Object *OBJ_SearchSibling(struct Object*, const char*);
struct Object *OBJ_SearchChild(struct Object*, const char*);
struct Object *OBJ_SearchScion(struct Object*, const char*);
struct Object *OBJ_Search(struct Object*, const char*);
u32            OBJ_Sequencing(struct Object*);
struct Object *OBJ_NewSanityChild(struct Object*, fnObjOps, ptu32_t, const char*);
struct Object *OBJ_Root(void);
struct Object *OBJ_Current(void);
s32            OBJ_IsRoot(struct Object*);
struct Object *OBJ_MatchPath(const char*, char**, char**);
struct Object *OBJ_BufferPath(struct Object*, char*);
void           OBJ_UnBufferPath(struct Object*);
s32            OBJ_NameSantiy(const char*);
struct Object *OBJ_Restore(struct Object*);
struct Object *OBJ_Replace(struct Object*);

s32            __OBJ_Type(struct Object*);
void           __OBJ_Free(struct Object*);
s32            __OBJ_Del(struct Object*);
void           __OBJ_Init(struct Object*);
const char    *__OBJ_Name(struct Object*);
fnObjOps       __OBJ_Ops(struct Object*);
ptu32_t        __OBJ_Represent(struct Object*);
struct Object *__OBJ_Parent(struct Object*);
struct Object *__OBJ_Child(struct Object*);
struct Object *__OBJ_Prev(struct Object *pObj);
struct Object *__OBJ_Next(struct Object *pObj);
s32            __OBJ_Link(struct Object*, list_t*);
s32            __OBJ_LinksCheck(struct Object*, u32);
s32            __OBJ_Protect(struct Object*);
s32            __OBJ_Forbid(struct Object*);
s32            __OBJ_IsProtected(struct Object*);
s32            __OBJ_Release(struct Object*);
s32            __OBJ_NameSantiy(const char*);
void           __OBJ_SetRepresent(struct Object*, ptu32_t);
void           __OBJ_SetName(struct Object*, const char*);
void           __OBJ_SetOps(struct Object*, fnObjOps);
s32            __OBJ_SetMountPoint(struct Object*);
s32            __OBJ_ClearMountPoint(struct Object*);
s32            __OBJ_InitSystem(void);

#ifdef __cplusplus
}
#endif

#endif //__OBJECT_H__

