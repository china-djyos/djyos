/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#include "project_config.h"
#include "djyos.h"
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "cpu.h"

extern ptu32_t djy_main(void);

ptu32_t __djy_main(void)
{
	djy_main();
	return 0;
}

void Sys_ModuleInit(void)
{
	uint16_t evtt_main;

	extern void Board_Init(void);
	Board_Init();
	extern void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut);
    Stdio_KnlInOutInit(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME);
    extern s32 ModuleInstall_Shell(ptu32_t para);
    ModuleInstall_Shell(0);

    extern void ModuleInstall_BlackBox(void);
    ModuleInstall_BlackBox( );

    extern bool_t ModuleInstall_Multiplex(void);
    ModuleInstall_Multiplex ();

    extern ptu32_t ModuleInstall_UART(ptu32_t SerialNo);
    #if CFG_UART1_ENABLE ==1
    ModuleInstall_UART(CN_UART1);
    #endif
    #if CFG_UART2_ENABLE ==1
     ModuleInstall_UART(CN_UART2);
    #endif

     extern int ModuleInstall_Flash(void);
     ModuleInstall_Flash();

     extern int ModuleInstall_GPIO(void);
     ModuleInstall_GPIO();
     extern s32 __embed_part_init(u32 bstart, u32 bcount, u32 doformat);
     extern s32 ModuleInstall_UnitMedia(s32(*dev_init)(u32 bstart, u32 bcount, u32 doformat),
             u8 parts, ...);
     ModuleInstall_UnitMedia(__embed_part_init,1,0,150,1);

     extern s32 ModuleInstall_IAP_FS(const char *target, const char *source, u32 opt);
     ModuleInstall_IAP_FS("/iboot","/dev/unit media/embed part 0",0);

     extern ptu32_t ModuleInstall_IAP(void);
     ModuleInstall_IAP();

     extern bool_t ModuleInstall_Ymodem(void);
     ModuleInstall_Ymodem();
    //-------------------later-------------------------//
    extern s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err);
    ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);

	evtt_main = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
	__djy_main,NULL,CFG_MAINSTACK_LIMIT, "main function");
	//事件的两个参数暂设为0,如果用shell启动,可用来采集shell命令行参数
	Djy_EventPop(evtt_main,NULL,0,NULL,0,0);

	extern bool_t Heap_DynamicModuleInit(void);
	Heap_DynamicModuleInit ( );

	printf("\r\n: info : all modules are configured.");
	printf("\r\n: info : os starts.\r\n");

	return ;
}

