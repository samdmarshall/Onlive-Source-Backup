#ifndef __ASM_ARM_ARCH_IO_H
#define __ASM_ARM_ARCH_IO_H

#define IO_SPACE_LIMIT 0xffffffff
#define IO_SPACE_REMAP 0xf2000000

#ifdef CONFIG_MV88DE3010_PCIE
#define __mem_pci(a)		(a)
#else /* CONFIG_MV88DE3010_PCIE */
#define __mem_pci(a)		((unsigned long)(a))
#endif
#define __io(a)				(a + IO_SPACE_REMAP)

#define __mem_isa(a)		(a)

#endif
