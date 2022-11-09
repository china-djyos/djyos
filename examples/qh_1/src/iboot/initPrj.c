/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#include "project_config.h"
#include "djyos.h"
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include <Iboot_info.h>


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

    //cpu onchip peripheral lowpower control
    void ModuleInstall_LowPower (void);
    ModuleInstall_LowPower();
    //end cpu onchip peripheral lowpower control

    //device file system
    extern s32 ModuleInstall_dev(void);
    ModuleInstall_dev();    // 安装设备文件系统；
    //end device file system

    //loader
    #if !defined (CFG_RUNMODE_BAREAPP)
    extern bool_t ModuleInstall_UpdateIboot(void);
    ModuleInstall_UpdateIboot( );
    #endif
    //end loader

    //message queue
    extern bool_t ModuleInstall_MsgQ(void);
    ModuleInstall_MsgQ ( );
    //end message queue

    //multiplex
    extern bool_t ModuleInstall_Multiplex(void);
    ModuleInstall_Multiplex ();
    //end multiplex

    //djybus
    extern bool_t ModuleInstall_DjyBus(void);
    ModuleInstall_DjyBus ( );
    //end djybus

    //iicbus
    extern bool_t ModuleInstall_IICBus(void);
    ModuleInstall_IICBus ( );
    //end iicbus

    //cpu onchip iic
    #if CFG_I2C1_ENABLE==1
    extern bool_t IIC1_Init(void);
    IIC1_Init();
    #endif
    #if CFG_I2C2_ENABLE==1
    extern bool_t IIC2_Init(void);
    IIC2_Init();
    #endif
    //end cpu onchip iic

    //----------------------------medium----------------------------//
    //font
    extern bool_t ModuleInstall_Font(void);
    ModuleInstall_Font ( );
    //end font

    //graphical kernel
    extern bool_t ModuleInstall_GK(void);
    ModuleInstall_GK();
    //end graphical kernel

    //graphical decorate development
    extern void ModuleInstall_Gdd_AND_Desktop(void);
    ModuleInstall_Gdd_AND_Desktop();
    //end graphical decorate development

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

    //ascii charset
    extern bool_t ModuleInstall_CharsetAscii(void);
    ModuleInstall_CharsetAscii ( );
    //end ascii charset

    //ascii dot font
    extern void ModuleInstall_FontAscii(void);
    ModuleInstall_FontAscii();
    //end ascii dot font

    //LCD driver ili9325
    extern ptu32_t ModuleInstall_ili9325(void);
    ModuleInstall_ili9325();
    //end LCD driver ili9325

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

    //stmpe811
    struct GkWinObj;
    extern ptu32_t ModuleInstall_Touch_Stmpe811(void);
    ModuleInstall_Touch_Stmpe811(stmpe811_desktop);
    #if(CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT == true)
    extern bool_t GDD_AddInputDev(const char *InputDevName);
    GDD_AddInputDev(CFG_STMPE811_TOUCH_DEV_NAME);
    #endif
    //end stmpe811

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
