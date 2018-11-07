/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/
#include "djyos.h"
#include "project_config.h"
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

	extern bool_t ModuleInstall_DjyBus(void);
	ModuleInstall_DjyBus ( );

	extern bool_t ModuleInstall_IICBus(void);
	ModuleInstall_IICBus ( );

	extern ptu32_t ModuleInstall_IAP(void);
	ModuleInstall_IAP( );

	extern bool_t ModuleInstall_Multiplex(void);
	ModuleInstall_Multiplex ();

	extern ptu32_t ModuleInstall_UART(u32 serial_no,u32 SendBufLen, u32 RecvBufLen,u8 mode);
	#if CFG_UART1_ENABLE ==1
	ModuleInstall_UART(CN_UART1,CFG_UART1_SENDBUF_LEN,CFG_UART1_RECVBUF_LEN,CFG_UART1_MODE);
	#endif
	#if CFG_UART2_ENABLE ==1
	ModuleInstall_UART(CN_UART2,CFG_UART2_SENDBUF_LEN,CFG_UART2_RECVBUF_LEN,CFG_UART2_MODE);
	#endif
	#if CFG_UART3_ENABLE ==1
	ModuleInstall_UART(CN_UART3,CFG_UART3_SENDBUF_LEN,CFG_UART3_RECVBUF_LEN,CFG_UART3_MODE);
	#endif
	#if CFG_UART4_ENABLE ==1
	ModuleInstall_UART(CN_UART4,CFG_UART4_SENDBUF_LEN,CFG_UART4_RECVBUF_LEN,CFG_UART4_MODE);
	#endif
	#if CFG_UART5_ENABLE ==1
	ModuleInstall_UART(CN_UART5,CFG_UART5_SENDBUF_LEN,CFG_UART5_RECVBUF_LEN,CFG_UART5_MODE);
	#endif

	extern bool_t ModuleInstall_MsgQ(void);
	ModuleInstall_MsgQ ( );

	bool_t IIC_Init(u8 iic_port);
	#if CFG_IIC1_ENABLE== true
	ModuleInstall_IIC(CN_IIC1);
	#endif
	#if CFG_IIC2_ENABLE== true
	ModuleInstall_IIC(CN_IIC2);
	#endif
	#if CFG_IIC3_ENABLE== true
	ModuleInstall_IIC(CN_IIC3);
	#endif
	#if CFG_IIC4_ENABLE== true
	ModuleInstall_IIC(CN_IIC4);
	#endif

	//-------------------medium-------------------------//
	extern bool_t ModuleInstall_Wdt(void);
	ModuleInstall_Wdt();

	//-------------------later-------------------------//
	extern s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err);
	ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);

	evtt_main = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
	__djy_main,NULL,CFG_MAINSTACK_LIMIT, "main function");
	//�¼���������������Ϊ0,�����shell����,�������ɼ�shell�����в���
	Djy_EventPop(evtt_main,NULL,0,NULL,0,0);

	extern bool_t Heap_DynamicModuleInit(void);
	Heap_DynamicModuleInit ( );

	printf("\r\n: info : all modules are configured.");
	printf("\r\n: info : os starts.\r\n");

	return ;
}

