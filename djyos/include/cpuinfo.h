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

#ifndef SRC_DJYOS_INCLUDE_CPUINFO_H_
#define SRC_DJYOS_INCLUDE_CPUINFO_H_

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

//we defines code and the storage for the system
#define MEM_NOCACHED     __attribute__((section(".data.nocached")))   //which used the none cached ram,like the dma
#define CODE_VECTOR      __attribute__((section(".code.vector")))     //exception or interrupt vector
//this storage is loaded before the preload,maybe not loaded,just in the rom storage
#define CODE_BEFOREPRELOAD    __attribute__((section(".code.beforepreload")))   //maybe called at the start up
#define DATA_BEFOREPRELOAD    __attribute__((section(".data.beforepreload")))   //maybe used by the start up
#define BSS_BEFOREPRELOAD     __attribute__((section(".bss.beforepreload")))    //maybe used by the start up
//this storage is loaded before the load,and this storage loaded by the preload
#define CODE_BEFORELOAD    __attribute__((section(".code.beforeload")))         //maybe called by the preload or critical
#define DATA_BEFORELOAD    __attribute__((section(".data.beforeload")))         //maybe used by the preload or critical
#define BSS_BEFORELOAD     __attribute__((section(".bss.beforeload")))          //maybe used by the preload or critical
//this storage is loaded by the Iboot_Loader
#define CODE_NORMAL    __attribute__((section(".code.normal")))                 //loaded by Iboot_Loader
#define DATA_NORMAL    __attribute__((section(".data.normal")))                 //loaded by Iboot_Loader
#define BSS_NORMAL     __attribute__((section(".bss.normal")))                  //loaded by Iboot_Loader


typedef enum
{
    EN_CPU_MSGTYPE_SIZE = 0,  //THIS IS MEAN IT IS THE STORAGE:based bytes
    EN_CPU_MSGTYPE_FREQUENCY, //THIS IS MEAN IT IS THE CLK:based HZ
    EN_CPU_MSGTYPE_STRING,    //THIS IS MEAN IT IS THE FIRMWARE STRING COMMENT
    EN_CPU_MSGTYPE_MULTIBYTES,//THIS IS MEAN IT IS THE BYTES DATA
}enCpuMsgType;

#define CN_CPU_SIGNATURE    "SIGNATURE"   //该名字固定用作CPU的唯一ID签名信息，方便应用生成唯一ID

CODE_BEFOREPRELOAD bool_t Core_LogCpuMsg(const char *msgname, const void *msg,u8 msglen, enCpuMsgType type);
bool_t Core_ShowCpuInfo(char *param);
s32  Core_GetCpuInfo(const char *name,void *buf, u8 len);


#endif /* SRC_DJYOS_INCLUDE_CPUINFO_H_ */
