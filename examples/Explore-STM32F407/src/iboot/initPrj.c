/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#include "project_config.h"
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
extern ptu32_t djy_main(void);

ptu32_t __djy_main(void)
{
	extern s32 ModuleInstall_STDIO(const char *in, \
	const char *out, const char *err);
	ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);

	djy_main();
	return 0;
}

void Sys_ModuleInit(void)
{
	uint16_t evtt_main;

	extern void Board_GpioInit(void);
	Board_GpioInit();

	extern void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut);
	Stdio_KnlInOutInit(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME);

//	extern bool_t ModuleInstall_Sh(void);
//	ModuleInstall_Sh();

	extern ptu32_t ModuleInstall_NewSh(ptu32_t para);
	ModuleInstall_NewSh(0);

	extern void ModuleInstall_CharsetNls(const char * DefaultCharset);
	ModuleInstall_CharsetNls("C");

	extern bool_t ModuleInstall_DjyBus(void);
	ModuleInstall_DjyBus ( );

	extern bool_t ModuleInstall_IICBus(void);
	ModuleInstall_IICBus ( );

	extern bool_t ModuleInstall_SPIBus(void);
	ModuleInstall_SPIBus();

	extern void ModuleInstall_Exp(void);
	ModuleInstall_Exp( );

	extern bool_t ModuleInstall_Font(void);
	ModuleInstall_Font ( );

	extern ptu32_t ModuleInstall_IAP(void);
	ModuleInstall_IAP( );

	extern bool_t ModuleInstall_MsgQ(void);
	ModuleInstall_MsgQ ( );

	extern bool_t ModuleInstall_GK(void);
	ModuleInstall_GK();

	extern bool_t ModuleInstall_HmiIn(void);
	ModuleInstall_HmiIn();      //初始化人机界面输入模块

	extern bool_t ModuleInstall_Multiplex(void);
	ModuleInstall_Multiplex ();

	extern bool_t ModuleInstall_Timer(void);
	ModuleInstall_Timer();

	extern bool_t ModuleInstall_Wdt(void);
	ModuleInstall_Wdt();

//	extern bool_t ModuleInstall_Ymodem(void);
//	ModuleInstall_Ymodem();

	

	extern bool_t ModuleInstall_I2C(u8 port);
	#if CFG_I2C1_ENABLE==1
	ModuleInstall_I2C(CN_I2C1);
	#endif
	#if CFG_I2C2_ENABLE==1
	ModuleInstall_I2C(CN_I2C2);
	#endif
	#if CFG_I2C3_ENABLE==1
	ModuleInstall_I2C(CN_I2C3);
	#endif

	extern bool_t ModuleInstall_SPI(u8 port);
	#if CFG_SPI1_ENABLE==1
	ModuleInstall_SPI(CN_SPI1);
	#endif
	#if CFG_SPI2_ENABLE==1
	ModuleInstall_SPI(CN_SPI2);
	#endif
	#if CFG_SPI3_ENABLE==1
	ModuleInstall_SPI(CN_SPI3);
	#endif

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

