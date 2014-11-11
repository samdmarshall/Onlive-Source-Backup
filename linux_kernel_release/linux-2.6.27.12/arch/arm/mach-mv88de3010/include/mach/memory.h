#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

/*
 * Physical DRAM offset: PHYS_OFFSET
 */
#define PHYS_OFFSET	(CONFIG_MV88DE3010_CPU0MEM_START)

#define __virt_to_bus(x) __virt_to_phys(x)
#define __bus_to_virt(x) __phys_to_virt(x)

#endif
