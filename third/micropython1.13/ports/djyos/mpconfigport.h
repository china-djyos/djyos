#include <stdint.h>

// options to control how MicroPython is built

// You can disable the built-in MicroPython compiler by setting the following
// config option to 0.  If you do this then you won't get a REPL prompt, but you
// will still be able to execute pre-compiled scripts, compiled with mpy-cross.
#define MICROPY_ENABLE_COMPILER     (1)
#define MICROPY_REPL_EVENT_DRIVEN   (0)
#define MICROPY_QSTR_BYTES_IN_HASH  (1)
#define MICROPY_READER_POSIX        (1)
//#define MICROPY_QSTR_EXTRA_POOL     mp_qstr_frozen_const_pool
#define MICROPY_ALLOC_PATH_MAX      (256)
#define MICROPY_ALLOC_PARSE_CHUNK_INIT (16)
#define MICROPY_COMP_CONST          (0)
#define MICROPY_COMP_DOUBLE_TUPLE_ASSIGN (0)
#define MICROPY_ENABLE_GC           (1)     //垃圾收集
#define MICROPY_ENABLE_FINALISER    (1)     //析构器，用于垃圾收集
#define MICROPY_GC_ALLOC_THRESHOLD  (0)
#define MICROPY_HELPER_REPL         (1)
#define MICROPY_ERROR_REPORTING     (MICROPY_ERROR_REPORTING_TERSE)
#define MICROPY_KBD_EXCEPTION       (1)
#define MICROPY_BUILTIN_METHOD_CHECK_SELF_ARG (0)
#define MICROPY_PY_ASYNC_AWAIT      (0)
#define MICROPY_PY_ASSIGN_EXPR      (0)
#define MICROPY_PY_BUILTINS_BYTEARRAY (1)
#define MICROPY_PY_BUILTINS_DICT_FROMKEYS (0)
#define MICROPY_PY_BUILTINS_ENUMERATE (0)
#define MICROPY_PY_BUILTINS_FILTER  (0)
#define MICROPY_PY_BUILTINS_REVERSED (0)
#define MICROPY_PY_BUILTINS_SET     (0)
#define MICROPY_PY_BUILTINS_SLICE   (0)
#define MICROPY_PY_BUILTINS_PROPERTY (0)
#define MICROPY_PY_BUILTINS_MIN_MAX (1)
#define MICROPY_PY_BUILTINS_STR_COUNT (0)
#define MICROPY_PY_BUILTINS_STR_OP_MODULO (1)   //支持print格式化输出
#define MICROPY_PY___FILE__         (0)
#define MICROPY_PY_GC               (1)
#define MICROPY_PY_ARRAY            (1)
#define MICROPY_PY_ATTRTUPLE        (0)
#define MICROPY_PY_COLLECTIONS      (0)
#define MICROPY_PY_IO               (1)
#define MICROPY_PY_STRUCT           (1)
#define MICROPY_PY_SYS              (1)
#define MICROPY_MODULE_FROZEN_MPY   (0)
#define MICROPY_CPYTHON_COMPAT      (0)
#define MICROPY_PY_BUILTINS_HELP    (1)
#define MICROPY_PY_BUILTINS_HELP_TEXT djyos_help_text
#define MICROPY_MODULE_GETATTR      (1)
#define MICROPY_STREAMS_POSIX_API   (1)
#define MICROPY_PY_SYS_PLATFORM     "djyos"
#define MICROPY_PYSTACK_ALIGN       (CN_ALIGN_SIZE)
#define MICROPY_STACK_CHECK         (0)
#define MICROPY_REPL_INFO           (1)
#define MICROPY_REPL_AUTO_INDENT    (1)
#define MICROPY_STREAMS_NON_BLOCK   (1)
#define MICROPY_USE_INTERNAL_PRINTF (0)     //注意一下
#define MICROPY_PY_BUILTINS_HELP_MODULES (1)
#define MICROPY_PY_IO_IOBASE        (0)
#define MICROPY_PY_IO_FILEIO        (1)
#define  MBEDTLS_NO_PLATFORM_ENTROPY

// math
#define MICROPY_PY_CMATH            (1)
#define MICROPY_PY_MATH_SPECIAL_FUNCTIONS (1)

//utime
#define MICROPY_PY_UTIME_MP_HAL           (1)
#define MICROPY_FLOAT_IMPL          (MICROPY_FLOAT_IMPL_FLOAT)
#define MICROPY_PY_UJSON            (1)
#define MICROPY_PY_MACHINE          (1)

#define MICROPY_VFS                 (1)

#define MODULE_EXAMPLE_ENABLED      (1)

#define MICROPY_PY_DJYIP            (1)
#define MICROPY_PY_USELECT          (1)
#define MICROPY_PY_THREAD           (1)
#define MICROPY_PY_UBINASCII        (1)

// type definitions for the specific machine

typedef intptr_t mp_int_t; // must be pointer size
typedef uintptr_t mp_uint_t; // must be pointer size
typedef long mp_off_t;

#define mp_type_fileio mp_type_djy_fileio
#if MICROPY_PY_DJYIP
extern const struct _mp_obj_module_t mp_module_usocket;
#endif
extern const struct _mp_obj_module_t mp_module_utime;
#if MICROPY_PY_UJSON
extern const struct _mp_obj_module_t mp_module_ujson;
#endif
#if MICROPY_PY_MACHINE
extern const struct _mp_obj_module_t mp_module_machine;
#endif
extern const struct _mp_obj_fun_builtin_fixed_t mp_builtin_shell_obj;

// extra built in names to add to the global namespace
#define MICROPY_PORT_BUILTINS \
    { MP_ROM_QSTR(MP_QSTR_open), MP_ROM_PTR(&mp_builtin_open_obj) },\
    { MP_ROM_QSTR(MP_QSTR_shell), MP_ROM_PTR(&mp_builtin_shell_obj) },

#define MICROPY_PORT_BUILTIN_MODULES \
    { MP_ROM_QSTR(MP_QSTR_utime), MP_ROM_PTR(&mp_module_utime) }, \
    { MP_ROM_QSTR(MP_QSTR_usocket), MP_ROM_PTR(&mp_module_usocket) }, \
    { MP_ROM_QSTR(MP_QSTR_machine), MP_ROM_PTR(&mp_module_machine) }, \

// We need to provide a declaration/definition of alloca()
#include <alloca.h>

#define MICROPY_HW_BOARD_NAME "DJYOS"
#define MICROPY_HW_MCU_NAME "DJYOS"


#define MP_STATE_PORT MP_STATE_VM

#define MICROPY_PORT_ROOT_POINTERS \
    const char *readline_hist[8];

#if MICROPY_PY_THREAD
#define MICROPY_EVENT_POLL_HOOK \
    do { \
        extern void mp_handle_pending(bool); \
        mp_handle_pending(true); \
        MP_THREAD_GIL_EXIT(); \
        MP_THREAD_GIL_ENTER(); \
    } while (0);
#else
#define MICROPY_EVENT_POLL_HOOK \
    do { \
        extern void mp_handle_pending(bool); \
        mp_handle_pending(true); \
        rt_thread_delay(1); \
    } while (0);
#endif

