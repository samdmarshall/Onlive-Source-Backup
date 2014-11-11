/*
 * arch/arm/mach-galois/pcie.c
 *
 * PCIe functions for Marvell Galois SoCs
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/mbus.h>
#include <asm/mach/pci.h>
#include <mach/irqs.h>
#include <mach/pcie.h>
#include <linux/delay.h>
#include <mach/galois_generic.h>
#include "Galois_memmap.h"
#include "global.h"
#include "galois_io.h"
#include "apb_perf_base.h"
#include "gpio.h"

static int basic_pcie_rd_conf(void __iomem *base, struct pci_bus *bus,
		       u32 devfn, int where, int size, u32 *val)
{
	writel(PCIE_CONF_BUS(bus->number) |
		PCIE_CONF_DEV(PCI_SLOT(devfn)) |
		PCIE_CONF_FUNC(PCI_FUNC(devfn)) |
		PCIE_CONF_REG(where) | PCIE_CONF_ADDR_EN,
			base + PCIE_CONF_ADDR_OFF);

	*val = readl(base + PCIE_CONF_DATA_OFF);

	if (size == 1)
		*val = (*val >> (8 * (where & 3))) & 0xff;
	else if (size == 2)
		*val = (*val >> (8 * (where & 3))) & 0xffff;
	
	return PCIBIOS_SUCCESSFUL;
}

static int basic_pcie_wr_conf(void __iomem *base, struct pci_bus *bus,
		       u32 devfn, int where, int size, u32 val)
{
	int ret = PCIBIOS_SUCCESSFUL;

	writel(PCIE_CONF_BUS(bus->number) |
		PCIE_CONF_DEV(PCI_SLOT(devfn)) |
		PCIE_CONF_FUNC(PCI_FUNC(devfn)) |
		PCIE_CONF_REG(where) | PCIE_CONF_ADDR_EN,
			base + PCIE_CONF_ADDR_OFF);
	/*
	 * Berlin register space does not support word or byte 
	 * operation, only double words operation supported, 
	 * we will use read->modify->write to set the value.
	 */
	if (size == 4) {
		writel(val, base + PCIE_CONF_DATA_OFF);
	} else if (size == 2) {
		u32 nval = readl(base + PCIE_CONF_DATA_OFF);
		nval &= ~(0xffff << ((where & 0x3) * 8));
		nval |= ((val & 0xffff) << ((where & 0x3) * 8));
		writel(nval, base + PCIE_CONF_DATA_OFF);
	} else if (size == 1) {
		u32 nval = readl(base + PCIE_CONF_DATA_OFF);
		nval &= ~(0xff << ((where & 3) * 8));
		nval |= ((val & 0xff) << ((where & 3) * 8));
		writel(nval, base + PCIE_CONF_DATA_OFF);
	} else {
		ret = PCIBIOS_BAD_REGISTER_NUMBER;
	}

	return ret;
}

static void __init basic_pcie_setup(void __iomem *base)
{
	u32 cmd;
	u32 mask;
	u32 data;

	/*
	 * PCIe Reset
	 */
#if 0
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkEnable), &data);
	data |= 1 << LSb32Gbl_clkEnable_pexCoreClkEn;
	GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkEnable), data);
	msleep(1);
#endif

#if 1
	/* 
	 * resetting PCIe core, don't read RA_Gbl_ResetTrigger register, or write other bits
	 * of RA_Gbl_ResetTrigger to 1, or other modules' core will be reset. 
	 * RA_Gbl_ResetTrigger register is weird register, pay attention of it. - Yufeng
	 */
	data = 1 << LSb32Gbl_ResetTrigger_pexSyncReset;
	GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ResetTrigger), data);
	msleep(1);
	
	data = 0;
	GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ResetTrigger), data);
	msleep(1);
#endif

#if 0	
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_chipCntl), &data);
	data |= 1 << LSb32Gbl_chipCntl_pcie_rc;
	GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_chipCntl), data);
	msleep(1);
#endif

	/*
	 * GPIO Initialize
	 */
#define MV_GPIO_PCIE_RESET		12
#define MV_GPIO_PCIE_POWER		14
#define MV_GPIO_PULLUP			1
#define MV_GPIO_PULLDOWN		0
#define MV_GPIO_CONFIN			1
#define MV_GPIO_CONFOUT			0
	mv_pcie_setup_pinmux();

	GPIO_PortSetInOut(MV_GPIO_PCIE_RESET, MV_GPIO_CONFOUT); /* output mode */
	GPIO_PortWrite(MV_GPIO_PCIE_RESET, MV_GPIO_PULLDOWN); /* low */
	
	GPIO_PortSetInOut(MV_GPIO_PCIE_POWER, MV_GPIO_CONFOUT); /* output mode */
	GPIO_PortWrite(MV_GPIO_PCIE_POWER, MV_GPIO_PULLDOWN); /* low */
	msleep(1);

	/* turn on power */
	GPIO_PortWrite(MV_GPIO_PCIE_POWER, MV_GPIO_PULLUP); /* high */
	msleep(1);
	
	/*turn on reset*/
	GPIO_PortWrite(MV_GPIO_PCIE_RESET, MV_GPIO_PULLUP); /* high */
	msleep(1);

	/*
	 * Root Complexity Master + Slave enable.
	 */
	cmd = readl(base + PCIE_CMD_OFF);
	cmd |= PCI_COMMAND_IO;
	cmd |= PCI_COMMAND_MEMORY;
	cmd |= PCI_COMMAND_MASTER;
	cmd |= 0x100000; 	/* Device Complexity */
	writel(cmd, base + PCIE_CMD_OFF);

	/*
	 * Enable interrupt lines A-D.
	 */
	mask = readl(base + PCIE_MASK_OFF);
	mask |= 0x0f000000;
	writel(mask, base + PCIE_MASK_OFF);
	
	/*
	 *	DownStream related registers initialization
	 */
	writel(0, base + PCIE_BAR_LO_OFF(0));
	writel(0, base + PCIE_DS_BASE_OFF(0));
	writel(0, base + PCIE_DS_BASE_OFF(1));
	writel(0, base + PCIE_DS_BASE_OFF(2));

	/*
	 * Disabling PCIe core regrets
	 */
	writel(0x00180003, base + 0x18d0);
}

static int pcie_valid_config(int bus, int dev)
{
	/*
	 * Don't go out when trying to access --
	 * 1. nonexisting device on local bus
	 * 2. where there's no device connected (no link)
	 */
	if (bus == 0 && dev == 0)
		return 1;

	if (readl(PCIE_REG_BASE + PCIE_STAT_OFF) & PCIE_STAT_LINK_DOWN)
		return 0;

	if (bus == 0 && dev != 1)
		return 0;

	return 1;
}


/*
 * PCIe config cycles are done by programming the PCIE_CONF_ADDR register
 * and then reading the PCIE_CONF_DATA register. Need to make sure these
 * transactions are atomic.
 */
static DEFINE_SPINLOCK(galois_pcie_lock);

static int pcie_rd_conf(struct pci_bus *bus, u32 devfn, int where,
			int size, u32 *val)
{
	unsigned long flags;
	int ret;

	if (pcie_valid_config(bus->number, PCI_SLOT(devfn)) == 0) {
		*val = 0xffffffff;
		return PCIBIOS_DEVICE_NOT_FOUND;
	}

	spin_lock_irqsave(&galois_pcie_lock, flags);
	ret = basic_pcie_rd_conf(PCIE_REG_BASE, bus, devfn, where, size, val);
	spin_unlock_irqrestore(&galois_pcie_lock, flags);
	
	return ret;
}

static int pcie_wr_conf(struct pci_bus *bus, u32 devfn,
			int where, int size, u32 val)
{
	unsigned long flags;
	int ret;

	if (pcie_valid_config(bus->number, PCI_SLOT(devfn)) == 0)
		return PCIBIOS_DEVICE_NOT_FOUND;
	
	spin_lock_irqsave(&galois_pcie_lock, flags);
	ret = basic_pcie_wr_conf(PCIE_REG_BASE, bus, devfn, where, size, val);
	spin_unlock_irqrestore(&galois_pcie_lock, flags);

	return ret;
}

static struct pci_ops pcie_ops = {
	.read = pcie_rd_conf,
	.write = pcie_wr_conf,
};


static int galois_pcie_setup(int nr, struct pci_sys_data *sys)
{	
	struct resource *res;
	
	/*
	 * Generic PCIe unit setup.
	 */
	basic_pcie_setup(PCIE_REG_BASE);

	/*
	 * Request resources.
	 */
	res = kzalloc(sizeof(struct resource) * 2, GFP_KERNEL);
	if (!res)
		panic("pcie_setup unable to alloc resources");

	/*
	 * IORESOURCE_MEM
	 */
	res[0].name = "PCIe Memory Space";
	res[0].flags = IORESOURCE_MEM;
	res[0].start = PCIE_MEM_BASE;
	res[0].end = res[0].start + PCIE_MEM_SIZE - 1;
	if (request_resource(&iomem_resource, &res[0]))
		panic("Request PCIe Memory resource failed\n");
	sys->resource[0] = &res[0];
	
	sys->resource[1] = NULL;

	{/* set the offset between bus-centric and linux-centric addresses 
	  * offset = pcie upstream base - pcie host base, 
	  * 32 bit addressing only, for 64 bit addressing, 
	  * we need to add (PCIE_HPCIE_BASE_OFF(1) << 32) -Qingchao
	  */
		resource_size_t offset = 0;
		/* PCIe upstream base and host base */
		u32 upstream_base = 0, host_base = 0;
		
		upstream_base = readl(PCIE_REG_BASE + PCIE_US_BASE_OFF);
		host_base = readl(PCIE_REG_BASE + PCIE_HPCIE_BASE_OFF(0));
		offset = upstream_base - host_base;
		
		sys->mem_offset = offset;
	}
	
	return 1;
}

static void __devinit rc_pci_fixup(struct pci_dev *dev)
{
	/*
	 * Prevent enumeration of root complex.
	 */
	if (dev->bus->parent == NULL && dev->devfn == 0) {
		int i;

		for (i = 0; i < DEVICE_COUNT_RESOURCE; i++) {
			dev->resource[i].start = 0;
			dev->resource[i].end   = 0;
			dev->resource[i].flags = 0;
		}
	}
}
DECLARE_PCI_FIXUP_HEADER(PCI_VENDOR_ID_MARVELL, PCI_ANY_ID, rc_pci_fixup);

static struct pci_bus __init *
galois_pcie_scan_bus(int nr, struct pci_sys_data *sys)
{
	struct pci_bus *bus;

	if (nr == 0) {
		bus = pci_scan_bus(sys->busnr, &pcie_ops, sys);
	} else {
		bus = NULL;
		BUG();
	}

	return bus;
}

static int __init galois_pcie_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
{
	return G_IRQ_pex2intr_int;
}

static struct hw_pci galois_pci __initdata = {
	.nr_controllers	= 1,
	.swizzle	= pci_std_swizzle,
	.setup		= galois_pcie_setup,
	.scan		= galois_pcie_scan_bus,
	.map_irq	= galois_pcie_map_irq,
};

static void __init galois_pcie_init(void)
{
	pci_common_init(&galois_pci);
}

static int __init mv88de3010_pci_init(void)
{
#ifndef CONFIG_MV88DE3010_PM_DISABLE_PCIE
	galois_pcie_init();
#endif
	return 0;
}
subsys_initcall(mv88de3010_pci_init);

static void galois_pcie_resume_early(struct pci_dev *dev)
{
	/*
	 * Generic PCIe unit setup.
	 */
	basic_pcie_setup(PCIE_REG_BASE);
	
	mv88de3010_open_irq(G_IRQ_pex2intr_int);
}
DECLARE_PCI_FIXUP_RESUME_EARLY(PCI_VENDOR_ID_MARVELL, PCI_ANY_ID, galois_pcie_resume_early);
