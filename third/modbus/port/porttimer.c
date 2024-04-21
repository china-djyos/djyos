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
 * File: $Id: porttimer.c,v 1.60 2013/08/13 15:07:05 Armink $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include <timer_hard.h>
/* ----------------------- static functions ---------------------------------*/
static u32 TimerTimeoutISR(ptu32_t TimerHandle);
static struct SemaphoreLCB   xSlaveTimeoutLock;
static ptu32_t TimerHandle = 0;
static ptu32_t prvvTIMERExpired(void);
static u8 SlaveTimerStack[0x800];
/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{
    u64 Timeout = 0;
    u16 EvttID = 0;

    if (Lock_SempCreate_s(&xSlaveTimeoutLock, 1, 0, CN_BLOCK_PRIO, "slave timeout"))
    {
        TimerHandle = HardTimer_Alloc(TimerTimeoutISR);
        if (TimerHandle)
        {
            __STM32Timer_Time2Counter(50 * usTim1Timerout50us, &Timeout);
            HardTimer_Ctrl(TimerHandle, EN_TIMER_SETINTPRO, false);
            HardTimer_Ctrl(TimerHandle, EN_TIMER_SETRELOAD, false);
            HardTimer_Ctrl(TimerHandle, EN_TIMER_SETCYCLE, Timeout);
            HardTimer_Ctrl(TimerHandle, EN_TIMER_ENINT,true);           //中断使能
            EvttID = DJY_EvttRegist(EN_INDEPENDENCE, 191, 0, 1, (void *)prvvTIMERExpired, 
                                    SlaveTimerStack, sizeof(SlaveTimerStack), "slave timeout");
            if (CN_EVTT_ID_INVALID != EvttID)
            {
                if (CN_EVENT_ID_INVALID != DJY_EventPop(EvttID, NULL, 0, 0, 0, 0))
                {
                    return TRUE;
                }
                else
                {
                    error_printf("modbus", "slave poll event pop fail\r\n");
                }
                DJY_EvttUnregist(EvttID);
            }
            else
            {
                error_printf("modbus", "slave poll Evtt regist fail\r\n");
            }

            HardTimer_Free(TimerHandle);
        }
        else
        {
            error_printf("modbus", "slave timer alloc fail\r\n");
        }
        Lock_SempDelete_s(&xSlaveTimeoutLock);
    }
    else
    {
        error_printf("modbus", "xSlaveTimeoutLock semp create fail\r\n");
    }

    return FALSE;
}

void vMBPortTimersEnable()
{
    HardTimer_Ctrl(TimerHandle,EN_TIMER_STARTCOUNT,0);         //开启定时器
}

void vMBPortTimersDisable()
{
    HardTimer_Ctrl(TimerHandle,EN_TIMER_PAUSECOUNT,0);         //停止定时器
}

ptu32_t prvvTIMERExpired(void)
{
    while (1)
    {
        Lock_SempPend(&xSlaveTimeoutLock, CN_TIMEOUT_FOREVER);
        (void) pxMBPortCBTimerExpired();        //执行超时回调函数.定时器时间到了,就是超时了
    }

    return 0;
}

static u32 TimerTimeoutISR(ptu32_t TimerHandle)
{
    UNUSED(TimerHandle);
    // 为了支持单周期，437的定时器，第一次定时不准，
    // 如果把定时器设置成单周期的，就没法用的。这里取巧变成单周期
    HardTimer_Ctrl(TimerHandle,EN_TIMER_PAUSECOUNT,0);         //停止定时器
    Lock_SempPost(&xSlaveTimeoutLock);
    return 0;
}
