#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "py/obj.h"
#include "py/runtime.h"
#include "py/builtin.h"

#include "py/stackctrl.h"
#include "py/gc.h"
#include "py/mphal.h"
#include "py/objmodule.h"

// This is the function which will be called from Python as example.add_ints(a, b).
STATIC mp_obj_t example_add_ints(mp_obj_t a_obj, mp_obj_t b_obj) {
    // Extract the ints from the micropython input objects
    int a = mp_obj_get_int(a_obj);
    int b = mp_obj_get_int(b_obj);

    // Calculate the addition and convert to MicroPython object.
    return mp_obj_new_int(a + b);
}

STATIC mp_obj_t example_sub_ints(mp_obj_t a_obj, mp_obj_t b_obj) {
    // Extract the ints from the micropython input objects
    int a = mp_obj_get_int(a_obj);
    int b = mp_obj_get_int(b_obj);

    // Calculate the addition and convert to MicroPython object.
    return mp_obj_new_int(a - b);
}

STATIC mp_obj_t math_nothing()
{
    printf("This is a function in class type and no parameter\r\n");
    return mp_const_none;
}
STATIC mp_obj_t math_one(mp_obj_t a_obj)
{
    int a = mp_obj_get_int(a_obj);
    return mp_obj_new_int(a + 100);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(example_math_nothing_obj, math_nothing);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(example_math_one_obj, math_one);
// type locals_dict_type
STATIC const mp_rom_map_elem_t math_locals_dict_table[] =
{
    { MP_ROM_QSTR(MP_QSTR_nothing), MP_ROM_PTR(&example_math_nothing_obj) },
    { MP_ROM_QSTR(MP_QSTR_one), MP_ROM_PTR(&example_math_one_obj) },
};

STATIC MP_DEFINE_CONST_DICT(math_locals_dict, math_locals_dict_table);
// Define type object.
const mp_obj_type_t example_math_type = {
    .base = { &mp_type_type },
    .name = MP_QSTR_math,
    .locals_dict = (mp_obj_dict_t *)&math_locals_dict,
};


// Define a Python reference to the function above
STATIC MP_DEFINE_CONST_FUN_OBJ_2(example_add_ints_obj, example_add_ints);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(example_sub_ints_obj, example_sub_ints);

// Define all properties of the example module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
STATIC const mp_rom_map_elem_t example_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_example) },
    { MP_ROM_QSTR(MP_QSTR_add_ints), MP_ROM_PTR(&example_add_ints_obj) },
    { MP_ROM_QSTR(MP_QSTR_sub_ints), MP_ROM_PTR(&example_sub_ints_obj) },
    { MP_ROM_QSTR(MP_QSTR_math), MP_ROM_PTR(&example_math_type) },
};
STATIC MP_DEFINE_CONST_DICT(example_module_globals, example_module_globals_table);

// Define module object.
const mp_obj_module_t example_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&example_module_globals,
};

// Register the module to make it available in Python
MP_REGISTER_MODULE(MP_QSTR_example, example_user_cmodule, MODULE_EXAMPLE_ENABLED);
//MP_REGISTER_MODULE(MP_QSTR_math, example_math_type, TYPE_MATH_ENABLED);


