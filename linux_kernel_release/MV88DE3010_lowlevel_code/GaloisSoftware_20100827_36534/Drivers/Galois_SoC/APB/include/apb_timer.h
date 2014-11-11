#ifndef __APB_TIMER_H__
#define __APB_TIMER_H__
#define APD_TIMER_1_LOADCOUNT		0x00
#define APB_TIMER_1_CURRENTVALUE	0x04
#define APB_TIMER_1_CONTROLREG		0x08
#define APB_TIMER_1_EOI				0x0C
#define APB_TIMER_1_INTSTATUS		0x10

#define APD_TIMER_REG_RANGE			0x14
/**********************************************************************************************
 * the offset of APB_TIMER_N_xxxx  is derived from APD_TIMER_REG_RANGE*(N-1) + APD_TIMER_xxx
 **********************************************************************************************/

#define APB_TIMERS_INTSTATUS		0xA0
#define APB_TIMERS_EOI				0xA4
#define APB_TIMERS_RAW_INTSTSTUS	0xA8
#define APB_TIMERS_COMP_VERSION		0xAC


#define bTimer_Enable			    1 /*bit*/
#define LSbTimer_Enable				0
#define bTimer_Mode					1
#define LSbTimer_Mode				1
#define bTimer_Interrupt_Mask		1
#define LSbTimer_Interrupt_Mask		2

#define TIMER_FREE_RUN_MODE			0
#define TIMER_USR_DEFINED_MODE		1
#endif // __APB_TIMER_H__

