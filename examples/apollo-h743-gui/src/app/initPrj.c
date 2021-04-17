/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#include "project_config.h"
#include "djyos.h"
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include <Iboot_info.h>


extern ptu32_t djy_main(void);
//如果这个CN_MANUFACTURER_NAME名字要改，那djysrc里的Iboot_info.c中相应的名字也要改
const char CN_MANUFACTURER_NAME[] = PRODUCT_MANUFACTURER_NAME;


const struct ProductInfo Djy_Product_Info __attribute__ ((section(".DjyProductInfo"))) =
{
    .VersionNumber[0] = PRODUCT_VERSION_LARGE,
    .VersionNumber[1] = PRODUCT_VERSION_MEDIUM,
    .VersionNumber[2] = PRODUCT_VERSION_SMALL,
#if(CN_PTR_BITS < 64)
    .ManufacturerNameAddr      = (u32)(&CN_MANUFACTURER_NAME),
    .ManufacturerNamereserved32    = 0xffffffff,
#else
    .ManufacturerNameAddr      = (u64)(&CN_MANUFACTURER_NAME),
#endif
    .ProductClassify = PRODUCT_PRODUCT_CLASSIFY,
    .ProductType = PRODUCT_PRODUCT_MODEL,

    .TypeCode = PRODUCT_PRODUCT_MODEL_CODE,

    .ProductionTime = {0xff,0xff,0xff,0xff},
    .ProductionNumber = {0xff,0xff,0xff,0xff,0xff},
    .reserved8 = 0,
    .BoardType = PRODUCT_BOARD_TYPE,
    .CPU_Type = PRODUCT_CPU_TYPE,
    .Reserved ={
                0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
            },
};


ptu32_t __djy_main(void)
{
    djy_main();

    return 0;
}

void Sys_ModuleInit(void)
{
    uint16_t evtt_main;

    //board config
    extern void Board_Init(void);
    Board_Init();
    //end board config

    //----------------------------early----------------------------//
    //black box
    extern void ModuleInstall_BlackBox(void);
    ModuleInstall_BlackBox( );
    //end black box

    //message queue
    extern bool_t ModuleInstall_MsgQ(void);
    ModuleInstall_MsgQ ( );
    //end message queue

    //multiplex
    extern bool_t ModuleInstall_Multiplex(void);
    ModuleInstall_Multiplex ();
    //end multiplex

    //djybus
    extern bool_t ModuleInstall_DjyBus(void);
    ModuleInstall_DjyBus ( );
    //end djybus

    //can bus
    extern bool_t ModuleInstall_CANBus(void);
    ModuleInstall_CANBus ( );
    //end can bus

    //iicbus
    extern bool_t ModuleInstall_IICBus(void);
    ModuleInstall_IICBus ( );
    //end iicbus

    //----------------------------medium----------------------------//
    //kernel
    #if(CFG_OS_TINY == flase)
    extern s32 kernel_command(void);
    kernel_command();
    #endif
    //end kernel

    //graphical kernel
    extern bool_t ModuleInstall_GK(void);
    ModuleInstall_GK();
    //end graphical kernel

    evtt_main = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
        __djy_main,NULL,CFG_MAINSTACK_LIMIT, "main function");
    //事件的两个参数暂设为0,如果用shell启动,可用来采集shell命令行参数
    DJY_EventPop(evtt_main,NULL,0,0,0,0);

    //heap
    #if ((CFG_DYNAMIC_MEM == true))
    extern bool_t Heap_DynamicModuleInit(void);
    Heap_DynamicModuleInit ( );
    #endif
    //end heap



    return ;
}
