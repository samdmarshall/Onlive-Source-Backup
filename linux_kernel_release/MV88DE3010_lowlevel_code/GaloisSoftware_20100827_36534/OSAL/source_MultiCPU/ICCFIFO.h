/*******************************************************************************
*******************************************************************************/



#ifndef _ICCFIFO_H_
#define _ICCFIFO_H_

#include "OSAL_config.h"
#include "OSAL_type.h"

typedef struct _MV_CC_ICCFIFO {
	
volatile	UINT32 m_StartOffset;	//! the offset of the FIFO buffer start
volatile	UINT32 m_RdOffset;		//! read offset of the FIFO buffer
volatile	UINT32 m_WrOffset;		//! write offset of the FIFO buffer
volatile	UINT32 m_FIFOSize;		//! ICCFIFO byte size exclude the FIFO head
volatile	UINT32 m_FrameSize;		//! ICCFIFO byte Frame size
volatile	UINT32 m_Flags;			//! flags of the FIFO buffer (Unused now!)
	
} MV_CC_ICCFIFO_t, *pMV_CC_ICCFIFO_t;



INT  MV_CC_ICCFIFOInit(pMV_CC_ICCFIFO_t pHandle, UINT32 FrameNMax, UINT32 FrameSize);


INT  MV_CC_ICCFIFORead(pMV_CC_ICCFIFO_t pHandle, UCHAR *pDstBuf);


INT  MV_CC_ICCFIFOReadN(pMV_CC_ICCFIFO_t pHandle, UCHAR *pDstBuf, UINT32 FrameNum);



INT  MV_CC_ICCFIFOReadGetAvailData(pMV_CC_ICCFIFO_t pHandle);


INT  MV_CC_ICCFIFOReadIfEmpty(pMV_CC_ICCFIFO_t pHandle);


INT  MV_CC_ICCFIFOReadDiscard(pMV_CC_ICCFIFO_t pHandle, UINT32 FrameNum);


INT  MV_CC_ICCFIFOWrite(pMV_CC_ICCFIFO_t pHandle, UCHAR *pSrcBuf);


INT  MV_CC_ICCFIFOWriteN(pMV_CC_ICCFIFO_t pHandle, UCHAR *pSrcBuf, UINT32 FrameNum);


INT  MV_CC_ICCFIFOWriteGetAvailSpace(pMV_CC_ICCFIFO_t pHandle);


INT  MV_CC_ICCFIFOWriteIfFull(pMV_CC_ICCFIFO_t pHandle);


INT  MV_CC_ICCFIFOCheck(pMV_CC_ICCFIFO_t pHandle);


#endif
