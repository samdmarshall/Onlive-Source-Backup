/**
 * @file op_model_mv88de3010.c
 * MV88DE3010 Performance Monitor Driver
 *
 */

#include <linux/types.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/oprofile.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <asm/delay.h>
#include <mach/galois_platform.h>
#include <mach/galois_generic.h>

#include <asm/system.h>
#include <mach/irqs.h>

#include "op_counter.h"
#include "op_arm_model.h"
#include "galois_hal_com.h"
#include "apb_timer_drv.h"

#define	PMU_ENABLE	0x001	/* Enable event counters */
#define PMN_RESET	0x002	/* Reset event counters */

/*We use APB timer 4 for data collection*/
#define MV88DE3010_PMUTIMER 4
#define MV88DE3010_PMUTIMER_IRQ (G_IRQ_APB_LSbTimerInst1_0+MV88DE3010_PMUTIMER)

#define EVT_UNUSED 	0xFF

struct pmu_counter {
	unsigned long samples;
	unsigned long reset_counter;
};

enum { 
       PMN0,
       PMN1,
       PMN2, 
       PMN3,
       MAX_COUNTERS 
};

static struct pmu_counter results[MAX_COUNTERS];

struct pmu_type {
	char *name;
	int num_counters;
	unsigned int int_enable;
	unsigned int int_mask[MAX_COUNTERS];
};

static struct pmu_type pmu_parms = {
	.name		= "arm/mv88de3010",
	.num_counters	= MAX_COUNTERS,
	.int_mask	= { [PMN0] = 0x01, [PMN1] = 0x02, 
			    [PMN2] = 0x04, [PMN3] = 0x08 },
};

static struct pmu_type *pmu;

static void write_pmnc(u32 pmnc_id, u32 val)
{
	switch (pmnc_id) {
	case PMN0:
		__asm__ __volatile__ ("mcr p15, 0, %0, c15, c12, 0" : : "r" (val));
		break;
	case PMN1:
		__asm__ __volatile__ ("mcr p15, 0, %0, c15, c12, 1" : : "r" (val));
		break;
	case PMN2:
		__asm__ __volatile__ ("mcr p15, 0, %0, c15, c12, 2" : : "r" (val));
		break;
	case PMN3:
		__asm__ __volatile__ ("mcr p15, 0, %0, c15, c12, 3" : : "r" (val));
		break;
	default:
		printk("write_pmnc:Invalid PMU counter number\n");
	}
}

static u32 read_pmnc(u32 pmnc_id)
{
	u32 val = 0;

	switch (pmnc_id) {
	case PMN0:
		__asm__ __volatile__ ("mrc p15, 0, %0, c15, c12, 0" : "=r" (val));
		break;
	case PMN1:
		__asm__ __volatile__ ("mrc p15, 0, %0, c15, c12, 1" : "=r" (val));
		break;
	case PMN2:
		__asm__ __volatile__ ("mrc p15, 0, %0, c15, c12, 2" : "=r" (val));
		break;
	case PMN3:
		__asm__ __volatile__ ("mrc p15, 0, %0, c15, c12, 3" : "=r" (val));
		break;
	default:
		printk("read_pmnc:Invalid PMU counter number\n");
	}

	return val;
}


static u32 __de3010_read_counter(int counter)
{
	u32 lo=0;

	switch (counter) {
	case PMN0:
		__asm__ __volatile__ ("mrc p15, 0, %0, c15, c13, 0" : "=r" (lo));
		/*__asm__ __volatile__ ("mrc p15, 0, %0, c15, c13, 1" : "=r" (hi));*/
		break;
	case PMN1:
		__asm__ __volatile__ ("mrc p15, 0, %0, c15, c13, 2" : "=r" (lo));
		/*__asm__ __volatile__ ("mrc p15, 0, %0, c15, c13, 3" : "=r" (hi));*/
		break;
	case PMN2:
		__asm__ __volatile__ ("mrc p15, 0, %0, c15, c13, 4" : "=r" (lo));
		/*__asm__ __volatile__ ("mrc p15, 0, %0, c15, c13, 5" : "=r" (hi));*/
		break;
	case PMN3:
		__asm__ __volatile__ ("mrc p15, 0, %0, c15, c13, 6" : "=r" (lo));
		/*__asm__ __volatile__ ("mrc p15, 0, %0, c15, c13, 7" : "=r" (hi));*/
		break;
	default:
		printk("__de3010_read_counter:Invalid PMU counter number\n");
	}

	return lo;
}

static u32 read_counter(int counter)
{
	return ( __de3010_read_counter(counter));
}

static void __de3010_write_counter(int counter, u32 val)
{
	switch (counter) {
	case PMN0:
		__asm__ __volatile__ ("mcr p15, 0, %0, c15, c13, 0" : : "r" (val));
		break;
	case PMN1:
		__asm__ __volatile__ ("mcr p15, 0, %0, c15, c13, 2" : : "r" (val));
		break;
	case PMN2:
		__asm__ __volatile__ ("mcr p15, 0, %0, c15, c13, 4" : : "r" (val));
		break;
	case PMN3:
		__asm__ __volatile__ ("mcr p15, 0, %0, c15, c13, 6" : : "r" (val));
		break;
	default:
		printk("__de3010_write_counter:Invalid PMU counter number\n");
	}
}

static void write_counter(int counter, u32 val)
{
	__de3010_write_counter(counter, val);
}

static void inline __de3010_enable_counter(int counter)
{
	u32 pmnc;

	pmnc = read_pmnc(counter);
	pmnc |= PMU_ENABLE;
	printk("PMU: Enable counter %d with event %x\n", counter, pmnc);
	write_pmnc(counter, pmnc);
}

static void inline __de3010_disable_counter(int counter)
{
	u32 pmnc;

	pmnc = read_pmnc(counter);
	pmnc &= ~PMU_ENABLE;
	write_pmnc(counter, pmnc);
}

static int mv88de3010_setup_ctrs(void)
{
	int i;

	for (i = PMN0; i < MAX_COUNTERS; i++) {
		if (counter_config[i].enabled)
			continue;

		counter_config[i].event = EVT_UNUSED;
	}

	for (i = PMN0; i < MAX_COUNTERS; i++) {
		if (counter_config[i].event == EVT_UNUSED) {
			counter_config[i].event = 0;
			pmu->int_enable &= ~pmu->int_mask[i];
			continue;
		}

		printk("PMU: Setup counter %d with event %x\n", i, (u32)counter_config[i].event);

		/*enable the event counting*/
		write_pmnc(i, counter_config[i].event&0x0FFFFFFE);

		/*Initialize counter to Zero value*/
		write_counter(i, 0);

		/*Set the counter to u32 max*/
		results[i].reset_counter = counter_config[i].count;
		pmu->int_enable |= pmu->int_mask[i];
	}

	return 0;
}

static irqreturn_t mv88de3010_pmu_interrupt(int irq, void *arg)
{
	int i;
	u32 count;

	TIMERx_ClearInt(MV88DE3010_PMUTIMER);

	for (i = PMN0; i < MAX_COUNTERS; i++) {
		count = read_counter(i); 
		if ( !(pmu->int_mask[i] & pmu->int_enable) ||
		     count < results[i].reset_counter )
			continue;

		write_counter(i, count%results[i].reset_counter);
		results[i].samples = count/results[i].reset_counter; 
		
		do{
			oprofile_add_sample(get_irq_regs(), i);
		}while(--results[i].samples);
	}

	return IRQ_HANDLED;
}

static void mv88de3010_pmu_stop(void)
{
	int i;
	/*Disable timer*/
	TIMERx_SetControlReg(MV88DE3010_PMUTIMER, 
		INT_MASK_BIT, INT_DISABLE_BIT, TIMER_USR_DEFINED_MODE);

	/*Stop the counter*/
	for (i = PMN0; i < MAX_COUNTERS; i++) {
		if(pmu->int_mask[i] & pmu->int_enable)
		{
			__de3010_disable_counter(i);
		}
	}

}

static int mv88de3010_pmu_start(void)
{
	int i;
	/*Enable counters*/
	for (i = PMN0; i < MAX_COUNTERS; i++) {
		if(pmu->int_mask[i] & pmu->int_enable)
		{
			__de3010_enable_counter(i);
		}
	}

	/*Setup APB timer for data collection*/
	TIMERx_SetControlReg(MV88DE3010_PMUTIMER, 
		INT_UNMASK_BIT, INT_DISABLE_BIT, TIMER_USR_DEFINED_MODE);
	TIMERx_SetLoadCount(MV88DE3010_PMUTIMER, 0x100000);

	/* enable timer */
	TIMERx_SetIntEn(MV88DE3010_PMUTIMER, INT_ENABLE_BIT);	

	return 0;
}

static int mv88de3010_detect_pmu(void)
{
	int ret;

	/*Setup APB timer for data collection*/
	TIMERx_SetControlReg(MV88DE3010_PMUTIMER, 
		INT_UNMASK_BIT, INT_DISABLE_BIT, TIMER_USR_DEFINED_MODE);
	TIMERx_SetLoadCount(MV88DE3010_PMUTIMER, 0x100000);
	mv88de3010_open_irq(MV88DE3010_PMUTIMER_IRQ);

	ret = request_irq(MV88DE3010_PMUTIMER_IRQ, mv88de3010_pmu_interrupt, IRQF_DISABLED,
			"mv88de3010 PMU timer", (void *)results);

	if (ret < 0) {
		galois_printk(KERN_ERR "oprofile: unable to request IRQ%d for MV88DE3010 PMU\n",
			MV88DE3010_PMUTIMER_IRQ);
		return ret;
	}

	pmu = &pmu_parms;
	
	return 0;
}

struct op_arm_model_spec op_mv88de3010_spec = {
	.init		= mv88de3010_detect_pmu,
	.num_counters	= MAX_COUNTERS,
	.setup_ctrs	= mv88de3010_setup_ctrs,
	.start		= mv88de3010_pmu_start,
	.stop		= mv88de3010_pmu_stop,
	.name		= "arm/mv88de3010",
};

