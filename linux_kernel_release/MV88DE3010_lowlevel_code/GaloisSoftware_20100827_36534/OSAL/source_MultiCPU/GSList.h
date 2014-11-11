/*******************************************************************************
*******************************************************************************/



#ifndef _GSLIST_H_
#define _GSLIST_H_

#include "OSAL_api.h"

typedef VOID (*pGSList_VisitFunc_t)(void *arg, MV_CC_ServiceID_U32_t uiSID, pMV_CC_DSS_ServiceInfo_t pData);

HRESULT MV_CC_DSS_GlobalServiceList_Init(void);
HRESULT MV_CC_DSS_GlobalServiceList_Exit(void);
HRESULT MV_CC_DSS_GlobalServiceList_Add(MV_CC_ServiceID_U32_t uiSID, pMV_CC_DSS_ServiceInfo_t pData);
HRESULT MV_CC_DSS_GlobalServiceList_Delete(MV_CC_ServiceID_U32_t uiSID);
HRESULT MV_CC_DSS_GlobalServiceList_Get(MV_CC_ServiceID_U32_t uiSID, pMV_CC_DSS_ServiceInfo_t *ppData);
HRESULT MV_CC_DSS_GlobalServiceList_Traversal(pGSList_VisitFunc_t func_visit, void * arg);

void *MV_CC_DSS_GlobalServiceList_SrvInfo_Ctor(void);
void MV_CC_DSS_GlobalServiceList_SrvInfo_Dtor(void *obj);
void GSList_VisitFunc_Demo(PVOID arg, MV_CC_ServiceID_U32_t uiSID, pMV_CC_DSS_ServiceInfo_t pData);

#endif	// #define _GSLIST_H_
