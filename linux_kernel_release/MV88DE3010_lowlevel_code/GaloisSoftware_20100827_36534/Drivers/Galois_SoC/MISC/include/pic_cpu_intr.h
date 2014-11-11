
#ifndef __PIC_CPU_INTR_H__
#define __PIC_CPU_INTR_H__
#include "pic.h"
#include "galois_os_def.h"

#define IRQ_inter_hostHtoCur		0x1F
#define IRQ_inter_hostLtoCur		0x1E
#define IRQ_inter_PCIEtoCPU0		0x1F
#define IRQ_inter_CPU1toCPU0		0x1E
#define IRQ_inter_PCIEtoCPU1		0x1F
#define IRQ_inter_CPU0toCPU1		0x1E
#define IRQ_inter_CPU1toPCIE		0x1F
#define IRQ_inter_CPU0toPCIE		0x1E
#define LSbPIC_HOST_ID_H		0x1F
#define LSbPIC_HOST_ID_L		0x1E

#define PIC_CPU0			0
#define PIC_CPU1			1
#define PIC_PCIE			2

#if (GALOIS_CPU_ID == 0)
#if !defined(BERLIN)
#define IRQ_dmaIntrAvio			IRQ_dmaIntr0Avio
#define IRQ_semIntrVpro			IRQ_semIntr0Vpro
#define IRQ_dmaIntrVpro			IRQ_dmaIntr0Vpro
#define IRQ_ictlInstCPUIrq		IRQ_ictlInst0CPUIrq
#endif
#define	TIMER_OS			0
#define PIC_HOST_ID_H			PIC_PCIE
#define PIC_HOST_ID_L			PIC_CPU1

#elif (GALOIS_CPU_ID == 1)
#if !defined(BERLIN)
#define IRQ_dmaIntrAvio			IRQ_dmaIntr1Avio
#define IRQ_semIntrVpro			IRQ_semIntr1Vpro
#define IRQ_dmaIntrVpro			IRQ_dmaIntr1Vpro
#define IRQ_ictlInstCPUIrq		IRQ_ictlInst1CPUIrq
#endif
#define PIC_HOST_ID_H			PIC_PCIE
#define PIC_HOST_ID_L			PIC_CPU0
#define	TIMER_OS			1

#elif (GALOIS_CPU_ID == 2)
#if !defined(BERLIN)
#define IRQ_dmaIntrAvio			IRQ_dmaIntr2Avio
#define IRQ_semIntrVpro			IRQ_semIntr2Vpro
#define IRQ_dmaIntrVpro			IRQ_dmaIntr2Vpro
#define IRQ_ictlInstCPUIrq		IRQ_ictlInst2CPUIrq
#endif
#define PIC_HOST_ID_H			PIC_CPU1
#define PIC_HOST_ID_L			PIC_CPU0
#define	TIMER_OS			2

#endif

#if defined(BERLIN)
#define IRQ_ictlInstCPUIrq		IRQ_ictlInst0CPUIrq
#endif

//#define TIMER_SYSTEM			6
//#ifdef SOC_ES1
//#define SYSTEM_TIMER_VALUE		(100000000 - 1)	/* interrupt/1s, used by Fang */
//#else
//#define SYSTEM_TIMER_VALUE		(1500000 - 1)
//#endif
//#define TIMER_DEBUG			7	/* performance timer */
//#define DEBUG_TIMER_VALUE		0xffffffff

#endif // __PIC_CPU_INTR_H__

