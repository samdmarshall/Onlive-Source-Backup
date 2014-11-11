#include "galois_cache_flush.h"
#include "galois_type.h"
#include <linux/types.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/pci.h>

static inline void mvOsMemcpy(void *dest, const void *src, size_t n)
{
	memcpy(dest, src, n);
}

static inline void mvOsMemset(void *buf, int c, size_t n)
{
	memset(buf, c, n);
}

static inline MV_U32 mvOsIoVirtToPhy(void* pDev, void* pVirtAddr)
{
	/* return pVirtAddr - 0xC000000 */
	return pci_map_single(pDev, pVirtAddr, 0, PCI_DMA_BIDIRECTIONAL);
}

/*
 * Non-$ malloc
 */
static inline void* mvOsIoUncachedMalloc(void* pDev, MV_U32 size, MV_U32* pPhyAddr)
{
	return pci_alloc_consistent(pDev, size, (dma_addr_t *)pPhyAddr);
}

static inline void mvOsIoUncachedFree(void* pDev, MV_U32 size, MV_U32 phyAddr, void* pVirtAddr)
{
	return pci_free_consistent(pDev, size, pVirtAddr, (dma_addr_t)phyAddr);
}

/*
 * $ malloc
 */
static inline void* mvOsIoCachedMalloc(void* pDev, MV_U32 size, MV_U32* pPhyAddr)
{
	void *p = kmalloc(size, GFP_KERNEL);
	*pPhyAddr = pci_map_single(pDev, p, 0, PCI_DMA_BIDIRECTIONAL);
	return p;
}

static inline void mvOsIoCachedFree(void* pDev, MV_U32 size, MV_U32 phyAddr, void* pVirtAddr)
{
	return kfree(pVirtAddr);
}

/*
 * D-cache clean-invalidate
 */
static inline MV_U32 mvOsCacheFlushInv( void* pDev, void* p, int size )
{
	CleanAndInvalidateDCacheRegion((int *)p, size);
	return (MV_U32)p;
}

/*
 * D-cache invalidate
 */
static inline MV_U32 mvOsCacheInvalidate( void* pDev, void* p, int size )
{
	InvalidateDCacheRegion((int *)p, size);
    return (MV_U32)p;
}

