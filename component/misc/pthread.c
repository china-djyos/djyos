/*
 * pthread.c
 *
 *  Created on: 2015��4��25��
 *      Author: zhangqf
 */
#include <pthread.h>

bool_t pthread_mutex_lock(struct MutexLCB *mutex)
{
    return  Lock_MutexPend(mutex, CN_TIMEOUT_FOREVER);
}
bool_t pthread_mutex_unlock(struct MutexLCB *mutex)
{
    Lock_MutexPost(mutex);
    return true;
}
bool_t pthread_mutex_init(struct MutexLCB *mutex, const char *name)
{
    if(NULL != Lock_MutexCreate_s(mutex,name))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool_t pthread_mutex_destroy(struct MutexLCB *mutex)
{
    return Lock_MutexDelete_s(mutex);
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

bool_t  pthread_cond_signal(int *cond)
{
    return false;
}
bool_t  pthread_cond_wait(int *cond, struct MutexLCB *mutex)
{
    return false;
}
bool_t  pthread_cond_init(int *cond,char *name)
{
    return false;
}
bool_t  pthread_cond_destroy(int *cond)
{

    return false;
}

int pthread_create(pthread_t  *threadId, const pthread_attr_t *attr,\
       ptu32_t (*taskroutine)(void ),void *arg)
{
    int result = -1;
    u16 evttID;
    u16 eventID;

    evttID = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,\
                           taskroutine,NULL,0x1000,NULL);
    if(evttID != CN_EVTT_ID_INVALID)
    {
        eventID = DJY_EventPop(evttID,NULL,0,(ptu32_t)arg,0,0);
        if(CN_EVENT_ID_INVALID != eventID)
        {
            if(NULL != threadId)
            {
                *threadId = (evttID<<16)|eventID;
            }
            result = 0;
        }
    }
    return result;
}



int exit(int no)
{
    while(1)
    {

    }
    return 0;
}
#pragma GCC diagnostic pop

//get the current process id
//we has no multi process yet,so we use the current taskID to replace it
int getpid(void)
{
	return (int)DJY_GetMyEventId();
}


