/*******************************************************************************
*******************************************************************************/



#include "OSAL_api.h"

#include "ICCHAL.h"

#include "ICCFIFO.h"



INT  MV_CC_ICCFIFOInit(pMV_CC_ICCFIFO_t pHandle, UINT32 FrameNMax, UINT32 FrameSize)
{
	UINT32 FIFOFullSize;
	
	if ((pHandle == NULL) ||
		(FrameSize == 0) || 
		(FrameNMax < MV_CC_ICCFIFO_FrameNMin))
		return E_INVALIDARG;
		
	FIFOFullSize = MV_CC_ICCFIFO_FullSize(FrameNMax, FrameSize);
	GaloisMemClear((void *)pHandle, FIFOFullSize);

	pHandle->m_StartOffset = MV_CC_HAL_MEMBOUNDARY;
	pHandle->m_FIFOSize = FIFOFullSize - MV_CC_HAL_MEMBOUNDARY;
	pHandle->m_FrameSize = FrameSize;
	

	MV_CC_HAL_FlushMemory(pHandle->m_FrameSize);
	
	return S_OK;
}



INT  MV_CC_ICCFIFORead(pMV_CC_ICCFIFO_t pHandle, UCHAR *pDstBuf)
{
	INT res,RdOffset,FrameSize,FIFOSize; 

	UCHAR *src_addr;
	
	if ((pHandle == NULL) ||
		(pDstBuf == NULL) ||
		((src_addr = ((UCHAR *)pHandle) + pHandle->m_StartOffset) == NULL))
		return E_INVALIDARG;
		
		
	res = MV_CC_ICCFIFOReadGetAvailData(pHandle);
	if ( res == 0 )
		return E_EMPTY;
	else
	if ( res < 0 )
		return E_FAIL;

	RdOffset = pHandle->m_RdOffset;		
	FrameSize = pHandle->m_FrameSize;
	FIFOSize = pHandle->m_FIFOSize;

	src_addr += RdOffset;
	GaloisMemcpy(pDstBuf, src_addr, FrameSize);
	
	
	RdOffset += FrameSize;
	if (RdOffset >= FIFOSize)
		pHandle->m_RdOffset = 0;
	else
		pHandle->m_RdOffset = RdOffset;
		
	MV_CC_HAL_FlushMemory(pHandle->m_RdOffset);
	
	
	return S_OK;
}



INT  MV_CC_ICCFIFOReadN(pMV_CC_ICCFIFO_t pHandle, UCHAR *pDstBuf, UINT32 FrameNum)
{
	UINT i, RdOffset,FrameSize,FIFOSize; 
	INT DataNum;

	UCHAR *src_addr;
	
	if ((pHandle == NULL) ||
		(pDstBuf == NULL) ||
		((src_addr = ((UCHAR *)pHandle) + pHandle->m_StartOffset) == NULL))
		return E_INVALIDARG;

	DataNum = MV_CC_ICCFIFOReadGetAvailData(pHandle);
	if ( DataNum == 0 )
		return E_EMPTY;
	else
	if ( DataNum < 0 )
		return E_FAIL;

	RdOffset = pHandle->m_RdOffset;		
	FrameSize = pHandle->m_FrameSize;
	FIFOSize = pHandle->m_FIFOSize;

	for (i=0; (i<FrameNum) && (i<(UINT)DataNum) ; i++)		
	{
		GaloisMemcpy(pDstBuf, src_addr + RdOffset, FrameSize);
		
		pDstBuf += FrameSize;
		RdOffset += FrameSize;
		if (RdOffset >= FIFOSize)
			RdOffset = 0;
	}
	
	pHandle->m_RdOffset = RdOffset;
	MV_CC_HAL_FlushMemory(pHandle->m_RdOffset);
	
	
	return i;
}




INT  MV_CC_ICCFIFOReadGetAvailData(pMV_CC_ICCFIFO_t pHandle)
{
	INT DataSize;
	
	//For performance, the parameter m_WrOffset and m_RdOffset don't check.
	if (pHandle == NULL)
		return E_INVALIDARG;
	
	DataSize = pHandle->m_WrOffset - pHandle->m_RdOffset;
	
	if (DataSize < 0)
		DataSize += pHandle->m_FIFOSize;
		
	return (DataSize/(pHandle->m_FrameSize));
}



INT  MV_CC_ICCFIFOReadIfEmpty(pMV_CC_ICCFIFO_t pHandle)
{
	return E_NOTIMPL;
}



INT  MV_CC_ICCFIFOReadDiscard(pMV_CC_ICCFIFO_t pHandle, UINT32 FrameNum)
{
	if ((pHandle == NULL) || (FrameNum != 0))
		return E_INVALIDARG;

	pHandle->m_RdOffset = pHandle->m_WrOffset;
		
	MV_CC_HAL_FlushMemory(pHandle->m_RdOffset);
	
	return S_OK;
}



INT  MV_CC_ICCFIFOWrite(pMV_CC_ICCFIFO_t pHandle, UCHAR *pSrcBuf)
{
	INT res,WrOffset,FrameSize,FIFOSize; 

	UCHAR *dst_addr;
	
	if ((pHandle == NULL) ||
		(pSrcBuf == NULL) ||
		((dst_addr = ((UCHAR *)pHandle) + pHandle->m_StartOffset) == NULL))
		return E_INVALIDARG;
		
		
	res = MV_CC_ICCFIFOWriteGetAvailSpace(pHandle);
	if ( res == 0 )
		return E_FULL;
	else
	if ( res < 0 )
		return E_FAIL;

	WrOffset = pHandle->m_WrOffset;		
	FrameSize = pHandle->m_FrameSize;
	FIFOSize = pHandle->m_FIFOSize;

	dst_addr += WrOffset;
	GaloisMemcpy(dst_addr, pSrcBuf, FrameSize);
	
	WrOffset += FrameSize;
	if (WrOffset >= FIFOSize)
		pHandle->m_WrOffset = 0;
	else
		pHandle->m_WrOffset = WrOffset;
		
	MV_CC_HAL_FlushMemory(pHandle->m_WrOffset);
	
	
	return S_OK;	
}


INT  MV_CC_ICCFIFOWriteN(pMV_CC_ICCFIFO_t pHandle, UCHAR *pSrcBuf, UINT32 FrameNum)
{
	UINT i, WrOffset,FrameSize,FIFOSize; 
	INT SpaceNum;

	UCHAR *dst_addr;
	
	if ((pHandle == NULL) ||
		(pSrcBuf == NULL) ||
		((dst_addr = ((UCHAR *)pHandle) + pHandle->m_StartOffset) == NULL))
		return E_INVALIDARG;

	SpaceNum = MV_CC_ICCFIFOWriteGetAvailSpace(pHandle);
	if ( SpaceNum == 0 )
		return E_FULL;
	else
	if ( SpaceNum < 0 )
		return E_FAIL;

	WrOffset = pHandle->m_WrOffset;		
	FrameSize = pHandle->m_FrameSize;
	FIFOSize = pHandle->m_FIFOSize;

	for (i=0; (i<FrameNum) && (i<(UINT)SpaceNum) ; i++)		
	{
		GaloisMemcpy(dst_addr + WrOffset, pSrcBuf, FrameSize);
		
		pSrcBuf += FrameSize;
		WrOffset += FrameSize;
		if (WrOffset >= FIFOSize)
			WrOffset = 0;
	}
	
	pHandle->m_WrOffset = WrOffset;
	MV_CC_HAL_FlushMemory(pHandle->m_WrOffset);
	
	
	return i;
}



INT  MV_CC_ICCFIFOWriteGetAvailSpace(pMV_CC_ICCFIFO_t pHandle)
{
	INT SpaceSize,res;
	
	//For performance, the parameter m_WrOffset and m_RdOffset don't check.
	if (pHandle == NULL)
		return E_INVALIDARG;
	
	SpaceSize = pHandle->m_RdOffset - pHandle->m_WrOffset;
	
	if (SpaceSize <= 0)
		SpaceSize += pHandle->m_FIFOSize;
		
	//The 1 Frame should be set apart for the pointer check, avoid the overflow.
	res = SpaceSize/(pHandle->m_FrameSize);
	res --;
		
	return res;
}


INT  MV_CC_ICCFIFOWriteIfFull(pMV_CC_ICCFIFO_t pHandle)
{
	return E_NOTIMPL;
}


INT  MV_CC_ICCFIFOCheck(pMV_CC_ICCFIFO_t pHandle)
{
	//For performance, the parameter m_WrOffset and m_RdOffset don't check.
	if (pHandle == NULL)
		return E_INVALIDARG;
	
	if (pHandle->m_FrameSize == 0)
		return E_FAIL;
	else
		return S_OK;
}
