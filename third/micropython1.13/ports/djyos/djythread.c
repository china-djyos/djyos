/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Damien P. George
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
#include <systime.h>

#include "py/runtime.h"
#include "py/gc.h"
#include "py/mpthread.h"
#include "py/stackctrl.h"

#if MICROPY_PY_THREAD

typedef struct _mp_thread_t
{
    u16 event_id;            // system id of thread
//    mp_state_thread_t *arg;  // thread Python args, a GC root pointer
//  void *stack;             // pointer to the stack
//  size_t stack_len;        // number of words in the stack
    struct _mp_thread_t *next;
} djy_thread_t;

STATIC djy_thread_t thread_root_node;
STATIC djy_thread_t *thread_root; // root pointer, handled by mp_thread_gc_others
u32 ThreadNum;
STATIC mp_thread_mutex_t thread_mutex;

// the mutex controls access to the linked list
//STATIC mp_thread_mutex_t thread_mutex;
void mp_thread_init(char *stack_top, char *stack_bottom)
{
    mp_thread_set_state(&mp_state_ctx.thread);
    MP_STATE_THREAD(stack_top) = stack_top;
    mp_stack_set_limit(stack_top - stack_bottom + 1024);

    thread_root = &thread_root_node;
    thread_root->event_id = DJY_GetMyEventId();
////  thread_root->arg = &mp_state_ctx.thread;
//    thread_root->stack = stack;
//    thread_root->stack_len = stack_len;
    thread_root->next = NULL;

    mp_thread_mutex_init(&thread_mutex);
}

void mp_thread_gc_others(void)
{
    djy_thread_t *th;
    struct EventInfo EventInfo;

    mp_thread_mutex_lock(&thread_mutex, 1);
    for (djy_thread_t *th = thread_root; th != NULL; th = th->next)
    {
        DJY_GetEventInfo(th->event_id, &EventInfo);
        if (th->event_id == DJY_GetMyEventId())
        {
            uint32_t SP;
//          gc_collect_root((void **)&SP, ((uint32_t)MP_STATE_THREAD(stack_top)
//                                         - (uint32_t)&SP) / sizeof(uint32_t));
            gc_collect_root((void **)&SP, ((uint32_t)EventInfo.StackTop
                                           - (uint32_t)&SP) / sizeof(uint32_t));
        }
        else
        {
//          gc_collect_root(EventInfo.Stack, ((uint32_t)MP_STATE_THREAD(stack_top)
//                                            - EventInfo.Stack) / sizeof(uint32_t));
            gc_collect_root((void **)EventInfo.Stack, (EventInfo.StackTop - EventInfo.Stack));
        }
    }
    mp_thread_mutex_unlock(&thread_mutex);
}

void mp_thread_mutex_init(mp_thread_mutex_t *m)
{
    Lock_MutexCreate_s(m,"micropython");
}

int mp_thread_mutex_lock(mp_thread_mutex_t *m, int wait)
{
    return (int)Lock_MutexPend(m, wait * CN_TIMEOUT_FOREVER);
}

 void mp_thread_mutex_unlock(mp_thread_mutex_t *m)
 {
    Lock_MutexPost(m);
}

void mp_thread_set_state(mp_state_thread_t *state)
{
    DJY_SetEventUserdata((ptu32_t)state);
}

mp_state_thread_t *mp_thread_get_state(void)
{
    return (mp_state_thread_t *)DJY_GetEventUserdata();
}

ptu32_t DJY_ThreadEntry(void)
{
    void *arg;
    void *(*entry)(void *);
    djy_thread_t th;

    DJY_GetEventPara((ptu32_t *)&arg, (ptu32_t *)&entry);

    // add thread to linked list of all threads
    th.event_id = DJY_GetMyEventId();
//  th->stack = EventInfo->StackBottom;
//  th->stack_len = EventInfo->StackSize / sizeof(ucpu_t);
    mp_thread_mutex_lock(&thread_mutex, 1);
    th.next = thread_root;
    thread_root = &th;
    mp_thread_mutex_unlock(&thread_mutex);

    entry(arg);
    ThreadNum--;
    return 0;
}
// create thread and run entry,with param arg
void mp_thread_create(void *(*entry)(void *), void *arg, size_t *stack_size)
{
    static u16 evtt_id = CN_EVTT_ID_INVALID;
    static size_t oldsize=0;
    u16 event_id = CN_EVENT_ID_INVALID;
    ThreadNum++;
    if (*stack_size == 0) {
        *stack_size = 4096; // default stack size
    } else if (*stack_size < 2048) {
        *stack_size = 2048; // minimum stack size
    }
    if (oldsize != *stack_size)
    {
        if (evtt_id != CN_EVTT_ID_INVALID)
            DJY_EvttUnregist(evtt_id);
        oldsize = *stack_size;
        evtt_id = DJY_EvttRegist(EN_INDEPENDENCE, CN_PRIO_RRS, 0, 5,
                              DJY_ThreadEntry, NULL,oldsize,NULL);
    }
    // adjust stack_size to provide room to recover from hitting the limit
    *stack_size -= 1024;
    if (evtt_id != CN_EVTT_ID_INVALID)
    {
        event_id = DJY_EventPop(evtt_id, NULL, 0, (ptu32_t)arg, (ptu32_t)entry, 0);
    }
    if (event_id == CN_EVENT_ID_INVALID)
    {
        ThreadNum--;
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "can't create djyos event"));
    }

}

// confirm ok
void mp_thread_start(void)
{
}

void mp_thread_finish(void) {
    djy_thread_t *prev = NULL;
    uint32_t SP;

    mp_thread_mutex_lock(&thread_mutex, 1);

    gc_collect_root((void **)&SP, ((uint32_t)MP_STATE_THREAD(stack_top)
                                   - (uint32_t)&SP) / sizeof(uint32_t));
    for (djy_thread_t *th = thread_root; th != NULL;prev = th, th = th->next) {
        // unlink the node from the list
        if (th->event_id == DJY_GetMyEventId()) {

            if (prev != NULL) {
                prev->next = th->next;
            } else {
                // move the start pointer
                thread_root = th->next;
            }
//          m_del_obj(djy_thread_t, th);
            break;
        }
    }
    mp_thread_mutex_unlock(&thread_mutex);
}


#endif // MICROPY_PY_THREAD

