/*******************************************************************************
*******************************************************************************/


#ifndef _MAP_H_
#define _MAP_H_

#include "com_type.h"
#include "ErrorCode.h"

typedef enum
{
	STD_MAP_NODE_VALUE,
	STD_MAP_NODE_POINTER
} std_map_node_type;

typedef struct _std_map_node
{
	PVOID pData;
	PVOID pKey;
	UINT bRed;
	struct _std_map_node* pLeft;
	struct _std_map_node* pRight;
	struct _std_map_node* pParent;
} std_map_node;

/* Compare(a,b) should return 1 if *a > *b, -1 if *a < *b, and 0 otherwise */
typedef INT (*std_map_compare_func)(PVOID a, PVOID b);
typedef VOID (*std_map_dtor_func)(PVOID obj);

typedef struct _std_map
{
	std_map_compare_func compare;
	std_map_dtor_func key_dtor;
	std_map_dtor_func data_dtor;
	std_map_node* m_pRoot;
	std_map_node* m_pNil;
	std_map_node_type m_eType;
	UINT uiSize;
} std_map;

std_map* std_map_ctor(std_map* self,
		      std_map_compare_func compare,
		      std_map_dtor_func key_dtor,
		      std_map_dtor_func data_dtor,
		      std_map_node_type eType);
VOID std_map_dtor(std_map* self);
HRESULT std_map_clear(std_map* self);
HRESULT std_map_find(std_map* self, PVOID pKey, PVOID* ppData);
HRESULT std_map_insert(std_map* self, PVOID pKey, PVOID pData);
HRESULT std_map_erase(std_map* self, PVOID pKey);
HRESULT std_map_size(std_map* self, UINT* puiSize);
HRESULT std_map_iter(std_map* self, std_map_node** ppNode);

#endif //<! #ifndef _MAP_H_
