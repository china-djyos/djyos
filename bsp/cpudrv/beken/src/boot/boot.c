#include <Iboot_Info.h>
#include <sys_config.h>
#include <wdt_pub.h>
#include <power_save_pub.h>
#include <arch.h>
#include <drv_model_pub.h>
#include <co_math.h>

extern void Load_Preload(void);


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
    sddev_control(WDT_DEV_NAME, WCMD_POWER_UP, NULL);
#if CFG_USE_STA_PS
    }
    GLOBAL_INT_RESTORE();
#endif
}

void reboot(u32 key)
{
    Set_RebootFlag();
#if (CFG_RUNMODE_BAREAPP == 0)
    Set_PreviouResetFlag();
#endif
    bk_reboot();
}


void reset(u32 key)
{
    Set_SoftResetFlag();
#if (CFG_RUNMODE_BAREAPP == 0)
    Set_PreviouResetFlag();
#endif
//    void (*fn_start)();
//    fn_start = 0x0;
//    fn_start();
//  _start();
    bk_reboot();
}

void restart_app(u32 key)
{
    Set_RestartAppFlag();
#if (CFG_RUNMODE_BAREAPP == 0)
    Set_PreviouResetFlag();
#endif
    Load_Preload();
}
