/*
 * Just any arbitrary offset to the start of the vmalloc VM area: the
 * current 8MB value just means that there will be a 8MB "hole" after the
 * physical memory until the kernel virtual memory starts.  That means that
 * any out-of-bounds memory accesses will hopefully be caught.
 * The vmalloc() routines leaves a hole of 4kB between each vmalloced
 * area for the same reason. ;)
 */

#define VMALLOC_OFFSET	  (8*1024*1024)

/*
 * high_memory shall be physical-memory-end plus PAGE_OFFSET,
 * high_memory can be changed if there exists vmalloc region overlap. 
 * please check check_membank_valid in arch/arm/mm/mmu.c -Qingchao 07/30
 *
 * Update: in Berlin, the memory should be less than 256MB. -Yufeng 04/27
 */
#define VMALLOC_START	  (((unsigned long)high_memory + VMALLOC_OFFSET) & ~(VMALLOC_OFFSET-1))

/* 
 * pay notice: we have up to 512M of Dram and Vmalloc area together,
 * the dram comes on the expense of the vmalloc.
 */
#include <linux/const.h>
#define UL(x)	_AC(x, UL)

/*
 * Update: vmalloc size should be greater than vmalloc_reserve (
 * which is 128M by default, defined in arch/arm/kernel/setup.c).
 * and must be 2M aligned.
 *
 * if CONFIG_MV88DE3010_CPU0MEM_SIZE / 2 < 0x08000000 (128M)
 * VMALLOC_END = PAGE_OFFSET + 
 * 		 CONFIG_MV88DE3010_CPU0MEM_SIZE + 
 * 		 VMALLOC_OFFSET + 0x08000000
 * else 
 * VMALLOC_END = PAGE_OFFSET + 
 * 		 CONFIG_MV88DE3010_CPU0MEM_SIZE + 
 * 		 VMALLOC_OFFSET + 
 * 		 CONFIG_MV88DE3010_CPU0MEM_SIZE / 2
 * -Qingchao 07/30
 *
 * For Single CPU system, Shared memory requires 0x30000000 = 768MB, and have to use 2G:2G split
 * -Yufeng 09/11
 * For Single CPU system (both CPU0 and CPU1), Shared memory requires 2 X CONFIG_MV88DE3010_SHMMEM_SIZE, and have to use 2G:2G split
 * For prevent allocation failed: out of vmalloc space, need a padding which 32M (16M + 16M) is default.
 * -Fang Bao 09/21
 */
#if defined(CONFIG_VMSPLIT_2G)
#define VMALLOC_END	(PAGE_OFFSET + UL(CONFIG_MV88DE3010_CPU0MEM_SIZE) + VMALLOC_OFFSET + UL(CONFIG_MV88DE3010_SHMMEM_SIZE) + UL(CONFIG_MV88DE3010_SHMMEM_SIZE) + 0x02000000)
#else
#if ((UL(CONFIG_MV88DE3010_CPU0MEM_SIZE) >> 1) < 0x08000000)
#define VMALLOC_END	(PAGE_OFFSET + UL(CONFIG_MV88DE3010_CPU0MEM_SIZE) + VMALLOC_OFFSET + 0x08000000)
#else
#define VMALLOC_END	(PAGE_OFFSET + UL(CONFIG_MV88DE3010_CPU0MEM_SIZE) + VMALLOC_OFFSET + (UL(CONFIG_MV88DE3010_CPU0MEM_SIZE) >> 1))
#endif
#endif

/*
 * VMALLOC_END shall be less than shared memory virtual address
 * - Qingchao 07/30
 */
#if (VMALLOC_END > UL(CONFIG_MV88DE3010_SHMMEM_VIRTSTART))
#error "VMALLOC_END shall not be greater than shared memory virtual address"
#endif

/* 
 * VMALLOC_END should be less than desc_map's virtual address(0xF6000000)
 * FIXME: use macro instead of 0xF6000000?
 */
#if (VMALLOC_END > UL(0xF6000000))
#error "VMALLOC_END shall not be greater than desc_map's virtual address (0xF6000000)"
#endif
