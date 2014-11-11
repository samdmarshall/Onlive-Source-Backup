/*******************************************************************************
*******************************************************************************/



#ifndef _MIDSRV_H_
#define _MIDSRV_H_

#include "OSAL_api.h"

#include "ICCNet.h"

#include "CCProtocol.h"




typedef struct _MV_CC_MidSrv {
	
	pMV_CC_ICCNet_t m_pICCNet;

	MV_OSAL_HANDLE_TASK_t m_Task;
	
	/* Protocol Object List */
	pMV_CC_Protocol_t m_PrList[MV_CC_Protocol_MAX];

	UINT32 m_TaskFlags;

	UINT32 m_ProtocolErrCount;
	UINT32 m_SuccessCount;
	
} MV_CC_MidSrv_t, *pMV_CC_MidSrv_t;





INT  MV_CC_MidSrvInit(pMV_CC_MidSrv_t pHandle, pMV_CC_ICCNet_t pICCNet);



INT  MV_CC_MidSrvExit(pMV_CC_MidSrv_t pHandle);





#endif
