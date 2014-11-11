#ifndef CYGONCE_HAL_PLATFORM_SETUP_H
#define CYGONCE_HAL_PLATFORM_SETUP_H
//=============================================================================
//
//      hal_platform_setup.h
//
//      Platform specific support for HAL (assembly code)
//
//=============================================================================
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
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    Patrick Doyle <wpd@delcomsys.com>
// Contributors: Patrick Doyle <wpd@delcomsys.com>
// Date:         2002-12-02
// Purpose:      DB5181 platform specific support routines
// Description: 
// Usage:        #include <cyg/hal/hal_platform_setup.h>
//     Only used by "vectors.S"         
//
//####DESCRIPTIONEND####
//
//=============================================================================

#include <pkgconf/system.h>             // System-wide configuration info
#include CYGBLD_HAL_VARIANT_H           // Variant specific configuration
#include CYGBLD_HAL_PLATFORM_H          // Platform specific configuration
#include CYGHWR_MEMORY_LAYOUT_H
#include <cyg/hal/hal_mmu.h>            // MMU definitions
#include <cyg/hal/orion.h>          // Platform specific hardware definitions


//#if defined(CYG_HAL_STARTUP_ROM) || defined(CYG_HAL_STARTUP_ROMRAM)
#define PLATFORM_SETUP1 _platform_setup1
#define CYGHWR_HAL_ARM_HAS_MMU
#define CYGSEM_HAL_ROM_RESET_USES_JUMP

// We need this here - can't rely on a translation table until MMU has
// been initialized
        .macro RAW_LED_MACRO x
        ldr     r0,=MV_GPP_OUT_DATA_REG
        ldr     r1,[r0]
        bic     r1,r1,#((1<<7)|(1<<6))
        bic		r1,r1,#((1<<15)|(1<<14))
        orr		r1,r1,#(((\x) & 0x0c)<<4)
        orr     r1,r1,#(((\x) & 0x03)<<14)
        str     r1, [r0]
        .endm


        // Configure GPP[6:7] and GPP[14:15] as Output & pull-up turned off
        .macro RAW_LED_PORT_INIT_MACRO
        ldr     r0,=MV_GPP_OUT_ENABLE_REG
        ldr     r1,[r0]
        bic     r1,r1,#((1<<7)|(1<<6))
        bic		r1,r1,#((1<<15)|(1<<14))
        str     r1,[r0]

        RAW_LED_MACRO 0
        .endm

// This macro represents the initial startup code for the platform        
        .macro  _platform_setup1

        RAW_LED_PORT_INIT_MACRO
	
        // Disable and clear caches
        mrc  p15,0,r0,c1,c0,0
        bic  r0,r0,#0x1000              // disable ICache
        bic  r0,r0,#0x000f              // disable DCache, write buffer,
                                        // MMU and alignment faults
        mcr  p15,0,r0,c1,c0,0
        nop
        nop
        mov  r0,#0
        mcr  p15,0,r0,c7,c6,0           // clear data cache
        
        // Set up a stack [for calling C code]
        ldr     r1,=__startup_stack
        ldr     r2,=ORION_SDRAM_PHYS_BASE
        orr     sp,r1,r2

		RAW_LED_MACRO 2
		
        // Create MMU tables        
        bl      hal_mmu_init

		RAW_LED_MACRO 3 
		
        // Enable MMU
#ifdef CYG_HAL_STARTUP_ROMRAM
        ldr     r1,=__exception_handlers
        sub     r1,r2,r1
        add     r2,r9,r1
#endif  
        ldr        r1,=MMU_Control_Init|MMU_Control_M
        mcr        MMU_CP,0,r1,MMU_Control,c0

		RAW_LED_MACRO 4
		
        nop
        nop
        nop
        .endm
//#else // defined(CYG_HAL_STARTUP_ROM) || defined(CYG_HAL_STARTUP_ROMRAM)
//#define PLATFORM_SETUP1
//#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// end of hal_platform_setup.h

// ------------------------------------------------------        
// --------------Static Data Definitions-----------------
// ------------------------------------------------------

#endif // CYGONCE_HAL_PLATFORM_SETUP_H
