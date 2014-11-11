/*******************************************************************************
*******************************************************************************/


#include "galois_cache_flush.h"

#define DCACHE_SIZE	0x8000 // 32kB
void CleanEntireDCache(void)
{
#ifndef __ARM_ASM__
#if 0
	__asm__ __volatile__(
		"\ntest_clean_loop:      \nMRC p15, 0, r1, c7, c10, 3\nBNE test_clean_loop\n"
		"mov r1, #0"	"\n\t"
		"mcr p15, 0, r1, c7, c10, 4\n\t"	/* drain WB */
			: /* output register */
			: /* input operand */ 
			: "r1"/* clobbered registers */);
#else
	__asm__ __volatile__(
		"mov r1, #0"	"\n\t"
		"mcr p15, 0, r1, c7, c10, 0"	"\n\t"
		"mcr p15, 0, r1, c7, c10, 4"	"\n\t"	/* drain WB */
			: /* output register */
			: /* input operand */ 
			: "r1" /* clobbered registers */);
#endif
#else
#if 0
	__asm {
test_clean_loop:
		MRC p15, 0, r1, c7, c10, 3
		BNE test_clean_loop
		mov	r1, #0
		mcr p15, 0, r1, c7, c10, 4			/* drain WB */
	}
#else
	__asm {
		mov	r1, #0
		mcr p15, 0, r1, c7, c10, 0
		mcr p15, 0, r1, c7, c10, 4			/* drain WB */
	}
#endif
#endif
}
void CleanAndInvalidateEntireDCache(void)
{
#ifndef __ARM_ASM__
#if 0
	__asm__ __volatile__(
		"\ntest_clean_invalidate_loop:      \nMRC p15, 0, r1, c7, c14, 3\nBNE test_clean_invalidate_loop\n"
		"mov r1, #0" "\n\t"
		"mcr p15, 0, r1, c7, c10, 4" "\n\t"	/* drain WB */
			: /* output register */
			: /* input operand */ 
			: "r1"/* clobbered registers */);
#else
	__asm__ __volatile__(
		"mov r1, #0"	"\n\t"
		"mcr p15, 0, r1, c7, c14, 0"	"\n\t"
		"mcr p15, 0, r1, c7, c10, 4"	"\n\t"	/* drain WB */
			: /* output register */
			: /* input operand */ 
			: "r1" /* clobbered registers */);
#endif
#else
#if 0
	__asm {
test_clean_invalidate_loop:
		MRC p15, 0, r1, c7, c14, 3
		BNE test_clean_invalidate_loop
		mov	r1, #0
		mcr p15, 0, r1, c7, c10, 4			/* drain WB */
	}
#else
	__asm {
		mov	r1, #0
		mcr p15, 0, r1, c7, c14, 0
		mcr p15, 0, r1, c7, c10, 4			/* drain WB */
	}
#endif
#endif
}
void InvalidateDCacheRegion(int *adr, int size)
{
#ifndef __ARM_ASM__
#if 0
		CLINE   EQU  5 // cache line 32 bytes.
#endif
	__asm__ __volatile__(
		"\nMOV     r2, #0\n"
		"\nMCR     p15, 0, r2, c7, c10, 4  /* drain WB */\n"
		"\nADD     r1, r0, r1              /* get the end of region*/\n"
		"\nBIC     r0, r0, #(1<<5)-1       /* clip 2 cline adr*/\n"
		"\ntest_invalidate_Region_loop:\n"
		"\nMCR     p15, 0, r0, c7, c6, 1   /* invalidate Dcline@adr*/\n"
		"\nADD     r0, r0, #1<<5           /* +1 next cline adr*/\n"
		"\nCMP     r0, r1                  /* check whether reach the end*/\n"
		"\nBLO     test_invalidate_Region_loop         /* invalidate # lines +1*/\n"
			: /* output register */
			: /* input operand */ 
			: "r0","r1","r2"/* clobbered registers */);
#else
	__asm {
		MOV		r0, adr
		MOV		r1, size
		MOV	    r2, #0
		MCR     p15, 0, r2, c7, c10, 4  // drain WB
		ADD     r1, r0, r1              // get the end of region
		BIC     r0, r0, #(1<<5)-1       // clip 2 cline adr
test_invalidate_Region_loop:
		MCR     p15, 0, r0, c7, c6, 1   // invalidate Dcline@adr
		ADD     r0, r0, #1<<5           // +1 next cline adr
		CMP     r0, r1                  // check whether reach the end
		BLO     test_invalidate_Region_loop         // invalidate # lines +1
	}
#endif
}
void CleanDCacheRegion(int *adr, int size)
{
	if (size >= DCACHE_SIZE) { // clean entire DCache maybe faster
#ifndef __ARM_ASM__
		__asm__ __volatile__(
			"mov r1, #0"	"\n\t"
			"mcr p15, 0, r1, c7, c10, 0"	"\n\t"
			"mcr p15, 0, r1, c7, c10, 4"	"\n\t"	/* drain WB */
				: /* output register */
				: /* input operand */ 
				: "r1" /* clobbered registers */);
#else
		__asm {
			MOV		r1, size
			mov	r1, #0
			mcr p15, 0, r1, c7, c10, 0
			mcr p15, 0, r1, c7, c10, 4	/* drain WB */
		}
#endif
	} else {
#ifndef __ARM_ASM__
#if 0
		CLINE   EQU  5 // cache line 32 bytes.
#endif
		__asm__ __volatile__(
			"\nADD     r1, r0, r1              /* get the end of region*/\n"
			"\nBIC     r0, r0, #(1<<5)-1       /* clip 2 cline adr*/\n"
			"\ntest_clean_Region_loop:\n"
			"\nMCR     p15, 0, r0, c7, c10, 1  /* clean Dcline@adr*/\n"
			"\nADD     r0, r0, #1<<5           /* +1 next cline adr*/\n"
			"\nCMP     r0, r1                  /* check whether reach the end*/\n"
			"\nBLO     test_clean_Region_loop  /* flush # lines +1*/\n"
			"\nMOV     r2, #0\n"
			"\nMCR     p15, 0, r2, c7, c10, 4  /* drain WB */\n"

				: /* output register */
				: /* input operand */ 
				: "r0","r1","r2"/* clobbered registers */);
#else
		__asm {
			MOV		r0, adr
			MOV		r1, size
			ADD     r1, r0, r1              // get the end of region
			BIC     r0, r0, #(1<<5)-1       // clip 2 cline adr
test_clean_Region_loop:
			MCR     p15, 0, r0, c7, c10, 1  // clean Dcline@adr
			ADD     r0, r0, #1<<5           // +1 next cline adr
			CMP     r0, r1                  // check whether reach the end
			BLO     test_clean_Region_loop  // flush # lines +1
			MOV	    r2, #0
			MCR     p15, 0, r2, c7, c10, 4  // drain WB
		}
#endif
	}
}
void CleanAndInvalidateDCacheRegion(int *adr, int size)
{
	if (size >= DCACHE_SIZE) {
#ifndef __ARM_ASM__
		__asm__ __volatile__(
			"mov r1, #0"	"\n\t"
			"mcr p15, 0, r1, c7, c14, 0"	"\n\t"
			"mcr p15, 0, r1, c7, c10, 4"	"\n\t"	/* drain WB */
				: /* output register */
				: /* input operand */
				: "r1" /* clobbered registers */);
#else
		__asm {
			mov	r1, #0
			mcr p15, 0, r1, c7, c14, 0
			mcr p15, 0, r1, c7, c10, 4	/* drain WB */
		}
#endif
	} else {
#ifndef __ARM_ASM__
#if 0
		CLINE   EQU  5 // cache line 32 bytes.
#endif
		__asm__ __volatile__(
			"\nADD     r1, r0, r1              /* get the end of region*/\n"
			"\nBIC     r0, r0, #(1<<5)-1 	   /* clip 2 cline adr*/ \n"
			"\ntest_clean_invalidate_Region_loop:\n"
			"\nMCR     p15, 0, r0, c7, c14, 1  /* clean and invalidate Dcline@adr*/\n"
			"\nADD     r0, r0, #1<<5           /* +1 next cline adr*/\n"
			"\nCMP     r0, r1                  /* check whether reach the end*/\n"
			"\nBLO     test_clean_invalidate_Region_loop         /* flush # lines +1*/\n"
			"\nMOV     r2, #0"	"\n"
			"\nMCR     p15, 0, r2, c7, c10, 4  /* drain WB */\n"
				: /* output register */
				: /* input operand */ 
				: "r0","r1","r2"/* clobbered registers */);
#else
		__asm {
			MOV		r0, adr
			MOV		r1, size
			ADD     r1, r0, r1              // get the end of region
			BIC     r0, r0, #(1<<5)-1       // clip 2 cline adr
test_clean_invalidate_Region_loop:
			MCR     p15, 0, r0, c7, c14, 1  // clean and invalidate Dcline@adr
			ADD     r0, r0, #1<<5           // +1 next cline adr
			CMP     r0, r1                  // check whether reach the end
			BLO     test_clean_invalidate_Region_loop         // flush # lines +1
			MOV	    r2, #0
			MCR     p15, 0, r2, c7, c10, 4  // drain WB
		}
#endif
	}
}
