/*******************************************************************************
*******************************************************************************/


#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "com_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ENABLE_DBGOUT

#define DBG_LV1		"[LEVEL1]"
#define DBG_LV2		"[LEVEL2]"
#define DBG_LV3		"[LEVEL3]"
#define DBG_LV4		"[LEVEL4]"
#define DBG_LV5		"[LEVEL5]"
#define DBG_AUDIO	"[AUDIO]"
#define DBG_VIDEO	"[VIDEO]"
#define DBG_NAV		"[NAV]"
#define DBG_OSAL	"[OSAL]"
#define DBG_CC		"[CC]"
#define DBG_DVDSPU	"[DVDSPU]"
#define DBG_PEAGENT	"[PEAGENT]"
#define DBG_VDM     "[VDM]"
#define DBG_VBUF    "[VBUF]"
#define DBG_GFX 	"[GFX]"
#define DBG_MON		"[MON]"
#define	DBG_PE		"[PE]"
#define	DBG_ADM		"[ADM]"
#define	DBG_VDM		"[VDM]"
#define	DBG_DC		"[DC]"
#define	DBG_DRM		"[DRM]"
#define	DBG_DMX		"[DMX]"
#define	DBG_LOG		"[LOG]"

HRESULT dbg_CoreInit( void );
BOOL dbg_CoreIsInited( void );
HRESULT dbg_CoreDeinit( void );

void dbg_Out( CHAR* fmt, ... );

void dbg_OutInISR( CHAR* fmt, ... );

HRESULT dbg_Out_ISRBuf_Transfer();

CHAR *dbg_OutGetKWL( void );

HRESULT dbg_OutRemoveKey( CHAR* pcKWL );

HRESULT dbg_OutAddKey( CHAR* pcKWL );

HRESULT dbg_Out_SetLevel(UINT uiLevel);

HRESULT dbg_Out_GetLevel(UINT *ptr_uiLevel);

CHAR *dbg_OutToLogGetKWL( void );

HRESULT dbg_OutToLogRemoveKey( CHAR* pcKWL );

HRESULT dbg_OutToLogAddKey( CHAR* pcKWL );

HRESULT dbg_OutToLog_SetLevel(UINT uiLevel);

HRESULT dbg_OutToLog_GetLevel(UINT *ptr_uiLevel);

	#ifdef ENABLE_SYSMON
	
	void dbg_OutToLog_Start(HANDLE hLogBuf);
	
	void dbg_OutToLog_Stop();
	
	#else
	
	#define dbg_OutToLog_Start( ... )
	
	#define dbg_OutToLog_Stop( ... )
	
	#endif

#else
#define dbg_CoreInit()					S_OK
#define dbg_CoreIsInited()				S_FALSE
#define dbg_CoreDeinit()				S_OK

#define dbg_Out( ... )
#define dbg_OutInISR( ... )	
#define dbg_Out_ISRBuf_Transfer()		S_OK
#define dbg_OutGetKWL( ... ) 			NULL
#define dbg_OutRemoveKey( ... )			S_OK
#define dbg_OutAddKey( ... )    		S_OK
#define dbg_OutToLogGetKWL( ... ) 		NULL
#define dbg_OutToLogRemoveKey( ... )	S_OK
#define dbg_OutToLogAddKey( ... )    	S_OK

#define dbg_OutToLog_Start( ... )
#define dbg_OutToLog_Stop( ... )

#define dbg_Out_SetLevel( ... )			S_OK
#define dbg_Out_GetLevel( ... )			S_OK

#define dbg_OutToLog_SetLevel( ... )	S_OK
#define dbg_OutToLog_GetLevel( ... )	S_OK

#endif	// ENABLE_DBGOUT



void t_Assert( const CHAR* pucFile, UINT uiLine, UINT bIn );

#ifdef DISABLE_ASSERT
#define MV_ASSERT( a )
#else
#define MV_ASSERT( a )					t_Assert( __FILE__, __LINE__, (UINT)( a ) )
#endif



#ifdef ENABLE_DBGOUT

	// Module = Navigator

	#define	MV_NAV_DBG_Info(...)				dbg_Out(DBG_NAV DBG_LV5 " " __VA_ARGS__)
	#define	MV_NAV_DBG_Warning(x, s, ...)		dbg_Out(DBG_NAV DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
	#define	MV_NAV_DBG_Error(x, s, ...)			{ \
												dbg_Out(DBG_NAV DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); \
												MV_ASSERT(0); \
												}
	// Module = Audio

	#define	MV_AUDIO_DBG_Info(...)				dbg_Out(DBG_AUDIO DBG_LV5 " " __VA_ARGS__)
	#define	MV_AUDIO_DBG_Warning(x, s, ...)		dbg_Out(DBG_AUDIO DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
	#define	MV_AUDIO_DBG_Error(x, s, ...)		{ \
												dbg_Out(DBG_AUDIO DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); \
												MV_ASSERT(0); \
												}

	// Module = Video

	#define	MV_VIDEO_DBG_Info(...)				dbg_Out(DBG_VIDEO DBG_LV5 " " __VA_ARGS__)
	#define	MV_VIDEO_DBG_Warning(x, s, ...)		dbg_Out(DBG_VIDEO DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
	#define	MV_VIDEO_DBG_Error(x, s, ...)		{ \
												dbg_Out(DBG_VIDEO DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); \
												MV_ASSERT(0); \
												}

	// Module = ADM

	#define	MV_ADM_DBG_Info(...)				dbg_Out(DBG_ADM DBG_LV5 " " __VA_ARGS__)
	#define	MV_ADM_DBG_Warning(x, s, ...)		dbg_Out(DBG_ADM DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
	#define	MV_ADM_DBG_Error(x, s, ...)			{ \
												dbg_Out(DBG_ADM DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); \
												MV_ASSERT(0); \
												}

	// Module = VDM

	#define	MV_VDM_DBG_Info(...)				dbg_Out(DBG_VDM DBG_LV5 " " __VA_ARGS__)
	#define	MV_VDM_DBG_Warning(x, s, ...)		dbg_Out(DBG_VDM DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
	#define	MV_VDM_DBG_Error(x, s, ...)			{ \
												dbg_Out(DBG_VDM DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); \
												MV_ASSERT(0); \
												}

	// Module = GFX

	#define	MV_GFX_DBG_Info(...)				dbg_Out(DBG_GFX DBG_LV5 " " __VA_ARGS__)
	#define	MV_GFX_DBG_Warning(x, s, ...)		dbg_Out(DBG_GFX DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
	#define	MV_GFX_DBG_Error(x, s, ...)			{ \
												dbg_Out(DBG_GFX DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); \
												MV_ASSERT(0); \
												}

	// Module = DC

	#define	MV_DC_DBG_Info(...)					dbg_Out(DBG_DC DBG_LV5 " " __VA_ARGS__)
	#define	MV_DC_DBG_Warning(x, s, ...)		dbg_Out(DBG_DC DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
	#define	MV_DC_DBG_Error(x, s, ...)			{ \
												dbg_Out(DBG_DC DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); \
												MV_ASSERT(0); \
												}
	// Module = DRM

	#define	MV_DRM_DBG_Info(...)				dbg_Out(DBG_DRM DBG_LV5 " " __VA_ARGS__)
	#define	MV_DRM_DBG_Warning(x, s, ...)		dbg_Out(DBG_DRM DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
	#define	MV_DRM_DBG_Error(x, s, ...)			{ \
												dbg_Out(DBG_DRM DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); \
												MV_ASSERT(0); \
												}
	// Module = DMX

	#define	MV_DMX_DBG_Info(...)				dbg_Out(DBG_DMX DBG_LV5 " " __VA_ARGS__)
	#define	MV_DMX_DBG_Warning(x, s, ...)		dbg_Out(DBG_DMX DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
	#define	MV_DMX_DBG_Error(x, s, ...)			{ \
												dbg_Out(DBG_DMX DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); \
												MV_ASSERT(0); \
												}
#else

	// Module = Navigator

	#define	MV_NAV_DBG_Info(...)				
	#define	MV_NAV_DBG_Warning(x, s, ...)		
	#define	MV_NAV_DBG_Error(x, s, ...)			{ MV_ASSERT(x == S_OK); }
	
	// Module = Audio

	#define	MV_AUDIO_DBG_Info(...)				
	#define	MV_AUDIO_DBG_Warning(x, s, ...)		
	#define	MV_AUDIO_DBG_Error(x, s, ...)		{ MV_ASSERT(x == S_OK); }

	// Module = Video

	#define	MV_VIDEO_DBG_Info(...)			
	#define	MV_VIDEO_DBG_Warning(x, s, ...)		
	#define	MV_VIDEO_DBG_Error(x, s, ...)		{ MV_ASSERT(x == S_OK); }	

	// Module = ADM

	#define	MV_ADM_DBG_Info(...)			
	#define	MV_ADM_DBG_Warning(x, s, ...)		
	#define	MV_ADM_DBG_Error(x, s, ...)			{ MV_ASSERT(x == S_OK); }	

	// Module = VDM

	#define	MV_VDM_DBG_Info(...)			
	#define	MV_VDM_DBG_Warning(x, s, ...)		
	#define	MV_VDM_DBG_Error(x, s, ...)			{ MV_ASSERT(x == S_OK); }	

	// Module = GFX

	#define	MV_GFX_DBG_Info(...)			
	#define	MV_GFX_DBG_Warning(x, s, ...)		
	#define	MV_GFX_DBG_Error(x, s, ...)			{ MV_ASSERT(x == S_OK); }	

	// Module = DC

	#define	MV_DC_DBG_Info(...)			
	#define	MV_DC_DBG_Warning(x, s, ...)		
	#define	MV_DC_DBG_Error(x, s, ...)			{ MV_ASSERT(x == S_OK); }	

	// Module = DRM

	#define	MV_DRM_DBG_Info(...)			
	#define	MV_DRM_DBG_Warning(x, s, ...)		
	#define	MV_DRM_DBG_Error(x, s, ...)			{ MV_ASSERT(x == S_OK); }	
	
	// Module = DMX

	#define	MV_DMX_DBG_Info(...)			
	#define	MV_DMX_DBG_Warning(x, s, ...)		
	#define	MV_DMX_DBG_Error(x, s, ...)			{ MV_ASSERT(x == S_OK); }	
	
		
#endif	// ENABLE_DBGOUT

#define DBG_MAX_TASK_COUNT	64
typedef struct
{
	INT		m_iID;
	INT		m_iPriority;
	INT64	m_iPeriod;
	INT		m_iSwitches_all;
	INT		m_iSwitches_period;
} tTask_Data;

typedef struct
{
	INT		m_iTask_Cnt;
	BOOL	m_bTotal_Task_Number_Overflow;
	INT64	m_iPeriodStartTime;
	INT64	m_iPeriodEndTime;
	tTask_Data	m_Task_Data[DBG_MAX_TASK_COUNT];
} tTask_Statistics;

extern tTask_Statistics Task_Stat;

HRESULT MV_DBG_CPU_Loading_Start_Count( void);
HRESULT MV_DBG_CPU_Loading_Stop_Count( void );
HRESULT MV_DBG_CPU_Loading_Output( void );
HRESULT MV_DBG_CPU_Loading_Stop_Count_And_Output( void );

#ifdef __cplusplus
}
#endif

#endif	// _DEBUG_H_

