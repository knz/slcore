#ifndef TLS_ARCH_MTALPHA_H
#define TLS_ARCH_MTALPHA_H

/*** Architecture-specific services ***/

#include <stdlib.h>
#include <stdint.h>
#include <svp/mgsim.h>

#include "malloc/mtmalloc.h"

/* EXT_FREE(P): delegate to the external memory allocator */
#define EXT_FREE excl_dlfree
/* EXT_MALLOC(P): delegate to the external memory allocator */
#define EXT_MALLOC excl_dlmalloc

/* EXT_REALLOC(P): delegate to the external memory allocator */
#define EXT_REALLOC excl_dlrealloc

/* MAP_STORAGE(P, Sz): map storage, return != 0 if successful */
static forceinline
int MAP_STORAGE(void *p, size_t sz)
{
    unsigned l2 = __builtin_ctz(sz);                          
    assert((l2 >= 12) && ((l2 - 12) < 8));                  
    mgsim_control((unsigned long)p, MGSCTL_TYPE_MEM, MGSCTL_MEM_MAP, l2-12); 
    return 1;                                                          
}
#define MAP_STORAGE_FAIL 0

#define RELEASE_STORAGE

static forceinline
void UNMAP_STORAGE(void *p, size_t sz) 
{
    unsigned l2 = __builtin_ctz(sz);                          
    assert((l2 >= 12) && ((l2 - 12) < 8));                  
    mgsim_control((unsigned long)p, MGSCTL_TYPE_MEM, MGSCTL_MEM_UNMAP, l2-12); 
}


static forceinline
void *TLS_BOTTOM(void) {
    void *ret;
    asm("ldbp %0" : "=r"(ret));
    return ret;
}

static forceinline
void *TLS_TOP(void) {
    void *ret;
    asm("ldfp %0" : "=r"(ret));
    return ret;
}

/* TLS_BASE(): return base address of all TLS spaces */
static forceinline
void *TLS_BASE(void)
{
    return (void*)(1ULL << 63);
}

/* TLS_NEXT(P): return address of space after P, 0 if no more space */
static forceinline
void *TLS_NEXT(void *p)
{
    uintptr_t tls_size = (uintptr_t)TLS_TOP() - (uintptr_t)TLS_BOTTOM();
    char *next = ((char*)p) + tls_size;
    if (!(((uintptr_t)next >= (uintptr_t)TLS_BASE()) &&
          ((uintptr_t)next <= ~(uintptr_t)0)))
        return 0;
    return next;
}


/* IS_TLS_PTR(P): true if P is managed by this heap allocator */
static forceinline
uintptr_t IS_TLS_PTR(void *ptr)
{
    return (((uintptr_t)ptr) & (((uintptr_t)1) << (sizeof(void*)*CHAR_BIT-1)));
}

/* IS_TLS_LOCAL(P): true if P is in the current thread's TLS address space */
static forceinline
bool IS_TLS_LOCAL(void *ptr)
{
    uintptr_t uptr = (uintptr_t)ptr;
    uintptr_t bottom = (uintptr_t)TLS_BOTTOM();
    uintptr_t top = (uintptr_t)TLS_TOP();
    return ((uptr > bottom) && (uptr < (top/2 + bottom/2))); // (t+b)/2 will overflow!
}

/* CURRENT_CB(): address of the control block for the current thread */
static forceinline
struct control_block* CURRENT_CB(void)
{
    return (struct control_block*)TLS_BOTTOM();
}

/* NR_SLOTS_IN_TLS(): maximum number of slots in the current thread's TLS */
static forceinline
size_t NR_SLOTS_IN_TLS(void)
{
    uintptr_t bottom = (uintptr_t)TLS_BOTTOM();
    uintptr_t top = (uintptr_t)TLS_TOP();
    uintptr_t med = top/2 + bottom/2;
    return (med - bottom)/SLOT_SIZE;
}


#endif
