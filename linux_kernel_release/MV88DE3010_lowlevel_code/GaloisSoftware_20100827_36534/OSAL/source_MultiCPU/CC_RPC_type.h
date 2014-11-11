/*******************************************************************************
*******************************************************************************/



#ifndef _MV_CC_RPC_TYPE_H_
#define _MV_CC_RPC_TYPE_H_

#include "OSAL_api.h"

#include "CCUDP.h"


#define RPC_Flags(x)						(((UINT8 *)(x))[(MV_CC_CCUDP_DATA_BYTEOFFSET) + 0])

#define RPC_ParamLen(x)						(((UINT8 *)(x))[(MV_CC_CCUDP_DATA_BYTEOFFSET) + 1])

#define RPC_CmdID(x)						(((UINT16 *)(x))[(MV_CC_CCUDP_DATA_BYTEOFFSET / 2) + 1])

#define RPC_RtnSt(x)						(((UINT16 *)(x))[(MV_CC_CCUDP_DATA_BYTEOFFSET / 2) + 1])

#define RPC_TraceDataSize(x)				(((UINT8 *)(x))[MV_CC_CCUDP_DATA_BYTEOFFSET + 6])

#define RPC_ParamStart(x)					((UINT8 *)((UINT8 *)(x) + MV_CC_RPC_DATA_BYTEOFFSET))

#define RPC_SHM_Len(x)						(((UINT32 *)(x))[(MV_CC_CCUDP_DATA_BYTEOFFSET / 4) + 2])

#define RPC_SHM_Key(x)						(((UINT32 *)(x))[(MV_CC_CCUDP_DATA_BYTEOFFSET / 4) + 3])


#define MV_CC_RPC_FLAGS_MSGTYPE				(UINT8)(0x01)		/* n */
#define MV_CC_RPC_FLAGS_MSGTYPE_MASK		(UINT8)(0xFE)		/* m */
#define MV_CC_RPC_FLAGS_MSGTYPE_Request		(UINT8)(0)			/* a */
#define MV_CC_RPC_FLAGS_MSGTYPE_Response	(UINT8)(1)			/* a */

#define MV_CC_RPC_FLAGS_SHM					(UINT8)(0x02)       /* n */
#define MV_CC_RPC_FLAGS_SHM_MASK			(UINT8)(0xFD)       /* m */
#define MV_CC_RPC_FLAGS_SHM_NoUsed			(UINT8)(0)          /* a */
#define MV_CC_RPC_FLAGS_SHM_Used			(UINT8)(1)          /* a */



typedef struct _MV_CC_RPC_Client {
	
	UINT32 					m_IndexofClntPool;
	
	UINT32					m_ServiceID;
	
	MV_CC_UDPPort_t 		m_ClntPort;
	
	MV_CC_ICCAddr_t 		m_SrvAddr;
	
	MV_OSAL_HANDLE_SEM_t 	m_UserTaskControl;
	
	UINT32 m_ClntFlags;

#define MV_CC_RPC_FLAGS_CLNTRUNSTAT				(UINT32)(0x00000001)		/* n */
#define MV_CC_RPC_FLAGS_CLNTRUNSTAT_MASK		(UINT32)(0xFFFFFFFE)		/* m */
#define MV_CC_RPC_FLAGS_CLNTRUNSTAT_Wait		(UINT32)(0)					/* a */
#define MV_CC_RPC_FLAGS_CLNTRUNSTAT_Run			(UINT32)(1)					/* a */	

	UCHAR *m_pParamBuf;
	UINT8 *m_pParamSize;
	MV_CC_RPC_RtnSt_U16_t	*m_pRPC_Rtn;
	UINT32 m_ResOnRecv;

	UINT32 m_point[LOGTYPE_TEST_RPC_TRACE_Num];
	
	UINT32 m_CallCount;
	UINT32 m_CallErrCount;
	
	UINT32 m_RtnCount;
	UINT32 m_RtnErrCount;
		
} MV_CC_RPC_Client_t, *pMV_CC_RPC_Client_t;




extern pMV_CC_MidSrv_t pMV_APP_MidSrv;






HRESULT  MV_CC_RPCClnt_OnUDPRecv(void  *ClntSelf,
							UCHAR *pMsgBuf);




								
								

HRESULT  MV_CC_RPCSrv_Task(void *arg);


void INLINE MV_TEST_RPC_TRACE_Add(UCHAR *pBuf, UINT32 num);
void INLINE MV_TEST_RPC_TRACE_AddData(UCHAR *pBuf, UINT32 num, UINT32 data);

#endif
