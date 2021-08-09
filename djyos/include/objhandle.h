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

#ifndef __OBJFILE_H__
#define __OBJFILE_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <include/list.h>
#include <fcntl.h>
#include <unistd.h>
#include <stat.h>
#include "object.h"

// 命名为对象句柄：
// 对象系统中，句柄是read/write对象的接口，一个object允许多个用户使用多个句柄
// read/write。一个文件在系统中，只会占用一个object结构，object不能直接被访问。
// 访问文件必须使用struct objhandle。文件可以被多次打开，根据文件系统的具体实
// 现，可能每次打开都分配一个struct objhandle结构，也可能共用一个 struct objhandle结构。
// fd与struct objhandle一一对应。
// 注意，一个打开的文件，其路径上的目录，即使没有打开，也会分配一个object，但不会
// 分配struct objhandle结构。

struct objhandle{
    list_t list;            // 句柄所关联的对象的句柄链表；
    u32 flags;              // 当前文件操作标志，如 O_RDONLY等，在fcntl.h中定义。
    struct Object *HostObj;    // 关联的对象
    u32 timeout;            // 同步访问的timeout，单位us，不超过1.19小时；
                            //注意，socket不适用，posix有特殊规定。
    struct MultiplexObjectCB * pMultiplexHead; // 多路复用目标对象头指针；
    u32 MultiplexEvents;    // 对象的当前访问状态，如可读，可写等。24bit，高8位无效，
                            // 可用于多路复用；
    ptu32_t UserTag;        // 用户标签，由用户设定该标签用途；
    ptu32_t context;        // handle访问上下文
};

#define CN_INVALID_FD  -1

s32 Handle2fd(struct objhandle *hdl);
struct objhandle *fd2Handle(s32 fd);
s32 Handle_FlagIsDirectory(u32 flags);
s32 Handle_FlagIsRegular(u32 flags);
s32 Handle_FlagIsCreate(u32 flags);
s32 Handle_FlagIsOnlyCreate(u32 flags);
s32 Handle_FlagIsAppend(u32 flags);
s32 Handle_FlagIsTrunc(u32 flags);
s32 Handle_FlagIsReadable(u32 flags);
s32 Handle_FlagIsWriteable(u32 flags);
s32 Handle_FlagIsBlockComplete(u32 flags);
s32 Handle_IsAppend(struct objhandle *hdl);
s32 Handle_IsReadable(struct objhandle *hdl);
s32 Handle_IsWritable(struct objhandle *hdl);
s32 iscontender(struct objhandle *hdl);
struct objhandle *Handle_New(void);
s32 Handle_Delete(struct objhandle *hdl);
void Handle_Init(struct objhandle *hdl, struct Object *ob, u32 flags, ptu32_t context);
const char *Handle_GetName(struct objhandle *hdl);
ptu32_t Handle_GetContext(struct objhandle *hdl);
void Handle_SetContext(struct objhandle *hdl, ptu32_t context);
ptu32_t handle_GetHostObjectPrivate(struct objhandle *hdl);
struct MultiplexObjectCB *__Handle_GetMultiplexHead(struct objhandle *hdl);
void __Handle_SetMultiplexHead(struct objhandle *hdl, struct MultiplexObjectCB *cb);
u32 handle_gettimeout(struct objhandle *hdl);
void Handle_SetTimeOut(struct objhandle *hdl, u32 timeout);
u32 Handle_GetMode(struct objhandle *hdl);
void Handle_SetMode(struct objhandle *hdl, u32 mode);
struct Object *Handle_GetHostObj(struct objhandle *hdl);
u32 Handle_MultiEvents(struct objhandle *hdl);
void Handle_SetMultiplexEvent(struct objhandle *hdl, u32 events);
void Handle_ClrMultiplexEvent(struct objhandle *hdl, u32 events);
#ifdef __cplusplus
}
#endif

#endif/* __OBJFILE_H__ */
