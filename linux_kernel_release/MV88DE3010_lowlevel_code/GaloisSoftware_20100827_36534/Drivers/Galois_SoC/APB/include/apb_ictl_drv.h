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
void ICTL_VectorIntEn(UNSG32 ictl_N, UNSG32 vector, UNSG32 irq_inten);
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
void ICTL_SetIntEn(UNSG32 ictl_N, UNSG32 reg_l_h, UNSG32 irq_inten);
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

UNSG32 ICTL_GetIntEn(UNSG32 ictl_N, UNSG32 reg_l_h);
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
void ICTL_VectorIntMask(UNSG32 ictl_N, UNSG32 vector, UNSG32 irq_intmask);

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
void ICTL_SetIntMask(UNSG32 ictl_N, UNSG32 reg_l_h, UNSG32 irq_intmask);
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
UNSG32 ICTL_GetIntMask(UNSG32 ictl_N, UNSG32 reg_l_h);

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
UNSG32 ICTL_GetIrqFinalStatus(UNSG32 ictl_N, UNSG32 reg_l_h);

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
UNSG32 ICTL_GetIrqRawStatus(UNSG32 ictl_N, UNSG32 reg_l_h);
