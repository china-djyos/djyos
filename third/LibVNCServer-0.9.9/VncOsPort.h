// =======================================================================

// ģ������: VNCServer
// ģ��汾: V1.00(��ʼ���汾)
// �ļ���  ��vnc_thread.h
// ������Ա: Administrator
// ����ʱ��: 2013-3-8
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <V1.00> <2013-3-8>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================

#ifndef __VNC_THREAD_H__
#define __VNC_THREAD_H__
#include "os.h"

typedef struct MutexLCB  tagMutexCB;
bool_t  TaskCreate(ptu32_t (*task)(void),void *arg,const char *name);
tagMutexCB *MutexCreate(const char *name);
bool_t MutexDelete(tagMutexCB *mutex);
bool_t MutexWait(tagMutexCB *mutex);
void MutexPost(tagMutexCB *mutex);

#define delay(x)       DJY_EventDelay(x*1000)  //��ͳһ��������ʱ

#endif /* __VNC_THREAD_H__ */
