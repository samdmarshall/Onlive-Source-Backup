#include "apb_timer.h"
#include "apb_perf_base.h"
#include "galois_io.h"
/*******************************************************************************
* Program Guide:
* 1. Initialize DW_apb_timers (Write “0” in bit 0 of TimerNControlReg; 
*                              set appropriate values for timer mode 
*                              and interrupt mask)
* 2. Load Counter Value (Write to TimerNLoadCount register
* 3. Enable Timer  (Write “1” in bit 0 of TimerNControlReg)
*******************************************************************************/




/*******************************************************************************
* Function:    TIMERx_SetLoadCount
*
* Description: Set Timer_N LoadCount.
*
* Inputs:      timer_N-- timer N (N=0 to 7)
*              count_val-- counter value
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void TIMERx_SetLoadCount(UNSG32 timer_N, UNSG32 count_val)
{	
	UNSG32 base;
	UNSG32 offset;

	base = APB_TIMER_INST_BASE;
	offset = APD_TIMER_1_LOADCOUNT + timer_N*APD_TIMER_REG_RANGE;

	GA_REG_WORD32_WRITE(base+offset, count_val);
}
/*******************************************************************************
* Function:    TIMERx_SetControlReg
*
* Description: Set Timer_N Control Register.
*
* Inputs:      timer_N-- timer N(N=0 to 7)
*              mask-- set mask bit. 1- masked, 0- not masked
*              enable-- timer eanble bit. 1- enable, 0- disable
*              mode -- timer mode. 1-user-defined count mode, 0- free run mode
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void TIMERx_SetControlReg(UNSG32 timer_N, UNSG32 mask, UNSG32 enable, UNSG32 mode)
{
	UNSG32 base;
	UNSG32 offset;
	UNSG32 val;

	base = APB_TIMER_INST_BASE;
	offset = APB_TIMER_1_CONTROLREG + timer_N*APD_TIMER_REG_RANGE;
	val = (mask << LSbTimer_Interrupt_Mask)  | (enable << LSbTimer_Enable) | (mode << LSbTimer_Mode);
	GA_REG_WORD32_WRITE(base+offset, val);
}
/*******************************************************************************
* Function:    TIMERx_SetModeAndMask
*
* Description: Set Timer_N Control Register.
*
* Inputs:      timer_N-- timer N(N=0 to 7)
*              mask-- set mask bit. 1- masked, 0- not masked
*              mode -- timer mode. 1-user-defined count mode, 0- free run mode
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void TIMERx_SetModeAndMask(UNSG32 timer_N, UNSG32 mask, UNSG32 mode)
{
	UNSG32 base;
	UNSG32 offset;
	UNSG32 val;

	base = APB_TIMER_INST_BASE;
	offset = APB_TIMER_1_CONTROLREG + timer_N*APD_TIMER_REG_RANGE;
	GA_REG_WORD32_READ(base+offset, &val);
	val &= ~( (1 << LSbTimer_Interrupt_Mask) | (1 << LSbTimer_Mode));
	val |= (mask << LSbTimer_Interrupt_Mask)  | (mode << LSbTimer_Mode);
	GA_REG_WORD32_WRITE(base+offset, val);
}
/*******************************************************************************
* Function:    TIMERx_SetMask
*
* Description: Set Timer_N Control Register.
*
* Inputs:      timer_N-- timer N(N=0 to 7)
*              mask-- set mask bit. 1- masked, 0- not masked
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void TIMERx_SetMask(UNSG32 timer_N, UNSG32 mask)
{
	UNSG32 base;
	UNSG32 offset;
	UNSG32 val;

	base = APB_TIMER_INST_BASE;
	offset = APB_TIMER_1_CONTROLREG + timer_N*APD_TIMER_REG_RANGE;
	GA_REG_WORD32_READ(base+offset, &val);
	val &= ~ (1 << LSbTimer_Interrupt_Mask);
	val |= (mask << LSbTimer_Interrupt_Mask);
	GA_REG_WORD32_WRITE(base+offset, val);
}
/*******************************************************************************
* Function:    TIMERx_SetIntEn
*
* Description: enable or disable Timer_N Control Register.
*
* Inputs:      timer_N-- timer N(N=0 to 7)
*              enable-- timer eanble bit. 1- enable, 0- disable
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void TIMERx_SetIntEn(UNSG32 timer_N, UNSG32 enable)
{
	UNSG32 base;
	UNSG32 offset;
	UNSG32 val;

	base = APB_TIMER_INST_BASE;
	offset = APB_TIMER_1_CONTROLREG + timer_N*APD_TIMER_REG_RANGE;
	GA_REG_WORD32_READ(base+offset, &val);
	val &= ~(1 << LSbTimer_Enable);
	val |= (enable << LSbTimer_Enable);
	GA_REG_WORD32_WRITE(base+offset, val);
}
/*******************************************************************************
* Function:    TIMERx_GetCurrentValue
*
* Description: Get Timer_N current Value
*
* Inputs:      timer_N-- timer N(N=0 to 7)
*
* Outputs:     none
*
* Return:      timer_N current value.
*******************************************************************************/
UNSG32 TIMERx_GetCurrentValue(UNSG32 timer_N)
{
	UNSG32 base;
	UNSG32 offset;
	UNSG32 val;

	base = APB_TIMER_INST_BASE;
	offset = APB_TIMER_1_CURRENTVALUE + timer_N*APD_TIMER_REG_RANGE;
	GA_REG_WORD32_READ(base+offset, &val);
	return val;
}
/*******************************************************************************
* Function:    TIMERx_ClearInt
*
* Description: clear timer N interrupt.
*
* Inputs:      timer_N-- timer N(N=0 to 7)
*
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void TIMERx_ClearInt(UNSG32 timer_N)
{
	UNSG32 base;
	UNSG32 offset;
	UNSG32 val;

	base = APB_TIMER_INST_BASE;
	offset = APB_TIMER_1_EOI + timer_N*APD_TIMER_REG_RANGE;
	GA_REG_WORD32_READ(base+offset, &val);
}
/*******************************************************************************
* Function:    TIMERx_GetIntStatus
*
* Description: Get timer N interrupt status.
*
* Inputs:      timer_N-- timer N(N=0 to 7)
*
* Outputs:     none
*
* Return:      timer N interrupt status
*******************************************************************************/
UNSG32 TIMERx_GetIntStatus (UNSG32 timer_N)
{
	UNSG32 base;
	UNSG32 offset;
	UNSG32 val;

	base = APB_TIMER_INST_BASE;
	offset = APB_TIMER_1_INTSTATUS + timer_N*APD_TIMER_REG_RANGE;
	GA_REG_WORD32_READ(base+offset, &val);
	return val;
}
/*******************************************************************************
* Function:    TIMERS_ClearAllInt
*
* Description: Clear All timers interrupt.
*
* Inputs:      none
*
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void TIMERS_ClearAllInt(void)
{
	UNSG32 base;
	UNSG32 offset;
	UNSG32 val;

	base = APB_TIMER_INST_BASE;
	offset = APB_TIMERS_EOI;
	GA_REG_WORD32_READ(base+offset, &val);
}
/*******************************************************************************
* Function:    TIMERS_ClearAllInt
*
* Description: Get All timers interrupt.
*
* Inputs:      none
*
* Outputs:     none
*
* Return:      All timers interrupt ststus.
*******************************************************************************/
UNSG32 TIMERS_GetAllIntStatus (void)
{
	UNSG32 base;
	UNSG32 offset;
	UNSG32 val;

	base = APB_TIMER_INST_BASE;
	offset = APB_TIMERS_INTSTATUS;
	GA_REG_WORD32_READ(base+offset, &val);
	return val;
}
