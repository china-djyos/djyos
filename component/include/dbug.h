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

#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>


//
// @brief  调试输出等级开关
//
#define CN_EMERG        0 // system is unusable
#define CN_ALERT        1 // action must be taken immediately
#define CN_CRIT         2 // critical conditions
#define CN_ERR          3 // error conditions
#define CN_WARNING      4 // warning conditions
#define CN_NOTICE       5 // normal but significant condition
#define CN_INFO         6 // informational
#define CN_DEBUG        7 // debug-level messages

extern u32 debuglevel;

/*调试级别的消息*/
#define debug_printf(module,fmt, ...) do{\
if (debuglevel >= (CN_DEBUG)) \
{\
    printf("\r\ndbug : %-6.6s : "fmt"", module, ##__VA_ARGS__); \
}\
}while (0)

/*提示信息，如驱动程序启动时，打印硬件信息*/
#define info_printf(module, fmt, ...) do{\
if (debuglevel >= CN_INFO) \
{\
    printf("\r\ninfo : %-6.6s : "fmt"", module, ##__VA_ARGS__); \
} \
}while (0)

/*正常但又重要的条件，用于提醒。常用于与安全相关的消息*/
#define notice_printf(module,fmt, ...) \
do{ if (debuglevel >= (CN_NOTICE)) printf("\r\n: note : %-6.6s : "fmt"", module, ##__VA_ARGS__); } while (0)

/*警告条件，对可能出现问题的情况进行警告*/

#define warning_printf(module,fmt, ...) do { \
if (debuglevel >= (CN_WARNING)) \
{\
    printf("\r\nwarn : %-6.6s : "fmt"", module, ##__VA_ARGS__); \
    printf("\r\n       FILE: %s", __FILE__); \
    printf("\r\n       LINE: %d\r\n", __LINE__); \
}\
} while (0)


/*错误条件，驱动程序常用KERN_ERR来报告硬件的错误*/
#define error_printf(module,fmt, ...) do { \
if (debuglevel >= (CN_ERR)) \
{\
    printf("\r\nerro : %-6.6s : "fmt"", module, ##__VA_ARGS__); \
    printf("\r\n       FILE: %s", __FILE__); \
    printf("\r\n       LINE: %d\r\n", __LINE__); \
}\
} while (0)

/*临界条件，通常涉及严重的硬件或软件操作失败*/
#define critical_printf(module,fmt, ...) do { \
if (debuglevel >= (CN_CRIT)) \
{\
    printf("\r\ncrit : %-6.6s : "fmt"", module, ##__VA_ARGS__); \
    printf("\r\n       FILE: %s", __FILE__); \
    printf("\r\n       LINE: %d\r\n", __LINE__); \
}\
} while (0)

/*报告消息，表示必须立即采取措施*/
#define alert_printf(module,fmt, ...) do { \
if (debuglevel >= (CN_ALERT)) \
{\
    printf("\r\nalet : %-6.6s : "fmt"", module, ##__VA_ARGS__); \
    printf("\r\n       FILE: %s", __FILE__); \
    printf("\r\n       LINE: %d\r\n", __LINE__); \
}\
} while (0)

/*紧急事件消息，系统崩溃之前提示，表示系统不可用*/
#define emergency_printf(module,fmt, ...) do { \
if (debuglevel >= (CN_EMERG)) \
{\
    printf("\r\nemer : %-6.6s : "fmt"", module, ##__VA_ARGS__); \
    printf("\r\n       FILE: %s", __FILE__); \
    printf("\r\n       LINE: %d\r\n", __LINE__); \
}\
} while (0)





#ifdef __cplusplus
}
#endif

#endif // ifndef __DEBUG_H__
