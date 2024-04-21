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
 * File: $Id: portevent.c,v 1.60 2013/08/13 15:07:05 Armink $
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Variables ----------------------------------------*/
static eMBEventType     xSlaveOsEvent;
static struct SemaphoreLCB   xSlaveEventLock;
/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortEventInit( void )
{
    if (NULL == Lock_SempCreate_s(&xSlaveEventLock, 1, 0, CN_BLOCK_PRIO, "slave event"))
    {
        error_printf("modbus", "xSlaveEventLock semp create fail\r\n");
        return FALSE;
    }
    return TRUE;
}

BOOL
xMBPortEventPost( eMBEventType eEvent )
{
    // 这里不用和主机一样加个所保护，是因为，从机没有主动发请求的操作，所有的数据处理都是协议自己处理
    // 在设置任务优先级的时候已经规避了，会覆盖的情况
    xSlaveOsEvent = eEvent;
    Lock_SempPost(&xSlaveEventLock);
    return TRUE;
}

BOOL
xMBPortEventGet( eMBEventType * eEvent )
{
    Lock_SempPend(&xSlaveEventLock, CN_TIMEOUT_FOREVER);
    switch (xSlaveOsEvent)
    {
    case EV_READY:
        *eEvent = EV_READY;
        break;
    case EV_FRAME_RECEIVED:
        *eEvent = EV_FRAME_RECEIVED;
        break;
    case EV_EXECUTE:
        *eEvent = EV_EXECUTE;
        break;
    case EV_FRAME_SENT:
        *eEvent = EV_FRAME_SENT;
        break;
    }
    return TRUE;
}
