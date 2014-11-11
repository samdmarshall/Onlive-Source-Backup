/*******************************************************************************
*******************************************************************************/



#ifndef __COMMON_TYPE_H__
#define __COMMON_TYPE_H__

#if defined( __LINUX__ )
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>

#ifdef ANDROID

#define fopen64 fopen
#define dirent64 dirent
#define open64 open

#endif




#elif defined( __LINUX_KERNEL__ )
/* Don't add files here, please include headers in your file */
#include <linux/kernel.h>
#include <linux/string.h>
#elif defined( __ECOS__ )

#include <cyg/kernel/kapi.h>


#ifndef CYGPKG_KERNEL_THREADS_DESTRUCTORS
#define CYGPKG_KERNEL_THREADS_DESTRUCTORS
#endif

#elif defined( WIN32 )
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#elif defined( __UBOOT__ )
#include <common.h>
#elif defined (ARM) && defined (__ARMCC_VERSION__)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#elif defined( NON_OS )
#else
#error ERROR: OS not specified
#endif

#if defined(__UBOOT__) || defined(__LINUX_KERNEL__) || defined( NON_OS )
#else
#include <assert.h>
#endif

#include "com_config.h"

#if defined( WIN32 )
#else
typedef unsigned char       UCHAR;
typedef char                CHAR;
#if defined(__UBOOT__) || defined(__LINUX_KERNEL__) || defined( NON_OS )
#else
typedef wchar_t             WCHAR;
#endif
#ifndef BOOL
typedef UCHAR               BOOL;
#endif
typedef UCHAR               BOOLEAN;
typedef short               SHORT;
typedef unsigned short      USHORT;
typedef int                 INT;
typedef unsigned int        UINT; 
typedef long                LONG;
typedef unsigned long       ULONG;
typedef long long			LONGLONG;
typedef unsigned long long	ULONGLONG;
typedef void                VOID;
typedef void*               PTR;
typedef void**              PHANDLE;
typedef void*               HANDLE;
typedef void*               PVOID;

typedef UCHAR               BYTE;

typedef CHAR                INT8;
typedef UCHAR               UINT8;
typedef short               INT16;
typedef unsigned short      UINT16;
typedef int                 INT32;
typedef unsigned int        UINT32; 
typedef long long           INT64;
typedef unsigned long long  UINT64; 
typedef unsigned int        SIZE_T;

typedef signed int			HRESULT;

typedef struct
{
	UINT	Data1;
	UINT16	Data2;
	UINT16	Data3;
	UCHAR	Data4[8];
} GUID;
#endif

#ifndef _MAX_PATH
#define _MAX_PATH						260
#endif

#ifndef TRUE
#define TRUE							(1)
#endif
#ifndef FALSE
#define FALSE							(0)
#endif

#ifndef true
#define true							(1)
#endif
#ifndef false
#define false							(0)
#endif

#ifndef True
#define True							(1)
#endif
#ifndef False
#define False							(0)
#endif

#ifndef NULL
#define NULL			((void *)0)
#endif

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif
#ifndef INOUT
#define INOUT
#endif

#define MV_CAST(type)	type


typedef INT32 F16DOT16;
#define F16DOT16_HIBITS 16
#define F16DOT16_LOBITS 16
#define itoF16DOT16(x)  ((x) << F16DOT16_LOBITS)
#define F16DOT16_0      0
#define F16DOT16_1      itoF16DOT16(1)
#define F16DOT16_half   (F16DOT16_1 >> 1)
#define ftoF16DOT16(float_value) ((F16DOT16)((float_value)*F16DOT16_1))

#define F16DOT16_LOMASK (F16DOT16_1 - 1)
#define F16DOT16_HIMASK itoF16DOT16((1 << F16DOT16_HIBITS) - 1)
#define F16DOT16_SIGNBIT      (1 << (F16DOT16_HIBITS+F16DOT16_LOBITS-1))

#define MAKE_F16DOT16(float_value) ((F16DOT16)((float_value)*F16DOT16_1))
#define F16DOT16_to_int(value) ((int)((F16DOT16)(value)) >> F16DOT16_LOBITS)
#define F16DOT16_to_float(value) (((float)1.0f * (F16DOT16)(value))/F16DOT16_1)
#define F16DOT16_to_double(value) (((double)1.0 * (F16DOT16)(value))/F16DOT16_1)
#define F16DOT16_mul(a,b) ((F16DOT16)(((long long)(a)*(b)) >> F16DOT16_LOBITS))
#define F16DOT16_div(a,b) ((F16DOT16)((((long long)(a)) << F16DOT16_LOBITS)/(b)))




#ifdef WIN32
#define ENABLE_BOUNDCHECKER
#define ENABLE_TRACE
#endif

#define GaloisCalloc		calloc

#define GALOIS_CACHE_LINE			32

#ifdef BERLIN_SINGLE_CPU
/* malloc cacheable virtual address */
#define GALOIS_CACHE_MALLOC(x)  	MV_SHM_GetCacheVirtAddr(MV_SHM_Malloc(x, GALOIS_CACHE_LINE))
#define GALOIS_NONCACHE_MALLOC(x)	MV_SHM_GetNonCacheVirtAddr(MV_SHM_Malloc(x, GALOIS_CACHE_LINE))
#define GALOIS_8_MALLOC(x)      MV_SHM_GetCacheVirtAddr(MV_SHM_Malloc(x, 8))
#define GALOIS_32_MALLOC(x)     MV_SHM_GetCacheVirtAddr(MV_SHM_Malloc(x, 32))
#define GALOIS_64_MALLOC(x)     MV_SHM_GetCacheVirtAddr(MV_SHM_Malloc(x, 64))
#define GALOIS_128_MALLOC(x)    MV_SHM_GetCacheVirtAddr(MV_SHM_Malloc(x, 128))
#define GALOIS_256_MALLOC(x)    MV_SHM_GetCacheVirtAddr(MV_SHM_Malloc(x, 256))
#define GALOIS_512_MALLOC(x)    MV_SHM_GetCacheVirtAddr(MV_SHM_Malloc(x, 512))
#define GALOIS_1K_MALLOC(x)     MV_SHM_GetCacheVirtAddr(MV_SHM_Malloc(x, 1024))
#define GALOIS_2K_MALLOC(x)     MV_SHM_GetCacheVirtAddr(MV_SHM_Malloc(x, 2048))
#define GALOIS_4K_MALLOC(x)     MV_SHM_GetCacheVirtAddr(MV_SHM_Malloc(x, 4096))

#define GALOIS_CACHE_FREE(x)    	MV_SHM_Free(MV_SHM_RevertCacheVirtAddr(x))
#define GALOIS_NONCACHE_FREE(x)       	MV_SHM_Free(MV_SHM_RevertNonCacheVirtAddr(x))
#define GALOIS_8_FREE(x)        MV_SHM_Free(MV_SHM_RevertCacheVirtAddr(x))
#define GALOIS_32_FREE(x)       MV_SHM_Free(MV_SHM_RevertCacheVirtAddr(x))	
#define GALOIS_64_FREE(x)       MV_SHM_Free(MV_SHM_RevertCacheVirtAddr(x))
#define GALOIS_128_FREE(x)      MV_SHM_Free(MV_SHM_RevertCacheVirtAddr(x))
#define GALOIS_256_FREE(x)      MV_SHM_Free(MV_SHM_RevertCacheVirtAddr(x))
#define GALOIS_512_FREE(x)      MV_SHM_Free(MV_SHM_RevertCacheVirtAddr(x))
#define GALOIS_1K_FREE(x)       MV_SHM_Free(MV_SHM_RevertCacheVirtAddr(x))
#define GALOIS_2K_FREE(x)       MV_SHM_Free(MV_SHM_RevertCacheVirtAddr(x))
#define GALOIS_4K_FREE(x)       MV_SHM_Free(MV_SHM_RevertCacheVirtAddr(x))

/* malloc non-cacheable physical address */
#define GALOIS_CACHE_MALLOC_PHY(x)  MV_SHM_GetNonCachePhysAddr(MV_SHM_Malloc(x, GALOIS_CACHE_LINE))
#define GALOIS_8_MALLOC_PHY(x)      MV_SHM_GetNonCachePhysAddr(MV_SHM_Malloc(x, 8))
#define GALOIS_32_MALLOC_PHY(x)	    MV_SHM_GetNonCachePhysAddr(MV_SHM_Malloc(x, 32))
#define GALOIS_64_MALLOC_PHY(x)     MV_SHM_GetNonCachePhysAddr(MV_SHM_Malloc(x, 64))
#define GALOIS_128_MALLOC_PHY(x)    MV_SHM_GetNonCachePhysAddr(MV_SHM_Malloc(x, 128))
#define GALOIS_256_MALLOC_PHY(x)    MV_SHM_GetNonCachePhysAddr(MV_SHM_Malloc(x, 256))
#define GALOIS_512_MALLOC_PHY(x)    MV_SHM_GetNonCachePhysAddr(MV_SHM_Malloc(x, 512))
#define GALOIS_1K_MALLOC_PHY(x)	    MV_SHM_GetNonCachePhysAddr(MV_SHM_Malloc(x, 1024))
#define GALOIS_2K_MALLOC_PHY(x)     MV_SHM_GetNonCachePhysAddr(MV_SHM_Malloc(x, 2048))
#define GALOIS_4K_MALLOC_PHY(x)     MV_SHM_GetNonCachePhysAddr(MV_SHM_Malloc(x, 4096))

#define GALOIS_CACHE_FREE_PHY(x)    MV_SHM_Free(MV_SHM_RevertNonCachePhysAddr(x))
#define GALOIS_8_FREE_PHY(x)	    MV_SHM_Free(MV_SHM_RevertNonCachePhysAddr(x))
#define GALOIS_32_FREE_PHY(x)       MV_SHM_Free(MV_SHM_RevertNonCachePhysAddr(x))	
#define GALOIS_64_FREE_PHY(x)       MV_SHM_Free(MV_SHM_RevertNonCachePhysAddr(x))
#define GALOIS_128_FREE_PHY(x)	    MV_SHM_Free(MV_SHM_RevertNonCachePhysAddr(x))
#define GALOIS_256_FREE_PHY(x)	    MV_SHM_Free(MV_SHM_RevertNonCachePhysAddr(x))
#define GALOIS_512_FREE_PHY(x)	    MV_SHM_Free(MV_SHM_RevertNonCachePhysAddr(x))
#define GALOIS_1K_FREE_PHY(x)       MV_SHM_Free(MV_SHM_RevertNonCachePhysAddr(x))
#define GALOIS_2K_FREE_PHY(x)       MV_SHM_Free(MV_SHM_RevertNonCachePhysAddr(x))
#define GALOIS_4K_FREE_PHY(x)       MV_SHM_Free(MV_SHM_RevertNonCachePhysAddr(x))
#else
#define GALOIS_CACHE_MALLOC(x)  GaloisAlignMalloc((x), GALOIS_CACHE_LINE)
#define GALOIS_8_MALLOC(x)	    GaloisAlignMalloc((x), 8)	
#define GALOIS_32_MALLOC(x)	    GaloisAlignMalloc((x), 32)	
#define GALOIS_64_MALLOC(x)     GaloisAlignMalloc((x), 64)	
#define GALOIS_128_MALLOC(x)    GaloisAlignMalloc((x), 128)	
#define GALOIS_256_MALLOC(x)	GaloisAlignMalloc((x), 256)	
#define GALOIS_512_MALLOC(x)	GaloisAlignMalloc((x), 512)	
#define GALOIS_1K_MALLOC(x)		GaloisAlignMalloc((x), 1024)	
#define GALOIS_2K_MALLOC(x)		GaloisAlignMalloc((x), 2048)
#define GALOIS_4K_MALLOC(x)		GaloisAlignMalloc((x), 4096)

#define GALOIS_CACHE_FREE   GaloisAlignFree
#define GALOIS_8_FREE		GaloisAlignFree
#define GALOIS_32_FREE      GaloisAlignFree	
#define GALOIS_64_FREE	    GaloisAlignFree
#define GALOIS_128_FREE	    GaloisAlignFree
#define GALOIS_256_FREE	    GaloisAlignFree
#define GALOIS_512_FREE	    GaloisAlignFree
#define GALOIS_1K_FREE	    GaloisAlignFree
#define GALOIS_2K_FREE	    GaloisAlignFree
#define GALOIS_4K_FREE	    GaloisAlignFree
#endif

#ifdef ENABLE_DEBUG
#ifdef ENABLE_BOUNDCHECKER
#define GaloisMalloc(size) BoundChecker_Malloc((size),0, (CHAR*) __FILE__, __LINE__)
#define GaloisCalloc(numElements, sizeOfElement) BoundChecker_Calloc(numElements, sizeOfElement, (CHAR*) __FILE__, __LINE__)
#define GaloisAlignMalloc(size,align) BoundChecker_Malloc((size),(align), (CHAR*) __FILE__, __LINE__)
#define GaloisFree(p) BoundChecker_Free(p,FALSE) 
#define GaloisAlignFree(p)    BoundChecker_Free((p),TRUE) 
#else
#define GaloisMalloc		malloc
#define GaloisCalloc		calloc
#define GaloisFree(p)								\
	do {									\
		if ((p) != NULL) {						\
			free((void *)(p));					\
			(p) = NULL;						\
		}								\
	} while (0)
#define GaloisAlignMalloc Galois_align_malloc    
#define GaloisAlignFree   Galois_align_free    
#endif
#else
#define GaloisMalloc		malloc
#define GaloisFree(p)			\
	do {						\
		free((void *)(p));		\
		(p) = NULL;				\
	} while (0)	
#define GaloisAlignMalloc Galois_align_malloc
#define GaloisAlignFree   Galois_align_free    
#endif

#if defined(__LINUX__) || !defined(SMALL_MEM_POOL_SUPPORT)
#define	CommonMemPoolMalloc	GaloisMalloc
#define CommonMemPoolFree	GaloisFree
#endif

#define	BDGMalloc	CommonMemPoolMalloc
#define BDGFree		CommonMemPoolFree
#define GFXMalloc	CommonMemPoolMalloc
#define GFXFree		CommonMemPoolFree
#define BDRESUBMalloc		CommonMemPoolMalloc
#define BDRESUBFree		CommonMemPoolFree

#define GaloisMemmove		memmove
#define GaloisMemcpy		memcpy
#define GaloisMemClear(buf, n)	memset((buf), 0, (n))
#define GaloisMemSet(buf, c, n)	memset((buf), (c), (n))
#define GaloisMemComp(buf1, buf2, n)	memcmp((buf1), (buf2), (n))

#define UINT_ADDR(a, b)		((UINT *)(a) + ((b) >> 2))
#define USHORT_ADDR(a,b)	((USHORT *)(a) + ((b) >> 1))
#define UCHAR_ADDR(a, b)	((UCHAR *)(a) + (b))
#define INT_ADDR(a, b)		((INT *)(a) + ((b) >> 2))
#define SHORT_ADDR(a,b)		((SHORT *)(a) + ((b) >> 1))
#define CHAR_ADDR(a, b)		((CHAR *)(a) + (b))



#ifndef MIN
#define MIN(A, B)		((A) < (B) ? (A) : (B))
#endif
#ifndef MAX
#define MAX(A, B)		((A) > (B) ? (A) : (B))
#endif

#ifndef SWAP
#define SWAP(a, b) \
	do \
	{ \
		(a) ^= (b); \
		(b) ^= (a); \
		(a) ^= (b); \
	} while (0)
#endif

#if defined(__UBOOT__) || defined(__LINUX_KERNEL__) || defined( NON_OS )
#else
#include "debug.h"
#endif

#ifdef ENABLE_DEBUG

#if defined( __LINUX_KERNEL__ )
#define DBGPRINTF(x)        printk x
#define ERRPRINTF(x)        printk x
#define LOGPRINTF(x)        printk x
#define ASSERT(x)           printk (x)
#else
#define DBGPRINTF(x)        printf x
#define ERRPRINTF(x)        printf x
#define LOGPRINTF(x)        printf x
#endif	// defined( __LINUX_KERNEL__ )

#else

#if defined( __LINUX_KERNEL__ )
#define ASSERT(x)           do {;} while (0)
#endif	// defined( __LINUX_KERNEL__ )
#define DBGPRINTF(x)		do {;} while (0)
#define	ERRPRINTF(x)		do {;} while (0)
#define LOGPRINTF(x)		do {;} while (0)

#endif  // ENABLE_DEBUG

#ifndef ASSERT
#define ASSERT(x)           MV_ASSERT(x)
#endif  // ASSERT

/*common definition*/
#define KB  				(0x400)
#define MB 					(0x100000)

#ifdef CPU_BIG_ENDIAN
#define FCC_GEN(a, b, c, d)		(((a)<<24) |((b)<<16) | ((c)<<8) | (d))
#else
#define FCC_GEN(a, b, c, d)		(((d)<<24) |((c)<<16) | ((b)<<8) | (a))
#endif

#define GaloisMemberIndexInArray(array, member) \
	(((unsigned long)(member) - (unsigned long)(array)) / sizeof(*(member))) 

#ifdef __cplusplus
extern "C" {
#endif

#define GaloisSizeOfArray(array) (sizeof(array)/sizeof(array[0]))
	
#ifdef __cplusplus
}
#endif

#define MV_CC_DEFAULT_NOWAIT		(0)						// Time out = 0 , no wait
#define MV_CC_DEFAULT_TIMEOUT_64	(20*1000ll*1000*1000)	// 1s = 1000ms * 1000us * 1000ns
#define MV_CC_DEFAULT_TIMEOUT_U64	(20*1000ull*1000*1000)	// 1s = 1000ms * 1000us * 1000ns
#define MV_CC_DEFAULT_TIMEOUTMS		(20*1000)				// 1s = 1000ms

#define MV_CC_NSEC_PER_SEC_64		(1*1000ll*1000*1000)	// 1s = 1000ms * 1000us * 1000ns
#define MV_CC_USEC_PER_SEC_64		(1*1000ll*1000)			// 1s = 1000ms * 1000us
#define MV_CC_MSEC_PER_SEC_64		(1*1000ll)				// 1s = 1000ms
#define MV_CC_NSEC_PER_MSEC_64		(1*1000ll*1000)			// 1ms = 1000us * 1000ns

#define MV_CC_NSEC_PER_SEC_U64		(1*1000ull*1000*1000)	// 1s = 1000ms * 1000us * 1000ns
#define MV_CC_USEC_PER_SEC_U64		(1*1000ull*1000)		// 1s = 1000ms * 1000us
#define MV_CC_MSEC_PER_SEC_U64		(1*1000ull)				// 1s = 1000ms
#define MV_CC_NSEC_PER_MSEC_U64		(1*1000ull*1000)		// 1ms = 1000us * 1000ns



#define B2N_16(x)                               \
  x = ((((x) & 0xff00) >> 8) |                  \
       (((x) & 0x00ff) << 8))

#define B2N_32(x)                               \
  x = ((((x) & 0xff000000) >> 24) |             \
       (((x) & 0x00ff0000) >>  8) |             \
       (((x) & 0x0000ff00) <<  8) |             \
       (((x) & 0x000000ff) << 24))

#define B2N_64(x)                               \
  x = ((((x) & 0xff00000000000000) >> 56) |     \
       (((x) & 0x00ff000000000000) >> 40) |     \
       (((x) & 0x0000ff0000000000) >> 24) |     \
       (((x) & 0x000000ff00000000) >>  8) |     \
       (((x) & 0x00000000ff000000) <<  8) |     \
       (((x) & 0x0000000000ff0000) << 24) |     \
       (((x) & 0x000000000000ff00) << 40) |     \
       (((x) & 0x00000000000000ff) << 56))

#include "galois_alloc.h"


#include "BoundChecker.h"
#include "Trace.h"

#ifdef BERLIN_SINGLE_CPU
#include "generic_buf.h"
#endif  //BERLIN_SINGLE_CPU

#endif	// #ifndef __COMMON_TYPE__
