/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#include "project_config.h"
#include "djyos.h"
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include <Iboot_info.h>


extern ptu32_t djy_main(void);
//������CN_MANUFACTURER_NAME����Ҫ�ģ���djysrc���Iboot_info.c����Ӧ������ҲҪ��
const char CN_MANUFACTURER_NAME[] = PRODUCT_MANUFACTURER_NAME;



ptu32_t __djy_main(void)
{
    djy_main();

	return 0;
}

void Sys_ModuleInit(void)
{
	uint16_t evtt_main;

    //shell
    extern void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut);
    Stdio_KnlInOutInit(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME);
    extern s32 ModuleInstall_Shell(ptu32_t para);
    ModuleInstall_Shell(0);
    //end shell

    //----------------------------early----------------------------//
    //black box
    extern void ModuleInstall_BlackBox(void);
    ModuleInstall_BlackBox( );
    //end black box

    //device file system
    extern s32 ModuleInstall_dev(void);
    ModuleInstall_dev();    // ��װ�豸�ļ�ϵͳ��
    //end device file system

    //message queue
    extern bool_t ModuleInstall_MsgQ(void);
    ModuleInstall_MsgQ ( );
    //end message queue

    //multiplex
    extern bool_t ModuleInstall_Multiplex(void);
    ModuleInstall_Multiplex ();
    //end multiplex

    //cpu drive inner flash
    s32 ModuleInstall_EmbededFlash(u32 doformat);
    ModuleInstall_EmbededFlash(CFG_EFLASH_PART_FORMAT);
    //end cpu drive inner flash

    //emflash insatall xip
    extern s32 ModuleInstall_EmFlashInstallXIP(const char *TargetFs,s32 bstart, s32 bend, u32 doformat);
    ModuleInstall_EmFlashInstallXIP(CFG_EFLASH_XIPFSMOUNT_NAME,CFG_EFLASH_XIP_PART_START,
    CFG_EFLASH_XIP_PART_END, CFG_EFLASH_XIP_PART_FORMAT);
    //end emflash insatall xip

    //----------------------------medium----------------------------//
    //kernel
    #if(CFG_OS_TINY == flase)
    extern s32 kernel_command(void);
    kernel_command();
    #endif
    //end kernel

    //cpu onchip uart
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
    //end cpu onchip uart

    //loader
    #if !defined (CFG_RUNMODE_BAREAPP)
    extern bool_t ModuleInstall_XIP(void);
    ModuleInstall_XIP( );
    #endif
    //end loader

    //----------------------------later----------------------------//
    //stdio
    #if(CFG_STDIO_STDIOFILE == true)
    extern s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err);
    ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);
    #endif
    //end stdio

	evtt_main = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
		__djy_main,NULL,CFG_MAINSTACK_LIMIT, "main function");
	//�¼���������������Ϊ0,�����shell����,�������ɼ�shell�����в���
	DJY_EventPop(evtt_main,NULL,0,0,0,0);

    //heap
    #if ((CFG_DYNAMIC_MEM == true))
    extern bool_t Heap_DynamicModuleInit(void);
    Heap_DynamicModuleInit ( );
    #endif
    //end heap



	return ;
}