/*
 * linux/include/asm-arm/arch-mv88de3010/uncompress.h
 * Author: Yufeng Zheng (yfzheng@marvell.com)
 */
#include <linux/compiler.h>
//#include "apb_perf_base.h"

#ifdef CONFIG_MV88DE3010_BERLIN
#define _MEMMAP_SM_REG_BASE		0xF7F80000
#define _SM_APB_REG_BASE			(_MEMMAP_SM_REG_BASE + 0x40000)
#define _SM_APB_UART0_BASE		(_SM_APB_REG_BASE + 0x9000)
#define _SM_APB_UART1_BASE		(_SM_APB_REG_BASE + 0xA000)
#define _SM_APB_UART2_BASE		(_SM_APB_REG_BASE + 0xB000)
#define _APB_UART_INST0_BASE		(_SM_APB_UART0_BASE)
#define _APB_UART_INST1_BASE		(_SM_APB_UART1_BASE)
#define _APB_UART_INST2_BASE		(_SM_APB_UART2_BASE)
#else /* BERLIN */
#define _MEMMAP_APBPERIF_REG_BASE 0xF7F30000
#define _APB_UART_INST0_BASE		(_MEMMAP_APBPERIF_REG_BASE + 0x1000)
#define _APB_UART_INST1_BASE		(_MEMMAP_APBPERIF_REG_BASE + 0x1C00)
#endif /* BERLIN */

#define _APB_UART_BASE	_APB_UART_INST0_BASE

#define GALOIS_REG_WORD32_READ(addr)		\
		(*((volatile unsigned int *)(addr)))
#define GALOIS_REG_WORD32_WRITE(addr, data)	\
		((*((volatile unsigned int *)(addr))) = ((unsigned int)(data)))

#define APB_UART_LCR	0x0C
#define APB_UART_LSR	0x14
#define APB_UART_THR	0x00

static inline void putc(char c)
{
	unsigned int value;
	unsigned int uart_base = _APB_UART_BASE;

	/*
	 * LCR[7]=1, known as DLAB bit, is used to enable rw DLL/DLH registers.
	 * it must be cleared(LCR[7]=0) to allow accessing other registers.
	 */
	value = GALOIS_REG_WORD32_READ(uart_base + APB_UART_LCR);
	GALOIS_REG_WORD32_WRITE(uart_base + APB_UART_LCR, value & ~(0x00000080));

	while (!(GALOIS_REG_WORD32_READ(uart_base + APB_UART_LSR) & (0x00000020)))
		barrier();

	GALOIS_REG_WORD32_WRITE(uart_base + APB_UART_THR, c);
}

static inline void flush(void)
{
}

/*
 * If nothing to do here, just use this:
 * #define arch_decomp_setup()
 *
 * else, because codes before decompress_kernel() calls "bl cache_on"
 * to enable MMU and caches, so here we may disable them to keep DRAM coherent.
 */
static __inline__ void arch_decomp_setup(void)
{
#if 0
	asm volatile(
			/*
			 * flush v4 I/D caches
			 */
			"mov	r1, #0x0\n"
			"mcr	p15, 0, r1, c7, c7, 0\n"	/* flush v3/v4 cache */
			"mcr	p15, 0, r1, c8, c7, 0\n"	/* flush v4 TLB */

			/*
			 * disable MMU stuff and D-cache [I-cache enable]
			 */
			"mrc	p15, 0, r1, c1, c0, 0\n"
			"bic	r1, r1, #0x00002300\n"	/* clear bits 13, 9:8 (--V- --RS) */
			"bic	r1, r1, #0x00000087\n"	/* clear bits 7, 2:0 (B--- -CAM) */
			"orr	r1, r1, #0x00001000\n"	/* set bit 12 (I) I-Cache:enable */
			"orr	r1, r1, #0x00000002\n"	/* set bit 2 (A) Align */
			"mcr	p15, 0, r1, c1, c0, 0\n"
			);
#endif
}

#define arch_decomp_wdog()
