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
  * File: $Id: port.c,v 1.60 2015/02/01 9:18:05 Armink $
  */

/* ----------------------- System includes --------------------------------*/

/* ----------------------- Modbus includes ----------------------------------*/
#include "port.h"
/* ----------------------- Variables ----------------------------------------*/
static struct SemaphoreLCB lock;
static int is_inited = 0;
/* ----------------------- Start implementation -----------------------------*/
void EnterCriticalSection(void)
{
    if(!is_inited)
    {
        if (NULL == Lock_SempCreate_s(&lock, 1, 1, CN_BLOCK_PRIO, "fmb_lock"))
        {
            error_printf("modbus", "semp create fail\r\n");
        }
        else
        {
            is_inited = 1;
        }
    }
    if (is_inited)
    {
        // 申请信号量
        Lock_SempPend(&lock, CN_TIMEOUT_FOREVER);
    }
}

void ExitCriticalSection(void)
{
    if (is_inited)
    {
        // 释放信号量
        Lock_SempPost(&lock);
    }
}

