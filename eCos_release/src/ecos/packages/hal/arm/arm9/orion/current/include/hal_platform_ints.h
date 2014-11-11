#ifndef CYGONCE_HAL_PLATFORM_INTS_H
#define CYGONCE_HAL_PLATFORM_INTS_H
//==========================================================================
//
//      hal_platform_ints.h
//
//      HAL Interrupt and clock support
//
//==========================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    Patrick Doyle <wpd@delcomsys.com>
// Contributors: Patrick Doyle <wpd@delcomsys.com>
// Date:         2002-12-01
// Purpose:      Define Interrupt support
// Description:  The interrupt details for the DB5181 will someday
//               be defined here.
// Usage:
//               #include <cyg/hal/hal_platform_ints.h>
//               ...
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <cyg/hal/orion.h>

// These are main interrupts on the ORION

#define CYGNUM_HAL_INTERRUPT_START      	0
#define CYGNUM_HAL_INTERRUPT_BRIDGE     	CYGNUM_HAL_INTERRUPT_START
#define CYGNUM_HAL_INTERRUPT_H2C			1
#define CYGNUM_HAL_INTERRUPT_C2H			2
#define CYGNUM_HAL_INTERRUPT_UART0      	3
#define CYGNUM_HAL_INTERRUPT_UART1	    	4
#define CYGNUM_HAL_INTERRUPT_TWSI			5    
#define CYGNUM_HAL_INTERRUPT_GPP_0_7	  	6
#define CYGNUM_HAL_INTERRUPT_GPP_8_15   	7
#define CYGNUM_HAL_INTERRUPT_GPP_16_23  	8
#define CYGNUM_HAL_INTERRUPT_GPP_24_31  	9
#define CYGNUM_HAL_INTERRUPT_PEX0_ERR   	10
#define CYGNUM_HAL_INTERRUPT_PEX0_INT   	11


#define CYGNUM_HAL_INTERRUPT_PCI_ERR    	15
#define CYGNUM_HAL_INTERRUPT_USB_BR_ERR 	16
#define CYGNUM_HAL_INTERRUPT_USB_CTRL(x)	((x==0)? 17:12)
#define CYGNUM_HAL_INTERRUPT_GB_RX      	18
#define CYGNUM_HAL_INTERRUPT_GB_TX      	19
#define CYGNUM_HAL_INTERRUPT_GB_MISC    	20
#define CYGNUM_HAL_INTERRUPT_GB_SUM     	21
#define CYGNUM_HAL_INTERRUPT_GB_ERR     	22
#define CYGNUM_HAL_INTERRUPT_IDMA_ERR     	23
#define CYGNUM_HAL_INTERRUPT_IDMA_0  		24
#define CYGNUM_HAL_INTERRUPT_IDMA_1       	25
#define CYGNUM_HAL_INTERRUPT_IDMA_2       	26
#define CYGNUM_HAL_INTERRUPT_IDMA_3       	27
#define CYGNUM_HAL_INTERRUPT_CESA      		28

#define CYGNUM_HAL_INTERRUPT_GPP_START		32
#define CYGNUM_HAL_INTERRUPT_GPP_0 			CYGNUM_HAL_INTERRUPT_GPP_START
#define CYGNUM_HAL_INTERRUPT_GPP_1			33
#define CYGNUM_HAL_INTERRUPT_GPP_2			34
#define CYGNUM_HAL_INTERRUPT_GPP_3			35
#define CYGNUM_HAL_INTERRUPT_GPP_4			36
#define CYGNUM_HAL_INTERRUPT_GPP_5			37
#define CYGNUM_HAL_INTERRUPT_GPP_6			38
#define CYGNUM_HAL_INTERRUPT_GPP_7			39
#define CYGNUM_HAL_INTERRUPT_GPP_8			40
#define CYGNUM_HAL_INTERRUPT_GPP_9			41
#define CYGNUM_HAL_INTERRUPT_GPP_10			42
#define CYGNUM_HAL_INTERRUPT_GPP_11			43
#define CYGNUM_HAL_INTERRUPT_GPP_12			44
#define CYGNUM_HAL_INTERRUPT_GPP_13			45
#define CYGNUM_HAL_INTERRUPT_GPP_14			46
#define CYGNUM_HAL_INTERRUPT_GPP_15			47
#define CYGNUM_HAL_INTERRUPT_GPP_16			48
#define CYGNUM_HAL_INTERRUPT_GPP_17			49
#define CYGNUM_HAL_INTERRUPT_GPP_18			50
#define CYGNUM_HAL_INTERRUPT_GPP_19			51
#define CYGNUM_HAL_INTERRUPT_GPP_20			52
#define CYGNUM_HAL_INTERRUPT_GPP_21			53
#define CYGNUM_HAL_INTERRUPT_GPP_22			54
#define CYGNUM_HAL_INTERRUPT_GPP_23			55
#define CYGNUM_HAL_INTERRUPT_GPP_24			56
#define CYGNUM_HAL_INTERRUPT_GPP_25			57
#define CYGNUM_HAL_INTERRUPT_GPP_26			58
#define CYGNUM_HAL_INTERRUPT_GPP_27			59
#define CYGNUM_HAL_INTERRUPT_GPP_28			60
#define CYGNUM_HAL_INTERRUPT_GPP_29			61
#define CYGNUM_HAL_INTERRUPT_GPP_30			62
#define CYGNUM_HAL_INTERRUPT_GPP_31			63


#define CYGNUM_HAL_INTERRUPT_NONE    		-1

#define CYGNUM_HAL_ISR_MIN            0
#define CYGNUM_HAL_ISR_MAX            CYGNUM_HAL_INTERRUPT_GPP_31
#define CYGNUM_HAL_ISR_COUNT          (CYGNUM_HAL_ISR_MAX-CYGNUM_HAL_ISR_MIN+1)

// The vector used by the Real time clock
#define CYGNUM_HAL_INTERRUPT_RTC      CYGNUM_HAL_INTERRUPT_BRIDGE

//----------------------------------------------------------------------------
// Reset.

// Watchdog is started with a very small delay to Reset immediatly.

#define HAL_PLATFORM_RESET()
/*                                     \
do {                                                             \
    HAL_WRITE_UINT32(WTCON, 0);                                  \
    HAL_WRITE_UINT32(WTDAT, 1);                                  \
    HAL_WRITE_UINT32(WTCON, (1<<0)|(0<<2)|(0<<3)|(1<<5)|(0<<8)); \
} while(0)
*/
// Base of flash
#define HAL_PLATFORM_RESET_ENTRY 0x00000000

#endif // CYGONCE_HAL_PLATFORM_INTS_H
