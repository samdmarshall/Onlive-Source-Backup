#include "apb_perf_base.h"
#include "apb_ictl.h"
#include "galois_io.h"
/*******************************************************************************
* Program Guide:
*  initialization 
*  1. disable all interrupt by writing to irq_inten and fiq_inten
*  2. initialize the peripheral deices that could generate interrupts.
*  3. Program the irq_vector, irq-pleevel, irq_intmask, fiq_intmask register as appropriate.
*  4. enable interrupts.

*  Interrupt service.
* Without vectored interrupts, a normal interrupt servicing sequence is as follows:
* 1. Poll the interrupt status register (irq_finalstatus, irq_maskstatus, or fiq_finalstatus, as appropriate)
*   to determine which interrupt source caused the interrupt.
* 2. Service the interrupt.
* 3. Optionally read the irq_status or fiq_status register to see if other currently masked interrupts are
*    pending; service these as required.
*******************************************************************************/

/*******************************************************************************
* Function:    ICTL_VectorIntEn
*
* Description: Set ICTL N interrupt enable or disable.(disable FIQ by default)
*
* Inputs:      ictl_N-- ictl N (N from 0 to 2)
*              vector - vector number
*              irq_inten-- set irq_inten regsiter with this value
*              
* Outputs:     none
*
* Return:      none
*******************************************************************************/

/*******************************************************************************
* Changelog:
* Fixed reg_l_h value when vector is large than 32. -Yufeng
*******************************************************************************/

void ICTL_VectorIntEn(UNSG32 ictl_N, UNSG32 vector, UNSG32 irq_inten)
{
	UNSG32 base, offset;
	UNSG32 reg_l_h = 0;
	UNSG32 value;
	if (vector >= 32) {
		vector -= 32;
		reg_l_h = 1;
	}
	switch (ictl_N) {
		case 0:
			base = APB_ICTL_INST0_BASE;
			break;
		case 1:
			base = APB_ICTL_INST1_BASE;
			break;
		case 2:
			base = APB_ICTL_INST2_BASE;
			break;
		default:
			base = APB_ICTL_INST0_BASE;
	}
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
/*******************************************************************************
* Function:    ICTL_SetIntEn
*
* Description: Set ICTL N interrupt enable or disable.
*
* Inputs:      ictl_N-- ictl N (N from 0 to 2)
*              reg_l_h-- register L or H
*              irq_inten-- set irq_inten regsiter with this value
*              fiq_inten-- set fiq_inten regsiter with this value
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void ICTL_SetIntEn(UNSG32 ictl_N, UNSG32 reg_l_h, UNSG32 irq_inten)
{
	UNSG32 base, offset;
	switch (ictl_N) {
		case 0:
			base = APB_ICTL_INST0_BASE;
			break;
		case 1:
			base = APB_ICTL_INST1_BASE;
			break;
		case 2:
			base = APB_ICTL_INST2_BASE;
			break;
		default:
			base = APB_ICTL_INST0_BASE;
	}
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
/*******************************************************************************
* Function:    ICTL_GetIntEn
*
* Description: Get ICTL N interrupt enable or disable.
*
* Inputs:      ictl_N-- ictl N (N from 0 to 2)
*              reg_l_h-- register L or H
*              irq_inten-- set irq_inten regsiter with this value
*              fiq_inten-- set fiq_inten regsiter with this value
* Outputs:     none
*
* Return:      none
*******************************************************************************/

UNSG32 ICTL_GetIntEn(UNSG32 ictl_N, UNSG32 reg_l_h)
{
	UNSG32 base, offset;
	UNSG32 irq_inten;
	switch (ictl_N) {
		case 0:
			base = APB_ICTL_INST0_BASE;
			break;
		case 1:
			base = APB_ICTL_INST1_BASE;
			break;
		case 2:
			base = APB_ICTL_INST2_BASE;
			break;
		default:
			base = APB_ICTL_INST0_BASE;
	}
	// offset for irq_inten
	switch(reg_l_h) {
		case 0:
			offset = APB_ICTL_IRQ_INTEN_L;
			break;
		default:
			offset = APB_ICTL_IRQ_INTEN_H;
	}
	GA_REG_WORD32_READ(base + offset, &irq_inten);
	return irq_inten;
}
/*******************************************************************************
* Function:    ICTL_VectorIntMask
*
* Description: Set ICTL N interrupt mask. (mask FIQ by default)
*
* Inputs:      ictl_N-- ictl N (N from 0 to 2)
*              vector - vector number
*              irq_intmask-- set irq_inmask register
*              
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void ICTL_VectorIntMask(UNSG32 ictl_N, UNSG32 vector, UNSG32 irq_intmask)
{
	UNSG32 base, offset;
	UNSG32 value;
	UNSG32 reg_l_h = 0;
	if (vector >= 32) {
		vector -= 32;
		reg_l_h = 1;
	}
	switch (ictl_N) {
		case 0:
			base = APB_ICTL_INST0_BASE;
			break;
		case 1:
			base = APB_ICTL_INST1_BASE;
			break;
		case 2:
			base = APB_ICTL_INST2_BASE;
			break;
		default:
			base = APB_ICTL_INST0_BASE;
	}
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
	GA_REG_WORD32_WRITE(base + offset, 0xFF);
}

/*******************************************************************************
* Function:    ICTL_SetIntMask
*
* Description: Set ICTL N interrupt mask. (mask FIQ by default)
*
* Inputs:      ictl_N-- ictl N (N from 0 to 2)
*              
*              irq_intmask-- set irq_intmask register
*              
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void ICTL_SetIntMask(UNSG32 ictl_N, UNSG32 reg_l_h, UNSG32 irq_intmask)
{
	UNSG32 base, offset;
	switch (ictl_N) {
		case 0:
			base = APB_ICTL_INST0_BASE;
			break;
		case 1:
			base = APB_ICTL_INST1_BASE;
			break;
		case 2:
			base = APB_ICTL_INST2_BASE;
			break;
		default:
			base = APB_ICTL_INST0_BASE;
	}
	// offset for irq_inten
	switch(reg_l_h) {
		case 0:
			offset = APB_ICTL_IRQ_INTMASK_L;
			break;
		default:
			offset = APB_ICTL_IRQ_INTMASK_H;
	}
	GA_REG_WORD32_WRITE(base + offset, irq_intmask);

	// offset for fiq_inten
	offset = APB_ICTL_FIQ_INTMASK;
	GA_REG_WORD32_WRITE(base + offset, 0xFF);
}
/*******************************************************************************
* Function:    ICTL_GetIntMask
*
* Description: Get ICTL N interrupt mask.
*
* Inputs:      ictl_N-- ictl N (N from 0 to 2)
*              reg_l_h-- register L or H
* Outputs:     none
*
* Return:      irq_intmask
*******************************************************************************/
UNSG32 ICTL_GetIntMask(UNSG32 ictl_N, UNSG32 reg_l_h)
{
	UNSG32 base, offset;
	UNSG32 irq_intmask;
	switch (ictl_N) {
		case 0:
			base = APB_ICTL_INST0_BASE;
			break;
		case 1:
			base = APB_ICTL_INST1_BASE;
			break;
		case 2:
			base = APB_ICTL_INST2_BASE;
			break;
		default:
			base = APB_ICTL_INST0_BASE;
	}
	// offset for irq_inten
	switch(reg_l_h) {
		case 0:
			offset = APB_ICTL_IRQ_INTMASK_L;
			break;
		default:
			offset = APB_ICTL_IRQ_INTMASK_H;
	}
	GA_REG_WORD32_READ(base + offset, &irq_intmask);
	return irq_intmask;
}

/*******************************************************************************
* Function:    ICTL_GetIrqFinalStatus
*
* Description: Get ICTL N Final interrupt Status
*
* Inputs:      ictl_N-- ictl N (N from 0 to 2)
*              reg_l_h-- register L or H
* Outputs:     none
*
* Return:      status
*******************************************************************************/
UNSG32 ICTL_GetIrqFinalStatus(UNSG32 ictl_N, UNSG32 reg_l_h)
{
	UNSG32 base, offset;
	UNSG32 irq_intsts;
	switch (ictl_N) {
		case 0:
			base = APB_ICTL_INST0_BASE;
			break;
		case 1:
			base = APB_ICTL_INST1_BASE;
			break;
		case 2:
			base = APB_ICTL_INST2_BASE;
			break;
		default:
			base = APB_ICTL_INST0_BASE;
	}
	// offset for irq_inten
	switch(reg_l_h) {
		case 0:
			offset = APB_ICTL_IRQ_FINALSTATUS_L;
			break;
		default:
			offset = APB_ICTL_IRQ_FINALSTATUS_H;
	}
	GA_REG_WORD32_READ(base + offset, &irq_intsts);
	return irq_intsts;
}

/*******************************************************************************
* Function:    ICTL_GetIrqRawStatus
*
* Description: Get ICTL N Raw interrupt Status
*
* Inputs:      ictl_N-- ictl N (N from 0 to 2)
*              reg_l_h-- register L or H
* Outputs:     none
*
* Return:      status
*******************************************************************************/
UNSG32 ICTL_GetIrqRawStatus(UNSG32 ictl_N, UNSG32 reg_l_h)
{
	UNSG32 base, offset;
	UNSG32 irq_intsts;
	switch (ictl_N) {
		case 0:
			base = APB_ICTL_INST0_BASE;
			break;
		case 1:
			base = APB_ICTL_INST1_BASE;
			break;
		case 2:
			base = APB_ICTL_INST2_BASE;
			break;
		default:
			base = APB_ICTL_INST0_BASE;
	}
	// offset for irq_inten
	switch(reg_l_h) {
		case 0:
			offset = APB_ICTL_IRQ_RAWSTATUS_L;
			break;
		default:
			offset = APB_ICTL_IRQ_RAWSTATUS_H;
	}
	GA_REG_WORD32_READ(base + offset, &irq_intsts);
	return irq_intsts;
}
