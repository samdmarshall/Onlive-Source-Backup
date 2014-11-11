/*******************************************************************************
*******************************************************************************/


#ifndef _DBG_INTERNAL_H_
#define _DBG_INTERNAL_H_

#ifdef ENABLE_DBGOUT

#if defined( __LINUX__ )
#include <stdarg.h>
#endif

#if defined( __LINUX__ )
#include <semaphore.h>
#define gettid()	1
#define Semaphore	sem_t
#define Semaphore_Create(sem,value)		sem_init(&sem,0,value)
#define Semaphore_Wait(sem)				sem_wait(&sem)
#define Semaphore_Post(sem)				sem_post(&sem)
#define Semaphore_Destroy(sem)			sem_destroy(&sem)
#elif defined( __ECOS__ )
#include <cyg/kernel/kapi.h>
#define gettid()	1
#define Semaphore	cyg_sem_t
#define Semaphore_Create(sem,value) 	cyg_semaphore_init(&sem,value)
#define Semaphore_Wait(sem)				cyg_semaphore_wait(&sem)
#define Semaphore_Post(sem)				cyg_semaphore_post(&sem)
#define Semaphore_Destroy(sem)			cyg_semaphore_destroy(&sem)
#elif defined( WIN32 )
#include <windows.h>
#define gettid()	1
#define Semaphore	HANDLE
#define Semaphore_Create(sem,value)		(sem = CreateSemaphore(0, value, 0x7FFFFFFF, 0) )
#define Semaphore_Wait(sem)				WaitForSingleObject(sem,INFINITE)
#define Semaphore_Post(sem)				ReleaseSemaphore(sem,1,0)
#define Semaphore_Destroy(sem)			CloseHandle(sem)
#elif defined( __UBOOT__ ) || defined( __LINUX_KERNEL__ )
#define gettid()	1
#define Semaphore	INT
#define Semaphore_Create( ... )
#define Semaphore_Wait( ... )
#define Semaphore_Post( ... )
#define Semaphore_Destroy( ... )
#endif

typedef struct dbg_OutSys_ISRBuf_struct
{
	CHAR *m_pOutBuf;
	UINT32 m_uiBufSize;
	UINT32 m_uiWrOffset;
	UINT32 m_uiRdOffset;
	UINT32 m_uiWrCount;
	UINT32 m_uiRdCount;
	
} dbg_OutSys_ISRBuf_t;

typedef	struct dbg_OutSys_struct
{
	CHAR* m_pcConsoleKWL;
	INT m_iConsoleKWLLen;
	
	Semaphore	m_Sem;
	
#ifdef ENABLE_SYSMON

	CHAR* m_pcConsoleKWL_ToLog;
	INT m_iConsoleKWLLen_ToLog;

#endif

#if defined( __ECOS__ )
	dbg_OutSys_ISRBuf_t m_ISRBuf;
#endif

} dbg_OutSys;

VOID dbg_OutSetKWL( dbg_OutSys* pOut, CHAR* pcKWL );

typedef	struct dbg_CoreData_struct
{
	dbg_OutSys*	m_pOut;
} dbg_CoreData;

dbg_OutSys* dbg_OutSysCreate( void );
void dbg_OutSysDestroy( dbg_OutSys* pOut );

#define DBG_LEVEL_MAX				(5)
#define DBG_LEVEL_5					(5)
#define DBG_LEVEL_4					(4)
#define DBG_LEVEL_3					(3)
#define DBG_LEVEL_2					(2)
#define DBG_LEVEL_1					(1)
#define DBG_LEVEL_0					(0)
#define DBG_LEVEL_MIN				(0)

#include "dbg_settings.h"

#endif	// ENABLE_DBGOUT
#endif	// _DBG_INTERNAL_H_
