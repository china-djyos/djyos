#include "sys_rtos.h"
#include "error.h"
#include "rtos_pub.h"
#include <systime.h>
#include <msgqueue.h>
#include <djytimer.h>
#if CFG_SUPPORT_DJYOS       //CK
#include "driver/entry/arch.h"      //lst
#endif
#define THREAD_TIMESLICE 5

#define RTOS_DEBUG   0
#if RTOS_DEBUG
#define RTOS_DBG(...)     rt_kprintf("[RTOS]"),rt_kprintf(__VA_ARGS__)
#else
#define RTOS_DBG(...)
#endif


/******************************************************
 *               Function Definitions
 ******************************************************/
OSStatus rtos_create_thread( beken_thread_t* thread, uint8_t priority, const char* name,
                        beken_thread_function_t function, uint32_t stack_size, beken_thread_arg_t arg )
{
    if(thread == NULL)
    {
        RTOS_DBG("can not create null thread\n");
        return kGeneralErr;
    }

    *thread = DJY_EvttRegist(EN_CORRELATIVE,priority,0,0,
            function,NULL,stack_size, name);
    if(*thread != CN_EVTT_ID_INVALID)
    {
        DJY_EventPop(*thread,NULL,0,arg,0,0);
    }
    else
    {
        RTOS_DBG("create thread fail\n");
        return kGeneralErr;
    }

    RTOS_DBG("create thread %s\n", name);

    return kNoErr;
}

OSStatus rtos_delete_thread( beken_thread_t* thread )
{
    extern struct EventECB  *g_ptEventRunning;    //当前正在执行的事件
    if(thread!=NULL)
        DJY_EvttUnregist(*thread);
    else
        DJY_EvttUnregist(g_ptEventRunning->evtt_id);
    return kNoErr;

}

OSStatus rtos_delay_milliseconds( uint32_t num_ms )
{
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    DJY_EventDelay(num_ms * 1000);
    GLOBAL_INT_RESTORE();
}

void rtos_thread_sleep(uint32_t seconds)
{
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    DJY_EventDelay(seconds*1000*1000);
    GLOBAL_INT_RESTORE();
}

static u32 rtos_sem_cnt = 0;
static u32 rtos_mutex_cnt = 0;

OSStatus rtos_init_semaphore( beken_semaphore_t* semaphore, int maxCount )
{
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    *semaphore = Lock_SempCreate(maxCount,0,CN_BLOCK_FIFO,NULL);
    GLOBAL_INT_RESTORE();
    rtos_sem_cnt++;
    RTOS_DBG("bk_rtos_init_semaphore:%d\r\n",rtos_sem_cnt);
    return (*semaphore != NULL) ? kNoErr : kGeneralErr;
}

OSStatus rtos_get_semaphore(beken_semaphore_t* semaphore, uint32_t timeout_ms )
{
    bool_t result;
    RTOS_DBG("bk_rtos_get_semaphore:%dms\r\n",timeout_ms);
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    if(timeout_ms==BEKEN_WAIT_FOREVER)
        result = Lock_SempPend(*semaphore,CN_TIMEOUT_FOREVER);
    else
        result = Lock_SempPend(*semaphore,timeout_ms*mS);
    GLOBAL_INT_RESTORE();

    if(result)
    {
        return kNoErr;
    }
    else
    {
        RTOS_DBG("take semaphore failed %d \n", result);
        return kTimeoutErr;
    }
}

int rtos_get_sema_count(beken_semaphore_t* semaphore )
{
    int n;
    RTOS_DBG("bk_rtos_get_sema_count\n");
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    n = Lock_SempQueryFree(*semaphore);
    GLOBAL_INT_RESTORE();
    return n;
}

int rtos_set_semaphore( beken_semaphore_t* semaphore)
{
    RTOS_DBG("bk_rtos_get_sema_count\r\n");
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    Lock_SempPost(*semaphore);
    GLOBAL_INT_RESTORE();
    return kNoErr;
}

OSStatus rtos_deinit_semaphore(beken_semaphore_t *semaphore )
{
    RTOS_DBG("bk_rtos_deinit_semaphore:%8x\n", *semaphore);
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    Lock_SempDelete(*semaphore);
    rtos_sem_cnt--;
    GLOBAL_INT_RESTORE();
    return kNoErr;
}

OSStatus rtos_init_mutex( beken_mutex_t* mutex )
{
    RTOS_DBG("bk_rtos_init_mutex\n");
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    *mutex = Lock_MutexCreate(NULL);
    GLOBAL_INT_RESTORE();
    if ( *mutex == NULL )
    {
        return kGeneralErr;
    }

    rtos_mutex_cnt++;

    return kNoErr;
}

OSStatus rtos_lock_mutex( beken_mutex_t* mutex)
{
    RTOS_DBG("bk_rtos_lock_mutex\n");
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    Lock_MutexPend(*mutex,CN_TIMEOUT_FOREVER);
    GLOBAL_INT_RESTORE();
    return kNoErr;
}

OSStatus rtos_unlock_mutex( beken_mutex_t* mutex)
{
    RTOS_DBG("bk_rtos_unlock_mutex\n");
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    Lock_MutexPost(*mutex);
    GLOBAL_INT_RESTORE();
    return kNoErr;
}

OSStatus rtos_deinit_mutex( beken_mutex_t* mutex)
{
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    Lock_MutexDelete(*mutex);
    rtos_mutex_cnt--;
    GLOBAL_INT_RESTORE();
    return kNoErr;
}

OSStatus rtos_init_queue(beken_queue_t* queue, const char* name, uint32_t message_size, uint32_t number_of_messages)
{
    RTOS_DBG("bk_rtos_init_queue:%s,%d,%d\r\n",name,message_size,number_of_messages);
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    *queue = MsgQ_Create(number_of_messages,message_size,CN_MSGQ_TYPE_FIFO);
    GLOBAL_INT_RESTORE();
    if(*queue == NULL)
        return kGeneralErr;
    return kNoErr;
}
OSStatus rtos_push_to_queue(beken_queue_t* queue, void* message, uint32_t timeout_ms)
{
    bool result = false;
    RTOS_DBG("bk_rtos_push_to_queue:%dms\r\n",timeout_ms);
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    if(timeout_ms==BEKEN_WAIT_FOREVER)
        result = MsgQ_Send(*queue,message,(*queue)->MsgSize,CN_TIMEOUT_FOREVER,CN_MSGQ_PRIO_NORMAL);
    else
        result = MsgQ_Send(*queue,message,(*queue)->MsgSize,timeout_ms*1000,CN_MSGQ_PRIO_NORMAL);
    GLOBAL_INT_RESTORE();
    if(result)
        return kNoErr;
    else
        return kGeneralErr;
}

OSStatus rtos_pop_from_queue(beken_queue_t* queue, void* message, uint32_t timeout_ms)
{
    bool result = false;
    RTOS_DBG("bk_rtos_pop_from_queue:%dms\r\n",timeout_ms);
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    if(timeout_ms==BEKEN_WAIT_FOREVER)
        result = MsgQ_Receive(*queue,message,(*queue)->MsgSize,CN_TIMEOUT_FOREVER);
    else
        result = MsgQ_Receive(*queue,message,(*queue)->MsgSize,timeout_ms*1000);
    GLOBAL_INT_RESTORE();
    if(result)
        return kNoErr;
    else
        return kGeneralErr;
}

OSStatus rtos_deinit_queue(beken_queue_t* queue)
{
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    MsgQ_Delete(*queue);
    GLOBAL_INT_RESTORE();
    return kNoErr;
}

BOOL rtos_is_queue_empty(beken_queue_t* queue )
{
    uint32_t level = MsgQ_NumMsgs(*queue);
    if(level == 0)
    {
        return true;
    }
    return false;
}

BOOL rtos_is_queue_full(beken_queue_t* queue)
{
    uint32_t level = MsgQ_NumMsgs(*queue);
    if(level == (*queue)->MsgSize)
    {
        return true;
    }
    return false;
}


static void timer_oneshot_callback(void* parameter)
{
    beken2_timer_t *timer = (beken2_timer_t*)parameter;

    RTOS_DBG("one shot callback\n");

    if(BEKEN_MAGIC_WORD != timer->beken_magic)
    {
        return;
    }
    if (timer->function)
    {
        timer->function(timer->left_arg, timer->right_arg );
    }
}

OSStatus rtos_start_oneshot_timer( beken2_timer_t* timer)
{
    RTOS_DBG("oneshot_timer start \n");

    if(timer->handle != NULL)
    {
        djytimer_start(timer->handle);
        return kNoErr;
    }

    return kGeneralErr;
}

OSStatus rtos_stop_oneshot_timer(beken2_timer_t* timer)
{
    RTOS_DBG("oneshot_timer stop \n");

    if(timer->handle != NULL)
    {
        djytimer_stop(timer->handle);
        return kNoErr;
    }

    return kGeneralErr;
}

BOOL rtos_is_oneshot_timer_init(beken2_timer_t* timer)
{
    RTOS_DBG("oneshot_timer is init \n");
    return timer->handle ? true : false;
}

BOOL rtos_is_oneshot_timer_running(beken2_timer_t* timer)
{
    RTOS_DBG("oneshot_timer is runing \n");
    if(djytimer_get_status(timer->handle)==DJYTIMER_RUNNIG)
        return true;
    return false;
}

OSStatus rtos_oneshot_reload_timer( beken2_timer_t* timer)
{
    RTOS_DBG("reload oneshot timer\n");
    if(timer->handle==NULL)
        timer->handle = djytimer_create(timer->time_ms, 1,timer_oneshot_callback, timer);
    if(timer->handle==NULL)
        return kGeneralErr;
    djytimer_start(timer->handle);
    return kNoErr;
}

OSStatus rtos_init_oneshot_timer( beken2_timer_t *timer,
                                    uint32_t time_ms,
                                    timer_2handler_t function,
                                    void* larg,
                                    void* rarg )
{
    OSStatus ret = kNoErr;

    RTOS_DBG("create oneshot_timer \n");
    timer->function = function;
    timer->left_arg = larg;
    timer->right_arg = rarg;
    timer->beken_magic = BEKEN_MAGIC_WORD;
    timer->time_ms = time_ms;
    timer->handle = djytimer_create(time_ms, 1,timer_oneshot_callback, timer);
    if ( timer->handle == NULL )
    {
        ret = kGeneralErr;
    }

    return ret;
}

OSStatus rtos_deinit_oneshot_timer( beken2_timer_t* timer )
{
    OSStatus ret = kNoErr;

    RTOS_DBG("delete oneshot_timer \n");
    djytimer_free(timer->handle);
    timer->handle = NULL;
    timer->function = 0;
    timer->left_arg = 0;
    timer->right_arg = 0;
    timer->beken_magic = 0;

    return ret;
}

static void timer_period_callback(void* parameter)
{
    beken_timer_t *timer = (beken_timer_t*)parameter;

    RTOS_DBG("period timer callback\n");

    if (timer->function)
    {
        timer->function(timer->arg);
    }
}

OSStatus rtos_start_timer( beken_timer_t* timer)
{
    RTOS_DBG("period_timer start \n");
    return kNoErr;
}

OSStatus rtos_stop_timer(beken_timer_t* timer)
{
    RTOS_DBG("period_timer stop \n");

    if(timer->handle != NULL)
    {
        djytimer_free(timer->handle);
        return kNoErr;
    }

    return kGeneralErr;
}

BOOL rtos_is_timer_init(beken_timer_t* timer)
{
    RTOS_DBG("period_timer is init \n");
    return timer->handle ? true : false;
}

BOOL rtos_is_timer_running(beken_timer_t* timer)
{
    RTOS_DBG("period_timer is runing \n");
    return timer->handle ? true : false;
    return true;
}

OSStatus rtos_reload_timer( beken_timer_t* timer)
{
    RTOS_DBG("reload period timer\n");
    djytimer_create(timer->time_ms, -1,timer_period_callback, timer);
    return kNoErr;
}

OSStatus rtos_change_period( beken_timer_t* timer, uint32_t time_ms)
{
    uint32_t timeout_value;
    djytimer_free(timer->handle);
    timer->handle = djytimer_create(time_ms, -1,timer_period_callback, timer);
    if ( timer->handle == NULL )
    {
        return kGeneralErr;
    }
    return kNoErr;
}

OSStatus rtos_init_timer( beken_timer_t* timer, uint32_t time_ms, timer_handler_t function, void* arg)
{
    OSStatus ret = kNoErr;

    RTOS_DBG("create period_timer \n");
    timer->function = function;
    timer->arg      = arg;
    timer->time_ms = time_ms;
    timer->handle = djytimer_create(time_ms, -1,timer_period_callback, timer);
    if ( timer->handle == NULL )
    {
        ret = kGeneralErr;
    }

    return ret;
}

//OSStatus rtos_init_timer_ex( beken_timer_t* timer, const char* name, uint32_t time_ms, timer_handler_t function, void* arg)
//{
//  OSStatus ret = kNoErr;
//
//    RTOS_DBG("create period_timer_ex \n");
//    timer->function = function;
//    timer->arg      = arg;
//
//    timer->handle = rt_timer_create(name,
//                                    timer_period_callback,
//                                    timer,
//                                    rt_tick_from_millisecond(time_ms),
//                                    RT_TIMER_FLAG_PERIODIC|RT_TIMER_FLAG_SOFT_TIMER);
//    if ( timer->handle == NULL )
//    {
//        ret = kNoErr;
//    }
//
//    return ret;
//}

OSStatus rtos_deinit_timer( beken_timer_t* timer )
{
    OSStatus ret = kNoErr;

    RTOS_DBG("delete period_timer \n");
    djytimer_free(timer->handle);
    timer->handle = 0;
    timer->function = 0;
    timer->arg      = 0;
    return ret;
}

uint32_t rtos_get_timer_expiry_time( beken_timer_t* timer )
{
    // TODO:
    rt_kprintf("[TODO]:Note, rtos_get_timer_expiry_time is not implement \n");
    return 0;
}

uint32_t rtos_get_time(void)
{
    return (uint32_t)(DJY_GetSysTime()/1000);
}
