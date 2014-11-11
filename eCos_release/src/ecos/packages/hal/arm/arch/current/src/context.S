// #===========================================================================
// #
// #    context.S
// #
// #    ARM context switch code
// #
// #===========================================================================
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
// #===========================================================================
// ######DESCRIPTIONBEGIN####
// #
// # Author(s):    nickg, gthomas
// # Contributors: nickg, gthomas
// # Date:         1998-09-15
// # Purpose:      ARM context switch code
// # Description:  This file contains implementations of the thread context 
// #               switch routines. It also contains the longjmp() and setjmp()
// #               routines.
// #
// #####DESCRIPTIONEND####
// #
// #===========================================================================

#include <pkgconf/hal.h>

#include "arm.inc"

        .text

// ----------------------------------------------------------------------------
//  function declaration macro (start body in ARM mode)
                
#ifdef __thumb__
#define FUNC_START_ARM(_name_, _r_)              \
        .code   16                              ;\
        .thumb_func                             ;\
        .globl _name_                           ;\
_name_:                                         ;\
        ldr     _r_,=_name_ ## _ARM             ;\
        bx      _r_                             ;\
        .code   32                              ;\
_name_ ## _ARM:

#else

#define FUNC_START_ARM(_name_, _r_) \
        .globl _name_; \
_name_: 
        
#endif
        
#define MMX_WR0		 	(0x00)
#define MMX_WR1		 	(0x08)
#define MMX_WR2		 	(0x10)
#define MMX_WR3			(0x18)
#define MMX_WR4		 	(0x20)
#define MMX_WR5		 	(0x28)
#define MMX_WR6		 	(0x30)
#define MMX_WR7		 	(0x38)
#define MMX_WR8		 	(0x40)
#define MMX_WR9		 	(0x48)
#define MMX_WR10		(0x50)
#define MMX_WR11		(0x58)
#define MMX_WR12		(0x60)
#define MMX_WR13		(0x68)
#define MMX_WR14		(0x70)
#define MMX_WR15		(0x78)
#define MMX_WCSSF		(0x80)
#define MMX_WCASF		(0x84)
#define MMX_WCGR0		(0x88)
#define MMX_WCGR1		(0x8C)
#define MMX_WCGR2		(0x90)
#define MMX_WCGR3		(0x94)

// ----------------------------------------------------------------------------
//  hal_thread_switch_context
//  Switch thread contexts
//  R0 = address of sp of next thread to execute
//  R1 = address of sp save location of current thread

// Need to save/restore R4..R12, R13 (sp), R14 (lr)

// Note: this is a little wasteful since r0..r3 don't need to be saved.
// They are saved here though so that the information can match the
// HAL_SavedRegisters
        
FUNC_START_ARM(hal_thread_switch_context, r2)
        mov     ip,sp
	mrc	p15, 0, r2, c15, c1, 0
	tst	r2, #0x3			@ CP0 and CP1 accessible?
	beq	no_iwmmxt_str
	sub	sp, sp, #8
	bic	sp, sp, #7
	wstrd	wr0,  [sp, #-MMX_WR0]
	wstrd	wR1,  [sp, #-MMX_WR1]
	wstrd	wR2,  [sp, #-MMX_WR2]
	wstrd	wR3,  [sp, #-MMX_WR3]
	wstrd	wR4,  [sp, #-MMX_WR4]
	wstrd	wR5,  [sp, #-MMX_WR5]
	wstrd	wR6,  [sp, #-MMX_WR6]
	wstrd	wR7,  [sp, #-MMX_WR7]
	wstrd	wR8,  [sp, #-MMX_WR8]
	wstrd	wR9,  [sp, #-MMX_WR9]
	wstrd	wR10, [sp, #-MMX_WR10]
	wstrd	wR11, [sp, #-MMX_WR11]
	wstrd	wR12, [sp, #-MMX_WR12]
	wstrd	wR13, [sp, #-MMX_WR13]
	wstrd	wR14, [sp, #-MMX_WR14]
	wstrd	wR15, [sp, #-MMX_WR15]
	wstrw	wCSSF, [sp, #-MMX_WCSSF]
	wstrw	wCASF, [sp, #-MMX_WCASF]
	wstrw	wCGR0, [sp, #-MMX_WCGR0]
	wstrw	wCGR1, [sp, #-MMX_WCGR1]
	wstrw	wCGR2, [sp, #-MMX_WCGR2]
	wstrw	wCGR3, [sp, #-MMX_WCGR3]
	mov     sp, ip
no_iwmmxt_str:
        sub     sp,sp,#(ARMREG_SIZE - armreg_lr - 4) // skip svc_sp, svc_lr, vector, cpsr, and pc
        stmfd   sp!,{ip,lr}
        stmfd   sp!,{r0-r10,fp,ip}
        mrs     r2,cpsr
        str     r2,[sp,#armreg_cpsr]
        str     sp,[r1]                 // return new stack pointer
#ifdef __thumb__
        b       hal_thread_load_context_ARM // skip mode switch stuff
#endif

        # Now load the destination thread by dropping through
        # to hal_thread_load_context
        
// ----------------------------------------------------------------------------
//  hal_thread_load_context
//  Load thread context
//  R0 = address of sp of next thread to execute
//  Note that this function is also the second half of
//  hal_thread_switch_context and is simply dropped into from it.
        
FUNC_START_ARM(hal_thread_load_context, r2)
        ldr     fp,[r0]                 // get context to restore
        mrs     r0,cpsr                 // disable IRQ's
        orr     r0,r0,#CPSR_IRQ_DISABLE|CPSR_FIQ_DISABLE
        msr     cpsr,r0
        ldr     r0,[fp,#armreg_cpsr]
        msr     spsr,r0
	mrc	p15, 0, r2, c15, c1, 0
	tst	r2, #0x3			@ CP0 and CP1 accessible?
	beq	no_iwmmxt_ldr
	ldr	sp, [fp, #armreg_sp]
	sub	sp, sp, #8
	bic	sp, sp, #7
	wldrd	wR0,  [sp, #-MMX_WR0]
	wldrd	wR1,  [sp, #-MMX_WR1]
	wldrd	wR2,  [sp, #-MMX_WR2]
	wldrd	wR3,  [sp, #-MMX_WR3]
	wldrd	wR4,  [sp, #-MMX_WR4]
	wldrd	wR5,  [sp, #-MMX_WR5]
	wldrd	wR6,  [sp, #-MMX_WR6]
	wldrd	wR7,  [sp, #-MMX_WR7]
	wldrd	wR8,  [sp, #-MMX_WR8]
	wldrd	wR9,  [sp, #-MMX_WR9]
	wldrd	wR10, [sp, #-MMX_WR10]
	wldrd	wR11, [sp, #-MMX_WR11]
	wldrd	wR12, [sp, #-MMX_WR12]
	wldrd	wR13, [sp, #-MMX_WR13]
	wldrd	wR14, [sp, #-MMX_WR14]
	wldrd	wR15, [sp, #-MMX_WR15]
	wldrw	wCSSF, [sp, #-MMX_WCSSF]
	wldrw	wCASF, [sp, #-MMX_WCASF]
	wldrw	wCGR0, [sp, #-MMX_WCGR0]
	wldrw	wCGR1, [sp, #-MMX_WCGR1]
	wldrw	wCGR2, [sp, #-MMX_WCGR2]
	wldrw	wCGR3, [sp, #-MMX_WCGR3]
no_iwmmxt_ldr:
        ldmfd   fp,{r0-r10,fp,ip,sp,lr}
#ifdef __thumb__
        mrs     r1,spsr                 // r1 is scratch 
                                        // [r0 holds initial thread arg]
        msr     cpsr,r1                 // hopefully no mode switch here!
        bx      lr
#else
        movs    pc,lr                   // also restores saved PSR
#endif

// ----------------------------------------------------------------------------
//  HAL longjmp, setjmp implementations
//  hal_setjmp saves only to callee save registers 4-14
//  and lr into buffer supplied in r0[arg0]

FUNC_START_ARM(hal_setjmp, r2)
        stmea   r0,{r4-r14}
        mov     r0,#0
#ifdef __thumb__
        bx      lr
#else
        mov     pc,lr;          # return
#endif

//  hal_longjmp loads state from r0[arg0] and returns
        
FUNC_START_ARM(hal_longjmp, r2)
        ldmfd   r0,{r4-r14}
        mov     r0,r1;          # return [arg1]
#ifdef __thumb__
        bx      lr
#else
        mov     pc,lr
#endif

// ----------------------------------------------------------------------------
//  end of context.S
