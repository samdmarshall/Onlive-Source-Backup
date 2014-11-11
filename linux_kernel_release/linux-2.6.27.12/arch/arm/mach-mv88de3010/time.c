/* 
 * arch/arm/mach-mv88de3010/time.c
 * modified from arch/arm/plat-orion/time.c
 *
 * Marvell MV88DE3010 timer handling.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 *
 * Timer GALOIS_CLOCKSOURCE(7) is used as free-running clocksource, 
 * while timer LINUX_TIMER(0) is used as clock_event_device.
 * Within clockevents mode, the original timer tick mechanism will 
 * no longer be useed.
 */

#include <linux/kernel.h>
#include <linux/clockchips.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/clocksource.h>
#include <linux/cnt32_to_63.h>
#include <asm/mach/time.h>
#include <mach/galois_platform.h>
#include <mach/galois_generic.h>
#include <mach/timex.h>

#include "Galois_memmap.h"
#include "galois_hal_com.h"
#include "pic_drv.h"
#include "apb_ictl_drv.h"
#include "apb_timer_drv.h"
#include "apb_perf_base.h"

#ifdef CONFIG_MV88DE3010_DYNAMIC_FREQ
#include "mach/freq.h"

SOC_FREQ_VECTOR_Linux socFreqVec;
#endif

/*
 * used to convert timer cycles to ns
 */
static unsigned long cyc2ns_scale = 0;
#define CYC2NS_SCALE_FACTOR 10

static void __init set_cyc2ns_scale(unsigned long timer_hz)
{
	unsigned long timer_khz = timer_hz/1000;
	cyc2ns_scale = (1000000 << CYC2NS_SCALE_FACTOR)/timer_khz;

	/* To clear the top bit of returned by cnt32_to_63().
	 * If it's even, then *cyc2ns_scale means also << 1 too.
	 * So if the LSB is 1, round it up
	 */
	if(cyc2ns_scale & 1)
		cyc2ns_scale++;
}

/* 
 * Old performance timer, kept here for reference.
 * Performance counting timer init/exit functions
 */
void mv88de3010_init_hrtimer(int timer)
{
	/* init performance timer */
	if ((timer > 7) || (timer < 0))
		return;
	TIMERx_SetControlReg(timer, 
			INT_UNMASK_BIT, INT_DISABLE_BIT, TIMER_USR_DEFINED_MODE);
	TIMERx_SetLoadCount(timer, 0xFFFFFFFF);
	TIMERx_SetMask(timer, INT_MASK_BIT);	/* mask interrupt */
	TIMERx_SetIntEn(timer, INT_ENABLE_BIT);	/* enable counter */

	return;
}

void mv88de3010_exit_hrtimer(int timer)
{
	if ((timer > 7) || (timer < 0))
		return;
	TIMERx_SetControlReg(timer, 
			INT_MASK_BIT, INT_DISABLE_BIT, TIMER_USR_DEFINED_MODE);
	return;
}

/*
 * GALOIS_CLOCKSOURCE is timer 7, reserved for Linux.
 * timer clock frequency is GALOIS_TIMER_CLK Hz
 */

unsigned long long sched_clock(void)
{
  	unsigned long long v = (unsigned long long)(cnt32_to_63(0xFFFFFFFFUL - TIMERx_GetCurrentValue(GALOIS_CLOCKSOURCE)));
	v *= cyc2ns_scale;
    v >>= CYC2NS_SCALE_FACTOR;
    return v;
}

/*
 * Number of timer ticks per jiffy.
 */
static unsigned long timer_load;

#ifdef CONFIG_GENERIC_TIME

/*
 * Clocksource handling.
 */
static cycle_t mv88de3010_clksrc_read(void)
{
    return (cycle_t) (0xffffffff - TIMERx_GetCurrentValue(GALOIS_CLOCKSOURCE));
}

static struct clocksource mv88de3010_clksrc = {
	.name		= "mv88de3010_clocksource",
	.shift		= 22,
	.rating		= 300,
	.read		= mv88de3010_clksrc_read,
	.mask		= CLOCKSOURCE_MASK(32),
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
};

#else
static unsigned long mv88de3010_gettimeoffset(void)
{
	unsigned long timer_value1, timer_value2, timer_passed;
	unsigned int status;

	timer_value2 = TIMERx_GetCurrentValue(LINUX_TIMER);
	do {
		timer_value1 = timer_value2;
		status = TIMERx_GetIntStatus(LINUX_TIMER);
		timer_value2 = TIMERx_GetCurrentValue(LINUX_TIMER);
	} while (timer_value2 > timer_value1);

	timer_passed = timer_load - timer_value2;
	if (status)
		timer_passed += timer_load;

	return ((timer_passed * cyc2ns_scale) >> CYC2NS_SCALE_FACTOR);
}
#endif /* CONFIG_GENERIC_TIME */

#ifdef CONFIG_GENERIC_CLOCKEVENTS
#define CLOCK_MIN 	0xf /*Minimum ns value, for clockevents*/
/*
 * Clockevent handling.
 */
static int
mv88de3010_clkevt_next_event(unsigned long delta, struct clock_event_device *dev)
{
	unsigned long flags;

	if (delta <= CLOCK_MIN)
		return -ETIME;

	local_irq_save(flags);
    TIMERx_SetControlReg(LINUX_TIMER, INT_UNMASK_BIT,
             INT_DISABLE_BIT, TIMER_USR_DEFINED_MODE);
    TIMERx_SetLoadCount(LINUX_TIMER, delta);
    TIMERx_SetIntEn(LINUX_TIMER, INT_ENABLE_BIT);   /* Enable counter */

	local_irq_restore(flags);

	return 0;
}

static void
mv88de3010_clkevt_mode(enum clock_event_mode mode, struct clock_event_device *dev)
{
	unsigned long flags;

	local_irq_save(flags);

	switch(mode) {
		case CLOCK_EVT_MODE_PERIODIC:

    		TIMERx_SetControlReg(LINUX_TIMER, INT_UNMASK_BIT,
				 INT_DISABLE_BIT, TIMER_USR_DEFINED_MODE);
#ifdef CONFIG_MV88DE3010_DYNAMIC_FREQ
    		TIMERx_SetLoadCount(LINUX_TIMER, socFreqVec.cfg / HZ);
#else
    		TIMERx_SetLoadCount(LINUX_TIMER, GALOIS_TIMER_CLK / HZ);
#endif
    		TIMERx_SetIntEn(LINUX_TIMER, INT_ENABLE_BIT);   /* Enable counter */
			break;
		case CLOCK_EVT_MODE_ONESHOT:
			printk("No such feature currently.\n");
			break;
		case CLOCK_EVT_MODE_UNUSED:
		case CLOCK_EVT_MODE_SHUTDOWN:
		default:
			TIMERx_SetControlReg(LINUX_TIMER, INT_UNMASK_BIT,
				 INT_DISABLE_BIT, TIMER_USR_DEFINED_MODE);
 			TIMERx_SetIntEn(LINUX_TIMER, INT_DISABLE_BIT); /* Disable counter */
			break;
	}
	local_irq_restore(flags);
}

static struct clock_event_device mv88de3010_clkevt = {
	.name		= "mv88de3010_tick",
	.features	= CLOCK_EVT_FEAT_ONESHOT | CLOCK_EVT_FEAT_PERIODIC,
	.shift		= 32,
	.rating		= 300,
	.cpumask	= CPU_MASK_CPU0,
	.set_next_event	= mv88de3010_clkevt_next_event,
	.set_mode	= mv88de3010_clkevt_mode,
};
#endif /* CONFIG_GENERIC_CLOCKEVENTS */

static irqreturn_t mv88de3010_timer_interrupt(int irq, void *dev_id)
{
    unsigned int ictl_finalsts, ictl_rawsts;

    ictl_finalsts = ICTL_GetIrqFinalStatus(CPUINDEX, 0);
    ictl_rawsts = ICTL_GetIrqRawStatus(CPUINDEX, 0);

    /*
     * ictl_finalsts have been cleared by irq_chip->ack()
     */
    if (ictl_rawsts & (1 << (irq - GALOIS_APB_IRQ_START))) {
#ifdef CONFIG_GENERIC_CLOCKEVENTS
 	   	mv88de3010_clkevt.event_handler(&mv88de3010_clkevt);
#else
		timer_tick();
#endif
        TIMERx_ClearInt(LINUX_TIMER);
        return IRQ_HANDLED;
    } else {
        printk("ERROR: irq=%d, ictl_rawsts=0x%08x\n", irq, ictl_rawsts);
        return IRQ_NONE;
    }
}

static struct irqaction mv88de3010_timer_irq = {
	.name		= "mv88de3010_tick",
	.flags		= IRQF_DISABLED | IRQF_TIMER,
	.handler	= mv88de3010_timer_interrupt
};

void __init mv88de3010_time_init(void)
{
    time_t secs_since_1970 = 1234567890 ; /* 23:31:30 13 Feb 2009 */

#ifndef CONFIG_MV88DE3010_DYNAMIC_FREQ
	set_cyc2ns_scale(GALOIS_TIMER_CLK); /* set timer cyc2ns scaler */
	timer_load = GALOIS_TIMER_CLK / HZ;
#else
	query_board_freq(&socFreqVec);
	mv88de3010_print_version();
	set_cyc2ns_scale(socFreqVec.cfg); /* set timer cyc2ns scaler */
	timer_load = socFreqVec.cfg / HZ;
#endif

	xtime.tv_sec = secs_since_1970;
	xtime.tv_nsec = 0;
	set_normalized_timespec(&wall_to_monotonic, -xtime.tv_sec, -xtime.tv_nsec);
	/* Add a 32bit clocksource from GALOIS_CLOCKSOURCE( timer 7) */
#ifdef CONFIG_GENERIC_TIME
	mv88de3010_clksrc.mult =
#ifdef CONFIG_MV88DE3010_DYNAMIC_FREQ
		clocksource_hz2mult(socFreqVec.cfg, mv88de3010_clksrc.shift);
#else
		clocksource_hz2mult(GALOIS_TIMER_CLK, mv88de3010_clksrc.shift);
#endif
	clocksource_register(&mv88de3010_clksrc);
#endif

	/* If using the clockevents, register the new clock chip support.
	 * Otherwise the old way. Any how the timer interrupt should be opened. */
#ifdef CONFIG_GENERIC_CLOCKEVENTS
#ifdef CONFIG_MV88DE3010_DYNAMIC_FREQ
	mv88de3010_clkevt.mult = clocksource_hz2mult(socFreqVec.cfg, mv88de3010_clkevt.shift);
#else
	mv88de3010_clkevt.mult = clocksource_hz2mult(GALOIS_TIMER_CLK, mv88de3010_clkevt.shift);
#endif
	mv88de3010_clkevt.max_delta_ns = clockevent_delta2ns(0xfffffffe, &mv88de3010_clkevt);
	mv88de3010_clkevt.min_delta_ns = clockevent_delta2ns(CLOCK_MIN, &mv88de3010_clkevt);
	clockevents_register_device(&mv88de3010_clkevt);
#else
	TIMERx_SetControlReg(LINUX_TIMER, INT_UNMASK_BIT, INT_DISABLE_BIT, TIMER_USR_DEFINED_MODE);
	TIMERx_SetLoadCount(LINUX_TIMER, timer_load);
	TIMERx_SetIntEn(LINUX_TIMER, INT_ENABLE_BIT);	/* Enable counter */
#endif /* CONFIG_GENERIC_CLOCKEVENTS */

	/* enable h/w timer interrupt */
	mv88de3010_open_irq(TIMER_IRQ);

	/* register s/w timer irq to linux */
	setup_irq(TIMER_IRQ, &mv88de3010_timer_irq);
}

struct sys_timer mv88de3010_timer = {
	.init = mv88de3010_time_init,
#ifndef CONFIG_GENERIC_TIME
	.offset = mv88de3010_gettimeoffset,
#endif
};

EXPORT_SYMBOL(mv88de3010_init_hrtimer);
EXPORT_SYMBOL(mv88de3010_exit_hrtimer);
EXPORT_SYMBOL(sched_clock);
