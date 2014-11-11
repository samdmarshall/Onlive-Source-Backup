

#ifndef _BOUNDCHECKER_H_
#define _BOUNDCHECKER_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined ( __ECOS__ ) && defined ( SMALL_MEM_POOL_SUPPORT )
HRESULT CommonMemPoolCreate(void);
void CommonMemPoolDestroy();                                                                 
void *CommonMemPoolMalloc(UINT size);
void CommonMemPoolFree(void *pMemAddr);
#endif

#ifdef ENABLE_BOUNDCHECKER

typedef struct _MV_MON_LogType_Memory {
	
	UINT32	m_cmd;
	UINT32	m_data1;
	UINT32	m_data2;
	UINT32	m_data3;
	UINT32	m_data4;
	char	m_filename[50];
	UINT32	m_data5[20];
} MV_MON_LogType_Memory_t, *pMV_MON_LogType_Memory_t;

HRESULT BoundChecker_Create();
HRESULT BoundChecker_Create2(UINT uiPoolSize);
HRESULT BoundChecker_Destroy();
HRESULT BoundChecker_Report();
HRESULT BoundChecker_ReportDelta();
PVOID BoundChecker_Malloc(UINT uiLength, UINT uiAlign, CHAR* szFileName, UINT uiLineNumber);
PVOID BoundChecker_Calloc(UINT numElements, UINT sizeOfElement, CHAR* szFileName, UINT uiLineNumber);
HRESULT BoundChecker_Free(PVOID pData, BOOL bAligned);
HRESULT  TEST_StartMemCheck(UINT32 *uiBytesUsed);
INT  TEST_CheckMemLeak(UINT32 *uiBytesUsed);


#else
#define BoundChecker_Create()		S_OK
#define BoundChecker_Destroy()		S_OK
#define BoundChecker_Report()		S_OK
#define BoundChecker_Malloc()		S_OK
#define BoundChecker_Calloc()		S_OK
#define BoundChecker_Free()		S_OK
#define BoundChecker_ReportDelta()		S_OK
#define TEST_StartMemCheck(...)		S_OK
#define TEST_CheckMemLeak(...)		S_OK
#endif //<! ENABLE_BOUNDCHECKER

#ifdef __cplusplus
}
#endif

#endif //<! _BOUNDCHECKER_H_
