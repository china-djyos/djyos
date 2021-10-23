/****************************************************
 *  Automatically-generated file. Do not edit!	*
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

    //cpu onchip peripheral lowpower control
    void ModuleInstall_LowPower (void);
    ModuleInstall_LowPower();
    //end cpu onchip peripheral lowpower control

    //device file system
    extern s32 ModuleInstall_dev(void);
    ModuleInstall_dev();    // 安装设备文件系统；
    //end device file system

    //loader
    #if !defined (CFG_RUNMODE_BAREAPP)
    extern bool_t ModuleInstall_UpdateIboot(void);
    ModuleInstall_UpdateIboot( );
    #endif
    //end loader

    //message queue
    extern bool_t ModuleInstall_MsgQ(void);
    ModuleInstall_MsgQ ( );
    //end message queue

    //multiplex
    extern bool_t ModuleInstall_Multiplex(void);
    ModuleInstall_Multiplex ();
    //end multiplex

    //----------------------------medium----------------------------//
    //kernel
    #if(CFG_OS_TINY == flase)
    extern s32 kernel_command(void);
    kernel_command();
    #endif
    //end kernel

    //cpu onchip uart
    ptu32_t ModuleInstall_UART(u32 port);
    #if CFG_USART1_ENABLE ==1
    ModuleInstall_UART(CN_USART1);
    #endif
    #if CFG_USART2_ENABLE ==1
    ModuleInstall_UART(CN_USART2);
    #endif
    #if CFG_USART3_ENABLE ==1
    ModuleInstall_UART(CN_USART3);
    #endif
    #if CFG_UART4_ENABLE ==1
    ModuleInstall_UART(CN_UART4);
    #endif
    #if CFG_UART5_ENABLE ==1
    ModuleInstall_UART(CN_UART5);
    #endif
    #if CFG_LPUART1_ENABLE ==1
    ModuleInstall_UART(CN_LPUART1);
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
