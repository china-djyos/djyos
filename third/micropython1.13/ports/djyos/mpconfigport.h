#include <stdint.h>

// options to control how MicroPython is built

// You can disable the built-in MicroPython compiler by setting the following
// config option to 0.  If you do this then you won't get a REPL prompt, but you
// will still be able to execute pre-compiled scripts, compiled with mpy-cross.
#define MICROPY_ENABLE_COMPILER     (1)
#define MICROPY_REPL_EVENT_DRIVEN   (0)
#define MICROPY_QSTR_BYTES_IN_HASH  (1)
#define MICROPY_READER_POSIX        (1)
#define MICROPY_QSTR_EXTRA_POOL     mp_qstr_frozen_const_pool
#define MICROPY_ALLOC_PATH_MAX      (256)
#define MICROPY_ALLOC_PARSE_CHUNK_INIT (16)
#define MICROPY_COMP_CONST          (0)
#define MICROPY_COMP_DOUBLE_TUPLE_ASSIGN (0)
#define MICROPY_ENABLE_GC           (1)     //�����ռ�
#define MICROPY_ENABLE_FINALISER    (0)     //�����������������ռ�
#define MICROPY_GC_ALLOC_THRESHOLD  (0)
#define MICROPY_HELPER_REPL         (1)
#define MICROPY_ERROR_REPORTING     (MICROPY_ERROR_REPORTING_TERSE)
#define MICROPY_KBD_EXCEPTION       (1)
#define MICROPY_BUILTIN_METHOD_CHECK_SELF_ARG (0)
#define MICROPY_PY_ASYNC_AWAIT      (0)
#define MICROPY_PY_ASSIGN_EXPR      (0)
#define MICROPY_PY_BUILTINS_BYTEARRAY (0)
#define MICROPY_PY_BUILTINS_DICT_FROMKEYS (0)
#define MICROPY_PY_BUILTINS_ENUMERATE (0)
#define MICROPY_PY_BUILTINS_FILTER  (0)
#define MICROPY_PY_BUILTINS_REVERSED (0)
#define MICROPY_PY_BUILTINS_SET     (0)
#define MICROPY_PY_BUILTINS_SLICE   (0)
#define MICROPY_PY_BUILTINS_PROPERTY (0)
#define MICROPY_PY_BUILTINS_MIN_MAX (0)
#define MICROPY_PY_BUILTINS_STR_COUNT (0)
#define MICROPY_PY_BUILTINS_STR_OP_MODULO (0)
#define MICROPY_PY___FILE__         (0)
#define MICROPY_PY_GC               (0)
#define MICROPY_PY_ARRAY            (1)
#define MICROPY_PY_ATTRTUPLE        (0)
#define MICROPY_PY_COLLECTIONS      (0)
#define MICROPY_PY_IO               (1)
#define MICROPY_PY_STRUCT           (0)
#define MICROPY_PY_SYS              (1)
#define MICROPY_MODULE_FROZEN_MPY   (1)
#define MICROPY_CPYTHON_COMPAT      (0)
#define MICROPY_PY_BUILTINS_HELP    (1)
#define MICROPY_MODULE_GETATTR      (1)
#define MICROPY_STREAMS_POSIX_API   (1)
#define MICROPY_PY_SYS_PLATFORM     "djyos"
#define MICROPY_PYSTACK_ALIGN       (CN_ALIGN_SIZE)
#define MICROPY_STACK_CHECK         (0)
#define MICROPY_REPL_INFO           (1)
#define MICROPY_REPL_AUTO_INDENT    (1)
#define MICROPY_STREAMS_NON_BLOCK   (1)
#define MICROPY_USE_INTERNAL_PRINTF (0)     //ע��һ��
#define MICROPY_PY_BUILTINS_HELP_MODULES (1)
#define MICROPY_PY_IO_IOBASE        (0)
#define MICROPY_PY_IO_FILEIO        (1)

#define MICROPY_FLOAT_IMPL          (MICROPY_FLOAT_IMPL_FLOAT)

#define MICROPY_VFS                 (1)

#define MODULE_EXAMPLE_ENABLED      (1)

#define MICROPY_PY_DJYIP            (1)

// type definitions for the specific machine

typedef intptr_t mp_int_t; // must be pointer size
typedef uintptr_t mp_uint_t; // must be pointer size
typedef long mp_off_t;

#define mp_type_fileio mp_type_djy_fileio
extern const struct _mp_obj_module_t mp_module_usocket;

// extra built in names to add to the global namespace
#define MICROPY_PORT_BUILTINS \
    { MP_ROM_QSTR(MP_QSTR_open), MP_ROM_PTR(&mp_builtin_open_obj) },

#define MICROPY_PORT_BUILTIN_MODULES \
    { MP_ROM_QSTR(MP_QSTR_usocket), MP_ROM_PTR(&mp_module_usocket) },

// We need to provide a declaration/definition of alloca()
#include <alloca.h>

#define MICROPY_HW_BOARD_NAME "DJYOS"
#define MICROPY_HW_MCU_NAME "DJYOS"

#define MICROPY_MIN_USE_STDOUT (1)
//#define MICROPY_EMIT_THUMB (1)


#define MP_STATE_PORT MP_STATE_VM

#define MICROPY_PORT_ROOT_POINTERS \
    const char *readline_hist[8];
