/*******************************************************************************
*******************************************************************************/



#ifndef _OSAL_API_H_
#define _OSAL_API_H_

#include "com_type.h"
#include "OSAL_config.h"
#include "OSAL_type.h"
#include "OSAL_Local_CPU_def.h"
#include "GaloisTime.h"

#include "MV_CC_GSConfig.h"

#ifdef __cplusplus
extern "C"
{
#endif	// #ifdef __cplusplus


/****************************** OSAL *******************************************/

HRESULT  MV_OSAL_Init(void);

HRESULT  MV_OSAL_Exit(void);

HRESULT  MV_OSAL_GetSysSrvStatus(MV_CC_ServiceID_U32_t ServiceID);


/****************************** OSAL SEM*******************************************/




typedef MV_OSAL_HANDLE 	MV_OSAL_HANDLE_SEM_t;
typedef MV_OSAL_HANDLE 	*pMV_OSAL_HANDLE_SEM_t;


HRESULT  MV_OSAL_Sem_Create(MV_OSAL_HANDLE_SEM_t 	*pHandle, 
							UINT					Value);


HRESULT  MV_OSAL_Sem_Destroy(MV_OSAL_HANDLE_SEM_t 	*pHandle);
                          			

HRESULT  MV_OSAL_Sem_Wait(MV_OSAL_HANDLE_SEM_t 	Handle);
                           			
HRESULT  MV_OSAL_Sem_TryWait(MV_OSAL_HANDLE_SEM_t 	Handle);

HRESULT  MV_OSAL_Sem_TimedWait(MV_OSAL_HANDLE_SEM_t 	Handle,
									  UINT64				NanoSec);

HRESULT  MV_OSAL_Sem_Post(MV_OSAL_HANDLE_SEM_t 	Handle);

HRESULT  MV_OSAL_Sem_GetValue(MV_OSAL_HANDLE_SEM_t 	Handle,
									 INT					*pValue);



/****************************** OSAL MUTEX*******************************************/



typedef MV_OSAL_HANDLE 	MV_OSAL_HANDLE_MUTEX_t;
typedef MV_OSAL_HANDLE 	*pMV_OSAL_HANDLE_MUTEX_t;

HRESULT  MV_OSAL_Mutex_Create(MV_OSAL_HANDLE_MUTEX_t 	*pHandle);


HRESULT  MV_OSAL_Mutex_Destroy(MV_OSAL_HANDLE_MUTEX_t 	*pHandle);
                           			                       			
HRESULT  MV_OSAL_Mutex_Lock(MV_OSAL_HANDLE_MUTEX_t 	Handle);
                           		

HRESULT  MV_OSAL_Mutex_TryLock(MV_OSAL_HANDLE_MUTEX_t 	Handle);

HRESULT  MV_OSAL_Mutex_Unlock(MV_OSAL_HANDLE_MUTEX_t 	Handle);


typedef MV_OSAL_HANDLE 	MV_OSAL_HANDLE_COND_t;
typedef MV_OSAL_HANDLE 	*pMV_OSAL_HANDLE_COND_t;


HRESULT  MV_OSAL_Cond_Create(MV_OSAL_HANDLE_COND_t 		*pHandle,
							 MV_OSAL_HANDLE_MUTEX_t 	MutexHandle);


HRESULT  MV_OSAL_Cond_Destroy(MV_OSAL_HANDLE_COND_t 	*pHandle);
                           			                      			
HRESULT  MV_OSAL_Cond_Wait(MV_OSAL_HANDLE_COND_t 	CondHandle);
                           			
HRESULT  MV_OSAL_Cond_TimedWait(MV_OSAL_HANDLE_COND_t 	CondHandle,
						   		MV_TimeSpec_t			*pTimeOut);

HRESULT  MV_OSAL_Cond_Signal(MV_OSAL_HANDLE_COND_t 	CondHandle);

HRESULT  MV_OSAL_Cond_Broadcast(MV_OSAL_HANDLE_COND_t 	CondHandle);

/****************************** OSAL TASK*******************************************/


typedef MV_OSAL_HANDLE 	MV_OSAL_HANDLE_TASK_t;
typedef MV_OSAL_HANDLE 	*pMV_OSAL_HANDLE_TASK_t;

#if defined( __LINUX__ )
#include <pthread.h>

#define MV_OSAL_Task_Exit(x)		pthread_exit(x)


#define MV_OSAL_Task_CleanUp_Push(x,y)	pthread_cleanup_push((x),(y))


#define MV_OSAL_Task_CleanUp_Pop(x)		pthread_cleanup_pop(x)

#elif defined( __ECOS__ )
#include <cyg/kernel/kapi.h>

void  MV_OSAL_Task_ExitForeCos();

#define MV_OSAL_Task_Exit(x)		MV_OSAL_Task_ExitForeCos()

#define MV_OSAL_Task_CleanUp_Push(x,y)		cyg_thread_add_destructor( (cyg_thread_destructor_fn)(x), (cyg_addrword_t)(y) )

#define MV_OSAL_Task_CleanUp_Pop(x,y)		cyg_thread_rem_destructor( (cyg_thread_destructor_fn)(x), (cyg_addrword_t)(y) )

#elif defined( WIN32 )
#include "windows.h"
#pragma warning ( disable: 4311 ) //warning C4311: 'type cast' : pointer truncation from 'void *' to 'DWORD'
#define MV_OSAL_Task_Exit(x)		ExitThread((DWORD)(x))

#define MV_OSAL_Task_CleanUp_Push(x,y)

#define MV_OSAL_Task_CleanUp_Pop(x)

#endif


HRESULT  MV_OSAL_Init_Task(void);


HRESULT  MV_OSAL_Exit_Task(void);


HRESULT  MV_OSAL_Task_Create_Advanced(MV_OSAL_HANDLE_TASK_t 	*pHandle, 
										VOID					*TaskFunc,
                           				VOID 					*Arg,
                           				MV_OSAL_PRIORITY_t const Priority,
                           				UINT32 const			NMsgsMax);


HRESULT  MV_OSAL_Task_Create_Detailed(MV_OSAL_HANDLE_TASK_t 	*pHandle, 
										VOID					*TaskFunc,
                           				VOID 					*Arg,
                           				MV_OSAL_PRIORITY_t const Priority,
                           				CHAR					*pName,
                           				VOID					*pStackBase,
                           				UINT32					StackSize,
                           				UINT32 const			NMsgsMax);
                           				
HRESULT  MV_OSAL_Task_CreateWithMsgQ(MV_OSAL_HANDLE_TASK_t 	*pHandle, 
									VOID					*TaskFunc,
                           			VOID 					*Arg,
                           			UINT32 const			NMsgsMax);

HRESULT  MV_OSAL_Task_Create(MV_OSAL_HANDLE_TASK_t 	*pHandle, 
									VOID					*TaskFunc,
                           			VOID 					*Arg);

HRESULT  MV_OSAL_Task_Destroy(MV_OSAL_HANDLE_TASK_t 	*pHandle);
                           			                     			
HRESULT  MV_OSAL_Task_Join(MV_OSAL_HANDLE_TASK_t 	*pHandle);

HRESULT  MV_OSAL_Task_Yield(void);

HRESULT  MV_OSAL_Task_Sleep(UINT32 TimeOutMS);

HRESULT  MV_OSAL_Task_Self(MV_OSAL_HANDLE_TASK_t 	*pHandle);

HRESULT MV_OSAL_Task_MsgQ_Post(MV_OSAL_HANDLE_TASK_t *pHandle, PVOID pMSG);

HRESULT MV_OSAL_Task_MsgQ_Get(MV_OSAL_HANDLE_TASK_t *pHandle, PVOID pMSG, UINT32 TimeOutMS);

HRESULT MV_OSAL_Task_MsgQ_GetTry(MV_OSAL_HANDLE_TASK_t *pHandle, PVOID pMSG);


/****************************** OSAL TIMER*******************************************/


typedef MV_OSAL_HANDLE 	MV_HANDLE_Timer_t;
typedef MV_OSAL_HANDLE 	*pMV_HANDLE_Timer_t;


HRESULT  MV_Timer_Create(MV_HANDLE_Timer_t	*pHandle,
						 UINT				Period,
						 PVOID				CallbackFunc,
						 PVOID				UserData);


HRESULT  MV_Timer_Destroy(MV_HANDLE_Timer_t	*pHandle);


HRESULT  MV_Timer_Reset(MV_HANDLE_Timer_t	*pHandle,
						 UINT				Period,
						 PVOID				CallbackFunc,
						 PVOID				UserData);


HRESULT  MV_Timer_Start(MV_HANDLE_Timer_t	*pHandle);


HRESULT  MV_Timer_Stop(MV_HANDLE_Timer_t	*pHandle);


HRESULT   MV_Timer_Resume(MV_HANDLE_Timer_t	*pHandle);


HRESULT   MV_Timer_Pause(MV_HANDLE_Timer_t	*pHandle);


HRESULT  MV_Timer_Reset_Period(MV_HANDLE_Timer_t	*pHandle,
						 UINT				Period);

/****************************** OSAL SHM*******************************************/

#if defined( __LINUX__ )






#elif defined( __ECOS__ )

#include "galois_cache_flush.h"

#define MV_SHM_NON_CACHE_ADDR(x)						((((UINT32)(x) >= (UINT32)MV_SHM_Start_PhysAddr_CACHE) && ((UINT32)(x) < (UINT32)MV_SHM_Start_PhysAddr_CACHE + MV_SHM_FullSize)) ? ((PVOID)((UINT32)MV_SHM_Start_PhysAddr + ((UINT32)(x) - (UINT32)MV_SHM_Start_PhysAddr_CACHE))): NULL)
#define MV_SHM_CACHE_ADDR(x)							((((UINT32)(x) >= (UINT32)MV_SHM_Start_PhysAddr) && ((UINT32)(x) < (UINT32)MV_SHM_Start_PhysAddr + MV_SHM_FullSize)) ? ((PVOID)((UINT32)MV_SHM_Start_PhysAddr_CACHE + ((UINT32)(x) - (UINT32)MV_SHM_Start_PhysAddr))): NULL)
#define MV_CC_SHM_InvalidateDCacheRegion				InvalidateDCacheRegion
#define MV_CC_SHM_CleanDCacheRegion						CleanDCacheRegion
#define MV_CC_SHM_CleanAndInvalidateDCacheRegion		CleanAndInvalidateDCacheRegion

#endif

#define MV_CC_SHM_Flush(x)	{			\
	volatile UINT i;										\
	i = (UINT)(x);											\
}

typedef UINT32 	MV_CC_SHM_Key_t;
typedef UINT32 	*pMV_CC_SHM_Key_t;

typedef MV_OSAL_HANDLE 	MV_CC_SHM_HANDLE_t;
typedef MV_OSAL_HANDLE 	*pMV_CC_SHM_HANDLE_t;

HRESULT  MV_CC_SHM_Init(void);

HRESULT  MV_CC_SHM_Exit(void);

PVOID MV_CC_SHM_Malloc( pMV_CC_SHM_Key_t pKey, UINT32 Size );

PVOID MV_CC_SHM_Get( pMV_CC_SHM_Key_t pKey );

HRESULT MV_CC_SHM_Free( PVOID Ptr );

HRESULT MV_CC_SHM_FreeKey( pMV_CC_SHM_Key_t pKey );

HRESULT MV_CC_SHM_GetInfo( pMV_CC_SHM_PoolInfo_t pInfo );


#if defined( __LINUX__ )

HRESULT MV_CC_SHM_InvalidateDCacheRegion(void *adr, int size);

HRESULT MV_CC_SHM_CleanDCacheRegion(void *adr, int size);

HRESULT MV_CC_SHM_CleanAndInvalidateDCacheRegion(void *adr, int size);

PVOID MV_CC_SHM_GetStartAddress();

INT MV_CC_SHM_GetDeviceFileID();

PVOID MV_CC_SHM_GetStartAddress_CACHE();

INT MV_CC_SHM_GetDeviceFileID_CACHE();

PVOID MV_SHM_NON_CACHE_ADDR(PVOID addr_cache);

PVOID MV_SHM_CACHE_ADDR(PVOID addr_nocache);

PVOID MV_SHM_PhysAddrToVirtAddr(PVOID addr_phys);

PVOID MV_SHM_VirtAddrToPhysAddr(PVOID addr_virt);

#endif


/****************************** OSAL MSGQ*******************************************/

typedef MV_CC_HANDLE 	MV_CC_HANDLE_MsgQ_t;
typedef MV_CC_HANDLE 	*pMV_CC_HANDLE_MsgQ_t;

typedef struct {
	//! Message ID
	UINT32 m_MsgID;
	
	//! Message 1st Parameter
	UINT32 m_Param1;
	
	//! Message 2nd Parameter
	UINT32 m_Param2;
	
} MV_CC_MSG_t, *pMV_CC_MSG_t;


HRESULT  MV_CC_MsgQ_Create(MV_CC_HANDLE_MsgQ_t 		*pHandle,
						   MV_CC_MsgQType_t			MsgQType,
						   MV_CC_ServiceID_U32_t 	ServiceID,
						   UINT32 		 			NMsgsMax);

HRESULT  MV_CC_MsgQ_Destroy(MV_CC_HANDLE_MsgQ_t 	*pHandle);

MV_CC_MsgQType_t  MV_CC_MsgQ_GetType(MV_CC_HANDLE_MsgQ_t 	Handle);

HRESULT  MV_CC_MsgQ_PostMsg(MV_CC_HANDLE_MsgQ_t 	Handle,
							PVOID 					pMSG);

HRESULT  MV_CC_MsgQ_PostMsgByID(MV_CC_ServiceID_U32_t 	ServiceID,
								PVOID 					pMSG);

HRESULT  MV_CC_MsgQ_GetMsg(MV_CC_HANDLE_MsgQ_t 	Handle, 
						   PVOID 				pMSG);

HRESULT  MV_CC_MsgQ_TimedGetMsg(MV_CC_HANDLE_MsgQ_t 	Handle, 
								PVOID 					pMSG, 
								UINT32 					TimeOutMS);

HRESULT MV_CC_MsgQ_TryGetMsg(MV_CC_HANDLE_MsgQ_t 	Handle, 
							 PVOID 					pMSG);

MV_CC_ServiceID_U32_t  MV_CC_MsgQ_GetServiceID(MV_CC_HANDLE_MsgQ_t 	Handle);

/****************************** OSAL MSGQ Ex*******************************************/

typedef MV_CC_HANDLE 	MV_CC_HANDLE_MsgQEx_t;
typedef MV_CC_HANDLE 	*pMV_CC_HANDLE_MsgQEx_t;

typedef struct {
	//! Message ID
	UINT8 m_MsgLen;
	
	UINT8 m_Data[MV_CC_MSGQ_DATA_MSGEXLEN_MAX];		// The size of the Data could be reduced by user's requirement
	
} MV_CC_MSGEx_t, *pMV_CC_MSGEx_t;


HRESULT  MV_CC_MsgQEx_Create(MV_CC_HANDLE_MsgQEx_t 		*pHandle,
						     MV_CC_MsgQType_t			MsgQType,
						     MV_CC_ServiceID_U32_t 		ServiceID,
						     UINT32 		 			NMsgsMax,
						     UINT32						MsgLenMax);

HRESULT  MV_CC_MsgQEx_Destroy(MV_CC_HANDLE_MsgQEx_t 	*pHandle);

MV_CC_MsgQType_t  MV_CC_MsgQEx_GetType(MV_CC_HANDLE_MsgQEx_t 	Handle);

HRESULT  MV_CC_MsgQEx_PostMsg(MV_CC_HANDLE_MsgQEx_t 	Handle,
							  PVOID 				pMSG);

HRESULT  MV_CC_MsgQEx_PostMsgByID(MV_CC_ServiceID_U32_t 	ServiceID,
								  PVOID 					pMSG);

HRESULT  MV_CC_MsgQEx_GetMsg(MV_CC_HANDLE_MsgQEx_t 	Handle, 
						     PVOID 					pMSG);

HRESULT  MV_CC_MsgQEx_TimedGetMsg(MV_CC_HANDLE_MsgQEx_t 	Handle, 
								  PVOID 				pMSG, 
								  UINT32 				TimeOutMS);

HRESULT MV_CC_MsgQEx_TryGetMsg(MV_CC_HANDLE_MsgQEx_t 	Handle, 
							   PVOID				pMSG);


MV_CC_ServiceID_U32_t  MV_CC_MsgQEx_GetServiceID(MV_CC_HANDLE_MsgQEx_t 	Handle);


/****************************** OSAL RPC*******************************************/


typedef MV_CC_HANDLE 	MV_CC_HANDLE_RPCClnt_t;
typedef MV_CC_HANDLE 	*pMV_CC_HANDLE_RPCClnt_t;;

typedef MV_CC_HANDLE 	MV_CC_HANDLE_RPCSrv_t;
typedef MV_CC_HANDLE 	*pMV_CC_HANDLE_RPCSrv_t;



HRESULT  MV_CC_RPCClnt_Create(pMV_CC_HANDLE_RPCClnt_t 	pHandle, 
						     MV_CC_ServiceID_U32_t 	ServiceID);


HRESULT  MV_CC_RPCClnt_Call(MV_CC_HANDLE_RPCClnt_t	Handle,
						   MV_CC_RPC_CmdID_U16_t 	RPC_Cmd, 
						   UCHAR 					*pParamBuf, 
						   UINT8 					*pParamSize,
						   MV_CC_RPC_RtnSt_U16_t	*RPC_Rtn,
						   UINT64					TimeOut);


HRESULT  MV_CC_RPCClnt_Destroy(pMV_CC_HANDLE_RPCClnt_t 	pHandle);


HRESULT  MV_CC_RPC_Call(	MV_CC_ServiceID_U32_t 	ServiceID,
						   	MV_CC_RPC_CmdID_U16_t 		RPC_Cmd, 
						   	UCHAR 						*pParamBuf, 
						   	UINT8 						*pParamSize,
						   	MV_CC_RPC_RtnSt_U16_t		*RPC_Rtn,
						   	UINT64						TimeOut);
						   									  

MV_CC_ServiceID_U32_t  MV_CC_RPCSrv_GetServiceID(MV_CC_HANDLE_RPCSrv_t 		Handle);


HRESULT  MV_CC_RPCSrv_Create(pMV_CC_HANDLE_RPCSrv_t 		pHandle, 
							MV_CC_ServiceID_U32_t 		ServiceID,
							pMV_CC_RPC_RegCmdItem_t 		pRegCmdList);


HRESULT  MV_CC_RPCSrv_Destroy(pMV_CC_HANDLE_RPCSrv_t 	pHandle);


UINT8 MV_CC_RPCParam_Pack(UCHAR *pParamBuf, INT num, ...);


UINT8 MV_CC_RPCParam_Unpack(UCHAR *pParamBuf, INT num, ...);


/****************************** OSAL CBUF*******************************************/


typedef MV_CC_HANDLE 	MV_CC_HANDLE_CBufCtrl_t;
typedef MV_CC_HANDLE 	*pMV_CC_HANDLE_CBufCtrl_t;

HRESULT  MV_CC_CBuf_Create(MV_CC_HANDLE_CBufCtrl_t 	*pHandle,
						   MV_CC_ServiceID_U32_t 	*pServiceID,
						   MV_CC_CBufCtrlType_t		CBufCtrlType,
						   UINT32					BufSize,
						   UINT32					EndBlockSize,
						   UINT32					Flags);

HRESULT  MV_CC_CBuf_Destroy(MV_CC_HANDLE_CBufCtrl_t 	*pHandle);

MV_CC_CBufCtrlType_t  MV_CC_CBuf_GetCtrlType(MV_CC_HANDLE_CBufCtrl_t 	Handle);

MV_CC_ServiceID_U32_t  MV_CC_CBuf_GetServiceID(MV_CC_HANDLE_CBufCtrl_t 	Handle);

UINT32  MV_CC_CBuf_GetBufSize(MV_CC_HANDLE_CBufCtrl_t 	Handle);

UINT32  MV_CC_CBuf_GetEndBlockSize(MV_CC_HANDLE_CBufCtrl_t 	Handle);

#define MV_CC_CBuf_GetMaxBlockSize	MV_CC_CBuf_GetEndBlockSize

UINT32  MV_CC_CBuf_GetBodyAddr(MV_CC_HANDLE_CBufCtrl_t 	Handle);

UINT32  MV_CC_CBuf_GetBaseAddr(MV_CC_HANDLE_CBufCtrl_t 	Handle);

UINT32 *MV_CC_CBuf_GetCBufRdPtr(MV_CC_HANDLE_CBufCtrl_t 	Handle);

UINT32 *MV_CC_CBuf_GetCBufWrPtr(MV_CC_HANDLE_CBufCtrl_t 	Handle);

UINT32 MV_CC_CBuf_GetRdOffset(MV_CC_HANDLE_CBufCtrl_t 	Handle);

UINT32 MV_CC_CBuf_GetWrOffset(MV_CC_HANDLE_CBufCtrl_t 	Handle);


HRESULT MV_CC_CBuf_SetRdOffset(MV_CC_HANDLE_CBufCtrl_t 	Handle,
							   UINT32					Value);

HRESULT MV_CC_CBuf_SetWrOffset(MV_CC_HANDLE_CBufCtrl_t 	Handle,
							   UINT32					Value);

HRESULT  MV_CC_CBuf_WriteTry(MV_CC_HANDLE_CBufCtrl_t 	Handle, 
						 	UINT32		 				*pWrBlockSize,
						 	PVOID			 			*pWrPtr);

HRESULT  MV_CC_CBuf_WriteFinish(MV_CC_HANDLE_CBufCtrl_t 	Handle, 
								UINT32		 				*pWrBlockSize);

HRESULT  MV_CC_CBuf_Write(MV_CC_HANDLE_CBufCtrl_t 	Handle, 
					  	  UINT32		 			*pWrBlockSize,
					  	  UCHAR 					*pSrcBuf);

HRESULT  MV_CC_CBuf_PostNotice(MV_CC_HANDLE_CBufCtrl_t 	Handle);

INT32  MV_CC_CBuf_GetSpaceSize(MV_CC_HANDLE_CBufCtrl_t 	Handle);

INT32  MV_CC_CBuf_GetSpaceSizeAndNoRoll(MV_CC_HANDLE_CBufCtrl_t 	Handle,
									    INT32 						*pLinearSize);

HRESULT  MV_CC_CBuf_WaitNotice(MV_CC_HANDLE_CBufCtrl_t 	Handle);

HRESULT  MV_CC_CBuf_TimedWaitNotice(MV_CC_HANDLE_CBufCtrl_t 	Handle,
									UINT32						TimeOutMS);

HRESULT  MV_CC_CBuf_ReadTry(MV_CC_HANDLE_CBufCtrl_t 	Handle, 
							UINT32		 				*pRdBlockSize,
							PVOID			 			*pRdPtr);

HRESULT  MV_CC_CBuf_ReadFinish(MV_CC_HANDLE_CBufCtrl_t 	Handle, 
						   	   UINT32		 			*pRdBlockSize);

HRESULT  MV_CC_CBuf_Read(MV_CC_HANDLE_CBufCtrl_t 	Handle, 
					 	 UINT32		 				*pRdBlockSize,
					 	 UCHAR 						*pDstBuf);

INT32  MV_CC_CBuf_GetDataSize(MV_CC_HANDLE_CBufCtrl_t 	Handle);

INT32  MV_CC_CBuf_GetDataSizeAndNoRoll(MV_CC_HANDLE_CBufCtrl_t 	Handle,
									   INT32 					*pLinearSize);

HRESULT MV_CC_CBuf_Reset(MV_CC_HANDLE_CBufCtrl_t 	Handle);

HRESULT MV_CC_CBuf_Reset_WrOffset(MV_CC_HANDLE_CBufCtrl_t 	Handle);

void *Galois_shm_align_malloc(UINT32 size, UINT32 align);
void Galois_shm_align_free(void * buf);
#define GALOIS_SHM_1K_MALLOC(x)		Galois_shm_align_malloc((x), 1024)	
#define GALOIS_SHM_1K_FREE(x) Galois_shm_align_free(x)
#define GALOIS_SHM_4K_MALLOC(x)		Galois_shm_align_malloc((x), 4096)	
#define GALOIS_SHM_4K_FREE(x) Galois_shm_align_free(x)

#ifdef __cplusplus
}		// extern "C"
#endif	// #ifdef __cplusplus

#endif
