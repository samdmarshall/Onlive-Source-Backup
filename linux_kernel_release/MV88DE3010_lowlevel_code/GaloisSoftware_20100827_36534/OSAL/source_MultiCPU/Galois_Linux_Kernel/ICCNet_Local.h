/*******************************************************************************
*******************************************************************************/



#ifndef _ICCNET_LOCAL_H_
#define _ICCNET_LOCAL_H_



typedef struct _MV_CC_ICCNet {
	
	MV_CC_HANDLE_HW_t m_pHW;

	MV_CC_ICCFIFOArray_t m_FIFOArray;
	
	MV_OSAL_CPUID_U8_t m_RdCpuNow;

	MV_OSAL_CPUID_U8_t m_LocalCPU;
	
	UINT32	m_RdCount;	
	UINT32	m_RdEmptyCount;
	
	UINT32	m_WrCount;	
	UINT32	m_WrFullCount;
	UINT32	m_WrSemErrCount;
	
} MV_CC_ICCNet_t, *pMV_CC_ICCNet_t;



#endif
