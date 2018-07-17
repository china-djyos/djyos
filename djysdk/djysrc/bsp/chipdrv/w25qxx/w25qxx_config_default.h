//用户注释不可以 “InitCode”或“Configure”开头
//InitCode 创建工程时，如下代码将被IDE copy到初始化函数中，不需要配置的，可以空着。——本行不可删改
#include "w25qxx_config.h"
extern s32 ModuleInstall_NOR(const char *DevName, u32 Flags, u8 StartBlk);
    if( ModuleInstall_NOR(CFG_W25QXX_BUS_NAME,CFG_W25QXX_FLAG,CFG_W25QXX_START_BLK) == false)
    {
        printf("W25QXX Install Error!\r\n");
        while( 1 );             //初始化时出错，死循环即可
    }

//Configure 如下配置常量，将在IDE中图形化显示，允许用户图形化配置。——本行不可删改
#define CFG_W25QXX_BUS_NAME              "NOR_W25QXX"
#define CFG_W25QXX_FLAG                   (2)
#define CFG_W25QXX_START_BLK              0
