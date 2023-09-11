#include <user_mb_app.h>
#include <user_mb_app_m.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

// =============================================================================
// 函数功能:modbus模块安装
// 输入参数:无
// 返回值 : true -- 成功；false -- 失败
// =============================================================================
bool_t ModuleInstall_Modbus(void)
{
    bool_t result = false;

#if CFG_MB_SLAVE_ENABLED == TRUE

    result = ModuleInstall_ModbusSlave(CFG_MB_S_ADDR, CFG_MB_S_TRANSFER_MODE, CFG_MB_S_PORT_NUM, 
                                        CFG_MB_S_PORT_BAUDRATE, CFG_MB_S_PORT_PARITY, CFG_MB_S_TCP_PORT_NUM);

#endif

#if CFG_MB_MASTER_ENABLED == TRUE

    result = ModuleInstall_ModbusMaster(CFG_MB_M_PORT_NUM, CFG_MB_M_PORT_BAUDRATE, CFG_MB_M_PORT_PARITY);

#endif

return result;
}
