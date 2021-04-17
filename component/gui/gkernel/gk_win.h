//----------------------------------------------------
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

//所属模块: gui
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: gui kernel主体部分
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-11-21
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __GK_WIN_H__
#define __GK_WIN_H__
#include <errno.h>
#include <ring.h>
#include "gk_syscall.h"
#ifdef __cplusplus
extern "C" {
#endif

struct DisplayObj;

struct GkChunnel       //用户程序(通常是gui windows) 和 gui kernel的通信管道
{
    struct RingBuf ring_syscall;         //gui 系统调用环形缓冲区
//  u8 *syscall_buf;                        //为系统调用环形缓冲区分配内存

    struct MutexLCB *syscall_mutex;      //互斥量,用于防止并发写
//  struct SemaphoreLCB *syscall_semp;   //信号量，写时如果缓冲区满则等候。

    struct SemaphoreLCB *usercall_semp;  //信号量，写时如果缓冲区满则等候。
    struct MsgQueue *usercall_msgq;      //usercall消息队列

};

struct GkWinObj *__GK_GetZsectionStart(struct GkWinObj *gkwin);
struct GkWinObj *__GK_GetZsectionEnd(struct GkWinObj *gkwin);
struct GkWinObj *__GK_CreateDesktop(struct GkscParaCreateDesktop *para);
struct GkWinObj *__GK_CreateWin(struct GkscParaCreateGkwin *para);
void __GK_AdoptWin(struct GkscParaAdoptWin *para);
void __GK_MoveWin(struct GkscParaMoveWin *para);
void __GK_SetBound(struct GkWinObj *gkwin);
void __GK_SetBoundMode(struct GkscParaSetBoundMode *para);
void __GK_SetPrio(struct GkscParaSetPrio *para);
bool_t __GK_SetRopCode(struct GkscParaSetRopCode *para);
void __GK_SetVisible(struct GkscParaSetVisible *para);
bool_t __GK_SetHyalineColor(struct GkscParaSetHyalineColor *para);
void __GK_SetDirectScreen(struct GkWinObj *gkwin);
void __GK_SetUnDirectScreen(struct GkWinObj *gkwin);
void __GK_DestroyWinChild(struct GkWinObj *gkwin);
void __GK_DestroyWin(struct GkWinObj *gkwin);
bool_t __GK_ChangeWinArea(struct GkscParaChangeWinArea *para);
void __GK_OutputRedraw(struct DisplayObj *display);
u16 __GK_SyscallChunnel(u16 command,u32 sync_time,void *param1,u16 size1,
                                                void *param2,u16 size2);
bool_t  __GK_UsercallChunnel(void *pdata,u16 size,u32 timeout);

ptu32_t __GK_UsercallServer(void);
ptu32_t __GK_Server(void);

#ifdef __cplusplus
}
#endif

#endif //__GK_WIN_H__

