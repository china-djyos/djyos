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


const mp_obj_type_t pin_type;

//STATIC void pin_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
//
//    Python_Pin *self = MP_OBJ_TO_PTR(self_in);
//
//    mp_printf(print, "gpio %d get is %d",self->ppin->PORT, PIO_Get(self->ppin));
//
//}
struct MP_PIN
{
    mp_obj_base_t base;
    mp_obj_t *str;
    char data[10];
};

mp_obj_t mp_pin_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {

    mp_obj_t *addr_items;
    size_t addr_len;
    const char *addr_str;

    struct MP_PIN *p = m_new_obj_with_finaliser(struct MP_PIN);
    p->str = m_new_obj_with_finaliser(mp_obj_t);
    if (n_args >= 1)
    {
        mp_obj_get_array_fixed_n(args[0], 2, &addr_items);
        addr_str = mp_obj_str_get_data(addr_items[0], &addr_len);
        if (addr_len == 0) {
            return mp_const_none;
        }
        p->str = addr_str ;
        p->data[0] = mp_obj_get_int(addr_items[1]);
    }

    if(n_args >= sizeof(p->data))
        n_args=sizeof(p->data);

    for(int i=2;i<=n_args;i++)
    {
        p->data[i-1] = mp_obj_get_int(args[i-1]);
    }

    p->base.type = &pin_type;

    if(PIN_Init(p->str,p->data,n_args) == -1)
    {
        mp_raise_OSError(0);
        m_del(mp_obj_t,p->str,1);
        m_del(mp_obj_t,p,1);
        return mp_const_none;
    }
    else
    {
        return MP_OBJ_FROM_PTR(p);
    }
}

STATIC mp_obj_t pin_obj_init(size_t n_args , const mp_obj_t *args) {

    struct MP_PIN *self = MP_OBJ_TO_PTR(args[0]);

    if(n_args>=2)
        self->data[1] = mp_obj_get_int(args[1]);
    if(n_args>=3)
        self->data[2] = mp_obj_get_int(args[2]);

    PIN_Init(self->str,self->data,n_args);

    return mp_const_true;

}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(pin_init_obj, 2, 3, pin_obj_init);


STATIC mp_obj_t pin_obj_value(size_t n_args , const mp_obj_t *args) {

    struct MP_PIN *self = MP_OBJ_TO_PTR(args[0]);
    size_t value;

    if (n_args == 1)
    {
        return mp_obj_new_int(PIN_Get(self->str,self->data,0));
    }
    else
    {
        value = mp_obj_get_int(args[1]);
        if(value)
        {
            PIN_SettoHigh(self->str,self->data,0);
        }
        else
        {
            PIN_SettoLow(self->str,self->data,0);
        }

    }
    return mp_const_none;

}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(pin_value_obj, 1, 2, pin_obj_value);

STATIC mp_obj_t pin_off(mp_obj_t self_in)
{
    struct MP_PIN *self = MP_OBJ_TO_PTR(self_in);
    PIN_PowerOff(self->str,self->data,0);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(pin_off_obj, pin_off);

STATIC mp_obj_t pin_on(mp_obj_t self_in)
{
    struct MP_PIN *self = MP_OBJ_TO_PTR(self_in);
    PIN_PowerOn(self->str,self->data,0);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(pin_on_obj, pin_on);

STATIC mp_obj_t pin_low(mp_obj_t self_in)
{
    struct MP_PIN *self = MP_OBJ_TO_PTR(self_in);
    PIN_SettoLow(self->str,self->data,0);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(pin_low_obj, pin_low);

STATIC mp_obj_t pin_high(mp_obj_t self_in)
{
    struct MP_PIN *self = MP_OBJ_TO_PTR(self_in);
    PIN_SettoHigh(self->str,self->data,0);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(pin_high_obj, pin_high);

STATIC const mp_rom_map_elem_t pin_locals_dict_table[] = {
    // instance methods
    { MP_ROM_QSTR(MP_QSTR_init),    MP_ROM_PTR(&pin_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_value),   MP_ROM_PTR(&pin_value_obj) },
    { MP_ROM_QSTR(MP_QSTR_off),     MP_ROM_PTR(&pin_off_obj) },
    { MP_ROM_QSTR(MP_QSTR_on),      MP_ROM_PTR(&pin_on_obj) },

    // Legacy names as used by pyb.Pin
    { MP_ROM_QSTR(MP_QSTR_low),     MP_ROM_PTR(&pin_low_obj) },
    { MP_ROM_QSTR(MP_QSTR_high),    MP_ROM_PTR(&pin_high_obj) },

    // class constants
    { MP_ROM_QSTR(MP_QSTR_IN),        MP_ROM_INT(MGPIO_PIN_IN) },
    { MP_ROM_QSTR(MP_QSTR_OUT),       MP_ROM_INT(MGPIO_PIN_OUT) },
    { MP_ROM_QSTR(MP_QSTR_AF),       MP_ROM_INT(MGPIO_PIN_AF) },
    { MP_ROM_QSTR(MP_QSTR_PULL_NONE),   MP_ROM_INT(MGPIO_PIN_PULLNONE) },
    { MP_ROM_QSTR(MP_QSTR_PULL_UP),   MP_ROM_INT(MGPIO_PIN_PULLUP) },
    { MP_ROM_QSTR(MP_QSTR_PULL_DOWN), MP_ROM_INT(MGPIO_PIN_PULLDOWM) },

};

STATIC MP_DEFINE_CONST_DICT(pin_locals_dict, pin_locals_dict_table);

const mp_obj_type_t pin_type = {
    { &mp_type_type },
    .name = MP_QSTR_Pin,
//    .print = pin_print,
    .make_new = mp_pin_make_new,
    .locals_dict = (mp_obj_dict_t *)&pin_locals_dict,
};

