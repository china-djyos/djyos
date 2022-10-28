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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <djyos.h>
#include <shell.h>
#include "component_config_isbus.h"

bool_t ISBUS_SlaveInit(u32 StackSize);
void ISBUS_HostInit(u32 HostStackSize);
bool_t debug_ctrl = true;
u32 dbg_stopack = 0;
u32 testresend = 0;
// ============================================================================
// 函数功能：安装内部串口通信模块。该模块仅适用于从机部分。
// 输入参数：StackSize，模块需要的内存尺寸，由于串行通信协议解析后，要调用用户
//              提供的回调函数处理数据，本模块自身需要的内存很少，最多不过数百
//              字节，请用户根据自己的回调函数决定。
//       CN_PRIO_RLYMAIN，事件优先级，由用户自己选取，一般选择较高优先级
// 返回值： 总是成功，不成功就进入死循环。
// ============================================================================
bool_t ModuleInstall_ISBUS(u32 HostStackSize,u32 SlaveStackSize)
{
#if(CFG_HOST_ENABLE == true)
    ISBUS_HostInit(HostStackSize);
#endif
#if(CFG_SLAVE_ENABLE == true)
    ISBUS_SlaveInit(SlaveStackSize);
#endif
    return true;
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

bool_t ndbgisbus(char *param)
{
    if(debug_ctrl == false)
        debug_ctrl = true;
    else
        debug_ctrl = false;
    return true;
}
#pragma GCC diagnostic pop

bool_t stopack(char *param)
{
    u32 times;
    times = atoi(param);
    dbg_stopack = times;
    return true;
}
bool_t resend(char *param)
{
    testresend = atoi(param);
    return true;
}

ADD_TO_ROUTINE_SHELL(ndbgisbus,ndbgisbus,"");
ADD_TO_ROUTINE_SHELL(stopack,stopack,"");
ADD_TO_ROUTINE_SHELL(resend,resend,"");

