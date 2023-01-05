#include<stdio.h>
#include"dbug.h"
#include"shell.h"
//DJYOS操作系统默认调试打印等级

#ifdef DEBUG
u32 debuglevel=CN_DEBUG;
#else
u32 debuglevel=CN_ERR;
#endif


// ============================================================================
// 功能：设置设备的打印等级
// 参数：param，字符0 ~ 7，对应设备的打印等级
// 返回：true -- 成功； false -- 失败
// 备注：
// ============================================================================
bool_t DbugSetPrintfLevel(u32 Level)
{
    if (Level <= CN_DEBUG)
    {
        debuglevel = Level;
        printf("Current device print level = %d\r\n", debuglevel);
        return true;
    }
    else
    {
        printf("device print level set fail, Current level = %d\r\n", debuglevel);
    }
    return false;
}

// ============================================================================
// 功能：shell设置设备的打印等级
// 参数：param，字符0 ~ 7，对应设备的打印等级
// 返回：true -- 成功； false -- 失败
// 备注：shell专用
// ============================================================================
bool_t ShellDbugSetPrintfLevel(char *param)
{
    u32 Level;
    Level = (u32)atoi(param);

    if (Level <= CN_DEBUG)
    {
        debuglevel = Level;
        printf("Current device print level = %d\r\n", debuglevel);
    }
    else
    {
        printf("device print level set fail, Current level = %d\r\n", debuglevel);
    }
    return true;
}

// ============================================================================
// 功能：获取设备的打印等级
// 参数：无
// 返回：当前的打印等级
// 备注：
// ============================================================================
u32 DbugGetPrintfLevel(void)
{
    return debuglevel;
}

ADD_TO_ROUTINE_SHELL(DebugLevel, ShellDbugSetPrintfLevel, "修改设备的打印等级，范围(0 ~ 7)");
