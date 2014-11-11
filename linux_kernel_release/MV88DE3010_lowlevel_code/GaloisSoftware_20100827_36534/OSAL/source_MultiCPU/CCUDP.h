/*******************************************************************************
*******************************************************************************/



#ifndef _CCUDP_H_
#define _CCUDP_H_

#include "OSAL_api.h"

#include "ICCNet.h"
#include "MidSrv.h"
#include "CCProtocol.h"


typedef struct _MV_CC_UDPPortItem {
	
	/* Mutex is OS dependent sizeof in Linux = 24 bytes*/
	MV_OSAL_HANDLE_MUTEX_t m_Mutex;
	
	void *m_pSrvSelf;

	/* Warning - The OnReceiving should be designed for parallel running with other service thread */
	INT (* OnReceiving)(void *SrvSelf, UCHAR *pFrameBuf);

#ifdef ENABLE_DEBUG_OSAL

	/* For Debug , if the memory is not enough , it will be removed */	
	UINT32 m_RdCount;
	UINT32 m_RdErrCount;	
	UINT32 m_WrCount;
	UINT32 m_WrErrCount;

#endif
		
} MV_CC_UDPPortItem_t, *pMV_CC_UDPPortItem_t;



#ifdef ENABLE_DEBUG_OSAL

	#define	MV_CC_UDPPortItem_RdCountInc(x)			(x).m_RdCount++
	#define	MV_CC_UDPPortItem_RdErrCountInc(x)		(x).m_RdErrCount++
	#define	MV_CC_UDPPortItem_WrCountInc(x)			(x).m_WrCount++
	#define	MV_CC_UDPPortItem_WrErrCountInc(x)		(x).m_WrErrCount++	

#else

	#define	MV_CC_UDPPortItem_RdCountInc(x)		
	#define	MV_CC_UDPPortItem_RdErrCountInc(x)	
	#define	MV_CC_UDPPortItem_WrCountInc(x)		
	#define	MV_CC_UDPPortItem_WrErrCountInc(x)	

#endif



typedef struct _MV_CC_UDP {
	
	MV_CC_Protocol_U8_t m_PrType;
	
	INT (* OnRecvDemux)(void *self, UCHAR *pFrameBuf);
	
	pMV_CC_ICCNet_t m_pICCNet;
	
	UINT32 m_RdCount;
	UINT32 m_RdErrCount;	
	UINT32 m_WrCount;
	UINT32 m_WrErrCount;	

	/* CC-UDP private data structure */
	MV_OSAL_HANDLE_MUTEX_t m_hPortTableMutex;		//! It is only used for creating and destroying the mutex of the table item.

	MV_CC_UDPPortItem_t m_PortTable[MV_CC_UDP_PORT_MAX];
		
} MV_CC_UDP_t, *pMV_CC_UDP_t;


typedef struct _MV_CC_UDPPort {
	
	MV_OSAL_PortID_U16_t 	m_PortID;
	
	MV_CC_SrvLevel_U8_t 	m_SrvLevel;
	MV_CC_SrvType_U8_t 		m_SrvType;
				   	
	pMV_CC_MidSrv_t 		m_pMidSrv;

} MV_CC_UDPPort_t, *pMV_CC_UDPPort_t;


INT  MV_CC_UDP_Constructor(pMV_CC_UDP_t self, pMV_CC_ICCNet_t pICCNet);



INT  MV_CC_UDP_Destructor(pMV_CC_UDP_t self);



INT MV_CC_UDP_OnRecvDemux(void *self, UCHAR *pFrameBuf);



INT MV_CC_UDP_SendTo(pMV_CC_MidSrv_t 		pMidSrv, 
					 MV_OSAL_PortID_U16_t 	SrcPort,				   	 
				   	 pMV_CC_ICCAddr_t 		pDstAddr,
				   	 UCHAR 				   	*pMsgBuf,
				   	 UINT8 					MsgLen, 
				     MV_CC_SrvLevel_U8_t 	SrvLevel,
				     MV_CC_SrvType_U8_t 	SrvType);


INT MV_CC_UDPPort_Open(pMV_CC_UDPPort_t 		self,
					   pMV_CC_MidSrv_t 			pMidSrv,
					   void						*pSrvObject,
					   void					   	*pOnReceivingFunc,
					   MV_OSAL_PortID_U16_t 	SrcPort,
					   MV_CC_SrvLevel_U8_t 		SrvLevel,
					   MV_CC_SrvType_U8_t 		SrvType);


INT MV_CC_UDPPort_GetStatus(pMV_CC_MidSrv_t 		pMidSrv,
							MV_OSAL_PortID_U16_t 	SrcPort);


INT MV_CC_UDPPort_SendMsg(pMV_CC_UDPPort_t 		self, 
				   	   	  pMV_CC_ICCAddr_t 		pDstAddr,
				   		  UCHAR 				*pMsgBuf,
				   		  UINT8 				MsgLen);


INT MV_CC_UDPPort_Close(pMV_CC_UDPPort_t self);




#endif
