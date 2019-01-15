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
//author:zhangqf
//date  :上午11:25:43/2017年5月3日
//usage :
//-----------------------------------------------------------------------------

#ifndef __IOCTL_H__
#define __IOCTL_H__

//ioctl 函数的命令号，此处暂且只定义了一个。
//最新的POSIX已经废弃了ioctl函数，而然并卵，大量开源项目仍然在用。
#define I_ATMARK    0      //Is the top message "marked''?
#define I_CANPUT    1      //Is a band writable?
#define I_CKBAND    2      //See if any messages exist in a band.
#define I_FDINSERT  3      //Send implementation-defined information about another STREAM.
#define I_FIND      4      //Look for a STREAMS module.
#define I_FLUSH     5      //Flush a STREAM.
#define I_FLUSHBAND 6      //Flush one band of a STREAM.
#define I_GETBAND   7      //Get the band of the top message on a STREAM.
#define I_GETCLTIME 8      //Get close time delay.
#define I_GETSIG    9      //Retrieve current notification signals.
#define I_GRDOPT    10     //Get the read mode.
#define I_GWROPT    11     //Get the write mode.
#define I_LINK      12     //Connect two STREAMs.
#define I_LIST      13     //Get all the module names on a STREAM.
#define I_LOOK      14     //Get the top module name.
#define I_NREAD     15     //Size the top message.
#define I_PEEK      16     //Peek at the top message on a STREAM.
#define I_PLINK     17     //Persistently connect two STREAMs.
#define I_POP       18     //Pop a STREAMS module.
#define I_PUNLINK   19     //Dismantle a persistent STREAMS link.
#define I_PUSH      20     //Push a STREAMS module.
#define I_RECVFD    21     //Get a file descriptor sent via I_SENDFD.
#define I_SENDFD    22     //Pass a file descriptor through a STREAMS pipe.
#define I_SETCLTIME 23     //Set close time delay.
#define I_SETSIG    24     //Ask for notification signals.
#define I_SRDOPT    25     //Set the read mode.
#define I_STR       26     //Send a STREAMS ioctl().
#define I_SWROPT    27     //Set the write mode.
#define I_UNLINK    28     //Disconnect two STREAMs.
//TODO：待补充，参照stropts.h.html，把POSIX中规定的用于上述命令的常数定义完整。

s32 ioctl(s32 fildes, s32 request, ...);

#endif /* __IOCTL_H__ */
