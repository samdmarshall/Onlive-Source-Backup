#ifndef __APB_UART_DRV_H__
#define __APB_UART_DRV_H__
#include "com_type.h"
#include "galois_io.h"
#include "apb_uart.h"
/*******************************************************************************
* Function:    APB_UART_init
*
* Description: initialize UART
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*			   tclk-- UART source clock
*			   baud-- UART baud rate
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void APB_UART_init(UNSG32 channel, UNSG32 tclk, UNSG32 baud);
/*******************************************************************************
* Function:    APB_UART_putc
*
* Description: initialize UART
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*			   c-- output character
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void APB_UART_putc(UNSG32 channel, SIGN8 ch);
/*******************************************************************************
* Function: APB_UART_tstc
*
* Description: Test if there's input character from UART.
*
* Inputs: channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*
* Outputs: none
*
* Return: 0-- there's no char input
* 		  1-- there's char input 
*******************************************************************************/
UNSG8 APB_UART_tstc(UNSG32 channel);
/*******************************************************************************
* Function:    APB_UART_getc_nonblock
*
* Description: non blocking mode read charactor from UART
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*			   c-- output character
* Outputs:     none
*
* Return:      0-- didn't get any character
*			   1-- get one character
*******************************************************************************/
UNSG8 APB_UART_getc_nonblock(UNSG32 channel, UNSG8 *ch);
/*******************************************************************************
* Function:    APB_UART_getc
*
* Description: blocking mode read charactor from UART. Wait until get something
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
* Outputs:     none
*
* Return:      return UIN8 value.
*******************************************************************************/
UNSG8 APB_UART_getc(UNSG32 channel);
/*******************************************************************************
* Function:    APB_UART_write
*
* Description: Write the buffer into UART.
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*			   buf-- pointer point to the string what will be written to UART
*			   len-- length of the string
* Outputs:     none
*
* Return:      none.
*******************************************************************************/
void APB_UART_write(UNSG32 channel, SIGN8 *buf);
/*******************************************************************************
* Function:    APB_UART_read
*
* Description: read the string from UART.
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*			   buf-- pointer point to the string what will be written to UART
*			   len-- length of the string
* Outputs:     none
*
* Return:      none.
*******************************************************************************/
void APB_UART_read(UNSG32 channel, SIGN8 *buf, UNSG32 len);
/*******************************************************************************
* Function:    APB_UART_printf
*
* Description: printf sytle output.print the output to UART
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*			   szFormat-- print format
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void APB_UART_printf(UNSG32 channel, UNSG8* szFormat, ...);
#endif //__APB_UART_DRV_H__
