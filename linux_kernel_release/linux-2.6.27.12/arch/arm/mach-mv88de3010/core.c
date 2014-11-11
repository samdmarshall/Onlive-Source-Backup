#include "Galois_memmap.h"
#include "galois_hal_com.h"
#include "pic_drv.h"
#include "apb_ictl_drv.h"
#include "apb_timer_drv.h"
#include "apb_perf_base.h"
#define MAIN_DEV_INIT_TBL /* to be deleted */
#include "dev_init_table.h"
#include "global.h"
#include "apb_gpio.h"
#include "pinmux.h"

#include <mach/irqs.h>
#include <mach/galois_generic.h>
#include <mach/galois_platform.h>
#include <mach/io.h>
#include <mach/timex.h>

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <linux/proc_fs.h>

#include <asm/page.h>
#include <asm/memory.h>
#include <asm/mach-types.h>
#include <asm/delay.h>
#include <asm/setup.h>

#include <asm/mach/time.h>
#include <asm/mach/irq.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#ifdef CONFIG_MV88DE3010_DYNAMIC_FREQ
#include <mach/freq.h>

extern SOC_FREQ_VECTOR_Linux socFreqVec;
#endif

/* 
 * sm_ictl.c operations
 */
extern void sm_ICTL_SetIntEn(unsigned int reg_l_h, unsigned int irq_inten);
extern void sm_ICTL_VectorIntEn(unsigned int vector, unsigned int irq_inten);
extern void sm_ICTL_SetIntMask(unsigned int reg_l_h, unsigned int irq_intmask);
extern void sm_ICTL_VectorIntMask(unsigned int vector, unsigned int irq_intmask);

#ifdef CONFIG_ARCH_MV88DE3010_DEBUG
#define DEBUG_PRINTK printk
#else
#define DEBUG_PRINTK(x, ...)
#endif

#define PIC_ALL_ACTIVE_HIGH	0xFFFFFFFF
#define PIC_ALL_ACTIVE_LOW	0x00000000
#define PIC_ALL_EDGE_TRIG	0xFFFFFFFF
#define PIC_ALL_LEVEL_TRIG	0x00000000

/*
 * mv88de3010 read-only information
 */
static char null_string[] = "";
volatile char *berlin_loader_vendor = null_string;
volatile char *berlin_loader_model = null_string;
volatile char *berlin_loader_rev = null_string;
volatile int spruce_flashless_loader = 0;
EXPORT_SYMBOL(berlin_loader_vendor);
EXPORT_SYMBOL(berlin_loader_model);
EXPORT_SYMBOL(berlin_loader_rev);
EXPORT_SYMBOL(spruce_flashless_loader);

unsigned int uboot_mem_start = 0, uboot_mem_size = 0;
static const char mv88de3010_proc_banner[] = 
	"Linux kernel svn revision: %u\n"
	"Linux-used-GaloisSoftware svn revision: %u\n"
	"CPU clock %ldHz, timer clock %ldHz\n"
	"Linux memory start: 0x%08x\n"
	"Berlin loader: %s\n"
	"Spruce flash-less loader: %d\n";

void mv88de3010_print_version(void)
{
#ifndef LINUXKERNEL_SVN_VERSION
#define LINUXKERNEL_SVN_VERSION 0
#endif
#ifndef GALOISSOFTWARE_SVN_VERSION
#define GALOISSOFTWARE_SVN_VERSION 0
#endif
	galois_printk(mv88de3010_proc_banner, 
			LINUXKERNEL_SVN_VERSION+0, GALOISSOFTWARE_SVN_VERSION+0, 
#ifdef CONFIG_MV88DE3010_DYNAMIC_FREQ
			socFreqVec.cpu0, socFreqVec.cfg, 
#else
			GALOIS_CPU_CLK, GALOIS_TIMER_CLK, 
#endif
			(unsigned int)(CONFIG_MV88DE3010_CPU0MEM_START),
			berlin_loader_vendor, 
			spruce_flashless_loader);
	return;
}

static int mv88de3010_read_proc(char *page, char **start, off_t off,
				 int count, int *eof, void *data)
{
	int len;

	len = snprintf(page, PAGE_SIZE, mv88de3010_proc_banner,
		LINUXKERNEL_SVN_VERSION+0, GALOISSOFTWARE_SVN_VERSION+0,
#ifdef CONFIG_MV88DE3010_DYNAMIC_FREQ
		socFreqVec.cpu0, socFreqVec.cfg, 
#else
		GALOIS_CPU_CLK, GALOIS_TIMER_CLK, 
#endif
		(unsigned int)(CONFIG_MV88DE3010_CPU0MEM_START),
		berlin_loader_vendor,
		spruce_flashless_loader);

	/*
	 * copied from proc_calc_metrics(page, start, off, count, eof, len)
	 */
	if (len <= off+count) 
		*eof = 1;
	*start = page + off;
	len -= off;
	if (len>count) 
		len = count;
	if (len<0) 
		len = 0;
	return len;
}

static int mv88de3010_timerfreq_read_proc(char *page, char **start, off_t off,
				 int count, int *eof, void *data)
{
	int len;

	len = snprintf(page, PAGE_SIZE, "%ld",
#ifdef CONFIG_MV88DE3010_DYNAMIC_FREQ
		socFreqVec.cfg
#else
		GALOIS_TIMER_CLK
#endif
		);

	/*
	 * copied from proc_calc_metrics(page, start, off, count, eof, len)
	 */
	if (len <= off+count) 
		*eof = 1;
	*start = page + off;
	len -= off;
	if (len>count) 
		len = count;
	if (len<0) 
		len = 0;
	return len;
}

static int __init mv88de3010_proc_init(void)
{
	create_proc_read_entry("mv88de3010", 0, NULL, mv88de3010_read_proc, NULL);
	create_proc_read_entry("mv88de3010_timerfreq", 0, NULL, mv88de3010_timerfreq_read_proc, NULL);
	return 0;
}
__initcall(mv88de3010_proc_init);

/*
 * the virtual address mapped should be < PAGE_OFFSET or >= VMALLOC_END.
 * Generally, it should be >= VMALLOC_END.
 */
static struct map_desc mv88de3010_io_desc[] __initdata = {
	{/* map phys space 0xF6000000~0xF9000000(doesn't cover Flash area) to same virtual space*/
		.virtual = 0xF6000000, 
		.pfn = __phys_to_pfn(0xF6000000), 
		.length = 0x03000000, 
		.type = MT_DEVICE
	},
	{/* map cross-CPU shared memory to virtual space(must less than 0xFF000000) */
		.virtual = CONFIG_MV88DE3010_SHMMEM_VIRTSTART, 
		.pfn = __phys_to_pfn(CONFIG_MV88DE3010_SHMMEM_START), 
		.length = CONFIG_MV88DE3010_SHMMEM_SIZE, 
		.type = MT_DEVICE
	},
	/* Don't map ROM,vectors space: 0xFF800000~0xFFFFFFFF */
};
/*
 * -Soc init, Serial init, mv88de3010_init-
 * do very early than any initcalls, it's better in setup_arch()
 */
static void __init soc_initialize(void)
{
#ifdef CONFIG_MV88DE3010_STANDALONE

#ifdef CONFIG_MV88DE3010_ASIC
	/*
	 * 1. reset eth phy for asic
	 */
#if 0
#ifdef CONFIG_MV88DE3010_BERLIN
	{
		unsigned int apb_base = SM_APB_GPIO_BASE;
		*(volatile unsigned int *)(apb_base + 0x08) = 0x0;/* sw control */
		udelay(999);
		*(volatile unsigned int *)(apb_base + 0x04) = 0x2;
		udelay(999);
		*(volatile unsigned int *)(apb_base + 0x00) = 0x0;
		udelay(999);
		*(volatile unsigned int *)(apb_base + 0x00) = 0x2;
		udelay(999);
	}
#else
	{
		unsigned int apb_base = APB_GPIO_INST0_BASE;
		*(volatile unsigned int *)(apb_base + APB_GPIO_PORTA_CTL) = 0x0;/* sw control */
		udelay(999);
		*(volatile unsigned int *)(apb_base + APB_GPIO_SWPORTA_DR) = 0x1;
		udelay(999);
		*(volatile unsigned int *)(apb_base + APB_GPIO_SWPORTA_DR) = 0x0;
		udelay(999);
		*(volatile unsigned int *)(apb_base + APB_GPIO_SWPORTA_DR) = 0x1;
		udelay(999);
	}
#endif
#endif

	/*
	 * 2. reset cache timing for Galois ES2
	 */
#ifndef CONFIG_MV88DE3010_BERLIN
#define CACHE_CTL_ADDR0		(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ChipCntl1)
#define CACHE_CTL_ADDR1		(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ChipCntl11)
	{
		unsigned int cpuid;
		unsigned int ctl;

		GA_REG_WORD32_READ(MEMMAP_PROCESSOR_ID_REG_BASE, &cpuid);
		switch (cpuid) {
			case 0:
				/* CPU#0 */
				GA_REG_WORD32_READ(CACHE_CTL_ADDR0,&ctl);
				ctl &= (~0x3e00);
				ctl |= (0x2600);
				GA_REG_WORD32_WRITE(CACHE_CTL_ADDR0,ctl);
				break;
			case 1:
				/* CPU#1 */
				GA_REG_WORD32_READ(CACHE_CTL_ADDR1,&ctl);
				ctl &= (~0x7c0);
				ctl |= (0x4c0);
				GA_REG_WORD32_WRITE(CACHE_CTL_ADDR1,ctl);
				break;
			default:
				asm volatile ("1: b 1b\n");
		}
	}
#endif /* CONFIG_MV88DE3010_BERLIN */
#endif /* CONFIG_MV88DE3010_ASIC */

	/* 
	 * If u-boot has done setup, uncomment it.
	 * XXX: why I found running it will disable ethernet's rx ?
	 */
	mv_general_setup_pinmux();
#endif /* CONFIG_MV88DE3010_STANDALONE */

	mv88de3010_init_hrtimer(GALOIS_CLOCKSOURCE);
	return;
}

static void __init mv88de3010_map_io(void)
{
	iotable_init(mv88de3010_io_desc, ARRAY_SIZE(mv88de3010_io_desc));
//	set_cyc2ns_scale(GALOIS_TIMER_CLK); /* set timer cyc2ns scaler */
	soc_initialize(); /* some ugly to put here, but it works:-) */
	return;
}

/*
 * Enable h/w interrupt in PIC or Ictl.
 */
void mv88de3010_open_irq(unsigned int irq)
{
	if (irq < GALOIS_APB_IRQ_START) {
		PIC_SetPerPIC_PerVector(CPUINDEX, irq, INT_ENABLE_BIT, INT_ENABLE_BIT);
	} else if (irq < GALOIS_SM_IRQ_START) {
		unsigned int apb_irq = irq - GALOIS_APB_IRQ_START;
		ICTL_VectorIntMask(CPUINDEX, apb_irq, INT_UNMASK_BIT);
		ICTL_VectorIntEn(CPUINDEX, apb_irq, INT_ENABLE_BIT);
		PIC_SetPerPIC_PerVector(CPUINDEX, GALOIS_ICTL0_VECTOR, 
				INT_ENABLE_BIT, INT_ENABLE_BIT);
	} else {
		unsigned int sm_irq = irq - GALOIS_SM_IRQ_START;
		sm_ICTL_VectorIntMask(sm_irq, INT_UNMASK_BIT);
		sm_ICTL_VectorIntEn(sm_irq, INT_ENABLE_BIT);
		PIC_SetPerPIC_PerVector(CPUINDEX, G_IRQ_sm2socHwInt0, 
				INT_ENABLE_BIT, INT_ENABLE_BIT);
	}
}

void mv88de3010_close_irq(unsigned int irq)
{
	if (irq < GALOIS_APB_IRQ_START) {
		PIC_SetPerPIC_PerVector(CPUINDEX, irq, INT_DISABLE_BIT, INT_DISABLE_BIT);
	} else if (irq < GALOIS_SM_IRQ_START) {
		unsigned int apb_irq = irq - GALOIS_APB_IRQ_START;
		ICTL_VectorIntMask(CPUINDEX, apb_irq, INT_MASK_BIT);
		ICTL_VectorIntEn(CPUINDEX, apb_irq, INT_DISABLE_BIT);
	} else {
		unsigned int sm_irq = irq - GALOIS_SM_IRQ_START;
		sm_ICTL_VectorIntMask(sm_irq, INT_MASK_BIT);
		sm_ICTL_VectorIntEn(sm_irq, INT_DISABLE_BIT);
	}
}

/*
 * Only called by mv88de3010_devtab_init() to setup global dev 
 * enable variables
 */
int devtab_var_hrtimer = 0;
int devtab_var_uart = 0;
int devtab_var_sm2soc = 0;
int devtab_var_sata = 0;
int devtab_var_eth = 0;
int devtab_var_usb = 0;
int devtab_var_pex = 0;
int devtab_var_intercpu = 0;
static void mv88de3010_devtab_var(DEV_INIT_T *entry)
{
	if (!entry->enable)
		return;

	switch (entry->vector_dev_type) {
		case DEV_TYPE_TIMER:
			if (entry->enable == GRP_DEV_VEC_ENABLE_ONLY)
				devtab_var_hrtimer = 1; /* Only for debug timer */
			break;
		case DEV_TYPE_UART:
			devtab_var_uart = 1;
			break;
#ifdef CONFIG_MV88DE3010_BERLIN
		case DEV_TYPE_SM2SOC:
			devtab_var_sm2soc = 1;
			break;
#endif
		case DEV_TYPE_SATA:
			devtab_var_sata = 1;
			break;
		case DEV_TYPE_ETH:
			devtab_var_eth = 1;
			break;
#ifdef CONFIG_MV88DE3010_BERLIN
		case DEV_TYPE_USB0:
		case DEV_TYPE_USB1:
			devtab_var_usb = 1;
			break;
#endif
		case DEV_TYPE_PEX_INT:
		case DEV_TYPE_PEX_ERR:
			devtab_var_pex = 1;
			break;
		case DEV_TYPE_INTER_HOST_L:
		case DEV_TYPE_INTER_HOST_H:
			devtab_var_intercpu = 1;
			break;
	}
}

/*
 * Initialize devices selectly according to dev_init_tbl
 */
static void mv88de3010_devtab_init(void)
{
	int devtab_size = ARRAY_SIZE(dev_init_tbl);
	unsigned int cpuid;
	unsigned int devtype;
	unsigned int vector;
	int i;

	for (i = 0; i < devtab_size; i++) {
		cpuid = (dev_init_tbl[i].group >> LSb_DRV_CUR_ID) & MASK_DRV_BYTES;
		if (cpuid == CPUINDEX) {
			if (dev_init_tbl[i].enable == GRP_DEV_VEC_ENABLE) {
				devtype = dev_init_tbl[i].vector_dev_type;
				vector = dev_init_tbl[i].vector_id;

				/* open h/w interrupt in pic/ictl */
				if (devtype <= DEV_TYPE_PIC_FIELD)
					mv88de3010_open_irq(vector);
				else if (devtype <= DEV_TYPE_ICTL_FIELD)
					mv88de3010_open_irq(vector + GALOIS_APB_IRQ_START);
#ifdef CONFIG_MV88DE3010_BERLIN
				else if (devtype <= DEV_TYPE_SM_FIELD)
					/* TODO */;
#endif
				else
					DEBUG_PRINTK("Error devtype in dev_init_tbl.\n");
			}
			if (dev_init_tbl[i].enable) {
				mv88de3010_devtab_var(&dev_init_tbl[i]);
			}
		}
	}
}

/*********************************************************************
 * -IRQ ack, mask, unmask, mv88de3010_init_irq-
 *********************************************************************/
static void mv88de3010_ack_irq(unsigned int irq)
{
#if 0
	if (irq < GALOIS_APB_IRQ_START) {
		PIC_ClrIntStsPerVector(irq, 1, 1);
	} else {
		unsigned int value;
		unsigned int apb_irq = irq - GALOIS_APB_IRQ_START;
		/* 
		 * Clear ICTL bit status
		 * Gaoming said this function has expired
		 */
		PIC_ClrIntStsPerVector(GALOIS_ICTL0_VECTOR, 1, 1);
		value = ICTL_GetIrqFinalStatus(CPUINDEX, 0);
		value = value & (~(1<<apb_irq));
		ICTL_SetIrqFinalStatus(CPUINDEX, 0, value);
	}
#endif
}

static void mv88de3010_mask_irq(unsigned int irq)
{
	if (irq < GALOIS_APB_IRQ_START) {
		PIC_SetPerPIC_PerVector(CPUINDEX, irq, INT_DISABLE_BIT, INT_DISABLE_BIT);
	} else if (irq < GALOIS_SM_IRQ_START) {
		unsigned int apb_irq = irq - GALOIS_APB_IRQ_START;
		/*
		 * PIC_SetPerPIC_PerVector(0, GALOIS_ICTL0_VECTOR, 0, 0);
		 * commenit above, don't disable APB level interrupt
		 */
		ICTL_VectorIntMask(CPUINDEX, apb_irq, INT_MASK_BIT);
	} else {
		unsigned int sm_irq = irq - GALOIS_SM_IRQ_START;
		/*
		 * PIC_SetPerPIC_PerVector(0, GALOIS_ICTL0_VECTOR, 0, 0);
		 * commenit above, don't disable APB level interrupt
		 */
		sm_ICTL_VectorIntMask(sm_irq, INT_MASK_BIT);
	}
}

static void mv88de3010_unmask_irq(unsigned int irq)
{
	if (irq < GALOIS_APB_IRQ_START) {
		PIC_SetPerPIC_PerVector(CPUINDEX, irq, INT_ENABLE_BIT, INT_ENABLE_BIT);
	} else if (irq < GALOIS_SM_IRQ_START) {
		unsigned int apb_irq = irq - GALOIS_APB_IRQ_START;
		/*
		 * PIC_SetPerPIC_PerVector(0, GALOIS_ICTL0_VECTOR, 1, 1);
		 * commenit above, GALOIS_ICTL0_VECTOR is always enabled.
		 */
		ICTL_VectorIntMask(CPUINDEX, apb_irq, INT_UNMASK_BIT);
	} else {
		unsigned int sm_irq = irq - GALOIS_SM_IRQ_START;
		/*
		 * PIC_SetPerPIC_PerVector(0, GALOIS_ICTL0_VECTOR, 1, 1);
		 * commenit above, GALOIS_ICTL0_VECTOR is always enabled.
		 */
		sm_ICTL_VectorIntMask(sm_irq, INT_UNMASK_BIT);
	}
}

/*
 * PIC chip for mv88de3010
 */
struct irq_chip mv88de3010_irq_chip = {
	.ack = mv88de3010_ack_irq,
	.mask = mv88de3010_mask_irq,
	.unmask = mv88de3010_unmask_irq,
};

void do_mv88de3010_init_irq(void)
{
	/* 
	 * Initialize PIC:
	 * 1.Disable CPU#0 Global FIQ/IRQ 
	 * 2.Set CPU#0 FIQ/IRQ target: active-low and level-triggered
	 * 3.Disable all CPU#0 interrupts (Initialize Global FIQ/IRQ, FIQ/IRQ target)
	 * 4.Set Common FIQ/IRQ source: interrupt active-high and level-triggered
	 * 5.Clear FIQ/IRQ source interrupt status [NO clr]
	 */
	PIC_SetPerPIC_TgtGIntE(CPUINDEX, INT_DISABLE_BIT, INT_DISABLE_BIT);
	PIC_SetPerPIC_TgtSel(CPUINDEX, PIC_ALL_ACTIVE_LOW, PIC_ALL_LEVEL_TRIG, 
			PIC_ALL_ACTIVE_LOW, PIC_ALL_LEVEL_TRIG);
	PIC_SetPerPIC(CPUINDEX, /* disable all interrupt bits */
			PIC_ALL_ACTIVE_LOW, PIC_ALL_LEVEL_TRIG, INT_DISABLE_ALL, INT_DISABLE_BIT,
			PIC_ALL_ACTIVE_LOW, PIC_ALL_LEVEL_TRIG, INT_DISABLE_ALL, INT_DISABLE_BIT);

#ifdef CONFIG_MV88DE3010_BERLIN
	PIC_SetSource(CPUINDEX, PIC_ALL_ACTIVE_HIGH, PIC_ALL_LEVEL_TRIG, 
			PIC_ALL_ACTIVE_HIGH, PIC_ALL_LEVEL_TRIG);
#else
	PIC_SetSource(PIC_ALL_ACTIVE_HIGH, PIC_ALL_LEVEL_TRIG, 
			PIC_ALL_ACTIVE_HIGH, PIC_ALL_LEVEL_TRIG);
#endif
#if 0
	PIC_ClrIntSts(0x0, 0x0);
#endif

	/* 
	 * Initialize APB_ICTL:
	 * 1.Disable all APB_ICTL interrupt
	 * 2.Mask all APB_ICTL interrupt
	 * 3.Clear all interrupt status [NO]
	 */
	ICTL_SetIntEn(CPUINDEX, 0, INT_DISABLE_ALL);
	ICTL_SetIntMask(CPUINDEX, 0, INT_MASK_ALL);
	/* NO clear status function */

	/* 
	 * Initialize SM ICTL:
	 * 1.Disable all SM ICTL interrupt
	 * 2.Mask all SM ICTL interrupt
	 * 3.Clear all interrupt status [NO]
	 */
	sm_ICTL_SetIntEn(0, INT_DISABLE_ALL);
	sm_ICTL_SetIntMask(0, INT_MASK_ALL);
	/* NO clear status function */

	/* 
	 * Enable ICTL0 interrupt in PIC always (XXX)
	 * Enable SM-ICTL1 interrupt in PIC always (XXX)
	 * Let APB-device interrupts(ictlInst0CPUIrq status) bypass PIC and propagate to CPU.
	 */
	mv88de3010_devtab_init();
	mv88de3010_open_irq(GALOIS_ICTL0_VECTOR);
	mv88de3010_open_irq(G_IRQ_sm2socHwInt0);
	mv88de3010_open_irq(IRQ_SATAINTR);
	mv88de3010_open_irq(IRQ_ETHINTR);

	/* 
	 * Disable Global FIQ, Enable Global IRQ
	 * NOTE: here the CPU core IRQ/FIQ is still masked.
	 */
	PIC_SetPerPIC_TgtGIntE(CPUINDEX, INT_DISABLE_BIT, INT_ENABLE_BIT);
}
EXPORT_SYMBOL(do_mv88de3010_init_irq);

static void __init mv88de3010_init_irq(void)
{
	int i;

	do_mv88de3010_init_irq();

	for (i = 0; i < NR_IRQS; i++) {
		set_irq_chip(i, &mv88de3010_irq_chip);
		set_irq_handler(i, handle_level_irq);
		set_irq_flags(i, IRQF_VALID | IRQF_PROBE);
	}

	return;
}

/*
 * serial_initialize is moved to serial.c
 */
#if 0
static void __init serial_initialize(void)
{
	struct uart_port serial_req;
	unsigned int irq;

#ifdef CONFIG_MV88DE3010_BERLIN_UARTBUG
	irq = 0; /* 0 to not use interrupt */
#else
	irq = IRQ_APB_UARTINST0;
	/* Enable UART interrupt in APB-ICTL */
	mv88de3010_open_irq(irq);
#endif

	/* Prapare uart_port for console */
	memset(&serial_req, 0, sizeof(serial_req));
	serial_req.membase = (unsigned char *)(APB_UART_INST0_BASE);
	serial_req.irq = irq; /* 0 to not use int */
	serial_req.uartclk = GALOIS_UART_TCLK;
	serial_req.regshift = 2;
	serial_req.iotype = SERIAL_IO_MEM;
	serial_req.flags = ASYNC_BOOT_AUTOCONF | ASYNC_SKIP_TEST;
	serial_req.line = 0;

	if (early_serial_setup(&serial_req) != 0)
		DEBUG_PRINTK("Early serial init of UART port #0 failed.\n");
	return;
}
#endif

/*
 * mv88de3010 SoC initialization, in arch_initcall()
 */
static void __init mv88de3010_init(void)
{
#if 0
	soc_initialize(); /* moved to .map_io() */
	serial_initialize();
#endif
}

static void __init mv88de3010_fixup(struct machine_desc *desc, struct tag *tag,
		char **cmdline, struct meminfo *mi)
{
	struct tag *tmp_tag = tag;
	/*
	 * MV88DE3010 only receives ATAG_CORE, ATAG_MEM, ATAG_CMDLINE, ATAG_NONE
	 * Here change ATAG_MEM to be controlled by Linux itself.
	 */
	if (tmp_tag->hdr.tag == ATAG_CORE) {
		for (; tmp_tag->hdr.size; tmp_tag = tag_next(tmp_tag)) {
			if (tmp_tag->hdr.tag == ATAG_MEM) {
				tmp_tag->u.mem.size = CONFIG_MV88DE3010_CPU0MEM_SIZE;
				tmp_tag->u.mem.start = CONFIG_MV88DE3010_CPU0MEM_START;
				break;
			}
		}
	}
}

/*
 * define global "struct machine_desc __mach_desc_MV88DE3010" structure.
 * .nr=MACH_TYPE_MV88DE3010
 * .name="MV88DE3010"
 */
#define MV88DE3010_PHYSIO_BASE 0xF0000000
#define ARM_BOOT_PARAMS_ADDR (CONFIG_MV88DE3010_CPU0MEM_START + 0x00000100)

MACHINE_START(MV88DE3010, "MV88DE3010")
	/* Maintainer: Yufeng Zheng<yfzheng@marvell.com> */
	.phys_io	= MV88DE3010_PHYSIO_BASE,
	.io_pg_offst	= (((u32)MV88DE3010_PHYSIO_BASE) >> 18) & 0xfffc,
	.boot_params	= ARM_BOOT_PARAMS_ADDR,

	.fixup		= mv88de3010_fixup,
	.map_io		= mv88de3010_map_io,
	.init_irq	= mv88de3010_init_irq,
	.timer		= &mv88de3010_timer,
	.init_machine	= mv88de3010_init,
MACHINE_END

/*
 * Export functions to be used.
 */
EXPORT_SYMBOL(mv88de3010_open_irq);
EXPORT_SYMBOL(mv88de3010_close_irq);

