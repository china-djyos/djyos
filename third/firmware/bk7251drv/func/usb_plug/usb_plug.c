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
char vol_to_percentage(int assign)
{
    int vol;
    char percentage = 0;
    if(assign == 0)
    {
        if(usb_is_pluged() != 0)
        {
            if(charge_started == 1)
            {
                usb_charge_stop();
//                printf("停止充电.\r\n");
                Djy_EventDelay(6000*1000);
            }
            vol = vbat_voltage_get();
            if(charge_started == 1)
            {
                usb_charge_start();
//                printf("开始充电.\r\n");
            }
            vol -= 80;
        }
        else
        {
            vol = vbat_voltage_get();
        }
//        printf("测量到的Vbat = %d  \r\n", vol);    //显示当前的电池电压
    }
    else
    {
        vol = assign;
//        printf("指定的Vbat = %d  \r\n", vol);    //显示当前的电池电压
    }
    if(vol >= 3930)
    {
        if(vol >= 4160)
            percentage = 100;
        else if(vol >= 4150)
            percentage = 99;
        else if(vol >= 4145)
            percentage = 98;
        else if(vol >= 4140)
            percentage = 97;
        else if(vol >= 4130)
            percentage = 96;
        else if(vol >= 4120)
            percentage = 95;
        else if(vol >= 4115)
            percentage = 94;
        else if(vol >= 4110)
            percentage = 93;
        else if(vol >= 4100)
            percentage = 92;
        else if(vol >= 4100)
            percentage = 92;
        else if(vol >= 4090)
            percentage = 91;
        else if(vol >= 4080)
            percentage = 90;
        else if(vol >= 4070)
            percentage = 89;
        else if(vol >= 4060)
            percentage = 88;
        else if(vol >= 4050)
            percentage = 87;
        else if(vol >= 4040)
            percentage = 86;
        else if(vol >= 4030)
            percentage = 85;
        else if(vol >= 4020)
            percentage = 84;
        else if(vol >= 4015)
            percentage = 83;
        else if(vol >= 4010)
            percentage = 82;
        else if(vol >= 4005)
            percentage = 81;
        else if(vol >= 3970)
            percentage = 80;
        else if(vol >= 3960)
            percentage = 79;
        else if(vol >= 3950)
            percentage = 78;
        else if(vol >= 3945)
            percentage = 77;
        else if(vol >= 3940)
            percentage = 76;
        else if(vol >= 3930)
            percentage = 75;
    }
    else if(vol >= 3790)
    {
        if(vol >= 3925)
            percentage = 74;
        else if(vol >= 3920)
            percentage = 73;
        else if(vol >= 3915)
            percentage = 72;
        else if(vol >= 3908)
            percentage = 71;
        else if(vol >= 3900)
            percentage = 70;
        else if(vol >= 3890)
            percentage = 69;
        else if(vol >= 3885)
            percentage = 68;
        else if(vol >= 3880)
            percentage = 67;
        else if(vol >= 3875)
            percentage = 66;
        else if(vol >= 3870)
            percentage = 65;
        else if(vol >= 3865)
            percentage = 64;
        else if(vol >= 3860)
            percentage = 63;
        else if(vol >= 3855)
            percentage = 62;
        else if(vol >= 3848)
            percentage = 61;
        else if(vol >= 3840)
            percentage = 60;
        else if(vol >= 3835)
            percentage = 59;
        else if(vol >= 3830)
            percentage = 58;
        else if(vol >= 3825)
            percentage = 57;
        else if(vol >= 3818)
            percentage = 56;
        else if(vol >= 3810)
            percentage = 55;
        else if(vol >= 3805)
            percentage = 54;
        else if(vol >= 3800)
            percentage = 53;
        else if(vol >= 3797)
            percentage = 52;
        else if(vol >= 3793)
            percentage = 51;
        else if(vol >= 3790)
            percentage = 50;
    }
    else if(vol >= 3720)
    {
        if(vol >= 3786)
            percentage = 49;
        else if(vol >= 3782)
            percentage = 48;
        else if(vol >= 3778)
            percentage = 47;
        else if(vol >= 3774)
            percentage = 46;
        else if(vol >= 3770)
            percentage = 45;
        else if(vol >= 3767)
            percentage = 44;
        else if(vol >= 3763)
            percentage = 43;
        else if(vol >= 3760)
            percentage = 42;
        else if(vol >= 3757)
            percentage = 41;
        else if(vol >= 3754)
            percentage = 40;
        else if(vol >= 3751)
            percentage = 39;
        else if(vol >= 3748)
            percentage = 38;
        else if(vol >= 3745)
            percentage = 37;
        else if(vol >= 3743)
            percentage = 36;
        else if(vol >= 3740)
            percentage = 35;
        else if(vol >= 3738)
            percentage = 34;
        else if(vol >= 3736)
            percentage = 33;
        else if(vol >= 3734)
            percentage = 32;
        else if(vol >= 3732)
            percentage = 31;
        else if(vol >= 3730)
            percentage = 30;
        else if(vol >= 3728)
            percentage = 29;
        else if(vol >= 3726)
            percentage = 28;
        else if(vol >= 3724)
            percentage = 27;
        else if(vol >= 3722)
            percentage = 26;
        else if(vol >= 3720)
            percentage = 25;
    }
    else if(vol >= 3590)
    {
        if(vol >= 3718)
            percentage = 24;
        else if(vol >= 3716)
            percentage = 23;
        else if(vol >= 3714)
            percentage = 22;
        else if(vol >= 3712)
            percentage = 21;
        else if(vol >= 3710)
            percentage = 20;
        else if(vol >= 3706)
            percentage = 19;
        else if(vol >= 3702)
            percentage = 18;
        else if(vol >= 3698)
            percentage = 17;
        else if(vol >= 3694)
            percentage = 16;
        else if(vol >= 3690)
            percentage = 15;
        else if(vol >= 3680)
            percentage = 14;
        else if(vol >= 3670)
            percentage = 13;
        else if(vol >= 3660)
            percentage = 12;
        else if(vol >= 3655)
            percentage = 11;
        else if(vol >= 3650)
            percentage = 10;
        else if(vol >= 3645)
            percentage = 9;
        else if(vol >= 3640)
            percentage = 8;
        else if(vol >= 3637)
            percentage = 7;
        else if(vol >= 3633)
            percentage = 6;
        else if(vol >= 3630)
            percentage = 5;
        else if(vol >= 3620)
            percentage = 4;
        else if(vol >= 3610)
            percentage = 3;
        else if(vol >= 3600)
            percentage = 2;
        else if(vol >= 3590)
            percentage = 1;
    }
    else if((vol < 3590) && (usb_is_pluged() == 0))
    {
        if(vol >= 2700)     //防止一些异常电压导致休眠
        {
            printf("电量不足，准备关机\r\n");
            Djy_EventDelay(3000*1000);
            CloseScreen();
            LP_DeepSleep();
        }
        else
        {
            percentage = -1; //异常电压，返回这个，电量显示不做响应。
        }
    }
    return percentage;

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
//    DD_HANDLE usb_plug_hdl;
//    UINT32 status;
//    USB_PLUG_INOUT_ST user_plug;
//
//    user_plug.handler = usb_plug_func_handler;
//    user_plug.usr_data = 0;
//
//    usb_plug_hdl = ddev_open(USB_PLUG_DEV_NAME, &status, (UINT32)&user_plug);
//    if(DD_HANDLE_UNVALID == usb_plug_hdl)
//    {
//        return;
//    }
    
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


