/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。
#include "djyos.h"
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
extern ptu32_t djy_main(void);

ptu32_t __djy_main(void)
{
    djy_main();
    return 0;
}

void Sys_ModuleInit(void)
{
    uint16_t evtt_main;

    extern void Board_GpioInit(void);
    Board_GpioInit();

    //-------------------early-------------------------//
    extern void ModuleInstall_BlackBox(void);
    ModuleInstall_BlackBox( );

    extern bool_t ModuleInstall_DjyBus(void);
    ModuleInstall_DjyBus ( );

    extern bool_t ModuleInstall_CANBus(void);
    ModuleInstall_CANBus ( );

    extern bool_t ModuleInstall_MsgQ(void);
    ModuleInstall_MsgQ ( );

    extern bool_t ModuleInstall_IICBus(void);
    ModuleInstall_IICBus ( );

    extern bool_t ModuleInstall_Multiplex(void);
    ModuleInstall_Multiplex ();

    #if !defined (CFG_RUNMODE_BAREAPP)
    extern ptu32_t ModuleInstall_IAP(void);
    ModuleInstall_IAP( );
    #endif

    //-------------------medium-------------------------//
    extern bool_t ModuleInstall_GK(void);
    ModuleInstall_GK();

    #if(CFG_OS_TINY == flase)
    extern s32 kernel_command(void);
    kernel_command();
    #endif

    //-------------------later-------------------------//
    evtt_main = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
    __djy_main,NULL,CFG_MAINSTACK_LIMIT, "main function");
    //事件的两个参数暂设为0,如果用shell启动,可用来采集shell命令行参数
    Djy_EventPop(evtt_main,NULL,0,NULL,0,0);

    #if ((CFG_DYNAMIC_MEM == true))
    extern bool_t Heap_DynamicModuleInit(void);
    Heap_DynamicModuleInit ( );
    #endif

    return ;
}

