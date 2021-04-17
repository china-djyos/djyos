/*
 * timer.c
 *
 *  Created on: 2021/3/8
 *      Author: WangXi
 */

#include <stdint.h>
#include <string.h>

#include "timer.h"

#include "py/runtime.h"
#include "py/gc.h"
#include "py/obj.h"

typedef struct _machine_timer_obj_t{
    mp_obj_base_t base;
    int8_t timerid;
    void *timeout_cb;
    tagTimer* timer;
} machine_timer_obj_t;

const mp_obj_type_t machine_timer_type;

STATIC void error_check(bool_t status, const char *msg) {
    if (!status) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, msg));
    }
}

STATIC void machine_timer_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    machine_timer_obj_t *self = MP_OBJ_TO_PTR(self_in);

    mp_printf(print, "Timer(%p; timer_id:%d) ", self->timer, self->timerid);
    /*
    if (self->timerid >= 0) {
        mp_printf(print, "timer_id=%d, ", self->timerid);
    } else {
        mp_printf(print, "timer_name=%s, ", self->dev_name);
    }
    mp_printf(print, "period=%d, ", self->timeout);
    mp_printf(print, "auto_reload=%d)", self->is_repeat);
    */
}

STATIC mp_obj_t machine_timer_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    machine_timer_obj_t *self = m_new_obj(machine_timer_obj_t);

    // check arguments
    mp_arg_check_num(n_args, n_kw, 1, 1, true);

    // check input timer device name or ID
    if (mp_obj_is_small_int(args[0])) {
        int device_id = mp_obj_get_int(args[0]);
        self->timerid = device_id;
    } else if (mp_obj_is_qstr(args[0])) {
        static int device_id = 0;
        self->timerid = -1;
    } else {
        error_check(0, "Input ADC device name or ID error.");
    }
    self->timer = NULL;
    self->base.type = &machine_timer_type;

    return MP_OBJ_FROM_PTR(self);
}

STATIC mp_obj_t machine_timer_deinit(mp_obj_t self_in) {
    machine_timer_obj_t *self = self_in;

    if (self->timer == NULL) {
        error_check(false, "Timer device close error");
    }
    else
    {
        Timer_Delete(self->timer);
        self->timer = NULL;
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_timer_deinit_obj, machine_timer_deinit);

STATIC void machine_timer_func(tagTimer * timer)
{
    void *func = Timer_GetTag(timer);
    mp_call_function_0(func);
}

STATIC mp_obj_t machine_timer_init(mp_uint_t n_args, const mp_obj_t *args, mp_map_t *kw_args) {
    machine_timer_obj_t *self = (machine_timer_obj_t *)args[0];
    int mode = 0;
    int timeout = 0;
    bool_t is_repeat = false;

    enum {
        ARG_mode,
        ARG_period,
        ARG_callback,
    };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_mode,         MP_ARG_INT, {.u_int = 1} },
        { MP_QSTR_period,       MP_ARG_INT, {.u_int = 0xffffffff} },
        { MP_QSTR_callback,     MP_ARG_OBJ, {.u_obj = mp_const_none} },
    };

    mp_arg_val_t dargs[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, dargs);

    if (2 == n_args) {
        timeout = dargs[0].u_int;
    } else if (3 == n_args) {
        is_repeat = dargs[ARG_mode].u_int;
        timeout = dargs[ARG_period].u_int;
    } else if (4 == n_args) {
        is_repeat = dargs[ARG_mode].u_int;
        timeout = dargs[ARG_period].u_int;
        self->timeout_cb = dargs[ARG_callback].u_obj;
    } else {
        mp_raise_ValueError("invalid format");
    }

    error_check(timeout > 0, "Set timeout value error");
    if(self->timer != NULL)
    {
        Timer_Delete(self->timer);
    }
    self->timer = Timer_Create(0, timeout * 1000, machine_timer_func);
    Timer_SetTag(self->timer, self->timeout_cb);
    Timer_Ctrl(self->timer, EN_TIMER_SOFT_SETRELOAD, is_repeat);
    Timer_Ctrl(self->timer, EN_TIMER_SOFT_START, 0);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(machine_timer_init_obj, 1, machine_timer_init);

STATIC const mp_rom_map_elem_t machine_timer_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&machine_timer_init_obj) },
    { MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&machine_timer_deinit_obj) },
    { MP_ROM_QSTR(MP_QSTR_deinit), MP_ROM_PTR(&machine_timer_deinit_obj) },

    { MP_ROM_QSTR(MP_QSTR_ONE_SHOT), MP_ROM_INT(false) },
    { MP_ROM_QSTR(MP_QSTR_PERIODIC), MP_ROM_INT(true) },
};
STATIC MP_DEFINE_CONST_DICT(machine_timer_locals_dict, machine_timer_locals_dict_table);

const mp_obj_type_t machine_timer_type = {
    { &mp_type_type },
    .name = MP_QSTR_Timer,
    .print = machine_timer_print,
    .make_new = machine_timer_make_new,
    .locals_dict = (mp_obj_t) &machine_timer_locals_dict,
};
