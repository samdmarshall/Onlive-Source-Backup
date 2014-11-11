#if !defined(COMMON_DEBUG_H)
#define COMMON_DEBUG_H

/* 
 *	Marvell Debug Interface
 * 
 *	MACRO
 *		MV_DEBUG is defined in debug version not in release version.
 *	
 *	Debug funtions:
 *		MV_PRINT:	print string in release and debug build.
 *		MV_DPRINT:	print string in debug build.
 *		MV_TRACE:	print string including file name, line number in release and debug build.
 *		MV_DTRACE:	print string including file name, line number in debug build.
 *		MV_ASSERT:	assert in release and debug build.
 *		MV_DASSERT: assert in debug build.
 */

/*
 *
 * Debug funtions
 *
 */
/* For both debug and release version */
//#define MV_PRINT galois_printk
#define MV_PRINT(...)
#define MV_ERROR_PRINT	galois_printk

#define MV_ASSERT(x)    do { \
				if ( !(x) ) \
					MV_PRINT("Debug at File %s: Line %d.\n", __FILE__, __LINE__); \
	                } while (0)

#define MV_TRACE(_x_)   do {\
				MV_PRINT("%s(%d) ", __FILE__, __LINE__); \
				MV_PRINT _x_;\
			} while(0)


/* 
 * Used with MV_DBG macro, see below .
 * Should be useful for Win driver too, so it is placed here.
 *
 */

#define DMSG_CORE    0x0001  /* CORE dbg msg */
#define DMSG_KERN    0x0002  /* kernel driver dbg msg */
#define DMSG_SCSI    0x0004  /* SCSI Subsystem dbg msg */
#define DMSG_ACDB    0x0008  /* A.C.'s debug msg */
#define DMSG_HBA     0x0010  /* HBA dbg msg */
#define DMSG_ERRH    0x0020  /* Error Handling dbg msg */
#define DMSG_FREQ    0x0040  /* indicates it's _VERR_ frequent dbg msg, will jam your console and severely impact your performance */
#define DMSG_IOCTL   0x0080  /* err in ioctl */
#define DMSG_MSG     0x0100  /* plain message, should be enabled all time */
#define DMSG_SCSI_FREQ 0x0200/* freq scsi dbg msg */
#define DMSG_RAID    0x0400  /* raid dbg msg */
#define DMSG_PROF    0x0800  /* profiling msg */
#define DMSG_PROF_FREQ 0x1000 /* freq profiling msg */
#define DMSG_TRACE   0x2000  /* trace msg */

/* For debug version only */
#if defined(MV_DEBUG)
        #ifdef _OS_LINUX
                extern unsigned int mv_dbg_opts;
                #define MV_DBG(x,...) do {\
			                      if (x&mv_dbg_opts) \
				                      MV_PRINT(__VA_ARGS__); \
    		                      } while (0)
                /* MV_DPRINT to be treated as CORE related debug msg  */
                #define MV_DPRINT(x)  do {\
			                      if (DMSG_CORE&mv_dbg_opts) \
				                      MV_PRINT x; \
    		                      } while (0)
        #else
                #define MV_DPRINT(x)	MV_PRINT x
                /* in case drivers for non-linux os go crazy */
                /* MS compiler doesn't support variadic parameter ...
                 * that's pre VS2005
                 */
                /* #define MV_DBG(x, ...)       do{}while(0) */
        #endif /* _OS_LINUX */

	#define MV_DASSERT	MV_ASSERT
	#define MV_DTRACE	MV_DTRACE
#else
        #ifdef _OS_LINUX
                #define MV_DBG(x,...)       do{}while(0)
        #endif
	#define MV_DPRINT(x)
	#define MV_DASSERT(x)
	#define MV_DTRACE(x)
#endif

MV_BOOLEAN mvLogRegisterModule(MV_U8 moduleId, MV_U32 filterMask, char* name);
MV_BOOLEAN mvLogSetModuleFilter(MV_U8 moduleId, MV_U32 filterMask);
MV_U32 mvLogGetModuleFilter(MV_U8 moduleId);
void mvLogMsg(MV_U8 moduleId, MV_U32 type, char* format, ...);

#endif /* COMMON_DEBUG_H */

