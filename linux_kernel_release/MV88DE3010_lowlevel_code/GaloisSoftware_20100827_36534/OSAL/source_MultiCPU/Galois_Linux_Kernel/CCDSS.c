/*******************************************************************************
*******************************************************************************/



#include "std_map.h"
#include "GSList.h"
#include "CCDSS.h"
#include "ICCHAL.h"

#include "ICCNet.h"
#include "MidSrv.h"

#if defined( __LINUX_KERNEL__ )

#include <linux/spinlock.h>

#define MV_OSAL_Mutex_Create(sem)		spin_lock_init(sem)
#define MV_OSAL_Mutex_Lock(sem)			spin_lock_bh(&sem)
#define MV_OSAL_Mutex_Unlock(sem)		spin_unlock_bh(&sem)
#define MV_OSAL_Mutex_Destroy(sem)

#endif

pMV_CC_DSP_t pMV_APP_DSS = NULL;

HRESULT MV_CC_DSS_DSPOnRecvDemux(void *self, UCHAR *pFrameBuf);


MV_CC_ServiceID_U32_t MV_CC_DSS_GetDynamicSID(pMV_CC_DSP_t 		self)
{
	MV_CC_ServiceID_U32_t temp;
	
	if (self == NULL)
		return MV_CC_ServiceID_None;
	
	MV_OSAL_Mutex_Lock(self->m_SeqIDMutex);
	
	temp = ++(self->m_SeqID);

	MV_OSAL_Mutex_Unlock(self->m_SeqIDMutex);
	
	return (temp | MV_CC_SID_BIT_DYNAMIC | MV_CC_SID_BIT_NORMAL | MV_CC_SID_BIT_LOCAL);
}

HRESULT MV_CC_DSS_DSPSendTo(pMV_CC_DSP_t 		self,
                          	MV_OSAL_CPUID_U8_t  DstCPU,
						  	UINT32				Head,
				   	      	UCHAR 				*pData,
				   	      	UINT32 				DataLen)
{
	HRESULT res;
	UCHAR MsgBuf[MV_CC_ICCFIFO_FRAME_SIZE];
	UCHAR *pMsgBuf = MsgBuf;
	
	/* Parameter Check */
	if ((self == NULL) || (self->m_pICCNet == NULL) ||
		(DstCPU >= MV_OSAL_CPU_NUM) ||		
		(DataLen > MV_CC_CCDSP_DATA_BYTESIZE) ||
		((DataLen > 0) && (pData == NULL)))
		MV_CC_DBG_Error(E_INVALIDARG, "MV_CC_DSS_DSPSendTo", NULL);
	
	/* assemble ICCP Datagram */
	ICCP_SrcCPU(pMsgBuf) = self->m_pICCNet->m_LocalCPU;
	ICCP_SrcProcess(pMsgBuf) = MV_CC_PROCESS_ID_BROADCAST;
	ICCP_DstCPU(pMsgBuf) = DstCPU;
	ICCP_DstProcess(pMsgBuf) = MV_CC_PROCESS_ID_BROADCAST;
	
	ICCP_DatagramID(pMsgBuf) = 0;
	ICCP_Protocol(pMsgBuf) = MV_CC_Protocol_CCDSP;
	ICCP_Padding(pMsgBuf) = 0xFF;
	ICCP_TimeStamp(pMsgBuf) = 0x12345678;

	/* assemble DSP Packet */
	GaloisMemcpy(DSPPacket_HeadStart(pMsgBuf), (UCHAR *)(&Head), sizeof(Head));
	if (DataLen > 0)
		GaloisMemcpy(DSPPacket_DataStart(pMsgBuf), pData, DataLen);
	
	//MV_CC_DBG_Info("(CPU-%d) sent DSP Parket CMD[%d] to (CPU-%d)\n", 
	//			ICCP_SrcCPU(pMsgBuf),	
	//			CCDSP_Command(pMsgBuf),
	//			ICCP_DstCPU(pMsgBuf));

	/* Send a CC-DSP Packet*/
	res = MV_CC_ICCNetWrite(self->m_pICCNet, pMsgBuf);	

	switch (res)
	{	
		case S_OK:
			
			self->m_WrCount++;
						
			break;
				
		case E_FULL:
			
			self->m_WrErrCount++;
						
			break;
		
		default:
			
			self->m_WrErrCount++;
						
			MV_CC_DBG_Error(res, "MV_CC_DSS_DSPSendTo", NULL);
			break;
	}	
	
	return res;
}

HRESULT MV_CC_DSS_Constructor(	pMV_CC_DSP_t 		self,
								pMV_CC_ICCNet_t 	pICCNet)
{
    HRESULT res;
	
	/* Parameter Check */
	if ((self == NULL) || (pICCNet == NULL))
		MV_CC_DBG_Error(E_INVALIDARG, "MV_CC_DSS_Constructor", NULL);

	if (pMV_APP_DSS != NULL)
		MV_CC_DBG_Error(E_NOTREADY, "MV_CC_DSS_Constructor", NULL);
		    
	self->m_PrType = MV_CC_Protocol_CCDSP;
	self->OnRecvDemux = MV_CC_DSS_DSPOnRecvDemux;
	self->m_pICCNet = pICCNet;

	self->m_RdCount = 0;
	self->m_RdErrCount = 0;	
	self->m_WrCount = 0;
	self->m_WrErrCount = 0;

    self->m_bStatusUpdate = false;
    self->m_RemoteCPU = MV_CC_DSS_REMOTECPU_INIT;
    self->m_SeqID = MV_CC_ServiceID_DynamicStart;

	MV_OSAL_Mutex_Create(&(self->m_SeqIDMutex));
	MV_OSAL_Mutex_Create(&(self->m_hGSListMutex));

	MV_OSAL_Mutex_Lock(self->m_hGSListMutex);
		
	res = MV_CC_DSS_GlobalServiceList_Init();
	if (res != S_OK)
	    MV_CC_DBG_Error(res, "MV_CC_DSS_Constructor MV_CC_DSS_GlobalServiceList_Init", NULL);

	MV_OSAL_Mutex_Unlock(self->m_hGSListMutex);


	if (MV_OSAL_CPU_LOCAL == MV_OSAL_CPU_MASTER)
		goto MV_CC_DSS_Constructor_Exit;

	// Check Remote CPU
	res = MV_CC_StartStatus_Check();
	if ((res == S_OK) || (res == MV_CC_STATUS_OK))
	{

		// Synchronize with Remote CPU
		res = MV_CC_DSS_DSPSendTo(	self,
						 			self->m_RemoteCPU,
						 			DSPPacket_HeadGEN(CMD_DSS_AskUpdate, 0, 0, 0),
						 			NULL,
						 			0);
		if (res != S_OK)
			MV_CC_DBG_Error(res, "MV_CC_DSS_Constructor Synchronize with Remote CPU (AskUpdate)", NULL);
	}

MV_CC_DSS_Constructor_Exit:

	pMV_APP_DSS = self;
	
	return S_OK;
}

HRESULT MV_CC_DSS_Destructor(pMV_CC_DSP_t 		self)
{
	HRESULT res;
	
	/* Parameter Check */
	if (self == NULL)
		MV_CC_DBG_Error(E_INVALIDARG, "MV_CC_DSS_Destructor", NULL);

	if (pMV_APP_DSS == NULL)
		MV_CC_DBG_Error(E_NOTREADY, "MV_CC_DSS_Destructor", NULL);

	pMV_APP_DSS = NULL;

	
	MV_OSAL_Mutex_Lock(self->m_hGSListMutex);

	res = MV_CC_DSS_GlobalServiceList_Exit();
	if (res != S_OK)
	    MV_CC_DBG_Error(res, "MV_CC_DSS_Destructor MV_CC_DSS_GlobalServiceList_Exit", NULL);
	    	
	MV_OSAL_Mutex_Unlock(self->m_hGSListMutex);

	MV_OSAL_Mutex_Destroy(&(self->m_hGSListMutex));
	MV_OSAL_Mutex_Destroy(&(self->m_SeqIDMutex));
		
	self->m_PrType = MV_CC_Protocol_TEST;
	self->OnRecvDemux = NULL;
	self->m_pICCNet = NULL;

	self->m_RdCount = 0;
	self->m_RdErrCount = 0;	
	self->m_WrCount = 0;
	self->m_WrErrCount = 0;

    self->m_bStatusUpdate = false;
    self->m_RemoteCPU = MV_CC_DSS_REMOTECPU_NONE;
    self->m_SeqID = MV_CC_ServiceID_DynamicStart;
        
	return S_OK;
}

HRESULT MV_CC_DSS_Reg(pMV_CC_DSS_ServiceInfo_t pSrvInfo)
{
	HRESULT res;
	pMV_CC_ICCNet_t pICCNet;
	pMV_CC_DSS_ServiceInfo_t pSrvInfo_copy;
	
	if (pSrvInfo == NULL)
		MV_CC_DBG_Error(E_INVALIDARG, "MV_CC_DSS_Reg", NULL);
		
	if (pMV_APP_DSS == NULL)
		MV_CC_DBG_Error(E_NOTREADY, "MV_CC_DSS_Reg", NULL);
		
	pICCNet = pMV_APP_DSS->m_pICCNet;
	
	// check service id need dynamic generator?
	if (pSrvInfo->m_ServiceID == MV_CC_ServiceID_DynamicApply)
	{
		//get a new dynamic service id
		pSrvInfo->m_ServiceID = MV_CC_DSS_GetDynamicSID(pMV_APP_DSS);
		if (pSrvInfo->m_ServiceID == MV_CC_ServiceID_None)
			MV_CC_DBG_Error(E_FAIL, "MV_CC_DSS_Reg", NULL);
	}

	pSrvInfo_copy = MV_CC_DSS_GlobalServiceList_SrvInfo_Ctor();
	if (pSrvInfo_copy == NULL)
		MV_CC_DBG_Error(E_OUTOFMEMORY, "MV_CC_DSS_Reg", NULL);	
	GaloisMemcpy(pSrvInfo_copy, pSrvInfo, sizeof(MV_CC_DSS_ServiceInfo_t));
	
	MV_OSAL_Mutex_Lock(pMV_APP_DSS->m_hGSListMutex);
	
	res = MV_CC_DSS_GlobalServiceList_Add(pSrvInfo->m_ServiceID, pSrvInfo_copy);
	if (res != S_OK)
	{
	    MV_CC_DBG_Warning(res, "MV_CC_DSS_Reg MV_CC_DSS_GlobalServiceList_Add", NULL);	
	    goto MV_CC_DSS_Reg_Failure;
	}
	
	// Only for test
	//res = MV_CC_DSS_GlobalServiceList_Traversal(GSList_VisitFunc_Demo, NULL);
	
	MV_OSAL_Mutex_Unlock(pMV_APP_DSS->m_hGSListMutex);
	
	// Synchronize with Remote CPU
	res = MV_CC_DSS_DSPSendTo(	pMV_APP_DSS,
					 			pMV_APP_DSS->m_RemoteCPU,
					 			DSPPacket_HeadGEN(CMD_DSS_Add, 1, 0, 0),
					 			(UCHAR *)pSrvInfo,
					 			sizeof(MV_CC_DSS_ServiceInfo_t));	
	
	if (res == E_FULL)
		res = S_OK;
		
	return res;
	
MV_CC_DSS_Reg_Failure:

	MV_OSAL_Mutex_Unlock(pMV_APP_DSS->m_hGSListMutex);
		
	return res;
}
 
HRESULT MV_CC_DSS_Free(pMV_CC_DSS_ServiceInfo_t pSrvInfo)
{
	HRESULT res;
	pMV_CC_ICCNet_t pICCNet;
		
	if (pSrvInfo == NULL)
		MV_CC_DBG_Error(E_INVALIDARG, "MV_CC_DSS_Free", NULL);

	if (pMV_APP_DSS == NULL)
		MV_CC_DBG_Error(E_NOTREADY, "MV_CC_DSS_Free", NULL);

	pICCNet = pMV_APP_DSS->m_pICCNet;
			
	MV_OSAL_Mutex_Lock(pMV_APP_DSS->m_hGSListMutex);
	
	res = MV_CC_DSS_GlobalServiceList_Delete(pSrvInfo->m_ServiceID);
	if (res != S_OK)
	{
	    MV_CC_DBG_Warning(res, "MV_CC_DSS_Free MV_CC_DSS_GlobalServiceList_Delete", NULL);	
	    goto MV_CC_DSS_Free_Failure;
	}

	// Only for test
	//res = MV_CC_DSS_GlobalServiceList_Traversal(GSList_VisitFunc_Demo, NULL);
	
	MV_OSAL_Mutex_Unlock(pMV_APP_DSS->m_hGSListMutex);
		
	// Synchronize with Remote CPU
	res = MV_CC_DSS_DSPSendTo(	pMV_APP_DSS,
					 			pMV_APP_DSS->m_RemoteCPU,
					 			DSPPacket_HeadGEN(CMD_DSS_Delete, 1, 0, 0),
					 			(UCHAR *)pSrvInfo,
					 			sizeof(MV_CC_DSS_ServiceInfo_t));	
		
	if (res == E_FULL)
		res = S_OK;
		
	return res;
						 			
MV_CC_DSS_Free_Failure:

	MV_OSAL_Mutex_Unlock(pMV_APP_DSS->m_hGSListMutex);
		
	return res;
}
  
HRESULT MV_CC_DSS_Inquiry(pMV_CC_DSS_ServiceInfo_t pSrvInfo)
{
	HRESULT res;
	pMV_CC_DSS_ServiceInfo_t pSrvInfo_Search;
	
	if (pSrvInfo == NULL)
		MV_CC_DBG_Error(E_INVALIDARG, "MV_CC_DSS_Inquiry", NULL);

	if (pMV_APP_DSS == NULL)
		MV_CC_DBG_Error(E_NOTREADY, "MV_CC_DSS_Inquiry", NULL);
		
	MV_OSAL_Mutex_Lock(pMV_APP_DSS->m_hGSListMutex);	
	
	res = MV_CC_DSS_GlobalServiceList_Get(pSrvInfo->m_ServiceID, &pSrvInfo_Search);
	if (res != S_OK)
	{
	    //MV_CC_DBG_Warning(res, "MV_CC_DSS_Inquiry MV_CC_DSS_GlobalServiceList_Get", NULL);	
	    goto MV_CC_DSS_Inquiry_Failure;
	}

	// Only for test
	//res = MV_CC_DSS_GlobalServiceList_Traversal(GSList_VisitFunc_Demo, NULL);
		
	if (pSrvInfo_Search != NULL)
		GaloisMemcpy(pSrvInfo, pSrvInfo_Search, sizeof(MV_CC_DSS_ServiceInfo_t));

MV_CC_DSS_Inquiry_Failure:
		
	MV_OSAL_Mutex_Unlock(pMV_APP_DSS->m_hGSListMutex);

	return res;
}

VOID GSList_VisitFunc_Update(PVOID arg, MV_CC_ServiceID_U32_t uiSID, pMV_CC_DSS_ServiceInfo_t pData)
{
	HRESULT res;
	MV_OSAL_CPUID_U8_t  DstCPU;
	pMV_CC_DSP_t 	pHandle;
	
	if (((pHandle = (pMV_CC_DSP_t)arg ) == NULL) || (pData == NULL))
	{
		MV_CC_DBG_Warning(E_INVALIDARG, "GSList_VisitFunc_Update", NULL);
		return;
	}
	
	DstCPU = pHandle->m_RemoteCPU;

	// Send CMD_DSS_UpdateOK;
	res = MV_CC_DSS_DSPSendTo(	pHandle,
					 			DstCPU,
					 			DSPPacket_HeadGEN(CMD_DSS_Add, 1, 0, 0),
					 			(UCHAR *)pData,
					 			sizeof(MV_CC_DSS_ServiceInfo_t));
	if (res != S_OK)
		MV_CC_DBG_Warning(res, "GSList_VisitFunc_Update MV_CC_DSS_DSPSendTo", NULL);

	return;
}

HRESULT DSS_Update(pMV_CC_DSP_t self)
{
	HRESULT res;
	pMV_CC_DSP_t 	pHandle;
	
	/* Parameter Check */
	if ((pHandle = (pMV_CC_DSP_t)self ) == NULL)
		MV_CC_DBG_Error(E_INVALIDARG, "DSS_Update", NULL);
		
	MV_OSAL_Mutex_Lock(pHandle->m_hGSListMutex);

	// Traversal Golbal Service List and Update
	res = MV_CC_DSS_GlobalServiceList_Traversal(GSList_VisitFunc_Update, (PVOID)pHandle);
	if (res != S_OK)
	{
		MV_CC_DBG_Warning(res, "DSS_Update MV_CC_DSS_GlobalServiceList_Traversal", NULL);
		
		MV_OSAL_Mutex_Unlock(pHandle->m_hGSListMutex);
		
		// Send CMD_DSS_UpdateOK;
		res = MV_CC_DSS_DSPSendTo(	pHandle,
						 			pHandle->m_RemoteCPU,
						 			DSPPacket_HeadGEN(CMD_DSS_UpdateError, 0, 0, 0),
						 			NULL,
						 			0);
	}
	else
	{
		MV_OSAL_Mutex_Unlock(pHandle->m_hGSListMutex);

		// Send CMD_DSS_UpdateOK;
		res = MV_CC_DSS_DSPSendTo(	pHandle,
						 			pHandle->m_RemoteCPU,
						 			DSPPacket_HeadGEN(CMD_DSS_UpdateOK, 0, 0, 0),
						 			NULL,
						 			0);
	}

	if (res == E_FULL)
		res = S_OK;
			
	return res;
}

   
HRESULT MV_CC_DSS_DSPOnRecvDemux(void *self, UCHAR *pFrameBuf)
{
	HRESULT res = S_OK, i;
	MV_OSAL_CPUID_U8_t  DstCPU;
	UINT8	Cmd, Param1;
	pMV_CC_DSP_t pHandle;
	pMV_CC_DSS_ServiceInfo_t pSrvInfo, pSrvInfo_copy;

	/* Parameter Check */
	if (((pHandle = (pMV_CC_DSP_t)self ) == NULL) || 
		(pFrameBuf == NULL))
		MV_CC_DBG_Error(E_NOTREADY, "MV_CC_DSS_DSPOnRecvDemux", NULL);

	DstCPU = ICCP_SrcCPU(pFrameBuf);
	Cmd = CCDSP_Command(pFrameBuf);
			

	switch (Cmd)
	{
		case CMD_DSS_AskUpdate:
			
			
			//if (DstCPU != MV_OSAL_CPU_LOCAL)
			if (DstCPU == pHandle->m_RemoteCPU)
			{
				// Ask DSS_Update_Task to update with Remote CPU
				//MV_OSAL_Sem_Post(pHandle->m_UpdateTaskSem);
				res = DSS_Update(pHandle);
			}
			else
			{
				MV_CC_DBG_Warning(res, "MV_CC_DSS_DSPOnRecvDemux CMD_DSS_AskUpdate error", NULL);
				res = E_FAIL;
			}

			break;

		case CMD_DSS_UpdateOK:

			MV_CC_DBG_Info("(CPU-%d)MV_CC_DSS_DSPOnRecvDemux CMD_DSS_UpdateOK from (CPU-%d)\n", MV_OSAL_CPU_LOCAL, DstCPU);
						
			pHandle->m_bStatusUpdate = true;
			res = S_OK;
			
			// Only for test
			//res = MV_CC_DSS_GlobalServiceList_Traversal(GSList_VisitFunc_Demo, NULL);
			
			break;

		case CMD_DSS_UpdateError:

			MV_CC_DBG_Info("(CPU-%d)MV_CC_DSS_DSPOnRecvDemux CMD_DSS_UpdateError from (CPU-%d)\n", MV_OSAL_CPU_LOCAL, DstCPU);
						
			//pHandle->m_bStatusUpdate = false;
			res = S_OK;
			
			break;
			
		case CMD_DSS_Add:

						
			Param1 = CCDSP_Param1(pFrameBuf);

			i = 0;
			while (i < Param1)
			{
				if ( (sizeof(MV_CC_DSS_ServiceInfo_t) * ( i + 1 )) > MV_CC_CCDSP_DATA_BYTESIZE)
				{
					res = E_OUTOFRANGE;
					MV_CC_DBG_Warning(res, "MV_CC_DSS_DSPOnRecvDemux CMD_DSS_Add->Parameter 1", NULl);
					break;
				}
				
				pSrvInfo = (pMV_CC_DSS_ServiceInfo_t)(DSPPacket_DataStart(pFrameBuf) + sizeof(MV_CC_DSS_ServiceInfo_t) * i);
				
				pSrvInfo_copy = MV_CC_DSS_GlobalServiceList_SrvInfo_Ctor();
				if (pSrvInfo_copy == NULL)
					MV_CC_DBG_Error(E_OUTOFMEMORY, "MV_CC_DSS_DSPOnRecvDemux", NULL);	
				GaloisMemcpy(pSrvInfo_copy, pSrvInfo, sizeof(MV_CC_DSS_ServiceInfo_t));
				
				MV_OSAL_Mutex_Lock(pHandle->m_hGSListMutex);
				
				res = MV_CC_DSS_GlobalServiceList_Add(pSrvInfo->m_ServiceID, pSrvInfo_copy);
				if (res != S_OK)
				    MV_CC_DBG_Warning(res, "MV_CC_DSS_DSPOnRecvDemux MV_CC_DSS_GlobalServiceList_Add", NULL);	

				// Only for test
				//res = MV_CC_DSS_GlobalServiceList_Traversal(GSList_VisitFunc_Demo, NULL);
					
				MV_OSAL_Mutex_Unlock(pHandle->m_hGSListMutex);
				
				i++;
			}
			
			break;

		case CMD_DSS_Delete:


			Param1 = CCDSP_Param1(pFrameBuf);
			i = 0;
			while (i < Param1)
			{
				if ( (sizeof(MV_CC_DSS_ServiceInfo_t) * ( i + 1 )) > MV_CC_CCDSP_DATA_BYTESIZE)
				{
					res = E_OUTOFRANGE;
					MV_CC_DBG_Warning(res, "MV_CC_DSS_DSPOnRecvDemux CMD_DSS_Delete->Parameter 1", NULL);
					break;
				}
				
				pSrvInfo = (pMV_CC_DSS_ServiceInfo_t)(DSPPacket_DataStart(pFrameBuf) + sizeof(MV_CC_DSS_ServiceInfo_t) * i);

				MV_OSAL_Mutex_Lock(pHandle->m_hGSListMutex);
				
				res = MV_CC_DSS_GlobalServiceList_Delete(pSrvInfo->m_ServiceID);
				if (res != S_OK)
				    MV_CC_DBG_Warning(res, "MV_CC_DSS_DSPOnRecvDemux MV_CC_DSS_GlobalServiceList_Delete", NULL);	

				// Only for test
				//res = MV_CC_DSS_GlobalServiceList_Traversal(GSList_VisitFunc_Demo, NULL);
								
				MV_OSAL_Mutex_Unlock(pHandle->m_hGSListMutex);

				i++;
			}

			break;
		
		default:
			
			res = E_BADVALUE;
			MV_CC_DBG_Warning(res, "MV_CC_DSS_DSPOnRecvDemux Error Command", NULL);
			
			break;									
	}
	
	if (res == S_OK)
		pHandle->m_RdCount++;
	else
		pHandle->m_RdErrCount++;
		
	return res;
}

VOID GSList_VisitFunc_GetList(PVOID arg, MV_CC_ServiceID_U32_t uiSID, pMV_CC_DSS_ServiceInfo_t pData)
{
	pMV_CC_DSS_ServiceInfo_DataList_t 	pSrvInfoList;
	
	if (((pSrvInfoList = (pMV_CC_DSS_ServiceInfo_DataList_t)arg ) == NULL) || (pData == NULL))
	{
		MV_CC_DBG_Warning(E_INVALIDARG, "GSList_VisitFunc_GetList", NULL);
		return;
	}
	
	if (pSrvInfoList->m_DataNum < pSrvInfoList->m_ListNum)
	{
		GaloisMemcpy(&(pSrvInfoList->m_SrvInfo[pSrvInfoList->m_DataNum]), pData, sizeof(MV_CC_DSS_ServiceInfo_t));
		pSrvInfoList->m_DataNum++;
	}
	
	pSrvInfoList->m_MaxNum++;

	return;
}

HRESULT MV_CC_DSS_GetList(pMV_CC_DSS_ServiceInfo_DataList_t pSrvInfoList)
{
	HRESULT res;
	pMV_CC_ICCNet_t pICCNet;
		
	if ((pSrvInfoList == NULL) || (pSrvInfoList->m_ListNum == 0))
		MV_CC_DBG_Error(E_INVALIDARG, "MV_CC_DSS_GetList", NULL);

	if (pMV_APP_DSS == NULL)
		MV_CC_DBG_Error(E_NOTREADY, "MV_CC_DSS_GetList", NULL);

	pICCNet = pMV_APP_DSS->m_pICCNet;	
		
	pSrvInfoList->m_DataNum = 0;
	pSrvInfoList->m_MaxNum = 0;
			
	MV_OSAL_Mutex_Lock(pMV_APP_DSS->m_hGSListMutex);

	res = MV_CC_DSS_GlobalServiceList_Traversal(GSList_VisitFunc_GetList, pSrvInfoList);
	if (res != S_OK)
	    MV_CC_DBG_Warning(res, "MV_CC_DSS_GetList MV_CC_DSS_GlobalServiceList_Traversal", NULL);	

	MV_OSAL_Mutex_Unlock(pMV_APP_DSS->m_hGSListMutex);

	return res;
}
