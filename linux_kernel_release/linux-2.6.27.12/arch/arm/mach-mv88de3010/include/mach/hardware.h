#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

/* assign a bus number over the bridge while scanning it */
#define pcibios_assign_all_busses()	1   

#define PCIBIOS_MIN_IO		0x1000		/* min IO allocate for PCI dev */
#define PCIBIOS_MIN_MEM		0x01000000	/* min MEM allocate for PCI dev */

#endif

