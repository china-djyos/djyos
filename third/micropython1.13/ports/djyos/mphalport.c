/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Armink (armink.ztl@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <string.h>

#include <djyos.h>
#include <systime.h>
#include <py/mpconfig.h>
#include <py/runtime.h>
#include <py/mphal.h>
#include "mphalport.h"

void mp_hal_stdio_deinit(u32 time)
{
    fcntl(STDIN_FILENO, F_SETTIMEOUT, CN_TIMEOUT_FOREVER);
}

u32 mp_hal_stdio_init(void)
{
    u32 result;
    fcntl(STDIN_FILENO, F_GETTIMEOUT,&result);
    fcntl(STDIN_FILENO, F_SETTIMEOUT, 0);
    return result;
}

// Receive single character
// 务必先调用 mp_hal_stdio_init 把stdin阻塞时间设为0.
int mp_hal_stdin_rx_chr(void)
{
    s32 res;
    while (1)
    {
        res = getchar( );
        if(res == EOF)
            mp_hal_delay_ms(10);
        else
            break;
    }
//    MP_THREAD_GIL_EXIT();
//    res = getchar( );
//    MP_THREAD_GIL_ENTER();
    return res;
}

// Send string of given length
void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    write(STDOUT_FILENO, str, len);
}

mp_uint_t mp_hal_ticks_us(void) {
    return DJY_GetSysTime();
}

mp_uint_t mp_hal_ticks_ms(void) {
    return DJY_GetSysTime() * 1000 ;
}

extern s64 __DJY_GetSysTick(void);
mp_uint_t mp_hal_ticks_cpu(void) {
    return (mp_uint_t)__DJY_GetSysTick();
}

void mp_hal_delay_us(mp_uint_t us)
{
    DJY_DelayUs(us);
}

void mp_hal_delay_ms(mp_uint_t ms)
{
//  mp_uint_t res;
//  s64 start;
//  start = DJY_GetSysTime();
//  for (res = ms; res >0; res--)
//  {
//      DJY_EventDelay(1);
//      MICROPY_EVENT_POLL_HOOK;
//      if ((mp_uint_t)((DJY_GetSysTime() - start) / 1000) >= ms)
//          break;
//  }
    MP_THREAD_GIL_EXIT();
    DJY_EventDelay(ms * mS);
    MP_THREAD_GIL_ENTER();
}
