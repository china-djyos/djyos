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

    extern bool_t ModuleInstall_Multiplex(void);
    ModuleInstall_Multiplex ();

    #if !defined (CFG_RUNMODE_BAREAPP)
    extern ptu32_t ModuleInstall_IAP(void);
    ModuleInstall_IAP( );
    #endif

    bool_t IIC_Init(u8 iic_port);
    #if CFG_IIC1_ENABLE== true
    ModuleInstall_IIC(CN_IIC1);
    #endif
    #if CFG_IIC2_ENABLE== true
    ModuleInstall_IIC(CN_IIC2);
    #endif
    #if CFG_IIC3_ENABLE== true
    ModuleInstall_IIC(CN_IIC3);
    #endif
    #if CFG_IIC4_ENABLE== true
    ModuleInstall_IIC(CN_IIC4);
    #endif

    extern ptu32_t ModuleInstall_UART(u32 serial_no,u32 SendBufLen, u32 RecvBufLen,u8 mode);
    #if CFG_UART1_ENABLE ==1
    ModuleInstall_UART(CN_UART1,CFG_UART1_SENDBUF_LEN,CFG_UART1_RECVBUF_LEN,CFG_UART1_MODE);
    #endif
    #if CFG_UART2_ENABLE ==1
    ModuleInstall_UART(CN_UART2,CFG_UART2_SENDBUF_LEN,CFG_UART2_RECVBUF_LEN,CFG_UART2_MODE);
    #endif
    #if CFG_UART3_ENABLE ==1
    ModuleInstall_UART(CN_UART3,CFG_UART3_SENDBUF_LEN,CFG_UART3_RECVBUF_LEN,CFG_UART3_MODE);
    #endif
    #if CFG_UART4_ENABLE ==1
    ModuleInstall_UART(CN_UART4,CFG_UART4_SENDBUF_LEN,CFG_UART4_RECVBUF_LEN,CFG_UART4_MODE);
    #endif
    #if CFG_UART5_ENABLE ==1
    ModuleInstall_UART(CN_UART5,CFG_UART5_SENDBUF_LEN,CFG_UART5_RECVBUF_LEN,CFG_UART5_MODE);
    #endif

    //-------------------medium-------------------------//
    #if(CFG_OS_TINY == flase)
    extern s32 kernel_command(void);
    kernel_command();
    #endif

    //-------------------later-------------------------//
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

