/*******************************************************************************
*******************************************************************************/



#ifndef _ICCNET_H_
#define _ICCNET_H_


#include "OSAL_type.h"
#include "OSAL_Local_CPU_def.h"

#include "ICCHAL.h"

#include "ICCFIFOArray.h"


#include "ICCNet_Local.h"


INT  MV_CC_ICCNetInit(pMV_CC_ICCNet_t pHandle);



INT  MV_CC_ICCNetExit(pMV_CC_ICCNet_t pHandle, UINT command);


INT  MV_CC_ICCNetRead(pMV_CC_ICCNet_t pHandle, UCHAR *pDatagramBuf);


INT  MV_CC_ICCNetWrite(pMV_CC_ICCNet_t pHandle, UCHAR *pDatagramBuf);

#endif
