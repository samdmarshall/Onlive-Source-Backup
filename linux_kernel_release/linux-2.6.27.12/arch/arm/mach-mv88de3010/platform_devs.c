#include "Galois_memmap.h"
#include "apb_perf_base.h"
#include "apb_uart.h"

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/serial_8250.h>
#include <linux/serial_reg.h>
#include <linux/types.h>
#include <linux/platform_device.h>

#include <asm/irq.h>

#include <mach/galois_platform.h>
#include <mach/nand_priv.h>
#include <mach/irqs.h>

static u64 mv88de3010_dma_mask = 0xffffffffULL;

/*
 * UART device
 */
static struct plat_serial8250_port platform_serial_ports[] = {
#ifdef CONFIG_MV88DE3010_UART0
	{
		.membase = (void *)(APB_UART_INST0_BASE),
		.mapbase = (unsigned long)(APB_UART_INST0_BASE),
		.irq = IRQ_APB_UARTINST0,
		.uartclk = GALOIS_UART_TCLK,
		.regshift = 2,
		.iotype = UPIO_DWAPB,
		.flags = UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
		.private_data = (void *)(APB_UART_INST0_BASE + APB_UART_USR), /* read USR to clear busy int */
	},
#endif
#ifdef CONFIG_MV88DE3010_UART1
	{
		.membase = (void *)(APB_UART_INST1_BASE),
		.mapbase = (unsigned long)(APB_UART_INST1_BASE),
		.irq = IRQ_APB_UARTINST1,
		.uartclk = GALOIS_UART_TCLK,
		.regshift = 2,
		.iotype = UPIO_DWAPB,
		.flags = UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
		.private_data = (void *)(APB_UART_INST1_BASE + APB_UART_USR), /* read USR to clear busy int */
	},
#endif
#ifdef CONFIG_MV88DE3010_UART2
	{
		.membase = (void *)(APB_UART_INST2_BASE),
		.mapbase = (unsigned long)(APB_UART_INST2_BASE),
		.irq = IRQ_APB_UARTINST2,
		.uartclk = GALOIS_UART_TCLK,
		.regshift = 2,
		.iotype = UPIO_DWAPB,
		.flags = UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
		.private_data = (void *)(APB_UART_INST2_BASE + APB_UART_USR), /* read USR to clear busy int */
	},
#endif
	{}
};

static struct platform_device mv88de3010_serial_dev = {
	.name = "serial8250",
	.id = PLAT8250_DEV_PLATFORM,
	.dev = {
		.platform_data = &platform_serial_ports,
	},
};

#ifdef CONFIG_MV88DE3010_PCIE
/*
 * PCIe host
 */
static struct resource mv88de3010_pcie_resource[] = {
	[0] = {
		.start 	= MEMMAP_PCIE_REG_BASE,
		.end	= MEMMAP_PCIE_REG_BASE + MEMMAP_PCIE_REG_SIZE - 1,
		.name	= "pcie_mem",
		.flags	= IORESOURCE_MEM,	
	},
	[1] = {
		.start	= G_IRQ_pex2intr_int,
		.end	= G_IRQ_pex2intr_int,
		.name 	= "pcie_irq",
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device mv88de3010_pcie_dev = {
	.name = "mv88de3010_pcie",
	.id	= -1,
	.num_resources = ARRAY_SIZE(mv88de3010_pcie_resource),
	.resource = mv88de3010_pcie_resource,
};
#endif

/*
 * SATA device
 */
static struct resource mv88de3010_sata_resource[] = {
	[0] = {
		.start	= MEMMAP_SATA_REG_BASE,
		.end	= MEMMAP_SATA_REG_BASE + MEMMAP_SATA_REG_SIZE - 1,
		.name	= "sata_mem",
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_SATAINTR,
		.end	= IRQ_SATAINTR,
		.name	= "sata_irq",
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device mv88de3010_sata_dev = {
	.name = "mv88de3010_sata",
	.id	= -1,
	.num_resources = ARRAY_SIZE(mv88de3010_sata_resource),
	.resource = mv88de3010_sata_resource,
};

/*
 * Ethernet device
 */
static struct resource mv88de3010_eth_resource[] = {
	[0] = {
		.start	= MEMMAP_ETHERNET_REG_BASE,
		.end	= MEMMAP_ETHERNET_REG_BASE + MEMMAP_ETHERNET_REG_SIZE - 1,
		.name	= "eth_mem",
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_ETHINTR,
		.end	= IRQ_ETHINTR,
		.name	= "eth_irq",
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device mv88de3010_eth_dev = {
	.name = "mv88de3010_eth",
	.id	= -1,
	.num_resources = ARRAY_SIZE(mv88de3010_eth_resource),
	.resource = mv88de3010_eth_resource,
};

#ifdef CONFIG_MV88DE3010_BERLIN_USB
/*
 * USB Host Controller device
 */

static struct resource mv88de3010_usb0_resource[] = {
	[0] = {	
		.start	= MEMMAP_USB0_REG_BASE,
		.end	= MEMMAP_USB0_REG_BASE + MEMMAP_USB0_REG_SIZE - 1,
		.name	= "usb0_mem",
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_USB0,
		.end	= IRQ_USB0,
		.name	= "usb0_irq",
		.flags	= IORESOURCE_IRQ,
	},
};
struct platform_device mv88de3010_usb0_dev = {
	.name = "mv88de3010_ehci",
	.id	= 0,
	.num_resources = ARRAY_SIZE(mv88de3010_usb0_resource),
	.resource = mv88de3010_usb0_resource,
	.dev = {
		.dma_mask = &mv88de3010_dma_mask,
		.coherent_dma_mask = 0xffffffff,
	},
};

static struct resource mv88de3010_usb1_resource[] = {
	[0] = {
		.start	= MEMMAP_USB1_REG_BASE,
		.end	= MEMMAP_USB1_REG_BASE + MEMMAP_USB1_REG_SIZE - 1,
		.name	= "usb1_mem",
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_USB1,
		.end	= IRQ_USB1,
		.name	= "usb1_irq",
		.flags	= IORESOURCE_IRQ,
	},
};
struct platform_device mv88de3010_usb1_dev = {
	.name = "mv88de3010_ehci",
	.id	= 1,
	.num_resources = ARRAY_SIZE(mv88de3010_usb1_resource),
	.resource = mv88de3010_usb1_resource,
	.dev = {
		.dma_mask = &mv88de3010_dma_mask,
		.coherent_dma_mask = 0xffffffff,
	},

}; 		

#endif /* CONFIG_MV88DE3010_BERLIN_USB */

/*
 * NAND device.
 */
#define NUM_OF_FLASH_CHIP 1
#define NAND_FLASH_CHIP_SIZE 0x40000000	/* 1GB */
#define NAND_FLASH_SIZE  NAND_FLASH_CHIP_SIZE*NUM_OF_FLASH_CHIP

#define NAND_PART0_OFFSET	0x0
#define NAND_PART0_SIZE		0x04000000 /* 64M */
#define NAND_PART1_OFFSET	(NAND_PART0_OFFSET+NAND_PART0_SIZE)
#define NAND_PART1_SIZE 	0x05000000 /* 80M:rootfs */
#define NAND_PART2_OFFSET	(NAND_PART1_OFFSET+NAND_PART1_SIZE)
#define NAND_PART2_SIZE 	0x05000000 /* 80M:app-A */
#define NAND_PART3_OFFSET	(NAND_PART2_OFFSET+NAND_PART2_SIZE)
#define NAND_PART3_SIZE 	0x05000000 /* 80M:app-B */
#define NAND_PART4_OFFSET	(NAND_PART3_OFFSET+NAND_PART3_SIZE)
#define NAND_PART4_SIZE 	(NAND_FLASH_CHIP_SIZE-NAND_PART4_OFFSET)

static struct mtd_partition mv_nand_partition[] = {
	[0] = {
		.name	= "NAND part#0(boot flow)",
		.offset	= NAND_PART0_OFFSET,
		.size = NAND_PART0_SIZE,
	},
	[1] = {
		.name = "NAND part#1(ro rootfs)",
		.offset = NAND_PART1_OFFSET,
		.size = NAND_PART1_SIZE,
	},
	[2] = {
	    .name = "NAND part#2(ro app-A)",
		.offset = NAND_PART2_OFFSET,
		.size = NAND_PART2_SIZE,
	},
	[3] = {
	    .name = "NAND part#3(ro app-B)",
		.offset = NAND_PART3_OFFSET,
		.size = NAND_PART3_SIZE,
	},
	[4] = {
	     .name = "NAND part#4(rw data)",
		 .offset = NAND_PART4_OFFSET,
		 .size = NAND_PART4_SIZE,
	},
};

static struct mtd_partition *mv_nand_partitions(int size, int *num_partitions)
{
	*num_partitions = ARRAY_SIZE(mv_nand_partition);
	return mv_nand_partition;
}

static struct mv_nand_data mv_nand_data=
{
	.szofpg = 2,	  /* size of page *KB */
	.szofblk = 256,	/* size of block *KB */
	.conspare = 1,    /* SPARE_EN */
	.numofchip = NUM_OF_FLASH_CHIP,    /*num of chip */
	.ncs    = 0,       /*one chip = 0,two chip = will be changed by select_chip*/
	.ndcr = MV_NAND_NDCR_VAL,
	.ndtr0cs0 = MV_NAND_NDTR0CS0_VAL,
	.ndtr1cs0 = MV_NAND_NDTR1CS0_VAL,
	.ndredel = MV_NAND_NDREDEL_VAL,
	.ndsr = MV_NAND_NDSR_VAL ,
	.ndeccctrl = MV_NAND_NDECCCTRL_VAL,
	.partition_info	= mv_nand_partitions,
};

static struct resource mv_nand_resources[] = {
	[0] = {
		.start	= MEMMAP_NAND_FLASH_REG_BASE,
		.end	= MEMMAP_NAND_FLASH_REG_BASE+MEMMAP_NAND_FLASH_REG_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device mv88de3010_nand_dev = {
	.name		= "mv_nand",
	.id		= -1,
	.dev		= {
				.platform_data	= &mv_nand_data,
	},
	.resource	= mv_nand_resources,
	.num_resources	= ARRAY_SIZE(mv_nand_resources),
};

#ifdef CONFIG_MV88DE3010_SDHC
static struct resource mv88de3010_sdhc_resource[] = {
	[0] = {
		.start  = MEMMAP_SDIO_REG_BASE,
		.end    = MEMMAP_SDIO_REG_BASE + MEMMAP_SDIO_REG_SIZE - 1,
		.name   = "sdio_mem",
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.start  = G_IRQ_sdio_interrupt,
		.end    = G_IRQ_sdio_interrupt,
		.name   = "sdio_irq",
		.flags  = IORESOURCE_IRQ,
	},
 };
struct platform_device mv88de3010_sdhc_dev = {
	.name = "mv_sdhci",
	.id     = -1,
	.dev        = {
	},
	.resource   = mv88de3010_sdhc_resource,
	.num_resources  = ARRAY_SIZE(mv88de3010_sdhc_resource),
};

#endif

static struct platform_device mv88de3010_cc_dev = {
	.name		= "MV_CC_Module",
	.id		= -1,
	.dev		= {
				.platform_data	= NULL,
	},
};

static struct platform_device *mv88de3010_platform_devs[] __initdata = {
	&mv88de3010_serial_dev,
	&mv88de3010_sata_dev,
#ifndef CONFIG_MV88DE3010_PM_DISABLE_ETH
	&mv88de3010_eth_dev,
#endif
#ifdef CONFIG_MV88DE3010_BERLIN_USB
	&mv88de3010_usb0_dev,
#ifndef CONFIG_MV88DE3010_PM_DISABLE_USB1
	&mv88de3010_usb1_dev,
#endif
#endif
	&mv88de3010_nand_dev,
#ifdef CONFIG_MV88DE3010_SDHC
#ifndef CONFIG_MV88DE3010_PM_DISABLE_SDIO
	&mv88de3010_sdhc_dev,
#endif
#endif
#ifdef CONFIG_MV88DE3010_PCIE
#ifndef CONFIG_MV88DE3010_PM_DISABLE_PCIE
	&mv88de3010_pcie_dev,
#endif
#endif
#ifdef CONFIG_MV88DE3010_CC_MODULE
	&mv88de3010_cc_dev,
#endif
};

static int __init mv88de3010_devs_init(void)
{
	int ret;

	ret = platform_add_devices(mv88de3010_platform_devs, ARRAY_SIZE(mv88de3010_platform_devs));
	return ret;
}

arch_initcall(mv88de3010_devs_init);

int mv88de3010_close_phys_cores(void)
{
#ifdef CONFIG_MV88DE3010_PLT_DISABLE2SATA
	galois_printk("Disable 2 SATA\n");
	*((unsigned int *)0xF7E901F8)=5;
	*((unsigned int *)0xF7E901FC)=0xC33C550;
#endif
#ifdef CONFIG_MV88DE3010_PM_DISABLE_USB1
	galois_printk("Disable USB1\n");
	/* turn off PHY layer */
	*((unsigned int *)0xF7EE0800) = 0;
	/* turn off core layer clock */
	*((unsigned int *)0xF7EA0090) = *((unsigned int *)0xF7EA0090) & ~(1<<12);
#endif
#ifdef CONFIG_MV88DE3010_PM_DISABLE_ETH
	galois_printk("Disable ETH\n");
	/* turn off core layer clock */
	*((unsigned int *)0xF7EA0090) = *((unsigned int *)0xF7EA0090) & ~(1<<7);
#endif
#ifdef CONFIG_MV88DE3010_PM_DISABLE_PCIE
	galois_printk("Disable PCIE\n");
	/* turn off PHY layer */
	*((unsigned int *)0xF7E41B00) = 0x00800087;
	/* turn off core layer clock control */
	*((unsigned int *)0xF7EA0090) = *((unsigned int *)0xF7EA0090) & ~(1<<8);
#endif
#ifdef CONFIG_MV88DE3010_PM_DISABLE_SDIO
	galois_printk("Disable SDIO\n");
	*((unsigned int *)0xF7EA0090) = *((unsigned int *)0xF7EA0090) & ~(1<<14);
#endif
#ifdef CONFIG_MV88DE3010_PM_DISABLE_SPI_TWSI
	galois_printk("Disable SPI_TWSI\n");
	*((unsigned int *)0xF7EA0090) = *((unsigned int *)0xF7EA0090) & ~(1<<10);
#endif
	return 0;
}

late_initcall(mv88de3010_close_phys_cores);
