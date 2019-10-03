/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#include "project_config.h"
#include "djyos.h"
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
extern ptu32_t djy_main(void);
#include <filesystems.h>

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
	ModuleInstall_dev();    // 安装设备文件系统；

	extern void ModuleInstall_BlackBox(void);
	ModuleInstall_BlackBox( );

	extern bool_t ModuleInstall_DjyBus(void);
	ModuleInstall_DjyBus ( );

	extern bool_t ModuleInstall_MsgQ(void);
	ModuleInstall_MsgQ ( );

	extern bool_t ModuleInstall_IICBus(void);
	ModuleInstall_IICBus ( );

	bool_t ModuleInstall_init_ioiic(const char * busname);
	ModuleInstall_init_ioiic(IO_IIC_BUS_NAME);

	extern bool_t ModuleInstall_Multiplex(void);
	ModuleInstall_Multiplex ();

	#if !defined (CFG_RUNMODE_BAREAPP)
	extern bool_t ModuleInstall_XIP(void);
	ModuleInstall_XIP( );
	#endif

	extern s32 ModuleInstall_FAT(const char *dir, u32 opt, void *data);
	ModuleInstall_FAT(CFG_FAT_MOUNT_POINT, CFG_FAT_MS_INSTALLUSE, CFG_FAT_MEDIA_KIND);

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

#if 0
	extern int ModuleInstall_QSPI_PSRAM(void);
	ModuleInstall_QSPI_PSRAM();
#endif
	//-------------------medium-------------------------//
	extern ptu32_t ModuleInstall_Charset(ptu32_t para);
	ModuleInstall_Charset(0);
	extern void ModuleInstall_CharsetNls(const char * DefaultCharset);
	ModuleInstall_CharsetNls("C");

	extern bool_t ModuleInstall_Font(void);
	ModuleInstall_Font ( );

	extern bool_t ModuleInstall_CharsetGb2312(void);
	ModuleInstall_CharsetGb2312 ( );

	extern void ModuleInstall_FontGB2312(void);
	ModuleInstall_FontGB2312();

	extern bool_t ModuleInstall_GK(void);
	ModuleInstall_GK();

	extern bool_t ModuleInstall_HmiIn(void);
	ModuleInstall_HmiIn();      //初始化人机界面输入模块

	#if(CFG_OS_TINY == flase)
	extern s32 kernel_command(void);
	kernel_command();
	#endif

	//extern void ModuleInstall_LowPower (void);
	//ModuleInstall_LowPower();

	extern bool_t ModuleInstall_Touch(void);
	ModuleInstall_Touch();    //初始化人机界面输入模块

    extern void random_init(void);
    random_init();
#if 1
    extern bool_t ModuleInstall_TcpIp(void);
    ModuleInstall_TcpIp( );

	extern void ModuleInstall_InitNet( );
	ModuleInstall_InitNet( );
#endif
	extern int ModuleInstall_GPIO(void);
	ModuleInstall_GPIO();
    extern int ModuleInstall_SDCARD(void);
    ModuleInstall_SDCARD();
//
//	extern ptu32_t ModuleInstall_st7796s(const char *DisplayName,const char* HeapName);
//	ModuleInstall_st7796s(CFG_ST7796S_DISPLAY_NAME,CFG_ST7796S_HEAP_NAME);

	extern int ModuleInstall_Flash(void);
	ModuleInstall_Flash();

//	extern s32 ModuleInstall_FlashInstallXIP(const char *TargetFs,s32 bstart, s32 bend, u32 doformat);
//	ModuleInstall_FlashInstallXIP(CFG_EFLASH_XIPFSMOUNT_NAME,CFG_EFLASH_XIP_PART_START,
//	CFG_EFLASH_XIP_PART_END, CFG_EFLASH_XIP_PART_FORMAT);

	//-------------------later-------------------------//
//	extern void ModuleInstall_Gdd_AND_Desktop(void);
//	ModuleInstall_Gdd_AND_Desktop();

	#if(CFG_STDIO_STDIOFILE == true)
	extern s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err);
	ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);
	#endif

//	extern struct GkWinObj;
//	extern bool_t ModuleInstall_FT6236(struct GkWinObj *desktop);
//	struct GkWinObj *desktop;
//	desktop = GK_GetDesktop(CFG_DISPLAY_NAME);
//	if(NULL == desktop)
//	{
//	printf("Desktop Not Exist !\r\n");
//	}
//	else
//	{
//	ModuleInstall_FT6236(desktop);
//	}
//	extern bool_t GDD_AddInputDev(const char *InputDevName);
//	GDD_AddInputDev(CFG_FT6236_TOUCH_NAME);

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

