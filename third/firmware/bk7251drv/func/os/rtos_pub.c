#include <djytimer.h>
#include <error.h>
#include <rtos_pub.h>
#include <stddef.h>
#include <sys_rtos.h>
#include "systime.h"
//#include "finsh.h"

#define THREAD_TIMESLICE 5

#define RTOS_DEBUG   0
#if RTOS_DEBUG
#define RTOS_DBG(...)     rt_kprintf("[RTOS]"),rt_kprintf(__VA_ARGS__)
#else
//#define RTOS_DBG(...)    printf("[RTOS]"),printf(__VA_ARGS__)
#define RTOS_DBG(...)
#endif

void delay(INT32 num)
{
    volatile INT32 i,j;

    for(i = 0; i < num; i ++)
    {
        for(j = 0; j < 100; j ++)
            ;
    }
}

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

    *thread = Djy_EvttRegist(EN_CORRELATIVE,priority,0,0,
            function,NULL,stack_size, name);
    if(*thread != CN_EVTT_ID_INVALID)
    {
        Djy_EventPop(*thread,NULL,0,arg,0,0);
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
        Djy_EvttUnregist(*thread);
    else
        Djy_EvttUnregist(g_ptEventRunning->evtt_id);
    return kNoErr;

}

void rtos_thread_sleep(uint32_t seconds)
{
    Djy_EventDelay(seconds*1000*mS);
}

static uint32_t rtos_sem_cnt = 0;
static uint32_t rtos_mutex_cnt = 0;

OSStatus rtos_init_semaphore( beken_semaphore_t* semaphore, int maxCount )
{
    *semaphore = Lock_SempCreate(maxCount,0,CN_BLOCK_FIFO,NULL);
    rtos_sem_cnt++;
    RTOS_DBG("rtos_init_semaphore:%d\r\n",rtos_sem_cnt);
    return (*semaphore != NULL) ? kNoErr : kGeneralErr;
}

OSStatus rtos_get_semaphore(beken_semaphore_t* semaphore, uint32_t timeout_ms )
{
    bool_t result;
    RTOS_DBG("rtos_get_semaphore:%dms\r\n",timeout_ms);
    if(timeout_ms==BEKEN_WAIT_FOREVER)
        result = Lock_SempPend(*semaphore,CN_TIMEOUT_FOREVER);
    else
        result = Lock_SempPend(*semaphore,timeout_ms*mS);

    if(result)
    {
        return kNoErr;
    }
    else
    {
        printf("take semaphore failed %d \n", result);
        return kTimeoutErr;
    }
}

int rtos_get_sema_count(beken_semaphore_t* semaphore )
{
    RTOS_DBG("rtos_get_sema_count\n");
    return (int)Lock_SempQueryFree(*semaphore);
}

int rtos_set_semaphore( beken_semaphore_t* semaphore)
{
    RTOS_DBG("rtos_get_sema_count\r\n");
    Lock_SempPost(*semaphore);
    return kNoErr;    
}

OSStatus rtos_deinit_semaphore( beken_semaphore_t* semaphore )
{
    RTOS_DBG("rtos_deinit_semaphore:%8x\n", *semaphore);
    Lock_SempDelete(*semaphore);
    return kNoErr;
}

OSStatus rtos_init_mutex( beken_mutex_t* mutex )
{
    RTOS_DBG("rtos_init_mutex\n");
    *mutex = Lock_MutexCreate(NULL);
    if ( *mutex == NULL )
    {
        return kGeneralErr;
    }

    rtos_mutex_cnt++;

    return kNoErr;
}

OSStatus rtos_lock_mutex( beken_mutex_t* mutex)
{
    RTOS_DBG("rtos_lock_mutex\n");
    Lock_MutexPend(*mutex,CN_TIMEOUT_FOREVER);
    return kNoErr;
}

OSStatus rtos_unlock_mutex( beken_mutex_t* mutex)
{
    RTOS_DBG("rtos_unlock_mutex\n");
    Lock_MutexPost(*mutex);
    return kNoErr;
}

OSStatus rtos_deinit_mutex( beken_mutex_t* mutex)
{
    Lock_MutexDelete(*mutex);
    return kNoErr;
}

#if 1
OSStatus rtos_init_queue(beken_queue_t* queue, const char* name, uint32_t message_size, uint32_t number_of_messages)
{
    RTOS_DBG("rtos_init_queue:%s,%d,%d\r\n",name,message_size,number_of_messages);
    *queue = MsgQ_Create(number_of_messages,message_size,CN_MSGQ_TYPE_FIFO);
    if(*queue == NULL)
        return kGeneralErr;
    return kNoErr;
}

OSStatus rtos_push_to_queue(beken_queue_t* queue, void* message, uint32_t timeout_ms)
{
    bool result = false;
    RTOS_DBG("rtos_push_to_queue:%dms\r\n",timeout_ms);
    if(timeout_ms==BEKEN_WAIT_FOREVER)
        result = MsgQ_Send(*queue,message,(*queue)->MsgSize,CN_TIMEOUT_FOREVER,CN_MSGQ_PRIO_NORMAL);
    else
        result = MsgQ_Send(*queue,message,(*queue)->MsgSize,timeout_ms*1000,CN_MSGQ_PRIO_NORMAL);
    if(result)
        return kNoErr;
    else
        return kGeneralErr;
}

OSStatus rtos_push_to_queue_front(beken_queue_t* queue, void* message, uint32_t timeout_ms)
{
    return kGeneralErr;
    // return rt_mq_urgent(mq, message, mq->msg_size);
}

OSStatus rtos_pop_from_queue(beken_queue_t* queue, void* message, uint32_t timeout_ms)
{
    bool result = false;
    RTOS_DBG("rtos_pop_from_queue:%dms\r\n",timeout_ms);
    if(timeout_ms==BEKEN_WAIT_FOREVER)
        result = MsgQ_Receive(*queue,message,(*queue)->MsgSize,CN_TIMEOUT_FOREVER);
    else
        result = MsgQ_Receive(*queue,message,(*queue)->MsgSize,timeout_ms*1000);
    if(result)
        return kNoErr;
    else
        return kGeneralErr;
}

OSStatus rtos_deinit_queue(beken_queue_t* queue)
{
    MsgQ_Delete(*queue);
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
#else
OSStatus rtos_init_queue(beken_queue_t* queue, const char* name, uint32_t message_size, uint32_t number_of_messages)
{
    *queue = rt_mq_create(name, message_size, number_of_messages, RT_IPC_FLAG_FIFO);
    
    if(*queue == RT_NULL)
    {
        rt_kprintf("create queue failed\n");
        return RT_ERROR;
    }

    return RT_EOK;
}

OSStatus rtos_push_to_queue(beken_queue_t* queue, void* message, uint32_t timeout_ms)
{
    rt_mq_t mq = *queue;
    rt_err_t result;
    
    result = rt_mq_send(mq, message, mq->msg_size);
    if(result != RT_EOK)
    {
        rt_kprintf("mq send failed %d \n", result);
        return kGeneralErr;
    }

    return kNoErr;
}

OSStatus rtos_push_to_queue_front( beken_queue_t* queue, void* message, uint32_t timeout_ms )
{
    rt_mq_t mq = *queue;
    
    return rt_mq_urgent(mq, message, mq->msg_size);
}

OSStatus rtos_pop_from_queue( beken_queue_t* queue, void* message, uint32_t timeout_ms )
{
    rt_mq_t mq = *queue;
    rt_err_t result;

    result = rt_mq_recv(mq, message, mq->msg_size, rt_tick_from_millisecond(timeout_ms));
    if(result != RT_EOK)
    {
        rt_kprintf("%s rt_mq_recv ret:%d, ms:=%d!\r\n", __FUNCTION__, result, rt_tick_from_millisecond(timeout_ms));
        return kGeneralErr;
    }

    return kNoErr;
}

OSStatus rtos_deinit_queue(beken_queue_t* queue)
{
    if(queue != RT_NULL)
    {
        rt_mq_delete(*queue);
        *queue = RT_NULL;

        return RT_EOK;
    }

    return RT_ERROR;
}

BOOL rtos_is_queue_empty(beken_queue_t* queue )
{
    uint32_t level;
    rt_bool_t result;
    rt_mq_t mq = *queue;

    /* disable interrupt */
    level = rt_hw_interrupt_disable();
    if(mq->entry == 0)
    {
        /* enable interrupt */
        rt_hw_interrupt_enable(level);
        
        return true;
    }
    
    rt_hw_interrupt_enable(level);

    return false;
}

BOOL rtos_is_queue_full(beken_queue_t* queue )
{
    uint32_t level;
    struct rt_mq_message *msg;
    rt_mq_t mq = *queue;

    /* disable interrupt */
    level = rt_hw_interrupt_disable();

    /* get a free list, there must be an empty item */
    msg = mq->msg_queue_free;

    /* message queue is full */
    if (msg == RT_NULL)
    {
        /* enable interrupt */
        rt_hw_interrupt_enable(level);

        return false;
    }

    rt_hw_interrupt_enable(level);

    return true;
}
#endif

OSStatus rtos_delay_milliseconds( uint32_t num_ms)
{
    Djy_EventDelay(num_ms*mS);
    return kNoErr;
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
    djytimer_start(timer->handle);
    return kNoErr;
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
    return 0;
}

uint32_t rtos_get_time(void)
{
    return (uint32_t)(DjyGetSysTime()/1000);
}
