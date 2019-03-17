/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#include "project_config.h"
#include "djyos.h"
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
extern ptu32_t djy_main(void);
#include "IoIicBus.h"
#include "filesystems.h"

ptu32_t __djy_main(void)
{
    djy_main();
    return 0;
}

void Sys_ModuleInit(void)
{
    uint16_t evtt_main;
//  u32 loop,sum;
//  u32 *pppp;
//  for(sum = 0; sum < 1000000; sum++)
//  {
//      for(pppp = 0xc0000000; pppp < (u32*)0xc2000000; pppp++)
//          *pppp = (u32)pppp;
//        for(pppp = 0xc0000000; pppp < (u32*)0xc2000000; pppp++)
//            if(*pppp != (u32)pppp)
//                loop = 100;
//  }
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

    extern s32 ModuleInstall_XIP_APP_FS(u32 opt, void *data);
    ModuleInstall_XIP_APP_FS(0,NULL);

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

    extern bool_t MoudleInit_Systime(ptu32_t para);
    MoudleInit_Systime(0);

    extern bool_t ModuleInstall_HardTimer(void);
    ModuleInstall_HardTimer();

    //-------------------medium-------------------------//
    extern ptu32_t ModuleInstall_Charset(ptu32_t para);
    ModuleInstall_Charset(0);
    extern void ModuleInstall_CharsetNls(const char * DefaultCharset);
    ModuleInstall_CharsetNls("C");

    extern bool_t ModuleInstall_CharsetAscii(void);
    ModuleInstall_CharsetAscii ( );

    extern bool_t ModuleInstall_CharsetGb2312(void);
    ModuleInstall_CharsetGb2312 ( );

    extern s32 ModuleInstall_EFS(const char *target, u32 opt, void *config);
    ModuleInstall_EFS(CFG_EFS_MOUNT_POINT, MS_INSTALLCREAT, CFG_EFS_FILE_BLOCK_SIZE);

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

    extern bool_t ModuleInstall_TcpIp(void);
    ModuleInstall_TcpIp( );

    extern bool_t TcpInit(void);
    TcpInit();

    extern bool_t ModuleInstall_Timer(void);
    ModuleInstall_Timer();

    extern s32 kernel_command(void);
    kernel_command();

    extern void ModuleInstall_InitNet( );
    ModuleInstall_InitNet( );

    extern struct DisplayObj* ModuleInstall_LCD(const char *DisplayName,const char* HeapName);
    ModuleInstall_LCD(CFG_DISPLAY_NAME,CFG_LCD_HEAP_NAME);

    extern u8 RNG_Init(void);
    RNG_Init();

    //-------------------later-------------------------//
    extern void ModuleInstall_Gdd_AND_Desktop(void);
    ModuleInstall_Gdd_AND_Desktop();

    extern s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err);
    ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);

    extern struct GkWinObj;
    extern bool_t ModuleInstall_FT5X26(char *BusName,struct GkWinObj *desktop,char *touch_dev_name );
    struct GkWinObj *desktop;
    desktop = GK_GetDesktop(CFG_DISPLAY_NAME);
    if(NULL == desktop)
    {
    printf("Desktop Not Exist !\r\n");
    }
    else
    {
    ModuleInstall_FT5X26(CFG_FT5X26_BUS_NAME,desktop,CFG_FT5X26_TOUCH_NAME);
    }
    extern bool_t GDD_AddInputDev(const char *InputDevName);
    GDD_AddInputDev(CFG_FT5X26_TOUCH_NAME);

    evtt_main = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
    __djy_main,NULL,CFG_MAINSTACK_LIMIT, "main function");
    //事件的两个参数暂设为0,如果用shell启动,可用来采集shell命令行参数
    Djy_EventPop(evtt_main,NULL,0,NULL,0,0);

    extern bool_t Heap_DynamicModuleInit(void);
    Heap_DynamicModuleInit ( );

    return ;
}

