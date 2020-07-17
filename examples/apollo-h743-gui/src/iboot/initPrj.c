/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#include "project_config.h"
#include "djyos.h"
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include <Iboot_info.h>

#include <djyfs/filesystems.h>

extern ptu32_t djy_main(void);
//如果这个CN_MANUFACTURER_NAME名字要改，那djysrc里的Iboot_info.c中相应的名字也要改
const char CN_MANUFACTURER_NAME[] = PRODUCT_MANUFACTURER_NAME;



ptu32_t __djy_main(void)
{
    djy_main();

    return 0;
}

void Sys_ModuleInit(void)
{
    uint16_t evtt_main;

    //board config
    extern void Board_Init(void);
    Board_Init();
    //end board config

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

    //message queue
    extern bool_t ModuleInstall_MsgQ(void);
    ModuleInstall_MsgQ ( );
    //end message queue

    //multiplex
    extern bool_t ModuleInstall_Multiplex(void);
    ModuleInstall_Multiplex ();
    //end multiplex

    //xip app file system
    extern s32 ModuleInstall_XIP_APP_FS(u32 opt, void *data);
    ModuleInstall_XIP_APP_FS(0,NULL);
    //end xip app file system

    //cpu onchip timer
    extern bool_t ModuleInstall_HardTimer(void);
    ModuleInstall_HardTimer();
    //end cpu onchip timer

    //cpu onchip systime
    extern bool_t MoudleInit_Systime(ptu32_t para);
    MoudleInit_Systime(0);
    //end cpu onchip systime

    //cpu onchip random
    extern u8 RNG_Init(void);
    RNG_Init();
    //end cpu onchip random

    //djybus
    extern bool_t ModuleInstall_DjyBus(void);
    ModuleInstall_DjyBus ( );
    //end djybus

    //can bus
    extern bool_t ModuleInstall_CANBus(void);
    ModuleInstall_CANBus ( );
    //end can bus

    //iicbus
    extern bool_t ModuleInstall_IICBus(void);
    ModuleInstall_IICBus ( );
    //end iicbus

    //IIC to 8gpio pcf8574
    extern bool_t ModuleInstall_PCF8574(void);
    ModuleInstall_PCF8574();
    //end IIC to 8gpio pcf8574

    //easy file system
    extern s32 ModuleInstall_EFS(const char *target, u32 opt, u32 config);
    ModuleInstall_EFS(CFG_EFS_MOUNT_POINT, CFG_EFS_INSTALL_OPTION, 0);
    //end easy file system

    //----------------------------medium----------------------------//
    //font
    extern bool_t ModuleInstall_Font(void);
    ModuleInstall_Font ( );
    //end font

    //kernel
    #if(CFG_OS_TINY == flase)
    extern s32 kernel_command(void);
    kernel_command();
    #endif
    //end kernel

    //Nls Charset
    extern ptu32_t ModuleInstall_Charset(ptu32_t para);
    ModuleInstall_Charset(0);
    extern void ModuleInstall_CharsetNls(const char * DefaultCharset);
    ModuleInstall_CharsetNls("C");
    //end Nls Charset

    //gb2312 charset
    extern bool_t ModuleInstall_CharsetGb2312(void);
    ModuleInstall_CharsetGb2312 ( );
    //end gb2312 charset

    //gb2312 dot
    extern void ModuleInstall_FontGB2312(void);
    ModuleInstall_FontGB2312();
    //end gb2312 dot

    //ascii charset
    extern bool_t ModuleInstall_CharsetAscii(void);
    ModuleInstall_CharsetAscii ( );
    //end ascii charset

    //Software Timers
    extern bool_t ModuleInstall_Timer(void);
    ModuleInstall_Timer();
    //end Software Timers

    //graphical kernel
    extern bool_t ModuleInstall_GK(void);
    ModuleInstall_GK();
    //end graphical kernel

    //graphical decorate development
    extern void ModuleInstall_Gdd_AND_Desktop(void);
    ModuleInstall_Gdd_AND_Desktop();
    //end graphical decorate development

    //cpu onchip lcd
    extern struct DisplayObj* ModuleInstall_LCD(const char *DisplayName,const char* HeapName);
    ModuleInstall_LCD(CFG_DISPLAY_NAME,CFG_LCD_HEAP_NAME);
    //end cpu onchip lcd

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

    //tcpip
    extern bool_t ModuleInstall_TcpIp(void);
    ModuleInstall_TcpIp( );
    //end tcpip

    //ethernet phy lan8720
    extern bool_t LAN8720_ResetInit(void);
    LAN8720_RESET( );
    LAN8720_ResetInit( );
    //end ethernet phy lan8720

    //cpu onchip MAC
    extern bool_t ModuleInstall_ETH(void);
    ModuleInstall_ETH( );
    //end cpu onchip MAC

    //human machine interface
    extern bool_t ModuleInstall_HmiIn(void);
    ModuleInstall_HmiIn();      //初始化人机界面输入模块
    //end human machine interface

    //touch
    extern bool_t ModuleInstall_Touch(void);
    ModuleInstall_Touch();    //初始化人机界面输入模块
    //end touch

    //----------------------------later----------------------------//
    //stdio
    #if(CFG_STDIO_STDIOFILE == true)
    extern s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err);
    ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);
    #endif
    //end stdio

    //network config
    extern void ModuleInstall_InitNet( );
    ModuleInstall_InitNet( );
    //end network config

    //touchscreen FT5X26
    extern bool_t ModuleInstall_FT5X26(void);
    ModuleInstall_FT5X26( );
    #if(CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT == true)
    extern bool_t GDD_AddInputDev(const char *InputDevName);
    GDD_AddInputDev(CFG_FT5X26_TOUCH_NAME);
    #endif
    //end touchscreen FT5X26

    evtt_main = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
        __djy_main,NULL,CFG_MAINSTACK_LIMIT, "main function");
    //事件的两个参数暂设为0,如果用shell启动,可用来采集shell命令行参数
    DJY_EventPop(evtt_main,NULL,0,0,0,0);

    //heap
    #if ((CFG_DYNAMIC_MEM == true))
    extern bool_t Heap_DynamicModuleInit(void);
    Heap_DynamicModuleInit ( );
    #endif
    //end heap



    return ;
}
