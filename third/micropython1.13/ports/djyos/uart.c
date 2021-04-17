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
#include "uart.h"
#include "uartctrl.h"

const char *PYTHON_UART[8] = {
        "/dev/UART0",
        "/dev/UART1",
        "/dev/UART2",
        "/dev/UART3",
        "/dev/UART4",
        "/dev/UART5",
        "/dev/UART6",
        "/dev/UART7",
};
char data[READ_MAX]={0};
const mp_obj_type_t uart_type;

mp_obj_t mp_uart_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {

    size_t uart=0;
    Python_Uart *u = m_new_obj_with_finaliser(Python_Uart);
    struct COMParam com={0};

    if(n_args>=1)
    {
        uart=mp_obj_get_int(args[0]);
        u->fd=open(PYTHON_UART[uart], O_RDWR);

        if(u->fd<=0)
            return mp_const_false;
    }
    if (n_args >= 2)
    {
        com.BaudRate=mp_obj_get_int(args[1]);
        fcntl(u->fd, CN_UART_COM_SET, &com);

    }

    fcntl(u->fd, F_SETTIMEOUT, 1000000);

    u->base.type = &uart_type;

//    printf("u->fd is %d\r\n",u->fd);
//    printf("com.BaudRate is %d\r\n",com.BaudRate);
    return MP_OBJ_FROM_PTR(u);
}

STATIC mp_obj_t uart_init(size_t n_args , const mp_obj_t *args)
{
    Python_Uart *self = MP_OBJ_TO_PTR(args[0]);
    struct COMParam com={0};
    com.BaudRate=115200;
    com.DataBits=8;
    com.Parity=0;
//    com.StopBits=1;

//    printf("bitrate  data_bit  parity_bit  stop_bit\r\n");

    if (n_args >= 2)
        com.BaudRate=mp_obj_get_int(args[1]);
    if (n_args >= 3)
        com.DataBits=mp_obj_get_int(args[2]);
    if (n_args >= 4)
        com.Parity=mp_obj_get_int(args[3]);
    if (n_args >= 5)
        com.StopBits=mp_obj_get_int(args[4]);

//    printf("self->fd is %d\r\n",self->fd);
//    printf("com.BaudRate is %d\r\n",com.BaudRate);
//    printf("com.DataBits is %d\r\n",com.DataBits);
//    printf("com.Parity is %d\r\n",com.Parity);
//    printf("com.StopBits is %d\r\n",com.StopBits);

    if(self->fd<=0)
        return mp_const_false;
    fcntl(self->fd, CN_UART_COM_SET, &com);

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(uart_init_obj, 1, 5, uart_init);


STATIC mp_obj_t uart_read(size_t n_args , const mp_obj_t *args)
{
    Python_Uart *self = MP_OBJ_TO_PTR(args[0]);
    size_t ret=0;
    size_t len=0;

    if(self->fd<=0)
        return mp_const_false;

    if (n_args == 1)
    {
        len=READ_MAX;
    }
    else
    {
        len=mp_obj_get_int(args[1]);
    }

    memset(data,0,READ_MAX);

    ret=read(self->fd, data, len);

    printf("ret is %d\r\n",ret);
//    if(ret>0)
//    {
//        printf("Device Read is ");
//        for(int i=0;i<ret;i++)
//        {
//            printf("%02x ",data[i]);
//        }
//        printf("\r\n");
//    }

    return mp_obj_new_str(data,ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(uart_read_obj, 1, 2,uart_read);


STATIC mp_obj_t uart_write(mp_obj_t self_in,mp_obj_t date)
{
    Python_Uart *self = MP_OBJ_TO_PTR(self_in);
    size_t ret=0;
    size_t addr_len;

    if(self->fd<=0)
        return mp_const_false;

    const char *addr_str = mp_obj_str_get_data(date, &addr_len);
    if (addr_len == 0)
        return mp_const_none;

    ret = write(self->fd, addr_str, addr_len);

    printf("ret is %d\r\n",ret);
//    printf("addr_str is %s\r\n",addr_str);

    return mp_obj_new_int(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(uart_write_obj, uart_write);


STATIC mp_obj_t uart_close(mp_obj_t self_in)
{
    Python_Uart *self = MP_OBJ_TO_PTR(self_in);
    close(self->fd);
    self->fd=NULL;
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(uart_close_obj, uart_close);

STATIC const mp_rom_map_elem_t uart_locals_dict_table[] = {

    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&uart_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&uart_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&uart_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_close), MP_ROM_PTR(&uart_close_obj) },

};

STATIC MP_DEFINE_CONST_DICT(uart_locals_dict, uart_locals_dict_table);

const mp_obj_type_t uart_type = {
    { &mp_type_type },
    .name = MP_QSTR_UART,
    .make_new = mp_uart_make_new,
    .locals_dict = (mp_obj_dict_t *)&uart_locals_dict,
};

