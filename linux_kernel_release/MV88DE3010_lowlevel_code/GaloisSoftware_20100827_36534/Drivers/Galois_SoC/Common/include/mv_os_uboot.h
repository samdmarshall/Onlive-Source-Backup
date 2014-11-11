#include "galois_cache_flush.h"
#include "galois_type.h"
#include <common.h>
#include <malloc.h>

extern void *memcpy(void *, const void *, size_t n);
extern void *memset(void *, int, size_t n);

static inline void *mvOsMemcpy(void *dest, const void *src, size_t n)
{
	memcpy(dest, src, n);
}

static inline void *mvOsMemset(void *buf, int c, size_t n)
{
	memset(buf, c, n);
}

static inline MV_U32 mvOsIoVirtToPhy( void* pDev, void* pVirtAddr )
{
    return (MV_U32)pVirtAddr; /* ronen - need to update for CIV */
}

static inline void* mvOsIoUncachedMalloc( void* pDev, MV_U32 size, MV_U32* pPhyAddr )
{
    *pPhyAddr = (MV_U32)malloc(size);
    return (void *)(*pPhyAddr);
}

static inline void mvOsIoUncachedFree(void* pDev, MV_U32 size, MV_U32 phyAddr, void* pVirtAddr)
{
    free(pVirtAddr);
}

static inline void* mvOsIoCachedMalloc( void* pDev, MV_U32 size, MV_U32* pPhyAddr )
{
    *pPhyAddr = (MV_U32)malloc(size);
    return (void *)(*pPhyAddr);
}

static inline void mvOsIoCachedFree( void* pDev, MV_U32 size, MV_U32 phyAddr, void* pVirtAddr )
{
    free(pVirtAddr);
}

static inline MV_U32 mvOsCacheFlushInv( void* pDev, void* p, int size )
{
	CleanAndInvalidateDCacheRegion((int *)p, size);
	return (MV_U32)p;
}

static inline MV_U32 mvOsCacheInvalidate( void* pDev, void* p, int size )
{
	InvalidateDCacheRegion((int *)p, size);
    return (MV_U32)p;/* ronen - need to be filled */
}
