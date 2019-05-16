/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#include "project_config.h"
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
	extern s32 ModuleInstall_dev(void);
	ModuleInstall_dev();    // 安装设备文件系统；

	extern void ModuleInstall_BlackBox(void);
	ModuleInstall_BlackBox( );

	extern bool_t ModuleInstall_Multiplex(void);
	ModuleInstall_Multiplex ();

	extern ptu32_t ModuleInstall_IAP(void);
	ModuleInstall_IAP( );

	extern bool_t ModuleInstall_MsgQ(void);
	ModuleInstall_MsgQ ( );

	//-------------------medium-------------------------//
	#if(CFG_OS_TINY == flase)
	extern s32 kernel_command(void);
	kernel_command();
	#endif

	//-------------------later-------------------------//
	#if(CFG_STDIO_STDIOFILE == true)
	extern s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err);
	ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);
	#endif

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

