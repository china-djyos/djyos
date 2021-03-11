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
#include "mpconfigport.h"
#include "board_mp_pin.h"

#if MICROPY_PY_PIN_STEM32

typedef struct _P_STEM32_PIN
{
    mp_obj_base_t base;

    Pin *ppin;

}P_STEM32_PIN;

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
P_STEM32_PIN *chip_pin_make_new(mp_obj_t *args,size_t n_args,mp_obj_type_t *type)
{
    mp_obj_t *addr_items;
    size_t addr_len;

    P_STEM32_PIN *p = m_new_obj_with_finaliser(P_STEM32_PIN);
    p->ppin = m_new_obj_with_finaliser(Pin);

    if (n_args >= 1)
    {
        mp_obj_get_array_fixed_n(args[0], 2, &addr_items);
        const char *addr_str = mp_obj_str_get_data(addr_items[0], &addr_len);
        if (addr_len == 0) {
            return mp_const_none;
        }
        p->ppin->PORT = transformation(addr_str);
        p->ppin->Pinx = 1 << mp_obj_get_int(addr_items[1]);
    }

    if (n_args >= 2)
        p->ppin->MODER = mp_obj_get_int(args[1]);
    if (n_args >= 3)
        p->ppin->PUPD = mp_obj_get_int(args[2]);

    if (n_args >= 4)
        p->ppin->AF = mp_obj_get_int(args[3]);
    else
        p->ppin->AF = AF_NUll;

    if (n_args >= 5)
        p->ppin->O_TYPER = mp_obj_get_int(args[4]);

    if (n_args >= 6)
        p->ppin->O_SPEEDR = mp_obj_get_int(args[5]);
    else
        p->ppin->O_SPEEDR = GPIO_SPEED_H;

    p->base.type = type;

//    printf("gpio port is %d\r\n",p->ppin->PORT);
//    printf("gpio pinx is %d\r\n",p->ppin->Pinx);
//    printf("gpio mode is %d\r\n",p->ppin->MODER);
//    printf("gpio type is %d\r\n",p->ppin->O_TYPER);
//    printf("gpio speed is %d\r\n",p->ppin->O_SPEEDR);
//    printf("gpio pupd is %d\r\n",p->ppin->PUPD);
//    printf("gpio af is %d\r\n",p->ppin->AF);

    PIO_Configure(p->ppin, 1);

    return p;
}

mp_obj_t chip_pin_obj_init(P_STEM32_PIN *p,size_t n_args , const mp_obj_t *args)
{
    if(n_args>=2)
        p->ppin->MODER = mp_obj_get_int(args[1]);
    if(n_args>=3)
        p->ppin->PUPD = mp_obj_get_int(args[2]);

    PIO_Configure(p->ppin, 1);

    return mp_const_true;
}

mp_obj_t chip_pin_obj_value(P_STEM32_PIN *p,size_t n_args , const mp_obj_t *args)
{
    if (n_args == 1)
    {
        return mp_obj_new_int(PIO_Get(p->ppin));
    }
    else
    {
        size_t pupd = mp_obj_get_int(args[1]);
        if(pupd)
        {
//            printf("gpio %d %d set to high\r\n",p->ppin->PORT,p->ppin->Pinx);
            GPIO_SettoHigh(p->ppin->PORT,p->ppin->Pinx);
        }
        else
        {
//            printf("gpio %d %d set to low\r\n",p->ppin->PORT,p->ppin->Pinx);
            GPIO_SettoLow(p->ppin->PORT,p->ppin->Pinx);
        }
        return mp_const_none;
    }
}

mp_obj_t chip_pin_off(P_STEM32_PIN *p)
{
    GPIO_PowerOff(p->ppin->PORT);

    return mp_const_none;
}

mp_obj_t chip_pin_on(P_STEM32_PIN *p)
{
    GPIO_PowerOn(p->ppin->PORT);

    return mp_const_none;
}

mp_obj_t chip_pin_low(P_STEM32_PIN *p)
{
    GPIO_SettoLow(p->ppin->PORT,p->ppin->Pinx);

    return mp_const_none;
}

mp_obj_t chip_pin_high(P_STEM32_PIN *p)
{
    GPIO_SettoHigh(p->ppin->PORT,p->ppin->Pinx);

    return mp_const_none;
}

#endif
