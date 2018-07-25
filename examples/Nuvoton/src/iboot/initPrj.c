/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"

#include "project_config.h"

char * const DJY_IBOOT_VERSION = CFG_DJY_IBOOT_VERSION;
char * const DJY_IBOOT_COMPILE_DATE = CFG_DJY_IBOOT_COMPILE_DATE;
char * const DJY_IBOOT_COMPILE_TIME = CFG_DJY_IBOOT_COMPILE_TIME;

const u8 g_IbootType  = CFG_IBOOT_TYPE;   //是否直接从程序存储器中加载（即Mcu模式）
const u8 g_IbootCRC   = CFG_IBOOT_CRC;  //是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验

align_type InitStack[CFG_INIT_STACK_SIZE/sizeof(align_type)] = {'d'};

const char *gc_pCfgStdinName =  CFG_STDIO_IN_NAME;    //标准输入设备
const char *gc_pCfgStdoutName = CFG_STDIO_OUT_NAME;   //标准输出设备
const char *gc_pCfgStderrName = CFG_STDIO_ERR_NAME;   //标准错误输出设备

const char *g_AppUpdateName=CFG_APP_UPDATE_NAME;
const char *g_AppUpdatePath=CFG_APP_UPDATE_PATH;
const char *g_IbootUpdateName=CFG_IBOOT_UPDATE_NAME;
const char *g_IbootUpdateFS =  NULL;

const u32 gc_u32CfgMainStackLen = 2048;

ptu32_t djy_main(void);
ptu32_t __djy_main(void)
{
    djy_main();
    return 0;
}

void Sys_ModuleInit(void)
{
    uint16_t evtt_main;

    Stdio_KnlInOutInit(0);
	extern bool_t ModuleInstall_Sh(void);
	ModuleInstall_Sh();

    extern void ModuleInstall_Exp(void);
    ModuleInstall_Exp( );

    extern ptu32_t ModuleInstall_IAP(void);
    ModuleInstall_IAP( );

    extern bool_t ModuleInstall_Multiplex(void);
    ModuleInstall_Multiplex ();

	extern ptu32_t ModuleInstall_UART(ptu32_t SerialNo);
	#if CFG_UART0_ENABLE ==1
	ModuleInstall_UART(CN_UART0);
	#endif
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

    extern s32 ModuleInstall_STDIO(const char *in, \
    const char *out, const char *err);
    ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);

	printf("\r\n: info : all modules are configured.");
	printf("\r\n: info : os starts.\r\n");

	 evtt_main = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
	                                __djy_main,NULL,gc_u32CfgMainStackLen,
	                                "main function");
	    //事件的两个参数暂设为0,如果用shell启动,可用来采集shell命令行参数
	    Djy_EventPop(evtt_main,NULL,0,NULL,0,0);

	    extern bool_t Heap_DynamicModuleInit(void);
	    Heap_DynamicModuleInit ( );

	return ;
}
