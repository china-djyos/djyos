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
#include "lib/utils/pyexec.h"
#include "py/gc.h"
#include "py/runtime.h"

// machine.info([dump_alloc_table])
// Print out lots of information about the board.
STATIC mp_obj_t machine_info(size_t n_args, const mp_obj_t *args) {
    // get and print unique id; 96 bits
    {
        printf("SN:...\r\n");
    }
    // to print info about memory
    {
        extern bool_t heap(void);
        heap();
    }

    // qstr info
    {
        size_t n_pool, n_qstr, n_str_data_bytes, n_total_bytes;
        qstr_pool_info(&n_pool, &n_qstr, &n_str_data_bytes, &n_total_bytes);
        printf("qstr:\n  n_pool=%u\n  n_qstr=%u\n  n_str_data_bytes=%u\n  n_total_bytes=%u\n", n_pool, n_qstr, n_str_data_bytes, n_total_bytes);
    }

    // GC info
    {
        gc_info_t info;
        gc_info(&info);
        printf("GC:\n");
        printf("  %u total\n", info.total);
        printf("  %u : %u\n", info.used, info.free);
        printf("  1=%u 2=%u m=%u\n", info.num_1block, info.num_2block, info.max_block);
    }

    // free space on flash
    {
        #if MICROPY_VFS_FAT
        for (mp_vfs_mount_t *vfs = MP_STATE_VM(vfs_mount_table); vfs != NULL; vfs = vfs->next) {
            if (strncmp("/flash", vfs->str, vfs->len) == 0) {
                // assumes that it's a FatFs filesystem
                fs_user_mount_t *vfs_fat = MP_OBJ_TO_PTR(vfs->obj);
                DWORD nclst;
                f_getfree(&vfs_fat->fatfs, &nclst);
                printf("LFS free: %u bytes\n", (uint)(nclst * vfs_fat->fatfs.csize * 512));
                break;
            }
        }
        #endif
    }

    #if MICROPY_PY_THREAD
    printf("thread info\r\n");
    #endif

    if (n_args == 1) {
        // arg given means dump gc allocation table
        gc_dump_alloc_table();
    }

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_info_obj, 0, 1, machine_info);

// Resets the pyboard in a manner similar to pushing the external RESET button.
STATIC mp_obj_t machine_reset(void) {
    runapp(0);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(machine_reset_obj, machine_reset);

STATIC mp_obj_t machine_soft_reset(void) {
    pyexec_system_exit = PYEXEC_FORCED_EXIT;
    mp_raise_type(&mp_type_SystemExit);
}
MP_DEFINE_CONST_FUN_OBJ_0(machine_soft_reset_obj, machine_soft_reset);

extern const struct _mp_obj_type_t pin_type;
extern const struct _mp_obj_type_t uart_type;
extern const struct _mp_obj_type_t machine_timer_type;

STATIC const mp_rom_map_elem_t mp_module_machine_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_machine) },
    
    { MP_ROM_QSTR(MP_QSTR_info),                MP_ROM_PTR(&machine_info_obj) },
    // { MP_ROM_QSTR(MP_QSTR_unique_id),           MP_ROM_PTR(&machine_unique_id_obj) },
    { MP_ROM_QSTR(MP_QSTR_reset),               MP_ROM_PTR(&machine_reset_obj) },
    { MP_ROM_QSTR(MP_QSTR_soft_reset),          MP_ROM_PTR(&machine_soft_reset_obj) },

    { MP_ROM_QSTR(MP_QSTR_Pin), MP_ROM_PTR(&pin_type) },
    { MP_ROM_QSTR(MP_QSTR_UART), MP_ROM_PTR(&uart_type) },
    { MP_ROM_QSTR(MP_QSTR_Timer), MP_ROM_PTR(&machine_timer_type) },

};

STATIC MP_DEFINE_CONST_DICT(mp_module_machine_globals, mp_module_machine_globals_table);

const mp_obj_module_t mp_module_machine = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&mp_module_machine_globals,
};

