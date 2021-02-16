/*
 * modutime.c
 *
 *  Created on: 2021.2.3
 *      Author: WangXi
 */

/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
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

#include "py/runtime.h"
#include "py/smallint.h"
#include "py/obj.h"
#include "lib/timeutils/timeutils.h"
#include "extmod/utime_mphal.h"

#include <time.h>

/// \module time - time related functions
///
/// The `time` module provides functions for getting the current time and date,
/// and for sleeping.

/// \function localtime([secs])
/// Convert a time expressed in seconds since Jan 1, 2000 into an 8-tuple which
/// contains: (year, month, mday, hour, minute, second, weekday, yearday)
/// If secs is not provided or None, then the current time from the RTC is used.
/// year includes the century (for example 2014)
/// month   is 1-12
/// mday    is 1-31
/// hour    is 0-23
/// minute  is 0-59
/// second  is 0-59
/// weekday is 0-6 for Mon-Sun.
/// yearday is 1-366
STATIC mp_obj_t time_localtime(size_t n_args, const mp_obj_t *args) {
    struct tm tmSt;
    if (n_args == 0 || args[0] == mp_const_none) {
        // get current date and time
        // note: need to call get time then get date to correctly access the registers

        // fixme
        Time_LocalTime_r(NULL, &tmSt);

        mp_obj_t tuple[8] = {
            mp_obj_new_int(1900 + tmSt.tm_year),
            mp_obj_new_int(tmSt.tm_mon),
            mp_obj_new_int(tmSt.tm_mday),
            mp_obj_new_int(tmSt.tm_hour),
            mp_obj_new_int(tmSt.tm_min),
            mp_obj_new_int(tmSt.tm_sec),
            mp_obj_new_int(tmSt.tm_wday - 1),
            mp_obj_new_int(tmSt.tm_yday),
        };
        return mp_obj_new_tuple(8, tuple);
    } else {
        mp_int_t seconds = mp_obj_get_int(args[0]);
        Time_LocalTime_r(seconds, &tmSt);
        mp_obj_t tuple[8] = {
            tuple[0] = mp_obj_new_int(tmSt.tm_year),
            tuple[1] = mp_obj_new_int(tmSt.tm_mon),
            tuple[2] = mp_obj_new_int(tmSt.tm_mday),
            tuple[3] = mp_obj_new_int(tmSt.tm_hour),
            tuple[4] = mp_obj_new_int(tmSt.tm_min),
            tuple[5] = mp_obj_new_int(tmSt.tm_sec),
            tuple[6] = mp_obj_new_int(tmSt.tm_wday),
            tuple[7] = mp_obj_new_int(tmSt.tm_yday),
        };
        return mp_obj_new_tuple(8, tuple);
    }
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(time_localtime_obj, 0, 1, time_localtime);


/// \function mktime()
/// This is inverse function of localtime. It's argument is a full 8-tuple
/// which expresses a time as per localtime. It returns an integer which is
/// the number of seconds since Jan 1, 2000.
STATIC mp_obj_t time_mktime(mp_obj_t tuple) {

    size_t len;
    mp_obj_t *elem;
    struct tm tmSt;

    mp_obj_get_array(tuple, &len, &elem);

    // localtime generates a tuple of len 8. CPython uses 9, so we accept both.
    if (len < 8 || len > 9) {
        mp_raise_msg_varg(&mp_type_TypeError, MP_ERROR_TEXT("mktime needs a tuple of length 8 or 9 (%d given)"), len);
    }
    memset(&tmSt, 0, sizeof(tmSt));
    tmSt.tm_year = mp_obj_get_int(elem[0]);
    tmSt.tm_mon = mp_obj_get_int(elem[1]);
    tmSt.tm_mday = mp_obj_get_int(elem[2]);
    tmSt.tm_hour = mp_obj_get_int(elem[3]);
    tmSt.tm_min = mp_obj_get_int(elem[4]);
    tmSt.tm_sec = mp_obj_get_int(elem[5]);

    // fixme
    tmSt.tm_year -= 100;
    return mp_obj_new_int_from_uint(Time_MkTime(&tmSt));
}
MP_DEFINE_CONST_FUN_OBJ_1(time_mktime_obj, time_mktime);

/// \function time()
/// Returns the number of seconds, as an integer, since 1/1/2000.
STATIC mp_obj_t time_time(void) {
    // get date and time
    // note: need to call get time then get date to correctly access the registers
    // fixme
    s64 ret = 0;
    Time_Time(&ret);
    return mp_obj_new_int(ret);
}
MP_DEFINE_CONST_FUN_OBJ_0(time_time_obj, time_time);

STATIC const mp_rom_map_elem_t time_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_utime) },

    { MP_ROM_QSTR(MP_QSTR_localtime), MP_ROM_PTR(&time_localtime_obj) },
    { MP_ROM_QSTR(MP_QSTR_mktime), MP_ROM_PTR(&time_mktime_obj) },
    { MP_ROM_QSTR(MP_QSTR_time), MP_ROM_PTR(&time_time_obj) },

    { MP_ROM_QSTR(MP_QSTR_sleep), MP_ROM_PTR(&mp_utime_sleep_obj) },
    { MP_ROM_QSTR(MP_QSTR_sleep_ms), MP_ROM_PTR(&mp_utime_sleep_ms_obj) },
    { MP_ROM_QSTR(MP_QSTR_sleep_us), MP_ROM_PTR(&mp_utime_sleep_us_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_ms), MP_ROM_PTR(&mp_utime_ticks_ms_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_us), MP_ROM_PTR(&mp_utime_ticks_us_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_cpu), MP_ROM_PTR(&mp_utime_ticks_cpu_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_add), MP_ROM_PTR(&mp_utime_ticks_add_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_diff), MP_ROM_PTR(&mp_utime_ticks_diff_obj) },

};

STATIC MP_DEFINE_CONST_DICT(time_module_globals, time_module_globals_table);

const mp_obj_module_t mp_module_utime = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&time_module_globals,
};
