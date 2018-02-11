/*
 * YAFFS: Yet Another Flash File System. A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2011 Aleph One Ltd.
 *   for Toby Churchill Ltd and Brightstar Engineering
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/*
 * OS glue functions for running on a DJYOS system.
 * ��׼�����ӿڡ�
 */

#include "../yaffs2-583dbd9/yaffs2-583dbd9/direct/yaffscfg.h"
#include "../yaffs2-583dbd9/yaffs2-583dbd9/yaffs_guts.h"
#include "../yaffs2-583dbd9/yaffs2-583dbd9/direct/yaffsfs.h"
#include "../yaffs2-583dbd9/yaffs2-583dbd9/yaffs_trace.h"
#include <assert.h>

#include <errno.h>
#include <unistd.h>

// DJYOS���
#include <lock.h>
/*
 * yaffsfs_SetError() and yaffsfs_GetError()
 * Do whatever to set the system error.
 * yaffsfs_GetError() just fetches the last error.
 */

static int yaffsfs_lastError;
struct MutexLCB YaffsLock;
const char LockName[] = "YaffsLock";
void yaffsfs_SetError(int err)
{
    //Do whatever to set error
    yaffsfs_lastError = err;
    //errno = err;// todo:
}

int yaffsfs_GetLastError(void)
{
    return yaffsfs_lastError;
}

/*
 * yaffsfs_CheckMemRegion()
 * Check that access to an address is valid.
 * This can check memory is in bounds and is writable etc.
 *
 * Returns 0 if ok, negative if not.
 */
int yaffsfs_CheckMemRegion(const void *addr, size_t size, int write_request)
{
    if(!addr)
        return -1;
    return 0;
}

/*
 * yaffsfs_Lock()
 * yaffsfs_Unlock()
 * A single mechanism to lock and unlock yaffs. Hook up to a mutex or whatever.
 * Here are two examples, one using POSIX pthreads, the other doing nothing.
 *
 * If we use pthreads then we also start a background gc thread.
 */

void yaffsfs_Lock(void)
{
    Lock_MutexPend(&YaffsLock, CN_TIMEOUT_FOREVER);
}

void yaffsfs_Unlock(void)
{
    Lock_MutexPost(&YaffsLock);
}

void yaffsfs_LockInit(void)
{
    struct MutexLCB *Lock;
    Lock = Lock_MutexCreate_s(&YaffsLock, LockName);
    if(NULL == Lock)// todo��������û��ͬ�����
    {
        yaffs_trace(YAFFS_TRACE_ERROR, "yaffs: Mounting error: yaffs lock init failed!\r\n");
    }
}

/*
 * yaffsfs_CurrentTime() retrns a 32-bit timestamp.
 *
 * Can return 0 if your system does not care about time.
 */

u32 yaffsfs_CurrentTime(void)
{
#if 0
    return time(NULL);// todo:time()��δʵ��
#else
    return (u32)0x12345678;
#endif  
}


/*
 * yaffsfs_malloc()
 * yaffsfs_free()
 *
 * Functions to allocate and free memory.
 */
static void *pHeapFirst;
void *yaffsfs_malloc(size_t size)
{
	extern void *(*M_MallocHeap)(ptu32_t size,pHeap_t Heap,u32 timeout);

	if(NULL != pHeapFirst)
	{
		return M_MallocHeap(size, (pHeap_t)pHeapFirst, CN_TIMEOUT_FOREVER);
	}
	else
	{
		return malloc(size);
	}
}

void yaffsfs_free(void *ptr)
{
	struct HeapCB *sdram2;
	extern void  (*M_FreeHeap)(void *pl_mem, pHeap_t Heap);

	if(NULL != pHeapFirst)
	{
		return M_FreeHeap(ptr, (pHeap_t)pHeapFirst);
	}
	else
	{
		return free(ptr);
	}
}

void yaffsfs_OSInitialisation(void)
{
    yaffsfs_LockInit(); // ��ʼ���ļ�������
	extern struct HeapCB *M_FindHeap(const char *HeapName);
	pHeapFirst = M_FindHeap("extram"); // ����ʹ��extram�ϵ�heap
}

/*
 * yaffs_bug_fn()
 * Function to report a bug.
 */
//-----------------------------------------------------------------------------
//����: ����BUG()�ľ���ʵ��.
//����: file_name -- �ļ���,__FILE__,�������֧��.
//      line_no -- �к�,__LINE__,�������֧��.
//����: >0 -- �ɹ�; 0 -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
void yaffs_bug_fn(const char *file_name, int line_no)
{
    printf("yaffs bug detected %s:%d\n",
        file_name, line_no);
    assert(0);
}
