#include "include.h"
#include "arm_arch.h"
#include <string.h>
#if CFG_SUPPORT_DJYOS       //CK
#include "driver/entry/arch.h"      //lst
#endif

#if(CFG_SUPPORT_DJYOS)      //lst
#include <stdlib.h>

static struct HeapCB *pMemHeap = NULL;
void os_meminit(void)
{
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    pMemHeap = Heap_FindHeap("dtcm");
    GLOBAL_INT_RESTORE();
}
#endif     //for #if(CFG_SUPPORT_DJYOS)

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
#if (CFG_SUPPORT_DJYOS) && (CFG_SOC_NAME == SOC_BK7221U)    //lst
void *dtcm_malloc(size_t size)
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
#endif      //#if (CFG_SUPPORT_DJYOS) && (CFG_SOC_NAME == SOC_BK7221U)

void *os_malloc(size_t size)
{
    void *ptr = NULL;
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    ptr = M_Malloc(size,CN_TIMEOUT_FOREVER);
    GLOBAL_INT_RESTORE();
    return ptr;
}

void * os_zalloc(size_t size)
{
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    void *n = M_Malloc(size,CN_TIMEOUT_FOREVER);
    GLOBAL_INT_RESTORE();
    if (n)
        os_memset(n, 0, size);
    return n;
}

void *os_realloc(void *ptr, size_t size)
{
    void *tmp;

    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    tmp = M_Malloc(size,CN_TIMEOUT_FOREVER);
    GLOBAL_INT_RESTORE();
    if(tmp)
    {
        os_memcpy(tmp, ptr, size);
        GLOBAL_INT_DISABLE();
        free(ptr);
        //chenws: 这里可能是专用堆的malloc, 补充专用堆的free.
        if(pMemHeap) M_FreeHeap(ptr,pMemHeap);
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
        free(ptr);
        //chenws: 这里可能是专用堆的malloc, 补充专用堆的free.
        if(pMemHeap) M_FreeHeap(ptr,pMemHeap);
        GLOBAL_INT_RESTORE();
    }
}

int os_memcmp_const(const void *a, const void *b, size_t len)
{
    return memcmp(a, b, len);
}
// EOF
