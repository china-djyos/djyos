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
//	extern void ModuleInstall_InitNet( );
//	ModuleInstall_InitNet( );

//	extern void CT_IIC_Init(void);
//	CT_IIC_Init();

//	bool_t IIC_Init(u8 iic_port);
//	#ifdef CFG_IIC1_ENABLE
//	IIC_Init(CN_IIC1);
//	#endif
//	#ifdef CFG_IIC2_ENABLE
//	IIC_Init(CN_IIC2);
//	#endif
//	#ifdef CFG_IIC3_ENABLE
//	IIC_Init(CN_IIC3);
//	#endif
//	#ifdef CFG_IIC4_ENABLE
//	IIC_Init(CN_IIC4);
//	#endif

//	u8 PCF8574_Init(void);
//	PCF8574_Init();

//	extern bool_t ModuleInstall_Keyboard(const char *dev_name);
//	ModuleInstall_Keyboard(CFG_KEYBOARD_NAME);

	extern bool_t CAN_Main(uint8_t baudrate);
	CAN_Main(CFG_CPU_CAN_BAUDRATE);

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

	extern void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut);
	Stdio_KnlInOutInit(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME);

	extern bool_t ModuleInstall_Sh(void);
	ModuleInstall_Sh();

	extern void HAL_TickInit(void);
	HAL_TickInit();

	

	extern void ModuleInstall_Exp(void);
	ModuleInstall_Exp( );

	extern ptu32_t ModuleInstall_IAP(void);
	ModuleInstall_IAP( );

	extern bool_t ModuleInstall_Multiplex(void);
	ModuleInstall_Multiplex ();

	extern bool_t ModuleInstall_MsgQ(void);
	ModuleInstall_MsgQ ( );

//	extern bool_t ModuleInstall_GK(void);
//	ModuleInstall_GK();

//	extern struct GkWinObj;
//	extern bool_t ModuleInstall_Touch_FT5206(struct GkWinObj *desktop,\
//	const char *touch_dev_name );
//	struct GkWinObj *desktop;
//	desktop = GK_GetDesktop(CFG_DESKTOP_NAME);
//	if(NULL == desktop)
//	{
//	printf("Desktop Not Exist !\r\n");
//	}
//	else
//	{
//	ModuleInstall_Touch_FT5206(desktop,CFG_TOUCH_NAME);
//	}

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

