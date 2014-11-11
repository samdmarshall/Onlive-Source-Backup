/*******************************************************************************
*******************************************************************************/


#ifndef _DBG_SETTINGS_H_
#define _DBG_SETTINGS_H_

#ifndef ENABLE_DBGOUT

#ifdef DEBUG_LEVEL
#undef DEBUG_LEVEL
#endif //def DEBUG_LEVEL
#define DEBUG_LEVEL 0

#else

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 1
#endif

#endif	// ENABLE_DBGOUT


#if DEBUG_LEVEL>4
 #define DBG_LVL_STR DBG_LV1 DBG_LV2 DBG_LV3 DBG_LV4 DBG_LV5
#elif DEBUG_LEVEL>3
 #define DBG_LVL_STR DBG_LV1 DBG_LV2 DBG_LV3 DBG_LV4
#elif DEBUG_LEVEL>2
 #define DBG_LVL_STR DBG_LV1 DBG_LV2 DBG_LV3
#elif DEBUG_LEVEL>1
 #define DBG_LVL_STR DBG_LV1 DBG_LV2
#elif DEBUG_LEVEL>0
 #define DBG_LVL_STR DBG_LV1
#else
 #define DBG_LVL_STR
#endif


#define DBG_OUT_CONSOLE DBG_LVL_STR "[PE][OSAL][CC][DVDSPU][MON][ADM][GFX][VDM]"


#ifdef ENABLE_SYSMON

#define DBG_OUT_TOLOG DBG_LVL_STR "[OSAL][CC][DVDSPU][PEAGENT][VDM][VBUF][MON][PE][ADM]"

#endif

#define DBG_OUT_CONSOLE_KWLEN_MAX	(256)
#define DBG_OUT_TOLOGSTRING_MAX		(1024)
#define DBG_OUT_ISR_BUFFER_SIZE		(16 * 1024)

extern dbg_CoreData *gpDbgCore;

#endif	// _DBG_SETTINGS_H_

