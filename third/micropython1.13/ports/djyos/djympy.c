#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <shell.h>
#include <string.h>
#include <systime.h>

#include "py/compile.h"
#include "py/runtime.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "lib/utils/pyexec.h"
#include "py/mpconfig.h"
#include "lib/mp-readline/readline.h"

const char djyos_help_text[] =
"Welcome to MicroPython on DJYOS!\n"
"\n"
"Control commands:\n"
"  CTRL-A        -- on a blank line, enter raw REPL mode\n"
"  CTRL-B        -- on a blank line, enter normal REPL mode\n"
"  CTRL-C        -- interrupt a running program\n"
"  CTRL-D        -- on a blank line, do a soft reset of the board\n"
"  CTRL-E        -- on a blank line, enter paste mode\n"
"\n"
"For further help on a specific object, type help(obj)\n"
;

int mp_hal_stdin_rx_chr(void);
#if MICROPY_ENABLE_COMPILER
void do_str(const char *src, mp_parse_input_kind_t input_kind) {
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, src, strlen(src), 0);
        qstr source_name = lex->source_name;
        mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);
        mp_obj_t module_fun = mp_compile(&parse_tree, source_name, true);
        mp_call_function_0(module_fun);
        nlr_pop();
    } else {
        // uncaught exception
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
    }
}
#endif

extern u32 ThreadNum;
STATIC u8* stack_top;
#define CN_RUN_FROM_SHELL   1
ptu32_t micropython(void)
{
    char *pyfile;
    u32 run_source;
    struct EventInfo *EventInfo;
    u32 stdin_time;

    EventInfo = malloc(sizeof(struct EventInfo));
    if (EventInfo == NULL)
    {
        printf("内存不足，不能运行python\r\n");
        return 1;
    }
    stdin_time = mp_hal_stdio_init();
    ThreadNum = 0;

    DJY_GetEventInfo(DJY_GetMyEventId(), EventInfo);
    stack_top = EventInfo->StackTop;
    // Python threading init
    #if MICROPY_PY_THREAD
    mp_thread_init(EventInfo->StackTop, EventInfo->StackBottom);
    #endif
    free(EventInfo);

    #if MICROPY_ENABLE_GC
    u8 *heap;
    heap = malloc(8192);
    if (heap != NULL)
        gc_init(heap, heap + M_CheckSize(heap));
    else
    {
        printf("内存不足，不能运行python\r\n");
        return 1;
    }
    #endif

    mp_init();
    DJY_GetEventPara(&pyfile, &run_source);
    if (pyfile)
        pyexec_file(pyfile);
    if (run_source == CN_RUN_FROM_SHELL)
    {
        #if MICROPY_ENABLE_COMPILER
        printf("press CTRL-D to exit micropython\r\n");
        #if MICROPY_REPL_EVENT_DRIVEN
        pyexec_event_repl_init();
        for (;;) {
            int c = mp_hal_stdin_rx_chr();
            if (pyexec_event_repl_process_char(c)) {
                break;
            }
        }
        #else
        pyexec_friendly_repl();
        #endif
        #else
        pyexec_frozen_module("frozentest.py");
        #endif
    }
    mp_hal_stdio_deinit(stdin_time);
    mp_deinit();
    while (ThreadNum != 0)
    {
        DJY_EventDelay(10*mS);
    }
#if MICROPY_ENABLE_GC
    free(heap);
#endif
    return 0;
}

#if MICROPY_ENABLE_GC
void gc_collect(void)
{
    gc_collect_start();
#if MICROPY_PY_THREAD
    // trace root pointers from any threads
    mp_thread_gc_others();
#else
    // WARNING: This gc_collect implementation doesn't try to get root
    // pointers from CPU registers, and thus may function incorrectly.
    void *dummy;
    // gc the main thread stack
    gc_collect_root(&dummy, ((mp_uint_t)stack_top - (mp_uint_t)&dummy) / sizeof(mp_uint_t));
#endif
    gc_collect_end();
//  gc_dump_info();
}
#endif
bool_t shell_exec_command(char *buf);
mp_obj_t CallShell(mp_obj_t cmd)
{
    char *cmdstr = mp_obj_str_get_str(cmd);
    shell_exec_command(cmdstr);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(mp_builtin_shell_obj, CallShell);

void nlr_jump_fail(void *val) {
    while (1) {
        ;
    }
}

void __fatal_error(const char *msg) {
    while (1) {
        ;
    }
}

#ifndef NDEBUG
void MP_WEAK __assert_func(const char *file, int line, const char *func, const char *expr) {
    printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
    __fatal_error("Assertion failed");
}
#endif

static u32 evtt_id = CN_EVTT_ID_INVALID;
int ModuleInstall_Python(u32 stack_size)
{
    evtt_id = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 1, 1,
                          micropython, NULL,stack_size,"micropython");
    return 0;
}
int python_repl(char *param)
{
    //必须使用 CN_TIMEOUT_FOREVER ，这样shell不会继续运行，不然会抢终端。
    DJY_EventPop(evtt_id, NULL, CN_TIMEOUT_FOREVER, (ptu32_t)param, CN_RUN_FROM_SHELL, 0);
    return 1;
}
ADD_TO_ROUTINE_SHELL(python,python_repl,"run python");

int runpython(char *param)
{
    if (param != NULL)
        DJY_EventPop(evtt_id, NULL, 0, (ptu32_t)param, 0, 0);
    return 0;
}

