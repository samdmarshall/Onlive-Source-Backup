#ifndef _GALOIS_PCIE_H_
#define _GALOIS_PCIE_H_

/* PCIe memory space, get it from upstream space */
#define PCIE_MEM_BASE	MEMMAP_PCIE_UPSTREAM_BASE
#define PCIE_MEM_SIZE	SZ_512M

/* PCIe register base */
#define PCIE_REG_BASE	((void __iomem *)MEMMAP_PCIE_REG_BASE)

/* PCIe downstream base offset */
#define PCIE_DS_BASE_OFF(n)  	(0x1c0c + ((n) << 2))

#define PCIE_US_BASE_OFF		0x1c00
#define PCIE_HPCIE_BASE_OFF(n)	(0x1c04 + ((n) << 2))

/* PCIe BARs offset */
#define PCIE_BAR_LO_OFF(n)		(0x0010 + ((n) << 3))
#define PCIE_BAR_HI_OFF(n)		(0x0014 + ((n) << 3))
#define PCIE_BAR_CTRL_OFF(n)	(0x1804 + ((n - 1) * 4))

#define PCIE_CONF_ADDR_OFF  	0x18F8
#define PCIE_CONF_DATA_OFF		0x18fc

#define  PCIE_CONF_ADDR_EN		0x80000000
#define  PCIE_CONF_REG(r)		((((r) & 0xf00) << 16) | ((r) & 0xfc))
#define  PCIE_CONF_BUS(b)		(((b) & 0xff) << 16)
#define  PCIE_CONF_DEV(d)		(((d) & 0x1f) << 11)
#define  PCIE_CONF_FUNC(f)		(((f) & 0x7) << 8)

#define PCIE_WIN04_CTRL_OFF(n)	(0x1820 + ((n) << 4))
#define PCIE_WIN04_BASE_OFF(n)	(0x1824 + ((n) << 4))
#define PCIE_WIN04_REMAP_OFF(n)	(0x182c + ((n) << 4))

#define PCIE_CAUSE_OFF			0x1900
#define PCIE_MASK_OFF			0x1910
#define PCIE_CMD_OFF			0x0004
#define PCIE_CTRL_OFF			0x1a00
#define PCIE_STAT_OFF			0x1a04
#define  PCIE_STAT_LINK_DOWN	1
#endif
