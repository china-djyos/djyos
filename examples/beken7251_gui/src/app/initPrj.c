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

    extern void Board_Init(void);
    Board_Init();

	extern void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut);
	Stdio_KnlInOutInit(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME);
	extern s32 ModuleInstall_Shell(ptu32_t para);
	ModuleInstall_Shell(0);

	//-------------------early-------------------------//
	extern s32 ModuleInstall_dev(void);
	ModuleInstall_dev();    // ��װ�豸�ļ�ϵͳ��

	extern void ModuleInstall_BlackBox(void);
	ModuleInstall_BlackBox( );

	extern bool_t ModuleInstall_DjyBus(void);
	ModuleInstall_DjyBus ( );

	extern bool_t ModuleInstall_Multiplex(void);
	ModuleInstall_Multiplex ();

	#if !defined (CFG_RUNMODE_BAREAPP)
	extern bool_t ModuleInstall_XIP(void);
	ModuleInstall_XIP( );
	#endif

	extern bool_t ModuleInstall_SPIBus(void);
	ModuleInstall_SPIBus();

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

	extern bool_t ModuleInstall_SPI(void);
	ModuleInstall_SPI();

	//-------------------medium-------------------------//
	extern bool_t ModuleInstall_GK(void);
	ModuleInstall_GK();

	#if(CFG_OS_TINY == flase)
	extern s32 kernel_command(void);
	kernel_command();
	#endif

//	extern ptu32_t ModuleInstall_st7796s(void);
//	ModuleInstall_st7796s();

//	extern int ModuleInstall_GPIO(void);
//	ModuleInstall_GPIO();

	extern int ModuleInstall_Flash(void);
	ModuleInstall_Flash();

	//-------------------later-------------------------//
	#if(CFG_STDIO_STDIOFILE == true)
	extern s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err);
	ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);
	#endif

	evtt_main = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
	__djy_main,NULL,CFG_MAINSTACK_LIMIT, "main function");
	//�¼���������������Ϊ0,�����shell����,�������ɼ�shell�����в���
	Djy_EventPop(evtt_main,NULL,0,NULL,0,0);

	#if ((CFG_DYNAMIC_MEM == true))
	extern bool_t Heap_DynamicModuleInit(void);
	Heap_DynamicModuleInit ( );
	#endif

	return ;
}

