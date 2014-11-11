/*******************************************************************************
*******************************************************************************/



#ifndef _COM_CONFIG_H_
#define _COM_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif	// #ifdef __cplusplus



#define MV_CC_HAL_MEMBOUNDARY				(64)			// in bytes, Hardware Memroy Boundary Size (DMA operation)

#include "sys_memmap.h"
#define MV_NONCACHE_Base					(MEMMAP_DRAM_UNCACHE_BASE) // refer to sys_memmap.h
#ifdef __LINUX_KERNEL__
#define MV_NONCACHE_Start					((MV_NONCACHE_Base) | (CONFIG_MV88DE3010_SHMMEM_START))
#define MV_NONCACHE_Size					(CONFIG_MV88DE3010_SHMMEM_SIZE)				// 64M Size for ICC FIFO, ICC EXTRA and Shared Memory
#else
#define MV_NONCACHE_Start					(GALOIS_NON_CACHE_START)  // refer to sys_memmap.h
#define MV_NONCACHE_Size					(GALOIS_NON_CACHE_SIZE)	  // refer to sys_memmap.h, 64M Size for ICC FIFO, ICC EXTRA and Shared Memory
#endif
#define MV_ICC_FIFO_ADDR_def				(MV_NONCACHE_Start)
#define MV_ICC_FIFO_SIZE_def				(0x100000)					// 1M data for ICC FIFO
#define MV_ICC_EXTRA_ADDR_def				(MV_NONCACHE_Start + MV_ICC_FIFO_SIZE_def)
#define MV_ICC_EXTRA_SIZE_def				(0x1000)					// 4K extra data for ICC
#ifdef __LINUX_KERNEL__
#define MV_ICC_FIFO_ADDR_def_KernVirt		(CONFIG_MV88DE3010_SHMMEM_VIRTSTART)						// Linux kernel Virtual Address of 0x5C000000
#define MV_ICC_EXTRA_ADDR_def_KernVirt		(MV_ICC_FIFO_ADDR_def_KernVirt + MV_ICC_FIFO_SIZE_def)			// Linux kernel Virtual Address
#endif




#if defined( __ECOS__ )

#define MV_OS_TICKNUM_PER_SECOND			(100)								// please ref the eCos setting
#define MV_OS_MS_PER_TICK					(1000 / MV_OS_TICKNUM_PER_SECOND)	
#define CYG_MS_PER_TICK						(MV_OS_MS_PER_TICK)					// from com_type.h

#endif

#if !defined(WIN32) && !defined(SoC_PCSim)

#define TIMER_SYSTEM						(6)								/* system timer, refer to pic_cpu_intr.h */
#define TIMER_DEBUG							(7)								/* performance timer */


#if defined( __LINUX__ ) || defined( __ECOS__ )
#if defined(FPGA)
#define SYSTEM_TIMER_CLOCK					(5000000)				// 5 MHz
#elif (BERLIN_CHIP_VERSION < BERLIN_B_0)
#define SYSTEM_TIMER_CLOCK					(65625000)				// 65.625 MHz
#elif (BERLIN_CHIP_VERSION >= BERLIN_B_0)
#define SYSTEM_TIMER_CLOCK					(75000000)				// 75 MHz
#else
#define SYSTEM_TIMER_CLOCK					(75000000)				// 75 MHz
#endif
#define SYSTEM_TIMER_RATIO					(1*1000*1000*1000 / SYSTEM_TIMER_CLOCK)
#define SYSTEM_TIMER_DELTA					(0xFFFFFFFF - 100 * 1000 / SYSTEM_TIMER_RATIO) 	
#endif

#define MV_TIME_CLOCK_UPDATE_MS				(200)

#define SYSTEM_TIMER_CLOCK_PROCFILE			"/proc/mv88de3010_timerfreq"

#define SYSTEM_TIMER_VALUE					(0xFFFFFFFF)
#define DEBUG_TIMER_VALUE					(0xFFFFFFFF)

#define PFTIMER_DEVICE 						"/dev/pftimer"

#define MV_SHM_DeviceName_ICCEXTRA			"/dev/MV_CC_SHM2"
#define MV_ADDR_system_timer_jiffies		(0)
#define MV_SIZE_system_timer_jiffies		(4)
#define MV_ADDR_system_Clock_Timer			(4)
#define MV_SIZE_system_Clock_Timer			(4)
#define MV_ADDR_system_Clock_Second			(8)
#define MV_SIZE_system_Clock_Second			(4)

#define MV_REGMAP_DEVICE					"/dev/regmap"
#define MV_ADDR_REG_TIMER_BASE				(0xF7F32000)
#define MV_SIZE_REG_TIMER_BASE				(0x1000)
#define MV_ADDR_REG_TIMER_OFFSET			(0x800)
#define MV_ADDR_REG_TIMER6_VALUE			(MV_ADDR_REG_TIMER_BASE + MV_ADDR_REG_TIMER_OFFSET + 0x14 * 6 + 0x04)
#define MV_SIZE_REG_TIMER6_VALUE			(4)
#define MV_ADDR_REG_TIMER7_VALUE			(MV_ADDR_REG_TIMER_BASE + MV_ADDR_REG_TIMER_OFFSET + 0x14 * 7 + 0x04)
#define MV_SIZE_REG_TIMER7_VALUE			(4)

#define MV_REG_TIMER_6						(6)
#define MV_REG_TIMER_7						(7)

#endif	// #if !defined(WIN32) && !defined(SoC_PCSim)



#define LOGTYPE_DEBUG_ALL	   				(0x00000001)
#define LOGTYPE_DEBUG_PE	   				(0x00010001)
#define LOGTYPE_TEST_RPC_PF   				(0x00010002)
#define LOGTYPE_TEST_RPC_TRACE				(0x00010003)
#define LOGTYPE_CPULOAD		   				(0x00010004)
#define LOGTYPE_CPUSWITCH	   				(0x00010005)
#define LOGTYPE_BUFFER						(0x00010006)
#define LOGTYPE_MEMORY						(0x00010007)   //for memory usage

#define LOGTYPE_TEST_RPC_TRACE_Num			(0x0c)			// number of the point
#define LOGTYPE_TEST_RPC_TRACE_PointSize	(0x04)			// 
#define LOGTYPE_TEST_RPC_TRACE_AllSize		(LOGTYPE_TEST_RPC_TRACE_Num * LOGTYPE_TEST_RPC_TRACE_PointSize)			// 

#define LOGTYPE_TEST_RPC_TRACE_0			(0x00)			// (CPU0 Linux User Space) User Task -> RPC Start 
#define LOGTYPE_TEST_RPC_TRACE_1			(0x01)			// (CPU0 Linux Kernel) MV_ICCNetWrite_process_message_thread 
#define LOGTYPE_TEST_RPC_TRACE_2			(0x02)			// (CPU1 eCos) MV_CC_MidSrv_ServerTask()->MV_CC_HAL_SemWait()
#define LOGTYPE_TEST_RPC_TRACE_3			(0x03)			// (CPU1 eCos) MV_CC_RPCSrv_Task()->MV_CC_RPC_TaskPool_WaitSignalAndGet()
#define LOGTYPE_TEST_RPC_TRACE_4			(0x04)			// (CPU1 eCos) MV_CC_RPCSrv_Task()->MV_CC_RPCSrv_Return()
#define LOGTYPE_TEST_RPC_TRACE_5			(0x05)			// (CPU0 Linux Kernel) (Linux kernel)->MV_ICCNetRead_process_message_thread()->MV_CC_HAL_SemWait()
#define LOGTYPE_TEST_RPC_TRACE_6			(0x06)			// (CPU0 Linux User Space) MV_CC_MidSrv_ServerTask()->recvmsg()
#define LOGTYPE_TEST_RPC_TRACE_7			(0x07)			// (CPU0 Linux User Space) MV_CC_MidSrv_ServerTask()->MV_CC_RPCClnt_OnUDPRecv()
#define LOGTYPE_TEST_RPC_TRACE_8			(0x08)			// (CPU0 Linux User Space) User Task -> MV_OSAL_Sem_TimedWait() RPC End

#define LOGTYPE_TEST_RPC_TRACE_CPU1INT		(0x09)			// (CPU1 INT)
#define LOGTYPE_TEST_RPC_TRACE_CPU1ISR		(0x0a)			// (CPU1 ISR)
#define LOGTYPE_TEST_RPC_TRACE_CPU1DSR		(0x0b)			// (CPU1 DSR)

#define TEST_RPC_TRACE_TIMER				(MV_REG_TIMER_6)
#define TEST_CPU_SWITCH_TIMER				(MV_REG_TIMER_6)



#define MV_LIBRARY_PATH_NAVMGR            "libNavMgr.so"
#define MV_LIBRARY_PATH_NAVIGATOR         "libNavi.so"
#define MV_LIBRARY_PATH_PLAYER            "libPlayer.so"

#ifdef __cplusplus
}		// extern "C"
#endif	// #ifdef __cplusplus

#endif
