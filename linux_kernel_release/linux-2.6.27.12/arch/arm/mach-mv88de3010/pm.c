/*
 * MV88DE3010 Power Management Routines
 *
 * Copyright (c) 2010 Marvell
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/suspend.h>
#include <linux/errno.h>
#include <linux/time.h>
#include <asm/cacheflush.h>
#include <asm/io.h>

#include <mach/irqs.h>
#include <mach/uncompress.h>
#include <mach/galois_generic.h>
#include <mach/galois_platform.h>
#include <mach/io.h>
#include <mach/timex.h>
#include <mach/pm.h>

#include "Galois_memmap.h"
#include "galois_hal_com.h"
#include "pic_drv.h"
#include "apb_ictl_drv.h"
#include "apb_timer_drv.h"
#include "apb_perf_base.h"
#include "global.h"
#include "apb_gpio.h"
#include "pinmux.h"
#include "SM_common.h"

#ifdef CONFIG_MV88DE3010_DYNAMIC_FREQ
#include "mach/freq.h"
extern SOC_FREQ_VECTOR_Linux socFreqVec;
#endif

struct mv88de3010_pm_regs mv_pm_regs;

static int mv88de3010_pm_valid(suspend_state_t state)
{
	return suspend_valid_only_mem(state);
}

extern void MV_CC_Module_resume(void);
extern void berlin_sm_resume(void);
static void mv88de3010_restore_sm(void)
{
	berlin_sm_resume();
}
static void mv88de3010_restore_serial(void)
{
	/*
	 * setup pinmux of UART0/1/2
	 */	
	mv_uart_setup_pinmux();

#ifdef CONFIG_MV88DE3010_UART0
#ifndef CONFIG_MV88DE3010_BERLIN_UARTBUG
	mv88de3010_open_irq(IRQ_APB_UARTINST0);
#endif
#endif
	
#ifdef CONFIG_MV88DE3010_UART1
#ifndef CONFIG_MV88DE3010_BERLIN_UARTBUG
	mv88de3010_open_irq(IRQ_APB_UARTINST1);
#endif
#endif

#ifdef CONFIG_MV88DE3010_UART2
#ifndef CONFIG_MV88DE3010_BERLIN_UARTBUG
	mv88de3010_open_irq(IRQ_APB_UARTINST2);
#endif
#endif
}

static void mv88de3010_restore_timer(void)
{
	TIMERx_SetControlReg(LINUX_TIMER, INT_UNMASK_BIT, INT_DISABLE_BIT, TIMER_USR_DEFINED_MODE);
	TIMERx_SetLoadCount(LINUX_TIMER, socFreqVec.cfg/HZ);

	TIMERx_SetIntEn(LINUX_TIMER, INT_ENABLE_BIT);	/* Enable counter */

	/* enable h/w timer interrupt */
	mv88de3010_open_irq(TIMER_IRQ);
}
extern void do_mv88de3010_init_irq(void);
static void mv88de3010_restore_pic(void)
{
	do_mv88de3010_init_irq();
}
static void mv88de3010_restore_pinmux(void)
{
	mv_general_setup_pinmux();
}

#ifdef CONFIG_MV88DE3010_PWM
extern void galois_pwm_resume(void);
extern void galois_pwm_suspend(void);
#endif

#ifdef CONFIG_MV88DE3010_SPI
extern void galois_spi_resume(void);
#endif

#ifdef CONFIG_MV88DE3010_I2C
extern void galois_twsi_resume(void);
#endif

#ifdef CONFIG_MV88DE3010_GPIO
extern void galois_gpio_suspend(void);
extern void galois_gpio_resume(void);
#endif

#include <linux/delay.h>
extern int nfc_resume_delay;
extern void mv88de3010_close_phys_cores(void);
static void mv88de3010_restore_system(void)
{
	/* soc_initialize */
	mv88de3010_restore_pinmux();
	mv88de3010_init_hrtimer(GALOIS_CLOCKSOURCE);

	/* mv88de3010_init_irq */
	mv88de3010_restore_pic();

	/* mv88de3010_time_init */
	mv88de3010_restore_timer();

	/* serial_initialize */
	mv88de3010_restore_serial();

	mv88de3010_restore_sm();

	{
		#define CHIP_CTRL_SW_GENERIC3_REG	(RA_Gbl_sw_generic3 + MEMMAP_CHIP_CTRL_REG_BASE)
		#define RESUME_SYNC_REG			(CHIP_CTRL_SW_GENERIC3_REG)

		#define RESUME_SYNC_BIT			(0x1)
		#define RESUME_SYNC_DONE(value)		((value) & (1 << RESUME_SYNC_BIT))
		#define RESUME_SYNC_CHECK_INTERVAL		(50)	/* in millisecond */
		
		unsigned int i = 0;
		unsigned int count = 500 / RESUME_SYNC_CHECK_INTERVAL;
		volatile unsigned int sync_value;
		
		sync_value = *(unsigned int *)RESUME_SYNC_REG;
		galois_printk("check cpu1 sync flag %p:%d, %08X\n",
				CHIP_CTRL_SW_GENERIC3_REG, RESUME_SYNC_BIT, sync_value);
		while (!RESUME_SYNC_DONE(sync_value)) {
			if (i % count)
				galois_printk("*");
			msleep_interruptible(RESUME_SYNC_CHECK_INTERVAL);
			sync_value = *(unsigned int *)RESUME_SYNC_REG;
			i++;
		}
		galois_printk(" %ums elapsed\n", i * RESUME_SYNC_CHECK_INTERVAL);
	}
#ifndef CONFIG_MV88DE3010_PM_DISABLE_SPI_TWSI
#ifdef CONFIG_MV88DE3010_SPI
	galois_spi_resume();
#endif
#ifdef CONFIG_MV88DE3010_I2C
	galois_twsi_resume();
#endif
#endif
#ifdef CONFIG_MV88DE3010_PWM
	galois_pwm_resume();
#endif

#ifdef CONFIG_MV88DE3010_GPIO
	galois_gpio_resume();
#endif

	mv88de3010_close_phys_cores();
}

extern void MV_CC_Module_suspend(void);
extern void berlin_sm_suspend(void);
static void mv88de3010_suspend_sm(void)
{
	berlin_sm_suspend();
}
static void mv88de3010_suspend_system(void)
{
#ifdef CONFIG_MV88DE3010_GPIO
	galois_gpio_suspend();
#endif
#ifdef CONFIG_MV88DE3010_PWM
	galois_pwm_suspend();
#endif
	mv88de3010_suspend_sm();
	/*
	 * close interrupt
	 */
	PIC_SetPerPIC_TgtGIntE(CPUINDEX, INT_DISABLE_BIT, INT_DISABLE_BIT);
}

static void flush_cpu_cache(void)
{
	__cpuc_flush_kern_all();
	//__cpuc_flush_l2cache_all();
}

#include <mach/pm.h>
static void EnterStandby_2(void)
{
	int msg[2];

	msg[0] = MV_SM_POWER_WARMDOWN_REQUEST_2;
	msg[1] = virt_to_phys(mv88de3010_cpu_resume);

	printk("%s,%s: To enter Standby\n", __FILE__, __func__);
	bsm_msg_send(MV_SM_ID_POWER,msg,2*sizeof(int));
}

/*
 * task freezed, deviced stopped
 */
static int mv88de3010_pm_enter(suspend_state_t state)
{
	struct mv88de3010_pm_regs *pm_regs = &mv_pm_regs;

	if (state != PM_SUSPEND_MEM)
		return -EINVAL;

	mv88de3010_suspend_system();

	pm_regs->pm_data.modeSaveFlags = PM_MODE_SAVE_FLAG_ALL;//0x27;
	pm_regs->pm_data.flushFunc = flush_cpu_cache;
	pm_regs->pm_data.standbyFunc = EnterStandby_2;
	pm_regs->pm_data.areaAddress = (unsigned int)&(pm_regs->pm_data);
	pm_regs->pm_data.extendedChecksumByteCount =
		sizeof(struct mv88de3010_pm_regs) - sizeof(struct pm_save_data);
	printk("ext size:%d, save size%d\n",
		pm_regs->pm_data.extendedChecksumByteCount,
		sizeof(struct pm_save_data));

	pm_regs->word0 = __raw_readl(PAGE_OFFSET);
	__raw_writel(virt_to_phys(&(pm_regs->pm_data)), PAGE_OFFSET);

	mv88de3010_cpu_sleep((unsigned int)&(pm_regs->pm_data),
				virt_to_phys(&(pm_regs->pm_data)));

	/* come back */
	__raw_writel(pm_regs->word0, PAGE_OFFSET);

	mv88de3010_restore_system();
	return 0;
}

static struct platform_suspend_ops mv88de3010_pm_ops = {
	.valid = mv88de3010_pm_valid,
	.enter = mv88de3010_pm_enter,
};

static int __init mv88de3010_pm_init(void)
{
	suspend_set_ops(&mv88de3010_pm_ops);
	return 0;
}

device_initcall(mv88de3010_pm_init);
