/*******************************************************************************
*******************************************************************************/



#ifndef _ICCFIFOARRAY_H_
#define _ICCFIFOARRAY_H_

#include "OSAL_type.h"
#include "ICCFIFO.h"

#if defined( __LINUX_KERNEL__ )

#include <linux/version.h>

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,24)
	#include <asm/semaphore.h>
#else
	#include <linux/semaphore.h> 
#endif

#else

#include "OSAL_api.h"

#endif




typedef struct _MV_CC_ICCFIFOArray {
	
	UINT8 m_LocalCPU;

#if defined( __LINUX_KERNEL__ )
	struct semaphore m_ArrayWrSem[MV_OSAL_CPU_NUM];
#else	
	MV_OSAL_HANDLE_SEM_t m_ArrayWrSem[MV_OSAL_CPU_NUM];
#endif
	
	UINT32 m_ArrayWrCount[MV_OSAL_CPU_NUM];	
	UINT32 m_ArrayRdCount[MV_OSAL_CPU_NUM];
	
	MV_CC_ICCFIFO_t *m_pArray[MV_OSAL_CPU_NUM][MV_OSAL_CPU_NUM];
	
} MV_CC_ICCFIFOArray_t, *pMV_CC_ICCFIFOArray_t;


INT  MV_CC_ICCFIFOArrayInit(pMV_CC_ICCFIFOArray_t pHandle,
							void *pICCFIFOArrayStart,
							MV_OSAL_CPUID_U8_t ThisCPU);



INT  MV_CC_ICCFIFOArrayDestroy(pMV_CC_ICCFIFOArray_t pHandle);




INT MV_CC_ICCFIFOArrayRead(	pMV_CC_ICCFIFOArray_t pHandle, 
							MV_OSAL_CPUID_U8_t SrcCPU, 
							UCHAR *pDstBuf);


INT MV_CC_ICCFIFOArrayWrite(pMV_CC_ICCFIFOArray_t pHandle, 
							MV_OSAL_CPUID_U8_t DstCPU, 
							UCHAR *pSrcBuf);

#endif
