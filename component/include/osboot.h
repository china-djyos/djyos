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
// Copyright (c) 2018 著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
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
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

//该定义用于检查堆栈使用的内存是否是合法地址
typedef struct
{
    const u8   *start;
    const u32   len;
}tagVmMemItem;
//该定义用于定义了我们的启动模式
typedef enum
{
    EN_BOOT_POWERDOWN = 0,//WHICH MEANS BOOT FROM POWER DOWN
    EN_BOOT_HRST,         //WHICH MEANS BOOT FROM EXTERNAL HARD RESET BUT POWERON
    EN_BOOT_SRST,         //WHICH MEANS BOOT FROM INTERNAL SOFT RESET
    EN_BOOT_REBOOT,       //WHICH MEANS BOOT FROM SOFT REBOOT
    EN_BOOT_RELOAD,       //WHICH MEANS BOOT FROM RELOAD
    EN_BOOT_UNKNOWN,      //WHICH MEANS BOOT FROM UNKNOWN REASON
    EN_BOOT_NOTIMPLEMENT, //WHICH MEANS BOOT BSP NOT IMPLEMENT
    EN_BOOT_LAST,
}enBootMode;

#define CN_BOOT_LEGALKEY   0XAA55AA55  //调用reboot、reset、reload传输该key，不会记录，否则会作为异常记录
typedef enBootMode  (*fnGetBootMode)(void); //用于获取我们的CPU启动模式的函数原型
//安装系统启动记录存储模块：tab是我们的栈可能使用的内存范围，可以是多个，NULL结束,不可更改；GetOsBootModeHard是软件启动标志，需要BSP提供；GetOsBootModeSoft是
//软件启动标志，需要BSP提供
bool_t ModuleInstall_OsBoot(const tagVmMemItem *tab[],fnGetBootMode GetOsBootModeHard,fnGetBootMode GetOsBootModeSoft);
//抛出异常重启信息，当调用系统的reboot、reset、reload等提供非法的KEY的时候
//合法的KEY是0xaa55aa55,其他的均会认为是非法的，这部分需要cpu的bsp 的port的支持
bool_t ThrowOsBootInfo(enBootMode mode);





