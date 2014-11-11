/*******************************************************************************
*******************************************************************************/



#ifndef _ICCHAL_H_
#define _ICCHAL_H_


#include "OSAL_config.h"
#include "OSAL_type.h"

#include "ICCHAL_Platform.h"


typedef MV_OSAL_HANDLE 	MV_CC_HANDLE_HW_t;
typedef MV_OSAL_HANDLE 	*pMV_CC_HANDLE_HW_t;





#define MV_CC_HAL_FlushMemory(x)		MV_CC_HAL_FlushMemory_Platform(x)





INT MV_CC_HAL_Init(MV_CC_HANDLE_HW_t *pHandle);



INT MV_CC_HAL_Exit(MV_CC_HANDLE_HW_t *pHandle, UINT command);



PVOID MV_CC_HAL_GetFIFOArrayAddr(MV_CC_HANDLE_HW_t *pHandle);



INT MV_CC_HAL_SemWait(MV_CC_HANDLE_HW_t *pHandle);



INT MV_CC_HAL_SemPost(MV_CC_HANDLE_HW_t *pHandle, MV_OSAL_CPUID_U8_t DstCpu);

VOID MV_CC_StartStatus_Clean(VOID);

VOID MV_CC_StartStatus_set(VOID);

HRESULT MV_CC_StartStatus_Check(VOID);


#endif
