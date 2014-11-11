/*******************************************************************************
*******************************************************************************/


////////////////////////////////////////////////////////////////////////////////
//! \file sys_memmap.h
//! \brief system level memory map, define memory block address for different CPUs.
//!	
//!	
//!	
//! Purpose:
//!	   define memory address for CPUs
//!
//!	Version, Date and Author :
//!		V 1.00,	7/16/2007,	Fenglei Wang
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef _SYS_MEMMAP_H_
#define _SYS_MEMMAP_H_

/* Memory map: cpu0_mem + cpu1_mem + share_mem */ 
//#define SYSMEM_SETTING_96_352_64
//#define SYSMEM_SETTING_208_208_96
//#define SYSMEM_SETTING_138_288_86
//#define SYSMEM_SETTING_160_288_64
//#define SYSMEM_SETTING_44_200_12
//#define SYSMEM_SETTING_88_68_356
//#define SYSMEM_SETTING_56_60_140
//#define SYSMEM_SETTING_248_68_196
#define SYSMEM_SETTING_138_68_306 

/*
 * Memory map
 */
#if defined( SYSMEM_SETTING_88_68_356 )
#define SYSMEM_DSP_ADDR			0x0
#define SYSMEM_DSP_SIZE			0x00400000

#define SYSMEM_CPU1_ADDR		0x00400000
#define SYSMEM_CPU1_SIZE		0x04000000

#define SYSMEM_CPU0_ADDR		(SYSMEM_CPU1_ADDR+SYSMEM_CPU1_SIZE)
#define SYSMEM_CPU0_SIZE		0x05800000

#define SYSMEM_SHM_ADDR			(SYSMEM_CPU0_ADDR+SYSMEM_CPU0_SIZE)
#define SYSMEM_SHM_SIZE			0x16400000
#elif defined( SYSMEM_SETTING_96_352_64 )
#define SYSMEM_DSP_ADDR			0x0
#define SYSMEM_DSP_SIZE			0x00400000

#define SYSMEM_CPU1_ADDR		0x00400000
#define SYSMEM_CPU1_SIZE		0x15C00000

#define SYSMEM_CPU0_ADDR		0x16000000
#define SYSMEM_CPU0_SIZE		0x06000000

#define SYSMEM_SHM_ADDR			0x1C000000
#define SYSMEM_SHM_SIZE			0x04000000
#elif defined( SYSMEM_SETTING_208_208_96 )
#define SYSMEM_DSP_ADDR			0x0
#define SYSMEM_DSP_SIZE			0x00400000

#define SYSMEM_CPU1_ADDR		0x00400000
#define SYSMEM_CPU1_SIZE		0x0CC00000

#define SYSMEM_CPU0_ADDR		0x0D000000
#define SYSMEM_CPU0_SIZE		0x0D000000

#define SYSMEM_SHM_ADDR			0x1A000000
#define SYSMEM_SHM_SIZE			0x06000000
#elif defined( SYSMEM_SETTING_138_288_86 )
#define SYSMEM_DSP_ADDR			0x0
#define SYSMEM_DSP_SIZE			0x00400000

#define SYSMEM_CPU1_ADDR		0x00400000
#define SYSMEM_CPU1_SIZE		0x11C00000

#define SYSMEM_CPU0_ADDR		0x12000000
#define SYSMEM_CPU0_SIZE		0x08A00000

#define SYSMEM_SHM_ADDR			0x1AA00000
#define SYSMEM_SHM_SIZE			0x05600000
#elif defined( SYSMEM_SETTING_160_288_64 )
#define SYSMEM_DSP_ADDR			0x0
#define SYSMEM_DSP_SIZE			0x00400000

#define SYSMEM_CPU1_ADDR		0x00400000
#define SYSMEM_CPU1_SIZE		0x11C00000

#define SYSMEM_CPU0_ADDR		0x12000000
#define SYSMEM_CPU0_SIZE		0x0A000000

#define SYSMEM_SHM_ADDR			0x1C000000
#define SYSMEM_SHM_SIZE			0x04000000
#elif defined( SYSMEM_SETTING_44_200_12 )
#define SYSMEM_DSP_ADDR			0x0
#define SYSMEM_DSP_SIZE			0x00400000

#define SYSMEM_CPU1_ADDR		0x00400000
#define SYSMEM_CPU1_SIZE		0x0C400000

#define SYSMEM_CPU0_ADDR		0x0C800000
#define SYSMEM_CPU0_SIZE		0x02C00000

#define SYSMEM_SHM_ADDR			0x0F400000
#define SYSMEM_SHM_SIZE			0x00C00000
#elif defined( SYSMEM_SETTING_56_60_140 )
#define SYSMEM_DSP_ADDR			0x0
#define SYSMEM_DSP_SIZE			0x00400000

#define SYSMEM_CPU1_ADDR		0x00400000
#define SYSMEM_CPU1_SIZE		0x03800000

#define SYSMEM_CPU0_ADDR		0x03c00000
#define SYSMEM_CPU0_SIZE		0x03800000

#define SYSMEM_SHM_ADDR			0x07400000
#define SYSMEM_SHM_SIZE			0x08c00000
#elif defined( SYSMEM_SETTING_248_68_196 )
#define SYSMEM_DSP_ADDR                 0x0
#define SYSMEM_DSP_SIZE                 0x00400000

#define SYSMEM_CPU1_ADDR                0x00400000
#define SYSMEM_CPU1_SIZE                0x04000000

#define SYSMEM_CPU0_ADDR                0x04400000
#define SYSMEM_CPU0_SIZE                0x0F800000

#define SYSMEM_SHM_ADDR                 0x13C00000
#define SYSMEM_SHM_SIZE                 0x0C400000
#elif defined( SYSMEM_SETTING_138_68_306 )
#define SYSMEM_DSP_ADDR                 0x0
#define SYSMEM_DSP_SIZE                 0x00400000

#define SYSMEM_CPU1_ADDR                0x00400000
#define SYSMEM_CPU1_SIZE                0x04000000

#define SYSMEM_CPU0_ADDR                0x04400000
#define SYSMEM_CPU0_SIZE                0x08A00000

#define SYSMEM_SHM_ADDR                 0x0CE00000
#define SYSMEM_SHM_SIZE                 0x13200000
#else
#error Memory setting not recognized!!!
#endif

/*
 * Memory map overlap check
 */




/*
 * just copy it from Galois_memmap.h
 */
#define        MEMMAP_DRAM_CACHE_BASE                      0x0
#define        MEMMAP_DRAM_CACHE_SIZE                      0x40000000
#define        MEMMAP_DRAM_UNCACHE_BASE                    0x40000000
#define        MEMMAP_DRAM_UNCACHE_SIZE                    0x40000000

#define GALOIS_DSP_MEM_ADDR_CACHE	( SYSMEM_DSP_ADDR | MEMMAP_DRAM_CACHE_BASE )
#define GALOIS_DSP_MEM_ADDR_UNCACHE	( SYSMEM_DSP_ADDR | MEMMAP_DRAM_UNCACHE_BASE )
#define GALOIS_DSP_MEM_SIZE		( SYSMEM_DSP_SIZE )

#define GALOIS_CPU0_MEM_ADDR_CACHE	( SYSMEM_CPU0_ADDR | MEMMAP_DRAM_CACHE_BASE )
#define GALOIS_CPU0_MEM_ADDR_UNCACHE	( SYSMEM_CPU0_ADDR | MEMMAP_DRAM_UNCACHE_BASE )
#define GALOIS_CPU0_MEM_SIZE		( SYSMEM_CPU0_SIZE )

#define GALOIS_CPU1_MEM_ADDR_CACHE	( SYSMEM_CPU1_ADDR | MEMMAP_DRAM_CACHE_BASE )
#define GALOIS_CPU1_MEM_ADDR_UNCACHE	( SYSMEM_CPU1_ADDR | MEMMAP_DRAM_UNCACHE_BASE )
#define GALOIS_CPU1_MEM_SIZE		( SYSMEM_CPU1_SIZE )

#define GALOIS_SHM_MEM_ADDR_CACHE	( SYSMEM_SHM_ADDR | MEMMAP_DRAM_CACHE_BASE )
#define GALOIS_SHM_MEM_ADDR_UNCACHE	( SYSMEM_SHM_ADDR | MEMMAP_DRAM_UNCACHE_BASE )
#define GALOIS_SHM_MEM_SIZE		( SYSMEM_SHM_SIZE )

/*
 *  Cacheable and uncacheable memory address conversion macro
 */
#define CACHE_ADDR_MASK			( MEMMAP_DRAM_UNCACHE_BASE - 1 )
#define NON_CACHE_ADDR( x )		( (void*)( ( (unsigned int)(x) & CACHE_ADDR_MASK ) | MEMMAP_DRAM_UNCACHE_BASE ) )
#define CACHE_ADDR( x )			( (void*)( (unsigned int)(x) & CACHE_ADDR_MASK ) )

/*
 * Customization data in memory, which is shared between bootloader and CPU1
 */
#define CUSTOMER_CONFIG_DATA_ENTRY_ADDR ( GALOIS_SHM_MEM_ADDR_UNCACHE + 0x200000 )
#define CUSTOMER_CONFIG_DATA_ENTRY_OFF  ( 0x200000 )
#define	CUSTOMER_SATA_DATA_ENTRY_ADDR   ( GALOIS_SHM_MEM_ADDR_UNCACHE + 0x200000 + (20<<20) )
#define	CUSTOMER_SATA_DATA_ENTRY_OFF    ( 0x200000 + (20<<20) )

/*
 * Compliant with the old naming
 */
#define GALOIS_CPU0_CACHE_START		GALOIS_CPU0_MEM_ADDR_CACHE
#define GALOIS_CPU0_CACHE_SIZE		GALOIS_CPU0_MEM_SIZE
#define GALOIS_CPU1_CACHE_START		GALOIS_CPU1_MEM_ADDR_CACHE
#define GALOIS_CPU1_CACHE_SIZE		GALOIS_CPU1_MEM_SIZE
#define GALOIS_NON_CACHE_START		GALOIS_SHM_MEM_ADDR_UNCACHE
#define GALOIS_NON_CACHE_SIZE		GALOIS_SHM_MEM_SIZE
#define GALOIS_CACHE_ADDR_END		GALOIS_SHM_MEM_ADDR_CACHE

#endif	// #ifndef _SYS_MEMMAP_H_
