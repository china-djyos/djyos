/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#include "project_config.h"
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

    extern void Board_GpioInit(void);
    Board_GpioInit();

    extern void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut);
    Stdio_KnlInOutInit(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME);
    extern s32 ModuleInstall_Shell(ptu32_t para);
    ModuleInstall_Shell(0);

    //-------------------early-------------------------//
//  extern void ModuleInstall_BlackBox(void);
//  ModuleInstall_BlackBox( );

    extern s32 ModuleInstall_XIP_APP_FS(u32 opt, void *data);
    ModuleInstall_XIP_APP_FS(0,NULL);

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

    extern bool_t ModuleInstall_MsgQ(void);
    ModuleInstall_MsgQ ( );

    extern ptu32_t ModuleInstall_IAP(void);
    ModuleInstall_IAP( );

    s32 ModuleInstall_EmbededFlash(u32 bstart, u32 bend, u32 doformat);

    ModuleInstall_EmbededFlash(CFG_EFLASH_PART_END, CFG_EFLASH_PART_FORMAT);

    extern bool_t ModuleInstall_HardTimer(void);
    ModuleInstall_HardTimer();

    //-------------------medium-------------------------//
    #if(CFG_OS_TINY == flase)
        extern s32 kernel_command(void);
        kernel_command();
    #endif

    extern bool_t ModuleInstall_Timer(void);
    ModuleInstall_Timer();

    extern bool_t ModuleInstall_Ymodem(void);
    ModuleInstall_Ymodem();
    bool_t ModuleInstall_App();
    ModuleInstall_App();
    //-------------------later-------------------------//
    extern s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err);
    ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);

    evtt_main = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
    __djy_main,NULL,CFG_MAINSTACK_LIMIT, "main function");
    //�¼���������������Ϊ0,�����shell����,�������ɼ�shell�����в���
    Djy_EventPop(evtt_main,NULL,0,NULL,0,0);

    extern bool_t Heap_DynamicModuleInit(void);
    Heap_DynamicModuleInit ( );

    return ;
}

