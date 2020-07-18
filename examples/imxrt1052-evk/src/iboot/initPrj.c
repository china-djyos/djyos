/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#include "project_config.h"
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

    extern void ClockSetXtal(void);
    ClockSetXtal();
    extern void Board_Init(void);
    Board_Init();

    extern void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut);
    Stdio_KnlInOutInit(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME);
    extern s32 ModuleInstall_Shell(ptu32_t para);
    ModuleInstall_Shell(0);

    //-------------------early-------------------------//
    extern bool_t ModuleInstall_DjyBus(void);
    ModuleInstall_DjyBus ( );

    extern bool_t ModuleInstall_IICBus(void);
    ModuleInstall_IICBus ( );

    extern bool_t IIC_Init(u8 iic_port);
    #if (CFG_IIC1_ENABLE == 1)
    IIC_Init(CN_IIC1);
    #endif
    #if (CFG_IIC2_ENABLE == 1)
    IIC_Init(CN_IIC2);
    #endif
    #if (CFG_IIC3_ENABLE == 1)
    IIC_Init(CN_IIC3);
    #endif
    #if (CFG_IIC4_ENABLE == 1)
    IIC_Init(CN_IIC4);
    #endif

    extern s32 ModuleInstall_dev(void);
    ModuleInstall_dev();    // 安装设备文件系统；

    extern ptu32_t ModuleInstall_UART(u8 port);
    #if (CFG_UART1_ENABLE == 1)
    ModuleInstall_UART(CN_UART1);
    #endif
    #if (CFG_UART2_ENABLE == 1)
    ModuleInstall_UART(CN_UART2);
    #endif
    #if (CFG_UART3_ENABLE == 1)
    ModuleInstall_UART(CN_UART3);
    #endif
    #if (CFG_UART4_ENABLE == 1)
    ModuleInstall_UART(CN_UART4);
    #endif
    #if (CFG_UART5_ENABLE == 1)
    ModuleInstall_UART(CN_UART5);
    #endif
    #if (CFG_UART6_ENABLE == 1)
    ModuleInstall_UART(CN_UART6);
    #endif
    #if (CFG_UART7_ENABLE == 1)
    ModuleInstall_UART(CN_UART7);
    #endif
    #if (CFG_UART8_ENABLE == 1)
    ModuleInstall_UART(CN_UART8);
    #endif

    extern void ModuleInstall_BlackBox(void);
    ModuleInstall_BlackBox( );

    extern bool_t ModuleInstall_MsgQ(void);
    ModuleInstall_MsgQ ( );

    #if !defined (CFG_RUNMODE_BAREAPP)
    extern ptu32_t ModuleInstall_IAP(void);
    ModuleInstall_IAP( );
    #endif

    extern bool_t ModuleInstall_Multiplex(void);
    ModuleInstall_Multiplex ();

    //-------------------medium-------------------------//
    #if(CFG_OS_TINY == flase)
    extern s32 kernel_command(void);
    kernel_command();
    #endif

    extern bool_t ModuleInstall_GK(void);
    ModuleInstall_GK();

    extern struct DisplayRsc* ModuleInstall_LCD(const char *DisplayName,\
    const char* HeapName);
    ModuleInstall_LCD(CFG_LCD_DISPLAY_NAME,CFG_LCD_HEAP_NAME);

    extern bool_t ModuleInstall_Font(void);
    ModuleInstall_Font ( );

    extern void ModuleInstall_FontGB2312(void);
    ModuleInstall_FontGB2312();

    extern bool_t ModuleInstall_HmiIn(void);
    ModuleInstall_HmiIn();      //初始化人机界面输入模块

    extern bool_t ModuleInstall_Touch(void);
    ModuleInstall_Touch();    //初始化人机界面输入模块

    extern ptu32_t ModuleInstall_Charset(ptu32_t para);
    ModuleInstall_Charset(0);
    extern void ModuleInstall_CharsetNls(const char * DefaultCharset);
    ModuleInstall_CharsetNls("C");

    extern bool_t ModuleInstall_CharsetGb2312(void);
    ModuleInstall_CharsetGb2312 ( );

    //-------------------later-------------------------//
    extern void ModuleInstall_Gdd_AND_Desktop(void);
    ModuleInstall_Gdd_AND_Desktop();

    #if(CFG_STDIO_STDIOFILE == true)
    extern s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err);
    ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);
    #endif

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

