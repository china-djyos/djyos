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

    extern bool_t ModuleInstall_DjyBus(void);
    ModuleInstall_DjyBus ( );

    extern bool_t ModuleInstall_IICBus(void);
    ModuleInstall_IICBus ( );

    bool_t ModuleInstall_init_ioiic(const char * busname);
    ModuleInstall_init_ioiic(CFG_IO_IIC_BUS_NAME);

    extern ptu32_t ModuleInstall_IAP(void);
    ModuleInstall_IAP( );

    extern bool_t ModuleInstall_MsgQ(void);
    ModuleInstall_MsgQ ( );

    extern bool_t ModuleInstall_Multiplex(void);
    ModuleInstall_Multiplex ();

    extern bool_t ModuleInstall_HardTimer(void);
    ModuleInstall_HardTimer();

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

    extern bool_t ModuleInstall_PCF8574(void);
    ModuleInstall_PCF8574();

    //-------------------medium-------------------------//
    extern bool_t ModuleInstall_Font(void);
    ModuleInstall_Font ( );

    extern void ModuleInstall_FontGB2312(void);
    ModuleInstall_FontGB2312();

    extern bool_t ModuleInstall_GK(void);
    ModuleInstall_GK();

    extern bool_t ModuleInstall_HmiIn(void);
    ModuleInstall_HmiIn();      //初始化人机界面输入模块

    extern bool_t ModuleInstall_Touch(void);
    ModuleInstall_Touch();    //初始化人机界面输入模块

    #if(CFG_OS_TINY == flase)
    extern s32 kernel_command(void);
    kernel_command();
    #endif

    extern ptu32_t ModuleInstall_Charset(ptu32_t para);
    ModuleInstall_Charset(0);
    extern void ModuleInstall_CharsetNls(const char * DefaultCharset);
    ModuleInstall_CharsetNls("C");

    extern bool_t ModuleInstall_CharsetGb2312(void);
    ModuleInstall_CharsetGb2312 ( );

    extern bool_t ModuleInstall_TcpIp(void);
    ModuleInstall_TcpIp( );

    extern bool_t LAN8720_ResetInit(void);
    LAN8720_RESET( );
    LAN8720_ResetInit( );

    extern bool_t ModuleInstall_ETH(void);
    ModuleInstall_ETH( );

    extern bool_t ModuleInstall_KeyBoardHal(const char *dev_name);
    ModuleInstall_KeyBoardHal(CFG_KEYBOARD_NAME);

    //-------------------later-------------------------//
    extern void ModuleInstall_Gdd_AND_Desktop(void);
    ModuleInstall_Gdd_AND_Desktop();

    #if(CFG_STDIO_STDIOFILE == true)
    extern s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err);
    ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);
    #endif

    extern void ModuleInstall_InitNet( );
    ModuleInstall_InitNet( );

    extern struct GkWinObj;
    extern bool_t ModuleInstall_FT5X26(struct GkWinObj *desktop);
    struct GkWinObj *desktop;
    desktop = GK_GetDesktop(CFG_DISPLAY_NAME);
    if(NULL == desktop)
    {
    printf("Desktop Not Exist !\r\n");
    }
    else
    {
    ModuleInstall_FT5X26(desktop);
    }
    extern bool_t GDD_AddInputDev(const char *InputDevName);
    GDD_AddInputDev(CFG_FT5X26_TOUCH_NAME);

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

