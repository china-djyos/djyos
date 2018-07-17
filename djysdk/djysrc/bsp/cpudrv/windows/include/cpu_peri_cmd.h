//----------------------------------------------------
//Copyright (C), 2004-2011,  罗侍田.
//版权所有 (C), 2004-2011,   罗侍田.
//所属模块:时钟模块
//作者:  罗侍田
//版本：V1.0.0
//文件描述: 命令行输入输出
//其他说明:
//修订历史:
// 1. 日期: 2012-10-09
//   作者:  罗侍田
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __CPU_PERI_CMD_H__
#define __CPU_PERI_CMD_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <windows.h>

u32 cmd_int(ufast_t uart_int_line);
u32 WINAPI win32_scanf_pro( LPVOID lpParameter );
ptu32_t ModuleInstall_Cmd(ptu32_t para);

#ifdef __cplusplus
}
#endif

#endif /*__CPU_PERI_CMD_H__*/

