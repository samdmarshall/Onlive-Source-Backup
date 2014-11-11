/*
 * This function is used to output character to UART0
 */
#define GALOIS_DEBUG 1

#ifdef CONFIG_MV88DE3010_BERLIN

.macro galois_putc rega,regb,x
.endm

.macro gdebug_putc x
.endm

.macro gdebug_putc_end x
.endm

.macro gdebug_data x
.endm

.macro gdebug_32bits_r9
.endm

#else /* BERLIN */

.macro galois_putc rega,regb,x
	ldr \rega,=(0xF7F3100C)	@LCR
	ldr \regb,[\rega]
	bic \regb,\regb,#0x00000080
	str \regb,[\rega]
1:
	ldr \rega,=(0xF7F31014)	@LSR
	ldr \regb,[\rega]
	tst \regb,#0x00000020		@ shouldn't use "and \regb,#0x20"
	beq 1b

	ldr \rega,=(0xF7F31000)	@THR
	mov \regb,#(\x)
	str \regb,[\rega]
.endm

.macro gdebug_putc x
	ldr r9,=(0xF7F3100C)	@LCR
	ldr r10,[r9]
	bic r10,r10,#0x00000080
	str r10,[r9]
1:
	ldr r9,=(0xF7F31014)	@LSR
	ldr r10,[r9]
	tst r10,#0x00000020		@ shouldn't use "and r10,#0x20"
	beq 1b

	ldr r9,=(0xF7F31000)	@THR
	mov r10,#(\x)
	str r10,[r9]
.endm

.macro gdebug_putc_end x
	gdebug_putc \x
	gdebug_putc 10
	gdebug_putc 13
.endm

.macro gdebug_data x
	ldr r9,=(0xF7F3100C)	@LCR
	ldr r10,[r9]
	bic r10,r10,#0x00000080
	str r10,[r9]
1:
	ldr r9,=(0xF7F31014)	@LSR
	ldr r10,[r9]
	tst r10,#0x00000020		@ shouldn't use "and r10,#0x20"
	beq 1b

	ldr r9,=(0xF7F31000)	@THR
	mov r10,\x
	add r10,r10,#48			@ 48+x is ascii code.
	str r10,[r9]
.endm

@ should save original r9
@ r0,r7,r8,r9,r10 is corrupted.
.macro gdebug_32bits_r9
	mov r0,#8
	mov r7,r9
2:
	mov r8,#0
	and r8,r7,#0xF0000000
	mov r8,r8,lsr #28
	gdebug_data r8
	mov r7,r7,lsl #4
	subs r0,r0,#1
	bgt 2b
.endm

#endif /* BERLIN */
