#include <Iboot_Info.h>
#include <sys_config.h>
#include <wdt_pub.h>
#include <power_save_pub.h>
//#include <arch.h>
#include <drv_model_pub.h>
#include <co_math.h>

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip boot"//CPU boot功能
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:required              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_BOOT == false )
//#warning  " boot  组件参数未配置，使用默认配置"
//%$#@target = header   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_BOOT    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@enum,false,true,
#define CFG_POWER_ON_RESET_TO_BOOT    false //控制上电复位后是否强制运行iboot
//%$#@end configue  ****参数配置结束
#endif
//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束
//@#$%component end configure

extern void Iboot_LoadPreload(void);
//void WrResetToReg(void);

static void bk_reboot(void)
{
    u32 wdt_val = 1;

#if CFG_USE_STA_PS
    GLOBAL_INT_DECLARATION();

    GLOBAL_INT_DISABLE();
    if(power_save_if_ps_rf_dtim_enabled()
        && power_save_if_rf_sleep())
    {
        power_save_wkup_event_set(NEED_DISABLE_BIT | NEED_REBOOT_BIT);
    }
    else
    {
#endif

//    printf("wdt reboot\r\n");
    sddev_control(WDT_DEV_NAME, WCMD_SET_PERIOD, &wdt_val);
//    WrResetToReg();
    sddev_control(WDT_DEV_NAME, WCMD_POWER_UP, NULL);
//    DJY_DelayUs(100000);
#if CFG_USE_STA_PS
    }
    GLOBAL_INT_RESTORE();
#endif
}

void CPU_Reboot(void)
{
    Iboot_SetRebootFlag();
#if (CFG_RUNMODE_BAREAPP == 0)
    Iboot_SetPreviouResetFlag();
#endif
//  bk_reboot();
    void (*fn_start)();
    fn_start = 0x0;
    fn_start();
}


void CPU_Reset(void)
{
    Iboot_SetSoftResetFlag();
#if (CFG_RUNMODE_BAREAPP == 0)
    Iboot_SetPreviouResetFlag();
#endif
//    void (*fn_start)();
//    fn_start = 0x0;
//    fn_start();
//  _start();
    bk_reboot();
}

void CPU_RestartSystem(void)
{
    Iboot_SetRestartAppFlag();
#if (CFG_RUNMODE_BAREAPP == 0)
    Iboot_SetPreviouResetFlag();
#endif
    AppStart();
//  void (*fn_start)();
//  fn_start = 0x0;
//  fn_start();
}
