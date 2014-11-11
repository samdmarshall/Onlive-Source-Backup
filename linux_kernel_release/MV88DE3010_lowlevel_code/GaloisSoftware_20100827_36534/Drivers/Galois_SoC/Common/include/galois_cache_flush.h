/*******************************************************************************
*******************************************************************************/


#ifndef __GALOIS_CACHE_FLUSH_H__
#define __GALOIS_CACHE_FLUSH_H__
void CleanEntireDCache(void);

void CleanAndInvalidateEntireDCache(void);

void InvalidateDCacheRegion(int *adr, int size);

void CleanDCacheRegion(int *adr, int size);

void CleanAndInvalidateDCacheRegion(int *adr, int size);
#endif //__GALOIS_CACHE_FLUSH_H__
