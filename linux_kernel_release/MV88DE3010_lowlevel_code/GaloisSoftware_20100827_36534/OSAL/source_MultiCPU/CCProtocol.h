/*******************************************************************************
*******************************************************************************/



#ifndef _CCPROTOCOL_H_
#define _CCPROTOCOL_H_

#include "OSAL_api.h"





typedef struct _MV_CC_Protocol {
	
	MV_CC_Protocol_U8_t m_PrType;
	
	INT (* OnRecvDemux)(void *self, UCHAR *pFrameBuf);
	
	pMV_CC_ICCNet_t m_pICCNet;

	UINT32 m_RdCount;
	UINT32 m_RdErrCount;	
	UINT32 m_WrCount;
	UINT32 m_WrErrCount;
	
} MV_CC_Protocol_t, *pMV_CC_Protocol_t;





pMV_CC_Protocol_t MV_CC_OpenPr(MV_CC_Protocol_U8_t PrType, pMV_CC_ICCNet_t pICCNet);


INT MV_CC_ClosePr(pMV_CC_Protocol_t pHandle);


INT MV_CC_PrOnRecv(pMV_CC_Protocol_t self, UCHAR *pFrameBuf);


#endif
