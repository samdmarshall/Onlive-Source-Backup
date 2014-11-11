/*******************************************************************************
*******************************************************************************/



#ifndef _CCDSS_H_
#define _CCDSS_H_

#if defined( __LINUX_KERNEL__ )

#include <linux/spinlock.h>

#endif

#include "OSAL_api.h"

#include "GSList.h"
#include "ICCNet.h"
#include "CCProtocol.h"

#define DSPPacket_HeadStart(x)				(UCHAR *)((x) + MV_CC_CCDSP_HEAD_BYTEOFFSET)
#define DSPPacket_DataStart(x)				(UCHAR *)((x) + MV_CC_CCDSP_DATA_BYTEOFFSET)
#define DSPPacket_HeadGEN(a, b, c, d)		((((UINT32)(d))<<24) |(((UINT32)(c))<<16) | (((UINT32)(b))<<8) | ((UINT32)(a)))
#define MV_CC_DSS_REMOTECPU_NONE 			(MV_OSAL_CPU_NUM)

#define MV_CC_SID_BIT_DYNAMIC 				(0x80000000)
#define MV_CC_SID_BIT_NORMAL 				(0x00000000)
#define MV_CC_SID_BIT_CPU0	 				(0x00000000)
#define MV_CC_SID_BIT_CPU1	 				(0x01000000)
#define MV_CC_SID_BIT_CPUX	 				(0x3F000000)

#if ( CPUINDEX == MV_OSAL_CPU_0 )
	#define MV_CC_DSS_REMOTECPU_INIT		MV_OSAL_CPU_1
	#define MV_CC_SID_BIT_LOCAL				MV_CC_SID_BIT_CPU0
#elif ( CPUINDEX == MV_OSAL_CPU_1 )
	#define MV_CC_DSS_REMOTECPU_INIT		MV_OSAL_CPU_0
	#define MV_CC_SID_BIT_LOCAL				MV_CC_SID_BIT_CPU1
#else
	#define MV_CC_DSS_REMOTECPU_INIT		MV_CC_DSS_REMOTECPU_NONE
	#define MV_CC_SID_BIT_LOCAL				MV_CC_SID_BIT_CPUX
#endif

typedef struct _MV_CC_DSP {
	
	MV_CC_Protocol_U8_t m_PrType;
	
	INT (* OnRecvDemux)(void *self, UCHAR *pFrameBuf);
	
	pMV_CC_ICCNet_t m_pICCNet;
	
	UINT32 m_RdCount;
	UINT32 m_RdErrCount;	
	UINT32 m_WrCount;
	UINT32 m_WrErrCount;	

	/* CC-DSP private data structure */
	MV_OSAL_CPUID_U8_t		m_RemoteCPU;
#if defined( __LINUX_KERNEL__ )	
	spinlock_t 				m_hGSListMutex;
#else
	MV_OSAL_HANDLE_MUTEX_t 	m_hGSListMutex;
#endif	
	BOOL 					m_bStatusUpdate;			// true = updated, false = not updated


	MV_CC_ServiceID_U32_t 	m_SeqID;

#if defined( __LINUX_KERNEL__ )
	spinlock_t 				m_SeqIDMutex;
#else
	MV_OSAL_HANDLE_SEM_t 	m_SeqIDMutex;
#endif

} MV_CC_DSP_t, *pMV_CC_DSP_t;


HRESULT MV_CC_DSS_Constructor(	pMV_CC_DSP_t 		self,
								pMV_CC_ICCNet_t 	pICCNet);
HRESULT MV_CC_DSS_Destructor(pMV_CC_DSP_t 		self);
HRESULT MV_CC_DSS_DSPOnRecvDemux(void *self, UCHAR *pFrameBuf);
HRESULT MV_CC_DSS_Reg(pMV_CC_DSS_ServiceInfo_t pSrvInfo);
HRESULT MV_CC_DSS_Free(pMV_CC_DSS_ServiceInfo_t pSrvInfo);
HRESULT MV_CC_DSS_Inquiry(pMV_CC_DSS_ServiceInfo_t pSrvInfo);
HRESULT MV_CC_DSS_GetList(pMV_CC_DSS_ServiceInfo_DataList_t pSrvInfoList);


#endif	// #define _CCDSS_H_
