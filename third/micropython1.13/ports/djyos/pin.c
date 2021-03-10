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

#define MOD_PIN_IN (0)
#define MOD_PIN_OUT (1)
#define MOD_PIN_AF (2)

#define MOD_PIN_PULLNONE (0)
#define MOD_PIN_PULLUP (1)
#define MOD_PIN_PULLDOWN (2)

const mp_obj_type_t pin_type;

//STATIC void pin_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
//
//    Python_Pin *self = MP_OBJ_TO_PTR(self_in);
//
//    mp_printf(print, "gpio %d get is %d",self->ppin->PORT, PIO_Get(self->ppin));
//
//}

mp_obj_t mp_pin_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {

    return MP_OBJ_FROM_PTR(chip_pin_make_new(args,n_args,&pin_type));
}

STATIC mp_obj_t pin_obj_init(size_t n_args , const mp_obj_t *args) {

    return chip_pin_obj_init(MP_OBJ_TO_PTR(args[0]),n_args,args);

}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(pin_init_obj, 2, 3, pin_obj_init);


STATIC mp_obj_t pin_obj_value(size_t n_args , const mp_obj_t *args) {

    return chip_pin_obj_value(MP_OBJ_TO_PTR(args[0]),n_args,args);

}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(pin_value_obj, 1, 2, pin_obj_value);

STATIC mp_obj_t pin_off(mp_obj_t self_in)
{
    return chip_pin_off(MP_OBJ_TO_PTR(self_in));
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(pin_off_obj, pin_off);

STATIC mp_obj_t pin_on(mp_obj_t self_in)
{
    return chip_pin_on(MP_OBJ_TO_PTR(self_in));
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(pin_on_obj, pin_on);

STATIC mp_obj_t pin_low(mp_obj_t self_in)
{
    return chip_pin_low(MP_OBJ_TO_PTR(self_in));
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(pin_low_obj, pin_low);

STATIC mp_obj_t pin_high(mp_obj_t self_in)
{
    return chip_pin_high(MP_OBJ_TO_PTR(self_in));
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
    { MP_ROM_QSTR(MP_QSTR_IN),        MP_ROM_INT(MOD_PIN_IN) },
    { MP_ROM_QSTR(MP_QSTR_OUT),       MP_ROM_INT(MOD_PIN_OUT) },
    { MP_ROM_QSTR(MP_QSTR_AF),       MP_ROM_INT(MOD_PIN_AF) },
    { MP_ROM_QSTR(MP_QSTR_PULL_NONE),   MP_ROM_INT(MOD_PIN_PULLNONE) },
    { MP_ROM_QSTR(MP_QSTR_PULL_UP),   MP_ROM_INT(MOD_PIN_PULLUP) },
    { MP_ROM_QSTR(MP_QSTR_PULL_DOWN), MP_ROM_INT(MOD_PIN_PULLDOWN) },

};

STATIC MP_DEFINE_CONST_DICT(pin_locals_dict, pin_locals_dict_table);

const mp_obj_type_t pin_type = {
    { &mp_type_type },
    .name = MP_QSTR_Pin,
//    .print = pin_print,
    .make_new = mp_pin_make_new,
    .locals_dict = (mp_obj_dict_t *)&pin_locals_dict,
};

