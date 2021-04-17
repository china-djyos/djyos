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

    //shell
    extern void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut);
    Stdio_KnlInOutInit(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME);
    extern s32 ModuleInstall_Shell(ptu32_t para);
    ModuleInstall_Shell(0);
    //end shell

    //----------------------------early----------------------------//
    //black box
    extern void ModuleInstall_BlackBox(void);
    ModuleInstall_BlackBox( );
    //end black box

    //device file system
    extern s32 ModuleInstall_dev(void);
    ModuleInstall_dev();    // 安装设备文件系统；
    //end device file system

    //message queue
    extern bool_t ModuleInstall_MsgQ(void);
    ModuleInstall_MsgQ ( );
    //end message queue

    //multiplex
    extern bool_t ModuleInstall_Multiplex(void);
    ModuleInstall_Multiplex ();
    //end multiplex

    //watch dog
    extern bool_t ModuleInstall_Wdt(void);
    ModuleInstall_Wdt();
    //end watch dog

    //cpu onchip timer
    extern bool_t ModuleInstall_HardTimer(void);
    ModuleInstall_HardTimer();
    //end cpu onchip timer

    //cpu onchip systime
    extern bool_t MoudleInit_Systime(ptu32_t para);
    MoudleInit_Systime(0);
    //end cpu onchip systime

    //----------------------------medium----------------------------//
    //kernel
    #if(CFG_OS_TINY == flase)
    extern s32 kernel_command(void);
    kernel_command();
    #endif
    //end kernel

    //Software Timers
    extern bool_t ModuleInstall_Timer(void);
    ModuleInstall_Timer();
    //end Software Timers

    //cpu onchip uart
    extern ptu32_t ModuleInstall_UART(ptu32_t SerialNo);
    #if CFG_UART1_ENABLE ==1
    ModuleInstall_UART(CN_UART1);
    #endif
    #if CFG_UART2_ENABLE ==1
    ModuleInstall_UART(CN_UART2);
    #endif
    #if CFG_UART3_ENABLE ==1
    ModuleInstall_UART(CN_UART3);
    #endif
    #if CFG_UART4_ENABLE ==1
    ModuleInstall_UART(CN_UART4);
    #endif
    #if CFG_UART5_ENABLE ==1
    ModuleInstall_UART(CN_UART5);
    #endif
    #if CFG_UART6_ENABLE ==1
    ModuleInstall_UART(CN_UART6);
    #endif
    #if CFG_UART7_ENABLE ==1
    ModuleInstall_UART(CN_UART7);
    #endif
    #if CFG_UART8_ENABLE ==1
    ModuleInstall_UART(CN_UART8);
    #endif
    //end cpu onchip uart

    //----------------------------later----------------------------//
    //stdio
    #if(CFG_STDIO_STDIOFILE == true)
    extern s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err);
    ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);
    #endif
    //end stdio

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
