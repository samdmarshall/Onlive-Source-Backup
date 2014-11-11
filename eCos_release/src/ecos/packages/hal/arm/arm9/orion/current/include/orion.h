#ifndef CYGONCE_ORION_H
#define CYGONCE_ORION_H

//=============================================================================
//
//      db5181.h
//
//      Platform specific support (register layout, etc)
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
// Author(s):    michael anburaj <michaelanburaj@hotmail.com>
// Contributors: michael anburaj <michaelanburaj@hotmail.com>
// Date:         2003-08-01
// Purpose:      ORION platform specific support definitions
// Description: 
// Usage:        #include <cyg/hal/orion.h>
//
//####DESCRIPTIONEND####
//
//=============================================================================

#include <pkgconf/hal_arm_arm9_orion.h>

// Memory layout details needed by conversion macro
#define ORION_SDRAM_PHYS_BASE         0x00000000
#define ORION_SDRAM_VIRT_BASE         0x00000000

#define ORION_FLASH_PHYS_BASE         0x00000000
#define ORION_FLASH_VIRT_BASE         0x80000000


// Internal clocks
#define FCLK CYGNUM_HAL_ARM_ORION_CPU_CLOCK
#define HCLK CYGNUM_HAL_ARM_ORION_BUS_CLOCK
#define TCLK CYGNUM_HAL_ARM_ORION_PERIPHERAL_CLOCK

#define MV_REG_BASEADDR		0xf1000000

/*orion main interrupts registers*/
#define MV_IRQ_CAUSE_REG			MV_REG_BASEADDR + 0x20200
#define MV_IRQ_MASK_REG				MV_REG_BASEADDR + 0x20204
#define MV_GPP_IRQ_CAUSE_REG		MV_REG_BASEADDR + 0x10114
#define MV_GPP_IRQ_MASK_REG     	MV_REG_BASEADDR + 0x1011c

/*timers related registers*/
#define MV_TIMERS_CONTROL_REG		MV_REG_BASEADDR + 0x20300
#define MV_TIMER_0_RELOAD_REG		MV_REG_BASEADDR + 0x20310
#define MV_TIMER_0_REG				MV_REG_BASEADDR + 0x20314
#define MV_TIMER_1_RELOAD_REG		MV_REG_BASEADDR + 0x20318
#define MV_TIMER_1_REG				MV_REG_BASEADDR + 0x2031c

/*timers interrupt registers*/
#define BRIDGE_INT_CAUSE_REG		MV_REG_BASEADDR + 0x20110
#define BRIDGE_INT_MASK_REG    		MV_REG_BASEADDR + 0x20114

//gpp settings register
#define MV_GPP_OUT_ENABLE_REG		MV_REG_BASEADDR + 0x10104
#define MV_GPP_OUT_DATA_REG			MV_REG_BASEADDR + 0x10100

//uart 0 related registers
#define MV_UART0_THR 				MV_REG_BASEADDR + 0x12000  //UART 0 Transmission Hold
#define MV_UART0_RBR 				MV_REG_BASEADDR + 0x12000  //UART 0 Receive buffer
#define MV_UART0_DLL 				MV_REG_BASEADDR + 0x12000  //UART 0 Divisor Latch Low	
#define MV_UART0_IER				MV_REG_BASEADDR + 0x12004  //UART 0 Interrupt Enable
#define MV_UART0_DLH				MV_REG_BASEADDR + 0x12004  //UART 0 Divisor Latch High
#define MV_UART0_IIR				MV_REG_BASEADDR + 0x12008  //UART 0 Interrup Identity
#define MV_UART0_FCR   				MV_REG_BASEADDR + 0x12008  //UART 0 FIFO control
#define MV_UART0_LCR 				MV_REG_BASEADDR + 0x1200c  //UART 0 Line control
#define MV_UART0_MCR   				MV_REG_BASEADDR + 0x12010  //UART 0 Modem control
#define MV_UART0_LSR				MV_REG_BASEADDR + 0x12014  //UART 0 Line Status
#define MV_UART0_MSR    			MV_REG_BASEADDR + 0x12018  //UART 0 Modem status
#define MV_UART0_SCR				MV_REG_BASEADDR + 0x1201c  //UART 0 Scratch Pad

//uart 1 related registers
#define MV_UART1_THR 				MV_REG_BASEADDR + 0x12100  //UART 1 Transmission Hold
#define MV_UART1_RBR 				MV_REG_BASEADDR + 0x12100  //UART 1 Receive buffer
#define MV_UART1_DLL 				MV_REG_BASEADDR + 0x12100  //UART 1 Divisor Latch Low	
#define MV_UART1_IER				MV_REG_BASEADDR + 0x12104  //UART 1 Interrupt Enable
#define MV_UART1_DLH				MV_REG_BASEADDR + 0x12104  //UART 1 Divisor Latch High
#define MV_UART1_IIR				MV_REG_BASEADDR + 0x12108  //UART 1 Interrup Identity
#define MV_UART1_FCR   				MV_REG_BASEADDR + 0x12108  //UART 1 FIFO control
#define MV_UART1_LCR 				MV_REG_BASEADDR + 0x1210c  //UART 1 Line control
#define MV_UART1_MCR   				MV_REG_BASEADDR + 0x12110  //UART 1 Modem control
#define MV_UART1_LSR				MV_REG_BASEADDR + 0x12114  //UART 1 Line Status
#define MV_UART1_MSR    			MV_REG_BASEADDR + 0x12118  //UART 1 Modem status
#define MV_UART1_SCR				MV_REG_BASEADDR + 0x1211c  //UART 1 Scratch Pad

//uart related offsets
#define OFFSET_THR 				(0x0)  //UART 0 Transmission Hold
#define OFFSET_RBR 				(0x0)  //UART 0 Receive buffer
#define OFFSET_DLL 				(0x0)  //UART 0 Divisor Latch Low	
#define OFFSET_IER				(0x4)  //UART 0 Interrupt Enable
#define OFFSET_DLH				(0x4)  //UART 0 Divisor Latch High
#define OFFSET_IIR				(0x8)  //UART 0 Interrup Identity
#define OFFSET_FCR   			(0x8)  //UART 0 FIFO control
#define OFFSET_LCR 				(0xc)  //UART 0 Line control
#define OFFSET_MCR   			(0x10)  //UART 0 Modem control
#define OFFSET_LSR				(0x14)  //UART 0 Line Status
#define OFFSET_MSR    			(0x18)  //UART 0 Modem status
#define OFFSET_SCR				(0x1c)  //UART 0 Scratch Pad



#define TIMER_BIT_MASK(x)	(1<<(x+1))

#define MV_VALID_INT_LOW		0x2cd9
#define MV_VALID_INT_HIGH		0xffff
//#define MV_PCI_MASK_REG(pciIf)		(0xf1041910 - ((pciIf) * 0xf1010000))
#define MV_PCI_MASK_ABCD		(BIT24 | BIT25 | BIT26 | BIT27 )

/* Believe it or not, these actually make the code more readable in a few
 * places.
 */
#ifndef BIT_00
#define BIT_00 0x00000001
#define BIT_01 0x00000002
#define BIT_02 0x00000004
#define BIT_03 0x00000008
#define BIT_04 0x00000010
#define BIT_05 0x00000020
#define BIT_06 0x00000040
#define BIT_07 0x00000080
#define BIT_08 0x00000100
#define BIT_09 0x00000200
#define BIT_10 0x00000400
#define BIT_11 0x00000800
#define BIT_12 0x00001000
#define BIT_13 0x00002000
#define BIT_14 0x00004000
#define BIT_15 0x00008000
#define BIT_16 0x00010000
#define BIT_17 0x00020000
#define BIT_18 0x00040000
#define BIT_19 0x00080000
#define BIT_20 0x00100000
#define BIT_21 0x00200000
#define BIT_22 0x00400000
#define BIT_23 0x00800000
#define BIT_24 0x01000000
#define BIT_25 0x02000000
#define BIT_26 0x04000000
#define BIT_27 0x08000000
#define BIT_28 0x10000000
#define BIT_29 0x20000000
#define BIT_30 0x40000000
#define BIT_31 0x80000000
#endif



#endif // CYGONCE_ORION_H
//-----------------------------------------------------------------------------
// end of orion.h
