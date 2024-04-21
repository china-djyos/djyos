/*
 * FreeModbus Libary: RT-Thread Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbport.h"
#include <timer_hard.h>

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- Variables ----------------------------------------*/
static u64 usT35TimeOut = 0;
static u64 usDelayTimeOut = 0;
static u64 usRespondTimeOut = 0;
static u32 MasterTimerTimeoutISR(ptu32_t TimerHandle);
static ptu32_t TimerHandle = 0;
static struct SemaphoreLCB   xMasterTimeoutLock;
bool_t __STM32Timer_Time2Counter(u64 a, u64* b);
/* ----------------------- static functions ---------------------------------*/
static ptu32_t prvvTIMER_MasterExpired(void);
static u8 MasterTimerStack[0x800];
/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortTimersInit(USHORT usTimeOut50us)
{
    u16 EvttID = 0;

    if (Lock_SempCreate_s(&xMasterTimeoutLock, 1, 0, CN_BLOCK_PRIO, "master timeout"))
    {
        TimerHandle = HardTimer_Alloc(MasterTimerTimeoutISR);
        if (TimerHandle)
        {
            __STM32Timer_Time2Counter(50 * usTimeOut50us, &usT35TimeOut);
            __STM32Timer_Time2Counter(MB_MASTER_DELAY_MS_CONVERT * 1000, &usDelayTimeOut);
            __STM32Timer_Time2Counter(MB_MASTER_TIMEOUT_MS_RESPOND * 1000, &usRespondTimeOut);

            HardTimer_Ctrl(TimerHandle, EN_TIMER_SETINTPRO, false);
            HardTimer_Ctrl(TimerHandle, EN_TIMER_SETRELOAD, false);
            HardTimer_Ctrl(TimerHandle, EN_TIMER_SETCYCLE, (u32)usT35TimeOut);
            HardTimer_Ctrl(TimerHandle, EN_TIMER_ENINT,true);           //中断使能

            EvttID = DJY_EvttRegist(EN_INDEPENDENCE, 191, 0, 1, (void *)prvvTIMER_MasterExpired, 
                                    MasterTimerStack, sizeof(MasterTimerStack), "master timeout");
            if (CN_EVTT_ID_INVALID != EvttID)
            {
                if (CN_EVENT_ID_INVALID != DJY_EventPop(EvttID, NULL, 0, 0, 0, 0))
                {
                    return TRUE;
                }
                else
                {
                    error_printf("modbus", "master poll event pop fail\r\n");
                }
                DJY_EvttUnregist(EvttID);
            }
            else
            {
                error_printf("modbus", "master poll Evtt regist fail\r\n");
            }

            HardTimer_Free(TimerHandle);
        }
        else
        {
            error_printf("modbus", "master timer alloc fail\r\n");
        }
        Lock_SempDelete_s(&xMasterTimeoutLock);
    }
    else
    {
        error_printf("modbus", "xMasterTimeoutLock semp create fail\r\n");
    }


    return FALSE;
}

void vMBMasterPortTimersT35Enable()
{
    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_T35);

    HardTimer_Ctrl(TimerHandle,EN_TIMER_SETCYCLE, (u32)usT35TimeOut);

    HardTimer_Ctrl(TimerHandle,EN_TIMER_STARTCOUNT,0);         //开始定时器
}

void vMBMasterPortTimersConvertDelayEnable()
{
    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_CONVERT_DELAY);   //设置当前定时器模式

    HardTimer_Ctrl(TimerHandle,EN_TIMER_SETCYCLE, (u32)usDelayTimeOut);

    HardTimer_Ctrl(TimerHandle,EN_TIMER_STARTCOUNT,0);         //开始定时器
}

void vMBMasterPortTimersRespondTimeoutEnable()
{
    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_RESPOND_TIMEOUT); //设置当前定时器模式

    HardTimer_Ctrl(TimerHandle,EN_TIMER_SETCYCLE, (u32)usRespondTimeOut);

    HardTimer_Ctrl(TimerHandle,EN_TIMER_STARTCOUNT,0);         //开始定时器

}

void vMBMasterPortTimersDisable()
{
    HardTimer_Ctrl(TimerHandle,EN_TIMER_PAUSECOUNT,0);         //停止定时器
}

ptu32_t prvvTIMER_MasterExpired(void)
{
    while (1)
    {
        Lock_SempPend(&xMasterTimeoutLock, CN_TIMEOUT_FOREVER);

        (void) pxMBMasterPortCBTimerExpired();      //超时了
    }

    return 0;
}

static u32 MasterTimerTimeoutISR(ptu32_t TimerHandle)
{
    UNUSED(TimerHandle);
    // 为了支持单周期，437的定时器，第一次定时不准，
    // 如果把定时器设置成单周期的，就没法用的。这里取巧变成单周期
    HardTimer_Ctrl(TimerHandle,EN_TIMER_PAUSECOUNT,0);         //停止计数
    Lock_SempPost(&xMasterTimeoutLock);
    return 0;
}

#endif
