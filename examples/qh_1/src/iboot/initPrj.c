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

	extern bool_t ModuleInstall_MsgQ(void);
	ModuleInstall_MsgQ ( );

	extern bool_t ModuleInstall_Multiplex(void);
	ModuleInstall_Multiplex ();

	extern ptu32_t ModuleInstall_UART(u32 serial_no);
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

	#if CFG_I2C1_ENABLE==1
	extern bool_t IIC1_Init(void);
	IIC1_Init();
	#endif
	#if CFG_I2C2_ENABLE==1
	extern bool_t IIC2_Init(void);
	IIC2_Init();
	#endif

	//-------------------medium-------------------------//
//	extern void ModuleInstall_LowPower (void);
//	ModuleInstall_LowPower();

	extern ptu32_t ModuleInstall_Charset(ptu32_t para);
	ModuleInstall_Charset(0);
	extern void ModuleInstall_CharsetNls(const char * DefaultCharset);
	ModuleInstall_CharsetNls("C");

	extern bool_t ModuleInstall_CharsetAscii(void);
	ModuleInstall_CharsetAscii ( );

	extern bool_t ModuleInstall_Font(void);
	ModuleInstall_Font ( );

	extern void ModuleInstall_FontAscii(void);
	ModuleInstall_FontAscii();

	extern bool_t ModuleInstall_GK(void);
	ModuleInstall_GK();

	extern bool_t ModuleInstall_HmiIn(void);
	ModuleInstall_HmiIn();      //��ʼ���˻���������ģ��

	extern bool_t ModuleInstall_Touch(void);
	ModuleInstall_Touch();    //��ʼ���˻���������ģ��

	extern ptu32_t ModuleInstall_ili9325(const char *DisplayName,const char* HeapName);
	ModuleInstall_ili9325(CFG_ILI9325_DISPLAY_NAME,CFG_ILI9325_HEAP_NAME);

	//-------------------later-------------------------//
	extern void ModuleInstall_Gdd_AND_Desktop(void);
	ModuleInstall_Gdd_AND_Desktop();

	extern s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err);
	ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);

	struct GkWinObj;
	extern ptu32_t ModuleInstall_Touch_Stmpe811(struct GkWinObj *desktop,\
	const char *touch_dev_name);
	extern struct GkWinObj *GK_GetDesktop(const char *display_name);
	struct GkWinObj *stmpe811_desktop;
	stmpe811_desktop = GK_GetDesktop(CFG_DISPLAY_NAME);
	if(NULL == stmpe811_desktop)
	{
	printf("stmpe811_desktop Not Exist !\r\n");
	}
	else
	{
	ModuleInstall_Touch_Stmpe811(stmpe811_desktop,CFG_STMPE811_TOUCH_DEV_NAME);
	}
	extern bool_t GDD_AddInputDev(const char *InputDevName);
	GDD_AddInputDev(CFG_INPUTDEV_NAME);

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
