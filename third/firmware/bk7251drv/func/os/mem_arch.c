#include "include.h"
#include "arm_arch.h"
#include <string.h>
#include <stdlib.h>
//#include <rtthread.h>

static struct HeapCB *pMemHeap = NULL;

void os_meminit(void)
{
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    pMemHeap = Heap_FindHeap("dtcm");
    GLOBAL_INT_RESTORE();
}

INT32 os_memcmp(const void *s1, const void *s2, UINT32 n)
{
    return memcmp(s1, s2, (unsigned int)n);
}

void *os_memmove(void *out, const void *in, UINT32 n)
{
    return memmove(out, in, n);
}

void *os_memcpy(void *out, const void *in, UINT32 n)
{
    return memcpy(out, in, n);
}

void *os_memset(void *b, int c, UINT32 len)
{
    return (void *)memset(b, c, (unsigned int)len);
}

void *os_malloc(size_t size)
{
    void *ptr = NULL;
    if(pMemHeap==NULL)
        return NULL;
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    ptr = M_MallocHeap(size,pMemHeap,0);
    GLOBAL_INT_RESTORE();
    return ptr;
}

void * os_zalloc(size_t size)
{
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    void *n = (void *)os_malloc(size);
    GLOBAL_INT_RESTORE();
    if (n)
        memset(n, 0, size);
    return n;
}

void *os_realloc(void *ptr, size_t size)
{
    void *tmp;

    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    tmp = (void *)os_malloc(size);
    GLOBAL_INT_RESTORE();
    if(tmp)
    {
        memcpy(tmp, ptr, size);
        GLOBAL_INT_DISABLE();
        os_free(ptr);
        GLOBAL_INT_RESTORE();
    }

    return tmp;
}

void os_free(void *ptr)
{
    if(ptr)
    {
        GLOBAL_INT_DECLARATION();
        GLOBAL_INT_DISABLE();
        M_FreeHeap(ptr,pMemHeap);
        GLOBAL_INT_RESTORE();
    }
}

int os_memcmp_const(const void *a, const void *b, size_t len)
{
    return memcmp(a, b, len);
}
// EOF
