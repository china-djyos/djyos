#include "sys_rtos.h"
#include "error.h"

#include "include.h"
#include "mem_pub.h"
#include "uart_pub.h"
#include "str_pub.h"
#include "sys_ctrl_pub.h"
#include "gpio_pub.h"
#include "usb_pub.h"
#include "net_param_pub.h"
#include "fake_clock_pub.h"
#include "board.h"

#if (CFG_SOC_NAME == SOC_BK7221U)
#if CFG_USE_USB_CHARGE
UINT32 charge_elect = 530;
UINT32 charge_func_init = 0;
UINT32 last_second;
static int charge_started = 0;  //1 -- 正在充电；0 -- 当前未在充电
extern int vbat_voltage_get(void);
int Get_StabilizeVol(void)
{
    int vol;
    if(charge_started == 1)
    {
        usb_charge_stop();
//                printf("停止充电.\r\n");
        bk_rtos_delay_milliseconds(6000);
    }
    vol = vbat_voltage_get();
    if(charge_started == 1)
    {
        usb_charge_start();
//                printf("开始充电.\r\n");
    }
    return vol;
}

UINT32 usb_charge_oper_val(UINT32 elect)
{
    if(elect < 450)
        elect = 450;
    else if(elect > 750)
        elect = 750;

    return (elect - 450) / 20;
}

void usb_charge_start()
{
    CHARGE_OPER_ST chrg;
    chrg.type = 0;
    if(!get_info_item(CHARGE_CONFIG_ITEM, (UINT8 *)chrg.cal, NULL, NULL))
    {
    }
    else
    {
        printf("load charge cal %x %x %x\r\n", chrg.cal[0], chrg.cal[1], chrg.cal[2]);
    }
    if(chrg.type == 1)
    {
        chrg.oper = usb_charge_oper_val(charge_elect);
    }
    else
    {
        chrg.oper = 0x1f;
    }
    sddev_control(SCTRL_DEV_NAME, CMD_SCTRL_USB_CHARGE_START, (void *)&chrg);
}

void usb_charge_stop()
{
    UINT32 charge_type = 1;
    sddev_control(SCTRL_DEV_NAME, CMD_SCTRL_USB_CHARGE_STOP, &charge_type);
}

void usb_charge_check_cb(void)
{
    UINT32 tmp;
    int vol;

    if(charge_func_init && usb_is_pluged())
    {
//        tmp = fclk_get_second();
//        if(tmp > last_second)
//        {
            vol = vbat_voltage_get();
            /*check per second,if charge full*/
//          printf("内部充电检测到的电压 = %d.   charge_started = %d.\r\n",vol,charge_started);
            if(charge_started == 1 && vol > 4300)
            {
                usb_charge_stop();
                charge_started = 0;
                printf("charger_is_full %d\r\n",vol);
            }
            else if(charge_started == 0 && vol < 4160)
            {
                usb_charge_start();
                charge_started = 1;
                printf("charger_start %d\r\n",vol);
            }

//            last_second = tmp;
//        }
    }
    else
    {
//      printf("检测到拔掉充电器时 charge_started = %d.\r\n",charge_started);
        if(charge_started == 1)
        {
            usb_charge_stop();
            charge_started = 0;
            printf("Charger removal.\r\n");
        }
    }

}

void usb_charger_calibration(UINT32 type)
{
    CHARGE_OPER_ST chrg;
    chrg.oper = 1;

    sddev_control(SCTRL_DEV_NAME, CMD_SCTRL_USB_CHARGE_CAL, &chrg);
    chrg.oper = 2;
    //sddev_control(SCTRL_DEV_NAME, CMD_SCTRL_USB_CHARGE_CAL, &chrg);
    chrg.oper = 3;
    sddev_control(SCTRL_DEV_NAME, CMD_SCTRL_USB_CHARGE_CAL, &chrg);
    chrg.oper = 4;
    sddev_control(SCTRL_DEV_NAME, CMD_SCTRL_USB_CHARGE_CAL, &chrg);
    if(chrg.cal[0] && chrg.cal[1] && chrg.cal[2])
    {
        printf("save charge cal %x %x %x\r\n", chrg.cal[0], chrg.cal[1], chrg.cal[2]);
        save_info_item(CHARGE_CONFIG_ITEM, chrg.cal, NULL, NULL);
    }
}
#endif
void usb_plug_func_handler(void *usr_data, UINT32 event)
{
    UINT32 charge_type = 1;

    switch(event)
    {
    case USB_PLUG_IN_EVENT:
#if CFG_USE_USB_CHARGE
        usb_charge_start();
#endif
        break;

    case USB_PLUG_OUT_EVENT:
#if CFG_USE_USB_CHARGE
        usb_charge_stop();
#endif
        break;

    case USB_PLUG_NO_EVENT:

        break;

    default:
        break;
    }

}


void usb_plug_func_open(void)
{
    DD_HANDLE usb_plug_hdl;
    UINT32 status;
    USB_PLUG_INOUT_ST user_plug;

    user_plug.handler = usb_plug_func_handler;
    user_plug.usr_data = 0;

    usb_plug_hdl = ddev_open(USB_PLUG_DEV_NAME, &status, (UINT32)&user_plug);
    if(DD_HANDLE_UNVALID == usb_plug_hdl)
    {
        return;
    }

#if CFG_USE_USB_CHARGE
    charge_func_init = 1;
#endif
}


#endif

void rt_user_idle_hook(void)
{
    #if CFG_USE_USB_CHARGE
    #if (CFG_SOC_NAME == SOC_BK7221U)
        usb_charge_check_cb();
    #endif
    #endif
}


