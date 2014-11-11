/*******************************************************************************
*******************************************************************************/



#include "std_map.h"
#include "GSList.h"

std_map* pMV_CC_DSS_GlobalServiceList;
std_map_node* pMV_CC_DSS_GlobalServiceList_nil;
BOOL bMV_CC_DSS_Status_GSList;

#define MV_CC_DSS_GlobalServiceList_Traversal_Stack_Depth				(50)
std_map_node* Traversal_Stack[MV_CC_DSS_GlobalServiceList_Traversal_Stack_Depth];

INT GlobalServiceList_compare_func(PVOID a, PVOID b)
{
	if (a > b)
		return 1;
	else
	if (a < b)
		return -1;
	else
		return 0;
}

void *MV_CC_DSS_GlobalServiceList_SrvInfo_Ctor()
{
	return MV_OSAL_Malloc(sizeof(MV_CC_DSS_ServiceInfo_t));
}

VOID MV_CC_DSS_GlobalServiceList_SrvInfo_Dtor(PVOID obj)
{
	if (obj)
	{
		MV_OSAL_Free(obj);
		obj = NULL;
	}
}

HRESULT MV_CC_DSS_GlobalServiceList_Init(void)
{
    bMV_CC_DSS_Status_GSList = false;
	pMV_CC_DSS_GlobalServiceList = NULL;
	pMV_CC_DSS_GlobalServiceList = std_map_ctor(pMV_CC_DSS_GlobalServiceList,
												GlobalServiceList_compare_func,
												NULL,
												MV_CC_DSS_GlobalServiceList_SrvInfo_Dtor,
												STD_MAP_NODE_POINTER);
												
	pMV_CC_DSS_GlobalServiceList_nil =  pMV_CC_DSS_GlobalServiceList->m_pNil;
	
	if (pMV_CC_DSS_GlobalServiceList == NULL)
		return E_OUTOFMEMORY;
	
	bMV_CC_DSS_Status_GSList = true;
	return S_OK;
}

HRESULT MV_CC_DSS_GlobalServiceList_Exit(void)
{
	HRESULT res;
	
	if (pMV_CC_DSS_GlobalServiceList == NULL)
		return E_NOTREADY;
		
	res = std_map_clear(pMV_CC_DSS_GlobalServiceList);
	if (res == S_OK)
	{
	    bMV_CC_DSS_Status_GSList = false;
		pMV_CC_DSS_GlobalServiceList = NULL;
	}

	return res; 
}

HRESULT MV_CC_DSS_GlobalServiceList_Add(MV_CC_ServiceID_U32_t uiSID, pMV_CC_DSS_ServiceInfo_t pData)
{
	HRESULT res;
	PVOID pData_find;
	
	if (pMV_CC_DSS_GlobalServiceList == NULL)
		return E_NOTREADY;
		
	res = std_map_find(pMV_CC_DSS_GlobalServiceList, (PVOID)uiSID, &pData_find);
	if (res != S_OK)
		return std_map_insert(pMV_CC_DSS_GlobalServiceList, (PVOID)uiSID, (PVOID)pData);
	else
		return E_ALREADYEXIST;
}

HRESULT MV_CC_DSS_GlobalServiceList_Delete(MV_CC_ServiceID_U32_t uiSID)
{
	if (pMV_CC_DSS_GlobalServiceList == NULL)
		return E_NOTREADY;
		
	return std_map_erase(pMV_CC_DSS_GlobalServiceList, (PVOID)uiSID);	
}

HRESULT MV_CC_DSS_GlobalServiceList_Get(MV_CC_ServiceID_U32_t uiSID, pMV_CC_DSS_ServiceInfo_t *ppData)
{
	if (pMV_CC_DSS_GlobalServiceList == NULL)
		return E_NOTREADY;
		
	return std_map_find(pMV_CC_DSS_GlobalServiceList, (PVOID)uiSID, (PVOID)ppData);	
}

VOID GSList_VisitFunc_Demo(PVOID arg, MV_CC_ServiceID_U32_t uiSID, pMV_CC_DSS_ServiceInfo_t pData)
{
#if defined( __LINUX_KERNEL__ )
#else
#endif
	return;
}

VOID Traversal_InOrder(std_map_node *x, pGSList_VisitFunc_t func_visit, PVOID arg)
{
	if (x != pMV_CC_DSS_GlobalServiceList_nil)
	{
		Traversal_InOrder(x->pLeft, func_visit, arg);
		if (func_visit != NULL)
			func_visit(arg, (MV_CC_ServiceID_U32_t)(x->pKey), (pMV_CC_DSS_ServiceInfo_t)(x->pData));
		Traversal_InOrder(x->pRight, func_visit, arg);
	}
	return;
}

VOID Traversal_InOrder_NoRecursion(std_map_node *x, pGSList_VisitFunc_t func_visit, PVOID arg)
{
	INT Stack_Top = 0;
	
	if (x != pMV_CC_DSS_GlobalServiceList_nil)
	while(Stack_Top >= 0)
	{
		// handle left child of Node
		if (x != pMV_CC_DSS_GlobalServiceList_nil)
		{
			// check stack depth
			if ( Stack_Top >= MV_CC_DSS_GlobalServiceList_Traversal_Stack_Depth)
				break;
			// push node into stack	
			Traversal_Stack[Stack_Top++] = x;
			
			x = x->pLeft;
		}
		else
		// find the leftest child of node
		{
			if (Stack_Top == 0)	// all done.
				break;
			// pop node from stack
			x = Traversal_Stack[--Stack_Top];
			
			if (func_visit != NULL)
				func_visit(arg, (MV_CC_ServiceID_U32_t)(x->pKey), (pMV_CC_DSS_ServiceInfo_t)(x->pData));
			
			x = x->pRight;
		}
	}
}

HRESULT MV_CC_DSS_GlobalServiceList_Traversal(pGSList_VisitFunc_t func_visit, PVOID arg)
{
	HRESULT res;
	UINT count;
	
	if (pMV_CC_DSS_GlobalServiceList == NULL)
		return E_NOTREADY;

	if (func_visit == NULL)
		return E_INVALIDARG;	
		
	res = std_map_size(pMV_CC_DSS_GlobalServiceList, &count);
	if (res != S_OK)
		return res;

#if defined( __LINUX_KERNEL__ )
#else
#endif

	//Inorder Traversal 
	//print all nodes information
	//warning!!! all nodes in the left of root
	Traversal_InOrder_NoRecursion(pMV_CC_DSS_GlobalServiceList->m_pRoot->pLeft, func_visit, arg);
	
	return S_OK;
}
