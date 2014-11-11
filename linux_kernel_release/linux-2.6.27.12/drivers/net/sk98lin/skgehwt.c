/******************************************************************************
 *
 * Name:	skgehwt.c
 * Project:	Gigabit Ethernet Adapters, Event Scheduler Module
 * Version:	$Revision: 2.4 $
 * Date:	$Date: 2007/07/31 13:22:14 $
 * Purpose:	Hardware Timer
 *
 ******************************************************************************/

/******************************************************************************
 *
 *	LICENSE:
 *	(C)Copyright Marvell.
 *	
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *	
 *	The information in this file is provided "AS IS" without warranty.
 *	/LICENSE
 *
 ******************************************************************************/

/*
 *	Event queue and dispatcher
 */
#if (defined(DEBUG) || ((!defined(LINT)) && (!defined(SK_SLIM))))
static const char SysKonnectFileId[] =
	"@(#) $Id: skgehwt.c,v 2.4 2007/07/31 13:22:14 rschmidt Exp $ (C) Marvell.";
#endif

#include "h/skdrv1st.h"		/* Driver Specific Definitions */
#include "h/skdrv2nd.h"		/* Adapter Control- and Driver specific Def. */

#ifdef __C2MAN__
/*
 *   Hardware Timer function queue management.
 */
intro()
{}
#endif

/*
 * Prototypes of local functions.
 */
#define SK_HWT_MAX	65000UL * 160		/* ca. 10 sec. */

/* correction factor (62.5/100 = 5/8) */
#define SK_HWT_FAC	(5 * (SK_U32)pAC->GIni.GIHstClkFact / 8)

/*
 * Initialize hardware timer.
 *
 * Must be called during init level 1.
 */
void SkHwtInit(
SK_AC	*pAC,	/* Adapter Context */
SK_IOC	IoC)	/* I/O Context */
{
	pAC->Hwt.TStart = 0 ;
	pAC->Hwt.TStop = 0;
	pAC->Hwt.TActive = SK_FALSE;

	SkHwtStop(pAC, IoC);
}

/*
 *
 * Start hardware timer (clock ticks are HW-dependent).
 *
 */
void SkHwtStart(
SK_AC	*pAC,	/* Adapter Context */
SK_IOC	IoC,	/* I/O Context */
SK_U32	Time)	/* Time in usec to load the timer */
{
	if (Time > SK_HWT_MAX) {
		Time = SK_HWT_MAX;
	}

	pAC->Hwt.TStart = Time;
	pAC->Hwt.TStop = 0L;

	if (!Time) {
		Time = 1L;
	}

	SK_OUT32(IoC, B2_TI_INI, Time * SK_HWT_FAC);

	SK_OUT16(IoC, B2_TI_CTRL, TIM_START);	/* Start timer */

	pAC->Hwt.TActive = SK_TRUE;
}

/*
 * Stop hardware timer.
 * and clear the timer IRQ
 */
void SkHwtStop(
SK_AC	*pAC,	/* Adapter Context */
SK_IOC	IoC)	/* I/O Context */
{
	SK_OUT16(IoC, B2_TI_CTRL, TIM_STOP);

	SK_OUT16(IoC, B2_TI_CTRL, TIM_CLR_IRQ);

	pAC->Hwt.TActive = SK_FALSE;
}

/*
 *	Stop hardware timer and read time elapsed since last start.
 *
 * returns
 *	The elapsed time since last start in units of 1 us.
 *
 */
SK_U32 SkHwtRead(
SK_AC	*pAC,	/* Adapter Context */
SK_IOC	IoC)	/* I/O Context */
{
	SK_U32	TRead;
	SK_U32	IStatus;
	SK_U32	TimerInt;

	TimerInt = CHIP_ID_YUKON_2(pAC) ? Y2_IS_TIMINT : IS_TIMINT;

	if (pAC->Hwt.TActive) {

		SkHwtStop(pAC, IoC);

		SK_IN32(IoC, B2_TI_VAL, &TRead);

		TRead /= SK_HWT_FAC;

		SK_IN32(IoC, B0_ISRC, &IStatus);

		/* Check if timer expired (or wrapped around) */
		if ((TRead > pAC->Hwt.TStart) || ((IStatus & TimerInt) != 0)) {

			SkHwtStop(pAC, IoC);

			pAC->Hwt.TStop = pAC->Hwt.TStart;
		}
		else {

			pAC->Hwt.TStop = pAC->Hwt.TStart - TRead;
		}
	}
	return(pAC->Hwt.TStop);
}

/*
 * interrupt source= timer
 */
void SkHwtIsr(
SK_AC	*pAC,	/* Adapter Context */
SK_IOC	IoC)	/* I/O Context */
{
	SkHwtStop(pAC, IoC);

	pAC->Hwt.TStop = pAC->Hwt.TStart;

	SkTimerDone(pAC, IoC);
}

/* End of file */
