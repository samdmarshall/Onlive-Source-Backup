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

//#include <cyg/hal/galois.h>
// These are main interrupts on the GALOIS

#define CYGNUM_HAL_INTERRUPT_START		0
#define CYGNUM_HAL_INTERRUPT_END		0x31


#define CYGNUM_HAL_INTERRUPT_ALL     		(0x0)
#define CYGNUM_HAL_INTERRUPT_RTC     		(0x1)
#define CYGNUM_HAL_INTERRUPT_HW_APP     	(0x2)

#define CYGNUM_HAL_INTERRUPT_NONE    		-1

#define CYGNUM_HAL_ISR_MIN            CYGNUM_HAL_INTERRUPT_START

#define CYGNUM_HAL_ISR_MAX            CYGNUM_HAL_INTERRUPT_END
#define CYGNUM_HAL_ISR_COUNT          (CYGNUM_HAL_ISR_MAX-CYGNUM_HAL_ISR_MIN+1)


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
