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
#include <stdint.h>
#include <string.h>
#include "py/obj.h"
#include "cpu_peri_gpio.h"
#include "board_mp_pin.h"

#if MICROPY_PY_PIN_BEKEN
typedef struct _P_BEKEN_PIN
{
    mp_obj_base_t base;

    u32 pinx;

    u32 mode;

}P_BEKEN_PIN;

P_BEKEN_PIN *chip_pin_make_new(mp_obj_t *args,size_t n_args,mp_obj_type_t *type)
{
    mp_obj_t *addr_items;
    size_t addr_len;
    size_t mode=0;
    size_t pupd=0;
    P_BEKEN_PIN *p = m_new_obj_with_finaliser(P_BEKEN_PIN);

    if (n_args >= 1)
    {
        mp_obj_get_array_fixed_n(args[0], 2, &addr_items);
        const char *addr_str = mp_obj_str_get_data(addr_items[0], &addr_len);
        if (addr_len == 0) {
            return mp_const_none;
        }
         p->pinx = mp_obj_get_int(addr_items[1]);
    }

    if (n_args >= 2)
        mode = mp_obj_get_int(args[1]);
    if (n_args >= 3)
        pupd = mp_obj_get_int(args[2]);

    if(mode == 0 || pupd == 0)
        p->pinx=PIN_MODE_INPUT;
    else if(mode == 0 || pupd == 1)
        p->pinx=PIN_MODE_INPUT_PULLUP;
    else if(mode == 0 || pupd == 2)
        p->pinx=PIN_MODE_INPUT_PULLDOWN;
    else if(mode == 1)
        p->pinx=PIN_MODE_OUTPUT;

    p->base.type = type;

    printf("gpio pinx is %d\r\n",p->pinx);
    printf("gpio mode is %d\r\n",p->mode);

    djy_gpio_mode(p->pinx,p->mode);

    return p;
}

mp_obj_t chip_pin_obj_init(P_BEKEN_PIN *p,size_t n_args , const mp_obj_t *args)
{
    size_t pupd=0;

    if(n_args>=2)
        p->mode = mp_obj_get_int(args[1]);
    if(n_args>=3)
        pupd = mp_obj_get_int(args[2]);

    djy_gpio_mode(p->pinx,p->mode);
    if(pupd == 1)
    {
        djy_gpio_write(p->pinx,1);    //À­¸ß
    }
    else if(pupd == 2)
    {
        djy_gpio_write(p->pinx,0);    //À­µÍ
    }
    return mp_const_true;
}

mp_obj_t chip_pin_obj_value(P_BEKEN_PIN *p,size_t n_args , const mp_obj_t *args)
{
    if (n_args == 1)
    {
        return mp_obj_new_int(djy_gpio_read(p->pinx));
    }
    else
    {
        size_t pupd = mp_obj_get_int(args[1]);
        djy_gpio_write(p->pinx,pupd);
        return mp_const_none;
    }
}

mp_obj_t chip_pin_off(P_BEKEN_PIN *p)
{
    return mp_const_none;
}

mp_obj_t chip_pin_on(P_BEKEN_PIN *p)
{
    return mp_const_none;
}

mp_obj_t chip_pin_low(P_BEKEN_PIN *p)
{
    djy_gpio_write(p->pinx,0);

    return mp_const_none;
}

mp_obj_t chip_pin_high(P_BEKEN_PIN *p)
{
    djy_gpio_write(p->pinx,1);

    return mp_const_none;
}

#endif
