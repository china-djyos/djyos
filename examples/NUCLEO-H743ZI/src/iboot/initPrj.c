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

	extern void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut);
	Stdio_KnlInOutInit(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME);
	extern s32 ModuleInstall_Shell(ptu32_t para);
	ModuleInstall_Shell(0);

	//-------------------early-------------------------//
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

	extern bool_t ModuleInstall_MsgQ(void);
	ModuleInstall_MsgQ ( );

	extern ptu32_t ModuleInstall_IAP(void);
	ModuleInstall_IAP( );

	//-------------------medium-------------------------//
	extern bool_t ModuleInstall_TcpIp(void);
	ModuleInstall_TcpIp( );

	bool_t TcpInit(void);
	TcpInit();

	extern bool_t ModuleInstall_Ymodem(void);
	ModuleInstall_Ymodem();

	extern s32 kernel_command(void);
	kernel_command();

	extern bool_t ModuleInstall_ETH(const char *devname, u8 *macaddress,\
	bool_t loop,u32 loopcycle,\
	bool_t (*rcvHook)(u8 *buf, u16 len));
	static u8 mac_addr[]={CFG_MAC_ADDR0,CFG_MAC_ADDR1,CFG_MAC_ADDR2,CFG_MAC_ADDR3,CFG_MAC_ADDR4,CFG_MAC_ADDR5};
//	ModuleInstall_ETH(CFG_ETH_DEV_NAME,mac_addr,CFG_ETH_LOOP_ENABLE,CFG_ETH_LOOP_CYCLE,NULL);

	extern void ModuleInstall_InitNet( );
	ModuleInstall_InitNet( );

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

