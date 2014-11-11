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
void TIMERx_SetLoadCount(UNSG32 timer_N, UNSG32 count_val);
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
void TIMERx_SetControlReg(UNSG32 timer_N, UNSG32 mask, UNSG32 enable, UNSG32 mode);
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
void TIMERx_SetModeAndMask(UNSG32 timer_N, UNSG32 mask, UNSG32 mode);
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
void TIMERx_SetMask(UNSG32 timer_N, UNSG32 mask);
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
void TIMERx_SetIntEn(UNSG32 timer_N, UNSG32 enable);
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
UNSG32 TIMERx_GetCurrentValue(UNSG32 timer_N);
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
void TIMERx_ClearInt(UNSG32 timer_N);
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
UNSG32 TIMERx_GetIntStatus (UNSG32 timer_N);
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
void TIMERS_ClearAllInt(void);
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
UNSG32 TIMERS_GetAllIntStatus (void);
