//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <stat.h>
#include <uartctrl.h>
#include "py/mpconfig.h"
#include "py/runtime.h"
#include "py/stream.h"
#include "py/mperrno.h"
#include "py/lexer.h"

typedef struct _djy_file_obj_t {
    mp_obj_base_t base;
    FILE *fp;
} djy_file_obj_t;


// Note: buffering and encoding args are currently ignored
// See vfs.c.297 mp_vfs_open()
extern const mp_obj_type_t mp_type_djy_fileio;
mp_obj_t mp_djy_open(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args)
{
    const char *fname,*mode_s;
    const mp_obj_type_t *type;
    djy_file_obj_t *o = m_new_obj_with_finaliser(djy_file_obj_t);
    (void)kw_args;
    if (n_args == 0)
    {
        printf("Please enter filename\r\n");
        return mp_const_none;
    }
    else if (n_args == 1)
    {
        mode_s = "r";
    }
    else
    {
        mode_s = mp_obj_str_get_str(pos_args[1]);
    }
    fname = mp_obj_str_get_str(pos_args[0]);
    type = &mp_type_djy_fileio;
    o->base.type = type;
    o->fp = fopen(fname,mode_s);
    return MP_OBJ_FROM_PTR(o);
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 0, mp_djy_open);


STATIC mp_obj_t djy_file_obj_make_new(const mp_obj_type_t *type, size_t n_args,
                        size_t n_kw, const mp_obj_t *args)
{
    (void)type;(void)n_args;(void)n_kw;(void)args;
//  mp_arg_val_t arg_vals[FILE_OPEN_NUM_ARGS];
//  mp_arg_parse_all_kw_array(n_args, n_kw, args, FILE_OPEN_NUM_ARGS, file_open_args, arg_vals);
//  return file_open(NULL, type, arg_vals);
    printf("----function djy_file_obj_make_new be call----\r\n");
    return mp_const_none;
}

STATIC mp_uint_t djy_file_obj_read(mp_obj_t self_in, void *buf, mp_uint_t size, int *errcode)
{
    djy_file_obj_t *self = MP_OBJ_TO_PTR(self_in);
    u32 sz_out;
    sz_out = fread(buf, 1, size, self->fp);
    if (sz_out == 0)
    {
        *errcode = errno;
        return MP_STREAM_ERROR;
    }
    return sz_out;
}

STATIC mp_uint_t djy_file_obj_write(mp_obj_t self_in, const void *buf, mp_uint_t size,
                        int *errcode)
{
    djy_file_obj_t *self = MP_OBJ_TO_PTR(self_in);
    u32 sz_out;
    sz_out = fwrite(buf, 1, size, self->fp);
    if (sz_out != size)
    {
        *errcode = errno;
        return MP_STREAM_ERROR;
    }
    return sz_out;
}

STATIC mp_obj_t djy_file_obj_uartcfg(size_t n_args, const mp_obj_t *args)
{
    struct COMParam com={0};
    djy_file_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    s32 sz_out;
    const char *param;
    if (n_args < 5)
        return mp_const_false;
    com.BaudRate = mp_obj_get_int(args[1]);    //is baudrate
    param = mp_obj_str_get_str(args[2]);    //is parity n/o/e/m/s
    if (param[0] == 'n')
        com.Parity = CN_UART_PARITY_NONE;
    if (param[0] == 'o')
        com.Parity = CN_UART_PARITY_ODD;
    if (param[0] == 'e')
        com.Parity = CN_UART_PARITY_EVEN;
    if (param[0] == 'm')
        com.Parity = CN_UART_PARITY_MARK;
    if (param[0] == 's')
        com.Parity = CN_UART_PARITY_SPACE;
    com.DataBits = mp_obj_get_int(args[3]);    //is data bits 5/6/7/8/9
    if (com.DataBits < CN_UART_DATABITS_5)
        com.DataBits = CN_UART_DATABITS_5;
    else if(com.DataBits > CN_UART_DATABITS_9)
        com.DataBits = CN_UART_DATABITS_9;
    com.StopBits = mp_obj_get_int(args[4]);   //is stop bits 0/15/2
    if(com.StopBits == 15)
        com.StopBits = CN_UART_STOPBITS_1_5;
    else if(com.StopBits == 2)
        com.StopBits = CN_UART_STOPBITS_2;
    else
        com.StopBits = CN_UART_STOPBITS_1;  //default

    sz_out = fcntl(self->fp->fd, CN_UART_COM_SET, &com);
    if (sz_out == -1)
        return mp_const_false;
    else
        return mp_const_true;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mp_stream_uartcfg_obj, 5, 5, djy_file_obj_uartcfg);

STATIC mp_obj_t file_obj___exit__(size_t n_args, const mp_obj_t *args) {
    (void)n_args;
    return mp_stream_close(args[0]);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(file_obj___exit___obj, 4, 4, file_obj___exit__);

STATIC mp_uint_t djy_file_obj_ioctl(mp_obj_t o_in, mp_uint_t request, uintptr_t arg, int *errcode)
{
    djy_file_obj_t *self = MP_OBJ_TO_PTR(o_in);
    s32 res;

    if (request == MP_STREAM_SEEK) {
        struct mp_stream_seek_t *s = (struct mp_stream_seek_t *)(uintptr_t)arg;

        fseek(self->fp, s->offset, s->whence);
        s->offset = ftell(self->fp);
        return 0;

    } else if (request == MP_STREAM_FLUSH) {
        res = fflush(self->fp);
        if (res != 0) {
            *errcode = errno;
            return MP_STREAM_ERROR;
        }
        return 0;

    } else if (request == MP_STREAM_CLOSE) {
        // if fs==NULL then the file is closed and in that case this method is a no-op
        if (self->fp != NULL) {
            res = fclose(self->fp);
            if (res != 0) {
                *errcode = errno;
                return MP_STREAM_ERROR;
            }
            else
                self->fp = NULL;
        }
        return 0;

    } else {
        *errcode = MP_EINVAL;
        return MP_STREAM_ERROR;
    }
}

mp_import_stat_t mp_import_stat(const char *path)
{
    struct stat mstat;
    stat(path, &mstat);
    if (mstat.st_mode & S_IFREG)
        return MP_IMPORT_STAT_FILE;
    else  if(mstat.st_mode & S_IFDIR)
        return MP_IMPORT_STAT_DIR;
    else
        return MP_IMPORT_STAT_NO_EXIST;
}

STATIC const mp_rom_map_elem_t djy_file_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&mp_stream_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&mp_stream_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_flush), MP_ROM_PTR(&mp_stream_flush_obj) },
    { MP_ROM_QSTR(MP_QSTR_close), MP_ROM_PTR(&mp_stream_close_obj) },
    { MP_ROM_QSTR(MP_QSTR_seek), MP_ROM_PTR(&mp_stream_seek_obj) },
    { MP_ROM_QSTR(MP_QSTR_tell), MP_ROM_PTR(&mp_stream_tell_obj) },
    { MP_ROM_QSTR(MP_QSTR_uartcfg), MP_ROM_PTR(&mp_stream_uartcfg_obj) },
    { MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&mp_stream_close_obj) },
    { MP_ROM_QSTR(MP_QSTR___enter__), MP_ROM_PTR(&mp_identity_obj) },
    { MP_ROM_QSTR(MP_QSTR___exit__), MP_ROM_PTR(&file_obj___exit___obj) },
};
STATIC MP_DEFINE_CONST_DICT(djy_rawfile_locals_dict, djy_file_locals_dict_table);


#if MICROPY_PY_IO_FILEIO
STATIC const mp_stream_p_t djy_fileio_stream_p = {
    .read = djy_file_obj_read,
    .write = djy_file_obj_write,
    .ioctl = djy_file_obj_ioctl,
};

STATIC void file_obj_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    mp_printf(print, "<io.%s %p>", mp_obj_get_type_str(self_in), MP_OBJ_TO_PTR(self_in));
}

const mp_obj_type_t mp_type_djy_fileio = {
    { &mp_type_type },
    .name = MP_QSTR_FileIO,
    .print = file_obj_print,
    .make_new = djy_file_obj_make_new,
    .getiter = mp_identity_getiter,
    .iternext = mp_stream_unbuffered_iter,
    .protocol = &djy_fileio_stream_p,
    .locals_dict = (mp_obj_dict_t *)&djy_rawfile_locals_dict,
};
#endif


