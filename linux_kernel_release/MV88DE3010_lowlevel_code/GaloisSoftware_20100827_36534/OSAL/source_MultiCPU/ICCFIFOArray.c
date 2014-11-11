/*******************************************************************************
*******************************************************************************/


#include "com_type.h"

#include "OSAL_Local_CPU_def.h"

#include "ICCHAL.h"

#include "ICCFIFOArray.h"

#if defined( __LINUX_KERNEL__ )

#define MV_OSAL_Sem_Create(sem,value)	sema_init(sem, value)
#define MV_OSAL_Sem_Wait(sem)			down(&sem)
#define MV_OSAL_Sem_Post(sem)			up(&sem)
#define MV_OSAL_Sem_Destroy(sem)

#endif


INT  MV_CC_ICCFIFOArrayInit(pMV_CC_ICCFIFOArray_t pHandle,
							void *pICCFIFOArrayStart,
							MV_OSAL_CPUID_U8_t ThisCPU)
{
	int i,j,res;
	UINT32	FIFOSize, FrameSize, FrameNMax, FIFOFullSize;
	pMV_CC_ICCFIFO_t pFIFO;
	
	/* Parameter Check */
	if ((pHandle == NULL) ||
		(pICCFIFOArrayStart == NULL) ||
		(ThisCPU >= MV_OSAL_CPU_NUM))
		MV_CC_DBG_Error(E_INVALIDARG, "MV_CC_ICCFIFOArrayInit", NULL);
		
	/* MV_CC_ICCFIFOArray_t data structure init */
	
	pHandle->m_LocalCPU = ThisCPU;
	
	for (i=0; i<MV_OSAL_CPU_NUM; i++) {
		
		MV_OSAL_Sem_Create(&(pHandle->m_ArrayWrSem[i]), 1);
					
		pHandle->m_ArrayWrCount[i] = 0;
		pHandle->m_ArrayRdCount[i] = 0;
	}
	
	if (ThisCPU != MV_OSAL_CPU_MASTER)
	{
		// Get FIFO information
		pFIFO = (pMV_CC_ICCFIFO_t)pICCFIFOArrayStart;
		FIFOSize = pFIFO->m_FIFOSize;
		FrameSize = pFIFO->m_FrameSize;
		if (FrameSize != 0)
		{
			FrameNMax = pFIFO->m_FIFOSize / pFIFO->m_FrameSize;
			FIFOFullSize = MV_CC_ICCFIFO_FullSize(FrameNMax, FrameSize);
			
			MV_CC_DBG_Info("(CPU-%d)ICCFIFO ( %d X %d = %d) (SLAVE CPU)\n", 
				ThisCPU, FrameSize, FrameNMax, FIFOFullSize);
		}
		else
			MV_CC_DBG_Error(E_FAIL, "MV_CC_ICCFIFOArrayInit FrameSize==0", NULL);
	}
	else
	{
		FIFOSize = MV_CC_ICCFIFO_FULL_SIZE - MV_CC_HAL_MEMBOUNDARY;
		FIFOFullSize = MV_CC_ICCFIFO_FULL_SIZE;
		FrameSize = MV_CC_ICCFIFO_FRAME_SIZE;
		FrameNMax = MV_CC_ICCFIFO_NMAX;
		
		MV_CC_DBG_Info("(CPU-%d)ICCFIFO ( %d X %d = %d) (MASTER CPU)\n", 
			ThisCPU, FrameSize, FrameNMax, FIFOFullSize);
	}
	
	for (i=0; i<MV_OSAL_CPU_NUM; i++)
		for (j=0; j<MV_OSAL_CPU_NUM; j++) {
		
			pHandle->m_pArray[i][j] = (MV_CC_ICCFIFO_t *)((UCHAR *)(pICCFIFOArrayStart) + (
										(i * MV_OSAL_CPU_NUM ) + j) * FIFOFullSize );
	
			MV_CC_DBG_Info("(CPU-%d)(CPU-%d) = %08xh\n", i, j, (unsigned int)pHandle->m_pArray[i][j]);

			/* Only Master CPU could initialize the every MV_CC_ICCFIFO of MV_CC_ICCFIFOArray*/		
			if (ThisCPU == MV_OSAL_CPU_MASTER) {
				
				res = MV_CC_ICCFIFOInit(pHandle->m_pArray[i][j], 
										FrameNMax, 
										FrameSize);
				if (res != S_OK)
					MV_CC_DBG_Error(res, "MV_CC_ICCFIFOArrayInit MV_CC_ICCFIFOInit", NULL);
			}
	}
	
	return S_OK;	
}



INT  MV_CC_ICCFIFOArrayDestroy(pMV_CC_ICCFIFOArray_t pHandle)
{
	int i;
	
	/* Parameter Check */
	if (pHandle == NULL)
		MV_CC_DBG_Error(E_INVALIDARG, "MV_CC_ICCFIFOArrayDestroy", NULL);
		
	for (i=0; i<MV_OSAL_CPU_NUM; i++) {
		
		MV_OSAL_Sem_Destroy(&(pHandle->m_ArrayWrSem[i]));
		
			
	}
	
	return S_OK;			
}



INT MV_CC_ICCFIFOArrayRead(	pMV_CC_ICCFIFOArray_t pHandle, 
							MV_OSAL_CPUID_U8_t SrcCPU, 
							UCHAR *pDstBuf)
{
	int res;
	
	pMV_CC_ICCFIFO_t pICCFIFO;
	
	if ((pHandle == NULL) ||
		(SrcCPU >= MV_OSAL_CPU_NUM) ||
		(pDstBuf == NULL) ||
		((pICCFIFO = pHandle->m_pArray[pHandle->m_LocalCPU][SrcCPU]) == NULL) )
		MV_CC_DBG_Error(E_INVALIDARG, "MV_CC_ICCFIFOArrayRead", NULL);

	res =  MV_CC_ICCFIFORead(pICCFIFO, pDstBuf);		
	if ((res < 0 ) && (res != E_EMPTY))
		MV_CC_DBG_Error(res, "MV_CC_ICCFIFOArrayRead MV_CC_ICCFIFORead", NULL);

	pHandle->m_ArrayRdCount[SrcCPU]++;
	
		
	return res;
}



INT MV_CC_ICCFIFOArrayWrite(pMV_CC_ICCFIFOArray_t pHandle, 
							MV_OSAL_CPUID_U8_t DstCPU, 
							UCHAR *pSrcBuf)
{
	int res;
	
	pMV_CC_ICCFIFO_t pICCFIFO;
	
	if ((pHandle == NULL) ||
		(DstCPU >= MV_OSAL_CPU_NUM) ||
		(pSrcBuf == NULL) ||
		((pICCFIFO = pHandle->m_pArray[DstCPU][pHandle->m_LocalCPU]) == NULL) )
		MV_CC_DBG_Error(E_INVALIDARG, "MV_CC_ICCFIFOArrayWrite", NULL);
		
	MV_OSAL_Sem_Wait(pHandle->m_ArrayWrSem[DstCPU]);
	

	res = MV_CC_ICCFIFOWrite(pICCFIFO, pSrcBuf);			
	if ((res < 0 ) && (res != E_FULL))
		MV_CC_DBG_Error(res, "MV_CC_ICCFIFOArrayWrite MV_CC_ICCFIFOWrite", NULL);
		
	pHandle->m_ArrayWrCount[DstCPU]++;

	
	MV_OSAL_Sem_Post(pHandle->m_ArrayWrSem[DstCPU]);
	
	
	return res;
}			

