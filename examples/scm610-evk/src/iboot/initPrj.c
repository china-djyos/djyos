/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。
#include "djyos.h"
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
extern ptu32_t djy_main(void);
#include <djyfs/filesystems.h>

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
    extern void ModuleInstall_BlackBox(void);
    ModuleInstall_BlackBox( );

    extern s32 ModuleInstall_dev(void);
    ModuleInstall_dev();    // 安装设备文件系统；

//  extern bool_t ModuleInstall_IICBus(void);
//  ModuleInstall_IICBus ( );

//  extern bool_t ModuleInstall_SPIBus(void);
//  ModuleInstall_SPIBus();

    extern bool_t ModuleInstall_DjyBus(void);
    ModuleInstall_DjyBus ( );

    extern s32 ModuleInstall_XIP_APP_FS(u32 opt, void *data);
    ModuleInstall_XIP_APP_FS(0,NULL);

    s32 ModuleInstall_EmbededFlash(u32 doformat);
    ModuleInstall_EmbededFlash(CFG_EFLASH_PART_FORMAT);

    extern s32 ModuleInstall_EmFlashInstallXIP(const char *TargetFs,s32 bstart, s32 bend, u32 doformat);
    ModuleInstall_EmFlashInstallXIP(CFG_EFLASH_XIPFSMOUNT_NAME,CFG_EFLASH_XIP_PART_START,
    CFG_EFLASH_XIP_PART_END, CFG_EFLASH_XIP_PART_FORMAT);

    extern bool_t MoudleInit_Systime(ptu32_t para);
    MoudleInit_Systime(0);

//  extern bool_t ModuleInstall_HardTimer(void);
//  ModuleInstall_HardTimer();

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

    extern bool_t ModuleInstall_Multiplex(void);
    ModuleInstall_Multiplex ();

//  extern bool_t ModuleInstall_MsgQ(void);
//  ModuleInstall_MsgQ ( );

    extern ptu32_t ModuleInstall_IAP(void);
    ModuleInstall_IAP( );

    //-------------------medium-------------------------//
    #if ((CFG_DYNAMIC_MEM == true))
    extern bool_t Heap_DynamicModuleInit(void);
    Heap_DynamicModuleInit ( );
    #endif

    extern void ModuleInstall_NetStaticIP( );
    ModuleInstall_NetStaticIP( );

//  extern ptu32_t ModuleInstall_CpuRtc(ptu32_t para);
//  ModuleInstall_CpuRtc(0);

    extern bool_t ModuleInstall_ETH(const char *devname, bool_t loop,u32 loopcycle);
    ModuleInstall_ETH(CFG_GMAC_NAME,CFG_GMAC_LOOP_MODE,CFG_GMAC_LOOP_CYCLE);

    extern bool_t ModuleInstall_TcpIp(void);
    ModuleInstall_TcpIp( );

    #if(CFG_OS_TINY == flase)
    extern s32 kernel_command(void);
    kernel_command();
    #endif

    extern bool_t ModuleInstall_Ymodem(void);
    ModuleInstall_Ymodem();

    //-------------------later-------------------------//
    #if(CFG_STDIO_STDIOFILE == true)
    extern s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err);
    ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);
    #endif

    return ;
}

