/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#include "project_config.h"
#include "djyos.h"
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
extern ptu32_t djy_main(void);
#include "IoIicBus.h"
#include "xip.h"
#include "filesystems.h"

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

	struct IO_IIC_Init IoIic;
	u32 IIC_IoCtrlFunc(enum IIc_Io IO,u32 tag);
	IoIic.BusName     =  IO_IIC_BUS_NAME;//总线名称，如IIC1
	IoIic.tag         =  IO_IIC_USER_TAG; //用户自己的标记
	IoIic.IIC_IoCtrl  =  IIC_IOCTRL_FUN; //控制函数
	//模块接口
	ModuleInstall_IO_IICBus(&IoIic);

	extern s32 ModuleInstall_XIP_IBOOT_FS(u32 opt, void *data);
	ModuleInstall_XIP_IBOOT_FS(0,NULL);

	extern bool_t ModuleInstall_MsgQ(void);
	ModuleInstall_MsgQ ( );

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

	extern ptu32_t ModuleInstall_IAP(void);
	ModuleInstall_IAP( );

	extern bool_t ModuleInstall_PCF8574(char *BusName);
	ModuleInstall_PCF8574(CFG_PCF8574_BUS_NAME);

	s32 ModuleInstall_EmbededFlash(const char *TargetFs,u32 bstart, u32 bend, u32 doformat);
	ModuleInstall_EmbededFlash(CFG_EFLASH_FSMOUNT_NAME,CFG_EFLASH_PART_START, CFG_EFLASH_PART_END, CFG_EFLASH_PART_OPTION);

	extern bool_t MoudleInit_Systime(ptu32_t para);
	MoudleInit_Systime(0);

	extern bool_t ModuleInstall_HardTimer(void);
	ModuleInstall_HardTimer();

	extern s32 ModuleInstall_YAF2(const char *target, u32 opt, void *data);
	ModuleInstall_YAF2(CFG_YAF_MOUNT_POINT, CFG_YAF_INSTALL_OPTION, CFG_YAF_ECC);

	s32 ModuleInstall_NAND(const char *TargetFs,u32 bstart, u32 bend, u32 doformat);
	ModuleInstall_NAND(CFG_NFLASH_FSMOUNT_NAME, CFG_NFLASH_PART_START, CFG_NFLASH_PART_END, CFG_NFLASH_PART_OPTION);

	//-------------------medium-------------------------//

	extern bool_t ModuleInstall_TcpIp(void);
	ModuleInstall_TcpIp( );

	extern bool_t TcpInit(void);
	TcpInit();
	extern bool_t ModuleInstall_Timer(void);
	ModuleInstall_Timer();

	extern bool_t ModuleInstall_Ymodem(void);
	ModuleInstall_Ymodem();

	extern s32 kernel_command(void);
	kernel_command();

	extern void ModuleInstall_InitNet( );
	ModuleInstall_InitNet( );

	extern ptu32_t ModuleInstall_CpuRtc(ptu32_t para);
	ModuleInstall_CpuRtc(0);

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

