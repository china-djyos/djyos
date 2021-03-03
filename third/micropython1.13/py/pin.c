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
#include "pin.h"
#include "cpu_peri_gpio.h"

const mp_obj_type_t pin_type;

size_t transformation(char *str)
{
    if(strcmp(str,"GPIO_A")==0)
        return GPIO_A;
    else if(strcmp(str,"GPIO_B")==0)
        return GPIO_B;
    else if(strcmp(str,"GPIO_C")==0)
        return GPIO_C;
    else if(strcmp(str,"GPIO_D")==0)
        return GPIO_D;
    else if(strcmp(str,"GPIO_E")==0)
        return GPIO_E;
    else if(strcmp(str,"GPIO_F")==0)
        return GPIO_F;
    else if(strcmp(str,"GPIO_G")==0)
        return GPIO_G;
    else if(strcmp(str,"GPIO_H")==0)
        return GPIO_H;
    else if(strcmp(str,"GPIO_I")==0)
        return GPIO_I;
    else if(strcmp(str,"GPIO_J")==0)
        return GPIO_J;
    else if(strcmp(str,"GPIO_K")==0)
        return GPIO_K;
    else
        return GPIO_A;
}

//STATIC void pin_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
//
//    Python_Pin *self = MP_OBJ_TO_PTR(self_in);
//
//    mp_printf(print, "gpio %d get is %d",self->ppin->PORT, PIO_Get(self->ppin));
//
//}

mp_obj_t mp_pin_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {

    size_t port = GPIO_A;
    size_t pinx = PIN0;
    size_t mode = GPIO_MODE_IN;
    size_t otype = GPIO_OTYPE_PP;
    size_t speed = GPIO_SPEED_H;
    size_t pupd = GPIO_PUPD_NONE;
    size_t af=AF_NUll;

    mp_obj_t *addr_items;
    size_t addr_len;

    Python_Pin *p = m_new_obj_with_finaliser(Python_Pin);
    p->ppin = m_new_obj_with_finaliser(Pin);

    if (n_args >= 1)
    {
        mp_obj_get_array_fixed_n(args[0], 2, &addr_items);

        const char *addr_str = mp_obj_str_get_data(addr_items[0], &addr_len);
        if (addr_len == 0) {

            return mp_const_none;
        }
        port=transformation(addr_str);
        pinx = 1<<mp_obj_get_int(addr_items[1]);

//        printf("addr_items[0] is %s\r\n",addr_str);
//        printf("addr_items[1] is %d\r\n",pinx);
    }

    if (n_args >= 2)
        mode = mp_obj_get_int(args[1]);
    if (n_args >= 3)
        pupd = mp_obj_get_int(args[2]);
    if (n_args >= 4)
        af = mp_obj_get_int(args[3]);

//    printf("gpio port is %d\r\n",port);
//    printf("gpio pinx is %d\r\n",pinx);
//    printf("gpio mode is %d\r\n",mode);
//    printf("gpio pupd is %d\r\n",pupd);
//    printf("gpio af is %d\r\n",af);

    p->ppin->PORT = port;
    p->ppin->Pinx = pinx;
    p->ppin->MODER = mode;
    p->ppin->O_TYPER = otype;
    p->ppin->O_SPEEDR = speed;
    p->ppin->PUPD = pupd;
    p->ppin->AF = af;
    p->base.type = &pin_type;

    PIO_Configure(p->ppin, 1);


    return MP_OBJ_FROM_PTR(p);
}

STATIC mp_obj_t pin_obj_init(size_t n_args , const mp_obj_t *args) {

    Python_Pin *self = MP_OBJ_TO_PTR(args[0]);

    if(n_args>=2)
        self->ppin->MODER = mp_obj_get_int(args[1]);
    if(n_args>=3)
        self->ppin->PUPD = mp_obj_get_int(args[2]);

//    printf("gpio init moder %d pupd %d\r\n",self->ppin->MODER,self->ppin->PUPD);

    PIO_Configure(self->ppin, 1);

    return mp_const_true;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(pin_init_obj, 2, 3, pin_obj_init);


STATIC mp_obj_t pin_obj_value(size_t n_args , const mp_obj_t *args) {

    Python_Pin *self = MP_OBJ_TO_PTR(args[0]);

    if (n_args == 1)
    {
        return mp_obj_new_int(PIO_Get(self->ppin));
    }
    else
    {
        size_t pupd = mp_obj_get_int(args[1]);
        if(pupd)
        {
            printf("gpio %d %d set to high\r\n",self->ppin->PORT,self->ppin->Pinx);
//            self->ppin->PUPD = GPIO_PUPD_PU;
            GPIO_SettoHigh(self->ppin->PORT,self->ppin->Pinx);
        }
         else
        {
            printf("gpio %d %d set to low\r\n",self->ppin->PORT,self->ppin->Pinx);
//            self->ppin->PUPD = GPIO_PUPD_PD;
            GPIO_SettoLow(self->ppin->PORT,self->ppin->Pinx);
        }

        return mp_const_none;
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(pin_value_obj, 1, 2, pin_obj_value);

STATIC mp_obj_t pin_off(mp_obj_t self_in)
{
    Python_Pin *self = MP_OBJ_TO_PTR(self_in);

//    printf("gpio %d power_off\r\n",self->ppin->PORT);
    GPIO_PowerOff(self->ppin->PORT);

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(pin_off_obj, pin_off);

STATIC mp_obj_t pin_on(mp_obj_t self_in)
{
    Python_Pin *self = MP_OBJ_TO_PTR(self_in);

    printf("gpio %d power_on\r\n",self->ppin->PORT);
    GPIO_PowerOn(self->ppin->PORT);

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(pin_on_obj, pin_on);

STATIC mp_obj_t pin_low(mp_obj_t self_in)
{
    Python_Pin *self = MP_OBJ_TO_PTR(self_in);

    printf("gpio %d %d set to low\r\n",self->ppin->PORT,self->ppin->Pinx);
//    self->ppin->PUPD = GPIO_PUPD_PD;
    GPIO_SettoLow(self->ppin->PORT,self->ppin->Pinx);

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(pin_low_obj, pin_low);

STATIC mp_obj_t pin_high(mp_obj_t self_in)
{
    Python_Pin *self = MP_OBJ_TO_PTR(self_in);

    printf("gpio %d %d set to high\r\n",self->ppin->PORT,self->ppin->Pinx);
//    self->ppin->PUPD = GPIO_PUPD_PD;
    GPIO_SettoHigh(self->ppin->PORT,self->ppin->Pinx);

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(pin_high_obj, pin_high);

STATIC const mp_rom_map_elem_t pin_locals_dict_table[] = {
    // instance methods
    { MP_ROM_QSTR(MP_QSTR_init),    MP_ROM_PTR(&pin_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_value),   MP_ROM_PTR(&pin_value_obj) },
    { MP_ROM_QSTR(MP_QSTR_off),     MP_ROM_PTR(&pin_off_obj) },
    { MP_ROM_QSTR(MP_QSTR_on),      MP_ROM_PTR(&pin_on_obj) },
////    { MP_ROM_QSTR(MP_QSTR_irq),     MP_ROM_PTR(&pin_irq_obj) },
//
    // Legacy names as used by pyb.Pin
    { MP_ROM_QSTR(MP_QSTR_low),     MP_ROM_PTR(&pin_low_obj) },
    { MP_ROM_QSTR(MP_QSTR_high),    MP_ROM_PTR(&pin_high_obj) },
//    { MP_ROM_QSTR(MP_QSTR_name),    MP_ROM_PTR(&pin_name_obj) },
//    { MP_ROM_QSTR(MP_QSTR_names),   MP_ROM_PTR(&pin_names_obj) },
//    { MP_ROM_QSTR(MP_QSTR_af_list), MP_ROM_PTR(&pin_af_list_obj) },
//    { MP_ROM_QSTR(MP_QSTR_port),    MP_ROM_PTR(&pin_port_obj) },
//    { MP_ROM_QSTR(MP_QSTR_pin),     MP_ROM_PTR(&pin_pin_obj) },
//    { MP_ROM_QSTR(MP_QSTR_gpio),    MP_ROM_PTR(&pin_gpio_obj) },
//    { MP_ROM_QSTR(MP_QSTR_mode),    MP_ROM_PTR(&pin_mode_obj) },
//    { MP_ROM_QSTR(MP_QSTR_pull),    MP_ROM_PTR(&pin_pull_obj) },
//    { MP_ROM_QSTR(MP_QSTR_af),      MP_ROM_PTR(&pin_af_obj) },
//
//    // class methods
//    { MP_ROM_QSTR(MP_QSTR_mapper),  MP_ROM_PTR(&pin_mapper_obj) },
//    { MP_ROM_QSTR(MP_QSTR_dict),    MP_ROM_PTR(&pin_map_dict_obj) },
//    { MP_ROM_QSTR(MP_QSTR_debug),   MP_ROM_PTR(&pin_debug_obj) },
//
//    // class attributes
//    { MP_ROM_QSTR(MP_QSTR_board),   MP_ROM_PTR(&pin_board_pins_obj_type) },
//    { MP_ROM_QSTR(MP_QSTR_cpu),     MP_ROM_PTR(&pin_cpu_pins_obj_type) },
//
//    // class constants
    { MP_ROM_QSTR(MP_QSTR_IN),        MP_ROM_INT(GPIO_MODE_IN) },
    { MP_ROM_QSTR(MP_QSTR_OUT),       MP_ROM_INT(GPIO_MODE_OUT) },
    { MP_ROM_QSTR(MP_QSTR_AF),       MP_ROM_INT(GPIO_MODE_AF) },
//    { MP_ROM_QSTR(MP_QSTR_OPEN_DRAIN), MP_ROM_INT(GPIO_MODE_OUTPUT_OD) },
//    { MP_ROM_QSTR(MP_QSTR_ALT),       MP_ROM_INT(GPIO_MODE_AF_PP) },
//    { MP_ROM_QSTR(MP_QSTR_ALT_OPEN_DRAIN), MP_ROM_INT(GPIO_MODE_AF_OD) },
//    { MP_ROM_QSTR(MP_QSTR_ANALOG),    MP_ROM_INT(GPIO_MODE_ANALOG) },
    { MP_ROM_QSTR(MP_QSTR_PULL_NONE),   MP_ROM_INT(GPIO_PUPD_NONE) },
    { MP_ROM_QSTR(MP_QSTR_PULL_UP),   MP_ROM_INT(GPIO_PUPD_PU) },
    { MP_ROM_QSTR(MP_QSTR_PULL_DOWN), MP_ROM_INT(GPIO_PUPD_PD) },
//    { MP_ROM_QSTR(MP_QSTR_IRQ_RISING), MP_ROM_INT(GPIO_MODE_IT_RISING) },
//    { MP_ROM_QSTR(MP_QSTR_IRQ_FALLING), MP_ROM_INT(GPIO_MODE_IT_FALLING) },
//
//    // legacy class constants
//    { MP_ROM_QSTR(MP_QSTR_OUT_PP),    MP_ROM_INT(GPIO_MODE_OUTPUT_PP) },
//    { MP_ROM_QSTR(MP_QSTR_OUT_OD),    MP_ROM_INT(GPIO_MODE_OUTPUT_OD) },
//    { MP_ROM_QSTR(MP_QSTR_AF_PP),     MP_ROM_INT(GPIO_MODE_AF_PP) },
//    { MP_ROM_QSTR(MP_QSTR_AF_OD),     MP_ROM_INT(GPIO_MODE_AF_OD) },
//    { MP_ROM_QSTR(MP_QSTR_PULL_NONE), MP_ROM_INT(GPIO_NOPULL) },

};

STATIC MP_DEFINE_CONST_DICT(pin_locals_dict, pin_locals_dict_table);

const mp_obj_type_t pin_type = {
    { &mp_type_type },
    .name = MP_QSTR_Pin,
//    .print = pin_print,
    .make_new = mp_pin_make_new,
    .locals_dict = (mp_obj_dict_t *)&pin_locals_dict,
};

