#ifndef __GALOIS_UART_H__
#define __GALOIS_UART_H__


#include <pkgconf/hal_arm_arm9_galois.h>
#include "apb_uart.h"

// Memory layout details needed by conversion macro


// Internal clocks
#define FCLK CYGNUM_HAL_ARM_GALOIS_CPU_CLOCK
#define HCLK CYGNUM_HAL_ARM_GALOIS_BUS_CLOCK
#define TCLK CYGNUM_HAL_ARM_GALOIS_PERIPHERAL_CLOCK

#define MV_UART0_BASE			APB_UART_INST0_BASE
#define MV_UART1_BASE			APB_UART_INST1_BASE
//uart 0 related registers
#define MV_UART0_THR 				MV_UART0_BASE + APB_UART_THR  //UART 0 Transmission Hold
#define MV_UART0_RBR 				MV_UART0_BASE + APB_UART_RBR  //UART 0 Receive buffer
#define MV_UART0_DLL 				MV_UART0_BASE + APB_UART_DLL  //UART 0 Divisor Latch Low	
#define MV_UART0_IER				MV_UART0_BASE + APB_UART_IER  //UART 0 Interrupt Enable
#define MV_UART0_DLH				MV_UART0_BASE + APB_UART_DLH  //UART 0 Divisor Latch High
#define MV_UART0_IIR				MV_UART0_BASE + APB_UART_IIR  //UART 0 Interrup Identity
#define MV_UART0_FCR   				MV_UART0_BASE + APB_UART_FCR  //UART 0 FIFO control
#define MV_UART0_LCR 				MV_UART0_BASE + APB_UART_LCR  //UART 0 Line control
#define MV_UART0_MCR   				MV_UART0_BASE + APB_UART_MCR  //UART 0 Modem control
#define MV_UART0_LSR				MV_UART0_BASE + APB_UART_LSR  //UART 0 Line Status
#define MV_UART0_MSR    			MV_UART0_BASE + APB_UART_MSR  //UART 0 Modem status
#define MV_UART0_SCR				MV_UART0_BASE + APB_UART_SCR  //UART 0 Scratch Pad

//uart 1 related registers
#define MV_UART1_THR 				MV_UART1_BASE + APB_UART_THR  //UART 1 Transmission Hold
#define MV_UART1_RBR 				MV_UART1_BASE + APB_UART_RBR  //UART 1 Receive buffer
#define MV_UART1_DLL 				MV_UART1_BASE + APB_UART_DLL  //UART 1 Divisor Latch Low	
#define MV_UART1_IER				MV_UART1_BASE + APB_UART_IER  //UART 1 Interrupt Enable
#define MV_UART1_DLH				MV_UART1_BASE + APB_UART_DLH  //UART 1 Divisor Latch High
#define MV_UART1_IIR				MV_UART1_BASE + APB_UART_IIR  //UART 1 Interrup Identity
#define MV_UART1_FCR   				MV_UART1_BASE + APB_UART_FCR  //UART 1 FIFO control
#define MV_UART1_LCR 				MV_UART1_BASE + APB_UART_LCR  //UART 1 Line control
#define MV_UART1_MCR   				MV_UART1_BASE + APB_UART_MCR  //UART 1 Modem control
#define MV_UART1_LSR				MV_UART1_BASE + APB_UART_LSR  //UART 1 Line Status
#define MV_UART1_MSR    			MV_UART1_BASE + APB_UART_MSR  //UART 1 Modem status
#define MV_UART1_SCR				MV_UART1_BASE + APB_UART_SCR  //UART 1 Scratch Pad

//uart related offsets
#define OFFSET_THR 				APB_UART_THR  //UART 0 Transmission Hold
#define OFFSET_RBR 				APB_UART_RBR  //UART 0 Receive buffer
#define OFFSET_DLL 				APB_UART_DLL  //UART 0 Divisor Latch Low	
#define OFFSET_IER				APB_UART_IER  //UART 0 Interrupt Enable
#define OFFSET_DLH				APB_UART_DLH  //UART 0 Divisor Latch High
#define OFFSET_IIR				APB_UART_IIR  //UART 0 Interrup Identity
#define OFFSET_FCR   			APB_UART_FCR  //UART 0 FIFO control
#define OFFSET_LCR 				APB_UART_LCR  //UART 0 Line control
#define OFFSET_MCR   			APB_UART_MCR  //UART 0 Modem control
#define OFFSET_LSR				APB_UART_LSR  //UART 0 Line Status
#define OFFSET_MSR    			APB_UART_MSR  //UART 0 Modem status
#define OFFSET_SCR				APB_UART_SCR  //UART 0 Scratch Pad




#endif // __GALOIS_UART_H__
