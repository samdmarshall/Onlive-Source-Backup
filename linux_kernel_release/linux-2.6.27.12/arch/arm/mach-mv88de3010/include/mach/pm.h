/*
 * MV88DE3010 Power Management Routines
 *
 * Copyright (c) 2010 Marvell
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License.
 */
#ifndef MV88DE3010_PM_H
#define MV88DE3010_PM_H

#include <asm/types.h>

/* mode save flags */
#define PM_MODE_SAVE_FLAG_SYS	0x1
#define PM_MODE_SAVE_FLAG_IRQ	0x2
#define PM_MODE_SAVE_FLAG_FIQ	0x4
#define PM_MODE_SAVE_FLAG_ABT	0x8
#define PM_MODE_SAVE_FLAG_UND	0x10
#define PM_MODE_SAVE_FLAG_SVC	0x20
#define PM_MODE_SAVE_FLAG_ALL	0x3f

/* CPSR Processor constants */
#define CPSR_Mode_MASK		(0x0000001F)
#define CPSR_Mode_USR		(0x10)
#define CPSR_Mode_FIQ		(0x11)
#define CPSR_Mode_IRQ		(0x12)
#define CPSR_Mode_SVC		(0x13)
#define CPSR_Mode_ABT		(0x17)
#define CPSR_Mode_UND		(0x1B)
#define CPSR_Mode_SYS		(0x1F)
#define CPSR_I_Bit		(0x80)
#define CPSR_F_Bit		(0x40)

#define WORD_SIZE 4

/* the position of each data memeber */
#define SleepState_begin		0x0
#define SleepState_checksum		0x0
#define SleepState_wordCount		(SleepState_checksum + WORD_SIZE)
#define SleepState_areaAddress		(SleepState_wordCount + WORD_SIZE)
#define SleepState_modeSaveFlags	(SleepState_areaAddress + WORD_SIZE)

/* save ARM registers */
#define SleepState_ENTRY_REGS		(SleepState_modeSaveFlags + WORD_SIZE)
#define SleepState_ENTRY_CPSR		(SleepState_ENTRY_REGS)
#define SleepState_ENTRY_SPSR		(SleepState_ENTRY_CPSR + WORD_SIZE)
#define SleepState_ENTRY_R0		(SleepState_ENTRY_SPSR + WORD_SIZE)
#define SleepState_ENTRY_R1		(SleepState_ENTRY_R0 + WORD_SIZE)
#define SleepState_SYS_REGS		(SleepState_ENTRY_REGS + 17*WORD_SIZE)
#define SleepState_FIQ_REGS		(SleepState_SYS_REGS + 2*WORD_SIZE)
#define SleepState_IRQ_REGS		(SleepState_FIQ_REGS + 8*WORD_SIZE)
#define SleepState_ABT_REGS		(SleepState_IRQ_REGS + 3*WORD_SIZE)
#define SleepState_UND_REGS		(SleepState_ABT_REGS + 3*WORD_SIZE)
#define SleepState_SVC_REGS		(SleepState_UND_REGS + 3*WORD_SIZE)

/* save MMU settings */
#define SleepState_Cp15_ACR_MMU		(SleepState_SVC_REGS + 3*WORD_SIZE)
#define SleepState_Cp15_AUXCR_MMU	(SleepState_Cp15_ACR_MMU + WORD_SIZE)
#define SleepState_Cp15_TTBR_MMU	(SleepState_Cp15_AUXCR_MMU + WORD_SIZE)
#define SleepState_Cp15_DACR_MMU	(SleepState_Cp15_TTBR_MMU + WORD_SIZE)
#define SleepState_Cp15_PID_MMU		(SleepState_Cp15_DACR_MMU + WORD_SIZE)
#define SleepState_Cp15_CPAR		(SleepState_Cp15_PID_MMU + WORD_SIZE)

#define SleepState_extendedChecksumByteCount	\
	(SleepState_Cp15_CPAR + WORD_SIZE)
#define SleepState_flushFunc		\
	(SleepState_extendedChecksumByteCount + WORD_SIZE)
#define SleepState_standbyFunc		\
	(SleepState_flushFunc + WORD_SIZE)
#define SleepState_end			(SleepState_standbyFunc + WORD_SIZE)
#define SleepState_size			(SleepState_end - SleepState_begin)

#ifndef __ASSEMBLY__

#ifdef __KERNEL__

struct pm_save_data {
	u32 checksum;
	u32 wordCount;
	u32 areaAddress;
	u32 modeSaveFlags;
	/* current mode registers cpsr, sprsr, r0-r12, lr, sp */
	u32 ENTRY_REGS[17];
	/* SYS mode registers:sp, lr */
	u32 SYS_REGS[2];
	/* FIQ mode registers:spsr, r8-r12, sp, lr */
	u32 FIQ_REGS[8];
	/* IRQ mode registers:spsr, sp, lr */
	u32 IRQ_REGS[3];
	/* ABT mode registers:spsr, sp, lr */
	u32 ABT_REGS[3];
	/* UND mode registers:spsr, sp, lr */
	u32 UND_REGS[3];
	/* SVC mode registers:spsr, sp, lr */
	u32 SVC_REGS[3];
	/* MMU registers */
	u32 CP15_ACR_MMU;
	u32 CP15_AUXCR_MMU;
	u32 CP15_TTBR_MMU;
	u32 CP15_DACR_MMU;
	u32 CP15_PID_MMU;
	u32 CP15_CPAR;

	u32 extendedChecksumByteCount;
	void (*flushFunc)(void);
	void (*standbyFunc)(void);
};

struct mv88de3010_pm_regs {
	/* It's used to save core registers. */
	struct pm_save_data	pm_data;

#if 0
	struct mfp_regs		mfp;
	struct gpio_regs	gpio;
	struct intc_regs	intc;
	struct apmu_regs	apmu;
	struct mpmu_regs	mpmu;
	struct apbclk_regs	apbclk;
	struct ciu_regs		ciu;
	struct squ_regs		squ;
	struct axifab_regs	axifab;
	struct smc_regs		smc;
#endif

	/* It's the address of DDR that stores key information.
	 */
	unsigned int word0;
};

extern void mv88de3010_cpu_sleep(unsigned int, unsigned int);
extern void mv88de3010_cpu_resume(void);
#endif
#endif

#endif
