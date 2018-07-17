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

#ifndef __OBJFILE_H__
#define __OBJFILE_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "object.h"
#include <list.h>
#include <fcntl.h>
#include <unistd.h>
#include <stat.h>

// 命名为端口，灵感来自于网络，网络中，同一个服务器用不同端口为不同用户提供服务。
// 对象系统中，端口是read/write对象的接口，一个Object允许多个用户使用多个端口
// read/write。一个文件在系统中，只会占用一个object结构，Object不能直接被访问。
// 访问文件必须使用struct ObjectPort 。文件可以被多次打开，根据文件系统的具体实
// 现，可能每次打开都分配一个ObjectPort结构，也可能共用一个 ObjectPort 结构。
// fd与ObjectHandle一一对应。object的VFileHead成员指向该文件的ObjectPort。
// 注意，一个打开的文件，其路径上的目录，即使没有打开，也会分配一个object，但不会
// 分配struct ObjectPort 结构。
struct __portBasic{
    list_t list; // 同一内核对象关联的文件控制块列表；
    u32 flags; // 访问模式，在fcntl.h中定义；
#define O_FILE_BASIC           0x80000000
};

struct __port{
    struct dListNode list; // 即__portBasic内容
    u32 flags; // 即__portBasic内容
    struct Object *obj; // 对象
    u32 timeout; // 同步访问的timeout，单位us，不超过1.19小时；
    struct MultiplexObjectCB * mphead; // 多路复用目标对象头指针；
    u32 access; // 对象的当前访问状态，如可读，可写等。24bit，高8位无效，可用于多路复用；
    ptu32_t tag; // 用户标签，由用户设定该标签用途；
    ptu32_t context; // 私有上下文；
};

typedef struct __port tagOFile;

s32                       testcontent(u32);
s32                       testdirectory(u32);
s32                       testregular(u32);
s32                       testcreat(u32);
s32                       testonlycreat(u32);
s32                       testappend(u32 );
s32                       testtrunc(u32);
s32                       testreadable(u32);
s32                       testwriteable(u32);
s32                       isdirectory(tagOFile*);
s32                       isreadable(tagOFile*);
s32                       isappend(tagOFile *);
s32                       iswritable(tagOFile*);
s32                       isondutyo(struct Object*);
void                      setmevento(struct Object*, u32);
void                      clrmevento(struct Object*, u32);
void                      of_setmevent(tagOFile*, u32);
void                      of_clrmevent(tagOFile*, u32);
u32                       of_access(tagOFile*);
s32                       of_setaccess(tagOFile*, u32);
struct MultiplexObjectCB *of_mulitplex(tagOFile*);
void                      of_setmulitplex(tagOFile*, struct MultiplexObjectCB*);
ptu32_t                   of_context(tagOFile*);
void                      of_setcontext(tagOFile*, ptu32_t);
ptu32_t                   of_represent(tagOFile*);
void                      of_setrepresent(tagOFile*, ptu32_t);
u32                       of_timeout(tagOFile*);
void                      of_settimeout(tagOFile*, u32);
u32                       of_mode(tagOFile*);
void                      of_setmode(tagOFile*, u32);
tagOFile                 *of_new(void);
s32                       of_free(tagOFile*);
struct Object            *of_o(tagOFile*);
s32                       ofno(tagOFile*);
tagOFile                 *of(s32 dwFD);
s32                       of_linko(tagOFile*, struct Object*);
void                      of_init(tagOFile*, struct Object*, u32, ptu32_t);
void                     *of_core(tagOFile*);
struct Object            *of_o(tagOFile*);
const char               *of_name(tagOFile*);
s32                       openo(struct Object*);
struct Object            *basic2obj(struct __portBasic*);
struct __portBasic       *of_basic(tagOFile*);
void                      of_initbasic(struct __portBasic*, u32);
void                      of_unlinkbasic(struct __portBasic*);
struct __portBasic       *of_basiclinko(struct Object*);
s32                       of_virtualize(struct Object*, struct __portBasic*, char*);
struct Object            *of_virtualizeo(struct Object*, struct __portBasic*, char*);

#ifdef __cplusplus
}
#endif

#endif/* __OBJFILE_H__ */
