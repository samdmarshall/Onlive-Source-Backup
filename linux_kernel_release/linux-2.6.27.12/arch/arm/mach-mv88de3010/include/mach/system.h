/*
 * Modified by OnLive, Inc. Oct 2010.
 */
#include <mach/hardware.h>
#include <mach/io.h>

static inline void arch_idle(void)
{
	cpu_do_idle();
}

extern void arch_reset(char mode);
