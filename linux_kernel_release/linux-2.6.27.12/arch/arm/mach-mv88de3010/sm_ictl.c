#include "Galois_memmap.h"
#include "galois_io.h"
#include "apb_perf_base.h"
#include "apb_ictl.h"

#define SM_APB_ICTL1_BASE		(SOC_SM_APB_REG_BASE + 0xE000)

void sm_ICTL_SetIntEn(unsigned int reg_l_h, unsigned int irq_inten)
{
	unsigned int base, offset;

	/* CPU#0 can always operate on SM ICTL1 */
	base = SM_APB_ICTL1_BASE;

	// offset for irq_inten
	switch(reg_l_h) {
		case 0:
			offset = APB_ICTL_IRQ_INTEN_L;
			break;
		default:
			offset = APB_ICTL_IRQ_INTEN_H;
	}
	GA_REG_WORD32_WRITE(base + offset, irq_inten);

	// offset for fiq_inten
	offset = APB_ICTL_FIQ_INTEN;
	GA_REG_WORD32_WRITE(base + offset, 0);
}

void sm_ICTL_VectorIntEn(unsigned int vector, unsigned int irq_inten)
{
	unsigned int base, offset;
	unsigned int reg_l_h = 0;
	unsigned int value;

	if (vector >= 32) {
		vector -= 32;
		reg_l_h = 1;
	}

	/* CPU#0 can always operate on SM ICTL1 */
	base = SM_APB_ICTL1_BASE;

	// offset for irq_inten
	switch(reg_l_h) {
		case 0:
			offset = APB_ICTL_IRQ_INTEN_L;
			break;
		default:
			offset = APB_ICTL_IRQ_INTEN_H;
	}
	GA_REG_WORD32_READ(base + offset, &value);
	if (irq_inten) {
		value |= (1<<vector);
	} else {
		value &= ~(1<<vector);
	}
	GA_REG_WORD32_WRITE(base + offset, value);

	// offset for fiq_inten
	offset = APB_ICTL_FIQ_INTEN;
	GA_REG_WORD32_WRITE(base + offset, 0);
}

void sm_ICTL_SetIntMask(unsigned int reg_l_h, unsigned int irq_intmask)
{
	unsigned int base, offset;

	/* CPU#0 can always operate on SM ICTL1 */
	base = SM_APB_ICTL1_BASE;

	// offset for irq_intmask
	switch(reg_l_h) {
		case 0:
			offset = APB_ICTL_IRQ_INTMASK_L;
			break;
		default:
			offset = APB_ICTL_IRQ_INTMASK_H;
	}
	GA_REG_WORD32_WRITE(base + offset, irq_intmask);

	// offset for fiq_intmask
	offset = APB_ICTL_FIQ_INTMASK;
	GA_REG_WORD32_WRITE(base + offset, 0xFFFFFFFF);
}

void sm_ICTL_VectorIntMask(unsigned int vector, unsigned int irq_intmask)
{
	unsigned int base, offset;
	unsigned int value;
	unsigned int reg_l_h = 0;

	if (vector >= 32) {
		vector -= 32;
		reg_l_h = 1;
	}

	/* CPU#0 can always operate on SM ICTL1 */
	base = SM_APB_ICTL1_BASE;

	// offset for irq_inten
	switch(reg_l_h) {
		case 0:
			offset = APB_ICTL_IRQ_INTMASK_L;
			break;
		default:
			offset = APB_ICTL_IRQ_INTMASK_H;
	}
	GA_REG_WORD32_READ(base + offset, &value);
	if (irq_intmask) {
		value |= (1<<vector);
	} else {
		value &= ~(1<<vector);
	}
	GA_REG_WORD32_WRITE(base + offset, value);

	// offset for fiq_inten
	offset = APB_ICTL_FIQ_INTMASK;
	GA_REG_WORD32_WRITE(base + offset, 0xFFFFFFFF);
}

