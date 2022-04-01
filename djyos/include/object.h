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
#include <include/list.h>
#include <fcntl.h>

// fnObjOps 函数返回值。
#define CN_OBJ_CMD_UNSUPPORT    (-1)    //该object不支持此命令
#define CN_OBJ_CMD_EXECUTED     (0)     //命令被执行（返回非布尔值的命令，结果由OpsArgs1返回）
#define CN_OBJ_CMD_TRUE         (1)     //命令返回正确（对于返回布尔值的命令）
#define CN_OBJ_CMD_FALSE        (2)     //命令返回错误（对于返回布尔值的命令）
//opsTarget的类型，有两种可能，struct obj*，或struct objhandle*，详见具体命令说明
//习惯用法：OpsArgs1用作返回执行结果的指针
typedef s32 (*fnObjOps)(void *opsTarget, u32 opscmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3);
//不同命令参数含义：
//-----------------CN_OBJ_CMD_OPEN---------------------------------
//功能：打开文件（目录）
// opsTarget：struct Object *，待打开的path中已经在Object队列中的最后一个Object
// OpsArgs1：struct objhandle *，返回结果。
// OpsArgs2：u64*，低32位是open调用的 oflag(O_CREAT等) 参数，高32位仅当 cmd == O_CREAT
//         时有用，表示文件访问权限， S_ISUID 系列常数，stat.h中定义
// OpsArgs3：path中opsTarget后的部分，可能是NULL。
// 返回：CN_OBJ_CMD_UNSUPPORT 或 CN_OBJ_CMD_EXECUTED
//-----------------CN_OBJ_CMD_READ---------------------------------
//功能：读文件
// opsTarget：struct objhandle * 类型，被读取的文件指针
// OpsArgs1：ssize_t *，返回结果，实际读得的数据长度。
// OpsArgs2：接收数据的 buf 指针。
// OpsArgs3：buf 的尺寸。
// 返回：CN_OBJ_CMD_UNSUPPORT 或 CN_OBJ_CMD_EXECUTED
//----------------CN_OBJ_CMD_WRITE---------------------------------
//功能：写文件
// opsTarget：struct objhandle * 类型，被写入的文件指针
// OpsArgs1：ssize_t *，返回结果，实际写入的数据长度。
// OpsArgs2：写入数据的 buf 指针。
// OpsArgs3：写入数据的长度。
// 返回：CN_OBJ_CMD_UNSUPPORT 或 CN_OBJ_CMD_EXECUTED
//----------------CN_OBJ_CMD_CLOSE---------------------------------
//功能：关闭文件（目录）
// opsTarget：struct objhandle * 类型，被关闭的文件指针
// OpsArgs1：无用。
// OpsArgs2：无用。
// OpsArgs3：无用。
// 返回：CN_OBJ_CMD_TRUE 或 CN_OBJ_CMD_FALSE 或 CN_OBJ_CMD_UNSUPPORT
//----------------CN_OBJ_CMD_SHOW----------------------------------
//功能：显示文件（目录）内容，注意，目录的内容，不是子目录列表，可能是目录的说明，
//      也可能是其他内容，由CN_OBJ_CMD_SHOW目录的实现者决定
// opsTarget：struct Object * 类型，被显示的object指针
// OpsArgs1：无用。
// OpsArgs2：显示参数，由具体实现者定义。
// OpsArgs3：path中opsTarget后的部分，可能是NULL。
// 返回：CN_OBJ_CMD_SUCCESS 或 CN_OBJ_CMD_FALSE 或 CN_OBJ_CMD_UNSUPPORT
//----------------CN_OBJ_CMD_SEEK----------------------------------
//功能：移动文件指针
// opsTarget：struct objhandle * 类型，被操作的文件指针
// OpsArgs1：off_t *，返回结果，成功（重定位后的位置）；失败（-1）；
// OpsArgs2：off_t *，目标偏移量。
// OpsArgs3：偏移参考，SEEK_SET、SEEK_CUR、SEEK_END 之一。
// 返回：CN_OBJ_CMD_UNSUPPORT 或 CN_OBJ_CMD_EXECUTED
//----------------CN_OBJ_CMD_TELL----------------------------------
//功能：获取文件指针位置
// opsTarget：struct objhandle *，被操作的文件指针
// OpsArgs1：off_t *，文件指针当前位置指针
// OpsArgs2：无用。
// OpsArgs3：无用。
// 返回：CN_OBJ_CMD_UNSUPPORT 或 CN_OBJ_CMD_EXECUTED
//----------------CN_OBJ_CMD_DELETE----------------------------------
//功能：删除文件
// opsTarget：struct Object* 类型，被操作的对象指针
// OpsArgs1：无用。
// OpsArgs2：无用。
// OpsArgs3：path中opsTarget后的部分，可能是NULL。
// 返回：CN_OBJ_CMD_SUCCESS 或 CN_OBJ_CMD_FALSE 或 CN_OBJ_CMD_UNSUPPORT
//----------------CN_OBJ_CMD_STAT----------------------------------
//功能：取文件状态
// opsTarget：struct Object *，被操作的对象指针
// OpsArgs1：struct stat *，保存操作结果的结构指针
// OpsArgs2：无用。
// OpsArgs3：path中opsTarget后的部分，可能是NULL。
// 返回：CN_OBJ_CMD_SUCCESS 或 CN_OBJ_CMD_FALSE 或 CN_OBJ_CMD_UNSUPPORT
//----------------CN_OBJ_CMD_TRUNCATE----------------------------------
//功能：截断文件
// opsTarget：struct objhandle *，被操作的文件指针
// OpsArgs1：无用
// OpsArgs2：s64*，截断尺寸
// OpsArgs3：无用。
// 返回：CN_OBJ_CMD_SUCCESS 或 CN_OBJ_CMD_FALSE 或 CN_OBJ_CMD_UNSUPPORT
//----------------CN_OBJ_CMD_READDIR----------------------------------
//功能：读取目录
// opsTarget：struct Object* 类型，被操作的对象指针
// OpsArgs1：DIR *，保存操作结果的结构指针
// OpsArgs2：无用。
// OpsArgs3：无用
// 返回：CN_OBJ_CMD_SUCCESS 或 CN_OBJ_CMD_FALSE 或 CN_OBJ_CMD_UNSUPPORT
//----------------CN_OBJ_CMD_SYNC----------------------------------
//功能：同步文件，把文件缓冲区刷新到文件里。
// opsTarget：struct objhandle *，被操作的文件指针
// OpsArgs1：无用
// OpsArgs2：无用
// OpsArgs3：无用。
// 返回：CN_OBJ_CMD_SUCCESS 或 CN_OBJ_CMD_FALSE 或 CN_OBJ_CMD_UNSUPPORT

//对象操作命令定义，新增不能超过CN_OBJ_CMD_USER，否则整个系统需要重新编译。
//16bit，fnObjOps参数cmd是32位数，其中16~23位表示命令码所需的参数长度。
//参数长度用于向内核态 copy 参数时使用，用户定义命令码时无须关心。24~31bit用作标志
#define CN_TARGET_IS_OBJ      (1<<31)               //fnObjOps 函数的 opsTarget 参数是object
#define CN_TARGET_IS_HDL      (0<<31)               //fnObjOps 函数的 opsTarget 参数是 handle

#define CN_OBJ_CMD_MSK        0xffff                // cmd占用低16bit
#define CN_OBJ_CMD_OPEN       (CN_TARGET_IS_OBJ|0)  // 打开文件。
#define CN_OBJ_CMD_READ       (CN_TARGET_IS_HDL|1)  // 读文件。
#define CN_OBJ_CMD_WRITE      (CN_TARGET_IS_HDL|2)  // 写文件。
#define CN_OBJ_CMD_CLOSE      (CN_TARGET_IS_HDL|3)  // 关闭文件
#define CN_OBJ_CMD_SHOW       (CN_TARGET_IS_OBJ|4)  // 显示文件（目录），不包括磁盘文件系统
#define CN_OBJ_CMD_SEEK       (CN_TARGET_IS_HDL|5)  // 重定位文件
#define CN_OBJ_CMD_TELL       (CN_TARGET_IS_HDL|6)  // 取文件指针位置
#define CN_OBJ_CMD_DELETE     (CN_TARGET_IS_OBJ|7)  // 删除文件。
#define CN_OBJ_CMD_STAT       (CN_TARGET_IS_OBJ|8)  // 取文件状态
#define CN_OBJ_CMD_TRUNCATE   (CN_TARGET_IS_HDL|9)  // 切断文件
#define CN_OBJ_CMD_READDIR    (CN_TARGET_IS_OBJ|10) // 读目录
#define CN_OBJ_CMD_SYNC       (CN_TARGET_IS_HDL|11) // 同步文件
#define CN_OBJ_FCNTL          (CN_TARGET_IS_HDL|12) // fcntl 命令组命令
#define CN_OBJ_IOCTL          (CN_TARGET_IS_HDL|13) // ioctl 命令组命令
#define CN_OBJ_RENAME         (CN_TARGET_IS_OBJ|14) // 更改文件名称或位置

#define CN_OBJ_CMD_USER       (CN_TARGET_IS_HDL|0x1000) // 用户自定义命令起始值，由用户写的驱动程序解释，不同模块可以定义重复的值


//权限管理在stat.h的 S_IRUSR 等已经定义了。
//union __rights{
//    struct __bit{
//        u32 r:1; // 可读；
//        u32 w:1; // 可写；
//        u32 x:1; // 可执行；
//        u32 res:12; // 保留；
//        u32 muses:16
//    }b;
//    u32 a;
//};
//
#define CN_OBJ_TEMPORARY    0x80000000
struct __ObjBitFlag
{
    u32 inuse:24;           // 对象被引用计数
    u32 temporary:1;        //是否临时对象
};
struct Object
{
    char *name;             // 对象名；当用于文件系统为文件名或目录名，用于设备是
                            //设备名，用于gui则是窗口名；
    ptu32_t ObjPrivate;     // 对象私有数据；可以是一个数，也可以指向描述对象的结构；
    fnObjOps ObjOps;        // 对象方法；即对象的操作；
    struct __ObjBitFlag BitFlag;    //对象属性位标志
    list_t handles;         // 对象句柄链表；对象被打开，系统会给用户分配一个句柄；
                            // 句柄的数据结构接入此链表；
    struct Object *prev,*next, *parent, *child; // 对象关系；构建对象树；
};

struct objhandle;
ptu32_t OBJ_GetPrivate(struct Object *ob);
void OBJ_SetPrivate(struct Object *ob, ptu32_t Private);
fnObjOps OBJ_GetOps(struct Object *ob);
s32 OBJ_SetOps(struct Object *ob, fnObjOps ops);
char *OBJ_GetName(struct Object *ob);
char *obj_rename(struct Object *ob,char *NewName);
struct Object *OBJ_GetParent(struct Object *ob);
struct Object *OBJ_GetChild(struct Object *ob);
struct Object *OBJ_GetPrev(struct Object *ob);
struct Object *OBJ_GetNext(struct Object *ob);
struct Object *OBJ_GetHead(struct Object *ob);
s32 OBJ_GetLevel(struct Object *ob);
s32 OBJ_IsHead(struct Object *ob);
s32 OBJ_IsLast(struct Object *ob);
s32 OBJ_IsOnDuty(struct Object *ob);
struct objhandle* obj_TraveFile(struct objhandle *Current, struct Object *Object);
s32 OBJ_LinkHandle(struct objhandle *hdl, struct Object *ob);
bool_t OBJ_Lock(void);
void OBJ_Unlock(void);

//u32 obj_InuseUp(struct Object *ob);
//void obj_InuseUpRange(struct Object *start, struct Object *end);
void OBJ_DutyUp(struct Object *Obj);
//u32 obj_InuseDown(struct Object *ob);
//void obj_InuseDownRange(struct Object *start, struct Object *end);
void OBJ_DutyDown(struct Object *Obj);

s32 OBJ_Delete(struct Object *ob);
struct Object *OBJ_Detach(struct Object *branch);
s32 OBJ_CheckName(const char *name);
struct Object *OBJ_MatchPath(const char *match, char **left);
struct Object *OBJ_BuildTempPath(struct Object *begin, fnObjOps ops,
                            ptu32_t ObjPrivate, char *path);
s32 OBJ_ReleaseTempPath(struct Object *start);
struct Object *OBJ_GetCurrent(void);
void OBJ_SetCurrent(struct Object *ob);
struct Object *OBJ_GetRoot(void);
struct Object *OBJ_NewPrev(struct Object *loc, fnObjOps ops,
                        ptu32_t ObjPrivate, const char *name);
struct Object *OBJ_NewNext(struct Object *loc, fnObjOps ops,
                        ptu32_t ObjPrivate, const char *name);
struct Object *OBJ_NewChild(struct Object *parent, fnObjOps ops,
                         ptu32_t ObjPrivate, const char *name);
struct Object *OBJ_NewHead(struct Object *loc,fnObjOps ops,
                           ptu32_t ObjPrivate, const char *name);
s32 obj_MoveToLast(struct Object *ob);
s32 OBJ_MoveToHead(struct Object *ob);
struct Object *OBJ_InsertToChild(struct Object *loc, struct Object *child);
s32 OBJ_InsertToNext(struct Object *loc, struct Object *next);
s32 OBJ_InsertToPrev(struct Object *loc, struct Object *prev);
s32 OBJ_ChildMoveToOrev(struct Object *parent);
s32 OBJ_ChildMoveToNext(struct Object *parent);
struct Object *OBJ_GetTwig(struct Object *ob);
struct Object *OBJ_ForeachChild(struct Object *parent, struct Object *child);
struct Object * OBJ_ForeachScion(struct Object *ancester, struct Object *scion);
struct Object *OBJ_SearchSibling(struct Object *ob, const char *name);
struct Object *OBJ_SearchChild(struct Object *parent, const char *name);
struct Object *OBJ_SearchScion(struct Object *ancester, const char *name);
struct Object *OBJ_SearchPath(struct Object *start, const char *path);
s32 OBJ_SetMultiplexEvent(struct Object *ob, u32 events);
s32 OBJ_ClrMultiplexEvent(struct Object *ob, u32 events);
s32 issocketactive(s32 Fd, s32 mode);
struct objhandle* OBJ_ForeachHandle(struct objhandle *Current, struct Object *Object);
s32 OBJ_CurWorkPathLen(void);
s32 OBJ_CurWorkPath(char *Buf, u32 BufSize);
s32 OBJ_SetPwd(const char *Path);

#ifdef __cplusplus
}
#endif

#endif //__OBJECT_H__

